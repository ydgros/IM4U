// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
//see S:\UE5SRC\EpicUE5\UE_5.0\Engine\Source\Editor\UnrealEd\Private\Fbx\FbxSkeletalMeshImport.cpp
#include "PmxFactory.h"
#include "PmdImporter.h"
#include "PmxImportUI.h"
#include "MMDSkeletalMeshImportData.h"
#include "MmdBoneNameUtils.h"
#include "Misc/PackageName.h"
#include "Misc/EngineVersionComparison.h"
#include "Modules/ModuleManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Developer/MeshUtilities/Public/MeshUtilities.h"
#include "Developer/PhysicsUtilities/Public/PhysicsAssetUtils.h"
#include "ImportUtils/SkelImport.h"
#include "ImportUtils/SkeletalMeshImportUtils.h"
#include "Misc/FbxErrors.h"
#include "AssetNotifications.h"
#include "Animation/MorphTarget.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Rendering/SkeletalMeshModel.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/PhysicsConstraintTemplate.h"
#include "PhysicsEngine/SkeletalBodySetup.h"
#include "ObjectTools.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/UnrealType.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "PMXImpoter"

DEFINE_LOG_CATEGORY(LogMMD4UE4_PMXFactory)

namespace
{
	FString GetMeshAssetDirectory(const USkeletalMesh* SkeletalMesh)
	{
		return FPackageName::GetLongPackagePath(SkeletalMesh->GetOutermost()->GetName());
	}

	/*
*MMDExtendAsset的存放子文件夹名
*与SkeletalMesh、PhysicsAsset、Skeleton并列于模型目录下
	 */
	const TCHAR* GetMMDExtendAssetFolderName()
	{
		return TEXT("ExtendAsset");
	}
}

/////////////////////////////////////////////////////////
// 3 "ProcessImportMesh..." functions outputing Unreal data from a filled FSkeletalMeshBinaryImport
// and a handfull of other minor stuff needed by these 
// Fully taken from SkeletalMeshImport.cpp

using namespace SkeletalMeshImportUtils;
using namespace MMD4UE4;
/////////////////////////////////////////////////////////

UPmxFactory::UPmxFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//FEngineVersion version = FEngineVersion::Current();
	//if (version.GetMajor() == 5 && version.GetMinor() > 1) {}

	SupportedClass = nullptr;
	SupportedClass = UPmxFactory::StaticClass();
	Formats.Empty();

	Formats.Add(TEXT("pmd;PMD meshes and animations"));
	Formats.Add(TEXT("pmx;PMX meshes and animations"));

	bCreateNew = false;
	bText = false;
	bEditorImport = true;
	
	bOperationCanceled = false;
	bDetectImportTypeOnImport = false;

	//ImportUI = NewObject<UPmxImportUI>(this, NAME_None, RF_NoFlags);
}

void UPmxFactory::PostInitProperties()
{
	Super::PostInitProperties();

	ImportUI = NewObject<UPmxImportUI>(this, NAME_None, RF_NoFlags);
}

bool UPmxFactory::DoesSupportClass(UClass* Class)
{
	return (Class == UPmxFactory::StaticClass());
}

UClass* UPmxFactory::ResolveSupportedClass()
{
	return UPmxFactory::StaticClass();
}

////////////////////////////////////////////////
//IM4U Develop Temp Define
//////////////////////////////////////////////
//#define DEBUG_MMD_UE4_ORIGINAL_CODE	(1)
#define DEBUG_MMD_PLUGIN_SKELTON	(1) 
//#define DEBUG_MMD_PLUGIN_STATICMESH	(1)
//#define DEBUG_MMD_PLUGIN_ANIMATION	(1)
//////////////////////////////////////////////

bool UPmxFactory::FImportPmxFromFile(FString file) {
	UObject* NewObject = nullptr;//UE5.8//位置不能移动
	FString filepath = file;
	int32 indexs = -1;
	if (filepath.FindLastChar('\\', indexs)) {
		filepath = filepath.Right(filepath.Len() - indexs - 1);
		if (filepath.FindLastChar('.', indexs)) {
			filepath = filepath.Left(indexs);
			if (FPaths::FileExists(file)) {
				FString _path = filepath;
				_path = FPaths::ProjectDir() + TEXT("Content/") + *_path;
				_path = FPaths::ConvertRelativePathToFull(*_path);
				if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*_path)) {
					FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*_path);
				}

				TArray<uint8> File_Result;
				if (FFileHelper::LoadFileToArray(File_Result, *file)) {
					const uint8* DataPtr = File_Result.GetData();
					//UE_LOG(LogTemp,Warning,TEXT("!!!%s"),*file);
					FPmxImporter* PmxImporter = FPmxImporter::GetInstance();
					EPMXImportType ForcedImportType = PMXIT_SkeletalMesh;
					// For multiple files, use the same settings
					bDetectImportTypeOnImport = false;
					importAssetTypeMMD = E_MMD_TO_UE4_SKELTON;
					bool bIsPmxFormat = true;
					if (file.Find(TEXT(".pmx"))!=INDEX_NONE)
					{
						//Is PMX format 
						bIsPmxFormat = true;
					}
					//Load MMD Model From binary File
					MMD4UE4::PmxMeshInfo pmxMeshInfoPtr;
					//pmxMaterialImportHelper.InitializeBaseValue(InParent);
					bool pmxImportResult = false;
					if (bIsPmxFormat)
					{
						//pmx ver
						pmxImportResult = pmxMeshInfoPtr.PMXLoaderBinary(
							DataPtr,
							File_Result.GetData() + File_Result.Num());
					}
					else
					{
						//pmd ver
						MMD4UE4::PmdMeshInfo PmdMeshInfo;
						if (PmdMeshInfo.PMDLoaderBinary(
							DataPtr,
							File_Result.GetData() + File_Result.Num()))
						{
							//Up convert From PMD to PMX format gfor ue4
							pmxImportResult = PmdMeshInfo.ConvertToPmxFormat(&pmxMeshInfoPtr);
						}
					}
					if (pmxImportResult)
					{
						// show Import Option Slate
						bool bImportAll = false;
						ImportUI->bIsObjImport = true;//obj mode
						ImportUI->OriginalImportType = EPMXImportType::PMXIT_SkeletalMesh;
						ImportUI->bImportMaterials = true;
						ImportUI->bCreateMaterialInstMode = true;
						ImportUI->bCreatePhysicsAsset = true;
						ImportUI->bImportMorphTargets = true;
						ImportUI->bImportTextures = false;
						ImportUI->bUnlitMaterials = false;
						ImportUI->SkeletalMeshImportData->bImportMorphTargets = true;
						FString packpath = "/Game/" + filepath + "/" + filepath;
						UObject* InParent = CreatePackage(*packpath);
						/*UObject* InParent = LoadObject<UObject>(ParentPackage, *filepath);
						InParent->MarkPackageDirty();*/
						
						PMXImportOptions* ImportOptions
							= GetImportOptions(
								PmxImporter,
								ImportUI,
								false,//bShowImportDialog, 
								InParent->GetPathName(),
								bOperationCanceled,
								bImportAll,
								ImportUI->bIsObjImport,//bIsPmxFormat,
								bIsPmxFormat,
								ForcedImportType
							);
						if (bImportAll)
						{
							// If the user chose to import all, we don't show the dialog again and use the same settings for each object until importing another set of files
							//bShowOption = false;
						}
						
						if (ImportOptions)
						{
							//Warn->BeginSlowTask(NSLOCTEXT("PmxFactory", "BeginImportingPmxMeshTask", "Importing Pmx mesh"), true);

								// For animation and static mesh we assume there is at lease one interesting node by default
								int32 InterestingNodeCount = 1;
								if (importAssetTypeMMD == E_MMD_TO_UE4_SKELTON)
								{
									#ifdef DEBUG_MMD_PLUGIN_SKELTON
									InterestingNodeCount = 1;//test ? not Anime?
									#endif
								}
								else if (importAssetTypeMMD == E_MMD_TO_UE4_STATICMESH)
								{
								}

								if (InterestingNodeCount > 1)
								{
									// the option only works when there are only one asset
									// ImportOptions->bUsedAsFullName = false;
								}
								UFactory::CurrentFilename = file;
								FString Filename(UFactory::CurrentFilename);
								
								UE_LOG(LogTemp, Warning, TEXT("!!!PMX Import :%s"), *Filename);
								if (InterestingNodeCount > 0)
								{
									//int32 ImportedMeshCount = 0; //UE5.8
									//UStaticMesh* NewStaticMesh = nullptr; //UE5.8

									if (importAssetTypeMMD == E_MMD_TO_UE4_SKELTON)// skeletal mesh
									{
#ifdef DEBUG_MMD_PLUGIN_SKELTON
										int32 TotalNumNodes = 0;
										//for (int32 i = 0; i < SkelMeshArray.Num(); i++)
										for (int32 i = 0; i < 1; i++)
										{
											int32 LODIndex = 0;
											#ifdef DEBUG_MMD_UE4_ORIGINAL_CODE
											#else
											// for MMD?
											//int32 MaxLODLevel = 1; //UE5.8
											FSkeletalMeshImportData smid;
											USkeletalMesh* NewMesh = nullptr;
											if (LODIndex == 0)
											{
												FName OutputName = FName(*FString::Printf(TEXT("%s"), *filepath));// FbxImporter->MakeNameForMesh(Name.ToString(), SkelMeshNodeArray[0]);

												NewMesh = ImportSkeletalMesh(
													InParent,
													&pmxMeshInfoPtr,//SkelMeshNodeArray,
													OutputName,
													RF_Public | RF_Standalone |  RF_MarkAsNative | RF_Transactional,
													FPaths::GetBaseFilename(Filename),
													&smid,// test for MMD,
													true
												);
												NewObject = NewMesh;
											}

											// import morph target
											if (NewMesh && ImportUI->SkeletalMeshImportData->bImportMorphTargets)
											{
												UE_LOG(LogTemp, Warning, TEXT("!!!PMX Import :bImportMorphTargets"));
												// Disable material importing when importing morph targets
												uint32 bImportMaterials = ImportOptions->bImportMaterials;
												ImportOptions->bImportMaterials = false; //UE5.8 build报错

												ImportFbxMorphTarget(
													//SkelMeshNodeArray, 
													pmxMeshInfoPtr,
													NewMesh,
													InParent,
													Filename,
													LODIndex, smid
												);

												ImportOptions->bImportMaterials = !!bImportMaterials;
											}

											//add self
											if (NewObject)
											{
												//MMD Extend asset
												CreateMMDExtendFromMMDModel(
													InParent,
													//FName(*NewObject->GetName()),
													Cast<USkeletalMesh>(NewObject),
													&pmxMeshInfoPtr
												);

											}

											//end phese
											if (NewObject)
											{
												int32 NodeIndex = 0;
												TotalNumNodes++;
												NodeIndex++;
												FFormatNamedArguments Args;
												Args.Add(TEXT("NodeIndex"), NodeIndex);
												Args.Add(TEXT("ArrayLength"), 1);// SkelMeshArray.Num());
												GWarn->StatusUpdate(NodeIndex, 1, FText::Format(NSLOCTEXT("UnrealEd", "Importingf", "Importing ({NodeIndex} of {ArrayLength})"), Args));
											}
										//	MarkPackageDirty();
										}
#endif
										// if total nodes we found is 0, we didn't find anything. 
										if (TotalNumNodes == 0)
										{
											AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("FailedToImport_NoMeshFoundOnRoot", "Could not find any valid mesh on the root hierarchy. If you have mesh in the sub hierarchy, please enable option of [Import Meshes In Bone Hierarchy] when import.")),
												"FFbxErrors::SkeletalMesh_NoMeshFoundOnRoot");
										}
#endif
									}
								}
								else
								{

									if (importAssetTypeMMD == E_MMD_TO_UE4_SKELTON)
									{
										AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("FailedToImport_InvalidBone", "Failed to find any bone hierarchy. Try disabling the \"Import As Skeletal\" option to import as a rigid mesh. ")), "FFbxErrors::SkeletalMesh_InvalidBone");
									}
									else
									{
										AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("FailedToImport_InvalidNode", "Could not find any node.")), "FFbxErrors::SkeletalMesh_InvalidNode");
									}
								}
							}

						if (NewObject == nullptr)
						{
							AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("FailedToImport_NoObject", "Import failed.")), "FFbxErrors::Generic_ImportingNewObjectFailed");
							//Warn->Log(ELogVerbosity::Error, "PMX Import ERR [NewObject is nullptr]...FLT");
							return false;
						} //UE5.8 build报错

						//InParent->MarkPackageDirty(); //变量表达式未使用
						return true;
						/*FbxImporter->ReleaseScene();
						Warn->EndSlowTask();*/
					}
					else {
						UE_LOG(LogTemp, Error, TEXT("!!!PMX Import error:: PMXLoaderBinary."));
					}
				}
				else {
					UE_LOG(LogTemp, Error, TEXT("!!!PMX Import error:: LoadFileToArray."));
				}
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("!!!PMX Import error:: FIle is not exist."));
			}

		}
		else {
			UE_LOG(LogTemp, Error, TEXT("!!!PMX Import error::filepath type error."));

		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("!!!PMX Import error::filepath error.%d,%s"), indexs, *filepath);
	}

	return false;
}

