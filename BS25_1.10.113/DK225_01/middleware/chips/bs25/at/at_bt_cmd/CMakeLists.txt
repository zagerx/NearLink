#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================


set(COMPONENT_NAME "bs_at_port")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/at/at_bt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/at_bt_cmd_register.c
    ${CMAKE_CURRENT_SOURCE_DIR}/at/at_btc_product.c
    ${CMAKE_CURRENT_SOURCE_DIR}/at/at_table.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/at
)

set(PRIVATE_HEADER
    ${ROOT_DIR}/middleware/utils/at/at
    ${ROOT_DIR}/middleware/utils/at/at/include
    ${ROOT_DIR}/middleware/utils/at/at/src
)

set(PRIVATE_DEFINES
    LOSCFG_BASE_CORE_USE_MULTI_LIST
)

set(PUBLIC_DEFINES
    AT_COMMAND
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
    true
)

build_component()