﻿using UnrealBuildTool;

public class SteamModule : ModuleRules
{
    public SteamModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                // "CoreUObject",
                // "Engine"
            }
        );
        
        
    }
}