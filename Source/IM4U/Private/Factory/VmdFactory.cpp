// Copyright 2023 NaN_Name. All Rights Reserved.
#ifndef HEADER_H_
#define HEADER_H_
#endif
#include "Factory/VmdFactory.h"
#include "Factory/VmdImportOption.h"
#include "VmdImporter.h"
#include "VmdImportUI.h"
#include "MMDExtendAsset.h"
#include "AnimationUtils.h"
#include "Animation/AnimData/CurveIdentifier.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "CoreMinimal.h"
#include "ImportUtils/SkelImport.h"
#include "Misc/EngineVersionComparison.h"
#include "Misc/PackageName.h"
#include "ObjectTools.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "RigEditor/IKRigController.h"

/*
#include "Factory/VmdImportOption.h"
*/

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
		*/ //ydgro
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
		FMessageDialog::Open(EAppMsgType::Ok, MessageDbg, &TitleStr);//ydgro
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
		//FMessageDialog::Open(EAppMsgType::Ok, MessageDbg, &TitleStr);//ydgro
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
					FMessageDialog::Open(EAppMsgType::Ok, MessageDbg);  //ydgro
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
						IKRig,
						ImportUI->MMD2UE4NameTableRow,
						ImportUI->MmdExtendAsset,
						&vmdMotionInfo
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
	MMD4UE4::VmdMotionInfo* vmdMotionInfo
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
		if (!ImportVMDToAnimSequence(LastCreatedAnim, Skeleton, SkeletalMesh, ReNameTable, IKRig, mmdExtend, vmdMotionInfo))
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
            adc.UpdateCurveNamesFromSkeleton(Skeleton); //ydgro
#endif

			adc.NotifyPopulated();

			adc.CloseBracket();
#endif
*/
			// mark package as dirty
			//MarkPackageDirty();  //表达式未使用
			//SkeletalMesh->MarkPackageDirty();
			if (const IAnimationDataModel* DataModel = LastCreatedAnim->GetDataModel())
			{
				TArray<FName> BoneTrackNames;
				DataModel->GetBoneTrackNames(BoneTrackNames);
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
					TEXT("Before animation PostEditChange: data model contains %d bone tracks."),
					BoneTrackNames.Num());
				for (const TCHAR* BoneNameString : {
					TEXT("_HemB-L-E01"), TEXT("_HemB-L-E02"), TEXT("_HemB-L-E03"), TEXT("_HemB-L-E04"),
					TEXT("_HemB-R-E01"), TEXT("_HemB-R-E02"), TEXT("_HemB-R-E03"), TEXT("_HemB-R-E04")
				})
				{
					const FName BoneName(BoneNameString);
					if (!BoneTrackNames.Contains(BoneName))
					{
						UE_LOG(LogMMD4UE4_VMDFactory, Error,
							TEXT("Before animation PostEditChange, data model has no exact-name bone track for '%s'."),
							BoneNameString);
					}
				}
			}

			LastCreatedAnim->PostEditChange();
			LastCreatedAnim->SetPreviewMesh(SkeletalMesh);
			LastCreatedAnim->MarkPackageDirty();

			if (const IAnimationDataModel* DataModel = LastCreatedAnim->GetDataModel())
			{
				const int32 NumberOfFrames = DataModel->GetNumberOfFrames();
				const int32 LastFrame = FMath::Max(NumberOfFrames - 1, 0);
				TArray<FName> BoneTrackNames;
				DataModel->GetBoneTrackNames(BoneTrackNames);
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
					TEXT("Final animation model contains %d bone tracks."),
					BoneTrackNames.Num());
				for (const TCHAR* BoneNameString : {
					TEXT("_HemB-L-E01"), TEXT("_HemB-L-E02"), TEXT("_HemB-L-E03"), TEXT("_HemB-L-E04"),
					TEXT("_HemB-R-E01"), TEXT("_HemB-R-E02"), TEXT("_HemB-R-E03"), TEXT("_HemB-R-E04")
				})
				{
					const FName BoneName(BoneNameString);
					const int32 TrackIndex = BoneTrackNames.IndexOfByKey(BoneName);
					if (TrackIndex == INDEX_NONE)
					{
						UE_LOG(LogMMD4UE4_VMDFactory, Error,
							TEXT("Final animation model has no exact-name bone track for '%s'."),
							BoneNameString);
						continue;
					}

					const FTransform FirstTransform = DataModel->GetBoneTrackTransform(
						BoneName, FFrameNumber(0));
					const FTransform LastTransform = DataModel->GetBoneTrackTransform(
						BoneName, FFrameNumber(LastFrame));
					UE_LOG(LogMMD4UE4_VMDFactory, Log,
						TEXT("Final Hem animation model '%s': track index=%d, frames=%d, first rot=%s, last rot=%s, delta=%.4f degrees."),
						BoneNameString,
						TrackIndex,
						NumberOfFrames,
						*FirstTransform.GetRotation().ToString(),
						*LastTransform.GetRotation().ToString(),
						FMath::RadiansToDegrees(FirstTransform.GetRotation().AngularDistance(
							LastTransform.GetRotation())));
				}
			}

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
			adc.UpdateCurveNamesFromSkeleton(Skeleton, ERawCurveTrackTypes::RCT_Float); //ydgro
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

		// VMD stores the MMD-side name; resolve it to the UE morph target name.
		FName Name = *vmdFaceTrackPtr->TrackName;
		if (Name.IsNone())
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Warning,
				TEXT("Skipping VMD morph track with an empty name."));
			continue;
		}
		FName MappedName;
		if (FindTableRowMMD2UEName(ReNameTable, Name, &MappedName))
		{
			Name = MappedName;
		}
		if (Name.IsNone())
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Warning,
				TEXT("Skipping VMD morph track '%s': resolved curve name is empty."),
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

					//FSmartName NewName; //ydgro
