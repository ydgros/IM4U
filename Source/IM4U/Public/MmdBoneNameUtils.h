#pragma once

#include "CoreMinimal.h"

namespace IM4U
{
	inline FString MakeEngineSafeMmdBoneName(const FString& BoneName)
	{
		if (BoneName.StartsWith(TEXT("+")))
		{
			FString EngineSafeName = BoneName;
			EngineSafeName[0] = TEXT('_');
			return EngineSafeName;
		}
		return BoneName;
	}

	inline FName MakeEngineSafeMmdBoneName(FName BoneName)
	{
		return FName(*MakeEngineSafeMmdBoneName(BoneName.ToString()));
	}

	inline bool AreEquivalentMmdBoneNames(FName Left, FName Right)
	{
		return MakeEngineSafeMmdBoneName(Left) == MakeEngineSafeMmdBoneName(Right);
	}

	/**
	 * 判定骨骼名是否为导入时被改名过的MMD骨骼。
	 * 原始名为'+'开头（如 +HairS-L-B01 / +HemB L E01），改名后统一为'_'开头。
	 * 用于诊断日志筛选，不依赖具体骨骼名列表。
	 */
	inline bool IsRenamedMmdBoneName(const FString& BoneName)
	{
		return BoneName.StartsWith(TEXT("+")) || BoneName.StartsWith(TEXT("_"));
	}

	inline bool IsRenamedMmdBoneName(FName BoneName)
	{
		return IsRenamedMmdBoneName(BoneName.ToString());
	}
}
