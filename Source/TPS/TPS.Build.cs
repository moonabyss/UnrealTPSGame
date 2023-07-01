// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Linq;
using EpicGames.Core;

public class TPS : ModuleRules
{
    public TPS(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Json",
            "JsonUtilities", "UMG" });

        if (Target.Configuration != UnrealTargetConfiguration.Shipping)
        {
            PublicDependencyModuleNames.Add("FunctionalTesting");
        }

        PublicIncludePaths.Add("TPS");

        if (Target.ProjectDefinitions.Contains("UNOPTIMIZED_CODE"))
        {
            OptimizeCode = CodeOptimization.Never;
        }
    }
}
