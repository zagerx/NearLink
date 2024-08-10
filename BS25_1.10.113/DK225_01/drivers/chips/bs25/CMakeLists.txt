#===============================================================================
# @brief    cmake file
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
#===============================================================================
add_subdirectory(porting)

set(COMPONENT_NAME "chip_bs25")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv31/interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv31/interrupt_handler.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv31/mpu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv31/vectors.s
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/${CORE}/interrupt_adapter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/${CORE}/vectors.c
)

if(DEFINED OS AND NOT ${OS} STREQUAL "non-os")
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv31/oam_trace.c)
    list(APPEND SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/${CORE}/interrupt_os_adapter.c)
endif()

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CORE}/
    ${CMAKE_CURRENT_SOURCE_DIR}/config/otp
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/
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

set(COMPONENT_NAME "bs25_clocks")

set(SOURCES
${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers/clocks.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks_glb.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/shared/clock_calibration.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/shared/clocks_core_common.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/hal/hal_32k_clock.c
${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks.c

)

if(${CORE} STREQUAL "acore")
    set(SOURCES
        ${SOURCES}
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/${CORE}/hal_clocks_application.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/${CORE}/hal_clocks_application_private.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/${CORE}/hal_cpu_clocks.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers/clocks_config.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/acore/clocks_core.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/acore/clocks_switch_private.c
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/acore/clocks_switch.c
    )
endif()

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/${CORE}
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/${CORE}
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/hal
)

if(${CORE} STREQUAL "acore")
    set(PUBLIC_HEADER
        ${PUBLIC_HEADER}
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/acore
        ${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers

    )
endif()

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CLOCKS
    SUPPORT_CLOCKS_CORE
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

set(COMPONENT_NAME "bs25_pmu")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu_bt_sub.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu_cmu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu_ldo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu_buck_ldo.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CLOCKS
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

set(COMPONENT_NAME "bs25_memory")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/hal_memory_apps.c
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/hal_memory_security.c
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/hal_memory.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/
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


set(COMPONENT_NAME "rom_callback")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/rom_config/${CORE}/output/rom_callback.S
    )

set(PUBLIC_HEADER
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

if("rom_callback" IN_LIST TARGET_COMPONENT)
    include(${CMAKE_CURRENT_SOURCE_DIR}/rom_config/${CORE}/output/rom_callback_wrap.cmake)
    target_link_options(${TARGET_NAME}
        PRIVATE
        ${CMAKE_CB_FLGAS}
    )
endif()
install_sdk("${ROOT_DIR}/interim_binary/bs25/bin" "*.bin")

set(COMPONENT_NAME "bs25_core_inc")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv31/interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv31/interrupt_handler.c
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/riscv/riscv31/vectors.s
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/${CORE}/interrupt_adapter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/${CORE}/vectors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu_interrupt.c
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu/pmu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks_glb.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/shared/clocks_core_common.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/hal/hal_32k_clock.c
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/hal_clocks.c
)

set(PUBLIC_HEADER
    ${CMAKE_CURRENT_SOURCE_DIR}/arch/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/${CORE}/
    ${CMAKE_CURRENT_SOURCE_DIR}/config/otp
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt/
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/drivers/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks/hal/${CORE}
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/${CORE}
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/drivers/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/clocks_core/hal
    ${CMAKE_CURRENT_SOURCE_DIR}/pmu
)

set(PRIVATE_HEADER
)

set(PRIVATE_DEFINES
)

set(PUBLIC_DEFINES
    SUPPORT_CLOCKS
    SUPPORT_CLOCKS_CORE
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

add_subdirectory_if_exist(pm_lpc)
add_subdirectory_if_exist(startup)

install_sdk("${ROOT_DIR}/interim_binary/${CHIP}/bin/boot_bin/ssb-bs25-evb-user/" "*.bin")
install_sdk("${ROOT_DIR}/interim_binary/${CHIP}/bin/boot_bin/ssb-bs25-evb-debug/" "*.bin")