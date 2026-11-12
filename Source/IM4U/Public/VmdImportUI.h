// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#pragma once
#include "VmdImportUI.generated.h"

/* Import mesh type */
UENUM()
enum EVMDImportType
{
	/* Select Animation if you'd like to import only animation. */
	VMDIT_Animation UMETA(DisplayName = "Animation"),
	VMDIT_MAX,
};

UCLASS(config = EditorUserSettings, AutoExpandCategories = (FTransform), HideCategories = Object, MinimalAPI)
class UVmdImportUI : public UObject
{
	GENERATED_UCLASS_BODY()

	/* Whether or not the imported file is in OBJ format */
	UPROPERTY()
		bool bIsObjImport;

	/* The original detected type of this import */
	UPROPERTY()
		TEnumAsByte<enum EVMDImportType> OriginalImportType;

	/* Type of asset to import from the FBX file */
	UPROPERTY()
		TEnumAsByte<enum EVMDImportType> MeshTypeToImport;

	/* Use the string in "Name" field as full name of mesh. The option only works when the scene contains one mesh. */
	//UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category = Miscellaneous, meta = (OBJRestrict = "true"))
	UPROPERTY()
		uint32 bOverrideFullName : 1;

	/* Skeleton to use for imported asset. When importing a mesh, leaving this as "None" will create a new skeleton. When importing and animation this MUST be specified to import the asset. */
	UPROPERTY(EditAnywhere, Category = Mesh, meta = (OBJRestrict = "false"))
		TObjectPtr<class USkeleton> Skeleton = nullptr;

	/* SkeletonMesh to use for imported asset. When importing a mesh, leaving this as "None" will create a new skeleton. When importing and animation this MUST be specified to import the asset. */
	UPROPERTY(EditAnywhere, Category = Mesh, meta = (OBJRestrict = "false"))
		TObjectPtr<class USkeletalMesh> SkeletonMesh = nullptr;

	/* IKRig用于骨骼重定向；设置后会启用IK求解 */
	// 不在导入菜单显示（与 MMD2UE4NameTableRow 等保持一致），如需显示取消下面的注释
	//UPROPERTY(EditAnywhere, Category = IK, meta = (ToolTip = "Optional IK Rig asset used together with the MMD Extend Asset IK chains."))
	UPROPERTY()
		TObjectPtr<class UIKRigDefinition> IKRig = nullptr;

	/* 用MMDExtendAsset的IK信息对VMD动作做IK/碰撞求解（需要同时指定MMD Extend Asset） */
	UPROPERTY(EditAnywhere, Category = IK, meta = (ToolTip = "Solve IK and collision (angle limit) from the MMD Extend Asset while importing the VMD motion."))
		bool bSolveIK = true;

	/* 求解IK时是否应用链路骨骼的角度限制（碰撞） */
	UPROPERTY(EditAnywhere, Category = IK, meta = (ToolTip = "Apply MMD angle limits (collision) to IK link bones during solving."))
		bool bApplyIKCollision = true;

	/* 导入时把MMD刚体演算结果烘焙进动画（裙摆/头发/胸部摆动）。
	   若播放时依赖引擎 Physics Asset 实时物理（Chaos），请关闭：两者叠加会导致穿模、抽搐。 */
	UPROPERTY(EditAnywhere, Category = Physics, meta = (ToolTip = "Bake MMD rigid body swing into the animation. Disable when the Skeletal Mesh Physics Asset simulates in engine, otherwise both fight each other (clipping/jitter)."))
		bool bBakeRigidBodyMotion = true;

	/* True to import animations from the FBX File */
	//UPROPERTY(EditAnywhere, config, Category = Animation, meta = (ImportType = "SkeletalMesh|Animation"))
	UPROPERTY()
		uint32 bImportAnimations : 1;

	/* Override for the name of the animation to import **/
	//UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Animation, meta = (editcondition = "bImportAnimations", ImportType = "SkeletalMesh"))
	UPROPERTY()
		FString AnimationName;

