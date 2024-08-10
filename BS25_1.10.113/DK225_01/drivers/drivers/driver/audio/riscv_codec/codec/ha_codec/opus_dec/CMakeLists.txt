#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
file(GLOB OSAL_SOURCES_INC "${CMAKE_CURRENT_SOURCE_DIR}/../../../framework/osal/liteos/*.c")
set(SOURCES1_HALF_FLASH
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/pitch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/celt_lpc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/celt_decoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/src/opus_decoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/xaopus_dec_comp.c
)

set(SOURCES2_HALF_ITCM
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/celt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/cwrs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/entcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/entdec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/modes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/bands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/kiss_fft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/laplace.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/mathops.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/mdct.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/quant_bands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/rate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/vq.c
)

set(SOURCES1_ALL_FLASH
    ${CMAKE_CURRENT_SOURCE_DIR}/xaopus_dec_comp.c
)

set(SOURCES2_ALL_ITCM
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/pitch.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/celt_lpc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/celt_decoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/src/opus_decoder.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/celt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/cwrs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/entcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/entdec.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/modes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/bands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/kiss_fft.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/laplace.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/mathops.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/mdct.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/quant_bands.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/rate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt/vq.c
)

#component opus_dec_half_itcm

set(COMPONENT_NAME "opus_dec_half_itcm")

set(SOURCES
    ${SOURCES2_HALF_ITCM}
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/src
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

set(CCFLAGS -O2 -DLINX_CODEC -DDECODE_CONFIG -DLINX_OPUS_DEC -mabi=ilp32f -march=rv32imfc
    -femit-muliadd -fimm-compare -fmerge-immshf -fldm-stm-optimize -ffunction-sections
    -fdata-sections -fvisibility=hidden)


set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

#component opus_dec_half_flash

set(COMPONENT_NAME "opus_dec_half_flash")

set(SOURCES
    ${OSAL_SOURCES_INC}
    ${SOURCES1_HALF_FLASH}
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/src
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

set(CCFLAGS -O2 -DLINX_CODEC -DDECODE_CONFIG -DLINX_OPUS_DEC -mabi=ilp32f -march=rv32imfc
    -femit-muliadd -fimm-compare -fmerge-immshf -fldm-stm-optimize -ffunction-sections
    -fdata-sections -fvisibility=hidden)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

#component opus_dec_all_itcm

set(COMPONENT_NAME "opus_dec_all_itcm")

set(SOURCES
    ${SOURCES2_ALL_ITCM}
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/src
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

set(CCFLAGS -O2 -DLINX_CODEC -DDECODE_CONFIG -DLINX_OPUS_DEC -mabi=ilp32f -march=rv32imfc
    -femit-muliadd -fimm-compare -fmerge-immshf -fldm-stm-optimize -ffunction-sections
    -fdata-sections -fvisibility=hidden)


set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

#component opus_dec_all_flash

set(COMPONENT_NAME "opus_dec_all_flash")

set(SOURCES
    ${OSAL_SOURCES_INC}
    ${SOURCES1_ALL_FLASH}
)

set(PUBLIC_HEADER
)

set(PRIVATE_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/celt
    ${CMAKE_CURRENT_SOURCE_DIR}/libopus/src
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

set(CCFLAGS -O2 -DLINX_CODEC -DDECODE_CONFIG -DLINX_OPUS_DEC -mabi=ilp32f -march=rv32imfc
    -femit-muliadd -fimm-compare -fmerge-immshf -fldm-stm-optimize -ffunction-sections
    -fdata-sections -fvisibility=hidden)

set(LIB_OUT_PATH ${ROOT_DIR}/interim_binary/${CHIP}/libs/audio/codec)

build_component()