bool UPmxFactory::ImportPmxFromFile(FString(file)) {
	
	auto Factory = NewObject<UPmxFactory>();
	return Factory->FImportPmxFromFile(file);
	
}
UObject* UPmxFactory::FactoryCreateBinary
	(
	UClass*			Class,
	UObject*			InParent,
	FName				Name,
	EObjectFlags		Flags,
	UObject*			Context,
	const TCHAR*		Type,
	const uint8*&		Buffer,
	const uint8*		BufferEnd,
	FFeedbackContext*	Warn,
	bool&				bOutOperationCanceled
	)
{
	
	// MMD Default 
	importAssetTypeMMD = E_MMD_TO_UE4_SKELTON;


	if (bOperationCanceled)
	{
		bOutOperationCanceled = true;
		
		GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.Broadcast(this, nullptr);
		return nullptr;
	}

	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPreImport.Broadcast(this, Class, InParent, Name, Type);

	UObject* NewObject = nullptr;

	FPmxImporter* PmxImporter = FPmxImporter::GetInstance();

	EPMXImportType ForcedImportType = PMXIT_StaticMesh;

	// For multiple files, use the same settings
	bDetectImportTypeOnImport = false;

	//judge MMD format(pmx or pmd)
	bool bIsPmxFormat = false;
	if (FString(Type).Equals(TEXT("pmx"), ESearchCase::IgnoreCase))
	{
		//Is PMX format 
		bIsPmxFormat = true;
	}
	//Load MMD Model From binary File
	MMD4UE4::PmxMeshInfo pmxMeshInfoPtr;
	pmxMaterialImportHelper.InitializeBaseValue(InParent);
	bool pmxImportResult = false;
	if (bIsPmxFormat)
	{
		//pmx ver
		pmxImportResult = pmxMeshInfoPtr.PMXLoaderBinary(Buffer, BufferEnd);
	}
	else
	{
		//pmd ver
		MMD4UE4::PmdMeshInfo PmdMeshInfo;
		if (PmdMeshInfo.PMDLoaderBinary(Buffer, BufferEnd))
		{
			//Up convert From PMD to PMX format gfor ue4
			pmxImportResult = PmdMeshInfo.ConvertToPmxFormat(&pmxMeshInfoPtr);
		}
	}
	if (!pmxImportResult)
	{
		// Log the error message and fail the import.
		Warn->Log(ELogVerbosity::Error, "PMX Import ERR...FLT");
		GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.Broadcast(this, nullptr);
		return nullptr;
	}
	else
	{
		//读取模型后的警告文显示：注释栏
		FText TitleStr = FText::Format(LOCTEXT("ImportReadMe_Generic", "{0}"), FText::FromString("tilesss"));
		const FText Message
			= FText::Format(LOCTEXT("ImportReadMe_Generic_Msg",
			"Important!! \nReadMe Lisence \n modele Name:\n'{0}'\n \n Model Comment JP:\n'{1}'"),
			FText::FromString(pmxMeshInfoPtr.modelNameJP), FText::FromString(pmxMeshInfoPtr.modelCommentJP));
		/*
		if (EAppReturnType::Ok != FMessageDialog::Open(EAppMsgType::OkCancel, Message))
		{
			GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.Broadcast(this, nullptr);
			return nullptr;
		}
		*/
		TitleStr = FText::Format(LOCTEXT("ImportReadMe_Generic_Dbg", "{0} 制限事項"), FText::FromString("IM4U Plugin"));
		const FText MessageDbg
			= FText(LOCTEXT("ImportReadMe_Generic_Dbg_Msg",
			"下一个导入选项的Slate仍在实施中。\n\
			导入对象只能生成Skeleton。\n\
			当前有效的参数是显示的项目有效。") ); 
		//FMessageDialog::Open(EAppMsgType::Ok, MessageDbg, &TitleStr);
	}

	// show Import Option Slate
	bool bImportAll = false;
	ImportUI->bIsObjImport = true;//obj mode
	ImportUI->OriginalImportType = EPMXImportType::PMXIT_SkeletalMesh;
	ImportUI->bImportMaterials = true;
	ImportUI->bCreateMaterialInstMode = true;
	ImportUI->bCreatePhysicsAsset = true;
	ImportUI->bImportMorphTargets = true;
	ImportUI->bImportTextures = true;
	ImportUI->bUnlitMaterials = false;
	ImportUI->SkeletalMeshImportData->bImportMorphTargets = true;
	UE_LOG(LogTemp, Warning, TEXT("!!!PMX Import :%s"), *(InParent->GetPathName()));

	PMXImportOptions* ImportOptions
		= GetImportOptions(
		PmxImporter,
		ImportUI,
		true,//bShowImportDialog, 
		InParent->GetPathName(),
		bOperationCanceled,
		bImportAll,
		ImportUI->bIsObjImport,//bIsPmxFormat,
		bIsPmxFormat,
		ForcedImportType
		);
	if (bImportAll)
	{
		// If the user chose to import all, we don't show the dialog again and use the same settings for each object until importing another set of files
		//bShowOption = false;
	}

	if (ImportOptions)
	{
		Warn->BeginSlowTask(NSLOCTEXT("PmxFactory", "BeginImportingPmxMeshTask", "Importing Pmx mesh"), true);
#if 1
		{

			// For animation and static mesh we assume there is at lease one interesting node by default
			int32 InterestingNodeCount = 1;

			if (importAssetTypeMMD == E_MMD_TO_UE4_SKELTON)
			{
#ifdef DEBUG_MMD_PLUGIN_SKELTON

				InterestingNodeCount = 1;//test ? not Anime?

#endif
			}
			else if (importAssetTypeMMD == E_MMD_TO_UE4_STATICMESH)
			{

			}


			if (InterestingNodeCount > 1)
			{
				// the option only works when there are only one asset
//				ImportOptions->bUsedAsFullName = false;
			}

			const FString Filename(UFactory::CurrentFilename);
			if (/*RootNodeToImport &&*/ InterestingNodeCount > 0)
			{
				//int32 ImportedMeshCount = 0; //UE5.8
				//UStaticMesh* NewStaticMesh = nullptr; //UE5.8
				if (importAssetTypeMMD == E_MMD_TO_UE4_STATICMESH)  // static mesh
				{

				}
				else if (importAssetTypeMMD == E_MMD_TO_UE4_SKELTON)// skeletal mesh
				{
#ifdef DEBUG_MMD_PLUGIN_SKELTON
					int32 TotalNumNodes = 0;

					//for (int32 i = 0; i < SkelMeshArray.Num(); i++)
					for (int32 i = 0; i < 1/*SkelMeshArray.Num()*/; i++)
					{
						int32 LODIndex=0;
#ifdef DEBUG_MMD_UE4_ORIGINAL_CODE
					
#else
						// for MMD?
						//int32 MaxLODLevel = 1; //UE5.8
						FSkeletalMeshImportData smid;
						USkeletalMesh* NewMesh = nullptr;
						if (LODIndex == 0 /*&& SkelMeshNodeArray.Num() != 0*/)
						{
							FName OutputName = FName(*FString::Printf(TEXT("%s"), *Name.ToString()));// FbxImporter->MakeNameForMesh(Name.ToString(), SkelMeshNodeArray[0]);
							
							//UEnum* CompileModeEnum = GetStaticEnum <EObjectFlags>();
						
							UE_LOG(LogTemp, Warning, TEXT("!!!PMX Import :%s"), *OutputName.ToString());
							
							NewMesh = ImportSkeletalMesh(
								InParent,
								&pmxMeshInfoPtr,//SkelMeshNodeArray,
								OutputName,
								Flags,
								//ImportUI->SkeletalMeshImportData,
								FPaths::GetBaseFilename(Filename),
								&smid,// test for MMD,
								true
								);
							NewObject = NewMesh;
						}

						// import morph target
						if (NewMesh && ImportUI->SkeletalMeshImportData->bImportMorphTargets)
						{
							// Disable material importing when importing morph targets
							uint32 bImportMaterials = ImportOptions->bImportMaterials;
							ImportOptions->bImportMaterials = false;

							ImportFbxMorphTarget(
								//SkelMeshNodeArray, 
								pmxMeshInfoPtr,
								NewMesh,
								InParent, 
								Filename, 
								LODIndex, smid
								);

							ImportOptions->bImportMaterials = !!bImportMaterials;
						}

						//add self
						if (NewObject)
						{
							//MMD Extend asset
							CreateMMDExtendFromMMDModel(
								InParent,
								//FName(*NewObject->GetName()),
								Cast<USkeletalMesh>(NewObject),
								&pmxMeshInfoPtr
								);
						}

						//end phese
						if (NewObject)
						{
							int32 NodeIndex = 0;
							TotalNumNodes++;
							NodeIndex++;
							FFormatNamedArguments Args;
							Args.Add(TEXT("NodeIndex"), NodeIndex);
							Args.Add(TEXT("ArrayLength"), 1);// SkelMeshArray.Num());
							GWarn->StatusUpdate(NodeIndex, 1/*SkelMeshArray.Num()*/, FText::Format(NSLOCTEXT("UnrealEd", "Importingf", "Importing ({NodeIndex} of {ArrayLength})"), Args));
						}
#endif
					}

					// if total nodes we found is 0, we didn't find anything. 
					if (TotalNumNodes == 0)
					{
						AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("FailedToImport_NoMeshFoundOnRoot", "Could not find any valid mesh on the root hierarchy. If you have mesh in the sub hierarchy, please enable option of [Import Meshes In Bone Hierarchy] when import.")),
							"FFbxErrors::SkeletalMesh_NoMeshFoundOnRoot");
					}
#endif
				}
			}
			else
			{
#if 1//DEBUG_MMD_UE4_ORIGINAL_CODE
				if (importAssetTypeMMD == E_MMD_TO_UE4_SKELTON)
				{
					AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("FailedToImport_InvalidBone", "Failed to find any bone hierarchy. Try disabling the \"Import As Skeletal\" option to import as a rigid mesh. ")),"FFbxErrors::SkeletalMesh_InvalidBone");
				}
				else
				{
					AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("FailedToImport_InvalidNode", "Could not find any node.")),"FFbxErrors::SkeletalMesh_InvalidNode");
				}
