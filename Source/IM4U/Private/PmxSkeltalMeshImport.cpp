// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#include "PmxFactory.h"
#include "Factory/VmdFactory.h"
#include "IM4UPrivatePCH.h"
#include "MeshDescription.h"
#include "MmdBoneNameUtils.h"
#if WITH_EDITOR
#include "SkinnedAssetCompiler.h"
#endif

#define LOCTEXT_NAMESPACE "PMXSkeltalMeshImpoter"
extern TMap<FName, FName> NameMap;
void initMmdNameMap();
////////////////////////////////////////////////////////////////////////////////////////////////
// FMorphMeshRawSource is removed after version 4.16. So added for only this plugin here.
// Converts a mesh to raw vertex data used to generate a morph target mesh

/* compare based on base mesh source vertex indices */
struct FCompareMorphTargetDeltas
{
	FORCEINLINE bool operator()(const FMorphTargetDelta& A, const FMorphTargetDelta& B) const
	{
		return ((int32) A.SourceIdx - (int32)B.SourceIdx) < 0 ? true : false;
	}
};

class FMorphMeshRawSource
{
public:
	struct FMorphMeshVertexRaw
	{
		FVector3f			Position;
		// Tangent, U-direction
		FVector3f			TangentX;
		// Binormal, V-direction
		FVector3f			TangentY;
		// Normal
		FVector3f			TangentZ;
	};

	/* vertex data used for comparisons */
	TArray<FMorphMeshVertexRaw> Vertices;

	/* index buffer used for comparison */
	TArray<uint32> Indices;

	/* indices to original imported wedge points */
	TArray<uint32> WedgePointIndices;

	/* Constructor (default) */
	FMorphMeshRawSource() {}
	FMorphMeshRawSource(USkeletalMesh* SrcMesh, USkeletalMesh* SkeletalMesh, int32 LODIndex = 0);
	FMorphMeshRawSource(FSkeletalMeshLODModel& LODModel);

	static void CalculateMorphTargetLODModel(const FMorphMeshRawSource& BaseSource, const FMorphMeshRawSource& TargetSource, FMorphTargetLODModel& MorphModel);
private:
	void Initialize(FSkeletalMeshLODModel& LODModel);
};

/*
Constructor.
Converts a skeletal mesh to raw vertex data
needed for creating a morph target mesh

@param	SrcMesh - source skeletal mesh to convert
@param	LODIndex - level of detail to use for the geometry
*/
FMorphMeshRawSource::FMorphMeshRawSource(USkeletalMesh* SrcMesh, USkeletalMesh* SkeletalMesh, int32 LODIndex)
{
	check(SrcMesh);
	check(SrcMesh->GetImportedModel());
	check(SrcMesh->GetImportedModel()->LODModels.IsValidIndex(LODIndex));

	// get the mesh data for the given lod
	FSkeletalMeshLODModel& LODModel = SrcMesh->GetImportedModel()->LODModels[LODIndex];

	Initialize(LODModel);
}

FMorphMeshRawSource::FMorphMeshRawSource(FSkeletalMeshLODModel& LODModel)
{
	Initialize(LODModel);
}

void FMorphMeshRawSource::Initialize(FSkeletalMeshLODModel& LODModel)
{
	// iterate over the chunks for the skeletal mesh
	for (int32 SectionIdx = 0; SectionIdx < LODModel.Sections.Num(); SectionIdx++)
	{
		const FSkelMeshSection& Section = LODModel.Sections[SectionIdx];
		for (int32 VertexIdx = 0; VertexIdx < Section.SoftVertices.Num(); VertexIdx++)
		{
			const FSoftSkinVertex& SourceVertex = Section.SoftVertices[VertexIdx];
			FMorphMeshVertexRaw RawVertex =
			{
				SourceVertex.Position,
				SourceVertex.TangentX,
				SourceVertex.TangentY,
				SourceVertex.TangentZ
			};
			Vertices.Add(RawVertex);
		}
	}

	// Copy the indices manually, since the LODModel's index buffer may have a different alignment.
	Indices.Empty(LODModel.IndexBuffer.Num());
	for (int32 Index = 0; Index < LODModel.IndexBuffer.Num(); Index++)
	{
		Indices.Add(LODModel.IndexBuffer[Index]);
	}

	// copy the wedge point indices
	int idxNum = LODModel.GetRawPointIndices().Num();
	if (idxNum)
	{
		WedgePointIndices.Empty(idxNum);
		WedgePointIndices.AddUninitialized(idxNum);
		FMemory::Memcpy(WedgePointIndices.GetData(), LODModel.GetRawPointIndices().GetData(), LODModel.GetRawPointIndices().Num()* LODModel.GetRawPointIndices().GetTypeSize());
		//LODModel.RawPointIndices.Unlock();
	}
}

void FMorphMeshRawSource::CalculateMorphTargetLODModel(const FMorphMeshRawSource& BaseSource, const FMorphMeshRawSource& TargetSource, FMorphTargetLODModel& MorphModel)
{
	// set the original number of vertices
	MorphModel.NumBaseMeshVerts = BaseSource.Vertices.Num();

	// empty morph mesh vertices first
	MorphModel.Vertices.Empty();

	// array to mark processed base vertices
	TArray<bool> WasProcessed;
	WasProcessed.Empty(BaseSource.Vertices.Num());
	WasProcessed.AddZeroed(BaseSource.Vertices.Num());

	TMap<uint32, uint32> WedgePointToVertexIndexMap;
	// Build a mapping of wedge point indices to vertex indices for fast lookup later.
	for (int32 Idx = 0; Idx < TargetSource.WedgePointIndices.Num(); Idx++)
	{
		WedgePointToVertexIndexMap.Add(TargetSource.WedgePointIndices[Idx], Idx);
	}

	// iterate over all the base mesh indices
	for (int32 Idx = 0; Idx < BaseSource.Indices.Num(); Idx++)
	{
		uint32 BaseVertIdx = BaseSource.Indices[Idx];
		if (!BaseSource.Vertices.IsValidIndex(static_cast<int32>(BaseVertIdx)))
		{
			continue;
		}

		// check for duplicate processing
		if (!WasProcessed[BaseVertIdx])
		{
			// mark this base vertex as already processed
			WasProcessed[BaseVertIdx] = true;

			// get base mesh vertex using its index buffer
			const FMorphMeshVertexRaw& VBase = BaseSource.Vertices[BaseVertIdx];

			// clothing can add extra verts, and we won't have source point, so we ignore those
			if (BaseSource.WedgePointIndices.IsValidIndex(BaseVertIdx))
			{
				// get the base mesh's original wedge point index
				uint32 BasePointIdx = BaseSource.WedgePointIndices[BaseVertIdx];

				// find the matching target vertex by searching for one
				// that has the same wedge point index
				uint32* TargetVertIdx = WedgePointToVertexIndexMap.Find(BasePointIdx);

				// only add the vertex if the source point was found
				if (TargetVertIdx != nullptr)
				{
					// get target mesh vertex using its index buffer
					if (!TargetSource.Vertices.IsValidIndex(static_cast<int32>(*TargetVertIdx)))
					{
						continue;
					}
					const FMorphMeshVertexRaw& VTarget = TargetSource.Vertices[*TargetVertIdx];

					// change in position from base to target
					FVector3f PositionDelta(VTarget.Position - VBase.Position);
					FVector3f NormalDeltaZ(VTarget.TangentZ - VBase.TangentZ);

					// check if position actually changed much
					if (PositionDelta.SizeSquared() > FMath::Square(THRESH_POINTS_ARE_NEAR) || 
						/* since we can't get imported morphtarget normal from FBX
						we can't compare normal unless it's calculated
						this is special flag to ignore normal diff*/
						// (true && NormalDeltaZ.SizeSquared() > 0.01f))
						(NormalDeltaZ.SizeSquared() > 0.01f)) //5.8
					{
						// create a new entry
						FMorphTargetDelta NewVertex;
						// position delta
						NewVertex.PositionDelta = PositionDelta;
						// normal delta
						NewVertex.TangentZDelta = NormalDeltaZ;
						// index of base mesh vert this entry is to modify
						NewVertex.SourceIdx = BaseVertIdx;

						// add it to the list of changed verts
						MorphModel.Vertices.Add(NewVertex);
					}
				}
			}
		}
	}

	// sort the array of vertices for this morph target based on the base mesh indices
	// that each vertex is associated with. This allows us to sequentially traverse the list
	// when applying the morph blends to each vertex.
	MorphModel.Vertices.Sort(FCompareMorphTargetDeltas());

	// remove array slack
	MorphModel.Vertices.Shrink();
}

