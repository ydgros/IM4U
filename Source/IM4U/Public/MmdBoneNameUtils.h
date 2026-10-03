#pragma once

#include "CoreMinimal.h"

namespace IM4U
{
	inline FString GetCanonicalHemBoneName(const FString& BoneName)
	{
		static const TCHAR* BoneNames[] = {
			TEXT("HemB-L-E01"), TEXT("HemB-L-E02"), TEXT("HemB-L-E03"), TEXT("HemB-L-E04"),
			TEXT("HemB-R-E01"), TEXT("HemB-R-E02"), TEXT("HemB-R-E03"), TEXT("HemB-R-E04")
		};
		FString NormalizedName = BoneName;
		NormalizedName.TrimStartAndEndInline();
		if (NormalizedName.StartsWith(TEXT("+")) || NormalizedName.StartsWith(TEXT("_")))
		{
			NormalizedName.RightChopInline(1);
		}
		NormalizedName.ReplaceInline(TEXT(" "), TEXT("-"));

		for (const TCHAR* Candidate : BoneNames)
		{
			if (NormalizedName.Equals(Candidate, ESearchCase::IgnoreCase))
			{
				return FString(TEXT("_")) + Candidate;
			}
		}
		return FString();
	}

	inline FString MakeEngineSafeMmdBoneName(const FString& BoneName)
	{
		const FString CanonicalName = GetCanonicalHemBoneName(BoneName);
		if (!CanonicalName.IsEmpty())
		{
			return CanonicalName;
		}

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
}
