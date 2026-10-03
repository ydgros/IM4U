// Copyright 2023 NaN_Name, Inc. All Rights Reserved.

/*=============================================================================
Main implementation of FFbxImporter : import FBX data to Unreal
=============================================================================*/
#include "PmxImporter.h"
#include "PmxFactory.h"
#include "PmxOptionWindow.h"
#include "MMDSkeletalMeshImportData.h"
#include "MMDStaticMeshImportData.h"
#include "MainFrame.h"
#include "EngineAnalytics.h"
#include "Factories/FbxStaticMeshImportData.h"

//DEFINE_LOG_CATEGORY(LogPmx);

#define LOCTEXT_NAMESPACE "PmxMainImport"
/*
//Removed undefined PmxMeshInfo static instance (no definition available)
TSharedPtr<PmxMeshInfo> PmxMeshInfo::StaticInstance;
TSharedPtr<FPmxImporter> FPmxImporter::StaticInstance;
*/

PMXImportOptions* GetImportOptions(
	//class FPmxImporter* PmxImporter,
	const class FPmxImporter* PmxImporter, //5.8
	UPmxImportUI* ImportUI,
	bool bShowOptionDialog,
	const FString& FullPath,
	bool& bOutOperationCanceled,
	bool& bOutImportAll,
	bool bIsObjFormat,
	bool bForceImportType,
	EPMXImportType ImportType 
	)
{
	if (!PmxImporter || !ImportUI)
	{
		bOutOperationCanceled = true;
		bOutImportAll = false;
		return nullptr;
	}

	bOutOperationCanceled = false;
	ImportUI->bCreatePhysicsAsset = true;
	if (bShowOptionDialog)
	{
		bOutImportAll = false;

		PMXImportOptions* ImportOptions 
			= PmxImporter->GetMutableImportOptions();
		if (!ImportOptions)
		{
			bOutOperationCanceled = true;
			return nullptr;
		}
		// if Skeleton was set by outside, please make sure copy back to UI
		if (ImportOptions->SkeletonForAnimation)
		{
			ImportUI->Skeleton = ImportOptions->SkeletonForAnimation;
		}
		else
		{
			ImportUI->Skeleton = nullptr;
		}

		if (ImportOptions->PhysicsAsset)
		{
			ImportUI->PhysicsAsset = ImportOptions->PhysicsAsset;
		}
		else
		{
			ImportUI->PhysicsAsset = nullptr;
		}
		if (bForceImportType)
		{
			ImportUI->MeshTypeToImport = ImportType;
			ImportUI->OriginalImportType = ImportType;
		}

		//last select asset ref
		if (ImportOptions->MmdExtendAsset)
		{
			ImportUI->MmdExtendAsset = ImportOptions->MmdExtendAsset;
		}
		else
		{
			ImportUI->MmdExtendAsset = nullptr;
		}
		if (ImportOptions->MMD2UE4NameTableRow)
		{
			ImportUI->MMD2UE4NameTableRow = ImportOptions->MMD2UE4NameTableRow;
		}
		else
		{
			ImportUI->MMD2UE4NameTableRow = nullptr;
		}
		if (ImportOptions->AnimSequenceAsset)
		{
			ImportUI->AnimSequenceAsset = ImportOptions->AnimSequenceAsset;
		}
		else
		{
			ImportUI->AnimSequenceAsset = nullptr;
		}

		ImportUI->bImportAsSkeletal = ImportUI->MeshTypeToImport == PMXIT_SkeletalMesh;
		ImportUI->bIsObjImport = bIsObjFormat;

		TSharedPtr<SWindow> ParentWindow;
		if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
		{
			IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
			ParentWindow = MainFrame.GetParentWindow();//注释后无法导入模型
			(void)MainFrame; //5.8 //parent window not needed; silence unused variable warnings
		}

		TSharedRef<SWindow> Window = SNew(SWindow)
			//.Title(NSLOCTEXT("UnrealEd", "FBXImportOpionsTitle", "FBX Import Options"))
			.Title(NSLOCTEXT("IM4U", "MMDImportOpionsTitle", "MMD Import Options"))
			.SizingRule(ESizingRule::Autosized);

		TSharedPtr<SPmxOptionWindow> PmxOptionWindow;
		Window->SetContent
			(
			SAssignNew(PmxOptionWindow, SPmxOptionWindow)
			.ImportUI(ImportUI)
			.WidgetWindow(Window)
			.FullPath(FText::FromString(FullPath))
			.ForcedImportType(bForceImportType ? TOptional<EPMXImportType>(ImportType) : TOptional<EPMXImportType>())
			.IsObjFormat(bIsObjFormat)
			);

		/* @todo: we can make this slow as showing progress bar later */
		FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);

		ImportUI->SaveConfig();

		if (ImportUI->StaticMeshImportData)
		{
			ImportUI->StaticMeshImportData->SaveConfig();
		}

		if (ImportUI->SkeletalMeshImportData)
		{
			ImportUI->SkeletalMeshImportData->SaveConfig();
		}
#if 0
		if (ImportUI->AnimSequenceImportData)
		{
			ImportUI->AnimSequenceImportData->SaveConfig();
		}

		if (ImportUI->TextureImportData)
		{
			ImportUI->TextureImportData->SaveConfig();
		}
#endif
		if (PmxOptionWindow->ShouldImport())
		{
			bOutImportAll = PmxOptionWindow->ShouldImportAll();

			// open dialog
			// see if it's canceled
			ApplyImportUIToImportOptions(ImportUI, *ImportOptions);

			return ImportOptions;
		}
		else
		{
			bOutOperationCanceled = true;
		}
	}
	else if (GIsAutomationTesting)
	{
		//Automation tests set ImportUI settings directly.  Just copy them over
		PMXImportOptions* ImportOptions = PmxImporter->GetMutableImportOptions();
		ApplyImportUIToImportOptions(ImportUI, *ImportOptions);
		return ImportOptions;
	}
	else
	{
		return PmxImporter->GetMutableImportOptions();
	}
	return nullptr;

}

