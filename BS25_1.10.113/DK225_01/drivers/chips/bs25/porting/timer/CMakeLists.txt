#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "timer_port")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/timer_porting.c
)

set(PUBLIC_HEADER
)

set(PUBLIC_DEFINES
TEST_TIMER_INDEX=1
)

set(WHOLE_LINK
    true
)

set(MAIN_COMPONENT
    false
)

build_component()