#endif
			}
		}

		if (NewObject == nullptr)
		{
			AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("FailedToImport_NoObject", "Import failed.")),"FFbxErrors::Generic_ImportingNewObjectFailed");
			Warn->Log(ELogVerbosity::Error, "PMX Import ERR [NewObject is nullptr]...FLT");
		}

		//FbxImporter->ReleaseScene();
#endif
		Warn->EndSlowTask();
	}
	else
	{
		const FText Message 
			= FText::Format(LOCTEXT("ImportFailed_Generic", 
				"Failed to import '{0}'. Failed to create asset '{1}'\nMMD停止模型导入。\nIM4U Plugin"),
				FText::FromString(*Name.ToString()), FText::FromString(*Name.ToString()));
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		//UE_LOG(LogAssetTools, Warning, TEXT("%s"), *Message.ToString());
	}

	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.Broadcast(this, NewObject);

	return NewObject;
}

//////////////////////////////////////////////////////////////////////
USkeletalMesh* UPmxFactory::ImportSkeletalMesh(
	UObject* InParent,
	MMD4UE4::PmxMeshInfo *pmxMeshInfoPtr,
	const FName& Name,
	EObjectFlags Flags,
	//UFbxSkeletalMeshImportData* TemplateImportData,
	FString Filename,
	//TArray<FbxShape*> *FbxShapeArray,
	FSkeletalMeshImportData* OutData,
	bool bCreateRenderData
	)
{
	bool bDiffPose;
	struct ExistingSkelMeshData* ExistSkelMeshDataPtr = nullptr;
	//int32 SkelType = 0; // 0 for skeletal mesh, 1 for rigid mesh //UE5.8
	//bool bCreateRenderData = true;

	if (true /*!FbxShapeArray*/)
	{
		//UObject* ExistingObject = StaticFindObjectFast(UObject::StaticClass(), InParent, *Name.ToString(), false, false, RF_PendingKill);//~UE4.10
		#if UE_VERSION_OLDER_THAN(5,2,0)
		UObject* ExistingObject = StaticFindObjectFast(UObject::StaticClass(), InParent, *Name.ToString(), false, false, EObjectFlags::RF_Standalone, EInternalObjectFlags::LoaderImport);//UE4.11~
		#else  
		//UObject* ExistingObject = StaticFindObjectFast(UObject::StaticClass(), InParent, *Name.ToString(), false, EObjectFlags::RF_Standalone, EInternalObjectFlags::LoaderImport);
		//UObject* ExistingObject = StaticFindObjectFast(UObject::StaticClass(), InParent, *Name.ToString(), EFindObjectFlags::None, EObjectFlags::RF_Standalone, EInternalObjectFlags::LoaderImport);
		UObject* ExistingObject = StaticFindObjectFast(UClass::StaticClass(), InParent, *Name.ToString(), EFindObjectFlags::None, EObjectFlags::RF_Standalone, EInternalObjectFlags::LoaderImport);//UE5.8 //UObjectGlobals.h //TextProperty.h
		#endif
		USkeletalMesh* ExistingSkelMesh = Cast<USkeletalMesh>(ExistingObject);

		if (ExistingSkelMesh)
		{
			ExistingSkelMesh->PreEditChange(nullptr);
			//ExistSkelMeshDataPtr = SaveExistingSkelMeshData(ExistingSkelMesh);
		}
		// if any other object exists, we can't import with this name
		else if (ExistingObject)
		{
			AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Warning, FText::Format(LOCTEXT("FbxSkeletaLMeshimport_OverlappingName", "Same name but different class: '{0}' already exists"), FText::FromString(ExistingObject->GetName()))),"FFbxErrors::Generic_SameNameAssetExists");
			return nullptr;
		}
	}

	// [from USkeletalMeshFactory::FactoryCreateBinary]
	USkeletalMesh* SkeletalMesh = NewObject<USkeletalMesh>(InParent, Name, Flags);

	SkeletalMesh->PreEditChange(nullptr);

	FSkeletalMeshImportData TempData;
	// Fill with data from buffer - contains the full .FBX file.
	FSkeletalMeshImportData* SkelMeshImportDataPtr = &TempData;
	if (OutData)
	{
		SkelMeshImportDataPtr = OutData;
	}

	/*Import Bone Start*/
	bool bUseTime0AsRefPose = false;// ImportOptions->bUseT0AsRefPose;
	// Note: importing morph data causes additional passes through this function, so disable the warning dialogs
	// from popping up again on each additional pass.  
	if (
		!ImportBone(
			//NodeArray, 
			pmxMeshInfoPtr,
			*SkelMeshImportDataPtr,
			//TemplateImportData, 
			//SortedLinkArray,
			bDiffPose, 
			false,//(FbxShapeArray != nullptr),
			bUseTime0AsRefPose)
			
			/*false*/
		)
	{
		AddTokenizedErrorMessage(FTokenizedMessage::Create(
			EMessageSeverity::Error, 
			LOCTEXT("FbxSkeletaLMeshimport_MultipleRootFound", "Multiple roots found")),"FFbxErrors::SkeletalMesh_MultipleRoots");
		// I can't delete object here since this is middle of import
		// but I can move to transient package, and GC will automatically collect it
		SkeletalMesh->ClearFlags(RF_Standalone);
		SkeletalMesh->Rename(nullptr, GetTransientPackage());
		UE_LOG(LogMMD4UE4_PMXFactory, Warning, TEXT("!!!ImportBone_out"));
		return nullptr;
	}

	for (int32 MaterialIndex = 0; MaterialIndex < pmxMeshInfoPtr->materialList.Num(); ++MaterialIndex)
	{
    	SkeletalMeshImportData::FMaterial NewMaterial;
    	SkelMeshImportDataPtr->Materials.Add(NewMaterial);
	}

	if (!FillSkelMeshImporterFromFbx(*SkelMeshImportDataPtr,pmxMeshInfoPtr,InParent))
	{
		// I can't delete object here since this is middle of import
		// but I can move to transient package, and GC will automatically collect it
		SkeletalMesh->ClearFlags(RF_Standalone);
		SkeletalMesh->Rename(nullptr, GetTransientPackage());
		UE_LOG(LogMMD4UE4_PMXFactory, Warning, TEXT("!!!FillSkelMeshImporterFromFbx_out"));
		return nullptr;
	}
	else
	{
		SkelMeshImportDataPtr->PointToRawMap.AddUninitialized(SkelMeshImportDataPtr->Points.Num());
		for (int32 PointIdx = 0; PointIdx<SkelMeshImportDataPtr->Points.Num(); PointIdx++)
		{
			SkelMeshImportDataPtr->PointToRawMap[PointIdx] = PointIdx;
		}
	}

	// process materials from import data
	ProcessImportMeshMaterials(SkeletalMesh->GetMaterials(), *SkelMeshImportDataPtr);

	// process reference skeleton from import data
	int32 SkeletalDepth = 0;
	if (!ProcessImportMeshSkeleton(SkeletalMesh->GetSkeleton(), SkeletalMesh->GetRefSkeleton(), SkeletalDepth, *SkelMeshImportDataPtr))
	{
		SkeletalMesh->ClearFlags(RF_Standalone);
		SkeletalMesh->Rename(nullptr, GetTransientPackage());
		return nullptr;
	}
	UE_LOG(LogMMD4UE4_PMXFactory, Warning, TEXT("Bones digested - %i  Depth of hierarchy - %i"), SkeletalMesh->GetRefSkeleton().GetNum(), SkeletalDepth);

	// process bone influences from import data
	SkeletalMeshImportUtils::ProcessImportMeshInfluences(*SkelMeshImportDataPtr,L"MMDMeshName");
	if (SkelMeshImportDataPtr->Points.IsEmpty() ||
		SkelMeshImportDataPtr->Wedges.IsEmpty() ||
		SkelMeshImportDataPtr->Faces.IsEmpty() ||
		SkelMeshImportDataPtr->Influences.IsEmpty() ||
		SkeletalMesh->GetRefSkeleton().GetNum() == 0)
	{
		UE_LOG(LogMMD4UE4_PMXFactory, Error,
			TEXT("PMX mesh build aborted: invalid import data. Points=%d Wedges=%d Faces=%d Influences=%d Bones=%d"),
			SkelMeshImportDataPtr->Points.Num(),
			SkelMeshImportDataPtr->Wedges.Num(),
			SkelMeshImportDataPtr->Faces.Num(),
			SkelMeshImportDataPtr->Influences.Num(),
			SkeletalMesh->GetRefSkeleton().GetNum());
		SkeletalMesh->ClearFlags(RF_Standalone);
		SkeletalMesh->Rename(nullptr, GetTransientPackage());
		return nullptr;
	}

	SkeletalMesh->PreEditChange(nullptr);
	//Dirty the DDC Key for any imported Skeletal Mesh
	SkeletalMesh->InvalidateDeriveDataCacheGUID();

	FSkeletalMeshModel* ImportedResource = SkeletalMesh->GetImportedModel();
	check(ImportedResource->LODModels.Num() == 0);
	ImportedResource->LODModels.Empty();
	ImportedResource->LODModels.Add(new FSkeletalMeshLODModel());
	constexpr int32 ImportLODModelIndex = 0;
	FSkeletalMeshLODModel& LODModel = ImportedResource->LODModels[ImportLODModelIndex];
	#if UE_VERSION_OLDER_THAN(5,4,0)
	SkeletalMesh->SaveLODImportedData(0, *SkelMeshImportDataPtr);
	#else
	/*#include "MeshDescription.h"
	FMeshDescription MeshDesc;
	Params.LODIndex = 0;
	Params.MeshDescription = &MeshDesc; // MeshDesc 必须已声明并填充
	SkeletalMesh->CommitMeshDescription(Params);
	SkeletalMesh->SaveLODImportedData(0, *SkelMeshImportDataPtr);*/

	// 构造参数结构体
	/*USkeletalMesh::FCommitMeshDescriptionParams MeshDesc; //堆栈错误 01 //Name=Variable is not available
	bool bCommitMeshDescription = SkeletalMesh->CommitMeshDescription(0, MeshDesc);*/

	//SkeletalMesh->PostEditChange();
	//SkeletalMesh->PostEditChange(nullptr);
	/*class USkeletalMesh : public USkinnedAsset, public IInterface_CollisionDataProvider, public IInterface_AssetUserData, public INodeMappingProviderInterface
	SkeletalMesh->CommitMeshDescription(0);
	SkeletalMesh->SaveLODImportedData(0, *SkelMeshImportDataPtr);
	//ENGINE_API void SaveLODImportedData(const int32 LODIndex, const FSkeletalMeshImportData& InMesh);
	//ENGINE_API bool CommitMeshDescription(const int32 InLODIndex, const FCommitMeshDescriptionParams& InParams = {});*/

	#endif

	{
		//We reimport both
		#if UE_VERSION_OLDER_THAN(5,4,0)
		SkeletalMesh->SetLODImportedDataVersions(ImportLODModelIndex, ESkeletalMeshGeoImportVersions::LatestVersion, ESkeletalMeshSkinningImportVersions::LatestVersion);
		#else
		//SkeletalMesh->SetLODImportedDataVersions(ImportLODModelIndex, ESkeletalMeshGeoImportVersions::LatestVersion, ESkeletalMeshSkinningImportVersions::LatestVersion);
		#endif
	}
	USkeletalMesh* Mesh = SkeletalMesh;
	//Mesh->ResetLODInfo(); //堆栈错误
	Mesh->SetNumSourceModels(0);
	FSkeletalMeshLODInfo & NewLODInfo = SkeletalMesh->AddLODInfo();
	NewLODInfo.ReductionSettings.NumOfTrianglesPercentage = 1.0f;
	NewLODInfo.ReductionSettings.NumOfVertPercentage = 1.0f;
	NewLODInfo.ReductionSettings.MaxDeviationPercentage = 0.0f;
	NewLODInfo.LODHysteresis = 0.02f;

	// Create initial bounding box based on expanded version of reference pose for meshes without physics assets. Can be overridden by artist.
	FBox3f BoundingBox(SkelMeshImportDataPtr->Points.GetData(), SkelMeshImportDataPtr->Points.Num());
	FBox3f Temp = BoundingBox;
	FVector3f MidMesh = 0.5f*(Temp.Min + Temp.Max);
	BoundingBox.Min = Temp.Min + 1.0f*(Temp.Min - MidMesh);
	BoundingBox.Max = Temp.Max + 1.0f*(Temp.Max - MidMesh);
	// Tuck up the bottom as this rarely extends lower than a reference pose's (e.g. having its feet on the floor).
	// Maya has Y in the vertical, other packages have Z.
	//BEN const int32 CoordToTuck = bAssumeMayaCoordinates ? 1 : 2;
	//BEN BoundingBox.Min[CoordToTuck]	= Temp.Min[CoordToTuck] + 0.1f*(Temp.Min[CoordToTuck] - MidMesh[CoordToTuck]);

	BoundingBox.Min[2] = Temp.Min[2] + 0.1f*(Temp.Min[2] - MidMesh[2]);
	SkeletalMesh->SetImportedBounds( FBoxSphereBounds(FBox(BoundingBox)) );

	// Store whether or not this mesh has vertex colors
	SkeletalMesh->SetHasVertexColors(SkelMeshImportDataPtr->bHasVertexColors);
	SkeletalMesh->SetVertexColorGuid(SkeletalMesh->GetHasVertexColors() ? FGuid::NewGuid() : FGuid());
	// Release the static mesh's resources.
	SkeletalMesh->ReleaseResources();

	// Flush the resource release commands to the rendering thread to ensure that the build doesn't occur while a resource is still
	// allocated, and potentially accessing the UStaticMesh.
	// Pass the number of texture coordinate sets to the LODModel.  Ensure there is at least one UV coord
	SkeletalMesh->ReleaseResourcesFence.Wait();
	LODModel.NumTexCoords = FMath::Max<uint32>(1, SkelMeshImportDataPtr->NumTexCoords);
