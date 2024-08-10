#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
set(LIB_PATH_16K ${ROOT_DIR}/drivers/drivers/driver/audio/riscv_codec/codec/ha_codec/l2hc_enc/${CHIP}/16k_lib/libl2hc_enc_16k.a)

set(LIB_PATH_48K ${ROOT_DIR}/drivers/drivers/driver/audio/riscv_codec/codec/ha_codec/l2hc_enc/${CHIP}/48k_lib/libl2hc_enc_48k.a)

#component l2hc_enc_16k

set(COMPONENT_NAME "l2hc_enc_16k")

set(SOURCES
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(LIBS ${LIB_PATH_16K})

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

file(COPY ${LIB_PATH_16K} DESTINATION ${INTERIM_BIN_PATH} FOLLOW_SYMLINK_CHAIN)

build_component()

#component l2hc_enc_48k

set(COMPONENT_NAME "l2hc_enc_48k")

set(SOURCES
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
)

set(LIBS ${LIB_PATH_48K})

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

file(COPY ${LIB_PATH_48K} DESTINATION ${INTERIM_BIN_PATH} FOLLOW_SYMLINK_CHAIN)

build_component()
