#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
add_subdirectory_if_exist(bs25_${CORE})
if(${CORE} STREQUAL "bt_core")
    add_subdirectory(bs25_bgle)
endif()
if(${APPLICATION} STREQUAL "ate" AND ${CORE} STREQUAL "acore")
    add_subdirectory(bs25_ate)
endif()
add_subdirectory_if_exist(mouse_iton)
add_subdirectory_if_exist(dongle_iton)
add_subdirectory_if_exist(bs25_hadm)
add_subdirectory_if_exist(bs25_pm)