// UPmxFactory

bool UPmxFactory::ImportBone(
	//TArray<FbxNode*>& NodeArray,//5.7
	MMD4UE4::PmxMeshInfo* PmxMeshInfo,
	FSkeletalMeshImportData& ImportData,
	//UFbxSkeletalMeshImportData* TemplateData,
	//TArray<FbxNode*> &SortedLinks,
	bool& bOutDiffPose,
	bool bDisableMissingBindPoseWarning,
	bool& bUseTime0AsRefPose
)

{
	if (PmxMeshInfo == nullptr)
	{
return false;
	}

	//bool GlobalLinkFoundFlag; //5.8
	//bool bAnyLinksNotInBindPose = false; //5.8
	FString LinksWithoutBindPoses;
	int32 NumberOfRoot = 0;
	initMmdNameMap();

	for (int LinkIndex = 0; LinkIndex < PmxMeshInfo->boneList.Num(); LinkIndex++)
	{
		// Add a bone for each FBX Link
		ImportData.RefBonesBinary.Add(SkeletalMeshImportData::FBone());
		//Link = SortedLinks[LinkIndex];
		// get the link parent and children
		int32 ParentIndex = INDEX_NONE; // base value for root if no parent found
		/*FbxNode* LinkParent = Link->GetParent();*/
		int32 LinkParent = PmxMeshInfo->boneList[LinkIndex].ParentBoneIndex;
		if (LinkIndex)
		{
			for (int32 ll = 0; ll < LinkIndex; ++ll) // <LinkIndex because parent is guaranteed to be before child in sortedLink
			{
				/*FbxNode* Otherlink = SortedLinks[ll];*/
				if (ll == LinkParent)
				{
					ParentIndex = ll;
					break;
				}
			}
		}
		//int32 LinkParent = PmxMeshInfo->boneList[LinkIndex].ParentBoneIndex;//5.7
		if (LinkParent >= 0 && LinkParent < PmxMeshInfo->boneList.Num())
		{
				ParentIndex = LinkParent;
		}
		// see how many root this has
		// if more than 
		if (ParentIndex == INDEX_NONE)
		{
			++NumberOfRoot;
			/*int32 RootIdx = -1;
			RootIdx = LinkIndex;*/ //5.8
			if (NumberOfRoot > 1)
			{
				AddTokenizedErrorMessage(
					FTokenizedMessage::Create(
						EMessageSeverity::Error,
						LOCTEXT("MultipleRootsFound", "Multiple roots are found in the bone hierarchy. We only support single root bone.")),
					FFbxErrors::SkeletalMesh_MultipleRoots
				);
				return false;
			}
		}

		// GlobalLinkFoundFlag = false; //5.8

		//ImportData.RefBonesBinary.AddZeroed();
		// set bone
		SkeletalMeshImportData::FBone& Bone = ImportData.RefBonesBinary[LinkIndex];
		FString BoneName;

		/*const char* LinkName = Link->GetName();
		BoneName = ANSI_TO_TCHAR(MakeName(LinkName));*/

		BoneName = PmxMeshInfo->boneList[LinkIndex].Name;
#if USE_ENG_NAME
		const FName* pn = (FName*)NameMap.FindKey(FName(PmxMeshInfo->boneList[LinkIndex].Name));
		if (pn)
			BoneName = pn->ToString();//For MMD
#endif
		const FString OriginalBoneName = BoneName;
		BoneName = IM4U::MakeEngineSafeMmdBoneName(BoneName);
		if (BoneName != OriginalBoneName)
		{
			UE_LOG(LogMMD4UE4_PMXFactory, Log,
				TEXT("Renamed animation-unsafe PMX bone '%s' to '%s'."),
				*OriginalBoneName,
				*BoneName);
		}
		Bone.Name = BoneName;

		SkeletalMeshImportData::FJointPos& JointMatrix = Bone.BonePos;

		JointMatrix.Length = 1.;
		JointMatrix.XSize = 100.;
		JointMatrix.YSize = 100.;
		JointMatrix.ZSize = 100.;

		// get the link parent and children
		Bone.ParentIndex = ParentIndex;
		Bone.NumChildren = 0;

		//For MMD
		FQuat4f  ftr = FQuat4f(0, 0, 0, 1.0);
		for (int32 ChildIndex = 0; ChildIndex < PmxMeshInfo->boneList.Num(); ChildIndex++)
		{
			if (LinkIndex == PmxMeshInfo->boneList[ChildIndex].ParentBoneIndex)
			{
				Bone.NumChildren++;
				/*int childIdx = -1;
				childIdx = ChildIndex;*/ //5.8
			}
		}

		//test MMD , not rot asix and LocalAsix
		FVector3f TransTemp = PmxMeshInfo->boneList[LinkIndex].Position;
		bool hasParent = ParentIndex != INDEX_NONE;
		if (hasParent)
		{
			if (ParentIndex < 0 || ParentIndex >= PmxMeshInfo->boneList.Num())
			{
			    UE_LOG(LogMMD4UE4_PMXFactory, Error, TEXT("ParentIndex %d out of range for bone %d"), ParentIndex, LinkIndex);
			    continue; // 跳过此骨骼
			}
			FTransform3f it = PmxMeshInfo->boneList[ParentIndex].absTF.Inverse();
			TransTemp = it.TransformPosition(
				PmxMeshInfo->boneList[LinkIndex].Position
			)
				- it.TransformPosition(PmxMeshInfo->boneList[ParentIndex].Position);
		}
		for (int32 ControlPointIndex = 0; ControlPointIndex < PmxMeshInfo->vertexList.Num(); ++ControlPointIndex)
		{
#define MAX_BONE_INFLUENCE 4
			for (int32 multiBone = 0; multiBone < MAX_BONE_INFLUENCE; ++multiBone)
			{
				int32 BoneIdx = PmxMeshInfo->vertexList[ControlPointIndex].BoneIndex[multiBone];
				// ... 骨骼索引检查与赋值 ...
				// FillSkelMeshImporterFromFbx权重索引检查
				if (BoneIdx < 0 || BoneIdx >= PmxMeshInfo->boneList.Num())
				{
					UE_LOG(LogMMD4UE4_PMXFactory, Error, TEXT("BoneIndex %d out of range for vertex %d"), BoneIdx, ControlPointIndex);
					BoneIdx = 0; // 或者 continue;
				}
				//ImportData.Influences.Last().BoneIndex = BoneIdx;//5.7 无法读写内存
				ftr = FRotator3f(0, 0, 0).Quaternion();
			}
		}
		//TransTemp *= 10;
		FTransform3f ft; ft.SetRotation(ftr);
		FVector3f fv1 = ft.TransformPosition(TransTemp);
		FTransform3f ft2 = ft.Inverse();
		FVector3f fv2 = ft2.TransformPosition(TransTemp);
		if (BoneName.Contains(L"L"))
			UE_LOG(LogMMD4UE4_PMXFactory, Warning, TEXT("%s,%d  fv0:%f,%f,%f   fv1:%f,%f,%f   fv2:%f,%f,%f"), *BoneName, hasParent, TransTemp.X, TransTemp.Y, TransTemp.Z, fv1.X, fv1.Y, fv1.Z, fv2.X, fv2.Y, fv2.Z);
		JointMatrix.Transform.SetTranslation(TransTemp);
		JointMatrix.Transform.SetRotation(ftr);// (FQuat(0, 0, 0, 1.0));
		JointMatrix.Transform.SetScale3D(FVector3f(1));
		if (hasParent)
			PmxMeshInfo->boneList[LinkIndex].absTF = PmxMeshInfo->boneList[ParentIndex].absTF * JointMatrix.Transform;
		else 			PmxMeshInfo->boneList[LinkIndex].absTF = JointMatrix.Transform;
	}
	return true;
}

