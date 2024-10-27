// "Shoot Them Up" Game by YevYar, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpTarget : TargetRules
{
	public ShootThemUpTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );

        switch (Target.Configuration)
        {
            case UnrealTargetConfiguration.Debug:
            case UnrealTargetConfiguration.DebugGame:
            case UnrealTargetConfiguration.Development:
            case UnrealTargetConfiguration.Test:
                ProjectDefinitions.Add("STU_DEBUG=1");
                break;
            default:
                ProjectDefinitions.Add("STU_DEBUG=0");
                break;
        }
    }
}
