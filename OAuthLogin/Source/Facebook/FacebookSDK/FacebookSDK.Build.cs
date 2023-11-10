// Copyright 2022 CQUnreal. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class FacebookSDK : ModuleRules
{
    public FacebookSDK(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "FacebookSDK_APL.xml"));
        }
    }
}
