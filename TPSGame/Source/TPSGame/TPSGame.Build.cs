// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class TPSGame : ModuleRules
{
	public TPSGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput","Boost" });
	}
	private string ModulePath
    {
		get { return ModuleDirectory; }
    }

	private string ThridPartyPath
    {
		get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty")); }
    }
}