#if 1
	if (bCreateRenderData)
	{
		TArray<FVector3f> LODPoints;
		TArray<SkeletalMeshImportData::FMeshWedge> LODWedges;
		TArray<SkeletalMeshImportData::FMeshFace> LODFaces;
		TArray<SkeletalMeshImportData::FVertInfluence> LODInfluences;
		TArray<int32> LODPointToRawMap;
		SkelMeshImportDataPtr->CopyLODImportData(LODPoints, LODWedges, LODFaces, LODInfluences, LODPointToRawMap);
		if (LODPoints.IsEmpty() ||
			LODWedges.IsEmpty() ||
			LODFaces.IsEmpty() ||
			LODInfluences.IsEmpty() ||
			LODPointToRawMap.Num() != LODPoints.Num())
		{
			UE_LOG(LogMMD4UE4_PMXFactory, Error,
				TEXT("PMX skeletal mesh build aborted: invalid LOD data. Points=%d Wedges=%d Faces=%d Influences=%d PointToRawMap=%d"),
				LODPoints.Num(),
				LODWedges.Num(),
				LODFaces.Num(),
				LODInfluences.Num(),
				LODPointToRawMap.Num());
			SkeletalMesh->ClearFlags(RF_Standalone);
			SkeletalMesh->Rename(nullptr, GetTransientPackage());
			return nullptr;
		}

		//const bool bShouldComputeNormals = true/*!ImportOptions->ShouldImportNormals()*/ || !SkelMeshImportDataPtr->bHasNormals;
		//const bool bShouldComputeTangents = true/*!ImportOptions->ShouldImportTangents()*/ || !SkelMeshImportDataPtr->bHasTangents;

		IMeshUtilities& MeshUtilities = FModuleManager::Get().LoadModuleChecked<IMeshUtilities>("MeshUtilities");

		TArray<FText> WarningMessages;
		TArray<FName> WarningNames;
		// Create actual rendering data.

		if (!MeshUtilities.BuildSkeletalMesh(
			ImportedResource->LODModels[0],"MMDMeshName",
			SkeletalMesh->GetRefSkeleton(),
			LODInfluences,
			LODWedges,
			LODFaces,
			LODPoints,
			LODPointToRawMap)
			)
		{
			if (WarningNames.Num() == WarningMessages.Num())
			{
				// temporary hack of message/names, should be one token or a struct
				for (int32 MessageIdx = 0; MessageIdx<WarningMessages.Num(); ++MessageIdx)
				{
					AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, WarningMessages[MessageIdx]), WarningNames[MessageIdx]);
				}
			}

			SkeletalMesh->MarkAsGarbage();
			return nullptr;
		}
		else if (WarningMessages.Num() > 0)
		{
			// temporary hack of message/names, should be one token or a struct
			if (WarningNames.Num() == WarningMessages.Num())
			{
				// temporary hack of message/names, should be one token or a struct
				for (int32 MessageIdx = 0; MessageIdx<WarningMessages.Num(); ++MessageIdx)
				{
					AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Warning, WarningMessages[MessageIdx]), WarningNames[MessageIdx]);
				}
			}
		}

		// Presize the per-section shadow casting array with the number of sections in the imported LOD.
		const int32 NumSections = LODModel.Sections.Num();
		for (int32 SectionIndex = 0; SectionIndex < NumSections; ++SectionIndex)
		{
			SkeletalMesh->GetLODInfo(0)->LODMaterialMap.Add(SectionIndex);
		}

		if (ExistSkelMeshDataPtr)
		{
			//RestoreExistingSkelMeshData(ExistSkelMeshDataPtr, SkeletalMesh);
		}

		// Store the current file path and timestamp for re-import purposes

		SkeletalMesh->GetAssetImportData()->Update(UFactory::CurrentFilename);
		
		SkeletalMesh->CalculateInvRefMatrices();
		SkeletalMesh->PostEditChange();
		//SkeletalMesh->MarkPackageDirty(); //表达式未使用
		
		// Now iterate over all skeletal mesh components re-initialising them.
		for (TObjectIterator<USkeletalMeshComponent> It; It; ++It)
		{
			USkeletalMeshComponent* SkelComp = *It;
			#if UE_VERSION_OLDER_THAN(5,2,0)
			if (SkelComp->SkeletalMesh == SkeletalMesh)
			#else  
			if (SkelComp->GetSkeletalMeshAsset() == SkeletalMesh)
			#endif
			{
				FComponentReregisterContext ReregisterContext(SkelComp);
			}
		}
	}