bool UPmxFactory::FillSkelMeshImporterFromFbx(
	FSkeletalMeshImportData& ImportData,
	MMD4UE4::PmxMeshInfo*& PmxMeshInfo,
	UObject* InParent
	/*FbxMesh*& Mesh,
	FbxSkin* Skin,
	FbxShape* FbxShape,
	TArray<FbxNode*> &SortedLinks,
	const TArray<FbxSurfaceMaterial*>& FbxMaterials*/
)

{
#if 1 //test Material Textuere //不能删
	TArray<UMaterialInterface*> Materials;//不能换位置
	TArray<UTexture*> textureAssetList;
	if (ImportUI->bImportTextures)
	{
		for (int k = 0; k < PmxMeshInfo->textureList.Num(); ++k)
		{
			pmxMaterialImportHelper.AssetsCreateTexture(
				InParent,
				/*Flags,
				Warn,*/
				FPaths::GetPath(GetCurrentFilename()),
				PmxMeshInfo->textureList[k].TexturePath,
				textureAssetList
			);

		}
	}
	//UE_LOG(LogCategoryPMXFactory, Warning, TEXT("PMX Import Texture Extecd Complete."));

	TArray<FString> UVSets;
	if (ImportUI->bImportMaterials)
	{
		for (int k = 0; k < PmxMeshInfo->materialList.Num(); ++k)
		{
			pmxMaterialImportHelper.CreateUnrealMaterial(
				PmxMeshInfo->modelNameJP,
				//InParent,
				PmxMeshInfo->materialList[k],
				ImportUI->bCreateMaterialInstMode,
				ImportUI->bUnlitMaterials,
				Materials,
				textureAssetList);
			{
				int ExistingMatIndex = k;
				int MaterialIndex = k;

				// material asset set flag for morph target 
				if (ImportUI->bImportMorphTargets) 
				{
					if (UMaterial* UnrealMaterialPtr = Cast<UMaterial>(Materials[MaterialIndex]))
					{
						//UnrealMaterialPtr->bUsedWithMorphTargets = 1; //5.7
						UnrealMaterialPtr->bHasPixelAnimation = 1; //5.8 //Material.h
					}
				}

				ImportData.Materials[ExistingMatIndex].MaterialImportName
					= //"M_" + 
					PmxMeshInfo->materialList[k].Name;
				ImportData.Materials[ExistingMatIndex].Material
					= Materials[MaterialIndex];
			}
		}
	}
	//UE_LOG(LogCategoryPMXFactory, Warning, TEXT("PMX Import Material Extecd Complete."));
#endif //不能删

	//store the UVs in arrays for fast access in the later looping of triangles 
	uint32 UniqueUVCount = UVSets.Num();

	UniqueUVCount = FMath::Min<uint32>(UniqueUVCount, MAX_TEXCOORDS);
	// One UV set is required but only import up to MAX_TEXCOORDS number of uv layers
	ImportData.NumTexCoords = FMath::Max<uint32>(ImportData.NumTexCoords, UniqueUVCount);

	ImportData.bHasNormals = false;
	ImportData.bHasTangents = true;

	// create the points / wedges / faces
	int32 ControlPointsCount = PmxMeshInfo->vertexList.Num();
	//Mesh->GetControlPointsCount();
	int32 ExistPointNum = ImportData.Points.Num();
	ImportData.Points.AddUninitialized(ControlPointsCount);

	for (int32 ControlPointsIndex = 0; ControlPointsIndex < ControlPointsCount; ControlPointsIndex++)
	{
		ImportData.Points[ControlPointsIndex + ExistPointNum]
			= PmxMeshInfo->vertexList[ControlPointsIndex].Position;
	}
#if 1 //vertex
	int32 VertexIndex;
	int32 TriangleCount = PmxMeshInfo->faseList.Num();//Mesh->GetPolygonCount();
	int32 ExistFaceNum = ImportData.Faces.Num();
	ImportData.Faces.AddUninitialized(TriangleCount);
	//int32 ExistWedgesNum = ImportData.Wedges.Num(); //5.8
	SkeletalMeshImportData::FVertex TmpWedges[3];

	int32 facecount = 0;
	int32 matIndx = 0;

	for (int32 TriangleIndex = ExistFaceNum, LocalIndex = 0; TriangleIndex < ExistFaceNum + TriangleCount; TriangleIndex++, LocalIndex++)
	{
		bool OddNegativeScale = true;// false;// IsOddNegativeScale(TotalMatrix);//不能换位置
		SkeletalMeshImportData::FTriangle& Triangle = ImportData.Faces[TriangleIndex];

		// smoothing mask
		// set the face smoothing by default. It could be any number, but not zero
		Triangle.SmoothingGroups = 255;

		for (VertexIndex = 0; VertexIndex < 3; VertexIndex++)
		{
			// If there are odd number negative scale, invert the vertex order for triangles
			int32 UnrealVertexIndex = OddNegativeScale ? 2 - VertexIndex : VertexIndex;

			if (true)
			{
				//for (NormalIndex = 0; NormalIndex < 3; ++NormalIndex)
				{
					int32 NormalIndex = UnrealVertexIndex;
					FVector3f TangentZ
						= PmxMeshInfo->vertexList[PmxMeshInfo->faseList[LocalIndex].VertexIndex[NormalIndex]].Normal;
					Triangle.TangentX[NormalIndex] = FVector3f::ZeroVector;
					Triangle.TangentY[NormalIndex] = FVector3f::ZeroVector;
					Triangle.TangentZ[NormalIndex] = TangentZ.GetSafeNormal();
				}
			}
			/*
			else //5.8 不能删
			{
				for (int32 NormalIndex = 0; NormalIndex < 3; ++NormalIndex)
				{
					Triangle.TangentX[NormalIndex] = FVector3f::ZeroVector;
					Triangle.TangentY[NormalIndex] = FVector3f::ZeroVector;
					Triangle.TangentZ[NormalIndex] = FVector3f::ZeroVector;
				}
			}//5.8 //不能删
			*/
		}

		// material index
		Triangle.MatIndex = 0; // default value

		if (true)
		{
			// for mmd

			if (PmxMeshInfo->materialList.Num() > matIndx)
			{
				facecount++;
				facecount++;
				facecount++;
				Triangle.MatIndex = matIndx;
				if (facecount >= PmxMeshInfo->materialList[matIndx].MaterialFaceNum)
				{
					matIndx++;
					facecount = 0;
				}
			}
		}

		Triangle.AuxMatIndex = 0;
		for (VertexIndex = 0; VertexIndex < 3; VertexIndex++)
		{
			// If there are odd number negative scale, invert the vertex order for triangles
			int32 UnrealVertexIndex = OddNegativeScale ? 2 - VertexIndex : VertexIndex;

			TmpWedges[UnrealVertexIndex].MatIndex = Triangle.MatIndex;
			TmpWedges[UnrealVertexIndex].VertexIndex
				= PmxMeshInfo->faseList[LocalIndex].VertexIndex[VertexIndex];
			// = ExistPointNum + Mesh->GetPolygonVertex(LocalIndex, VertexIndex);
			// Initialize all colors to white.
			TmpWedges[UnrealVertexIndex].Color = FColor::White;
		}

		// uvs
		// Some FBX meshes can have no UV sets, so also check the UniqueUVCount
		for (uint32 UVLayerIndex = 0; UVLayerIndex < UniqueUVCount; UVLayerIndex++)
		{
				// Set all UV's to zero.  If we are here the mesh had no UV sets so we only need to do this for the
				// first UV set which always exists.
				TmpWedges[VertexIndex].UVs[UVLayerIndex].X = 0;
				TmpWedges[VertexIndex].UVs[UVLayerIndex].Y = 0;
		}

		// basic wedges matching : 3 unique per face. TODO Can we do better ?
		for (VertexIndex = 0; VertexIndex < 3; VertexIndex++)
		{
			const int32 w = ImportData.Wedges.AddUninitialized();
			ImportData.Wedges[w].VertexIndex = TmpWedges[VertexIndex].VertexIndex;
			ImportData.Wedges[w].MatIndex = TmpWedges[VertexIndex].MatIndex;
			ImportData.Wedges[w].Color = TmpWedges[VertexIndex].Color;
			ImportData.Wedges[w].Reserved = 0;

			FVector2f tempUV
				= PmxMeshInfo->vertexList[TmpWedges[VertexIndex].VertexIndex].UV;
			TmpWedges[VertexIndex].UVs[0].X = tempUV.X;
			TmpWedges[VertexIndex].UVs[0].Y = tempUV.Y;
			FMemory::Memcpy(ImportData.Wedges[w].UVs,
				TmpWedges[VertexIndex].UVs,
				sizeof(FVector2D) * MAX_TEXCOORDS);
			Triangle.WedgeIndex[VertexIndex] = w;
		}
	}
#endif
#if 1
	// now we can work on a per-cluster basis with good ordering

	//For mmd. skining
	if (PmxMeshInfo->boneList.Num() > 0)
	{
		// create influences for each cluster
		//	for each vertex index in the cluster
		for (int32 ControlPointIndex = 0;
			ControlPointIndex < PmxMeshInfo->vertexList.Num();
			++ControlPointIndex)
		{
			// int32 multiBone = 0;
			int32 multiBone; //5.8
			switch (PmxMeshInfo->vertexList[ControlPointIndex].WeightType)
			{
			case 0://0:BDEF1
			{
				ImportData.Influences.AddUninitialized();
				ImportData.Influences.Last().BoneIndex = PmxMeshInfo->vertexList[ControlPointIndex].BoneIndex[0];
				ImportData.Influences.Last().Weight = PmxMeshInfo->vertexList[ControlPointIndex].BoneWeight[0];
				ImportData.Influences.Last().VertexIndex = ExistPointNum + ControlPointIndex;
			}
			break;
			case 1:// 1:BDEF2
			{
				for (multiBone = 0; multiBone < 2; ++multiBone)
				{
					ImportData.Influences.AddUninitialized();
					ImportData.Influences.Last().BoneIndex = PmxMeshInfo->vertexList[ControlPointIndex].BoneIndex[multiBone];
					ImportData.Influences.Last().Weight = PmxMeshInfo->vertexList[ControlPointIndex].BoneWeight[multiBone];
					ImportData.Influences.Last().VertexIndex = ExistPointNum + ControlPointIndex;
				}
				//UE_LOG(LogMMD4UE4_PMXFactory, Log, TEXT("BDEF2"));
			}
			break;
			case 2: //2:BDEF4
			{
				for (multiBone = 0; multiBone < 4; ++multiBone)
				{
					ImportData.Influences.AddUninitialized();
					ImportData.Influences.Last().BoneIndex = PmxMeshInfo->vertexList[ControlPointIndex].BoneIndex[multiBone];
					ImportData.Influences.Last().Weight = PmxMeshInfo->vertexList[ControlPointIndex].BoneWeight[multiBone];
					ImportData.Influences.Last().VertexIndex = ExistPointNum + ControlPointIndex;
				}
			}
			break;
			case 3: //3:SDEF
			{
				//限制：SDEF
				//将SDEF转换为BDEF2进行处理。
				//这是SDEF_C、SDEF_R0、SDEF_由于不知道如何设置R1的参数。
				//根据在另一PF（ex.MMD4Mecanim或Dxlib）中的安装例进行分析及信息收集
				//并保留，直到找到满足MMD SDEF操作规范的方法
				//关于SDEF的规格（MMD）请参考以下页面。
				//Ref:：各软件的SDEF变形差异-FC2
				// http://mikudan.blog120.fc2.com/blog-entry-339.html

				/////////////////////////////////////
				for (multiBone = 0; multiBone < 2; ++multiBone)
				{
					ImportData.Influences.AddUninitialized();
					ImportData.Influences.Last().BoneIndex = PmxMeshInfo->vertexList[ControlPointIndex].BoneIndex[multiBone];
					ImportData.Influences.Last().Weight = PmxMeshInfo->vertexList[ControlPointIndex].BoneWeight[multiBone];
					ImportData.Influences.Last().VertexIndex = ExistPointNum + ControlPointIndex;
				}
			}
			break;
#if 0 //for pmx ver 2.1 formnat
			case 4:
				// 制限事項：QDEF
				// QDEFに関して、MMDでの仕様を調べる事。
			{
				for (multiBone = 0; multiBone < 4; ++multiBone)
				{
					ImportData.Influences.AddUninitialized();
					ImportData.Influences.Last().BoneIndex = PmxMeshInfo->vertexList[ControlPointIndex].BoneIndex[multiBone];
					ImportData.Influences.Last().Weight = PmxMeshInfo->vertexList[ControlPointIndex].BoneWeight[multiBone];
					ImportData.Influences.Last().VertexIndex = ExistPointNum + ControlPointIndex;
				}
			}
			break;
#endif
			default:
			{
				//異常系
				//0:BDEF1 形式と同じ手法で暫定対応する
				ImportData.Influences.AddUninitialized();
				ImportData.Influences.Last().BoneIndex = PmxMeshInfo->vertexList[ControlPointIndex].BoneIndex[0];
				ImportData.Influences.Last().Weight = PmxMeshInfo->vertexList[ControlPointIndex].BoneWeight[0];
				ImportData.Influences.Last().VertexIndex = ExistPointNum + ControlPointIndex;
				UE_LOG(LogMMD4UE4_PMXFactory, Error,
					TEXT("Unkown Weight Type :: type = %d , vertex index = %d "),
					PmxMeshInfo->vertexList[ControlPointIndex].WeightType
					, ControlPointIndex
				);
			}
			break;
			}
		}

	}
#endif
	else // for rigid mesh
	{
		// find the bone index
		// int32 BoneIndex = -1; //5.8
		/*for (int32 LinkIndex = 0; LinkIndex < SortedLinks.Num(); LinkIndex++)
		{
			// the bone is the node itself
			if (Node == SortedLinks[LinkIndex])
			{
				BoneIndex = LinkIndex;
				break;
			}
		}*/
		// BoneIndex = 0; //5.8
		//	for each vertex in the mesh
		for (int32 ControlPointIndex = 0; ControlPointIndex < ControlPointsCount; ++ControlPointIndex)
		{
			int32 BoneIndex = 0; //5.8
			ImportData.Influences.AddUninitialized();
			ImportData.Influences.Last().BoneIndex = BoneIndex;
			ImportData.Influences.Last().Weight = 1.0;
			ImportData.Influences.Last().VertexIndex = ExistPointNum + ControlPointIndex;
		}
	}
	/*
	// clean up
	if (UniqueUVCount > 0)
	{
		delete[] LayerElementUV;
		delete[] UVReferenceMode;
		delete[] UVMappingMode;
	}
	*/

	return true;
}

