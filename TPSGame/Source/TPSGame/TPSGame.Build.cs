// Copyright Epic Games, Inc. All Rights Reserved.



using System;
using System.IO;
using UnrealBuildTool;


public class TPSGame : ModuleRules
{


    public TPSGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { });


        PublicDefinitions.Add("DBG");
        PublicDefinitions.Add("BETA");
        PublicDefinitions.Add("OFFICIAL_BUILD");


        string BoostPath = Path.Combine("C:/Users/cksdu/Desktop/TPS_Project/TPSGame" + "/ThirdParty/Boost");
        PublicIncludePaths.AddRange(new string[] {
                Path.Combine(BoostPath, "include")
                });


        string libPath = Path.Combine("C:/Users/cksdu/Desktop/TPS_Project/TPSGame" + "/ThirdParty/Boost/lib");

        bUseRTTI = true;

        PublicAdditionalLibraries.AddRange(new string[] {
        Path.Combine(libPath, "libboost_atomic-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_atomic-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_chrono-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_chrono-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_container-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_container-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_context-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_context-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_contract-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_contract-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_coroutine-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_coroutine-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_date_time-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_date_time-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_exception-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_exception-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_fiber-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_fiber-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_filesystem-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_filesystem-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_graph-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_graph-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_iostreams-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_iostreams-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_json-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_json-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_locale-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_locale-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_log-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_log-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_log_setup-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_log_setup-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_c99-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_c99-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_c99f-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_c99f-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_c99l-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_c99l-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_tr1-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_tr1-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_tr1f-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_tr1f-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_tr1l-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_math_tr1l-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_nowide-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_nowide-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_prg_exec_monitor-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_prg_exec_monitor-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_program_options-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_program_options-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_python310-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_python310-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_random-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_random-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_regex-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_regex-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_serialization-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_serialization-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_stacktrace_noop-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_stacktrace_noop-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_stacktrace_windbg-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_stacktrace_windbg-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_stacktrace_windbg_cached-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_stacktrace_windbg_cached-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_system-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_system-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_test_exec_monitor-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_test_exec_monitor-vc142-mt-x64-1_84.lib"),
        //Path.Combine(libPath, "libboost_thread-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_thread-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_timer-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_timer-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_type_erasure-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_type_erasure-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_unit_test_framework-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_unit_test_framework-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_url-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_url-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_wave-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_wave-vc142-mt-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_wserialization-vc142-mt-gd-x64-1_84.lib"),
        Path.Combine(libPath, "libboost_wserialization-vc142-mt-x64-1_84.lib"),
            });

    }
}