#endif

	if (InParent != GetTransientPackage())
	{
		// see if we have skeleton set up
		// if creating skeleton, create skeleeton
		USkeleton* Skeleton = nullptr;
		bool bSkeletonCreated = false;
		//Skeleton = ImportOptions->SkeletonForAnimation;
		if (Skeleton == nullptr)
		{
			FString ObjectName = FString::Printf(TEXT("%s_Skeleton"), *SkeletalMesh->GetName());
			const FString AssetDirectory = GetMeshAssetDirectory(SkeletalMesh);
			Skeleton = CreateAsset<USkeleton>(
				AssetDirectory, ObjectName, true, &bSkeletonCreated);
			if (!Skeleton)
			{
				// same object exists, try to see if it's skeleton, if so, load
				const FString ObjectPath = FString::Printf(
					TEXT("%s.%s"),
					*(AssetDirectory / ObjectName),
					*ObjectName);
				Skeleton = LoadObject<USkeleton>(nullptr, *ObjectPath);

				// if not skeleton, we're done, we can't create skeleton with same name
				// @todo in the future, we'll allow them to rename
				if (!Skeleton)
				{
					AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, FText::Format(LOCTEXT("FbxSkeletaLMeshimport_SkeletonRecreateError", "'{0}' already exists. It fails to recreate it."), FText::FromString(ObjectName))),"FFbxErrors::SkeletalMesh_SkeletonRecreateError");
					return SkeletalMesh;
				}
			}
		}
		
		// merge bones to the selected skeleton
		if ( !Skeleton->MergeAllBonesToBoneTree( SkeletalMesh ) )
		{
			if (EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo,
				LOCTEXT("SkeletonFailed_BoneMerge", "FAILED TO MERGE BONES:\n\n This could happen if significant hierarchical change has been made\n - i.e. inserting bone between nodes\n Would you like to regenerate Skeleton from this mesh? \n\n ***WARNING: THIS WILL REQUIRE RECOMPRESS ALL ANIMATION DATA AND POTENTIALLY INVALIDATE***\n")))
			{
				if (Skeleton->RecreateBoneTree(SkeletalMesh))
				{
					FAssetNotifications::SkeletonNeedsToBeSaved(Skeleton);
				}
			}
		}
		else
		{
			/*
			// ask if they'd like to update their position form this mesh
			if ( ImportOptions->SkeletonForAnimation && ImportOptions->bUpdateSkeletonReferencePose ) 
			{
				Skeleton->UpdateReferencePoseFromMesh(SkeletalMesh);
				FAssetNotifications::SkeletonNeedsToBeSaved(Skeleton);
			}*/
		}

			SkeletalMesh->SetSkeleton(Skeleton);
			//SkeletalMesh->MarkPackageDirty(); //表达式未使用
			SkeletalMesh->PostEditChange();
			if (bSkeletonCreated)
			{
				FAssetRegistryModule::AssetCreated(Skeleton);
			}

			// Create PhysicsAsset here: FPhysicsAssetUtils::CreateFromSkeletalMesh
			// requires the mesh to already have a valid skeleton.
			if (ImportUI->bCreatePhysicsAsset && SkeletalMesh->GetPhysicsAsset() == nullptr)
			{
				const FString PhysicsObjectName =
					FString::Printf(TEXT("%s_PhysicsAsset"), *SkeletalMesh->GetName());
				const FString PhysicsAssetDirectory = GetMeshAssetDirectory(SkeletalMesh);
				bool bPhysicsAssetCreated = false;
				UPhysicsAsset* NewPhysicsAsset = CreateAsset<UPhysicsAsset>(
					PhysicsAssetDirectory, PhysicsObjectName, true, &bPhysicsAssetCreated);
				if (!NewPhysicsAsset)
				{
					AddTokenizedErrorMessage(
						FTokenizedMessage::Create(
							EMessageSeverity::Warning,
							FText::Format(
								LOCTEXT("FbxSkeletaLMeshimport_CouldNotCreatePhysicsAsset",
									"Could not create Physics Asset ('{0}') for '{1}'"),
								FText::FromString(PhysicsObjectName),
								FText::FromString(SkeletalMesh->GetName()))),
						"FFbxErrors::SkeletalMesh_FailedToCreatePhyscisAsset");
				}
				else
				{
					FPhysAssetCreateParams NewBodyData;
					NewBodyData.bDisableCollisionsByDefault = true;
					NewBodyData.MinBoneSize = 2;
					FText CreationErrorMessage;
					if (!FPhysicsAssetUtils::CreateFromSkeletalMesh(
							NewPhysicsAsset, SkeletalMesh, NewBodyData, CreationErrorMessage))
					{
						AddTokenizedErrorMessage(
							FTokenizedMessage::Create(EMessageSeverity::Warning, CreationErrorMessage),
							"FFbxErrors::SkeletalMesh_FailedToCreatePhyscisAsset");
						TArray<UObject*> ObjectsToDelete;
						ObjectsToDelete.Add(NewPhysicsAsset);
						ObjectTools::DeleteObjects(ObjectsToDelete, false);
						NewPhysicsAsset = nullptr;
					}

					if (NewPhysicsAsset)
					{
						BuildMmdRigidBodies(NewPhysicsAsset, SkeletalMesh, pmxMeshInfoPtr);
						NewPhysicsAsset->MarkPackageDirty();
						NewPhysicsAsset->PreEditChange(nullptr);
						NewPhysicsAsset->PostEditChange();
						SkeletalMesh->SetPhysicsAsset(NewPhysicsAsset);
						SkeletalMesh->MarkPackageDirty();
						if (bPhysicsAssetCreated)
						{
							FAssetRegistryModule::AssetCreated(NewPhysicsAsset);
						}
					}
				}
			}
		
	}
	return SkeletalMesh;
}

