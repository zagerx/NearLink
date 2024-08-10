#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
#===============================================================================
add_subdirectory_if_exist(sle_uuid_server)
add_subdirectory_if_exist(sle_uuid_client)
add_subdirectory_if_exist(sle_mouse_server)

set(SOURCES "${SOURCES}" PARENT_SCOPE)
set(PUBLIC_HEADER "${PUBLIC_HEADER}" PARENT_SCOPE)