#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(FLASH_LIB_PATH_16K ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/16k_lib/libl2hc_dec_flash_16k.a)
set(ITCM_LIB_PATH_16K ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/16k_lib/libl2hc_dec_itcm_16k.a)
set(FLASH_LIB_PATH_48K ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/48k_lib/libl2hc_dec_flash_48k.a)
set(ITCM_LIB_PATH_48K ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/48k_lib/libl2hc_dec_itcm_48k.a)

# l2hc_dec_flash_16k
set(COMPONENT_NAME "l2hc_dec_flash_16k")

set(SOURCES
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(LIBS ${FLASH_LIB_PATH_16K})

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
    false
)

set(MAIN_COMPONENT
    false
)

set(DEFINES)

set(CCFLAGS)

set(INTERIM_BIN_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec/)

file(COPY ${FLASH_LIB_PATH_16K} DESTINATION ${INTERIM_BIN_PATH} FOLLOW_SYMLINK_CHAIN)

build_component()

# l2hc_dec_itcm_16k
set(COMPONENT_NAME "l2hc_dec_itcm_16k")

set(SOURCES
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(LIBS ${ITCM_LIB_PATH_16K})

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
    false
)

set(MAIN_COMPONENT
    false
)

set(DEFINES)

set(CCFLAGS)

set(INTERIM_BIN_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec/)

file(COPY ${ITCM_LIB_PATH_16K} DESTINATION ${INTERIM_BIN_PATH} FOLLOW_SYMLINK_CHAIN)

build_component()

# l2hc_dec_flash_16k
set(COMPONENT_NAME "l2hc_dec_flash_48k")

set(SOURCES
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(LIBS ${FLASH_LIB_PATH_48K})

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
    false
)

set(MAIN_COMPONENT
    false
)

set(DEFINES)

set(CCFLAGS)

set(INTERIM_BIN_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec/)

file(COPY ${FLASH_LIB_PATH_48K} DESTINATION ${INTERIM_BIN_PATH} FOLLOW_SYMLINK_CHAIN)

build_component()

# l2hc_dec_itcm_48k
set(COMPONENT_NAME "l2hc_dec_itcm_48k")

set(SOURCES
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(LIBS ${ITCM_LIB_PATH_48K})

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
    false
)

set(MAIN_COMPONENT
    false
)

set(DEFINES)

set(CCFLAGS)

set(INTERIM_BIN_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec/)

file(COPY ${ITCM_LIB_PATH_48K} DESTINATION ${INTERIM_BIN_PATH} FOLLOW_SYMLINK_CHAIN)

build_component()