void UPmxFactory::BuildMmdRigidBodies(
	UPhysicsAsset* PhysicsAsset,
	USkeletalMesh* SkeletalMesh,
	MMD4UE4::PmxMeshInfo* PmxMeshInfo)
{
	if (!PhysicsAsset || !SkeletalMesh || !PmxMeshInfo)
	{
		return;
	}

	const int32 BodySetupCount = PhysicsAsset->SkeletalBodySetups.Num();
	for (int32 BodyIndex = 0; BodyIndex < BodySetupCount; ++BodyIndex)
	{
		USkeletalBodySetup* BodySetup = PhysicsAsset->SkeletalBodySetups[BodyIndex].Get();
		if (!BodySetup)
		{
			continue;
		}

		FKAggregateGeom& AggGeom = BodySetup->AggGeom;
		AggGeom.EmptyElements();

		const TArray<MMD4UE4::PMX_RIGIDBODY> RigidBodies = PmxMeshInfo->findRigid(BodySetup->BoneName);
		if (RigidBodies.Num() < 1)
		{
			FKBoxElem Placeholder(0.5, 0.5, 1);
			Placeholder.SetContributeToMass(false);
			AggGeom.BoxElems.Add(Placeholder);
			BodySetup->PhysicsType = PhysType_Kinematic;
			BodySetup->CollisionReponse = EBodyCollisionResponse::BodyCollision_Disabled;
			continue;
		}

		bool bHasDynamicBody = false;
		float TotalLinearDamping = 0.0f;
		float TotalAngularDamping = 0.0f;
		int32 DynamicBodyCount = 0;

		for (const MMD4UE4::PMX_RIGIDBODY& RigidBody : RigidBodies)
		{
			// OpType: 0=跟随骨骼(Kinematic), 1/2=物理演算(Dynamic)
			const bool bRigidBodyDynamic = RigidBody.OpType != 0;
			if (bRigidBodyDynamic)
			{
				bHasDynamicBody = true;
				++DynamicBodyCount;
				// MMD的“移动/回転減衰”(0~1)是每帧残留比例，映射到UE阻尼时需换算
				TotalLinearDamping += FMath::Clamp(RigidBody.PosDim, 0.0f, 1.0f);
				TotalAngularDamping += FMath::Clamp(RigidBody.RotDim, 0.0f, 1.0f);
			}

			FTransform ElementTransform;
			ElementTransform.SetLocation(FVector(RigidBody.Position));
			ElementTransform.SetRotation(RigidBody.Quat);

			if (RigidBody.ShapeType == 0)
			{
				FKSphereElem Sphere(RigidBody.Size.X);
				Sphere.SetTransform(ElementTransform);
				Sphere.SetContributeToMass(bRigidBodyDynamic);
				AggGeom.SphereElems.Add(Sphere);
			}
			else if (RigidBody.ShapeType == 1)
			{
				FKBoxElem Box(RigidBody.Size.X, RigidBody.Size.Z, RigidBody.Size.Y);
				Box.SetTransform(ElementTransform);
				Box.SetContributeToMass(bRigidBodyDynamic);
				AggGeom.BoxElems.Add(Box);
			}
			else if (RigidBody.ShapeType == 2)
			{
				FKSphylElem Capsule(RigidBody.Size.X, RigidBody.Size.Y);
				Capsule.SetTransform(ElementTransform);
				Capsule.SetContributeToMass(bRigidBodyDynamic);
				AggGeom.SphylElems.Add(Capsule);
			}
		}

		if (bHasDynamicBody)
		{
			// PhysType_Simulated = "Do not follow owner, but simulate"，物理演算必须用它；
			// PhysType_Default 表示跟随骨骼动画，会把物理结果覆盖掉。
			BodySetup->PhysicsType = PhysType_Simulated;
			BodySetup->CollisionReponse = EBodyCollisionResponse::BodyCollision_Enabled;
			BodySetup->DefaultInstance.bEnableGravity = true;
			// MMD 減衰=1.0 表示"每次迭代仅剩0%速度"，并非UE的阻尼满值；
			// 直接代入会让刚体完全冻结，因此换算为(1-減衰)并限制上限。
			const float MmdLinearDamp = TotalLinearDamping / DynamicBodyCount;
			const float MmdAngularDamp = TotalAngularDamping / DynamicBodyCount;
			BodySetup->DefaultInstance.LinearDamping =
				FMath::Clamp((1.0f - MmdLinearDamp) * 2.0f, 0.0f, 2.0f);
			BodySetup->DefaultInstance.AngularDamping =
				FMath::Clamp((1.0f - MmdAngularDamp) * 2.0f, 0.0f, 2.0f);
			UE_LOG(LogMMD4UE4_PMXFactory, Log,
				TEXT("MMD rigid body [%s]: PhysicsType=%d (bodies=%d, damp lin=%.2f ang=%.2f)"),
				*BodySetup->BoneName.ToString(),
				static_cast<int32>(BodySetup->PhysicsType),
				DynamicBodyCount,
				BodySetup->DefaultInstance.LinearDamping,
				BodySetup->DefaultInstance.AngularDamping);
		}
		else
		{
			BodySetup->PhysicsType = PhysType_Kinematic;
			BodySetup->DefaultInstance.bEnableGravity = false;
			BodySetup->CollisionReponse = EBodyCollisionResponse::BodyCollision_Enabled;
		}
	}

	// Kinematic与Dynamic骨骼之间开启碰撞
	for (int32 i = 0; i < BodySetupCount; ++i)
	{
		for (int32 j = i + 1; j < BodySetupCount; ++j)
		{
			if (PhysicsAsset->SkeletalBodySetups[i] && PhysicsAsset->SkeletalBodySetups[j] &&
				PhysicsAsset->SkeletalBodySetups[i]->PhysicsType != PhysicsAsset->SkeletalBodySetups[j]->PhysicsType)
			{
				PhysicsAsset->EnableCollision(j, i);
			}
		}
	}

	// MMD关节 -> 角度限制约束
	for (const MMD4UE4::PMX_JOINT& PmxJoint : PmxMeshInfo->jointList)
	{
		if (!PmxMeshInfo->rigidList.IsValidIndex(PmxJoint.RigidBodyAIndex) ||
			!PmxMeshInfo->rigidList.IsValidIndex(PmxJoint.RigidBodyBIndex))
		{
			continue;
		}

		const MMD4UE4::PMX_RIGIDBODY& RigidBodyA = PmxMeshInfo->rigidList[PmxJoint.RigidBodyAIndex];
		const MMD4UE4::PMX_RIGIDBODY& RigidBodyB = PmxMeshInfo->rigidList[PmxJoint.RigidBodyBIndex];
		if (RigidBodyA.BoneIndex >= static_cast<uint32>(PmxMeshInfo->boneList.Num()) ||
			RigidBodyB.BoneIndex >= static_cast<uint32>(PmxMeshInfo->boneList.Num()))
		{
			continue;
		}

		const FName BoneA(*PmxMeshInfo->boneList[RigidBodyA.BoneIndex].Name);
		const FName BoneB(*PmxMeshInfo->boneList[RigidBodyB.BoneIndex].Name);
		if (BoneA.IsNone() || BoneB.IsNone() || BoneA == BoneB)
		{
			continue;
		}

		UPhysicsConstraintTemplate* JointTemplate = NewObject<UPhysicsConstraintTemplate>(PhysicsAsset);
		if (!JointTemplate)
		{
			continue;
		}
		FConstraintInstance& JointConstraint = JointTemplate->DefaultInstance;
		JointConstraint.ConstraintBone1 = BoneA;
		JointConstraint.ConstraintBone2 = BoneB;
		JointConstraint.ProfileInstance.LinearLimit.XMotion = ELinearConstraintMotion::LCM_Limited;
		JointConstraint.ProfileInstance.LinearLimit.YMotion = ELinearConstraintMotion::LCM_Limited;
		JointConstraint.ProfileInstance.LinearLimit.ZMotion = ELinearConstraintMotion::LCM_Limited;
		JointConstraint.ProfileInstance.ConeLimit.Swing1Motion = EAngularConstraintMotion::ACM_Limited;
		JointConstraint.ProfileInstance.ConeLimit.Swing2Motion = EAngularConstraintMotion::ACM_Limited;
		JointConstraint.ProfileInstance.TwistLimit.TwistMotion = EAngularConstraintMotion::ACM_Limited;
		JointConstraint.ProfileInstance.ConeLimit.Swing1LimitDegrees =
			FMath::RadiansToDegrees(FMath::Abs(PmxJoint.ConstrainRotationMax[0]));
		JointConstraint.ProfileInstance.ConeLimit.Swing2LimitDegrees =
			FMath::RadiansToDegrees(FMath::Abs(PmxJoint.ConstrainRotationMax[1]));
		JointConstraint.ProfileInstance.TwistLimit.TwistLimitDegrees =
			FMath::RadiansToDegrees(FMath::Max(
				FMath::Abs(PmxJoint.ConstrainRotationMin[2]),
				FMath::Abs(PmxJoint.ConstrainRotationMax[2])));
		JointConstraint.SetDisableCollision(true);
		PhysicsAsset->ConstraintSetup.Add(JointTemplate);
	}
}

