// Copyright 2023 NaN_Name. All Rights Reserved.
#ifndef HEADER_H_
#define HEADER_H_
#endif
#include "VmdFactory.h"
#include "VmdImportOption.h"
#include "VmdImporter.h"
#include "VmdImportUI.h"
#include "MMDExtendAsset.h"
#include "MmdBoneNameUtils.h"
#include "AnimationUtils.h"
#include "Animation/AnimData/CurveIdentifier.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "CoreMinimal.h"
#include "ImportUtils/SkelImport.h"
#include "Misc/EngineVersionComparison.h"
#include "Misc/PackageName.h"
#include "ObjectTools.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/PhysicsConstraintTemplate.h"
#include "PhysicsEngine/SkeletalBodySetup.h"
#include "RigEditor/IKRigController.h"

#define LOCTEXT_NAMESPACE "VMDImportFactory"

DEFINE_LOG_CATEGORY(LogMMD4UE4_VMDFactory)

#define ADD_NAME_MAP( x , y ) NameMap.Add((y),(x))

void initMmdNameMap()
{
	if (NameMap.Num() == 0)
	{
		ADD_NAME_MAP(L"操作中心", L"op_center");
		ADD_NAME_MAP(L"操作中枢", L"op_center");
		ADD_NAME_MAP(L"全ての親", L"all_parent");
		ADD_NAME_MAP(L"全部の親", L"all_parent");
		ADD_NAME_MAP(L"全親", L"all_parent");
		ADD_NAME_MAP(L"センター", L"center");
		ADD_NAME_MAP(L"Center", L"center");
		ADD_NAME_MAP(L"センター2", L"center2");
		ADD_NAME_MAP(L"Center2", L"center2");
		ADD_NAME_MAP(L"グルーブ", L"groove");
		ADD_NAME_MAP(L"groove", L"groove");
		ADD_NAME_MAP(L"グルーブ2", L"groove2");
		ADD_NAME_MAP(L"groove2", L"groove2");
		ADD_NAME_MAP(L"腰", L"waist");
		ADD_NAME_MAP(L"腰骨", L"waist");
		ADD_NAME_MAP(L"下半身", L"lowerBody");
		ADD_NAME_MAP(L"lowerBody", L"lowerBody");
		ADD_NAME_MAP(L"下半身2", L"lowerBody");
		ADD_NAME_MAP(L"上半身", L"upperBody");
		ADD_NAME_MAP(L"upperBody", L"upperBody");
		ADD_NAME_MAP(L"上半身2", L"upperBody2");
		ADD_NAME_MAP(L"upperBody2", L"upperBody2");
		ADD_NAME_MAP(L"首", L"neck");
		ADD_NAME_MAP(L"首骨", L"neck");
		ADD_NAME_MAP(L"頭", L"head");
		ADD_NAME_MAP(L"頭部", L"head");
		ADD_NAME_MAP(L"左目", L"eyeL");
		ADD_NAME_MAP(L"右目", L"eyeR");
		ADD_NAME_MAP(L"目L", L"eyeL");
		ADD_NAME_MAP(L"目R", L"eyeR");
		ADD_NAME_MAP(L"両目", L"eyes");
		ADD_NAME_MAP(L"目", L"eyes");
		ADD_NAME_MAP(L"左肩", L"shoulderL");
		ADD_NAME_MAP(L"肩L", L"shoulderL");
		ADD_NAME_MAP(L"左肩先", L"shoulderL");
		ADD_NAME_MAP(L"左腕", L"armL");
		ADD_NAME_MAP(L"腕L", L"armL");
		ADD_NAME_MAP(L"左ひじ", L"elbowL");
		ADD_NAME_MAP(L"左肘", L"elbowL");
		ADD_NAME_MAP(L"左手首", L"wristL");
		ADD_NAME_MAP(L"手首L", L"wristL");
		ADD_NAME_MAP(L"左親指０", L"thumb0L");
		ADD_NAME_MAP(L"左親指0", L"thumb0L");
		ADD_NAME_MAP(L"左親指１", L"thumb1L");
		ADD_NAME_MAP(L"左親指1", L"thumb1L");
		ADD_NAME_MAP(L"左親指２", L"thumb2L");
		ADD_NAME_MAP(L"左親指2", L"thumb2L");
		ADD_NAME_MAP(L"左人指０", L"fore0L");
		ADD_NAME_MAP(L"左人指0", L"fore0L");
		ADD_NAME_MAP(L"左人指１", L"fore1L");
		ADD_NAME_MAP(L"左人指1", L"fore1L");
		ADD_NAME_MAP(L"左人指２", L"fore2L");
		ADD_NAME_MAP(L"左人指2", L"fore2L");
		ADD_NAME_MAP(L"左中指０", L"middle0L");
		ADD_NAME_MAP(L"左中指0", L"middle0L");
		ADD_NAME_MAP(L"左中指１", L"middle1L");
		ADD_NAME_MAP(L"左中指1", L"middle1L");
		ADD_NAME_MAP(L"左中指２", L"middle2L");
		ADD_NAME_MAP(L"左中指2", L"middle2L");
		ADD_NAME_MAP(L"左薬指０", L"third0L");
		ADD_NAME_MAP(L"左薬指0", L"third0L");
		ADD_NAME_MAP(L"左薬指１", L"third1L");
		ADD_NAME_MAP(L"左薬指1", L"third1L");
		ADD_NAME_MAP(L"左薬指２", L"third2L");
		ADD_NAME_MAP(L"左薬指2", L"third2L");
		ADD_NAME_MAP(L"左小指０", L"little0L");
		ADD_NAME_MAP(L"左小指0", L"little0L");
		ADD_NAME_MAP(L"左小指１", L"little1L");
		ADD_NAME_MAP(L"左小指1", L"little1L");
		ADD_NAME_MAP(L"左小指２", L"little2L");
		ADD_NAME_MAP(L"左小指2", L"little2L");
		ADD_NAME_MAP(L"左足", L"legL");
		ADD_NAME_MAP(L"足L", L"legL");
		ADD_NAME_MAP(L"左足首", L"ankleL");
		ADD_NAME_MAP(L"足首L", L"ankleL");
		ADD_NAME_MAP(L"左ひざ", L"kneeL");
		ADD_NAME_MAP(L"左膝", L"kneeL");
		ADD_NAME_MAP(L"左足先", L"toeL");
		ADD_NAME_MAP(L"左つま先", L"toeL");
		ADD_NAME_MAP(L"左つま先ＩＫ", L"ikToeL");
		ADD_NAME_MAP(L"左足ＩＫ", L"ikLegL");
		ADD_NAME_MAP(L"左胸", L"breastL");
		ADD_NAME_MAP(L"胸L", L"breastL");
		ADD_NAME_MAP(L"左胸上", L"breastUpperL");
		ADD_NAME_MAP(L"左胸上先", L"breastUpperL");
		ADD_NAME_MAP(L"左胸上前", L"breastUpperL");
		ADD_NAME_MAP(L"左胸上前先", L"breastUpperL");
		ADD_NAME_MAP(L"左胸下", L"breastLowerL");
		ADD_NAME_MAP(L"左胸下先", L"breastLowerFrontL");
		ADD_NAME_MAP(L"左胸下前", L"breastLowerFrontL");
		ADD_NAME_MAP(L"左胸下前先", L"breastLowerFrontL");
		ADD_NAME_MAP(L"左胸先", L"breastFrontL");
		ADD_NAME_MAP(L"左胸前", L"breastFrontL");
		ADD_NAME_MAP(L"左胸前先", L"breastFrontL");
		ADD_NAME_MAP(L"左乳", L"breastL");
		ADD_NAME_MAP(L"左乳房", L"breastL");
		ADD_NAME_MAP(L"左おっぱい", L"breastL");
		ADD_NAME_MAP(L"左おっぱい上", L"breastUpperL");
		ADD_NAME_MAP(L"左おっぱい下", L"breastLowerL");
		ADD_NAME_MAP(L"左おっぱい下先", L"breastLowerFrontL");
		ADD_NAME_MAP(L"左おっぱい前", L"breastFrontL");
		ADD_NAME_MAP(L"右肩", L"shoulderR");
		ADD_NAME_MAP(L"肩R", L"shoulderR");
		ADD_NAME_MAP(L"右肩先", L"shoulderR");
		ADD_NAME_MAP(L"右腕", L"armR");
		ADD_NAME_MAP(L"腕R", L"armR");
		ADD_NAME_MAP(L"右ひじ", L"elbowR");
		ADD_NAME_MAP(L"右肘", L"elbowR");
		ADD_NAME_MAP(L"右手首", L"wristR");
		ADD_NAME_MAP(L"手首R", L"wristR");
		ADD_NAME_MAP(L"右親指０", L"thumb0R");
		ADD_NAME_MAP(L"右親指0", L"thumb0R");
		ADD_NAME_MAP(L"右親指１", L"thumb1R");
		ADD_NAME_MAP(L"右親指1", L"thumb1R");
		ADD_NAME_MAP(L"右親指２", L"thumb2R");
		ADD_NAME_MAP(L"右親指2", L"thumb2R");
		ADD_NAME_MAP(L"右人指０", L"fore0R");
		ADD_NAME_MAP(L"右人指0", L"fore0R");
		ADD_NAME_MAP(L"右人指１", L"fore1R");
		ADD_NAME_MAP(L"右人指1", L"fore1R");
		ADD_NAME_MAP(L"右人指２", L"fore2R");
		ADD_NAME_MAP(L"右人指2", L"fore2R");
		ADD_NAME_MAP(L"右中指０", L"middle0R");
		ADD_NAME_MAP(L"右中指0", L"middle0R");
		ADD_NAME_MAP(L"右中指１", L"middle1R");
		ADD_NAME_MAP(L"右中指1", L"middle1R");
		ADD_NAME_MAP(L"右中指２", L"middle2R");
		ADD_NAME_MAP(L"右中指2", L"middle2R");
		ADD_NAME_MAP(L"右薬指０", L"third0R");
		ADD_NAME_MAP(L"右薬指0", L"third0R");
		ADD_NAME_MAP(L"右薬指１", L"third1R");
		ADD_NAME_MAP(L"右薬指1", L"third1R");
		ADD_NAME_MAP(L"右薬指２", L"third2R");
		ADD_NAME_MAP(L"右薬指2", L"third2R");
		ADD_NAME_MAP(L"右小指０", L"little0R");
		ADD_NAME_MAP(L"右小指0", L"little0R");
		ADD_NAME_MAP(L"右小指１", L"little1R");
		ADD_NAME_MAP(L"右小指1", L"little1R");
		ADD_NAME_MAP(L"右小指２", L"little2R");
		ADD_NAME_MAP(L"右小指2", L"little2R");
		ADD_NAME_MAP(L"右足", L"legR");
		ADD_NAME_MAP(L"足R", L"legR");
		ADD_NAME_MAP(L"右足首", L"ankleR");
		ADD_NAME_MAP(L"足首R", L"ankleR");
		ADD_NAME_MAP(L"右ひざ", L"kneeR");
		ADD_NAME_MAP(L"右膝", L"kneeR");
		ADD_NAME_MAP(L"右足先", L"toeR");
		ADD_NAME_MAP(L"右つま先", L"toeR");
		ADD_NAME_MAP(L"右つま先ＩＫ", L"ikToeR");
		ADD_NAME_MAP(L"右足ＩＫ", L"ikLegR");
		ADD_NAME_MAP(L"右胸", L"breastR");
		ADD_NAME_MAP(L"胸R", L"breastR");
		ADD_NAME_MAP(L"右胸上", L"breastUpperR");
		ADD_NAME_MAP(L"右胸上先", L"breastUpperR");
		ADD_NAME_MAP(L"右胸上前", L"breastUpperR");
		ADD_NAME_MAP(L"右胸上前先", L"breastUpperR");
		ADD_NAME_MAP(L"右胸下", L"breastLowerR");
		ADD_NAME_MAP(L"右胸下先", L"breastLowerFrontR");
		ADD_NAME_MAP(L"右胸下前", L"breastLowerFrontR");
		ADD_NAME_MAP(L"右胸下前先", L"breastLowerFrontR");
		ADD_NAME_MAP(L"右胸先", L"breastFrontR");
		ADD_NAME_MAP(L"右胸前", L"breastFrontR");
		ADD_NAME_MAP(L"右胸前先", L"breastFrontR");
		ADD_NAME_MAP(L"右乳", L"breastR");
		ADD_NAME_MAP(L"右乳房", L"breastR");
		ADD_NAME_MAP(L"右おっぱい", L"breastR");
		ADD_NAME_MAP(L"右おっぱい上", L"breastUpperR");
		ADD_NAME_MAP(L"右おっぱい下", L"breastLowerR");
		ADD_NAME_MAP(L"右おっぱい下先", L"breastLowerFrontR");
		ADD_NAME_MAP(L"右おっぱい前", L"breastFrontR");
		ADD_NAME_MAP(L"左足", L"左足D");
		ADD_NAME_MAP(L"左ひざ", L"左ひざD");
		ADD_NAME_MAP(L"左足首", L"左足首D");
		ADD_NAME_MAP(L"左つま先", L"左足先EX");
		ADD_NAME_MAP(L"右足", L"右足D");
		ADD_NAME_MAP(L"右ひざ", L"右ひざD");
		ADD_NAME_MAP(L"右足首", L"右足首D");
		ADD_NAME_MAP(L"右つま先", L"右足先EX");
		ADD_NAME_MAP(L"腰キャンセル右", L"右腰キャンセル");
		ADD_NAME_MAP(L"腰キャンセル左", L"左腰キャンセル");
	}
}

/////////////////////////////////////////////////////////
//prototype ::from dxlib 
//创建以X轴为中心的旋转矩阵
void CreateRotationXMatrix(FMatrix* Out, float Angle);
// 求仅旋转分量矩阵的积（3）×3以外的部分也不代入值）
void MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly(FMatrix* Out, FMatrix* In1, FMatrix* In2);
// 判定角度限制的共同函数（subIndexJdg的判定比较不明…）
void CheckLimitAngle(
	const FVector& RotMin,
	const FVector& RotMax,
	FVector* outAngle, //target angle ( in and out param)
	bool subIndexJdg //(ik link index < ik loop temp):: linkBoneIndex < ikt
);
///////////////////////////////////////////////////////

UVmdFactory::UVmdFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = nullptr;
	//SupportedClass = UPmxFactory::StaticClass();
	Formats.Empty();

	Formats.Add(TEXT("vmd;vmd animations"));

	bCreateNew = false;
	bText = false;
	bEditorImport = true;

	initMmdNameMap();
}

void UVmdFactory::PostInitProperties()
{
	Super::PostInitProperties();

	ImportUI = NewObject<UVmdImportUI>(this, NAME_None, RF_NoFlags);
}

bool UVmdFactory::DoesSupportClass(UClass* Class)
{
	return (Class == UVmdFactory::StaticClass());
}

UClass* UVmdFactory::ResolveSupportedClass()
{
	return UVmdFactory::StaticClass();
}

