#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
#===============================================================================

add_subdirectory_if_exist(ble)
add_subdirectory_if_exist(sle)
set(SOURCES "${SOURCES}" PARENT_SCOPE)
set(PUBLIC_HEADER "${PUBLIC_HEADER}" PARENT_SCOPE)