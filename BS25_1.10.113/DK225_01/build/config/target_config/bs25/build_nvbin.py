#!/usr/bin/env python3
# encoding=utf-8
# ============================================================================
# @brief    Build nvbin
# Copyright CompanyNameMagicTag 2023-2023. All rights reserved.
# ============================================================================

import os
import sys

top_dir = os.path.abspath(os.path.join(os.getcwd(),"."))
sys.path.append(os.path.join(top_dir, 'build', 'script', 'nv'))

from nv_binary import nv_begin

if __name__ == '__main__':
    #配置文件路径
    nv_config_json = os.path.join(top_dir, "build", "config", "target_config", "bs25", "nv_bin_cfg", "mk_nv_bin_cfg.json")
    #输出路径
    nv_output_path = os.path.join(top_dir, "interim_binary", "bs25", "bin")
    if not os.path.exists(nv_output_path):
        os.makedirs(nv_output_path)

    targets = ["acore"]
    nv_begin(nv_config_json, targets, 0)