UObject* UVmdFactory::FactoryCreateBinary
(
	UClass* Class,
	UObject* InParent,
	FName Name,
	EObjectFlags Flags,
	UObject* Context,
	const TCHAR* Type,
	const uint8*& Buffer,
	const uint8* BufferEnd,
	FFeedbackContext* Warn,
	bool& bOutOperationCanceled
)
{
	MMD4UE4::VmdMotionInfo vmdMotionInfo;

	if (vmdMotionInfo.VMDLoaderBinary(Buffer, BufferEnd) == false)
	{
		UE_LOG(LogMMD4UE4_VMDFactory, Error,
		       TEXT("VMD Import Cancel:: vmd data load faile."));
		return nullptr;
	}

	/////////////////////////////////////////
	UAnimSequence* LastCreatedAnim = nullptr;
	USkeleton* Skeleton = nullptr;
	USkeletalMesh* SkeletalMesh = nullptr;
	UIKRigDefinition* IKRig = nullptr;
	VMDImportOptions* ImportOptions = nullptr;

if (false)
	{
		/*
		//读取模型后的警告文显示：注释栏
		FText TitleStr = FText::Format(
			LOCTEXT("ImportReadMe_Generic_Dbg", "{0} 制限事項"), FText::FromString("IM4U Plugin"));
		*/
		const FText MessageDbg
			= FText(LOCTEXT("ImportReadMe_Generic_Dbg_Comment",
			                "当前有效的参数包括：\n\
				：：Skeleton Asset（必需：与动画相关联）\n\
				：：SkeletalMesh Asset（可选：Animation关联到MorphTarget.nullptr时，MorphTargetSkip。）\n\
				：：动画资源（执行仅将Morph添加到现有资源（非空值）的过程。在空值下创建包含Bone和Morph的新资源）\n\
				：：DataTable（MMD2UE4Name）Asset（任意：在nullptr以外读取时，用MMD = UE4替换Bone和MorphName，执行导入。需要事先以CSV形式导入或新建。）\n\
				：：MmdExtendAsse（可选：在nullptr以外从VMD生成AnimSeq资产时，从Extend参照IK信息进行计算时使用。必须事先导入模型或手动生成资产。）\n\
				\n\
				\n\
				注意：新Asset生成因IK等未对应而不推荐。仅支持追加Morph。"
				)
			);
#if UE_VERSION_OLDER_THAN(5,3,0)
		FMessageDialog::Open(EAppMsgType::Ok, MessageDbg, &TitleStr);
#else  
		FMessageDialog::Open(EAppMsgType::Ok, MessageDbg);  
#endif
	}

	/* 导入VMD时显示警告 */
if (false)
	{
		//读取模型后的警告文显示：注释栏
		FText TitleStr = FText(LOCTEXT("ImportVMD_TargetModelInfo", "警告[ImportVMD_TargetModelInfo]"));
		const FText MessageDbg
			= FText::Format(LOCTEXT("ImportVMD_TargetModelInfo_Comment",
			    "注意：运动数据取入信息:\\\
				\n\
				此VMD是为“{ 0 }”创建的文件。\n\
				\n\
				对于模型运动，仅捕获具有相同骨骼名称的数据。\n\
				如果包含名称与模型侧骨骼名称不同的相同骨骼，则\n\
				预先创建转换表（MMD2UE4NameTableRow），\n\
				可通过在InportOption画面中指定进行导入。"
			)
			                , FText::FromString(vmdMotionInfo.ModelName)
			);
		//FMessageDialog::Open(EAppMsgType::Ok, MessageDbg, &TitleStr);
		FMessageDialog::Open(EAppMsgType::Ok, MessageDbg);
	}

	/* factory animation asset from vmd data */

	if (vmdMotionInfo.keyCameraList.Num() == 0)
	{
		//如果不是摄影机动画
		FVmdImporter* VmdImporter = FVmdImporter::GetInstance();

		EVMDImportType ForcedImportType = VMDIT_Animation;
		bool bOperationCanceled = false;
		bool bIsPmxFormat = true;
		// show Import Option Slate
		bool bImportAll = false;
		if (!ImportUI)
		{
			ImportUI = NewObject<UVmdImportUI>(this, NAME_None, RF_NoFlags);
		}
		const FString ParentPath = InParent ? InParent->GetPathName() : FString();
		ImportUI->bIsObjImport = false; //anim mode
		ImportUI->OriginalImportType = EVMDImportType::VMDIT_Animation;
		ImportOptions
			= GetVMDImportOptions(
				VmdImporter,
				ImportUI,
				true, //bShowImportDialog, 
				ParentPath,
				bOperationCanceled,
				bImportAll,
				ImportUI->bIsObjImport, //bIsPmxFormat,
				bIsPmxFormat,
				ForcedImportType
			);
		/* 第一次判定 */
		if (ImportOptions)
		{
			Skeleton = ImportUI->Skeleton;
			SkeletalMesh = ImportUI->SkeletonMesh;
			/* 最低限度的参数设置检查 */
			if (!IsValid(Skeleton) ||
				!IsValid(SkeletalMesh) ||
				(Skeleton != SkeletalMesh->GetSkeleton()))
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Warning,
				       TEXT("[ImportAnimations]::Parameter check for Import option!"));

				{
					//	读取模型后的警告文显示：注释栏
					FText TitleStr = FText(LOCTEXT("ImportVMD_OptionWarn_CheckPh1", "警告[ImportVMD_TargetModelInfo]"));
					const FText MessageDbg
						= FText::Format(LOCTEXT("ImportVMD_OptionWarn_CheckPh1_Comment",
						                        "注意：“导入”选项的参数检查：：\n\
						\n\
						[强制要求](必須)\n\
						-骨架资源：选择目标骨架。\n\
						如果为nullptr，则表示导入错误。\n\
						[可选](任意)\n\
						-骨骼网格资源：选择目标骨骼网格。\n\
						-但是，SkellMesh包括骨架。(但是，网格必须选择相同的骨架)\n\
						如果为nullptr，则跳过导入变形曲线。(未捕获变形)\n\
						\n\
						重试导入选项！"
						)
						                , FText::FromString(vmdMotionInfo.ModelName)
						);
#if UE_VERSION_OLDER_THAN(5,3,0)
					FMessageDialog::Open(EAppMsgType::Ok, MessageDbg, &TitleStr);
#else  
					FMessageDialog::Open(EAppMsgType::Ok, MessageDbg);
#endif
				}
				/* 再来一次*/
				ImportOptions
					= GetVMDImportOptions(
						VmdImporter,
						ImportUI,
						true, //bShowImportDialog, 
						ParentPath,
						bOperationCanceled,
						bImportAll,
						ImportUI->bIsObjImport, //bIsPmxFormat,
						bIsPmxFormat,
						ForcedImportType
					);
			}
		}
		if (ImportOptions)
		{
			Skeleton = ImportUI->Skeleton;
			SkeletalMesh = ImportUI->SkeletonMesh;
			IKRig = ImportUI->IKRig;
			bool preParamChk = true; 
			/*包含关系检查*/
			if (IsValid(SkeletalMesh))
			{
				if (!IsValid(Skeleton) ||
					!IsValid(SkeletalMesh) ||
					Skeleton != SkeletalMesh->GetSkeleton())
				{
					//TBD::ERR case
					{
						UE_LOG(LogMMD4UE4_VMDFactory, Error,
						       TEXT("ImportAnimations : Skeleton not equrl skeletalmesh->skelton ...")
						);
					}
					preParamChk = false;
				}
			}
			if (preParamChk)
			{
				if (!ImportOptions->AnimSequenceAsset)
				{
					//create AnimSequence Asset from VMD
					LastCreatedAnim = ImportAnimations(
						Skeleton,
						SkeletalMesh,
						InParent,
						Name.ToString(),
						ImportUI->bSolveIK ? IKRig : nullptr,
						ImportUI->MMD2UE4NameTableRow,
						ImportUI->bSolveIK ? ImportUI->MmdExtendAsset : nullptr,
						&vmdMotionInfo,
						ImportUI->bApplyIKCollision
					);
				}
				else
				{
					//TBB::Option中未选择AinimSeq时，结束
					// add morph curve only to exist ainimation
					LastCreatedAnim = AddtionalMorphCurveImportToAnimations(
						SkeletalMesh,
						ImportOptions->AnimSequenceAsset, //UAnimSequence* exsistAnimSequ,
						ImportUI->MMD2UE4NameTableRow,
						&vmdMotionInfo
					);
				}
			}
			else
			{
				//TBD::ERR case
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Error,
					       TEXT("ImportAnimations : preParamChk false. import ERROR !!!! ...")
					);
				}
			}
		}
		else
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Warning,
			       TEXT("VMD Import Cancel"));
		}
	}
	else
	{
		//导入相机动画
		//今后安装预定？
		UE_LOG(LogMMD4UE4_VMDFactory, Warning,
		       TEXT("VMD Import Cancel::Camera root... not impl"));

		LastCreatedAnim = nullptr;
	}
	return LastCreatedAnim;
};

UAnimSequence* UVmdFactory::ImportAnimations(
	USkeleton* Skeleton,
	USkeletalMesh* SkeletalMesh,
	UObject* Outer,
	const FString& Name,
	//UFbxAnimSequenceImportData* TemplateImportData, 
	//TArray<FbxNode*>& NodeArray,
	UIKRigDefinition* IKRig,
	UDataTable* ReNameTable,
	UMMDExtendAsset* mmdExtend,
	MMD4UE4::VmdMotionInfo* vmdMotionInfo,
	bool bApplyIKCollision
)
{
	UAnimSequence* LastCreatedAnim = nullptr;
	bool bCreatedNewAsset = false;


	// we need skeleton to create animsequence
	if (!IsValid(Skeleton))
	{
		//TBD::ERR case
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Error,
			       TEXT("ImportAnimations : args Skeleton is nullptr ...")
			);
		}
		return nullptr;
	}

	{
		FString SequenceName = Name;


		SequenceName += "_";
		//SequenceName += ANSI_TO_TCHAR(CurAnimStack->GetName());
		SequenceName += Skeleton->GetName();

		UE_LOG(LogMMD4UE4_VMDFactory, Log, TEXT("VMD animation asset name: %s"), *SequenceName);
		// See if this sequence already exists.
		SequenceName = ObjectTools::SanitizeObjectName(SequenceName);

		UE_LOG(LogMMD4UE4_VMDFactory, Log, TEXT("Sanitized VMD animation asset name: %s"), *SequenceName);
		UObject* PackageOwner = IsValid(SkeletalMesh) ? static_cast<UObject*>(SkeletalMesh) : Outer;
		if (!IsValid(PackageOwner))
		{
			return nullptr;
		}
		const FString AssetDirectory = FPackageName::GetLongPackagePath(
			PackageOwner->GetOutermost()->GetName());
		const FString ParentPackagePath = FString::Printf(
			TEXT("%s/%s"),
			*AssetDirectory,
			*SequenceName);
		if (!FPackageName::IsValidLongPackageName(ParentPackagePath))
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Error,
				TEXT("ImportAnimations: invalid destination package path '%s'."),
				*ParentPackagePath);
			return nullptr;
		}
		UObject* ParentPackage = CreatePackage(*ParentPackagePath);
		if (!IsValid(ParentPackage))
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Error,
				TEXT("ImportAnimations: failed to create destination package '%s'."),
				*ParentPackagePath);
			return nullptr;
		}
		UObject* Object = LoadObject<UObject>(ParentPackage, *SequenceName, nullptr, LOAD_None, nullptr);
		UAnimSequence* DestSeq = Cast<UAnimSequence>(Object);
		// if object with same name exists, warn user
		if (Object && !DestSeq)
		{
			//AddTokenizedErrorMessage(FTokenizedMessage::Create(EMessageSeverity::Error, LOCTEXT("Error_AssetExist", "Asset with same name exists. Can't overwrite another asset")), FFbxErrors::Generic_SameNameAssetExists);
			//continue; // Move on to next sequence..
			return LastCreatedAnim;
		}

		// If not, create new one now.
		if (!DestSeq)
		{
			DestSeq = NewObject<UAnimSequence>(ParentPackage, *SequenceName, RF_Public | RF_Standalone);
			if (!IsValid(DestSeq))
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Error,
					TEXT("ImportAnimations: failed to create animation '%s' in '%s'."),
					*SequenceName,
					*ParentPackagePath);
				return nullptr;
			}

			bCreatedNewAsset = true;
		}
		else
		{
			DestSeq->ResetAnimation();
		}

		DestSeq->SetSkeleton(Skeleton);

		LastCreatedAnim = DestSeq;
	}

	// Create RawCurve -> Track Curve Key

	if (LastCreatedAnim)
	{
		bool importSuccessFlag = true;
		/*vmd animation regist*/
		if (!ImportVMDToAnimSequence(LastCreatedAnim, Skeleton, SkeletalMesh, ReNameTable, IKRig, mmdExtend, vmdMotionInfo, bApplyIKCollision))
		{
			//TBD::ERR case
			check(false);
			importSuccessFlag = false;
		}
		/*morph animation regist*/
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 2
		if (!ImportMorphCurveToAnimSequence(LastCreatedAnim, Skeleton, SkeletalMesh, ReNameTable, vmdMotionInfo))
		{
			//TBD::ERR case
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Error,TEXT("ImportMorphCurveToAnimSequence is false root..."));
			}
			//check(false);
			importSuccessFlag = false;
		}
#endif
		/*Import正常時PreviewMesh更新*/
		if ((importSuccessFlag) && IsValid(SkeletalMesh))
		{
			LastCreatedAnim->SetPreviewMesh(SkeletalMesh);
			if (SkeletalMesh->GetPhysicsAsset())
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
					TEXT("[ImportAnimations] MMD PhysicsAsset preserved: %s"),
					*SkeletalMesh->GetPhysicsAsset()->GetName());
			}
			else
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Warning,
					TEXT("[ImportAnimations] Skeletal mesh has no PhysicsAsset; import the PMX model first to add MMD physics."));
			}
			UE_LOG(LogMMD4UE4_VMDFactory, Log,
			       TEXT("[ImportAnimations] Set PreviewMesh Pointer.")
			);
		}
	}

	// end process?
	if (LastCreatedAnim)
	{
		// refresh TrackToskeletonMapIndex
		//LastCreatedAnim->RefreshTrackMapFromAnimTrackNames();
/*if (false)
		{
			//LastCreatedAnim->BakeTrackCurvesToRawAnimation();
		}
		else
		{
			// otherwise just compress
			//LastCreatedAnim->PostProcessSequence();
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION < 2
			auto& adc = LastCreatedAnim->GetController();
			adc.OpenBracket(LOCTEXT("ImportAsSkeletalMesh", "Importing VMD Animation"));
#if UE_VERSION_OLDER_THAN(5,3,0)
			adc.UpdateCurveNamesFromSkeleton(Skeleton, ERawCurveTrackTypes::RCT_Float);
#else  
            adc.UpdateCurveNamesFromSkeleton(Skeleton);
#endif

			adc.NotifyPopulated();

			adc.CloseBracket();
#endif
*/
			// mark package as dirty
			//MarkPackageDirty();  //表达式未使用
			//SkeletalMesh->MarkPackageDirty();
			LastCreatedAnim->PostEditChange();
			LastCreatedAnim->SetPreviewMesh(SkeletalMesh);
			LastCreatedAnim->MarkPackageDirty();

			if (IsValid(SkeletalMesh) && IsValid(Skeleton))
			{
				Skeleton->SetPreviewMesh(SkeletalMesh);
				Skeleton->PostEditChange();
				SkeletalMesh->MarkPackageDirty();
			}

			if (bCreatedNewAsset)
			{
				FAssetRegistryModule::AssetCreated(LastCreatedAnim);
			}
		//}
	}
	return LastCreatedAnim;
}

/*
Start
copy from: http://d.hatena.ne.jp/edvakf/touch/20111016/1318716097
x1~y2 : 0 <= xy <= 1 :bezier points
x : 0<= x <= 1 : frame rate
*/
float UVmdFactory::interpolateBezier(float x1, float y1, float x2, float y2, float x)
{
	float t = 0.5, s = 0.5;
	for (int i = 0; i < 15; i++)
	{
		float ft = (3 * s * s * t * x1) + (3 * s * t * t * x2) + (t * t * t) - x;
		if (ft == 0) break; // Math.abs(ft) < 0.00001 でもいいかも
		if (FGenericPlatformMath::Abs(ft) < 0.0001) break;
		if (ft > 0)
			t -= 1.0 / (float)(4 << i);
		else // ft < 0
			t += 1.0 / (float)(4 << i);
		s = 1 - t;
	}
	return (3 * s * s * t * y1) + (3 * s * t * t * y2) + (t * t * t);
}

/* End */

