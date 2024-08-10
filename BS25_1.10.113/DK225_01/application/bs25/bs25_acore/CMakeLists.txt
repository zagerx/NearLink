#===============================================================================
# @brief    cmake make file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "bs25_app_standard")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/startup.S
    ${CMAKE_CURRENT_SOURCE_DIR}/app_os_init.c
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/system_init.c)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/system_init.c)
endif()

set(PRIVATE_HEADER
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/version
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

set(COMPONENT_CCFLAGS
)

set(WHOLE_LINK
true
)

set(BUILD_AS_OBJ
false
)

set(MAIN_COMPONENT
true
)
build_component()

set(COMPONENT_NAME "ota_upgrade_bs25")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ota/ota_upgrade.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ota/ota_upgrade_handle.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/ota
)

set(PRIVATE_HEADER

)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
)

# use this when you want to add ccflags like -include xxx
set(COMPONENT_PUBLIC_CCFLAGS
)

set(COMPONENT_CCFLAGS
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()