UObject* UPmxFactory::CreateAssetOfClass(
	UClass* AssetClass,
	FString(ParentPackageName),
	FString(ObjectName),
	bool bAllowReplace,
	bool* bOutCreated
)
{
	if (bOutCreated)
	{
		*bOutCreated = false;
	}

	// See if this sequence already exists.
	FString 	ParentPath = FString::Printf(
		TEXT("%s/%s"),
		*ParentPackageName,
		*ObjectName);
	if (!FPackageName::IsValidLongPackageName(ParentPath))
	{
		UE_LOG(LogMMD4UE4_PMXFactory, Error,
			TEXT("Cannot create asset '%s': invalid package path '%s'."),
			*ObjectName,
			*ParentPath);
		return nullptr;
	}
	UObject* Parent = CreatePackage(*ParentPath);
	if (!Parent)
	{
		UE_LOG(LogMMD4UE4_PMXFactory, Error,
			TEXT("Cannot create package '%s' for asset '%s'."),
			*ParentPath,
			*ObjectName);
		return nullptr;
	}
	// See if an object with this name exists
	UObject* Object = LoadObject<UObject>(Parent, *ObjectName, nullptr, LOAD_None, nullptr);

	// if object with same name but different class exists, warn user
	if ((Object != nullptr) && (Object->GetClass() != AssetClass))
	{
		//UnFbx::FFbxImporter* FFbxImporter = UnFbx::FFbxImporter::GetInstance();
		//FFbxImporter->AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("Error_AssetExist", "Asset with same name exists. Can't overwrite another asset")), FFbxErrors::Generic_SameNameAssetExists);
		return nullptr;
	}

	// if object with same name exists, warn user
	if (Object != nullptr && !bAllowReplace)
	{
		// until we have proper error message handling so we don't ask every time, but once, I'm disabling it. 
		/*if ( EAppReturnType::Yes != FMessageDialog::Open( EAppMsgType::YesNo, LocalizeSecure(NSLOCTEXT("UnrealEd", "Error_AssetExistAsk", "Asset with the name (`~) exists. Would you like to overwrite?").ToString(), *ParentPath) ) ) 
		 {
			return NULL;
		 }*/
		//UnFbx::FFbxImporter* FFbxImporter = UnFbx::FFbxImporter::GetInstance();
		//FFbxImporter->AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Warning, FText::Format(LOCTEXT("FbxSkeletaLMeshimport_SameNameExist", "Asset with the name ('{0}') exists. Overwriting..."), FText::FromString(ParentPath))), FFbxErrors::Generic_SameNameAssetOverriding);
	}

	if (Object == nullptr)
	{
		Object = NewObject<UObject>(Parent, AssetClass, *ObjectName, RF_Public | RF_Standalone);
		if (Object && bOutCreated)
		{
			*bOutCreated = true;
		}
	}
	return Object;
}