/*
将VMD表情数据添加到现有AnimSequ资源的过程
与MMD4Mecanimu的综合利用测试功能
*/
UAnimSequence* UVmdFactory::AddtionalMorphCurveImportToAnimations(
	USkeletalMesh* SkeletalMesh,
	UAnimSequence* exsistAnimSequ,
	UDataTable* ReNameTable,
	MMD4UE4::VmdMotionInfo* vmdMotionInfo
)
{
	USkeleton* Skeleton = nullptr;
	// we need skeleton to create animsequence
	if (exsistAnimSequ == nullptr)
	{
		return nullptr;
	}
	{
		//TDB::if exsite assets need fucn?
		//exsistAnimSequ->RecycleAnimSequence();

		Skeleton = exsistAnimSequ->GetSkeleton();
	}

	// Create RawCurve -> Track Curve Key

	if (exsistAnimSequ)
	{
		//exsistAnimSequ->NumFrames = vmdMotionInfo->maxFrame;
		//exsistAnimSequ->SequenceLength = FGenericPlatformMath::Max<float>(1.0f / 30.0f*(float)exsistAnimSequ->NumFrames, MINIMUM_ANIMATION_LENGTH);

		if (!ImportMorphCurveToAnimSequence(
				exsistAnimSequ,
				Skeleton,
				SkeletalMesh,
				ReNameTable,
				vmdMotionInfo)
		)
		{
			//TBD::ERR case
			check(false);
		}
	}

	// end process?

	if (exsistAnimSequ)
	{
		bool existAsset = true;

		// refresh TrackToskeletonMapIndex
		//exsistAnimSequ->RefreshTrackMapFromAnimTrackNames();
		if (existAsset)
		{
			//exsistAnimSequ->BakeTrackCurvesToRawAnimation();
		/*}
		else
		{*/
			// otherwise just compress
			//exsistAnimSequ->PostProcessSequence();
			
			auto& adc = exsistAnimSequ->GetController();
			adc.OpenBracket(LOCTEXT("ImportAsSkeletalMesh", "Importing VMD Animation"));
#if UE_VERSION_OLDER_THAN(5,3,0)
			adc.UpdateCurveNamesFromSkeleton(Skeleton, ERawCurveTrackTypes::RCT_Float);
#else  
			adc.UpdateAttributesFromSkeleton(Skeleton);
#endif
			adc.NotifyPopulated();
			adc.CloseBracket();

			// mark package as dirty
			/*MarkPackageDirty(); //表达式未使用
			SkeletalMesh->MarkPackageDirty();*/

			exsistAnimSequ->PostEditChange();
			exsistAnimSequ->SetPreviewMesh(SkeletalMesh);
			exsistAnimSequ->MarkPackageDirty();

			Skeleton->SetPreviewMesh(SkeletalMesh);
			Skeleton->PostEditChange();
			if (SkeletalMesh)
			{
				SkeletalMesh->MarkPackageDirty();
			}
		}
	}

	return exsistAnimSequ;
}
/*
导入Morph目标AnimCurve
将Morphtarget FloatCurve从VMD文件数据导入AnimSeq
*/
bool UVmdFactory::ImportMorphCurveToAnimSequence(
	UAnimSequence* DestSeq,
	USkeleton* Skeleton,
	USkeletalMesh* SkeletalMesh,
	UDataTable* ReNameTable,
	MMD4UE4::VmdMotionInfo* vmdMotionInfo
)
{
	if (!DestSeq || !Skeleton || !vmdMotionInfo)
	{
		//TBD:: ERR in Param...
		return false;
	}
	//USkeletalMesh * mesh = Skeleton->GetAssetPreviewMesh(DestSeq);// GetPreviewMesh();
	USkeletalMesh* mesh = SkeletalMesh;
	if (!mesh)
	{
		//如果进入该路径的条件在Skeleton Asset生成后一次也没有打开资源
		//nullptr的样子。比起使用这个函数，还是考虑别的手段比较好…。需要调查的范围。
		//TDB:ERR。previewMesh is nullptr
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Error,
				TEXT("ImportMorphCurveToAnimSequence GetAssetPreviewMesh Not Found...")
			);
		}
		return false;
	}
	/* morph animation regist*/

	auto& adc = DestSeq->GetController();
	//adc.OpenBracket(LOCTEXT("AddNewRawTrack_Bracket", "Adding new Morph Animation Track"));
	for (int i = 0; i < vmdMotionInfo->keyFaceList.Num(); ++i)
	{
		MMD4UE4::VmdFaceTrackList* vmdFaceTrackPtr = &vmdMotionInfo->keyFaceList[i];

		// 解析到实际存在的MorphTarget（名称表优先，回退原名）
		FName Name = ResolveMorphTargetName(mesh, ReNameTable, vmdFaceTrackPtr->TrackName);
		if (Name.IsNone())
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Log,
				TEXT("Skipping VMD morph track '%s': no matching MorphTarget on mesh."),
				*vmdFaceTrackPtr->TrackName);
			continue;
		}
		//self
		if (mesh != nullptr)
		{
			UMorphTarget* morphTargetPtr = mesh->FindMorphTarget(Name);
			if (!morphTargetPtr)
			{
				//TDB::ERR. not found Morph Target(Name) in mesh
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Warning,
						TEXT("ImportMorphCurveToAnimSequence Target Morph Not Found...Search[%s]VMD-Org[%s]"),
						*Name.ToString(), *vmdFaceTrackPtr->TrackName);
				}
			}
			else {

				if (vmdFaceTrackPtr->keyList.Num() > 0) {

					//FSmartName NewName;
#if UE_VERSION_OLDER_THAN(5,3,0)
					Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, Name, NewName);
#else  
					Skeleton->AddCurveMetaData(Name);
					Skeleton->AccumulateCurveMetaData(Name, false, true);
#endif
					FAnimationCurveIdentifier curve2;
					curve2.CurveType = ERawCurveTrackTypes::RCT_Float;
					curve2.CurveName = Name;
					TArray<FRichCurveKey> keyarrys;

					const bool bCurveAdded = adc.AddCurve(curve2);
					{

						MMD4UE4::VMD_FACE_KEY* faceKeyPtr = nullptr;
						for (int s = 0; s < vmdFaceTrackPtr->keyList.Num(); ++s)
						{
							if (!vmdFaceTrackPtr->sortIndexList.IsValidIndex(s) ||
								!vmdFaceTrackPtr->keyList.IsValidIndex(vmdFaceTrackPtr->sortIndexList[s]))
							{
								UE_LOG(LogMMD4UE4_VMDFactory, Warning,
									TEXT("Skipping invalid VMD morph key index for track '%s'"),
									*vmdFaceTrackPtr->TrackName);
								continue;
							}
							faceKeyPtr = &vmdFaceTrackPtr->keyList[vmdFaceTrackPtr->sortIndexList[s]];
							float SequenceLength;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
							SequenceLength = DestSeq->GetPlayLength();
#else
							SequenceLength = DestSeq->SequenceLength;
#endif
							float timeCurve = faceKeyPtr->Frame / 30.0f;
							if (timeCurve > SequenceLength)
							{
								//this key frame(time) more than Target SeqLength ... 
								break;
							}
							keyarrys.Add(FRichCurveKey(timeCurve, faceKeyPtr->Factor));

						}
						const bool bKeysSet = adc.SetCurveKeys(curve2, keyarrys);
						if (bKeysSet)
						{
							UE_LOG(LogMMD4UE4_VMDFactory, Log,
								TEXT("Imported VMD morph curve '%s': keys=%d, curveAdded=%s"),
								*Name.ToString(),
								keyarrys.Num(),
								bCurveAdded ? TEXT("true") : TEXT("false"));
						}
						else
						{
							UE_LOG(LogMMD4UE4_VMDFactory, Warning,
								TEXT("Failed to set VMD morph curve keys '%s': keys=%d, curveAdded=%s"),
								*Name.ToString(),
								keyarrys.Num(),
								bCurveAdded ? TEXT("true") : TEXT("false"));
						}
					}

				}

			}
		}
		
		
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
		DestSeq->Modify();	
#else
		DestSeq->MarkRawDataAsModified();
#endif
		

	}
	return true;
}

