// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
/*=============================================================================
Main implementation of FFbxImporter : import FBX data to Unreal
=============================================================================*/
#include "MMDSkeletalMeshImportData.h"
#include "MMDStaticMeshImportData.h"
#include "VmdFactory.h"
#include "VmdImporter.h"
#include "VmdOptionWindow.h"
#include "EngineAnalytics.h"
#include "MainFrame.h"
#include "Rig/IKRigDefinition.h"

//DEFINE_LOG_CATEGORY(LogPmx);
#define LOCTEXT_NAMESPACE "VmdMainImport"
//TSharedPtr<PmxMeshInfo> PmxMeshInfo::StaticInstance;

VMDImportOptions* GetVMDImportOptions(
	class FVmdImporter* VmdImporter,
	UVmdImportUI* ImportUI,
	bool bShowOptionDialog,
	const FString& FullPath,
	bool& bOutOperationCanceled,
	bool& bOutImportAll,
	bool bIsObjFormat,
	bool bForceImportType ,
	EVMDImportType ImportType)
{
	if (!VmdImporter || !ImportUI)
	{
		bOutOperationCanceled = true;
		bOutImportAll = false;
		return nullptr;
	}

	bOutOperationCanceled = false;

	// 清空已失效的资产引用：这些是裸UObject指针，资产被删除后会悬空，
	// 再次导入时访问会导致 0xC0000005。仍有效的引用保留（沿用上次选择）。
	if (VMDImportOptions* CachedOptions = VmdImporter->GetMutableImportOptions())
	{
		if (CachedOptions->SkeletonForAnimation && !IsValid(CachedOptions->SkeletonForAnimation))
		{
			CachedOptions->SkeletonForAnimation = nullptr;
		}
		if (CachedOptions->SkeletalMeshForAnimation && !IsValid(CachedOptions->SkeletalMeshForAnimation))
		{
			CachedOptions->SkeletalMeshForAnimation = nullptr;
		}
		if (CachedOptions->AnimSequenceAsset && !IsValid(CachedOptions->AnimSequenceAsset))
		{
			CachedOptions->AnimSequenceAsset = nullptr;
		}
		if (CachedOptions->MMD2UE4NameTableRow && !IsValid(CachedOptions->MMD2UE4NameTableRow))
		{
			CachedOptions->MMD2UE4NameTableRow = nullptr;
		}
		if (CachedOptions->MmdExtendAsset && !IsValid(CachedOptions->MmdExtendAsset))
		{
			CachedOptions->MmdExtendAsset = nullptr;
		}
		if (CachedOptions->IKRig && !IsValid(CachedOptions->IKRig))
		{
			CachedOptions->IKRig = nullptr;
		}
	}

	if (bShowOptionDialog)
	{
		bOutImportAll = false;

		VMDImportOptions* ImportOptions
			= VmdImporter->GetMutableImportOptions();
		if (!ImportOptions)
		{
			bOutOperationCanceled = true;
			bOutImportAll = false;
			return nullptr;
		}
		// if Skeleton was set by outside, please make sure copy back to UI
		if (ImportOptions->SkeletonForAnimation)
		{
			ImportUI->Skeleton = ImportOptions->SkeletonForAnimation;
		}
		else
		{
			ImportUI->Skeleton = nullptr;
		}

		if (ImportOptions->SkeletalMeshForAnimation)
		{
			ImportUI->SkeletonMesh = ImportOptions->SkeletalMeshForAnimation;
		}
		else
		{
			ImportUI->SkeletonMesh = nullptr;
		}

		//last select asset ref

		if (ImportOptions->MmdExtendAsset)
		{
			ImportUI->MmdExtendAsset = ImportOptions->MmdExtendAsset;
		}
		else
		{
			ImportUI->MmdExtendAsset = nullptr;
		}
		if (ImportOptions->IKRig)
		{
			ImportUI->IKRig = ImportOptions->IKRig;
		}
		else
		{
			ImportUI->IKRig = nullptr;
		}
		if (ImportOptions->MMD2UE4NameTableRow)
		{
			ImportUI->MMD2UE4NameTableRow = ImportOptions->MMD2UE4NameTableRow;
		}
		else
		{
			ImportUI->MMD2UE4NameTableRow = nullptr;
		}
		if (ImportOptions->AnimSequenceAsset)
		{
			ImportUI->AnimSequenceAsset = ImportOptions->AnimSequenceAsset;
		}
		else
		{
			ImportUI->AnimSequenceAsset = nullptr;
		}

		// IK/碰撞求解：沿用上次的勾选状态
		ImportUI->bSolveIK = ImportOptions->bSolveIK;
		ImportUI->bApplyIKCollision = ImportOptions->bApplyIKCollision;

		//ImportUI->bImportAsSkeletal = ImportUI->MeshTypeToImport == VMDIT_Animation;
		ImportUI->bIsObjImport = bIsObjFormat;

		TSharedPtr<SWindow> ParentWindow;

		if (FModuleManager::Get().IsModuleLoaded("MainFrame"))
		{
			IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
			ParentWindow = MainFrame.GetParentWindow();
		}

		TSharedRef<SWindow> Window = SNew(SWindow)
			.Title(NSLOCTEXT("IM4U", "VMDImportOpionsTitle", "VMD Import Options"))
			.SizingRule(ESizingRule::Autosized);

		TSharedPtr<SVmdOptionWindow> VmdOptionWindow;
		Window->SetContent
			(
			SAssignNew(VmdOptionWindow, SVmdOptionWindow)
			.ImportUI(ImportUI)
			.WidgetWindow(Window)
			.FullPath(FText::FromString(FullPath))
			.ForcedImportType(bForceImportType ? TOptional<EVMDImportType>(ImportType) : TOptional<EVMDImportType>())
			.IsObjFormat(bIsObjFormat)
			);

		// @todo: we can make this slow as showing progress bar later
		FSlateApplication::Get().AddModalWindow(Window, ParentWindow, false);

		ImportUI->SaveConfig();

		if (ImportUI->StaticMeshImportData)
		{
			ImportUI->StaticMeshImportData->SaveConfig();
		}

		if (ImportUI->SkeletalMeshImportData)
		{
			ImportUI->SkeletalMeshImportData->SaveConfig();
		}

		if (VmdOptionWindow->ShouldImport())
		{
			bOutImportAll = VmdOptionWindow->ShouldImportAll();

			// open dialog
			// see if it's canceled
			ApplyVMDImportUIToImportOptions(ImportUI, *ImportOptions);

			return ImportOptions;
		}
		else
		{
			bOutOperationCanceled = true;
		}
	}
	else if (GIsAutomationTesting)
	{
		//Automation tests set ImportUI settings directly.  Just copy them over
		VMDImportOptions* ImportOptions = VmdImporter->GetMutableImportOptions();
		ApplyVMDImportUIToImportOptions(ImportUI, *ImportOptions);
		return ImportOptions;
	}
	else
	{
		return VmdImporter->GetMutableImportOptions();
	}
	return nullptr;

}