UMMDExtendAsset * UPmxFactory::CreateMMDExtendFromMMDModel(
	UObject* InParent,
	USkeletalMesh* SkeletalMesh, // issue #2: fix param use skeleton mesh
	MMD4UE4::PmxMeshInfo * PmxMeshInfo
	)
{
	check(SkeletalMesh->GetSkeleton());
	//Add UE4.9
	if (SkeletalMesh->GetSkeleton() == nullptr)
	{
		return nullptr;
	}

	//issue #2 : Fix MMDExtend IK Index
	const FReferenceSkeleton ReferenceSkeleton = SkeletalMesh->GetSkeleton()->GetReferenceSkeleton();
	const FName& Name = FName(*SkeletalMesh->GetName());

	//MMD Extend asset

	// TBD::アセット生成関数で既存アセット時の判断ができていないと思われる。
	// 場合によってはVMDFactoryのアセット生成処理を元に再設計すること
	FString ObjectName = FString::Printf(TEXT("%s_MMDExtendAsset"), *Name.ToString());
	const FString AssetDirectory = GetMeshAssetDirectory(SkeletalMesh) / GetMMDExtendAssetFolderName();
	bool bMMDExtendAssetCreated = false;
	//UMMDExtendAsset * NewMMDExtendAsset = nullptr; //UE5.8
	UMMDExtendAsset *NewMMDExtendAsset = CreateAsset<UMMDExtendAsset>(
		AssetDirectory, ObjectName, true, &bMMDExtendAssetCreated);
	if (!NewMMDExtendAsset)
	{

		// same object exists, try to see if it's asset, if so, load
		const FString ObjectPath = FString::Printf(
			TEXT("%s.%s"),
			*(AssetDirectory / ObjectName),
			*ObjectName);
		NewMMDExtendAsset = LoadObject<UMMDExtendAsset>(nullptr, *ObjectPath);

		if (!NewMMDExtendAsset)
		{
			AddTokenizedErrorMessage(
				FTokenizedMessage::Create(
				EMessageSeverity::Warning,
				FText::Format(LOCTEXT("CouldNotCreateMMDExtendAsset",
				"Could not create MMD Extend Asset ('{0}') for '{1}'"),
				FText::FromString(ObjectName),
				FText::FromString(Name.ToString()))
				),"FFbxErrors::SkeletalMesh_FailedToCreatePhyscisAsset");
		}
		else
		{
			NewMMDExtendAsset->IkInfoList.Empty();
		}
	}
	
	//create asset info
	if (NewMMDExtendAsset)
	{
		if (NewMMDExtendAsset->IkInfoList.Num() > 0)
		{
			NewMMDExtendAsset->IkInfoList.Empty();
		}
		//create IK
		//mapping
		NewMMDExtendAsset->ModelName = PmxMeshInfo->modelNameJP;
		NewMMDExtendAsset->ModelComment = FText::FromString( PmxMeshInfo->modelCommentJP);

		// 保存MMD原始刚体数据（保留OpType/減衰等原值，供VMD导入时物理烘焙使用）
		NewMMDExtendAsset->RigidBodyList.Reset();
		NewMMDExtendAsset->RigidBodyList.Reserve(PmxMeshInfo->rigidList.Num());
		for (const MMD4UE4::PMX_RIGIDBODY& PmxRigid : PmxMeshInfo->rigidList)
		{
			FMMD_RigidBodyInfo& RigidBodyInfo = NewMMDExtendAsset->RigidBodyList.AddDefaulted_GetRef();
			RigidBodyInfo.Name = PmxRigid.Name;
			RigidBodyInfo.BoneName = IM4U::MakeEngineSafeMmdBoneName(PmxRigid.fnName);
			RigidBodyInfo.ShapeType = PmxRigid.ShapeType;
			RigidBodyInfo.Size = FVector(PmxRigid.Size);
			RigidBodyInfo.Position = FVector(PmxRigid.Position);
			RigidBodyInfo.Rotation = PmxRigid.Quat;
			RigidBodyInfo.Mass = PmxRigid.Mass;
			RigidBodyInfo.PosDim = PmxRigid.PosDim;
			RigidBodyInfo.RotDim = PmxRigid.RotDim;
			RigidBodyInfo.Recoil = PmxRigid.Recoil;
			RigidBodyInfo.Friction = PmxRigid.Friction;
			RigidBodyInfo.OpType = PmxRigid.OpType;
		}

		// 保存MMD原始关节数据（旋转限制/弹簧，供烘焙时做角度约束）
		NewMMDExtendAsset->JointList.Reset();
		NewMMDExtendAsset->JointList.Reserve(PmxMeshInfo->jointList.Num());
		for (const MMD4UE4::PMX_JOINT& PmxJoint : PmxMeshInfo->jointList)
		{
			FMMD_JointInfo& JointInfo = NewMMDExtendAsset->JointList.AddDefaulted_GetRef();
			JointInfo.Name = PmxJoint.Name;
			JointInfo.RigidBodyAIndex = PmxJoint.RigidBodyAIndex;
			JointInfo.RigidBodyBIndex = PmxJoint.RigidBodyBIndex;
			JointInfo.ConstrainRotationMin = FVector(
				PmxJoint.ConstrainRotationMin[0], PmxJoint.ConstrainRotationMin[1], PmxJoint.ConstrainRotationMin[2]);
			JointInfo.ConstrainRotationMax = FVector(
				PmxJoint.ConstrainRotationMax[0], PmxJoint.ConstrainRotationMax[1], PmxJoint.ConstrainRotationMax[2]);
			JointInfo.ConstrainPositionMin = FVector(
				PmxJoint.ConstrainPositionMin[0], PmxJoint.ConstrainPositionMin[1], PmxJoint.ConstrainPositionMin[2]);
			JointInfo.ConstrainPositionMax = FVector(
				PmxJoint.ConstrainPositionMax[0], PmxJoint.ConstrainPositionMax[1], PmxJoint.ConstrainPositionMax[2]);
			JointInfo.SpringRotation = FVector(
				PmxJoint.SpringRotation[0], PmxJoint.SpringRotation[1], PmxJoint.SpringRotation[2]);
			JointInfo.SpringPosition = FVector(
				PmxJoint.SpringPosition[0], PmxJoint.SpringPosition[1], PmxJoint.SpringPosition[2]);
		}
		//
		for (int boneIdx = 0; boneIdx < PmxMeshInfo->boneList.Num(); ++boneIdx)
		{
			//check IK bone 
			if (PmxMeshInfo->boneList[boneIdx].Flag_IK)
			{
				MMD4UE4::PMX_IK * tempPmxIKPtr = &PmxMeshInfo->boneList[boneIdx].IKInfo;
				FMMD_IKInfo addMMDIkInfo;

				addMMDIkInfo.LoopNum = tempPmxIKPtr->LoopNum;
				//set limit rot[rad]
				addMMDIkInfo.RotLimit = tempPmxIKPtr->RotLimit;
				// this bone
				addMMDIkInfo.IKBoneName = IM4U::MakeEngineSafeMmdBoneName(
					FName(*PmxMeshInfo->boneList[boneIdx].Name));
				//issue #2: Fix IK bone index 
				//this bone(ik-bone) index, from skeleton.
				addMMDIkInfo.IKBoneIndex = ReferenceSkeleton.FindBoneIndex(addMMDIkInfo.IKBoneName);
				//ik target 
				if (!PmxMeshInfo->boneList.IsValidIndex(tempPmxIKPtr->TargetBoneIndex))
				{
					UE_LOG(LogMMD4UE4_PMXFactory, Warning,
						TEXT("Skipping IK bone '%s': invalid target index %d"),
						*addMMDIkInfo.IKBoneName.ToString(),
						tempPmxIKPtr->TargetBoneIndex);
					continue;
				}
				addMMDIkInfo.TargetBoneName = IM4U::MakeEngineSafeMmdBoneName(
					FName(*PmxMeshInfo->boneList[tempPmxIKPtr->TargetBoneIndex].Name));
				//issue #2: Fix Target Bone Index 
				//target bone(ik-target bone) index, from skeleton.
				addMMDIkInfo.TargetBoneIndex = ReferenceSkeleton.FindBoneIndex(addMMDIkInfo.TargetBoneName);
				//set sub ik
				addMMDIkInfo.ikLinkList.Reset();
				for (int ikInfoID = 0; ikInfoID < tempPmxIKPtr->LinkNum; ++ikInfoID)
				{
					if (!PmxMeshInfo->boneList.IsValidIndex(tempPmxIKPtr->Link[ikInfoID].BoneIndex))
					{
						continue;
					}
					FMMD_IKLINK& IkLink = addMMDIkInfo.ikLinkList.AddDefaulted_GetRef();
					//link bone index
					IkLink.BoneName = IM4U::MakeEngineSafeMmdBoneName(
						FName(*PmxMeshInfo->boneList[tempPmxIKPtr->Link[ikInfoID].BoneIndex].Name));
					//issue #2: Fix link bone index
					//link bone index from skeleton.
					IkLink.BoneIndex
						= ReferenceSkeleton.FindBoneIndex(IkLink.BoneName);
					//limit flag
					IkLink.RotLockFlag = tempPmxIKPtr->Link[ikInfoID].RotLockFlag;
					//min
					IkLink.RotLockMin.X = tempPmxIKPtr->Link[ikInfoID].RotLockMin[0];
					IkLink.RotLockMin.Y = tempPmxIKPtr->Link[ikInfoID].RotLockMin[1];
					IkLink.RotLockMin.Z = tempPmxIKPtr->Link[ikInfoID].RotLockMin[2];
					//max
					IkLink.RotLockMax.X = tempPmxIKPtr->Link[ikInfoID].RotLockMax[0];
					IkLink.RotLockMax.Y = tempPmxIKPtr->Link[ikInfoID].RotLockMax[1];
					IkLink.RotLockMax.Z = tempPmxIKPtr->Link[ikInfoID].RotLockMax[2];
				}
				addMMDIkInfo.checkIKIndex =
					addMMDIkInfo.IKBoneIndex != INDEX_NONE &&
					addMMDIkInfo.TargetBoneIndex != INDEX_NONE &&
					addMMDIkInfo.ikLinkList.Num() > 0;

				//碰撞（角度限制）：MMD侧IK目标带角度限制时，作为碰撞体求解并限制链路旋转
				addMMDIkInfo.bEnableCollision = false;
				for (int ikInfoID = 0; ikInfoID < addMMDIkInfo.ikLinkList.Num(); ++ikInfoID)
				{
					if (addMMDIkInfo.ikLinkList[ikInfoID].RotLockFlag)
					{
						addMMDIkInfo.bEnableCollision = true;
						break;
					}
				}

				//add
				NewMMDExtendAsset->IkInfoList.Add(addMMDIkInfo);
			}
		}
		NewMMDExtendAsset->MarkPackageDirty();
		if (bMMDExtendAssetCreated)
		{
			FAssetRegistryModule::AssetCreated(NewMMDExtendAsset);
		}
	}
	return NewMMDExtendAsset;
}
#undef LOCTEXT_NAMESPACE