/*
Import VMD Animation
从VMD文件的数据将运动数据导入AnimSeq
*/
bool UVmdFactory::ImportVMDToAnimSequence(
	UAnimSequence* DestSeq,
	USkeleton* Skeleton,
	USkeletalMesh* SkeletalMesh,
	UDataTable* ReNameTable,
	UIKRigDefinition* IKRig,
	UMMDExtendAsset* mmdExtend,
	MMD4UE4::VmdMotionInfo* vmdMotionInfo,
	bool bApplyIKCollision
)
{
	if (!DestSeq || !Skeleton || !vmdMotionInfo)
	{
		UE_LOG(LogMMD4UE4_VMDFactory, Error,
			//%p输出指针地址 %x输出无符号十六进制
		       TEXT("ImportVMDToAnimSequence : Ref InParam is nullptr. DestSeq[%p],Skelton[%p],vmdMotionInfo[%p]"),
		      DestSeq, Skeleton, vmdMotionInfo);
		//TBD:: ERR in Param...
		return false;
	}
	if (!ReNameTable)
	{
		UE_LOG(LogMMD4UE4_VMDFactory, Warning,
		       TEXT("ImportVMDToAnimSequence : Target ReNameTable is nullptr."));
	}
	if (!mmdExtend)
	{
		UE_LOG(LogMMD4UE4_VMDFactory, Warning,
		       TEXT("ImportVMDToAnimSequence : Target MMDExtendAsset is nullptr."));
	}

	float ResampleRate = 30.f;

	auto& adc = DestSeq->GetController();
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
	adc.InitializeModel();
	//adc.OpenBracket(LOCTEXT("AddNewRawTrack_Bracket", "Adding new Bone Animation Track"));
#endif

	const FFrameRate ResampleFrameRate(ResampleRate, 1);
	adc.SetFrameRate(ResampleFrameRate);

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1

	const FFrameNumber NumberOfFrames = FGenericPlatformMath::Max<int32>((int32)vmdMotionInfo->maxFrame, 1);
	adc.SetNumberOfFrames(NumberOfFrames.Value,false);
	const int32 NumSampleFrames = NumberOfFrames.Value;
#else
	adc.NotifyPopulated();
	adc.SetPlayLength(FGenericPlatformMath::Max<float>(1.0f / 30.0f * (float)vmdMotionInfo->maxFrame, MINIMUM_ANIMATION_LENGTH));
	const int32 NumSampleFrames = FMath::Max<int32>((int32)vmdMotionInfo->maxFrame, 1);
#endif
	const int32 NumBones = Skeleton->GetReferenceSkeleton().GetNum();

	const TArray<FTransform>& RefBonePose = Skeleton->GetReferenceSkeleton().GetRefBonePose();

	TArray<FRawAnimSequenceTrack> TempRawTrackList;
	bool bLoggedVmdBoneTrackNames = false;

	check(RefBonePose.Num() == NumBones);
	// 注册与Skeleton的Bone关系@必要事项
	for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
	{
		TempRawTrackList.Add(FRawAnimSequenceTrack());
		check(BoneIndex == TempRawTrackList.Num() - 1);
		FRawAnimSequenceTrack& RawTrack = TempRawTrackList[BoneIndex];

		auto refTranslation = RefBonePose[BoneIndex].GetTranslation();

		FName targetName = Skeleton->GetReferenceSkeleton().GetBoneName(BoneIndex);;
		FName* pn = NameMap.Find(targetName);
		if (pn)
			targetName = *pn;

		if (ReNameTable)
		{
			//如果指定了转换表的资源，则从表中获取转换名称
			FMMD2UE4NameTableRow* dataRow;
			FString ContextString;
			dataRow = ReNameTable->FindRow<FMMD2UE4NameTableRow>(targetName, ContextString);
			if (dataRow)
			{
				targetName = FName(*dataRow->MmdOriginalName);
			}
		}

		//UE_LOG(LogTemp, Warning, TEXT("%s"),*targetName.ToString());
		int vmdKeyListIndex = vmdMotionInfo->FindKeyTrackName(targetName.ToString(),
		                                                      MMD4UE4::VmdMotionInfo::EVMD_KEYBONE);
		if (vmdKeyListIndex == INDEX_NONE)
		{
			auto NormalizeBoneTrackName = [](FString BoneName)
			{
				BoneName.TrimStartAndEndInline();
				while (BoneName.StartsWith(TEXT("+")) || BoneName.StartsWith(TEXT("_")))
				{
					BoneName.RightChopInline(1);
				}
				BoneName.ReplaceInline(TEXT("-"), TEXT(" "));
				BoneName.ReplaceInline(TEXT("_"), TEXT(" "));
				BoneName.TrimStartAndEndInline();

				TArray<FString> NameParts;
				BoneName.ParseIntoArrayWS(NameParts);
				return FString::Join(NameParts, TEXT(" "));
			};
			const FString NormalizedTargetName = NormalizeBoneTrackName(targetName.ToString());

			int32 NormalizedMatchIndex = INDEX_NONE;
			bool bHasAmbiguousMatch = false;
			for (int32 TrackIndex = 0; TrackIndex < vmdMotionInfo->keyBoneList.Num(); ++TrackIndex)
			{
				const FString NormalizedTrackName =
					NormalizeBoneTrackName(vmdMotionInfo->keyBoneList[TrackIndex].TrackName);
				if (NormalizedTrackName.Equals(NormalizedTargetName, ESearchCase::IgnoreCase))
				{
					if (NormalizedMatchIndex != INDEX_NONE)
					{
						bHasAmbiguousMatch = true;
						break;
					}
					NormalizedMatchIndex = TrackIndex;
				}
			}

			if (!bHasAmbiguousMatch && NormalizedMatchIndex != INDEX_NONE)
			{
				vmdKeyListIndex = NormalizedMatchIndex;
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
					TEXT("Matched VMD bone track '%s' to skeleton bone '%s' after normalizing name separators."),
					*vmdMotionInfo->keyBoneList[vmdKeyListIndex].TrackName,
					*targetName.ToString());
				const FString MatchedBoneNameString = targetName.ToString();
				if (IM4U::IsRenamedMmdBoneName(MatchedBoneNameString))
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Log,
						TEXT("Matched renamed MMD bone animation source '%s' to skeleton bone '%s' (%d sorted keys)."),
						*vmdMotionInfo->keyBoneList[vmdKeyListIndex].TrackName,
						*MatchedBoneNameString,
						vmdMotionInfo->keyBoneList[vmdKeyListIndex].sortIndexList.Num());
				}
			}
		}
		if (vmdKeyListIndex == -1)
		{
			if (!bLoggedVmdBoneTrackNames)
			{
				bLoggedVmdBoneTrackNames = true;
				UE_LOG(LogMMD4UE4_VMDFactory, Warning,
					TEXT("VMD contains %d bone tracks; listing source track names to diagnose unmatched skeleton bones."),
					vmdMotionInfo->keyBoneList.Num());
				for (int32 TrackIndex = 0; TrackIndex < vmdMotionInfo->keyBoneList.Num(); ++TrackIndex)
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Log,
						TEXT("VMD source bone track[%d]: '%s'"),
						TrackIndex,
						*vmdMotionInfo->keyBoneList[TrackIndex].TrackName);
				}
			}

			const FString TargetBoneNameString = targetName.ToString();
			// 被改名过的MMD骨骼（原'+'开头）以及附件类骨骼，单独提示便于排查
			const bool bIsAccessoryBone =
				IM4U::IsRenamedMmdBoneName(TargetBoneNameString) ||
				TargetBoneNameString.Contains(TEXT("Bowknot"), ESearchCase::IgnoreCase) ||
				TargetBoneNameString.Contains(TEXT("HairS"), ESearchCase::IgnoreCase) ||
				TargetBoneNameString.Contains(TEXT("HatB"), ESearchCase::IgnoreCase);
			// 按首字符前缀收集候选，用于区分"VMD完全没有该骨骼"与"名称写法不同"
			FString SimilarTrackNames;
			{
				// 取骨架骨骼名的第一个字符（跳过 + _ 前缀）作为比较前缀
				FString ComparePrefix = TargetBoneNameString;
				ComparePrefix.TrimStartAndEndInline();
				while (ComparePrefix.StartsWith(TEXT("+")) || ComparePrefix.StartsWith(TEXT("_")))
				{
					ComparePrefix.RightChopInline(1);
				}
				ComparePrefix = ComparePrefix.Left(1).ToLower();

				for (const MMD4UE4::VmdKeyTrackList& Track : vmdMotionInfo->keyBoneList)
				{
					FString NormalizedTrackName = Track.TrackName;
					NormalizedTrackName.TrimStartAndEndInline();
					while (NormalizedTrackName.StartsWith(TEXT("+")) || NormalizedTrackName.StartsWith(TEXT("_")))
					{
						NormalizedTrackName.RightChopInline(1);
					}
					const bool bMatchesAccessoryKeyword =
						Track.TrackName.Contains(TEXT("Bowknot"), ESearchCase::IgnoreCase) ||
						Track.TrackName.Contains(TEXT("HairS"), ESearchCase::IgnoreCase) ||
						Track.TrackName.Contains(TEXT("HatB"), ESearchCase::IgnoreCase);
					const bool bSharesNamePrefix =
						!ComparePrefix.IsEmpty() &&
						NormalizedTrackName.Left(1).ToLower() == ComparePrefix;
					if (bMatchesAccessoryKeyword || bSharesNamePrefix)
					{
						if (!SimilarTrackNames.IsEmpty())
						{
							SimilarTrackNames += TEXT(", ");
						}
						SimilarTrackNames += Track.TrackName;
					}
				}
			}
			if (bIsAccessoryBone || !SimilarTrackNames.IsEmpty())
			{
				if (SimilarTrackNames.IsEmpty())
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Warning,
						TEXT("Unmatched bone '%s'; VMD has no bone track with a similar name."),
						*TargetBoneNameString);
				}
				else
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Warning,
						TEXT("Unmatched bone '%s'; VMD has similarly named bone tracks: %s"),
						*TargetBoneNameString,
						*SimilarTrackNames);
				}
			}
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Warning,
				       TEXT("ImportVMDToAnimSequence Target Bone Not Found...[%s]"),
				       *targetName.ToString());
			}
			//未匹配到VMD轨道：不分配身份关键帧，交由AddTrack部分确保正式轨道存在
		}
		else
		{
			check(vmdKeyListIndex > -1);
			int sortIndex = 0;
			int preKeyIndex = -1;
			auto& kybone = vmdMotionInfo->keyBoneList[vmdKeyListIndex];
			if (kybone.sortIndexList.Num() == 0 ||
				!kybone.keyList.IsValidIndex(kybone.sortIndexList[0]))
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Warning,
					TEXT("ImportVMDToAnimSequence: ignoring empty or invalid track [%s]"),
					*targetName.ToString());
				continue;
			}
			//if (kybone.keyList.Num() < 2)					continue;
			int nextKeyIndex = kybone.sortIndexList[sortIndex];
			int nextKeyFrame = kybone.keyList[nextKeyIndex].Frame;
			int baseKeyFrame = 0;

			{
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
				       TEXT("ImportVMDToAnimSequence Target Bone Found...Name[%s]-KeyNum[%d]"),
				       *targetName.ToString(),
				       kybone.sortIndexList.Num());
			}

			//事先针对各轨迹，在没有父Bone的情况下，在Local坐标下计算预定全部注册的帧（如果有更好的处理……讨论）
			//如果进入90度以上的轴旋转，则由于四元数的原因或处理有错误而进入多余的旋转。
			//通过上述方式，仅通过Z旋转（旋转运动），下半身和上半身的轴成为物理上不可能的旋转的组合。臭虫。

			for (int32 i = 0; i < DestSeq->GetNumberOfSampledKeys(); i++)
			{
				if (i == 0)
				{
					if (i == nextKeyFrame)
					{
						FTransform tempTranceform(
							FQuat(
								kybone.keyList[nextKeyIndex].Quaternion[0],
								kybone.keyList[nextKeyIndex].Quaternion[2] * (-1),
								kybone.keyList[nextKeyIndex].Quaternion[1],
								kybone.keyList[nextKeyIndex].Quaternion[3]
							),
							FVector(
								kybone.keyList[nextKeyIndex].Position[0],
								kybone.keyList[nextKeyIndex].Position[2] * (-1),
								kybone.keyList[nextKeyIndex].Position[1]
							) * 10.0f,
							FVector(1, 1, 1)
						);
						//将从引用姿势移动了Key的姿势的值作为初始值
						RawTrack.PosKeys.Add(FVector3f(tempTranceform.GetTranslation() + refTranslation));
						RawTrack.RotKeys.Add(FQuat4f(tempTranceform.GetRotation()));
						RawTrack.ScaleKeys.Add(FVector3f(tempTranceform.GetScale3D()));

						preKeyIndex = nextKeyIndex;
						uint32 lastKF = nextKeyFrame;
						while (sortIndex + 1 < kybone.sortIndexList.Num() && kybone.keyList[nextKeyIndex].Frame <=
							lastKF)
						{
							sortIndex++;
							nextKeyIndex = kybone.sortIndexList[sortIndex];
						}
						lastKF = nextKeyFrame = kybone.keyList[nextKeyIndex].Frame;

						while (sortIndex + 1 < kybone.sortIndexList.Num() && kybone.keyList[kybone.sortIndexList[
							sortIndex + 1]].Frame == lastKF)
						{
							sortIndex++;
							nextKeyIndex = kybone.sortIndexList[sortIndex];
						}
						nextKeyFrame = kybone.keyList[nextKeyIndex].Frame;
					}
					else
					{
						preKeyIndex = nextKeyIndex;
						//例外处理。未为初始帧（0）设置关键帧
						FTransform nrmTrnc;
						nrmTrnc.SetIdentity();
						RawTrack.PosKeys.Add(FVector3f(nrmTrnc.GetTranslation() + refTranslation));
						RawTrack.RotKeys.Add(FQuat4f(nrmTrnc.GetRotation()));
						RawTrack.ScaleKeys.Add(FVector3f(nrmTrnc.GetScale3D()));
					}
				}
				else //if (nextKeyFrame == i)
				{
					float blendRate = 1;
					FTransform NextTranc;
					FTransform PreTranc;
					FTransform NowTranc;

					NextTranc.SetIdentity();
					PreTranc.SetIdentity();
					NowTranc.SetIdentity();

					if (nextKeyIndex > 0)
					{
						MMD4UE4::VMD_KEY& PreKey = kybone.keyList[preKeyIndex];
						MMD4UE4::VMD_KEY& NextKey = kybone.keyList[nextKeyIndex];
						if (NextKey.Frame <= (uint32)i)
						{
							blendRate = 1.0f;
						}
						else
						{
							//TBD:：帧间为1的话不以0.5计算吗？
							blendRate = 1.0f - (float)(NextKey.Frame - (uint32)i) / (float)(NextKey.Frame - PreKey.
								Frame);
						}
						//pose
						NextTranc.SetLocation(
							FVector(
								NextKey.Position[0],
								NextKey.Position[2] * (-1),
								NextKey.Position[1]
							));
						PreTranc.SetLocation(
							FVector(
								PreKey.Position[0],
								PreKey.Position[2] * (-1),
								PreKey.Position[1]
							));

						NowTranc.SetLocation(
							FVector(
								interpolateBezier(
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_0][D_VMD_KEY_BEZIER_AR_1_BEZ_X][
										D_VMD_KEY_BEZIER_AR_2_KND_X] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_0][D_VMD_KEY_BEZIER_AR_1_BEZ_Y][
										D_VMD_KEY_BEZIER_AR_2_KND_X] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_1][D_VMD_KEY_BEZIER_AR_1_BEZ_X][
										D_VMD_KEY_BEZIER_AR_2_KND_X] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_1][D_VMD_KEY_BEZIER_AR_1_BEZ_Y][
										D_VMD_KEY_BEZIER_AR_2_KND_X] / 127.0f,
									blendRate
								) * (NextTranc.GetTranslation().X - PreTranc.GetTranslation().X) + PreTranc.
								GetTranslation().X
								,
								interpolateBezier(
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_0][D_VMD_KEY_BEZIER_AR_1_BEZ_X][
										D_VMD_KEY_BEZIER_AR_2_KND_Z] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_0][D_VMD_KEY_BEZIER_AR_1_BEZ_Y][
										D_VMD_KEY_BEZIER_AR_2_KND_Z] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_1][D_VMD_KEY_BEZIER_AR_1_BEZ_X][
										D_VMD_KEY_BEZIER_AR_2_KND_Z] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_1][D_VMD_KEY_BEZIER_AR_1_BEZ_Y][
										D_VMD_KEY_BEZIER_AR_2_KND_Z] / 127.0f,
									blendRate
								) * (NextTranc.GetTranslation().Y - PreTranc.GetTranslation().Y) + PreTranc.
								GetTranslation().Y
								,
								interpolateBezier(
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_0][D_VMD_KEY_BEZIER_AR_1_BEZ_X][
										D_VMD_KEY_BEZIER_AR_2_KND_Y] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_0][D_VMD_KEY_BEZIER_AR_1_BEZ_Y][
										D_VMD_KEY_BEZIER_AR_2_KND_Y] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_1][D_VMD_KEY_BEZIER_AR_1_BEZ_X][
										D_VMD_KEY_BEZIER_AR_2_KND_Y] / 127.0f,
									NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_1][D_VMD_KEY_BEZIER_AR_1_BEZ_Y][
										D_VMD_KEY_BEZIER_AR_2_KND_Y] / 127.0f,
									blendRate
								) * (NextTranc.GetTranslation().Z - PreTranc.GetTranslation().Z) + PreTranc.
								GetTranslation().Z
							)
						);
						//rot
						NextTranc.SetRotation(
							FQuat(
								NextKey.Quaternion[0],
								NextKey.Quaternion[2] * (-1),
								NextKey.Quaternion[1],
								NextKey.Quaternion[3]
							));
						PreTranc.SetRotation(
							FQuat(
								PreKey.Quaternion[0],
								PreKey.Quaternion[2] * (-1),
								PreKey.Quaternion[1],
								PreKey.Quaternion[3]
							));

						float bezirT = interpolateBezier(
							NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_0][D_VMD_KEY_BEZIER_AR_1_BEZ_X][
								D_VMD_KEY_BEZIER_AR_2_KND_R] / 127.0f,
							NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_0][D_VMD_KEY_BEZIER_AR_1_BEZ_Y][
								D_VMD_KEY_BEZIER_AR_2_KND_R] / 127.0f,
							NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_1][D_VMD_KEY_BEZIER_AR_1_BEZ_X][
								D_VMD_KEY_BEZIER_AR_2_KND_R] / 127.0f,
							NextKey.Bezier[D_VMD_KEY_BEZIER_AR_0_BEZ_1][D_VMD_KEY_BEZIER_AR_1_BEZ_Y][
								D_VMD_KEY_BEZIER_AR_2_KND_R] / 127.0f,
							blendRate
						);
						NowTranc.SetRotation(
							FQuat::Slerp(PreTranc.GetRotation(), NextTranc.GetRotation(), bezirT)
						);
						/*UE_LOG(LogMMD4UE4_VMDFactory, Warning,
							TEXT("interpolateBezier Rot:[%s],F[%d/%d],BLD[%.2f],biz[%.2f]BEZ[%s]"),
							*targetName.ToString(), i, NextKey.Frame, blendRate, bezirT,*NowTranc.GetRotation().ToString()
							);*/
					}
					else
					{
						NowTranc.SetLocation(
							FVector(
								kybone.keyList[nextKeyIndex].Position[0],
								kybone.keyList[nextKeyIndex].Position[2] * (-1),
								kybone.keyList[nextKeyIndex].Position[1]
							));
						NowTranc.SetRotation(
							FQuat(
								kybone.keyList[nextKeyIndex].Quaternion[0],
								kybone.keyList[nextKeyIndex].Quaternion[2] * (-1),
								kybone.keyList[nextKeyIndex].Quaternion[1],
								kybone.keyList[nextKeyIndex].Quaternion[3]
							));
						//TBD:需要重新研究该路线存在的花纹、处理
						//check(false);
					}

					FTransform tempTranceform(
						NowTranc.GetRotation(),
						NowTranc.GetTranslation() * 10.0f,
						FVector(1, 1, 1)
					);
					//将从引用姿势移动了Key的姿势的值作为初始值
					RawTrack.PosKeys.Add(FVector3f(tempTranceform.GetTranslation() + refTranslation));
					RawTrack.RotKeys.Add(FQuat4f(tempTranceform.GetRotation()));
					RawTrack.ScaleKeys.Add(FVector3f(tempTranceform.GetScale3D()));

					if (nextKeyFrame == i)
					{

						preKeyIndex = nextKeyIndex;
						uint32 lastKF = nextKeyFrame;
						while (sortIndex + 1 < kybone.sortIndexList.Num() && kybone.keyList[nextKeyIndex].Frame <=
							lastKF)
						{
							sortIndex++;
							nextKeyIndex = kybone.sortIndexList[sortIndex];
						}
						lastKF = nextKeyFrame = kybone.keyList[nextKeyIndex].Frame;

						while (sortIndex + 1 < kybone.sortIndexList.Num() && kybone.keyList[kybone.sortIndexList[
							sortIndex + 1]].Frame == lastKF)
						{
							sortIndex++;
							nextKeyIndex = kybone.sortIndexList[sortIndex];
						}
						nextKeyFrame = kybone.keyList[nextKeyIndex].Frame;
					}
				}
			}
		}
	}
	adc.OpenBracket(LOCTEXT("AddNewRawTrack_Bracket", "Adding new Bone Animation Track"));
	/* IK/碰撞求解：有ExtendAsset的IK链时修正逐帧局部姿态（IK开关帧可逐帧开关IK） */
	ApplyIkSolveToBoneTracks(
		Skeleton,
		TempRawTrackList,
		TempRawTrackList,
		IKRig,
		mmdExtend,
		vmdMotionInfo,
		NumSampleFrames,
		RefBonePose,
		bApplyIKCollision);

	/* 刚体物理烘焙：把MMD刚体演算结果写入轨道，播放动画即带摆动 */
	if (IsValid(SkeletalMesh))
	{
		BakeRigidBodyMotion(
			SkeletalMesh,
			Skeleton->GetReferenceSkeleton(),
			TempRawTrackList,
			NumSampleFrames,
			ResampleRate,
			mmdExtend);
	}

	/* AddTrack */
	for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
	{
		FName BoneName = Skeleton->GetReferenceSkeleton().GetBoneName(BoneIndex);

		FRawAnimSequenceTrack& RawTrack = TempRawTrackList[BoneIndex];

		//DestSeq->AddNewRawTrack(BoneName, &RawTrack);

		const bool bHasTrackKeys =
			RawTrack.PosKeys.Num() > 0 ||
			RawTrack.RotKeys.Num() > 0 ||
			RawTrack.ScaleKeys.Num() > 0;
		// 物理骨骼的摆动已烘焙进轨道，需要正常写入
		if (bHasTrackKeys)
		{
			bool bTrackExists = false;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
			if (const IAnimationDataModel* DataModel = DestSeq->GetDataModel())
			{
				TArray<FName> ExistingTrackNames;
				DataModel->GetBoneTrackNames(ExistingTrackNames);
				bTrackExists = ExistingTrackNames.Contains(BoneName);
			}
			if (!bTrackExists)
			{
				bTrackExists = adc.AddBoneCurve(BoneName);
			}
#else
			bTrackExists = adc.AddBoneTrack(BoneName) != INDEX_NONE;
#endif

			if (!bTrackExists)
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Warning,
					TEXT("Could not create or find animation track for skeleton bone '%s'."),
					*BoneName.ToString());
				continue;
			}

			{
				if (BoneName == L"腰") {
					for (int32 ix = 0; ix < RawTrack.PosKeys.Num(); ix++) {
						//RawTrack.PosKeys[ix] = FVector3f(0.0f,0.0f,0.0f);
						RawTrack.PosKeys[ix].X = 0.0f;
						RawTrack.PosKeys[ix].Y = 0.0f;
						RawTrack.RotKeys[ix] = FQuat4f(0.0f, 0.0f, 0.0f,1.0f);
					}
					//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), RawTrack.PosKeys[1].X, RawTrack.PosKeys[1].Y, RawTrack.PosKeys[1].Z);//看看有多少个
				}

				const FString BoneNameString = BoneName.ToString();
				const bool bIsMmdMotionDiagnosticBone =
					IM4U::IsRenamedMmdBoneName(BoneNameString);
				if (bIsMmdMotionDiagnosticBone)
				{
					float MaxPositionDelta = 0.0f;
					float MaxRotationDeltaDegrees = 0.0f;
					if (RawTrack.PosKeys.Num() > 0 && RawTrack.RotKeys.Num() > 0)
					{
						const FVector FirstPosition(RawTrack.PosKeys[0]);
						const FQuat4f& FirstRotation = RawTrack.RotKeys[0];
						for (int32 KeyIndex = 1;
							KeyIndex < FMath::Min(RawTrack.PosKeys.Num(), RawTrack.RotKeys.Num());
							++KeyIndex)
						{
							MaxPositionDelta = FMath::Max(
								MaxPositionDelta,
								FVector::Distance(FirstPosition, FVector(RawTrack.PosKeys[KeyIndex])));
							const FQuat4f& Rotation = RawTrack.RotKeys[KeyIndex];
							const float AbsDot = FMath::Clamp(FMath::Abs(
								FirstRotation.X * Rotation.X +
								FirstRotation.Y * Rotation.Y +
								FirstRotation.Z * Rotation.Z +
								FirstRotation.W * Rotation.W), 0.0f, 1.0f);
							MaxRotationDeltaDegrees = FMath::Max(
								MaxRotationDeltaDegrees,
								FMath::RadiansToDegrees(2.0f * FMath::Acos(AbsDot)));
						}
					}
					UE_LOG(LogMMD4UE4_VMDFactory, Log,
						TEXT("MMD animation track '%s': keys=%d, max position delta=%.4f, max rotation delta=%.4f degrees."),
						*BoneNameString,
						RawTrack.PosKeys.Num(),
						MaxPositionDelta,
						MaxRotationDeltaDegrees);
				}

				const bool bTrackKeysSet = adc.SetBoneTrackKeys(
					BoneName,
					RawTrack.PosKeys,
					RawTrack.RotKeys,
					RawTrack.ScaleKeys);
				if (!bTrackKeysSet)
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Error,
						TEXT("Failed to write animation keys for bone '%s' (position=%d, rotation=%d, scale=%d, sequence frames=%d)."),
						*BoneNameString,
						RawTrack.PosKeys.Num(),
						RawTrack.RotKeys.Num(),
						RawTrack.ScaleKeys.Num(),
						DestSeq->GetNumberOfSampledKeys());
				}
				else if (bIsMmdMotionDiagnosticBone)
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Log,
						TEXT("Animation keys accepted by controller for '%s'."),
						*BoneNameString);
				}
				
			}
		}
	}
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
	USkeletalMesh* MorphMesh = SkeletalMesh ? SkeletalMesh : Skeleton->GetPreviewMesh();
	if (!MorphMesh && vmdMotionInfo->keyFaceList.Num() > 0)
	{
		UE_LOG(LogMMD4UE4_VMDFactory, Warning,
			TEXT("ImportVMDToAnimSequence: no skeletal mesh available to resolve VMD morph targets."));
	}
	for (int i = 0; i < vmdMotionInfo->keyFaceList.Num(); ++i)
	{
		MMD4UE4::VmdFaceTrackList* vmdFaceTrackPtr = &vmdMotionInfo->keyFaceList[i];

		// VMD stores the MMD-side name; resolve it to the UE morph target name.
		FName Name = *vmdFaceTrackPtr->TrackName;
		if (Name.IsNone())
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Warning,
				TEXT("Skipping VMD morph track with an empty name."));
			continue;
		}
		// 解析到实际存在的MorphTarget（名称表优先，回退原名）
		Name = ResolveMorphTargetName(MorphMesh, ReNameTable, vmdFaceTrackPtr->TrackName);
		if (Name.IsNone())
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Log,
				TEXT("Skipping VMD morph track '%s': no matching MorphTarget on mesh."),
				*vmdFaceTrackPtr->TrackName);
			continue;
		}
		if (MorphMesh != nullptr)
		{
			UMorphTarget* morphTargetPtr = MorphMesh->FindMorphTarget(Name);
			if (!morphTargetPtr)
			{
				//TDB::ERR. not found Morph Target(Name) in mesh
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Warning,
						TEXT("ImportMorphCurveToAnimSequence Target Morph Not Found...Search[%s]VMD-Org[%s]"),
						*Name.ToString(), *vmdFaceTrackPtr->TrackName);
				}
			}
			else {

				if (vmdFaceTrackPtr->keyList.Num() > 0) {

					//FSmartName NewName;
#if UE_VERSION_OLDER_THAN(5,3,0)
					Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, Name, NewName);
#else  
					Skeleton->AddCurveMetaData(Name);
					Skeleton->AccumulateCurveMetaData(Name, false, true);
#endif
					FAnimationCurveIdentifier curve2;
					curve2.CurveType = ERawCurveTrackTypes::RCT_Float;
					curve2.CurveName = Name;
					TArray<FRichCurveKey> keyarrys;

					const bool bCurveAdded = adc.AddCurve(curve2);
					{

						MMD4UE4::VMD_FACE_KEY* faceKeyPtr = nullptr;
						for (int s = 0; s < vmdFaceTrackPtr->keyList.Num(); ++s)
						{
							if (!vmdFaceTrackPtr->sortIndexList.IsValidIndex(s) ||
								!vmdFaceTrackPtr->keyList.IsValidIndex(vmdFaceTrackPtr->sortIndexList[s]))
							{
								UE_LOG(LogMMD4UE4_VMDFactory, Warning,
									TEXT("Skipping invalid VMD morph key index for track '%s'"),
									*vmdFaceTrackPtr->TrackName);
								continue;
							}
							faceKeyPtr = &vmdFaceTrackPtr->keyList[vmdFaceTrackPtr->sortIndexList[s]];
							float SequenceLength;
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
							SequenceLength = DestSeq->GetPlayLength();
#else
							SequenceLength = DestSeq->SequenceLength;
#endif
							float timeCurve = faceKeyPtr->Frame / 30.0f;
							if (timeCurve > SequenceLength)
							{
								//this key frame(time) more than Target SeqLength ... 
								break;
							}
							keyarrys.Add(FRichCurveKey(timeCurve, faceKeyPtr->Factor));

						}
						const bool bKeysSet = adc.SetCurveKeys(curve2, keyarrys);
						if (bKeysSet)
						{
							UE_LOG(LogMMD4UE4_VMDFactory, Log,
								TEXT("Imported VMD morph curve '%s': keys=%d, curveAdded=%s"),
								*Name.ToString(),
								keyarrys.Num(),
								bCurveAdded ? TEXT("true") : TEXT("false"));
						}
						else
						{
							UE_LOG(LogMMD4UE4_VMDFactory, Warning,
								TEXT("Failed to set VMD morph curve keys '%s': keys=%d, curveAdded=%s"),
								*Name.ToString(),
								keyarrys.Num(),
								bCurveAdded ? TEXT("true") : TEXT("false"));
						}
					}

				}

			}
		}


