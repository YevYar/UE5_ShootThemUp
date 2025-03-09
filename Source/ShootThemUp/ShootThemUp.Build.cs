// "Shoot Them Up" Game by YevYar, All rights reserved

using UnrealBuildTool;

public class ShootThemUp : ModuleRules
{
	public ShootThemUp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Niagara",
            "PhysicsCore",
            "GameplayTasks",
            "NavigationSystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Allow includes without full path, just by the name of the file. However, I prefer full path
        PublicIncludePaths.AddRange(new string[] {
            "ShootThemUp/Public/AI",
            "ShootThemUp/Public/AI/Components",
            "ShootThemUp/Public/AI/EQS",
            "ShootThemUp/Public/AI/Services",
            "ShootThemUp/Public/AI/Tasks",
            "ShootThemUp/Public/Animations",
            "ShootThemUp/Public/Dev",
            "ShootThemUp/Public/Pickups",
            "ShootThemUp/Public/Player",
            "ShootThemUp/Public/UI",
            "ShootThemUp/Public/Weapons",
            "ShootThemUp/Public/Weapons/Components"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
