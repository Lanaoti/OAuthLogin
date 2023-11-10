// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class OAuthLogin : ModuleRules
{
    [ConfigFile(ConfigHierarchyType.Game, "/Script/OAuthLogin.OAuthLoginSettings", "bEnableHuaWei")]
    bool bEnableHuaWei = false;

    [ConfigFile(ConfigHierarchyType.Game, "/Script/OAuthLogin.OAuthLoginSettings", "bEnableWeChat")]
    bool bEnableWeChat = false;

    [ConfigFile(ConfigHierarchyType.Game, "/Script/OAuthLogin.OAuthLoginSettings", "bEnableQQ")]
    bool bEnableQQ = false;

    [ConfigFile(ConfigHierarchyType.Game, "/Script/OAuthLogin.OAuthLoginSettings", "bEnableTapTap")]
    bool bEnableTapTap = false;

    [ConfigFile(ConfigHierarchyType.Game, "/Script/OAuthLogin.OAuthLoginSettings", "bEnableHaoYouKuaiBao")]
    bool bEnableHaoYouKuaiBao = false;

    [ConfigFile(ConfigHierarchyType.Game, "/Script/OAuthLogin.OAuthLoginSettings", "bEnableGoogle")]
    bool bEnableGoogle = false;

    [ConfigFile(ConfigHierarchyType.Game, "/Script/OAuthLogin.OAuthLoginSettings", "bEnableFacebook")]
    bool bEnableFacebook = false;

    [ConfigFile(ConfigHierarchyType.Game, "/Script/OAuthLogin.OAuthLoginSettings", "bEnableSteam")]
    bool bEnableSteam = false;

    public OAuthLogin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {

            });


        PrivateIncludePaths.AddRange(
            new string[] {
            });


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
            });


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "JSON",
                "JsonUtilities"
            });

        if (bEnableHuaWei)
        {
            DynamicallyLoadedModuleNames.Add("HuaWei");
        }

        if (bEnableWeChat)
        {
            DynamicallyLoadedModuleNames.Add("WeChat");
        }

        if (bEnableQQ)
        {
            DynamicallyLoadedModuleNames.Add("QQ");
        }

        if (bEnableTapTap)
        {
            DynamicallyLoadedModuleNames.Add("TapTap");
        }

        if (bEnableHaoYouKuaiBao)
        {
            DynamicallyLoadedModuleNames.Add("HaoYouKuaiBao");
        }

        if (bEnableGoogle)
        {
            DynamicallyLoadedModuleNames.Add("Google");
        }

        if (bEnableFacebook)
        {
            DynamicallyLoadedModuleNames.Add("Facebook");
        }

        if (bEnableSteam)
        {
            DynamicallyLoadedModuleNames.Add("Steam");
        }

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "OAuthLogin_UPL.xml"));
        }
    }
}