#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION > 1
		DestSeq->Modify();
#else
		DestSeq->MarkRawDataAsModified();
#endif

	}
#if UE_VERSION_OLDER_THAN(5,3,0)
	adc.UpdateCurveNamesFromSkeleton(Skeleton, ERawCurveTrackTypes::RCT_Float);
#else  
	adc.UpdateAttributesFromSkeleton(Skeleton);
#endif
	adc.NotifyPopulated();
	if (const IAnimationDataModel* ControllerModel = adc.GetModel())
	{
		TArray<FName> ControllerTrackNames;
		ControllerModel->GetBoneTrackNames(ControllerTrackNames);
		UE_LOG(LogMMD4UE4_VMDFactory, Log,
			TEXT("After NotifyPopulated, controller model has %d tracks; sequence model pointer match=%s."),
			ControllerTrackNames.Num(),
			DestSeq->GetDataModel() == ControllerModel ? TEXT("true") : TEXT("false"));
		for (const FName TrackName : ControllerTrackNames)
		{
			if (IM4U::IsRenamedMmdBoneName(TrackName))
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
					TEXT("Controller model renamed MMD bone track name: '%s'."),
					*TrackName.ToString());
			}
		}
	}
#else
	adc.NotifyPopulated();
#endif
	
	adc.CloseBracket();
	//GWarn->EndSlowTask();
	return true;
}

/*
IK/碰撞求解（CCD-IK）
使用MMDExtendAsset的IK链信息与VMD的IK开关关键帧，逐帧修正局部姿态。
碰撞体现为：链路骨骼受MMD角度限制（RotLockFlag / RotLockMin / RotLockMax）约束。
*/
void UVmdFactory::SolveIkForFrame(
	const FReferenceSkeleton& RefSkeleton,
	TArray<FMMD_IKInfo> IkInfoList,
	TArray<FTransform>& LocalTransforms,
	const TArray<FTransform>& RefPose,
	int32 Frame,
	bool bApplyIKCollision
)
{
	const int32 NumBones = RefSkeleton.GetNum();
	if (NumBones <= 0)
	{
		return;
	}
	LocalTransforms.SetNum(FMath::Max(NumBones, LocalTransforms.Num()));

	auto CalcGlobal = [&RefSkeleton, &LocalTransforms](int32 BoneIndex)
	{
		TArray<int32, TInlineAllocator<32>> Chain;
		int32 current = BoneIndex;
		while (current != INDEX_NONE && RefSkeleton.IsValidIndex(current))
		{
			Chain.Add(current);
			current = RefSkeleton.GetParentIndex(current);
		}
		FTransform result = FTransform::Identity;
		for (int32 chainIndex = Chain.Num() - 1; chainIndex >= 0; --chainIndex)
		{
			result = LocalTransforms[Chain[chainIndex]] * result;
		}
		return result;
	};

		(void)Frame;

		// 逐次迭代整体计算链路，链深较小，开销可控
		for (int32 ikIndex = 0; ikIndex < IkInfoList.Num(); ++ikIndex)
	{
		const FMMD_IKInfo& ikInfo = IkInfoList[ikIndex];
		if (!RefSkeleton.IsValidIndex(ikInfo.IKBoneIndex) ||
			!RefSkeleton.IsValidIndex(ikInfo.TargetBoneIndex))
		{
			continue;
		}

		const int32 ikBoneIndex = ikInfo.IKBoneIndex;
		const int32 targetBoneIndex = ikInfo.TargetBoneIndex;
		// 碰撞开启时提高迭代次数，让角度限制收敛
		const int32 defaultLoop = (bApplyIKCollision && ikInfo.bEnableCollision) ? 32 : 8;
		const int32 loopCount = FMath::Clamp(ikInfo.LoopNum > 0 ? ikInfo.LoopNum : defaultLoop, 1, 255);		const float rotLimit = ikInfo.RotLimit;

		for (int32 loop = 0; loop < loopCount; ++loop)
		{
			const FTransform ikGlobal = CalcGlobal(ikBoneIndex);
			const FTransform targetGlobal = CalcGlobal(targetBoneIndex);
			if (targetGlobal.GetLocation().Equals(ikGlobal.GetLocation(), 0.01f))
			{
				break;
			}

			// 从离IK目标最远的链接骨骼开始求解
			int32 maxDistanceLink = INDEX_NONE;
			float maxDistanceSq = -1.0f;
			for (int32 linkIndex = 0; linkIndex < ikInfo.ikLinkList.Num(); ++linkIndex)
			{
				const int32 linkBoneIndex = ikInfo.ikLinkList[linkIndex].BoneIndex;
				if (!RefSkeleton.IsValidIndex(linkBoneIndex))
				{
					continue;
				}
				if (!RefSkeleton.BoneIsChildOf(linkBoneIndex, ikBoneIndex))
				{
					continue;
				}
				if (RefSkeleton.BoneIsChildOf(linkBoneIndex, targetBoneIndex))
				{
					continue;
				}
				const FTransform linkGlobal = CalcGlobal(linkBoneIndex);
				const float distanceSq =
					(ikGlobal.GetLocation() - linkGlobal.GetLocation()).SizeSquared();
				if (distanceSq > maxDistanceSq)
				{
					maxDistanceSq = distanceSq;
					maxDistanceLink = linkIndex;
				}
			}
			if (maxDistanceLink == INDEX_NONE)
			{
				break;
			}

			const FMMD_IKLINK& ikLink = ikInfo.ikLinkList[maxDistanceLink];
			const int32 linkBoneIndex = ikLink.BoneIndex;
			const int32 linkParentIndex = RefSkeleton.GetParentIndex(linkBoneIndex);
			if (!RefSkeleton.IsValidIndex(linkBoneIndex))
			{
				break;
			}

			const FTransform linkGlobal = CalcGlobal(linkBoneIndex);
			const FTransform ikGlobalNow = CalcGlobal(ikBoneIndex);
			const FTransform targetGlobalNow = CalcGlobal(targetBoneIndex);
			const FTransform parentGlobal = linkParentIndex != INDEX_NONE
				? CalcGlobal(linkParentIndex)
				: FTransform::Identity;

			FVector linkToIk = ikGlobalNow.GetLocation() - linkGlobal.GetLocation();
			FVector linkToTarget = targetGlobalNow.GetLocation() - linkGlobal.GetLocation();
			if (linkToIk.IsNearlyZero() || linkToTarget.IsNearlyZero())
			{
				break;
			}
			linkToIk.Normalize();
			linkToTarget.Normalize();

			// 限制单次迭代的旋转量
			FQuat addWorldRot = FQuat::FindBetweenNormals(linkToIk, linkToTarget);
			if (rotLimit > 0.0f && addWorldRot.GetAngle() > rotLimit)
			{
				addWorldRot = FQuat(addWorldRot.GetRotationAxis(), rotLimit);
			}
			if (addWorldRot.GetAngle() < KINDA_SMALL_NUMBER)
			{
				break;
			}

			// 新的世界旋转换算回局部旋转：Local = ParentWorld^-1 * NewWorld
			const FQuat newWorldRot = addWorldRot * linkGlobal.GetRotation();
			FQuat newLocalRot = parentGlobal.GetRotation().Inverse() * newWorldRot;

			if (bApplyIKCollision && ikLink.RotLockFlag)
			{
				// 碰撞：把链接骨骼的旋转夹取到MMD角度限制范围内
				const FRotator localRotator(newLocalRot);
				const FRotator clampedRotator(
					FMath::Clamp(localRotator.Pitch, ikLink.RotLockMin.X, ikLink.RotLockMax.X),
					FMath::Clamp(localRotator.Yaw, ikLink.RotLockMin.Y, ikLink.RotLockMax.Y),
					FMath::Clamp(localRotator.Roll, ikLink.RotLockMin.Z, ikLink.RotLockMax.Z));
				newLocalRot = clampedRotator.Quaternion();
			}

			LocalTransforms[linkBoneIndex].SetRotation(newLocalRot.GetNormalized());
		}
	}
}

