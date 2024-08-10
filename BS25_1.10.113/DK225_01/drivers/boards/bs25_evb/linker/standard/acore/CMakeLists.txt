#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "bs25_std_lds")

if(NOT ${COMPONENT_NAME} IN_LIST TARGET_COMPONENT)
    return()
endif()

set(LINK_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.prelds" CACHE INTERNAL "" FORCE)
build_component()
