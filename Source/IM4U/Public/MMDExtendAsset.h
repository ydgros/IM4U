// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MMDExtendAsset.generated.h"

//MMD IK Link Structure
//MMD Extend Info : Ik info
//IK链接信息
USTRUCT(BlueprintType)
struct FMMD_IKLINK
{
	GENERATED_BODY()

	//Link bone index (for skeleton bone index, use ik func)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 link bone index"))
	//int32 BoneIndex;	
	int32 BoneIndex = -1;

	//Link Bone Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 link bone name"))
	FName BoneName;

	//Rotation limit flag (0:OFF 1:ON)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "If enabled, rotation limite, 0:OFF, 1:ON "))
	//uint32 RotLockFlag:1;
	uint32 RotLockFlag : 1 = 0;

	//Rotation angle limit Euler[min]
	//【修复】直接初始化 FVector，解决 Ensure condition failed 错误
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "Rotation angle limit Euler[dig:-180~180] min"))
	//FVector RotLockMin;
	FVector RotLockMin = FVector::ZeroVector;

	//Rotation angle limit Euler[max]
	//【修复】直接初始化 FVector，解决 Ensure condition failed 错误
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "Rotation angle limit Euler[dig:-180~180] max"))
	//FVector RotLockMax;
	FVector RotLockMax = FVector::ZeroVector;

	FMMD_IKLINK()
	{
		//成员已在声明处初始化，构造函数留空或用于额外逻辑
		//BoneIndex = -1;
		//RotLockFlag = 0;
	}
};

	//MMD IK Info Structure
//IK信息
USTRUCT(BlueprintType)
struct FMMD_IKInfo
{
	GENERATED_BODY()

	//IK calculation flag
	//【修复】添加 UPROPERTY 并初始化，防止序列化未知结构错误
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	bool checkIKIndex = false;

	//IK target bone index vmd key index
	//【修复】添加 UPROPERTY 并初始化，使用 int32 替代 int
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	int32 IKBoneIndexVMDKey = -1;

	//IK target bone index (use ik func. ref skeleton.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 IK bone index"))
	int32 IKBoneIndex = -1;

	//IK Bone Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 IK bone name"))
	FName IKBoneName;

	//Target bone index (use ik func. ref skeleton.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 target bone index"))
	int32 TargetBoneIndex = -1;

	//Target Bone Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "ue4 target bone name"))
	FName TargetBoneName;

	//CCD-IK loop count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "CCD-IK loop count"))
	int32 LoopNum = 0;

	//CCD-IK unit angle[dig]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "CCD-IK unit angle[dig]"))
	float RotLimit = 0.0f;

	//IK link info list
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "CCD-IK link IK info"))
	TArray<FMMD_IKLINK> ikLinkList;

	//IK collision(角限制) enable : MMD侧IK目标骨骼带角度限制时为true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD", meta = (ToolTip = "Enable IK collision(angle limit) solving for this IK bone"))
	bool bEnableCollision = false;

	FMMD_IKInfo()
	{
		// 成员已在声明处初始化
		//checkIKIndex = false;
		//IKBoneIndexVMDKey = -1;
		//IKBoneIndex = -1;
		//TargetBoneIndex = -1;
		//LoopNum = 0;
		//RotLimit = 0;
	}
};

//MMD Rigid Body Info (PMX原值，供VMD导入时物理烘焙使用)
USTRUCT(BlueprintType)
struct FMMD_RigidBodyInfo
{
	GENERATED_BODY()

	//刚体名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FString Name;

	//绑定的骨骼名（UE侧）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FName BoneName;

	//0:球 1:箱 2:胶囊
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	uint8 ShapeType = 0;

	//尺寸
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FVector Size = FVector::ZeroVector;

	//相对骨骼的位置/旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FVector Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FQuat Rotation = FQuat::Identity;

	//质量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	float Mass = 0.0f;

	//移动衰减(0~1，1=几乎不减速)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	float PosDim = 1.0f;

	//旋转衰减(0~1，1=几乎不减速)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	float RotDim = 1.0f;

	//回弹力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	float Recoil = 0.0f;

	//摩擦力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	float Friction = 0.0f;

	//0:Bone跟随 1:物理演算 2:物理演算+Bone位置合わせ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	uint8 OpType = 0;

	//剛体グループ番号(0~15)：同组刚体互不碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	uint8 GroupIndex = 0;

	//剛体グループ対象：位掩码，bit i 置位表示与第i组碰撞
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	int32 GroupTarget = 0xFFFF;
};

//MMD Joint Info (PMX原值)
USTRUCT(BlueprintType)
struct FMMD_JointInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FString Name;

	//连接的两个刚体在RigidBodyList中的索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	int32 RigidBodyAIndex = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	int32 RigidBodyBIndex = INDEX_NONE;

	//旋转限制(弧度)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FVector ConstrainRotationMin = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FVector ConstrainRotationMax = FVector::ZeroVector;

	//移动限制
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FVector ConstrainPositionMin = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FVector ConstrainPositionMax = FVector::ZeroVector;

	//弹簧系数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FVector SpringRotation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MMD")
	FVector SpringPosition = FVector::ZeroVector;
};

//MMD Extend Asset Class
UCLASS()
class UMMDExtendAsset : public UObject
{
	GENERATED_BODY()

public:
	//MMD target model name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Header)
	FString ModelName;

	//MMD model comment 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Header, meta = (MultiLine = "true"))
	FText ModelComment;

	//MMD-IK-Info is used to generate the AnimSequence from VMD file.
	UPROPERTY(EditAnywhere, Category = IK)
	TArray<FMMD_IKInfo> IkInfoList;

	//MMD原始刚体数据（VMD导入时用于物理烘焙，保留OpType/減衰等原值）
	UPROPERTY(EditAnywhere, Category = Physics)
	TArray<FMMD_RigidBodyInfo> RigidBodyList;

	//MMD原始关节数据（旋转限制、弹簧，用于烘焙时的角度约束）
	UPROPERTY(EditAnywhere, Category = Physics)
	TArray<FMMD_JointInfo> JointList;

	UMMDExtendAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//TBD::用途不明
	//bool CanEditChange( const FProperty* InProperty ) const;
//private:
};