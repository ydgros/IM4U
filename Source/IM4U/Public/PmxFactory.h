// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#pragma once
#include "Engine.h"
#include "Factories/Factory.h"
#include "CoreMinimal.h"
#include "Runtime/Core/Public/Logging/TokenizedMessage.h"
#include "ImportUtils/SkelImport.h"
#include "PmxImporter.h"
#include "PmxImportUI.h"
#include "PmxMaterialImport.h"
#include "PmxFactory.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMMD4UE4_PMXFactory, Log, All)

UCLASS()
class IM4U_API UPmxFactory : public UFactory // public UFbxFactory
{
	GENERATED_UCLASS_BODY()
	//class UPmxImportUI* ImportUI;
	UPROPERTY()
	UPmxImportUI* ImportUI = nullptr;
	// Begin UFactory Interface
	virtual void PostInitProperties() override;
	virtual bool DoesSupportClass(UClass * Class) override;
	virtual UClass* ResolveSupportedClass() override;
protected: //5.8
	virtual UObject* FactoryCreateBinary( //堆栈错误 02
		UClass* InClass, 
		UObject* InParent, 
		FName InName, 
		EObjectFlags Flags, 
		UObject* Context,
		const TCHAR* Type, 
		const uint8*& Buffer, 
		const uint8* BufferEnd, 
		FFeedbackContext* Warn,
		bool& bOutOperationCanceled
		) override;
	// End UFactory Interface

	enum E_LOAD_ASSETS_TYPE_MMD
	{
		E_MMD_TO_UE4_UNKOWN,
		E_MMD_TO_UE4_SKELTON,
		E_MMD_TO_UE4_STATICMESH,
		E_MMD_TO_UE4_ANIMATION
	};
	E_LOAD_ASSETS_TYPE_MMD importAssetTypeMMD;

	USkeletalMesh* ImportSkeletalMesh( //堆栈错误 01
		UObject* InParent,
		MMD4UE4::PmxMeshInfo *pmxMeshInfoPtr,
		const FName& FName,
		EObjectFlags Flags,
		//UFbxSkeletalMeshImportData* TemplateImportData,
		FString Filename,
		//TArray<FbxShape*> *FbxShapeArray,
		FSkeletalMeshImportData* OutData,
		bool bCreateRenderData
		);

	bool ImportBone(
		//TArray<FbxNode*>& NodeArray,
		MMD4UE4::PmxMeshInfo *PmxMeshInfo,
		FSkeletalMeshImportData &ImportData,
		//UFbxSkeletalMeshImportData* TemplateData,
		//TArray<FbxNode*> &SortedLinks,
		bool& bOutDiffPose,
		bool bDisableMissingBindPoseWarning,
		bool & bUseTime0AsRefPose
		);

	bool FillSkelMeshImporterFromFbx(
		FSkeletalMeshImportData& ImportData,
		MMD4UE4::PmxMeshInfo *& PmxMeshInfo,
		UObject* InParent
		/*FbxMesh*& Mesh,
		FbxSkin* Skin,
		FbxShape* FbxShape,
		TArray<FbxNode*> &SortedLinks,
		const TArray<FbxSurfaceMaterial*>& FbxMaterials*/
		);

	/* Create a new asset from the package and objectname and class */
	static UObject* CreateAssetOfClass(
		UClass* AssetClass,
		FString ParentPackageName,
		FString ObjectName,
		bool bAllowReplace = false,
		bool* bOutCreated = nullptr
		);

	/* Templated function to create an asset with given package and name */
	template< class T>
	static T * CreateAsset(
		FString ParentPackageName,
		FString ObjectName,
		bool bAllowReplace = false,
		bool* bOutCreated = nullptr)
	{
		//return (T*)CreateAssetOfClass(T::StaticClass(), ParentPackageName, ObjectName, bAllowReplace);
		return (static_cast<T*>(CreateAssetOfClass(
			T::StaticClass(), ParentPackageName, ObjectName, bAllowReplace, bOutCreated))); //5.8
	}

	void ImportMorphTargetsInternal(
		//TArray<FbxNode*>& SkelMeshNodeArray,
		MMD4UE4::PmxMeshInfo & PmxMeshInfo,
		USkeletalMesh* BaseSkelMesh,
		UObject* InParent,
		const FString& InFilename,
		int32 LODIndex, FSkeletalMeshImportData& BaseImportData
		);

	// Import Morph target
	void ImportFbxMorphTarget(
		//TArray<FbxNode*> &SkelMeshNodeArray,
		MMD4UE4::PmxMeshInfo & PmxMeshInfo,
		USkeletalMesh* BaseSkelMesh,
		UObject* InParent,
		const FString& Filename,
		int32 LODIndex, FSkeletalMeshImportData& ImportData
		);

	void AddTokenizedErrorMessage(
		TSharedRef<FTokenizedMessage> Error,
		FName FbxErrorName
		);

	UMMDExtendAsset * CreateMMDExtendFromMMDModel(
		UObject* InParent,
		USkeletalMesh* SkeletalMesh, // issue #2: fix param use skeleton mesh
		MMD4UE4::PmxMeshInfo * PmxMeshInfo
		);

public:
	//UFUNCTION(BlueprintCallable, Category = "im4u")
	UFUNCTION()//5.8
		static bool ImportPmxFromFile(FString file);

	bool FImportPmxFromFile(FString file);


protected:
	bool bDetectImportTypeOnImport;
	/* true if the import operation was canceled. */
	bool bOperationCanceled;

	UPmxMaterialImport pmxMaterialImportHelper;
};