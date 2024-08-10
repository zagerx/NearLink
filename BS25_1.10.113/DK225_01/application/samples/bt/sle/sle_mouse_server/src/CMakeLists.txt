#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
#===============================================================================
set(SOURCES "${SOURCES}"
    "${CMAKE_CURRENT_SOURCE_DIR}/sle_mouse_hid_server.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/sle_mouse_adv.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/sle_mouse_dis_server.c"
    PARENT_SCOPE)
