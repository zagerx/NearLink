#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
file(GLOB OSAL_SOURCES_INC "${CMAKE_CURRENT_SOURCE_DIR}/../../../framework/osal/liteos/*.c")
set(SOURCES1_HALF_FLASH
    ${CMAKE_CURRENT_SOURCE_DIR}/xasbc_dec_comp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/plc/msbc_plc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_bitalloc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_bitstream_decode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_decoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_dequant.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_framing.c
)

set(SOURCES2_HALF_ITCM
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_synthesis_dct8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_synthesis_generated8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_synthesis.c
)

set(SOURCES1_ALL_FLASH
    ${CMAKE_CURRENT_SOURCE_DIR}/xasbc_dec_comp.c
)

set(SOURCES2_ALL_ITCM
    ${CMAKE_CURRENT_SOURCE_DIR}/plc/msbc_plc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_bitalloc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_bitstream_decode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_decoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_dequant.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_framing.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_synthesis_dct8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_synthesis_generated8.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sbc_synthesis.c
)

#component sbc_dec_itcm

set(COMPONENT_NAME "sbc_dec_half_itcm")

set(SOURCES
    ${SOURCES2_HALF_ITCM}
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/plc
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

set(CCFLAGS -O2 -DLINX_CODEC -DAUDIO_OPT -DLINX_SBC_DEC)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

#component sbc_dec_half_flash

set(COMPONENT_NAME "sbc_dec_half_flash")

set(SOURCES
    ${OSAL_SOURCES_INC}
    ${SOURCES1_HALF_FLASH}
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/plc
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

set(CCFLAGS1 -O2 -DLINX_CODEC -DAUDIO_OPT -DLINX_SBC_DEC)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

#component sbc_dec_all_itcm

set(COMPONENT_NAME "sbc_dec_all_itcm")

set(SOURCES
    ${SOURCES2_ALL_ITCM}
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/plc
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

set(CCFLAGS -O2 -DLINX_CODEC -DAUDIO_OPT -DLINX_SBC_DEC)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

#component sbc_dec_all_flash

set(COMPONENT_NAME "sbc_dec_all_flash")

set(SOURCES
    ${OSAL_SOURCES_INC}
    ${SOURCES1_ALL_FLASH}
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/plc
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

set(CCFLAGS1 -O2 -DLINX_CODEC -DAUDIO_OPT -DLINX_SBC_DEC)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()