void ApplyVMDImportUIToImportOptions(
	UVmdImportUI* ImportUI,
	VMDImportOptions& InOutImportOptions
	)
{

	check(ImportUI);

	InOutImportOptions.SkeletonForAnimation = ImportUI->Skeleton;
	InOutImportOptions.SkeletalMeshForAnimation = ImportUI->SkeletonMesh;

#if 0
	// animation options
	InOutImportOptions.AnimationLengthImportType = ImportUI->AnimSequenceImportData->AnimationLength;
	InOutImportOptions.AnimationRange.X = ImportUI->AnimSequenceImportData->StartFrame;
	InOutImportOptions.AnimationRange.Y = ImportUI->AnimSequenceImportData->EndFrame;
	InOutImportOptions.AnimationName = ImportUI->AnimationName;
	InOutImportOptions.bPreserveLocalTransform = ImportUI->bPreserveLocalTransform;
	InOutImportOptions.bImportCustomAttribute = ImportUI->AnimSequenceImportData->bImportCustomAttribute;
#endif
	//add self
	InOutImportOptions.AnimSequenceAsset = ImportUI->AnimSequenceAsset;
	InOutImportOptions.MMD2UE4NameTableRow = ImportUI->MMD2UE4NameTableRow;
	InOutImportOptions.MmdExtendAsset = ImportUI->MmdExtendAsset;
	InOutImportOptions.IKRig = ImportUI->IKRig;
	InOutImportOptions.bSolveIK = ImportUI->bSolveIK;
	InOutImportOptions.bApplyIKCollision = ImportUI->bApplyIKCollision;
}

