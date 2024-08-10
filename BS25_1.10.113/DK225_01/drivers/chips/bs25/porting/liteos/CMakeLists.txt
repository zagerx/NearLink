#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "liteos_port")

if(${OS} MATCHES "liteos")
    set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/idle_config.c
        ${CMAKE_CURRENT_SOURCE_DIR}/tick_timer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/os_dfx.c
        ${CMAKE_CURRENT_SOURCE_DIR}/memory_info.c
    )
    set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/
    )
endif()

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