void ApplyImportUIToImportOptions(
	UPmxImportUI* ImportUI,
	PMXImportOptions& InOutImportOptions
	)
{
	check(ImportUI);

	InOutImportOptions.bImportMaterials = ImportUI->bImportMaterials;
	//InOutImportOptions.bInvertNormalMap = ImportUI->TextureImportData->bInvertNormalMaps;
	InOutImportOptions.bImportTextures = ImportUI->bImportTextures;
	InOutImportOptions.bCreateMaterialInstMode = ImportUI->bCreateMaterialInstMode;
	InOutImportOptions.bUnlitMaterials = ImportUI->bUnlitMaterials;
	InOutImportOptions.bUsedAsFullName = ImportUI->bOverrideFullName;
	InOutImportOptions.bConvertScene = ImportUI->bConvertScene;
	InOutImportOptions.bImportAnimations = ImportUI->bImportAnimations;
#if 1
	InOutImportOptions.SkeletonForAnimation = ImportUI->Skeleton;
#endif
	if (ImportUI->MeshTypeToImport == PMXIT_StaticMesh)
	{
		UMMDStaticMeshImportData* StaticMeshData = ImportUI->StaticMeshImportData;
		InOutImportOptions.NormalImportMethod = StaticMeshData->NormalImportMethod;
		InOutImportOptions.ImportTranslation = StaticMeshData->ImportTranslation;
		InOutImportOptions.ImportRotation = StaticMeshData->ImportRotation;
		InOutImportOptions.ImportUniformScale = StaticMeshData->ImportUniformScale;
	}
	else if (ImportUI->MeshTypeToImport == PMXIT_SkeletalMesh)
	{
		UMMDSkeletalMeshImportData* SkeletalMeshData = ImportUI->SkeletalMeshImportData;
		InOutImportOptions.NormalImportMethod = SkeletalMeshData->NormalImportMethod;
		InOutImportOptions.ImportTranslation = SkeletalMeshData->ImportTranslation;
		InOutImportOptions.ImportRotation = SkeletalMeshData->ImportRotation;
		InOutImportOptions.ImportUniformScale = SkeletalMeshData->ImportUniformScale;

#if 0
		if (ImportUI->bImportAnimations)
		{
			// Copy the transform information into the animation data to match the mesh.
			UFbxAnimSequenceImportData* AnimData = ImportUI->AnimSequenceImportData;
			AnimData->ImportTranslation = SkeletalMeshData->ImportTranslation;
			AnimData->ImportRotation = SkeletalMeshData->ImportRotation;
			AnimData->ImportUniformScale = SkeletalMeshData->ImportUniformScale;
		}
	}
	else
	{
		UFbxAnimSequenceImportData* AnimData = ImportUI->AnimSequenceImportData;
		InOutImportOptions.NormalImportMethod = FBXNIM_ComputeNormals;
		InOutImportOptions.ImportTranslation = AnimData->ImportTranslation;
		InOutImportOptions.ImportRotation = AnimData->ImportRotation;
		InOutImportOptions.ImportUniformScale = AnimData->ImportUniformScale;
#endif
	}
	//add self pre over write..
	ImportUI->SkeletalMeshImportData->bImportMorphTargets = ImportUI->bImportMorphTargets;
	// only re-sample if they don't want to use default sample rate
	InOutImportOptions.bResample = ImportUI->bUseDefaultSampleRate == false;
	InOutImportOptions.bImportMorph = ImportUI->SkeletalMeshImportData->bImportMorphTargets;
	InOutImportOptions.bUpdateSkeletonReferencePose = ImportUI->SkeletalMeshImportData->bUpdateSkeletonReferencePose;
	InOutImportOptions.bImportRigidMesh = ImportUI->OriginalImportType == PMXIT_StaticMesh && ImportUI->MeshTypeToImport == PMXIT_SkeletalMesh;
	InOutImportOptions.bUseT0AsRefPose = ImportUI->SkeletalMeshImportData->bUseT0AsRefPose;
	InOutImportOptions.bPreserveSmoothingGroups = ImportUI->SkeletalMeshImportData->bPreserveSmoothingGroups;
	InOutImportOptions.bKeepOverlappingVertices = ImportUI->SkeletalMeshImportData->bKeepOverlappingVertices;
	InOutImportOptions.bCombineToSingle = ImportUI->bCombineMeshes;
	InOutImportOptions.VertexColorImportOption = ImportUI->StaticMeshImportData->VertexColorImportOption;
	InOutImportOptions.VertexOverrideColor = ImportUI->StaticMeshImportData->VertexOverrideColor;
	InOutImportOptions.bRemoveDegenerates = ImportUI->StaticMeshImportData->bRemoveDegenerates;
	InOutImportOptions.bGenerateLightmapUVs = ImportUI->StaticMeshImportData->bGenerateLightmapUVs;
	InOutImportOptions.bOneConvexHullPerUCX = ImportUI->StaticMeshImportData->bOneConvexHullPerUCX;
	InOutImportOptions.bAutoGenerateCollision = ImportUI->StaticMeshImportData->bAutoGenerateCollision;
	InOutImportOptions.StaticMeshLODGroup = ImportUI->StaticMeshImportData->StaticMeshLODGroup;
	InOutImportOptions.bImportMeshesInBoneHierarchy = ImportUI->SkeletalMeshImportData->bImportMeshesInBoneHierarchy;
	InOutImportOptions.bCreatePhysicsAsset = ImportUI->bCreatePhysicsAsset;
	InOutImportOptions.PhysicsAsset = ImportUI->PhysicsAsset;
#if 0
	// animation options
	InOutImportOptions.AnimationLengthImportType = ImportUI->AnimSequenceImportData->AnimationLength;
	InOutImportOptions.AnimationRange.X = ImportUI->AnimSequenceImportData->StartFrame;
	InOutImportOptions.AnimationRange.Y = ImportUI->AnimSequenceImportData->EndFrame;
	InOutImportOptions.AnimationName = ImportUI->AnimationName;
	InOutImportOptions.bPreserveLocalTransform = ImportUI->bPreserveLocalTransform;
	InOutImportOptions.bImportCustomAttribute = ImportUI->AnimSequenceImportData->bImportCustomAttribute;
#endif
	//add self
	InOutImportOptions.AnimSequenceAsset = ImportUI->AnimSequenceAsset;
	InOutImportOptions.MMD2UE4NameTableRow = ImportUI->MMD2UE4NameTableRow;
	InOutImportOptions.MmdExtendAsset = ImportUI->MmdExtendAsset;
}