TSharedPtr<FVmdImporter> FVmdImporter::StaticInstance;

FVmdImporter::FVmdImporter()
	:/* Scene(nullptr)
	, */ImportOptions(nullptr)
{
	ImportOptions = MakeUnique<VMDImportOptions>();
	FMemory::Memzero(*ImportOptions);
	// FMemory::Memzero 会把 bool 全部置 0，这里恢复默认开启的IK选项
	ImportOptions->bSolveIK = true;
	ImportOptions->bApplyIKCollision = true;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
FVmdImporter::~FVmdImporter()
{
	CleanUp();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
FVmdImporter* FVmdImporter::GetInstance()
{
	// CleanUp() 会释放 ImportOptions，此时必须重建单例，
	// 否则后续导入拿到的是空壳对象，解引用空指针导致崩溃。
	if (!StaticInstance.IsValid() || !StaticInstance->ImportOptions.IsValid())
	{
		StaticInstance = MakeShareable(new FVmdImporter());
	}
	return StaticInstance.Get();
}

void FVmdImporter::DeleteInstance()
{
	StaticInstance.Reset();
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
void FVmdImporter::CleanUp()
{
#if 0
	ClearTokenizedErrorMessages();
	ReleaseScene();

	delete GeometryConverter;
	GeometryConverter = nullptr;
#endif
	ImportOptions.Reset();
#if 0
	if (SdkManager)
	{
		SdkManager->Destroy();
	}
	SdkManager = nullptr;
	Logger = nullptr;
#endif
}

const VMDImportOptions* FVmdImporter::GetImportOptions() const
{
	return ImportOptions.Get();
}

VMDImportOptions* FVmdImporter::GetMutableImportOptions() const
{
	return ImportOptions.Get();
}

///////////////////////////////////////////////////////////////////////////

UVmdImportUI::UVmdImportUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)//, MMD2UE4NameTableRow(MMD2UE4NameTableRowDmmy)
{
#if 0
	StaticMeshImportData = CreateDefaultSubobject<UMMDStaticMeshImportData>(TEXT("StaticMeshImportData"));
	SkeletalMeshImportData = CreateDefaultSubobject<UMMDSkeletalMeshImportData>(TEXT("SkeletalMeshImportData"));
	/*AnimSequenceImportData = CreateDefaultSubobject<UFbxAnimSequenceImportData>(TEXT("AnimSequenceImportData"));
	TextureImportData = CreateDefaultSubobject<UFbxTextureImportData>(TEXT("TextureImportData"));
	*/
#endif
}


bool UVmdImportUI::CanEditChange(const FProperty* InProperty) const
{
	bool bIsMutable = Super::CanEditChange(InProperty);
	if (bIsMutable && InProperty != nullptr)
	{
		FName PropName = InProperty->GetFName();

		if (PropName == TEXT("StartFrame") || PropName == TEXT("EndFrame"))
		{
			//bIsMutable = AnimSequenceImportData->AnimationLength == FBXALIT_SetRange && bImportAnimations;
		}
		else if (PropName == TEXT("bImportCustomAttribute") || PropName == TEXT("AnimationLength"))
		{
			bIsMutable = bImportAnimations;
		}

		if (bIsObjImport == false && InProperty->GetBoolMetaData(TEXT("OBJRestrict")))
		{
			bIsMutable = false;
		}
	}

	return bIsMutable;
}


#undef LOCTEXT_NAMESPACE