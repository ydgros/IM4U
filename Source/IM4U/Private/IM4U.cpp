// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#include "IIM4U.h"
#include "PmxImporter.h"
#include "VmdImporter.h"
#include "Factories/FbxAssetImportData.h"

class FIM4U : public IIM4U //5.8
{
public://基class IModuleInterface中函数 FIM4U::StartupModule 的可见性从 public 更改为 private
	/* IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FIM4U, IM4U )

void UFbxAssetImportData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}//ydgro 实现 已声明 UFbxAssetImportData::PostEditChangeProperty PostEditChangeProperty

void FIM4U::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}

void FIM4U::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FPmxImporter::DeleteInstance();
	FVmdImporter::DeleteInstance();
}