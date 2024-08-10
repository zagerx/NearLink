#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
file(GLOB OSAL_SOURCES_INC "${CMAKE_CURRENT_SOURCE_DIR}/../../../framework/osal/liteos/*.c")
set(SOURCES1_HALF_FLASH
    ${CMAKE_CURRENT_SOURCE_DIR}/xasbc_enc_comp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_enc_bit_alloc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_encoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_packing.c
)

set(SOURCES2_HALF_ITCM
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_dct.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_analysis.c
)

set(SOURCES1_ALL_FLASH
    ${CMAKE_CURRENT_SOURCE_DIR}/xasbc_enc_comp.c
)

set(SOURCES2_ALL_ITCM
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_enc_bit_alloc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_encoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_packing.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_dct.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_analysis.c
)

# component sbc_enc_half_itcm

set(COMPONENT_NAME "sbc_enc_half_itcm")

set(SOURCES
    ${SOURCES2_HALF_ITCM}
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../framework/osal/liteos
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
    false
)

set(MAIN_COMPONENT
    false
)

set(DEFINES)

set(CCFLAGS -O2 -DLINX_CODEC -DAUDIO_OPT -DLINX_SBC_ENC)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

#component sbc_enc_half_flash

set(COMPONENT_NAME "sbc_enc_half_flash")

set(SOURCES
    ${OSAL_SOURCES_INC}
    ${SOURCES1_HALF_FLASH}
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../framework/osal/liteos
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
    false
)

set(MAIN_COMPONENT
    false
)

set(DEFINES)

set(CCFLAGS -O2 -DLINX_CODEC -DAUDIO_OPT -DLINX_SBC_ENC)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

# component sbc_enc_all_itcm

set(COMPONENT_NAME "sbc_enc_all_itcm")

set(SOURCES
    ${SOURCES2_ALL_ITCM}
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../framework/osal/liteos
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
    false
)

set(MAIN_COMPONENT
    false
)

set(DEFINES)

set(CCFLAGS -O2 -DLINX_CODEC -DAUDIO_OPT -DLINX_SBC_ENC)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

#component sbc_enc_all_flash

set(COMPONENT_NAME "sbc_enc_all_flash")

set(SOURCES
    ${OSAL_SOURCES_INC}
    ${SOURCES1_ALL_FLASH}
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../framework/osal/liteos
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
    false
)

set(MAIN_COMPONENT
    false
)

set(DEFINES)

set(CCFLAGS -O2 -DLINX_CODEC -DAUDIO_OPT -DLINX_SBC_ENC)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()