#if UE_VERSION_OLDER_THAN(5,3,0)
					Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, Name, NewName);
#else  
					Skeleton->AddCurveMetaData(Name); //ydgro
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
	MMD4UE4::VmdMotionInfo* vmdMotionInfo
)
{
	// nullptr check in-param
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
	else
	{
		int32 LegIKCount = 0;
		for (const FMMD_IKInfo& IKInfo : mmdExtend->IkInfoList)
		{
			const FString IKName = IKInfo.IKBoneName.ToString();
			if (IKName.Contains(TEXT("足")) ||
				IKName.Contains(TEXT("Leg"), ESearchCase::IgnoreCase))
			{
				++LegIKCount;
			}
		}
		UE_LOG(LogMMD4UE4_VMDFactory, Log,
			TEXT("ImportVMDToAnimSequence : %d leg IK definitions available."),
			LegIKCount);
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
#else
	adc.NotifyPopulated();
	adc.SetPlayLength(FGenericPlatformMath::Max<float>(1.0f / 30.0f * (float)vmdMotionInfo->maxFrame, MINIMUM_ANIMATION_LENGTH));
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
				if (MatchedBoneNameString.Contains(TEXT("Bowknot"), ESearchCase::IgnoreCase) ||
					MatchedBoneNameString.Contains(TEXT("HairS"), ESearchCase::IgnoreCase) ||
					MatchedBoneNameString.Contains(TEXT("HatB"), ESearchCase::IgnoreCase))
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Log,
						TEXT("Matched accessory animation source '%s' to skeleton bone '%s' (%d sorted keys)."),
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
			const bool bIsAccessoryBone =
				TargetBoneNameString.Contains(TEXT("Bowknot"), ESearchCase::IgnoreCase) ||
				TargetBoneNameString.Contains(TEXT("HairS"), ESearchCase::IgnoreCase) ||
				TargetBoneNameString.Contains(TEXT("HatB"), ESearchCase::IgnoreCase);
			if (bIsAccessoryBone)
			{
				FString MatchingAccessoryTrackNames;
				for (const MMD4UE4::VmdKeyTrackList& Track : vmdMotionInfo->keyBoneList)
				{
					if (Track.TrackName.Contains(TEXT("Bowknot"), ESearchCase::IgnoreCase) ||
						Track.TrackName.Contains(TEXT("HairS"), ESearchCase::IgnoreCase) ||
						Track.TrackName.Contains(TEXT("HatB"), ESearchCase::IgnoreCase))
					{
						if (!MatchingAccessoryTrackNames.IsEmpty())
						{
							MatchingAccessoryTrackNames += TEXT(", ");
						}
						MatchingAccessoryTrackNames += Track.TrackName;
					}
				}
				if (MatchingAccessoryTrackNames.IsEmpty())
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Warning,
						TEXT("Unmatched accessory bone '%s'; VMD has no bone tracks containing 'Bowknot', 'HairS', or 'HatB'."),
						*TargetBoneNameString);
				}
				else
				{
					UE_LOG(LogMMD4UE4_VMDFactory, Warning,
						TEXT("Unmatched accessory bone '%s'; VMD accessory tracks: %s"),
						*TargetBoneNameString,
						*MatchingAccessoryTrackNames);
				}
			}
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Warning,
				       TEXT("ImportVMDToAnimSequence Target Bone Not Found...[%s]"),
				       *targetName.ToString());
			}
			//nop
			//设定与帧相同的值
			for (int32 i = 0; i < DestSeq->GetNumberOfSampledKeys(); i++)
			{
				FTransform nrmTrnc;
				nrmTrnc.SetIdentity();
				RawTrack.PosKeys.Add(FVector3f(nrmTrnc.GetTranslation() + refTranslation));
				RawTrack.RotKeys.Add(FQuat4f(nrmTrnc.GetRotation()));
				RawTrack.ScaleKeys.Add(FVector3f(nrmTrnc.GetScale3D()));
			}
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

			bool dbg = false;
			if (targetName == L"右ひじ")
				dbg = true;

			//事先针对各轨迹，在没有父Bone的情况下，在Local坐标下计算预定全部注册的帧（如果有更好的处理……讨论）
			//如果进入90度以上的轴旋转，则由于四元数的原因或处理有错误而进入多余的旋转。
			//通过上述方式，仅通过Z旋转（旋转运动），下半身和上半身的轴成为物理上不可能的旋转的组合。臭虫。

			if (targetName == L"右足ＩＫ")
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Log, TEXT("右足ＩＫ"));
			}
			if (targetName == L"左足ＩＫ")
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Log, TEXT("左足ＩＫ"));
			}

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
	// Bake leg IK foot locks into the imported animation to reduce foot sliding.
	if (mmdExtend)
	{
		const FReferenceSkeleton& ReferenceSkeleton = Skeleton->GetReferenceSkeleton();
		const int32 NumFrames = DestSeq->GetNumberOfSampledKeys();
		const float LockDistanceThreshold = 1.5f;
		const int32 MaxIterations = 8;

		auto BuildGlobalPose = [&ReferenceSkeleton, &TempRawTrackList, NumBones, NumFrames](int32 FrameIndex,
			TArray<FTransform>& GlobalPose)
		{
			GlobalPose.SetNum(NumBones);
			for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
			{
				const FRawAnimSequenceTrack& Track = TempRawTrackList[BoneIndex];
				const int32 KeyIndex = FMath::Min(FrameIndex, Track.PosKeys.Num() - 1);
				FTransform LocalTransform = FTransform::Identity;
				if (KeyIndex >= 0)
				{
					LocalTransform = FTransform(
						FQuat(Track.RotKeys[KeyIndex]),
						FVector(Track.PosKeys[KeyIndex]),
						FVector(Track.ScaleKeys.IsValidIndex(KeyIndex) ? Track.ScaleKeys[KeyIndex] : FVector3f(1.0f)));
				}
				const int32 ParentIndex = ReferenceSkeleton.GetParentIndex(BoneIndex);
				GlobalPose[BoneIndex] = ParentIndex == INDEX_NONE
					? LocalTransform
					: LocalTransform * GlobalPose[ParentIndex];
			}
		};

		auto SetLocalFromGlobal = [&ReferenceSkeleton, &TempRawTrackList](int32 BoneIndex, int32 FrameIndex,
			const FTransform& GlobalTransform, const TArray<FTransform>& GlobalPose)
		{
			const int32 ParentIndex = ReferenceSkeleton.GetParentIndex(BoneIndex);
			const FTransform LocalTransform = ParentIndex == INDEX_NONE
				? GlobalTransform
				: GlobalTransform.GetRelativeTransform(GlobalPose[ParentIndex]);
			TempRawTrackList[BoneIndex].PosKeys[FrameIndex] = FVector3f(LocalTransform.GetLocation());
			TempRawTrackList[BoneIndex].RotKeys[FrameIndex] = FQuat4f(LocalTransform.GetRotation());
		};

		for (const FMMD_IKInfo& IKInfo : mmdExtend->IkInfoList)
		{
			const FString IKName = IKInfo.IKBoneName.ToString();
			if ((!IKName.Contains(TEXT("足")) && !IKName.Contains(TEXT("Leg"), ESearchCase::IgnoreCase)) ||
				IKInfo.IKBoneIndex < 0 || IKInfo.TargetBoneIndex < 0 ||
				IKInfo.IKBoneIndex >= NumBones || IKInfo.TargetBoneIndex >= NumBones ||
				IKInfo.ikLinkList.Num() == 0)
			{
				continue;
			}

			TArray<FTransform> GlobalPose;
			FVector LockedLocation = FVector::ZeroVector;
			bool bHasLock = false;
			int32 LockedFrameCount = 0;
			for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
			{
				BuildGlobalPose(FrameIndex, GlobalPose);
				const FVector CurrentLocation = GlobalPose[IKInfo.IKBoneIndex].GetLocation();
				if (!bHasLock || FVector::DistSquared(CurrentLocation, LockedLocation) > FMath::Square(LockDistanceThreshold))
				{
					LockedLocation = CurrentLocation;
					bHasLock = true;
					continue;
				}

				FTransform IKGlobal = GlobalPose[IKInfo.IKBoneIndex];
				IKGlobal.SetLocation(LockedLocation);
				SetLocalFromGlobal(IKInfo.IKBoneIndex, FrameIndex, IKGlobal, GlobalPose);
				++LockedFrameCount;

				for (int32 Iteration = 0; Iteration < MaxIterations; ++Iteration)
				{
					BuildGlobalPose(FrameIndex, GlobalPose);
					const FVector EndLocation = GlobalPose[IKInfo.TargetBoneIndex].GetLocation();
					if (FVector::DistSquared(EndLocation, LockedLocation) <= FMath::Square(0.5f))
					{
						break;
					}

					bool bSolved = false;
					for (int32 LinkIndex = IKInfo.ikLinkList.Num() - 1; LinkIndex >= 0; --LinkIndex)
					{
						const int32 LinkBoneIndex = IKInfo.ikLinkList[LinkIndex].BoneIndex;
						if (LinkBoneIndex < 0 || LinkBoneIndex >= NumBones)
						{
							continue;
						}
						const FVector LinkLocation = GlobalPose[LinkBoneIndex].GetLocation();
						const FVector ToEnd = EndLocation - LinkLocation;
						const FVector ToTarget = LockedLocation - LinkLocation;
						if (ToEnd.IsNearlyZero() || ToTarget.IsNearlyZero())
						{
							continue;
						}

						FQuat DeltaRotation = FQuat::FindBetweenNormals(ToEnd.GetSafeNormal(), ToTarget.GetSafeNormal());
						const float MaxAngle = IKInfo.RotLimit > 0.0f ? IKInfo.RotLimit : PI;
						const float Angle = DeltaRotation.GetAngle();
						if (Angle > MaxAngle)
						{
							DeltaRotation = FQuat(DeltaRotation.GetRotationAxis(), MaxAngle);
						}

						FTransform LinkGlobal = GlobalPose[LinkBoneIndex];
						LinkGlobal.SetRotation(DeltaRotation * LinkGlobal.GetRotation());
						SetLocalFromGlobal(LinkBoneIndex, FrameIndex, LinkGlobal, GlobalPose);
						BuildGlobalPose(FrameIndex, GlobalPose);
						bSolved = true;
						break;
					}
					if (!bSolved)
					{
						break;
					}
				}
			}

			if (LockedFrameCount > 0)
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
					TEXT("Baked leg IK foot lock: %s, locked frames=%d/%d"),
					*IKName, LockedFrameCount, NumFrames);
			}
		}
	}
	adc.OpenBracket(LOCTEXT("AddNewRawTrack_Bracket", "Adding new Bone Animation Track"));
	/* AddTrack */
	for (int32 BoneIndex = 0; BoneIndex < NumBones; ++BoneIndex)
	{
		FName BoneName = Skeleton->GetReferenceSkeleton().GetBoneName(BoneIndex);

		FRawAnimSequenceTrack& RawTrack = TempRawTrackList[BoneIndex];

		//DestSeq->AddNewRawTrack(BoneName, &RawTrack);

		int32 NewTrackIndex = INDEX_NONE; //5.8
		if (RawTrack.PosKeys.Num() > 1)
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
					BoneNameString.StartsWith(TEXT("_HemB-L-E")) ||
					BoneNameString.StartsWith(TEXT("_HemB-R-E")) ||
					BoneNameString.Contains(TEXT("HairS"), ESearchCase::IgnoreCase) ||
					BoneNameString.Contains(TEXT("BowknotB"), ESearchCase::IgnoreCase) ||
					BoneNameString.Contains(TEXT("HatB"), ESearchCase::IgnoreCase);
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
		FName MappedName;
		if (FindTableRowMMD2UEName(ReNameTable, Name, &MappedName))
		{
			Name = MappedName;
		}
		if (Name.IsNone())
		{
			UE_LOG(LogMMD4UE4_VMDFactory, Warning,
				TEXT("Skipping VMD morph track '%s': resolved curve name is empty."),
				*vmdFaceTrackPtr->TrackName);
			continue;
		}
		//self
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

					//FSmartName NewName; //ydgro
#if UE_VERSION_OLDER_THAN(5,3,0)
					Skeleton->AddSmartNameAndModify(USkeleton::AnimCurveMappingName, Name, NewName);
#else  
					Skeleton->AddCurveMetaData(Name); //ydgro
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
	adc.UpdateAttributesFromSkeleton(Skeleton); //ydgro
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
			if (TrackName.ToString().Contains(TEXT("HemB")))
			{
				UE_LOG(LogMMD4UE4_VMDFactory, Log,
					TEXT("Controller model HemB track name: '%s'."),
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

	UVmdFactory* MyFactory = NewObject<UVmdFactory>(); //5.8
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
					VMDImportOptions* ImportOptions = nullptr; //5.8
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
#undef LOCTEXT_NAMESPACE