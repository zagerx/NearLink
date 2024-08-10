#!/usr/bin/env python3
# encoding=utf-8
# =========================================================================
# @brief    Target Definitions File
# Copyright CompanyNameMagicTag 2022-2022. All rights reserved.
# =========================================================================
import os
import copy

target_template = {
    'target_standard_bs25_application_evb_template': {
        'chip': 'bs25',
        'core': 'acore',
        'board': 'evb',
        'tool_chain': 'riscv32_musl_b090_fp',
        'build_type': 'COMPILE',
        'os': 'liteos',
        'std_libs': ['m', 'c', 'gcc'],
        'defines': ['-:TARGET_CHIP_BS25=1', 'BUILD_APPLICATION_STANDARD', 'UART_DRIVER_CONFIG_USE_VETOS_IN_STEAD_OF_TIMERS',
                    'LOSCFG_DRIVERS_EMMC', 'LOSCFG_FS_FAT_CACHE', "CONFIG_APPS_CORE", "CMD_ENABLE", "LIBAPP_VERSION",
                    "LIBBUILD_VERSION", "LIBCMD", "LIBCONNECTIVITY", "LIBCPU_LOAD", "LIBCPU_UTILS", "LIBLIB_UTILS",
                    "LIBLOG", "LIBLOG_READER", "LIBPANIC", "LIBTEST_COMMON", "LITEOS_ONETRACK", "PRE_ASIC",
                    "SUPPORT_CXX", "USE_CMSIS_OS", "USE_LITEOS", "VERSION_STANDARD", "_ALL_SOURCE", "__LITEOS__",
                    "CONFIG_NV_SUPPORT_SINGLE_CORE_SYSTEM", 'CONFIG_NV_FEATURE_SUPPORT',
                    'TEST_SUITE'],
        'defines_set': ['libsec_defines', 'chip_defines', 'version_defines'],
        'ram_component': ['bs25_std_lds', 'chip_bs25', 'bs25_app_standard', 'driver_header', '-:sec_port', 'pm_lpc', 'app_info',
                          'bs25_mem_config', 'libboundscheck', 'bs25_clocks', 'bs25_pmu', 'nv', 'nv_bs25'],
        'ram_component_set': ['pmp_set', 'std_common_lib', 'time_set', 'mem', 'watchdog', 'sec', 'security', 'pm_pmu_set', 'pm_clock_set', 'adc'],
        'bin_name': 'application',
        'application': 'standard',
        'ccflags': [
            '-:-mabi=ilp32', '-:-march=rv32imc', '-mabi=ilp32f', '-march=rv32imfc', '-madjust-regorder', '-madjust-const-cost', '-freorder-commu-args', '-fimm-compare-expand',
            '-frmv-str-zero', '-mfp-const-opt', '-frtl-sequence-abstract', "-:-Wno-type-limits",
            '-frtl-hoist-sink', '-fsafe-alias-multipointer', '-finline-optimize-size', '--short-enums',
            '-fmuliadd-expand', '-mlli-expand', '-Wa,-mcjal-expand', '-foptimize-reg-alloc', '-fsplit-multi-zero-assignments',
            '-floop-optimize-size',  '-mpattern-abstract', '-foptimize-pro-and-epilogue'],
        'linkflags': [
            '-Wl,--cjal-relax',
            '-Wl,--dslf',
        ],
        'arch': 'riscv31',
        'hso_enable': True,
        'gen_parse_tool': False,
        'gen_sec_bin': True,
        'image_version': 'bs25_hs',
        'sector_cfg': 'standard-bs25',
        'ssb_cfg': 'ssb-bs25-evb-debug',
    },
    'target_standard_bs25_application_evb_template': {
        'chip': 'bs25',
        'core': 'acore',
        'board': 'evb',
        'tool_chain': 'riscv32_musl_b090_fp',
        'build_type': 'COMPILE',
        'os': 'liteos',
        'std_libs': ['m', 'c', 'gcc'],
        'defines': ['-:TARGET_CHIP_BS25=1', 'BUILD_APPLICATION_STANDARD', 'UART_DRIVER_CONFIG_USE_VETOS_IN_STEAD_OF_TIMERS',
                    'LOSCFG_DRIVERS_EMMC', 'LOSCFG_FS_FAT_CACHE', "CONFIG_APPS_CORE", "CMD_ENABLE", "LIBAPP_VERSION",
                    "LIBBUILD_VERSION", "LIBCMD", "LIBCONNECTIVITY", "LIBCPU_LOAD", "LIBCPU_UTILS", "LIBLIB_UTILS",
                    "LIBLOG", "LIBLOG_READER", "LIBPANIC", "LIBTEST_COMMON", "LITEOS_ONETRACK", "PRE_ASIC",
                    "SUPPORT_CXX", "USE_CMSIS_OS", "USE_LITEOS", "VERSION_STANDARD", "_ALL_SOURCE", "__LITEOS__",
                    "CONFIG_NV_SUPPORT_SINGLE_CORE_SYSTEM", 'CONFIG_NV_FEATURE_SUPPORT',
                    'TEST_SUITE'],
        'defines_set': ['libsec_defines', 'chip_defines', 'version_defines'],
        'ram_component': ['bs25_std_lds', 'chip_bs25', 'bs25_app_standard', 'driver_header', '-:sec_port', 'pm_lpc', 'app_info',
                          'bs25_mem_config', 'libboundscheck', 'bs25_clocks', 'bs25_pmu', 'nv', 'nv_bs25'],
        'ram_component_set': ['pmp_set', 'std_common_lib', 'time_set', 'mem', 'watchdog', 'sec', 'security', 'pm_pmu_set', 'pm_clock_set', 'adc'],
        'bin_name': 'application',
        'application': 'standard',
        'ccflags': [
            '-:-mabi=ilp32', '-:-march=rv32imc', '-mabi=ilp32f', '-march=rv32imfc', '-madjust-regorder', '-madjust-const-cost', '-freorder-commu-args', '-fimm-compare-expand',
            '-frmv-str-zero', '-mfp-const-opt', '-frtl-sequence-abstract', "-:-Wno-type-limits",
            '-frtl-hoist-sink', '-fsafe-alias-multipointer', '-finline-optimize-size', '--short-enums',
            '-fmuliadd-expand', '-mlli-expand', '-Wa,-mcjal-expand', '-foptimize-reg-alloc', '-fsplit-multi-zero-assignments',
            '-floop-optimize-size',  '-mpattern-abstract', '-foptimize-pro-and-epilogue'],
        'linkflags': [
            '-Wl,--cjal-relax',
            '-Wl,--dslf',
        ],
        'arch': 'riscv31',
        'hso_enable': True,
        'gen_parse_tool': False,
        'gen_sec_bin': True,
        'image_version': 'bs25_hs',
        'sector_cfg': 'standard-bs25',
        'ssb_cfg': 'ssb-bs25-evb-debug',
    },
}
