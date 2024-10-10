// "Shoot Them Up" Game by YevYar, All rights reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class ShootThemUpEditorTarget : TargetRules
{
	public ShootThemUpEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "ShootThemUp" } );
	}
}
