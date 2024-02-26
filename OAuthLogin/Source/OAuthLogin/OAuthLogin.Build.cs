// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System.Collections.Generic;
#if UE_5_0_OR_LATER
using EpicGames.Core;
#else
using Tools.DotNETCommon;
#endif

struct OAuthChannel
{
    public OAuthChannel(string ModuleName, string MacroName, bool bEnable)
    {
        this.ModuleName = ModuleName;
        this.MacroName = MacroName;

        this.bEnable = bEnable;
    }

    public string ModuleName;
    public string MacroName;

    public bool bEnable;
}

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

        OptimizeCode = CodeOptimization.InShippingBuildsOnly;

        ConfigCache.ReadSettings(DirectoryReference.FromFile(Target.ProjectFile), Target.Platform, this);

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
                "Json",
                "JsonUtilities"
            });

        System.Console.WriteLine("-------------------- OAuthLogin Config ---------------------");

        List<OAuthChannel> OAuthChannels = new List<OAuthChannel>();
        OAuthChannels.Add(new OAuthChannel("HuaWei",        "ENABLE_HUAWEI",        bEnableHuaWei));
        OAuthChannels.Add(new OAuthChannel("WeChat",        "ENABLE_WECHAT",        bEnableWeChat));
        OAuthChannels.Add(new OAuthChannel("QQ",            "ENABLE_QQ",            bEnableQQ));
        OAuthChannels.Add(new OAuthChannel("TapTap",        "ENABLE_TAPTAP",        bEnableTapTap));
        OAuthChannels.Add(new OAuthChannel("HaoYouKuaiBao", "ENABLE_HAOYOUKUAIBAO", bEnableHaoYouKuaiBao));
        OAuthChannels.Add(new OAuthChannel("Google",        "ENABLE_GOOGLE",        bEnableGoogle));
        OAuthChannels.Add(new OAuthChannel("Facebook",      "ENABLE_FACEBOOK",      bEnableFacebook));
        OAuthChannels.Add(new OAuthChannel("Steam",         "ENABLE_STEAM",         bEnableSteam));

        foreach (OAuthChannel Channel in OAuthChannels)
        {
            if (Channel.bEnable)
            {
                DynamicallyLoadedModuleNames.Add(Channel.ModuleName);
            }
            PublicDefinitions.Add(string.Format("{0} = {1}", Channel.MacroName, (Channel.bEnable ? "1" : "0")));

            System.Console.WriteLine(string.Format("{0} enable is {1}", Channel.ModuleName, Channel.bEnable));
        }
        System.Console.WriteLine("---------------------------------------------------------------");

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "OAuthLogin_UPL.xml"));
        }
    }
}