/*
A class encapsulating morph target processing that occurs during import on a separate thread
*/
class FAsyncImportMorphTargetWork : public FNonAbandonableTask
{
public:
	FAsyncImportMorphTargetWork(
		USkeletalMesh* InTempSkelMesh,
		int32 InLODIndex,
		const FSkeletalMeshImportData& InImportData,
		bool bInKeepOverlappingVertices
	)
		: TempSkeletalMesh(InTempSkelMesh)
		, ImportData(InImportData)
		, LODIndex(InLODIndex)
		, bKeepOverlappingVertices(bInKeepOverlappingVertices)
	{
		MeshUtilities = &FModuleManager::Get().LoadModuleChecked<IMeshUtilities>("MeshUtilities");
	}

	void DoWork()
	{
		TArray<FVector3f> LODPoints;
		TArray<SkeletalMeshImportData::FMeshWedge> LODWedges;
		TArray<SkeletalMeshImportData::FMeshFace> LODFaces;
		TArray<SkeletalMeshImportData::FVertInfluence> LODInfluences;
		TArray<int32> LODPointToRawMap;
		ImportData.CopyLODImportData(LODPoints, LODWedges, LODFaces, LODInfluences, LODPointToRawMap);

		ImportData.Empty();

		MeshUtilities->BuildSkeletalMesh(
			TempSkeletalMesh->GetImportedModel()->LODModels[0], "MMDSKMS",
			TempSkeletalMesh->GetRefSkeleton(),
			LODInfluences,
			LODWedges,
			LODFaces,
			LODPoints,
			LODPointToRawMap
		);

	}

