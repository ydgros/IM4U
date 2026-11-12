// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#pragma once
#include "Engine.h"
#include "Factories/Factory.h"
#include "ImportUtils/SkelImport.h"
#include "AnimationUtils.h"
#include "MMD2UE4NameTableRow.h"
#include "VmdImportUI.h"
#include "VmdImporter.h"
#include "MMDExtendAsset.h"
#include "VmdFactory.generated.h"

// Forward declaration
class UInterpGroupInst;
static TMap<FName, FName> NameMap;

DECLARE_LOG_CATEGORY_EXTERN(LogMMD4UE4_VMDFactory, Log, All)

UCLASS()
class IM4U_API UVmdFactory : public UFactory 
{
	GENERATED_UCLASS_BODY()

	// Begin UFactory Interface
	virtual void PostInitProperties() override;
	virtual bool DoesSupportClass(UClass * Class) override;
	virtual UClass* ResolveSupportedClass() override;
protected:	
	virtual UObject* FactoryCreateBinary(
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

	/*Create AnimSequence from VMD data.（新建用父函数*/
	UAnimSequence * ImportAnimations(
		USkeleton* Skeleton,
		USkeletalMesh* SkeletalMesh,
		UObject* Outer,
		const FString& Name, 
		UIKRigDefinition* IKRig,
		UDataTable* ReNameTable,
		UMMDExtendAsset* mmdExtend,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo,
		bool bApplyIKCollision = true,
		bool bBakeRigidBodyMotion = true
		);

	UPROPERTY()
	class UVmdImportUI* ImportUI;

	/*面向MMD的贝塞尔曲线的计算处理*/
	float interpolateBezier(float x1, float y1, float  x2, float y2, float x);

	/*将VMD表情数据添加到现有AnimSequ资源的过程
	与MMD4Mecanimu的综合利用测试功能*/
	UAnimSequence * AddtionalMorphCurveImportToAnimations(
		USkeletalMesh* SkeletalMesh,
		UAnimSequence* exsistAnimSequ,
		UDataTable* ReNameTable,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo
		);

	/*将Morphtarget FloatCurve从VMD文件数据导入AnimSeq*/
	bool ImportMorphCurveToAnimSequence(
		UAnimSequence* DestSeq,
		USkeleton* Skeleton,
		USkeletalMesh* SkeletalMesh,
		UDataTable* ReNameTable,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo
		);

	/*从VMD文件的数据将运动数据导入AnimSeq*/
	bool ImportVMDToAnimSequence(
		UAnimSequence* DestSeq,
		USkeleton* Skeleton,
		USkeletalMesh* SkeletalMesh,
		UDataTable* ReNameTable,
		UIKRigDefinition* IKRig,
		UMMDExtendAsset* mmdExtend,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo,
		bool bApplyIKCollision = true,
		bool bBakeRigidBodyMotion = true
		);

	/*用MMDExtendAsset中的IK链对逐帧局部变换做CCD-IK求解（含碰撞角度限制）
	  未提供IK信息时保留VMD原始姿态；IK开关关键帧存在时逐帧开关IK*/
	void ApplyIkSolveToBoneTracks(
		USkeleton* Skeleton,
		const TArray<FRawAnimSequenceTrack>& InRefTracks,
		TArray<FRawAnimSequenceTrack>& OutTracks,
		UIKRigDefinition* IKRig,
		UMMDExtendAsset* mmdExtend,
		MMD4UE4::VmdMotionInfo* vmdMotionInfo,
		int32 NumFrames,
		const TArray<FTransform>& RefPose,
		bool bApplyIKCollision = true
		);

	/*CCD-IK单帧求解（就地修改局部变换）*/
	void SolveIkForFrame(
		const FReferenceSkeleton& RefSkeleton,
		TArray<FMMD_IKInfo> IkInfoList,
		TArray<FTransform>& LocalTransforms,
		const TArray<FTransform>& RefPose,
		int32 Frame,
		bool bApplyIKCollision
		);

	/*刚体物理烘焙：用MMD刚体对Simulated骨骼做弹簧-阻尼+惯性演算，结果写入轨道
	  没有任何Simulated刚体时返回false，轨道保持不变*/
	bool BakeRigidBodyMotion(
		USkeletalMesh* SkeletalMesh,
		const FReferenceSkeleton& RefSkeleton,
		TArray<FRawAnimSequenceTrack>& Tracks,
		int32 NumFrames,
		float FrameRate,
		UMMDExtendAsset* mmdExtend = nullptr
		);

	/*从MMD侧的名称检索并取得TableRow的UE侧名称*/
	bool FindTableRowMMD2UEName(
		UDataTable* ReNameTable,
		FName mmdName,
		FName * ue4Name
		);

	/*解析VMD morph名到实际存在的MorphTarget名（名称表优先，回退原名/大小写不敏感/去前后缀）*/
	FName ResolveMorphTargetName(
		USkeletalMesh* MorphMesh,
		UDataTable* ReNameTable,
		const FString& VmdTrackName
		);

	/*从Bone名称中搜索并获取与RefSkelton匹配的BoneIndex*/
	int32 FindRefBoneInfoIndexFromBoneName(
		const FReferenceSkeleton & RefSkelton,
		const FName & TargetName
		);

	/*递归计算当前关键帧中指定Bone的Glb坐标*/
	FTransform CalcGlbTransformFromBoneIndex(
		UAnimSequence* DestSeq,
		USkeleton* Skeleton,
		int32 BoneIndex,
		int32 keyIndex
		);
	float baseFrameRate;


public:
	//UFUNCTION(BlueprintCallable, Category = "im4u")
	UFUNCTION()
		static bool ImportVmdFromFile(FString file, USkeletalMesh* SkeletalMesh);
};
