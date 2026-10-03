// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class IM4U : ModuleRules
	{
		private string ModulePath
		{
			get { return ModuleDirectory; }
		}

		private string ThirdPartyPath
		{
			get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
		}

		
		public IM4U(ReadOnlyTargetRules Target) : base(Target)
		{
			//string LibName; //not used
			/*if ((Target.Platform == UnrealTargetPlatform.Win64 ))
			{


			}*/
	
			/*PublicIncludePaths.AddRange(
                new string[] {
					// ... add public include paths required here ...
				}
				);*/

			PrivateIncludePaths.AddRange(
                new string[] {
                    "IM4U/Private",
					ThirdPartyPath+"/glm"
					// ... add other private include paths required here ...
				}
                );

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine", 
					"InputCore", 
					"AssetTools",
					"Slate" ,
					"SlateCore",
					"RawMesh" ,
					"MessageLog",
					"MainFrame",
					"PropertyEditor",
                    "RHI",
                    "RenderCore",
                    "ContentBrowser",
					"PhysicsUtilities","SkeletalMeshUtilitiesCommon",
                    "IKRig",
					"IKRigEditor"
					// ... add other public dependencies that you statically link with here ...
				}
				);

			if (Target.Type == TargetType.Editor)
			{
				PrivateDependencyModuleNames.AddRange(new string[]
				{
					  "UnrealEd",
				});
			}

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"EditorStyle",
					"EditorWidgets",
					"ApplicationCore",
					"MeshDescription",
					"MeshUtilities"
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
                {
                    "AssetRegistry",
					// ... add any modules that your module loads dynamically here ...
				}
				);

            /*PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks", "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
            if (Target.bBuildEditor)
            {
                PrivateDependencyModuleNames.AddRange(new string[] { "LevelEditor", "UnrealEd" });
            }
            else
            {
                PrivateDependencyModuleNames.AddRange(new string[] { });
            }*/
        }
    }
}
