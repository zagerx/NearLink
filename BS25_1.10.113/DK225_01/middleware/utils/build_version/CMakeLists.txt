#===============================================================================
# @brief    cmake file
# Copyright (c) CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(COMPONENT_NAME "build_version")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/private/build_version.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/public
)

set(PRIVATE_HEADER
)

execute_process(
    COMMAND date +%s
    OUTPUT_VARIABLE TIMESTAMP_STRING
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(BUILD_TIMESTAMP ${TIMESTAMP_STRING})

set(PRIVATE_DEFINES
    -DBUILD_TIMESTAMP=${BUILD_TIMESTAMP}
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