void UVmdFactory::ApplyIkSolveToBoneTracks(
	USkeleton* Skeleton,
	const TArray<FRawAnimSequenceTrack>& InRefTracks,
	TArray<FRawAnimSequenceTrack>& OutTracks,
	UIKRigDefinition* IKRig,
	UMMDExtendAsset* mmdExtend,
	MMD4UE4::VmdMotionInfo* vmdMotionInfo,
	int32 NumFrames,
	const TArray<FTransform>& RefPose,
	bool bApplyIKCollision
)
{
	if (!Skeleton || !vmdMotionInfo || NumFrames <= 0)
	{
		return;
	}

	(void)InRefTracks;

	const bool bHasIkInfo = IsValid(mmdExtend) && mmdExtend->IkInfoList.Num() > 0;
	const bool bHasIkSwitchKeys = vmdMotionInfo->keyIkList.Num() > 0;
	if (!bHasIkInfo)
	{
		// 没有ExtendAsset的IK链时无法逐帧求解，保留VMD原始姿态
		if (IKRig || bHasIkSwitchKeys)
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Warning,
				TEXT("VMD IK solve skipped: MMDExtendAsset has no IK info (raw VMD pose kept)."));
		}
		return;
	}

	const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();
	const int32 NumBones = RefSkeleton.GetNum();
	const int32 NumKeys = OutTracks.Num();
	const int32 SolveFrameCount = FMath::Min<int32>(NumFrames, NumKeys);
	if (NumBones <= 0 || SolveFrameCount <= 0)
	{
		return;
	}

	// 仅保留能在骨架中定位到的IK链
	TArray<FMMD_IKInfo> IkInfoList;
	IkInfoList.Reserve(mmdExtend->IkInfoList.Num());
	for (int32 ikIndex = 0; ikIndex < mmdExtend->IkInfoList.Num(); ++ikIndex)
	{
		FMMD_IKInfo ikInfo = mmdExtend->IkInfoList[ikIndex];
		if (ikInfo.IKBoneIndex == INDEX_NONE)
		{
			ikInfo.IKBoneIndex = RefSkeleton.FindBoneIndex(ikInfo.IKBoneName);
		}
		if (ikInfo.TargetBoneIndex == INDEX_NONE)
		{
			ikInfo.TargetBoneIndex = RefSkeleton.FindBoneIndex(ikInfo.TargetBoneName);
		}
		for (int32 linkIndex = 0; linkIndex < ikInfo.ikLinkList.Num(); ++linkIndex)
		{
			FMMD_IKLINK& ikLink = ikInfo.ikLinkList[linkIndex];
			if (ikLink.BoneIndex == INDEX_NONE)
			{
				ikLink.BoneIndex = RefSkeleton.FindBoneIndex(ikLink.BoneName);
			}
		}
		if (RefSkeleton.IsValidIndex(ikInfo.IKBoneIndex) &&
			RefSkeleton.IsValidIndex(ikInfo.TargetBoneIndex))
		{
			ikInfo.checkIKIndex = true;
			IkInfoList.Add(ikInfo);
		}
	}
	if (IkInfoList.Num() == 0)
	{
		UE_LOG(LogMMD4UE4_VMDFactory, Warning,
			TEXT("VMD IK solve skipped: no IK chain matched the skeleton (raw VMD pose kept)."));
		return;
	}

	// VMD名称按15字节截断，去重后用于匹配IK开关帧
	auto FindVmdIkBoneName = [vmdMotionInfo](const FName& IKBoneName) -> FName
	{
		if (IKBoneName.IsNone())
		{
			return IKBoneName;
		}
		const FString ikBoneString = IKBoneName.ToString();
		for (int32 trackIndex = 0; trackIndex < vmdMotionInfo->keyIkList.Num(); ++trackIndex)
		{
			if (vmdMotionInfo->keyIkList[trackIndex].TrackName == ikBoneString)
			{
				return IKBoneName;
			}
		}
		// 15byte截断匹配（例如"右足ＩＫ"被截断）
		const FString truncated = ikBoneString.Left(15);
		for (int32 trackIndex = 0; trackIndex < vmdMotionInfo->keyIkList.Num(); ++trackIndex)
		{
			if (vmdMotionInfo->keyIkList[trackIndex].TrackName == truncated)
			{
				return FName(*truncated);
			}
		}
		return IKBoneName;
	};

	const bool bUseSwitchKeys = vmdMotionInfo->keyIkList.Num() > 0;
	const bool bDefaultIkEnabled =
		!vmdMotionInfo->bHasVisibleIKExtension || vmdMotionInfo->bAllIkEnabled;

	TArray<FTransform> LocalTransforms;
	LocalTransforms.SetNum(NumBones);

	int32 SolvedFrameCount = 0;
	for (int32 Frame = 0; Frame < SolveFrameCount; ++Frame)
	{
		TArray<FMMD_IKInfo> ActiveIkList;
		for (int32 ikIndex = 0; ikIndex < IkInfoList.Num(); ++ikIndex)
		{
			const FMMD_IKInfo& ikInfo = IkInfoList[ikIndex];
			bool bIkEnabled = bDefaultIkEnabled;
			if (bUseSwitchKeys)
			{
				bIkEnabled = vmdMotionInfo->GetIkEnabledAtFrame(
					FindVmdIkBoneName(ikInfo.IKBoneName),
					static_cast<uint32>(Frame),
					bDefaultIkEnabled);
			}
			if (bIkEnabled)
			{
				ActiveIkList.Add(ikInfo);
			}
		}
		if (ActiveIkList.Num() == 0)
		{
			continue;
		}

		for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
		{
			const FRawAnimSequenceTrack& Track = OutTracks[BoneIndex];
			const bool bHasPos = Track.PosKeys.IsValidIndex(Frame);
			const bool bHasRot = Track.RotKeys.IsValidIndex(Frame);
			const bool bHasScale = Track.ScaleKeys.IsValidIndex(Frame);
			if (!bHasPos && !bHasRot && !bHasScale)
			{
				LocalTransforms[BoneIndex] = RefPose.IsValidIndex(BoneIndex)
					? RefPose[BoneIndex]
					: FTransform::Identity;
				continue;
			}
			const FVector RefTranslation = RefPose.IsValidIndex(BoneIndex)
				? RefPose[BoneIndex].GetTranslation()
				: FVector::ZeroVector;
			LocalTransforms[BoneIndex] = FTransform(
				bHasRot ? FQuat(Track.RotKeys[Frame]) : FQuat::Identity,
				bHasPos ? FVector(Track.PosKeys[Frame]) - RefTranslation : FVector::ZeroVector,
				bHasScale ? FVector(Track.ScaleKeys[Frame]) : FVector::OneVector);
		}

		SolveIkForFrame(RefSkeleton, ActiveIkList, LocalTransforms, RefPose, Frame, bApplyIKCollision);

		for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
		{
			FRawAnimSequenceTrack& Track = OutTracks[BoneIndex];
			const bool bHasPos = Track.PosKeys.IsValidIndex(Frame);
			const bool bHasRot = Track.RotKeys.IsValidIndex(Frame);
			if (!bHasPos && !bHasRot)
			{
				continue;
			}
			if (bHasRot)
			{
				Track.RotKeys[Frame] = FQuat4f(LocalTransforms[BoneIndex].GetRotation());
			}
			if (bHasPos)
			{
				const FVector RefTranslation = RefPose.IsValidIndex(BoneIndex)
					? RefPose[BoneIndex].GetTranslation()
					: FVector::ZeroVector;
				Track.PosKeys[Frame] =
					FVector3f(LocalTransforms[BoneIndex].GetTranslation() + RefTranslation);
			}
		}
		++SolvedFrameCount;
	}

	bool bAnyCollision = false;
	for (int32 ikIndex = 0; ikIndex < IkInfoList.Num(); ++ikIndex)
	{
		bAnyCollision |= IkInfoList[ikIndex].bEnableCollision;
	}
	UE_LOG(LogMMD4UE4_VMDFactory, Log,
		TEXT("VMD IK solve: ikChains=%d, frames=%d/%d, ikSwitchKeys=%s, collision=%s, ikRig=%s"),
		IkInfoList.Num(),
		SolvedFrameCount,
		SolveFrameCount,
		bUseSwitchKeys ? TEXT("on") : TEXT("off"),
		bAnyCollision ? TEXT("on") : TEXT("off"),
		IKRig ? TEXT("set") : TEXT("none"));
}



//////////////////////////////////////////////////////////////////////////////////////
// 创建以X轴为中心的旋转矩阵
void CreateRotationXMatrix(FMatrix* Out, float Angle)
{
	float Sin, Cos;

	//_SINCOS(Angle, &Sin, &Cos);
	//	Sin = sinf( Angle ) ;
	//	Cos = cosf( Angle ) ;
	Sin = FMath::Sin(Angle);
	Cos = FMath::Cos(Angle);

	//_MEMSET(Out, 0, sizeof(MATRIX));
	FMemory::Memzero(Out, sizeof(FMatrix));
	Out->M[0][0] = 1.0f;
	Out->M[1][1] = Cos;
	Out->M[1][2] = Sin;
	Out->M[2][1] = -Sin;
	Out->M[2][2] = Cos;
	Out->M[3][3] = 1.0f;

	//return 0;
}

// 求仅旋转分量矩阵的积（3）×3以外的部分也不代入值）
void MV1LoadModelToVMD_CreateMultiplyMatrixRotOnly(FMatrix* Out, FMatrix* In1, FMatrix* In2)
{
	Out->M[0][0] = In1->M[0][0] * In2->M[0][0] + In1->M[0][1] * In2->M[1][0] + In1->M[0][2] * In2->M[2][0];
	Out->M[0][1] = In1->M[0][0] * In2->M[0][1] + In1->M[0][1] * In2->M[1][1] + In1->M[0][2] * In2->M[2][1];
	Out->M[0][2] = In1->M[0][0] * In2->M[0][2] + In1->M[0][1] * In2->M[1][2] + In1->M[0][2] * In2->M[2][2];

	Out->M[1][0] = In1->M[1][0] * In2->M[0][0] + In1->M[1][1] * In2->M[1][0] + In1->M[1][2] * In2->M[2][0];
	Out->M[1][1] = In1->M[1][0] * In2->M[0][1] + In1->M[1][1] * In2->M[1][1] + In1->M[1][2] * In2->M[2][1];
	Out->M[1][2] = In1->M[1][0] * In2->M[0][2] + In1->M[1][1] * In2->M[1][2] + In1->M[1][2] * In2->M[2][2];

	Out->M[2][0] = In1->M[2][0] * In2->M[0][0] + In1->M[2][1] * In2->M[1][0] + In1->M[2][2] * In2->M[2][0];
	Out->M[2][1] = In1->M[2][0] * In2->M[0][1] + In1->M[2][1] * In2->M[1][1] + In1->M[2][2] * In2->M[2][1];
	Out->M[2][2] = In1->M[2][0] * In2->M[0][2] + In1->M[2][1] * In2->M[1][2] + In1->M[2][2] * In2->M[2][2];
}

/////////////////////////////////////
// 判定角度限制的共同函数（subIndexJdg的判定比较不明…）
void CheckLimitAngle(
	const FVector& RotMin,
	const FVector& RotMax,
	FVector* outAngle, //target angle ( in and out param)
	bool subIndexJdg //(ik link index < ik loop temp):: linkBoneIndex < ikt
)
{
	//#define DEBUG_CheckLimitAngle
#ifdef DEBUG_CheckLimitAngle
	FVector debugVec = *outAngle;
#endif

	*outAngle = ClampVector(
		*outAngle,
		RotMin,
		RotMax
	);

	//debug
#ifdef DEBUG_CheckLimitAngle
	UE_LOG(LogMMD4UE4_VMDFactory, Log,
		TEXT("CheckLimitAngle::out[%s]<-In[%s]:MI[%s]MX[%s]"),
		*outAngle->ToString(),
		*debugVec.ToString(),
		*RotMin.ToString(),
		*RotMax.ToString()
		);
#endif
}

//////////////////////////////////////////////////////////////////////////////////////

/*
从MMD侧的名称检索并取得TableRow的UE侧名称
Return :T is Found
@param :ue4Name is Found Row Name
*/
bool UVmdFactory::FindTableRowMMD2UEName(
	UDataTable* ReNameTable,
	FName mmdName,
	FName* ue4Name
)
{
	if (ReNameTable == nullptr || ue4Name == nullptr)
	{
		return false;
	}

	TArray<FName> getTableNames = ReNameTable->GetRowNames();

	FMMD2UE4NameTableRow* dataRow;
	FString ContextString;
	for (int i = 0; i < getTableNames.Num(); ++i)
	{
		ContextString = "";
		dataRow = ReNameTable->FindRow<FMMD2UE4NameTableRow>(getTableNames[i], ContextString);
		if (dataRow)
		{
			if (mmdName == FName(*dataRow->MmdOriginalName))
			{
				*ue4Name = getTableNames[i];
				return true;
			}
		}
	}
	return false;
}

/*
解析VMD morph名 -> 实际MorphTarget名
顺序：名称表映射 -> 原名 -> 名称表反向匹配
@return 命中的名字；未命中返回NAME_None
*/
FName UVmdFactory::ResolveMorphTargetName(
	USkeletalMesh* MorphMesh,
	UDataTable* ReNameTable,
	const FString& VmdTrackName
)
{
	if (!IsValid(MorphMesh) || VmdTrackName.IsEmpty())
	{
		return NAME_None;
	}

	// 1) 名称表映射（MMD原名 -> UE名）
	FName MappedName;
	if (ReNameTable && FindTableRowMMD2UEName(ReNameTable, FName(*VmdTrackName), &MappedName))
	{
		if (MorphMesh->FindMorphTarget(MappedName))
		{
			return MappedName;
		}
	}

	// 2) 直接用原名（PMX导入的MorphTarget就是原样命名，VMD也存原名，通常这一步命中）
	const FName DirectName(*VmdTrackName);
	if (MorphMesh->FindMorphTarget(DirectName))
	{
		return DirectName;
	}

	// 3) 名称表反向：VMD里存的名字可能已是UE名，需查回MMD原名再找
	if (ReNameTable)
	{
		FString ContextString;
		if (const FMMD2UE4NameTableRow* Row =
			ReNameTable->FindRow<FMMD2UE4NameTableRow>(DirectName, ContextString))
		{
			const FName OriginalName(*Row->MmdOriginalName);
			if (MorphMesh->FindMorphTarget(OriginalName))
			{
				return OriginalName;
			}
		}
	}

	return NAME_None;
}

/*
从Bone名称中搜索并获取与RefSkelton匹配的BoneIndex
Return :index, -1 is not found
@param :TargetName is Target Bone Name
*/
int32 UVmdFactory::FindRefBoneInfoIndexFromBoneName(
	const FReferenceSkeleton& RefSkelton,
	const FName& TargetName
)
{
	for (int i = 0; i < RefSkelton.GetRefBoneInfo().Num(); ++i)
	{
		if (RefSkelton.GetRefBoneInfo()[i].Name == TargetName)
		{
			return i;
		}
	}
	return -1;
}


/*
递归计算当前关键帧中指定Bone的Glb坐标
Return :trncform
@param :TargetName is Target Bone Name
*/
FTransform UVmdFactory::CalcGlbTransformFromBoneIndex(
	UAnimSequence* DestSeq,
	USkeleton* Skeleton,
	int32 BoneIndex,
	int32 keyIndex
)
{
	if (DestSeq == nullptr || Skeleton == nullptr || BoneIndex < 0 || keyIndex < 0 ||
		BoneIndex >= Skeleton->GetReferenceSkeleton().GetNum())
	{
		//error root
		return FTransform::Identity;
	}
#if UE_VERSION_OLDER_THAN(5,3,0)
	const auto& BoneTracks = DestSeq->GetDataModel()->GetBoneAnimationTracks();
	if (!BoneTracks.IsValidIndex(BoneIndex))
	{
		return FTransform::Identity;
	}
	const auto& TrackData = BoneTracks[BoneIndex].InternalTrackData;
	if (!TrackData.RotKeys.IsValidIndex(keyIndex) ||
		!TrackData.PosKeys.IsValidIndex(keyIndex) ||
		!TrackData.ScaleKeys.IsValidIndex(keyIndex))
	{
		return FTransform::Identity;
	}
	FTransform resultTrans(
		FQuat(TrackData.RotKeys[keyIndex]), // qt.X, qt.Y, qt.Z, qt.W),
		FVector(TrackData.PosKeys[keyIndex]),
		FVector(TrackData.ScaleKeys[keyIndex])
	);
#else  
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	const auto& BoneTracks = DestSeq->GetDataModel()->GetBoneAnimationTracks();
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
	if (!BoneTracks.IsValidIndex(BoneIndex))
	{
		return FTransform::Identity;
	}
	const auto& TrackData = BoneTracks[BoneIndex].InternalTrackData;
	if (!TrackData.RotKeys.IsValidIndex(keyIndex) ||
		!TrackData.PosKeys.IsValidIndex(keyIndex) ||
		!TrackData.ScaleKeys.IsValidIndex(keyIndex))
	{
		return FTransform::Identity;
	}
	FTransform resultTrans(
		FQuat(TrackData.RotKeys[keyIndex]), // qt.X, qt.Y, qt.Z, qt.W),
		FVector(TrackData.PosKeys[keyIndex]),
		FVector(TrackData.ScaleKeys[keyIndex])
	);

#endif

	int ParentBoneIndex = Skeleton->GetReferenceSkeleton().GetParentIndex(BoneIndex);
	if (ParentBoneIndex >= 0)
	{
		//found parent bone
		resultTrans *= CalcGlbTransformFromBoneIndex(
			DestSeq,
			Skeleton,
			ParentBoneIndex,
			keyIndex
		);
	}
	return resultTrans;
}
bool UVmdFactory::ImportVmdFromFile(FString file, USkeletalMesh* SkeletalMesh) {
	initMmdNameMap();
	MMD4UE4::VmdMotionInfo vmdMotionInfo;

	UVmdFactory* MyFactory = NewObject<UVmdFactory>(); //UE5.8
	TArray<uint8> File_Result;

	FString filepath = file;
	int32 indexs = -1;
	if (filepath.FindLastChar('\\', indexs)) {
		filepath = filepath.Right(filepath.Len() - indexs - 1);
		if (filepath.FindLastChar('.', indexs)) {
			filepath = filepath.Left(indexs);
			if (FPaths::FileExists(file)) {

				if (FFileHelper::LoadFileToArray(File_Result, *file)) {
					const uint8* DataPtr = File_Result.GetData();
					if (vmdMotionInfo.VMDLoaderBinary(DataPtr,nullptr) == false)
					{
						UE_LOG(LogMMD4UE4_VMDFactory, Error, TEXT("!!!VMD Import error::vmd data load faile."));
						return false;
					}
					UAnimSequence* LastCreatedAnim = nullptr;
					USkeleton* Skeleton = nullptr;
					VMDImportOptions* ImportOptions = nullptr; //UE5.8
					//UIKRigDefinition* IKRig = nullptr;
					//UMMDExtendAsset* MMDasset = nullptr;
					if (SkeletalMesh) {
						Skeleton = SkeletalMesh->GetSkeleton();
						UDataTable* MMD2UE4NameTable = nullptr;
						LastCreatedAnim = MyFactory->ImportAnimations(
							Skeleton,
							SkeletalMesh,
							nullptr,
							filepath,
							nullptr,
							MMD2UE4NameTable,
							nullptr,
							&vmdMotionInfo
						);
						return true;
					}
					else {
						UE_LOG(LogMMD4UE4_VMDFactory, Error, TEXT("!!!VMD Import error::SkeletalMesh is nullptr."));
					}
				}
				else {
					UE_LOG(LogMMD4UE4_VMDFactory, Error, TEXT("!!!VMD Import error:: LoadFileToArray."));
				}
			}
			else {
				UE_LOG(LogMMD4UE4_VMDFactory, Error, TEXT("!!!VMD Import error:: FIle is not exist."));
			}

		}
		else {
			UE_LOG(LogMMD4UE4_VMDFactory, Error, TEXT("!!!VMD Import error::filepath type error."));
		}
	}
	else {
		UE_LOG(LogMMD4UE4_VMDFactory, Error, TEXT("!!!VMD Import error::filepath error.%d,%s"), indexs, *filepath);
	}
	return false;
}

