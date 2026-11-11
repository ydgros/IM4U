// Copyright 2023 NaN_Name, Inc. All Rights Reserved.
#pragma once
// UE5.9: the engine FBX import data headers are no longer shipped with the plugin's FBX SDK-free build;
// the MMD import data derives from the generic asset import data instead.
#include "EditorFramework/AssetImportData.h"
#include "MMDMeshImportData.generated.h"

UENUM()
enum EMMDNormalImportMethod
{
	MMDNIM_ComputeNormals UMETA(DisplayName="Compute Normals"),
	MMDNIM_ImportNormals UMETA(DisplayName="Import Normals"),
	MMDNIM_ImportNormalsAndTangents UMETA(DisplayName="Import Normals and Tangents"),
	MMDNIM_MAX,
};

/**
 * Import data and options used when importing any mesh from MMD
 */
UCLASS(config=EditorUserSettings, configdonotcheckdefaults, abstract)
class UMMDMeshImportData : public UAssetImportData// UMMDAssetImportData
{
	GENERATED_UCLASS_BODY()

	/** Translation applied to the imported geometry */
	UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category=ImportSettings, meta=(ToolTip="Import translation applied to the imported MMD asset."))
	FVector ImportTranslation;

	/** Rotation applied to the imported geometry */
	UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category=ImportSettings, meta=(ToolTip="Import rotation applied to the imported MMD asset."))
	FRotator ImportRotation;

	/** Scale applied to the imported geometry */
	UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category=ImportSettings, meta=(ToolTip="Import scale applied to the imported MMD asset."))
	float ImportUniformScale;

	/** Enables importing of mesh LODs from MMD LOD groups, if present in the MMD file */
	UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category=ImportSettings, meta=(OBJRestrict="true", ImportType="Mesh", ToolTip="If enabled, creates LOD models for Unreal meshes from LODs in the import file; If not enabled, only the base mesh from the LOD group is imported"))
	uint32 bImportMeshLODs:1;

	/** Enabling this option will read the tangents(tangent,binormal,normal) from MMD file instead of generating them automatically. */
	UPROPERTY(EditAnywhere, AdvancedDisplay, config, Category=ImportSettings, meta=(ImportType="Mesh"))
	TEnumAsByte<enum EMMDNormalImportMethod> NormalImportMethod;

	//bool CanEditChange( const FProperty* InProperty ) const;
	virtual bool CanEditChange( const FProperty* InProperty ) const override;
};