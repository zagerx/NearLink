#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2023-2023. All rights reserved.
#===============================================================================
if(DEFINED CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_DONGLE)
set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt_dongle/sle_amic_vdt_usb/sle_vdt_usb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt_dongle/sle_amic_vdt_client/sle_vdt_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt_codec/sle_vdt_codec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt_dongle.c
)
elseif(DEFINED CONFIG_SAMPLE_SUPPORT_SLE_AMIC_VDT_SERVER)
set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt/sle_amic_vdt_pdm/sle_vdt_pdm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt/sle_amic_vdt_server/sle_vdt_server_adv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt/sle_amic_vdt_server/sle_vdt_server.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt_codec/sle_vdt_codec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle_amic_vdt_dongle.c
)
endif()
set(SOURCES "${SOURCES}" ${SOURCES_LIST} PARENT_SCOPE)