TSharedPtr<FPmxImporter> FPmxImporter::StaticInstance;
////////////////////////////////////////////
FPmxImporter::FPmxImporter()
	:ImportOptions(nullptr)//5.8
	/*
	, Scene(NULL)
	, GeometryConverter(NULL)
	, SdkManager(NULL)
	, Importer(NULL)
	, bFirstMesh(true)
	, Logger(NULL)
	*/
{
#if 0
	// Create the SdkManager
	SdkManager = FbxManager::Create();

	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(SdkManager, IOSROOT);
	SdkManager->SetIOSettings(ios);

	// Create the geometry converter
	GeometryConverter = new FbxGeometryConverter(SdkManager);
	Scene = NULL;

	CurPhase = NOTSTARTED;
#endif
	ImportOptions = MakeUnique<PMXImportOptions>();
	FMemory::Memzero(*ImportOptions);

}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
FPmxImporter::~FPmxImporter()
{
	CleanUp();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
FPmxImporter* FPmxImporter::GetInstance()
{
	if (!StaticInstance.IsValid())
	{
		StaticInstance = MakeShareable(new FPmxImporter());
	}
	return StaticInstance.Get();
}

void FPmxImporter::DeleteInstance()
{
	StaticInstance.Reset();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void FPmxImporter::CleanUp()
{
#if 0
	ClearTokenizedErrorMessages();
	ReleaseScene();

	delete GeometryConverter;
	GeometryConverter = NULL;
#endif
	ImportOptions.Reset();
#if 0
	if (SdkManager)
	{
		SdkManager->Destroy();
	}
	SdkManager = NULL;
	Logger = NULL;
#endif
}

const PMXImportOptions* FPmxImporter::GetImportOptions() const
{
	return ImportOptions.Get();
}

PMXImportOptions* FPmxImporter::GetMutableImportOptions() const
{
	return ImportOptions.Get();
}

///////////////////////////////////////////////////////////////////////////

UPmxImportUI::UPmxImportUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)//, MMD2UE4NameTableRow(MMD2UE4NameTableRowDmmy)
{
	bCombineMeshes = true;

	StaticMeshImportData = CreateDefaultSubobject<UMMDStaticMeshImportData>(TEXT("StaticMeshImportData"));
	SkeletalMeshImportData = CreateDefaultSubobject<UMMDSkeletalMeshImportData>(TEXT("SkeletalMeshImportData"));

}

bool UPmxImportUI::CanEditChange(const FProperty* InProperty) const
{
	bool bIsMutable = Super::CanEditChange(InProperty);
	if (bIsMutable && InProperty != nullptr)
	{
		FName PropName = InProperty->GetFName();

		if (PropName == TEXT("StartFrame") || PropName == TEXT("EndFrame"))
		{
			//bIsMutable = AnimSequenceImportData->AnimationLength == FBXALIT_SetRange && bImportAnimations;
		}
		else if (PropName == TEXT("bImportCustomAttribute") || PropName == TEXT("AnimationLength"))
		{
			bIsMutable = bImportAnimations;
		}

		if (bIsObjImport == false && InProperty->GetBoolMetaData(TEXT("OBJRestrict")))
		{
			bIsMutable = false;
		}
	}

	return bIsMutable;
}


#undef LOCTEXT_NAMESPACE