	static const TCHAR* Name()
	{
		return TEXT("FAsyncImportMorphTargetWork");
	}

#if 1
	//FORCEINLINE TStatId static GetStatId() const
	FORCEINLINE TStatId static GetStatId()//5.8
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FAsyncImportMorphTargetWork, STATGROUP_ThreadPoolAsyncTasks);
	}
#endif

private:
	USkeletalMesh* TempSkeletalMesh;
	FSkeletalMeshImportData ImportData;
	IMeshUtilities* MeshUtilities;
	int32 LODIndex;
	bool bKeepOverlappingVertices;
};

void UPmxFactory::ImportMorphTargetsInternal(
	//TArray<FbxNode*>& SkelMeshNodeArray,
	MMD4UE4::PmxMeshInfo& PmxMeshInfo,
	USkeletalMesh* BaseSkelMesh,
	UObject* InParent,
	const FString& InFilename,
	int32 LODIndex, FSkeletalMeshImportData& BaseImportData
)
{
	/*FbxString ShapeNodeName;
	TMap<FString, TArray<FbxShape*>> ShapeNameToShapeArray;*/
	TMap<FString, MMD4UE4::PMX_MORPH>  ShapeNameToShapeArray;
	TFunction<void(int32, float, TSet<int32>&, TMap<int32, FVector3f>&)> CollectVertexMorphOffsets;
	CollectVertexMorphOffsets = [&PmxMeshInfo, &CollectVertexMorphOffsets](
		int32 MorphIndex,
		float Weight,
		TSet<int32>& RecursionPath,
		TMap<int32, FVector3f>& OutOffsets)
	{
		if (!PmxMeshInfo.morphList.IsValidIndex(MorphIndex))
		{
			UE_LOG(LogMMD4UE4_PMXFactory, Warning,
				TEXT("Skipping PMX group morph reference with invalid morph index %d."),
				MorphIndex);
			return;
		}
		if (RecursionPath.Contains(MorphIndex))
		{
			UE_LOG(LogMMD4UE4_PMXFactory, Warning,
				TEXT("Skipping cyclic PMX group morph reference at index %d."),
				MorphIndex);
			return;
		}

		const MMD4UE4::PMX_MORPH& Morph = PmxMeshInfo.morphList[MorphIndex];
		if (Morph.Type == 1)
		{
			for (const MMD4UE4::PMX_MORPH_VERTEX& Vertex : Morph.Vertex)
			{
				if (!FMath::IsFinite(Weight) ||
					!FMath::IsFinite(Vertex.Offset.X) ||
					!FMath::IsFinite(Vertex.Offset.Y) ||
					!FMath::IsFinite(Vertex.Offset.Z))
				{
					continue;
				}
				const FVector3f WeightedOffset = Vertex.Offset * Weight;
				if (FVector3f* ExistingOffset = OutOffsets.Find(Vertex.Index))
				{
					*ExistingOffset += WeightedOffset;
				}
				else
				{
					OutOffsets.Add(Vertex.Index, WeightedOffset);
				}
			}
			return;
		}
		if (Morph.Type != 0)
		{
			return;
		}

		RecursionPath.Add(MorphIndex);
		for (const MMD4UE4::PMX_MORPH_GROUP& GroupEntry : Morph.Group)
		{
			CollectVertexMorphOffsets(
				GroupEntry.Index,
				Weight * GroupEntry.Ratio,
				RecursionPath,
				OutOffsets);
		}
		RecursionPath.Remove(MorphIndex);
	};

	TMap<FName, TMap<int32, FVector3f>> SourceOffsetsByMorph;
	for (int32 NodeIndex = 0; NodeIndex < PmxMeshInfo.morphList.Num(); NodeIndex++)
	{
		MMD4UE4::PMX_MORPH* pmxMorphPtr = &(PmxMeshInfo.morphList[NodeIndex]);
		if ((pmxMorphPtr->Type == 0 || pmxMorphPtr->Type == 1) &&
			pmxMorphPtr->DataNum > 0)
		{

			FString ShapeName = pmxMorphPtr->Name;
			ShapeName.TrimStartAndEndInline();
			if (ShapeName.IsEmpty())
			{
				UE_LOG(LogMMD4UE4_PMXFactory, Warning,
					TEXT("Skipping vertex morph with an empty name at index %d"),
					NodeIndex);
				continue;
			}

			TMap<int32, FVector3f> MorphOffsets;
			TSet<int32> RecursionPath;
			CollectVertexMorphOffsets(NodeIndex, 1.0f, RecursionPath, MorphOffsets);
			if (MorphOffsets.IsEmpty())
			{
				continue;
			}

			MMD4UE4::PMX_MORPH& ShapeArray = ShapeNameToShapeArray.FindOrAdd(ShapeName);
			ShapeArray = *pmxMorphPtr;
			ShapeArray.Type = 1;
			ShapeArray.Vertex.Reset(MorphOffsets.Num());
			for (const TPair<int32, FVector3f>& MorphOffset : MorphOffsets)
			{
				MMD4UE4::PMX_MORPH_VERTEX& Vertex = ShapeArray.Vertex.AddDefaulted_GetRef();
				Vertex.Index = MorphOffset.Key;
				Vertex.Offset = MorphOffset.Value;
			}
		}
	}
	bool bImportOperationCanceled = false;
	for (auto Iter = ShapeNameToShapeArray.CreateIterator(); Iter && !bImportOperationCanceled; ++Iter)
	{
		FString ShapeName = Iter.Key();
		MMD4UE4::PMX_MORPH& ShapeArray = Iter.Value();
		FSkeletalMeshImportData ShapeImportData;
		BaseImportData.CopyDataNeedByMorphTargetImport(ShapeImportData);

		//Store the rebuild morph data into the base import data, this will allow us to rebuild the morph data in case the mesh is rebuild and the vertex count change because of options (max bone per section, normals compute...)
		int32 MorphTargetIndex;
		if (BaseImportData.MorphTargetNames.Find(ShapeName, MorphTargetIndex))
		{
			BaseImportData.MorphTargetNames.RemoveAt(MorphTargetIndex);
			BaseImportData.MorphTargetModifiedPoints.RemoveAt(MorphTargetIndex);
			BaseImportData.MorphTargets.RemoveAt(MorphTargetIndex);
			SourceOffsetsByMorph.Remove(FName(*ShapeName));
		}
		BaseImportData.MorphTargetNames.Add(ShapeName);
		TSet<uint32>& ModifiedPoints = BaseImportData.MorphTargetModifiedPoints.AddDefaulted_GetRef();

		TMap<int32, FVector3f> MorphOffsets;

		for (const MMD4UE4::PMX_MORPH_VERTEX& MorphVertex : ShapeArray.Vertex)
		{
			if (!ShapeImportData.Points.IsValidIndex(MorphVertex.Index))
			{
				UE_LOG(LogMMD4UE4_PMXFactory, Warning,
					TEXT("Skipping morph '%s' vertex with invalid index %d"),
					*ShapeName, MorphVertex.Index);
				continue;
			}
			if (FVector3f* ExistingOffset = MorphOffsets.Find(MorphVertex.Index))
			{
				*ExistingOffset += MorphVertex.Offset;
			}
			else
			{
				MorphOffsets.Add(MorphVertex.Index, MorphVertex.Offset);
			}
		}

		TArray<FVector3f> CompressPoints;
		CompressPoints.Reserve(MorphOffsets.Num());
		int32 NonZeroOffsetCount = 0;
		float MaxOffsetMagnitude = 0.0f;
		for (const TPair<int32, FVector3f>& MorphOffset : MorphOffsets)
		{
			if (!FMath::IsFinite(MorphOffset.Value.X) ||
				!FMath::IsFinite(MorphOffset.Value.Y) ||
				!FMath::IsFinite(MorphOffset.Value.Z))
			{
				continue;
			}
			ModifiedPoints.Add(MorphOffset.Key);
			ShapeImportData.Points[MorphOffset.Key] += MorphOffset.Value;
			if (!MorphOffset.Value.IsNearlyZero())
			{
				++NonZeroOffsetCount;
				MaxOffsetMagnitude = FMath::Max(MaxOffsetMagnitude, MorphOffset.Value.Size());
			}
		}
		SourceOffsetsByMorph.Add(FName(*ShapeName), MorphOffsets);
		for (const uint32 PointIndex : ModifiedPoints)
		{
			CompressPoints.Add(ShapeImportData.Points[PointIndex]);
		}
		UE_LOG(LogMMD4UE4_PMXFactory, Log,
			TEXT("PMX morph input '%s': points=%d, sourceOffsets=%d, modified=%d, nonZero=%d, maxOffset=%g, compressed=%d"),
			*ShapeName,
			ShapeImportData.Points.Num(),
			MorphOffsets.Num(),
			ModifiedPoints.Num(),
			NonZeroOffsetCount,
			MaxOffsetMagnitude,
			CompressPoints.Num());
		ShapeImportData.Points = MoveTemp(CompressPoints);
		//GatherPointsForMorphTarget(&ShapeImportData, SkelMeshNodeArray, &ShapeArray, ModifiedPoints);
		//We do not need this data anymore empty it so we reduce the size of what we save into memory
		ShapeImportData.PointToRawMap.Empty();
		BaseImportData.MorphTargets.Add(ShapeImportData);
		check(BaseImportData.MorphTargetNames.Num() == BaseImportData.MorphTargets.Num() && BaseImportData.MorphTargetNames.Num() == BaseImportData.MorphTargetModifiedPoints.Num());
	}

	if (BaseSkelMesh->GetImportedModel() && BaseSkelMesh->GetImportedModel()->LODModels.IsValidIndex(LODIndex))
	{
		// PMX morph data is added after the base mesh has already been built, so the
		// mesh builder could not have created these targets from the import data.
		FOverlappingThresholds hd;
#if UE_VERSION_OLDER_THAN(5,4,0)
		FLODUtilities::BuildMorphTargets(
			BaseSkelMesh,
			BaseImportData,
			LODIndex,
			true,
			true,
			true,
			hd);
#else
		if (!BaseSkelMesh->GetMeshDescription(LODIndex))
		{
			FMeshDescription MeshDescription;
			const FSkeletalMeshLODModel& LODModel =
				BaseSkelMesh->GetImportedModel()->LODModels[LODIndex];
			LODModel.GetMeshDescription(BaseSkelMesh, LODIndex, MeshDescription);
			BaseSkelMesh->CreateMeshDescription(LODIndex, MoveTemp(MeshDescription));
		}

		if (const FMeshDescription* MeshDescription = BaseSkelMesh->GetMeshDescription(LODIndex))
		{
			FLODUtilities::BuildMorphTargets(
				BaseSkelMesh,
				*MeshDescription,
				BaseImportData,
				LODIndex,
				true,
				true,
				true,
				hd);

#if WITH_EDITOR
			if (BaseSkelMesh->GetMorphTargets().Num() == 0)
			{
				FSkeletalMeshLODModel& LODModel =
					BaseSkelMesh->GetImportedModel()->LODModels[LODIndex];
				const TArray<uint32>& RawPointIndices = LODModel.GetRawPointIndices();
				int32 MaxVertexIndex = 0;
				for (const FSkelMeshSection& Section : LODModel.Sections)
				{
					MaxVertexIndex = FMath::Max(
						MaxVertexIndex,
						static_cast<int32>(Section.BaseVertexIndex + Section.SoftVertices.Num()));
				}

				if (RawPointIndices.Num() != MaxVertexIndex)
				{
					UE_LOG(LogMMD4UE4_PMXFactory, Error,
						TEXT("Cannot build PMX morph target fallback: raw point map has %d entries, expected %d."),
						RawPointIndices.Num(),
						MaxVertexIndex);
				}
				else
				{
					int32 FallbackRegistered = 0;
					for (const TPair<FName, TMap<int32, FVector3f>>& MorphOffsets : SourceOffsetsByMorph)
					{
						if (BaseSkelMesh->FindMorphTarget(MorphOffsets.Key))
						{
							continue;
						}

						TArray<FMorphTargetDelta> Deltas;
						for (const FSkelMeshSection& Section : LODModel.Sections)
						{
							for (int32 SectionVertexIndex = 0;
								SectionVertexIndex < Section.SoftVertices.Num();
								++SectionVertexIndex)
							{
								const uint32 LODVertexIndex = Section.BaseVertexIndex + SectionVertexIndex;
								const int32 SourcePointIndex = RawPointIndices[LODVertexIndex];
								const FVector3f* PositionDelta = MorphOffsets.Value.Find(SourcePointIndex);
								if (!PositionDelta || PositionDelta->IsNearlyZero())
								{
									continue;
								}

								FMorphTargetDelta& Delta = Deltas.AddDefaulted_GetRef();
								Delta.SourceIdx = LODVertexIndex;
								Delta.PositionDelta = *PositionDelta;
							}
						}
						if (Deltas.IsEmpty())
						{
							UE_LOG(LogMMD4UE4_PMXFactory, Warning,
								TEXT("PMX morph target fallback found no render vertices for '%s' (input points=%d)."),
								*MorphOffsets.Key.ToString(),
								MorphOffsets.Value.Num());
							continue;
						}

						UMorphTarget* MorphTarget = NewObject<UMorphTarget>(
							BaseSkelMesh,
							MorphOffsets.Key,
							RF_Public | RF_Standalone);
						MorphTarget->PopulateDeltas(Deltas, LODIndex, LODModel.Sections);
						if (MorphTarget->HasValidData() &&
							BaseSkelMesh->RegisterMorphTarget(MorphTarget, true))
						{
							++FallbackRegistered;
						}
						else
						{
							UE_LOG(LogMMD4UE4_PMXFactory, Warning,
								TEXT("PMX morph target fallback produced no valid deltas for '%s' (input=%d, deltas=%d)."),
								*MorphOffsets.Key.ToString(),
								MorphOffsets.Value.Num(),
								Deltas.Num());
						}
					}
					if (FallbackRegistered > 0)
					{
						UE_LOG(LogMMD4UE4_PMXFactory, Log,
							TEXT("Registered %d PMX morph targets using raw point mapping fallback."),
							FallbackRegistered);
					}
				}
			}
#endif
		}
		else
		{
			UE_LOG(LogMMD4UE4_PMXFactory, Warning,
				TEXT("Cannot build PMX morph targets for LOD %d: mesh description is unavailable."),
				LODIndex);
		}
#endif
		UE_LOG(LogMMD4UE4_PMXFactory, Log,
			TEXT("Processed PMX morph targets for LOD %d: requested=%d, registered=%d."),
			LODIndex,
			BaseImportData.MorphTargetNames.Num(),
			BaseSkelMesh->GetMorphTargets().Num());
		if (BaseImportData.MorphTargetNames.Num() > 0 &&
			BaseSkelMesh->GetMorphTargets().Num() == 0)
		{
			UE_LOG(LogMMD4UE4_PMXFactory, Warning,
				TEXT("PMX morph target build produced no registered morph targets."));
		}
	}

}