/*
刚体物理烘焙
把MMD刚体(PhysType_Simulated骨骼)的物理演算结果写入动画轨道，导入后播放即带摆动。
演算模型：惯性跟随 + 回正重力 + 阻尼 + 关节角度限制(近似碰撞)。
*/
bool UVmdFactory::BakeRigidBodyMotion(
	USkeletalMesh* SkeletalMesh,
	const FReferenceSkeleton& RefSkeleton,
	TArray<FRawAnimSequenceTrack>& Tracks,
	int32 NumFrames,
	float FrameRate,
	UMMDExtendAsset* mmdExtend
)
{
	if (!IsValid(SkeletalMesh) || NumFrames <= 1 || FrameRate <= 0.0f)
	{
		return false;
	}
	if (RefSkeleton.GetNum() <= 0 || Tracks.Num() <= 0)
	{
		return false;
	}
	const UPhysicsAsset* PhysicsAsset = SkeletalMesh->GetPhysicsAsset();

	struct FSimBone
	{
		int32 BoneIndex = INDEX_NONE;
		// 驱动源：最近的"非物理"祖先骨骼（VMD里有动画的那根）
		int32 DriverBoneIndex = INDEX_NONE;
		float AngularDamping = 0.9f;
		float Recoil = 0.0f;
		float LimitPitch = -1.0f;
		float LimitYaw = -1.0f;
		float LimitRoll = -1.0f;
		FVector AngularVelocity = FVector::ZeroVector;
		FQuat PrevRotation = FQuat::Identity;
		// 层级深度：用于产生相位延迟与幅度衰减，避免整组骨骼同步摆动
		int32 ChainDepth = 0;
		// 每根骨骼独有的相位偏移（弧度）
		float PhaseOffset = 0.0f;
	};

	TArray<FSimBone> SimBones;
	const bool bUseExtendData = IsValid(mmdExtend) && mmdExtend->RigidBodyList.Num() > 0;

	if (bUseExtendData)
	{
		// 优先使用MMDExtendAsset的MMD原始刚体数据（保留OpType/減衰真值）
		for (const FMMD_RigidBodyInfo& RigidBodyInfo : mmdExtend->RigidBodyList)
		{
			// OpType: 0=骨骼跟随，1/2=物理演算
			if (RigidBodyInfo.OpType == 0)
			{
				continue;
			}
			const int32 BoneIndex = RefSkeleton.FindBoneIndex(RigidBodyInfo.BoneName);
			if (!RefSkeleton.IsValidIndex(BoneIndex))
			{
				continue;
			}
			FSimBone SimBone;
			SimBone.BoneIndex = BoneIndex;
			// MMD RotDim(0~1，1=几乎不衰减) → UE阻尼语义换算
			SimBone.AngularDamping = FMath::Clamp((1.0f - FMath::Clamp(RigidBodyInfo.RotDim, 0.0f, 1.0f)) * 2.0f, 0.0f, 1.0f);
			SimBone.Recoil = RigidBodyInfo.Recoil;
			SimBones.Add(SimBone);
		}

		// 关节角度限制：用原始关节数据（比从PhysicsAsset反推更准）
		for (const FMMD_JointInfo& JointInfo : mmdExtend->JointList)
		{
			if (!mmdExtend->RigidBodyList.IsValidIndex(JointInfo.RigidBodyAIndex) ||
				!mmdExtend->RigidBodyList.IsValidIndex(JointInfo.RigidBodyBIndex))
			{
				continue;
			}
			const FName BoneA = mmdExtend->RigidBodyList[JointInfo.RigidBodyAIndex].BoneName;
			const FName BoneB = mmdExtend->RigidBodyList[JointInfo.RigidBodyBIndex].BoneName;
			const int32 BoneAIndex = RefSkeleton.FindBoneIndex(BoneA);
			const int32 BoneBIndex = RefSkeleton.FindBoneIndex(BoneB);
			for (FSimBone& SimBone : SimBones)
			{
				if (SimBone.BoneIndex != BoneAIndex && SimBone.BoneIndex != BoneBIndex)
				{
					continue;
				}
				// 弧度 -> 度
				SimBone.LimitPitch = FMath::RadiansToDegrees(FMath::Min(
					FMath::Abs(JointInfo.ConstrainRotationMin.X), FMath::Abs(JointInfo.ConstrainRotationMax.X)));
				SimBone.LimitYaw = FMath::RadiansToDegrees(FMath::Min(
					FMath::Abs(JointInfo.ConstrainRotationMin.Y), FMath::Abs(JointInfo.ConstrainRotationMax.Y)));
				SimBone.LimitRoll = FMath::RadiansToDegrees(FMath::Min(
					FMath::Abs(JointInfo.ConstrainRotationMin.Z), FMath::Abs(JointInfo.ConstrainRotationMax.Z)));
				break;
			}
		}
	}
	else
	{
		// 回退：从PhysicsAsset读取（ExtendAsset未提供时）
		if (!IsValid(PhysicsAsset))
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Log,
				TEXT("BakeRigidBodyMotion: no MMDExtendAsset and mesh has no physics asset."));
			return false;
		}
		for (const TObjectPtr<USkeletalBodySetup>& BodySetup : PhysicsAsset->SkeletalBodySetups)
		{
			if (!IsValid(BodySetup) || BodySetup->PhysicsType != PhysType_Simulated)
			{
				continue;
			}
			const int32 BoneIndex = RefSkeleton.FindBoneIndex(BodySetup->BoneName);
			if (!RefSkeleton.IsValidIndex(BoneIndex))
			{
				continue;
			}
			FSimBone SimBone;
			SimBone.BoneIndex = BoneIndex;
			SimBone.AngularDamping = FMath::Clamp(BodySetup->DefaultInstance.AngularDamping, 0.0f, 1.0f);
			SimBones.Add(SimBone);
		}

		for (const TObjectPtr<UPhysicsConstraintTemplate>& ConstraintTemplate : PhysicsAsset->ConstraintSetup)
		{
			if (!IsValid(ConstraintTemplate))
			{
				continue;
			}
			const FConstraintInstance& Constraint = ConstraintTemplate->DefaultInstance;
			const int32 Bone1 = RefSkeleton.FindBoneIndex(Constraint.ConstraintBone1);
			const int32 Bone2 = RefSkeleton.FindBoneIndex(Constraint.ConstraintBone2);
			for (FSimBone& SimBone : SimBones)
			{
				if (SimBone.BoneIndex != Bone1 && SimBone.BoneIndex != Bone2)
				{
					continue;
				}
				SimBone.LimitPitch = Constraint.ProfileInstance.ConeLimit.Swing1LimitDegrees;
				SimBone.LimitYaw = Constraint.ProfileInstance.ConeLimit.Swing2LimitDegrees;
				SimBone.LimitRoll = Constraint.ProfileInstance.TwistLimit.TwistLimitDegrees;
				break;
			}
		}
	}

	if (SimBones.Num() == 0)
	{
		UE_LOG(LogMMD4UE4_VMDFactory, Log,
			TEXT("BakeRigidBodyMotion: no simulated body found (source=%s)."),
			bUseExtendData ? TEXT("ExtendAsset") : TEXT("PhysicsAsset"));
		return false;
	}

	// 为每根物理骨骼找驱动源：向上遍历所有祖先，选"旋转变化量最大"的那个。
	// 头发/胸部自身在VMD里通常只有1帧，真正的运动来自身体（上半身/センター等），
	// 而最近的祖先可能是不动的（如Root、おっぱい調整），必须按实际动画量挑选。
	{
		TSet<int32> SimulatedIndices;
		for (const FSimBone& SimBone : SimBones)
		{
			if (RefSkeleton.IsValidIndex(SimBone.BoneIndex))
			{
				SimulatedIndices.Add(SimBone.BoneIndex);
			}
		}

		// 预先算出每根骨骼的累计旋转变化量
		TArray<float> MotionAmounts;
		MotionAmounts.SetNumZeroed(Tracks.Num());
		for (int32 BoneIndex = 0; BoneIndex < Tracks.Num(); ++BoneIndex)
		{
			const FRawAnimSequenceTrack& Track = Tracks[BoneIndex];
			float TotalMotion = 0.0f;
			const int32 KeyCount = Track.RotKeys.Num();
			for (int32 Frame = 1; Frame < KeyCount; ++Frame)
			{
				const FQuat Delta = FQuat(Track.RotKeys[Frame - 1]).Inverse() * FQuat(Track.RotKeys[Frame]);
				TotalMotion += Delta.GetAngle();
			}
			MotionAmounts[BoneIndex] = TotalMotion;
		}

		// 躯干骨骼优先级：胸部/头发挂在上半身上，应当由上半身驱动。
		// 下半身/センター走路时摆幅通常更大，若与上半身同权会被误选，
		// 导致胸部跟着骨盆扭动。数值越大越优先。
		auto GetTorsoPriority = [&RefSkeleton](int32 BoneIndex) -> int32
		{
			if (!RefSkeleton.IsValidIndex(BoneIndex))
			{
				return 0;
			}
			const FString BoneName = RefSkeleton.GetBoneName(BoneIndex).ToString();
			// 上半身系（含上半身2/3、chest、spine）最优先
			static const TCHAR* UpperKeywords[] = {
				TEXT("上半身"), TEXT("chest"), TEXT("spine")
			};
			for (const TCHAR* Keyword : UpperKeywords)
			{
				if (BoneName.Contains(Keyword))
				{
					return 2;
				}
			}
			// 下半身/根干系次之
			static const TCHAR* LowerKeywords[] = {
				TEXT("下半身"), TEXT("センター"), TEXT("center"), TEXT("腰"), TEXT("hips")
			};
			for (const TCHAR* Keyword : LowerKeywords)
			{
				if (BoneName.Contains(Keyword))
				{
					return 1;
				}
			}
			return 0;
		};

		// 头部/颈部骨骼：不应作为胸部或头发的驱动源。
		// 「頭」在动作里摆动幅度往往最大，若不排除会被优先选中。
		auto IsHeadBone = [&RefSkeleton](int32 BoneIndex) -> bool
		{
			if (!RefSkeleton.IsValidIndex(BoneIndex))
			{
				return false;
			}
			const FString BoneName = RefSkeleton.GetBoneName(BoneIndex).ToString();
			return BoneName == TEXT("頭") || BoneName == TEXT("首") ||
				BoneName.Contains(TEXT("neck")) || BoneName.Contains(TEXT("head"));
		};

		for (FSimBone& SimBone : SimBones)
		{
			int32 BestBone = INDEX_NONE;
			float BestMotion = 0.0f;
			if (!RefSkeleton.IsValidIndex(SimBone.BoneIndex))
			{
				SimBone.DriverBoneIndex = INDEX_NONE;
				continue;
			}
			int32 ParentIndex = RefSkeleton.GetParentIndex(SimBone.BoneIndex);
			while (ParentIndex != INDEX_NONE && RefSkeleton.IsValidIndex(ParentIndex))
			{
				if (!SimulatedIndices.Contains(ParentIndex) && MotionAmounts.IsValidIndex(ParentIndex))
				{
					// 躯干分级加权：上半身 > 下半身/センター > 其它。
					// 只加权不加层级会让下半身（摆幅更大）抢走胸部驱动源。
					float Weighted = MotionAmounts[ParentIndex];
					// 优先级必须绝对压过运动量差异（运动量最大约几千度）
					Weighted += GetTorsoPriority(ParentIndex) * 100000.0f;
					if (IsHeadBone(ParentIndex))
					{
						// 头部大幅摆动不应驱动胸部，权重压到躯干之下
						Weighted *= 0.1f;
					}
					if (Weighted > BestMotion)
					{
						BestMotion = Weighted;
						BestBone = ParentIndex;
					}
				}
				ParentIndex = RefSkeleton.GetParentIndex(ParentIndex);
			}

			// 祖先里没有任何运动时，退而使用"最近的祖先"，绝不跨到无关骨骼
			// （曾经错误地挑了全身运动最大的骨骼，导致胸部绑到左肘等无关部位）
			if (BestBone == INDEX_NONE)
			{
				int32 FallbackIndex = RefSkeleton.GetParentIndex(SimBone.BoneIndex);
				while (FallbackIndex != INDEX_NONE && RefSkeleton.IsValidIndex(FallbackIndex))
				{
					if (!SimulatedIndices.Contains(FallbackIndex))
					{
						BestBone = FallbackIndex;
						break;
					}
					FallbackIndex = RefSkeleton.GetParentIndex(FallbackIndex);
				}
			}

			// 若选出的驱动源本身不运动（如Root），借用同组里运动量最大的驱动源。
			// 胸部骨骼的父链常常直接挂到始终静止的Root，导致摆动恒为0。
			if (BestBone == INDEX_NONE ||
				(MotionAmounts.IsValidIndex(BestBone) &&
					MotionAmounts[BestBone] <= KINDA_SMALL_NUMBER))
			{
				// 在本骨骼的"祖先前缀"范围内，找已被其他物理骨骼选中的、有运动的驱动源
				TSet<int32> AncestorSet;
				int32 Walk = SimBone.BoneIndex;
				while (Walk != INDEX_NONE && RefSkeleton.IsValidIndex(Walk))
				{
					AncestorSet.Add(Walk);
					Walk = RefSkeleton.GetParentIndex(Walk);
				}

				int32 BestFallback = INDEX_NONE;
				float BestFallbackMotion = 0.0f;
				for (int32 OtherIndex = 0; OtherIndex < SimBones.Num(); ++OtherIndex)
				{
					const int32 OtherDriver = SimBones[OtherIndex].DriverBoneIndex;
					if (OtherDriver == INDEX_NONE ||
						!MotionAmounts.IsValidIndex(OtherDriver) ||
						MotionAmounts[OtherDriver] <= BestFallbackMotion)
					{
						continue;
					}
					if (AncestorSet.Contains(OtherDriver))
					{
						BestFallback = OtherDriver;
						BestFallbackMotion = MotionAmounts[OtherDriver];
					}
				}

				// 祖先里仍无运动源时，退而在"躯干"范围内选，绝不选四肢
				// （曾经选中左ひじ等手臂骨骼，导致胸部跟着手臂反向摆动）
				if (BestFallback == INDEX_NONE)
				{
					auto IsLimbBone = [&RefSkeleton](int32 BoneIndex) -> bool
					{
						static const TCHAR* LimbKeywords[] = {
							TEXT("腕"), TEXT("ひじ"), TEXT("手首"), TEXT("手先"), TEXT("指"),
							TEXT("足"), TEXT("ひざ"), TEXT("足首"), TEXT("つま先"),
							TEXT("肩"), TEXT("肘"), TEXT("腕捩"), TEXT("手捩")
						};
						const FString BoneName = RefSkeleton.GetBoneName(BoneIndex).ToString();
						for (const TCHAR* Keyword : LimbKeywords)
						{
							if (BoneName.Contains(Keyword))
							{
								return true;
							}
						}
						return false;
					};

					for (int32 BoneIndex = 0; BoneIndex < MotionAmounts.Num(); ++BoneIndex)
					{
						if (SimulatedIndices.Contains(BoneIndex) || IsLimbBone(BoneIndex))
						{
							continue;
						}
						// 头部同样不能驱动胸部/头发，否则胸部会跟着点头摆动
						if (IsHeadBone(BoneIndex))
						{
							continue;
						}
						float Weighted =
							MotionAmounts[BoneIndex] + GetTorsoPriority(BoneIndex) * 100000.0f;
						if (Weighted > BestFallbackMotion)
						{
							BestFallbackMotion = Weighted;
							BestFallback = BoneIndex;
						}
					}
				}
				BestBone = BestFallback;
			}
			SimBone.DriverBoneIndex = BestBone;

			UE_LOG(LogMMD4UE4_VMDFactory, Log,
				TEXT("[driver] bone=%s -> driver=%s motion=%.1f"),
				*RefSkeleton.GetBoneName(SimBone.BoneIndex).ToString(),
				RefSkeleton.IsValidIndex(BestBone)
					? *RefSkeleton.GetBoneName(BestBone).ToString()
					: TEXT("None"),
				MotionAmounts.IsValidIndex(BestBone) ? FMath::RadiansToDegrees(MotionAmounts[BestBone]) : 0.0f);
		}

		// 链深：祖先中物理骨骼的数量（左胸上=0、左胸=1、左胸下=2…）。
		// 用于让同族骨骼产生幅度/相位的层次差异，而不是整齐划一地摆。
		for (FSimBone& SimBone : SimBones)
		{
			int32 Depth = 0;
			int32 Walk = RefSkeleton.IsValidIndex(SimBone.BoneIndex)
				? RefSkeleton.GetParentIndex(SimBone.BoneIndex)
				: INDEX_NONE;
			while (Walk != INDEX_NONE && RefSkeleton.IsValidIndex(Walk))
			{
				if (SimulatedIndices.Contains(Walk))
				{
					++Depth;
				}
				Walk = RefSkeleton.GetParentIndex(Walk);
			}
			SimBone.ChainDepth = Depth;
		}
		// 按"骨骼名同族"统一驱动源：左胸上/左胸/左胸下等共享同一基础名，
		// 且其中至少有一个能正确找到躯干驱动源；其余成员沿用该驱动源，
		// 避免个别骨骼（如左胸，父级直连Root）被错误绑到頭部/手臂。
		{
			// 去掉方位/序号后缀，得到基础名
			auto MakeBaseBoneName = [&RefSkeleton](int32 BoneIndex) -> FString
			{
				if (!RefSkeleton.IsValidIndex(BoneIndex))
				{
					return FString();
				}
				FString BaseName = RefSkeleton.GetBoneName(BoneIndex).ToString();
				static const TCHAR* Suffixes[] = {
					TEXT("上"), TEXT("下"), TEXT("前"), TEXT("後"), TEXT("左"), TEXT("右"),
					TEXT("先"), TEXT("2"), TEXT("3"), TEXT("4"), TEXT("5")
				};
				for (const TCHAR* Suffix : Suffixes)
				{
					if (BaseName.EndsWith(Suffix))
					{
						BaseName.LeftChopInline(FCString::Strlen(Suffix));
						break;
					}
				}
				return BaseName;
			};

			// 每族选出驱动源：优先躯干骨骼，其次运动量最大
			TMap<FString, int32> FamilyBestDriver;
			TMap<FString, float> FamilyBestMotion;
			for (const FSimBone& SimBone : SimBones)
			{
				const FString Family = MakeBaseBoneName(SimBone.BoneIndex);
				if (Family.IsEmpty())
				{
					continue;
				}
				const int32 Driver = SimBone.DriverBoneIndex;
				if (!MotionAmounts.IsValidIndex(Driver))
				{
					continue;
				}

				// 躯干驱动源权重远高于其它骨骼（頭/手臂等），避免胸部跟着头摆
				// 与驱动源选择保持一致的躯干分级权重
				const float Motion =
					MotionAmounts[Driver] + GetTorsoPriority(Driver) * 100000.0f;

				float& BestMotion = FamilyBestMotion.FindOrAdd(Family, -1.0f);
				if (Motion > BestMotion)
				{
					BestMotion = Motion;
					FamilyBestDriver.Add(Family, Driver);
				}
			}

			// 全族统一使用该驱动源
			for (FSimBone& SimBone : SimBones)
			{
				const FString Family = MakeBaseBoneName(SimBone.BoneIndex);
				if (const int32* BestDriver = FamilyBestDriver.Find(Family))
				{
					// 只有当该族存在"有运动"的驱动源时才统一
					const float BestMotion = FamilyBestMotion.FindRef(Family);
					if (BestMotion > KINDA_SMALL_NUMBER)
					{
						SimBone.DriverBoneIndex = *BestDriver;
					}
				}
			}
		}
	}

	UE_LOG(LogMMD4UE4_VMDFactory, Log,
		TEXT("BakeRigidBodyMotion: %d simulated bones from %s."),
		SimBones.Num(),
		bUseExtendData ? TEXT("MMDExtendAsset") : TEXT("PhysicsAsset"));

	// 模拟帧数只由传入的帧数决定（Tracks.Num() 是骨骼数量，不能用作帧数上限）
	const int32 SimFrameCount = NumFrames;
	if (SimFrameCount <= 1)
	{
		return false;
	}

	// 弹簧跟随模型：摆动偏移(rad)被拉向"驱动骨骼相对参考姿势的旋转偏差"，
	// 用欠阻尼弹簧产生过冲与回弹，模拟MMD刚体的软体摆动。
	// 之前用"驱动角速度逐帧累加"是错误的：累加量不会归零，会单方向无限漂移
	// （实测冲到148度），而且叠加在动画姿势上形成二次积分。
	const float FollowStiffness = 0.22f;   // 追随目标的速度（每帧向目标靠拢的比例）
	const float FollowDamping = 0.55f;     // 阻尼：保留速度的比例，越大越"飘"
	// 单帧最大摆动角度（度）。MMD胸部骨骼极短，需要较大角度才有可见位移，
	// 但超过90度会让骨骼翻面自交。
	const float MaxSwingDegrees = 45.0f;

	int32 BakedKeyCount = 0;
	for (FSimBone& SimBone : SimBones)
	{
		if (!Tracks.IsValidIndex(SimBone.BoneIndex))
		{
			continue;
		}
		FRawAnimSequenceTrack& Track = Tracks[SimBone.BoneIndex];

		// 物理骨骼可能没有VMD轨道（或只有少量关键帧），先补齐整段帧。
		// 无轨道时用参考姿势的局部变换作为基准，否则摆动会以Identity为中心。
		// 注意：SetBoneTrackKeys要求pos/rot/scale三组键数量一致，必须一起补齐。
		const int32 PrevKeyCount = Track.RotKeys.Num();
		if (PrevKeyCount < SimFrameCount)
		{
			FQuat FillRotation = FQuat::Identity;
			FVector3f FillTranslation = FVector3f::ZeroVector;
			FVector3f FillScale = FVector3f::OneVector;
			if (PrevKeyCount > 0)
			{
				FillRotation = FQuat(Track.RotKeys[PrevKeyCount - 1]);
				if (Track.PosKeys.IsValidIndex(PrevKeyCount - 1))
				{
					FillTranslation = Track.PosKeys[PrevKeyCount - 1];
				}
				if (Track.ScaleKeys.IsValidIndex(PrevKeyCount - 1))
				{
					FillScale = Track.ScaleKeys[PrevKeyCount - 1];
				}
			}
			else if (RefSkeleton.IsValidIndex(SimBone.BoneIndex))
			{
				const FTransform& RefTransform = RefSkeleton.GetRefBonePose()[SimBone.BoneIndex];
				FillRotation = RefTransform.GetRotation();
				FillTranslation = FVector3f(RefTransform.GetTranslation());
				FillScale = FVector3f(RefTransform.GetScale3D());
			}

			Track.RotKeys.SetNum(SimFrameCount);
			Track.PosKeys.SetNum(SimFrameCount);
			Track.ScaleKeys.SetNum(SimFrameCount);
			for (int32 Frame = PrevKeyCount; Frame < SimFrameCount; ++Frame)
			{
				Track.RotKeys[Frame] = FQuat4f(FillRotation);
				Track.PosKeys[Frame] = FillTranslation;
				Track.ScaleKeys[Frame] = FillScale;
			}
		}

		// 驱动骨骼的动画轨迹（身体运动 -> 头发/胸部甩动）
		const FRawAnimSequenceTrack* DriverTrack =
			Tracks.IsValidIndex(SimBone.DriverBoneIndex) ? &Tracks[SimBone.DriverBoneIndex] : nullptr;

		// 驱动骨骼的局部旋转常常几乎不变，真正的朝向由祖先累积而来
		// （例如上半身的旋转挂在センター/下半身上）。这里预先算出驱动骨骼
		// 到根链上所有骨骼的索引，用于按帧累积世界朝向。
		TArray<int32> DriverChain;
		for (int32 Walk = SimBone.DriverBoneIndex; Walk != INDEX_NONE && RefSkeleton.IsValidIndex(Walk);
			Walk = RefSkeleton.GetParentIndex(Walk))
		{
			DriverChain.Add(Walk);
		}

		// 按帧取驱动骨骼的累积旋转（驱动链上各骨骼局部旋转的乘积）
		auto GetDriverCumulativeAt = [&DriverChain, &Tracks](int32 AnyFrame) -> FQuat
		{
			FQuat Accum = FQuat::Identity;
			for (const int32 ChainBone : DriverChain)
			{
				if (!Tracks.IsValidIndex(ChainBone))
				{
					continue;
				}
				const FRawAnimSequenceTrack& ChainTrack = Tracks[ChainBone];
				if (ChainTrack.RotKeys.Num() == 0)
				{
					continue;
				}
				const int32 Clamped = FMath::Clamp(AnyFrame, 0, ChainTrack.RotKeys.Num() - 1);
				Accum = Accum * FQuat(ChainTrack.RotKeys[Clamped]);
			}
			return Accum;
		};

		// 驱动骨骼在帧0的累积朝向，作为摆动偏差的基准
		const FQuat DriverStartCumulative = GetDriverCumulativeAt(0);

		// 驱动轨道帧数可能与模拟帧数不同（如582 vs 154），需按比例映射
		const int32 DriverKeyCount = DriverTrack ? DriverTrack->RotKeys.Num() : 0;
		auto MapToDriverFrame = [DriverKeyCount, SimFrameCount](int32 SimFrame) -> int32
		{
			if (DriverKeyCount <= 1)
			{
				return INDEX_NONE;
			}
			const int32 Mapped = FMath::RoundToInt(
				static_cast<float>(SimFrame) * static_cast<float>(DriverKeyCount - 1) /
				static_cast<float>(FMath::Max(SimFrameCount - 1, 1)));
			return FMath::Clamp(Mapped, 0, DriverKeyCount - 1);
		};

		// 上一帧的驱动目标（rad，欧拉角差分），用于求变化率
		FVector PrevDriverTarget = FVector::ZeroVector;
		// 摆动速度（rad/帧），弹簧系统的状态量
		FVector SwingVelocity = FVector::ZeroVector;

		SimBone.PrevRotation = FQuat(Track.RotKeys[0]);
		for (int32 Frame = 1; Frame < SimFrameCount; ++Frame)
		{
			const FQuat AnimRotation = FQuat(Track.RotKeys[Frame]);

			// 本帧的驱动目标：驱动骨骼的"世界朝向"相对起始朝向的旋转偏差。
			// 用累积旋转而非局部旋转，因为躯干的旋转往往来自祖先骨骼。
			// 这是"绝对量"，所以弹簧偏移永远不会无限累积。
			FVector DriverTarget = PrevDriverTarget;
			const int32 DriverFrame = MapToDriverFrame(Frame);
			if (DriverFrame != INDEX_NONE)
			{
				// 相位延迟：按骨骼序号错开，让同组骨骼不同步，形成波浪式跟随。
				// （ChainDepth 在同级骨骼上恒为0，改用序号保证差异）
				const int32 PhaseFrames = (SimBone.BoneIndex % 4) * 2;
				const int32 PhasedFrame = FMath::Clamp(
					DriverFrame - PhaseFrames,
					0, DriverKeyCount - 1);

				const FQuat DriverNow = GetDriverCumulativeAt(PhasedFrame);
				const FQuat DriverCumulative = DriverStartCumulative.Inverse() * DriverNow;

				// 用欧拉角差分：避免GetRotationAxis在角度跨越π时轴翻转导致正负抵消
				// FRotator::Euler() 返回度，转成弧度与SwingOffset单位统一
				DriverTarget = FMath::DegreesToRadians(FRotator(DriverCumulative).Euler());
			}

			// 摆动应当只是躯干运动的"滞后分量"，而非完整复制。
			// 骨骼层级本身已经让胸部跟随躯干，若再叠加完整的驱动偏差会形成双倍旋转
			// （视觉上表现为抽搐/翻面）。因此按增益缩放，并随链深衰减。
			const float DepthScale = 0.55f / (1.0f + SimBone.ChainDepth * 0.5f);
			const FVector SpringTarget = DriverTarget * DepthScale;

			// 欠阻尼弹簧积分：SwingVelocity 追向目标，SwingOffset 累加速度
			SwingVelocity += (SpringTarget - SimBone.AngularVelocity) * FollowStiffness;
			SwingVelocity *= FollowDamping;
			SimBone.AngularVelocity += SwingVelocity;

			// 限幅（弧度）：单帧最大摆动角度
			const float MaxSwingRad = FMath::DegreesToRadians(MaxSwingDegrees);
			SimBone.AngularVelocity.X = FMath::Clamp(SimBone.AngularVelocity.X, -MaxSwingRad, MaxSwingRad);
			SimBone.AngularVelocity.Y = FMath::Clamp(SimBone.AngularVelocity.Y, -MaxSwingRad, MaxSwingRad);
			SimBone.AngularVelocity.Z = FMath::Clamp(SimBone.AngularVelocity.Z, -MaxSwingRad, MaxSwingRad);

			FVector SwingOffset = SimBone.AngularVelocity;

			// 关节角度限制（软约束）：硬裁剪会让胸/发固定在同一饱和角度，
			// 这里改为超出限制时按比例衰减，保留随动作变化的部分。
			if (SimBone.LimitPitch > 0.1f)
			{
				const float LimitRad = FMath::DegreesToRadians(SimBone.LimitPitch);
				if (FMath::Abs(SwingOffset.X) > LimitRad)
				{
					SwingOffset.X = FMath::Sign(SwingOffset.X) *
						(LimitRad + (FMath::Abs(SwingOffset.X) - LimitRad) * 0.15f);
				}
			}
			if (SimBone.LimitYaw > 0.1f)
			{
				const float LimitRad = FMath::DegreesToRadians(SimBone.LimitYaw);
				if (FMath::Abs(SwingOffset.Y) > LimitRad)
				{
					SwingOffset.Y = FMath::Sign(SwingOffset.Y) *
						(LimitRad + (FMath::Abs(SwingOffset.Y) - LimitRad) * 0.15f);
				}
			}
			if (SimBone.LimitRoll > 0.1f)
			{
				const float LimitRad = FMath::DegreesToRadians(SimBone.LimitRoll);
				if (FMath::Abs(SwingOffset.Z) > LimitRad)
				{
					SwingOffset.Z = FMath::Sign(SwingOffset.Z) *
						(LimitRad + (FMath::Abs(SwingOffset.Z) - LimitRad) * 0.15f);
				}
			}
			PrevDriverTarget = DriverTarget;

			// 结果 = 动画姿势 + 受限后的摆动偏移
			// 注意：FQuat::MakeFromEuler / FRotator::MakeFromEuler 的参数是"度"，
			// SwingOffset 内部按弧度计算，这里必须转成度，否则摆幅会被缩小57倍。
			const FVector SwingOffsetDegrees = FMath::RadiansToDegrees(SwingOffset);
			const FQuat SimRotation = AnimRotation * FQuat::MakeFromEuler(SwingOffsetDegrees);

			SimBone.PrevRotation = SimRotation;
			Track.RotKeys[Frame] = FQuat4f(SimRotation.GetNormalized());
			++BakedKeyCount;

			// 驱动目标恒为0说明驱动骨骼选错或整段无运动
			if (Frame % 120 == 1)
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
					TEXT("[bake] f%d driver=%s target=%.2fdeg swing=%.2fdeg"),
					Frame,
					RefSkeleton.IsValidIndex(SimBone.DriverBoneIndex)
						? *RefSkeleton.GetBoneName(SimBone.DriverBoneIndex).ToString()
						: TEXT("None"),
					FMath::RadiansToDegrees(DriverTarget.Size()),
					FMath::RadiansToDegrees(SwingOffset.Size()));
			}
		}
	}

	UE_LOG(LogMMD4UE4_VMDFactory, Log,
		TEXT("Rigid body baked into animation: bones=%d, frames=%d, keys=%d"),
		SimBones.Num(),
		SimFrameCount,
		BakedKeyCount);

	// 摆动幅度汇总（用于确认烘焙是否产生了可见位移）
	float MaxOverallSwing = 0.0f;
	for (const FSimBone& SimBone : SimBones)
	{
		if (!Tracks.IsValidIndex(SimBone.BoneIndex))
		{
			continue;
		}
		const FRawAnimSequenceTrack& Track = Tracks[SimBone.BoneIndex];
		if (Track.RotKeys.Num() <= 1)
		{
			continue;
		}
		const FQuat FirstRotation = FQuat(Track.RotKeys[0]);
		float MaxBoneSwing = 0.0f;
		for (int32 Frame = 1; Frame < Track.RotKeys.Num(); ++Frame)
		{
			const FQuat Delta = FirstRotation.Inverse() * FQuat(Track.RotKeys[Frame]);
			MaxBoneSwing = FMath::Max(MaxBoneSwing, FMath::RadiansToDegrees(Delta.GetAngle()));
		}
		MaxOverallSwing = FMath::Max(MaxOverallSwing, MaxBoneSwing);
	}
	UE_LOG(LogMMD4UE4_VMDFactory, Log,
		TEXT("Rigid body swing amplitude: max %.2f deg over %d bones."),
		MaxOverallSwing,
		SimBones.Num());

	return BakedKeyCount > 0;
}

#undef LOCTEXT_NAMESPACE