	/* Import data used when importing static meshes */
	//UPROPERTY(EditAnywhere, Instanced, Category = Mesh, meta = (ImportType = "StaticMesh"))
	UPROPERTY()
		TObjectPtr<class UMMDStaticMeshImportData> StaticMeshImportData = nullptr;
		
	/* Import data used when importing skeletal meshes */
	//UPROPERTY(EditAnywhere, Instanced, Category = Mesh, meta = (ImportType = "SkeletalMesh"))
	//class UFbxSkeletalMeshImportData* SkeletalMeshImportData;

	/* Import data used when importing skeletal meshes */
	//UPROPERTY(EditAnywhere, Instanced, Category = Mesh, meta = (ImportType = "SkeletalMesh"))
	UPROPERTY()
		TObjectPtr<class UMMDSkeletalMeshImportData> SkeletalMeshImportData = nullptr;

	/* Import data used when importing animations */
	//UPROPERTY(EditAnywhere, Instanced, Category = Animation, meta = (editcondition = "bImportAnimations", ImportType = "Animation"))
	//class UFbxAnimSequenceImportData* AnimSequenceImportData;

	/* Type of asset to import from the FBX file */
	//UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category = Animation, meta = (editcondition = "bImportAnimations", ImportType = "SkeletalMesh|Animation"))
	UPROPERTY()
		bool bPreserveLocalTransform;

	/* Import data used when importing textures */
	//UPROPERTY(EditAnywhere, Instanced, Category = Material)
	//class UFbxTextureImportData* TextureImportData;

	// Begin UObject Interface
	virtual bool CanEditChange(const FProperty* InProperty) const override;
	// End UObject Interface

	/////////////////////////

	/* Whether to automatically create Unreal materials for materials found in the FBX scene */


	/* Skeleton to use for imported asset. When importing a mesh, leaving this as "None" will create a new skeleton. When importing and animation this MUST be specified to import the asset. */
	//UPROPERTY(EditAnywhere, Category = Animation, meta = (OBJRestrict = "false")) //隐藏AnimSequenceAsset导入菜单，取消注释，打开导入菜单
	UPROPERTY()
		TObjectPtr<class UAnimSequence> AnimSequenceAsset = nullptr;

	/* Skeleton to use for imported asset. When importing a mesh, leaving this as "None" will create a new skeleton. When importing and animation this MUST be specified to import the asset. */
	//UPROPERTY(EditAnywhere, Category = AnimationList, meta = (ImportType = "Animation"))
		TArray<class UMMDSkeletalMeshImportData*> TestArrayList;
	
	/* MMD2UE4NameTableRow to use for imported asset. When importing a Anim, leaving this as "None" will create a new skeleton. When importing and animation this MUST be specified to import the asset. */
	//UPROPERTY(EditAnywhere, Category = Animation, meta = (OBJRestrict = "false")) //隐藏MMD2UE4NameTableRow导入菜单，取消注释，打开导入菜单
	UPROPERTY()
		TObjectPtr<UDataTable> MMD2UE4NameTableRow = nullptr;

	/* mmd extend assset to use for calc ik . */
	UPROPERTY(EditAnywhere, Category = IK, meta = (ToolTip = "MMD Extend Asset that holds the IK chains used for IK/collision solving."))
		TObjectPtr<class UMMDExtendAsset> MmdExtendAsset = nullptr;

	//struct FTableRowBase MMD2UE4NameTableRowDmmy;

	//暫定版::上記クラスだと上手く表示できていないため必要な項目のみ抽出
	/* True to import I-Aggree , I Read README for Model. */
	//UPROPERTY(EditAnywhere, Category = Mast_ReadmeForModel, meta = (OBJRestrict = "true", ToolTip = "If enabled, creates Unreal morph objects for the imported meshes"))
		//uint32 bImportIAgree : 1;
};