if(DEFINED CONFIG_SAMPLE_DUAL_MODE_SLE_CLIENT)
set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/sle/sle_client.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle/sle_callbacks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble/ble_callbacks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/application_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/application_task_ble.c
    ${CMAKE_CURRENT_SOURCE_DIR}/application_task_sle_client.c
)
endif()

if(DEFINED CONFIG_SAMPLE_DUAL_MODE_SLE_SERVER)
set(SOURCES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/sle/sle_server.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sle/sle_callbacks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ble/ble_callbacks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/application_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/application_task_ble.c
    ${CMAKE_CURRENT_SOURCE_DIR}/application_task_sle_server.c
)
endif()

set(SOURCES "${SOURCES}" ${SOURCES_LIST} PARENT_SCOPE)
