// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SSkeletonWidget.h"
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

	UMMDExtendAsset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//TBD::用途不明
	//bool CanEditChange( const FProperty* InProperty ) const;
//private:
};