// Import hh target
void UPmxFactory::ImportFbxMorphTarget(
	//TArray<FbxNode*> &SkelMeshNodeArray,
	MMD4UE4::PmxMeshInfo& PmxMeshInfo,
	USkeletalMesh* BaseSkelMesh,
	UObject* InParent,
	const FString& Filename,
	int32 LODIndex,
	FSkeletalMeshImportData& ImportData
)
{
	//bool bHasMorph = false; //5.8
	if (PmxMeshInfo.morphList.Num() > 0)
	{
#if !UE_VERSION_OLDER_THAN(5,4,0)
		if (BaseSkelMesh)
		{
			BaseSkelMesh->CommitMeshDescription(LODIndex);
		}
#endif
#if WITH_EDITOR
		if (BaseSkelMesh)
		{
			TArray<USkinnedAsset*> AssetsToFinish;
			AssetsToFinish.Add(BaseSkelMesh);
			FSkinnedAssetCompilingManager::FFinishCompilationOptions FinishOptions;
			FinishOptions.bIncludeDependentAssets = true;
			FSkinnedAssetCompilingManager::Get().FinishCompilation(AssetsToFinish, FinishOptions);
		}
#endif
		ImportMorphTargetsInternal(
			//SkelMeshNodeArray,//未声明标识符 
			PmxMeshInfo,
			BaseSkelMesh,
			InParent,
			Filename,
			LODIndex, ImportData
		);
#if UE_VERSION_OLDER_THAN(5,4,0)//ydgro
		BaseSkelMesh->SaveLODImportedData(LODIndex,ImportData);
#endif
	}
	//BaseSkelMesh->SaveLODImportedData(0, ImportData);//UE5.7不再提供支持

	//后处理。在UE5.7需要调用CreateMeshDescription
	if (BaseSkelMesh)
    {
        BaseSkelMesh->PostLoad();
    }
}

void UPmxFactory::AddTokenizedErrorMessage(
	TSharedRef<FTokenizedMessage> ErrorMsg,
	FName FbxErrorName
)

{
	// if not found, use normal log
	switch (ErrorMsg->GetSeverity())
	{
	case EMessageSeverity::Error:
		UE_LOG(LogMMD4UE4_PMXFactory, Error, TEXT("%d_%s"), __LINE__, *(ErrorMsg->ToText().ToString()));
		break;
	case EMessageSeverity::Warning:
		UE_LOG(LogMMD4UE4_PMXFactory, Warning, TEXT("%d_%s"), __LINE__, *(ErrorMsg->ToText().ToString()));
		break;
	default:
		UE_LOG(LogMMD4UE4_PMXFactory, Warning, TEXT("%d_%s"), __LINE__, *(ErrorMsg->ToText().ToString()));
		break;
	}
}
#undef LOCTEXT_NAMESPACE