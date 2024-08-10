#!/usr/bin/env python3
# encoding=utf-8
# ============================================================================
# @brief    Target Definitions File
# Copyright CompanyNameMagicTag 2022-2022. All rights reserved.
# ============================================================================
closed_componnet = [
    'bg_common', 'bt_app', 'gle_app', 'bth_sdk', 'ai_toothbrush', 'usb_class', 'usb_unified', 'm', 'c',
    'l2hc_dec_flash_16k', 'l2hc_dec_itcm_16k', 'l2hc_dec_flash_48k', 'l2hc_dec_itcm_48k',
    'opus_dec_half_itcm', 'opus_dec_half_flash', 'opus_dec_all_itcm', 'opus_dec_all_flash',
    'opus_enc_half_itcm', 'opus_enc_half_flash', 'opus_enc_all_itcm', 'opus_enc_all_flash',
    'opus_enc_2.5ms_itcm', 'opus_enc_2.5ms_flash', 'l2hc_enc_16k', 'l2hc_enc_48k',
    'sbc_dec_half_itcm', 'sbc_dec_half_flash', 'sbc_dec_all_itcm', 'sbc_dec_all_flash',
    'sbc_enc_half_itcm', 'sbc_enc_half_flash', 'sbc_enc_all_itcm', 'sbc_enc_all_flash'
]

target = {
    'standard-bs25-app-evb': {
        'base_target_name': 'target_standard_bs25_application_evb_template',
        'CONFIG_WATCHDOG_USING_V100': 'y',
        'defines': ["-:TARGET_CHIP_BS25=1", "-:BS25_CHIP_V100=1", 'PRE_ASIC', 'BS25_PRODUCT_EVB', 'VERSION_STANDARD', 'USE_CMSIS_OS', "LOSCFG_MEM_TASK_STAT",
                    'LOSCFG_DRIVERS_EMMC', 'LOSCFG_FS_FAT_CACHE', 'USE_VECTORS', 'CMD_ENABLE', 'HSO_SUPPORT', 'LOSCFG_KERNEL_PRINTF', '__LITEOS__', 'DUMP_MEM_SUPPORT',
                    "CONFIG_UART_FIFO_DEPTH=64", 'LITEOS_208', 'LIBUTIL_COMPAT', 'SUPPORT_DIAG_V2_PROTOCOL', 'USE_EMBED_FLASH', 'AT_COMMAND', 'SUPPORT_NV_UPGRADE','SW_UART_DEBUG'],
        'ram_component': [
                          # Application & Testsuite components
                          'bt_app', 'gle_app', 'bth_sdk', 'test_ble_hid',
                          'samples',
                          'test_lcd_tft', 'test_i2c_led', 'test_i2c', 'test_adc',
                          'test_pown_key', 'test_spi', 'test_pwm', 'test_flash', 'test_rtc_unified',
                          'test_i2s', 'test_keyscan', 'test_qdec', 'test_eflash', 'test_cmsis', 'test_calendar',
                          'test_usb_unified', 'bs_at_port',
                          # Middleware components
                          'app_init', 'nv', 'dfx_exception', 'at', 'bs_at_port', 'dfx_file_operation',
                          'partition', 'partition_bs25', 'ota_upgrade_bs25', 'ota_port_bs25',
                          'usb_class', 'usb_class_open', 'usb_class_header',
                          'update_common', 'update_local', 'update_storage',  'lzma_21.07',
                          'update_common_bs25', 'update_storage_bs25', 'codec', 'tiot_driver',
                          # Drivers components
                          'ipc_old', 'hal_cpu_hifi', 'bs25_memory', 'porting_inc', 'ulp_aon', 'hal_sec_rsa',
                          'hal_mips',
                          # Protocol components
                          'bg_common', 'bts_header',
                          # Others components
                          'error_code', 'cmn_header', 'board_config', "dfx_port_bs25", "algorithm", 'liteos_208_6_0_b017',
                          '-:lib_utils',  'osal', 'liteos_port', 'non_os', 'lpm', 'libboundscheck', '-:hal_cpu_core',
                          'segger_b090_fp',
                          ],
        'ram_component_set' : ['codec_set', 'otp', 'usb_unified', 'lpc', 'cpu', 'pdm', 'cpu_trace', 'xip_os', 'flash', 'gpio_v100', 'ipc', 'spi', 'qspi', 'uart', 'i2c', 'dma', 'mem_monitor',
                               'sio_v150', 'qdec', 'keyscan', 'pinctrl', 'i2s', 'dfx_set', 'embed_flash', 'pwm', 'rtc_unified', 'pm_set', 'calendar'],
        'packet': True,
        'nv_update': True,
        'ssb_cfg': 'ssb-bs25-evb-debug',
    },
    "bs25-pm": {
        'base_target_name': 'standard-bs25-app-evb',
        'defines': [],
        'ram_component':['app_pm', 'pm_sys', '-:samples', '-:tiot_driver'],
        'packet': True,
        'nv_update': True
    },
}

# custom copy rules, put it in target_group below and it takes effect.
# <root> means root path
# <out_root> means output_root path
# <pack_target> means target_group key_name
target_copy = {

}

target_group = {

}
