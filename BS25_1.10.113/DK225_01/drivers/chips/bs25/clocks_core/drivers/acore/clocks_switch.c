/*
 * Copyright (c) @CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: SYSTEM CLOCKS SWITCH DRIVER
 * Author: @CompanyNameTag
 * Create: 2020-7-28
 */
#include "clocks_switch.h"
#include "hal_cpu_clocks.h"
#include "clocks_core.h"
#include "clocks_config.h"
#include "panic.h"
#include "hal_xip.h"
#include "osal_interrupt.h"
#include "securec.h"
#ifdef BUILD_APPLICATION_STANDARD
#include "los_hwi.h"
#endif
#include "ipc.h"

bool g_pll_used_vote[CLOCKS_PLL_SRC_MAX][CLOCKS_CCRG_MODULE_MAX] = { {false} };
uint8_t g_pll_used_vote_count[CLOCKS_PLL_SRC_MAX] = { 0 };
clocks_clk_cfg_t g_current_clocks[CLOCKS_CCRG_MODULE_MAX] = { 0 };
clock_switch_proc_func g_clock_sw_pre_handler[CLOCKS_CCRG_MODULE_MAX];
static bcpu_clk_src_t g_bt_current_clk = BCPU_CLK_SRC_TCXO;

clocks_clk_cfg_t *clocks_get_current_clock_cfg(void)
{
    return g_current_clocks;
}

void clocks_pll_init_vote(void)
{
    (void)memcpy_s((uint8_t *)g_current_clocks, sizeof(g_current_clocks),
                   (void *)SHARED_MEM_START, sizeof(g_current_clocks));
    uint32_t irq_sts = osal_irq_lock();

    for (uint32_t i = 0;i < CLOCKS_CCRG_MODULE_MAX; i++) {
        clocks_pll_src_t pll_src = clocks_pll_veto_type_translation(g_current_clocks[i].clk_src);
        if (pll_src != CLOCKS_PLL_SRC_MAX) {
            g_pll_used_vote[pll_src][i] = true;
            g_pll_used_vote_count[pll_src]++;
        }

        pmu_cmu_core_t pll_used = clocks_pll_used_init_type_translation(g_current_clocks[i].clk_src);
        if (pll_used != PMU_CMU_MAX) {
            pmu_cmu_init_pll_status(pll_used, true);
        }
    }
    osal_irq_restore(irq_sts);
}

void clocks_pll_add_vote(clocks_pll_src_t pll_src, clocks_ccrg_module_t module)
{
    if (pll_src >= CLOCKS_PLL_SRC_MAX) {
        return;
    }
    if (!g_pll_used_vote[pll_src][module]) {
        g_pll_used_vote[pll_src][module] = true;
        if (g_pll_used_vote_count[pll_src] == 0) {
            clocks_pll_power_on(pll_src);
        }
        g_pll_used_vote_count[pll_src]++;
    }
}

void clocks_pll_remove_vote(clocks_pll_src_t pll_src, clocks_ccrg_module_t module)
{
    if (pll_src >= CLOCKS_PLL_SRC_MAX) {
        return;
    }
    if (g_pll_used_vote[pll_src][module]) {
        g_pll_used_vote[pll_src][module] = false;
        g_pll_used_vote_count[pll_src]--;
        if (g_pll_used_vote_count[pll_src] == 0) {
            clocks_pll_power_off(pll_src);
        }
    }
}

void clock_switch_pre_handler_register(clock_switch_proc_func func, clocks_ccrg_module_t module)
{
    g_clock_sw_pre_handler[module] = func;
}

static void system_ccrg_clock_config_pre_handler(clocks_ccrg_module_t module)
{
    if (g_clock_sw_pre_handler[module] != NULL) {
        g_clock_sw_pre_handler[module]();
    }
}

void system_ccrg_clock_config(clocks_ccrg_module_t module, clocks_clk_src_t clk_src, uint8_t clk_div)
{
    if ((g_current_clocks[module].clk_src == clk_src && g_current_clocks[module].clk_div == clk_div)) {
        return;
    }

    system_ccrg_clock_config_pre_handler(module);
    hal_clocks_ccrg_mcu_group_clk_src_t src_sel;

    // When clk_src equals CLOCKS_CLK_SRC_MAX, the clock will be disable the clock.
    if (clk_src == CLOCKS_CLK_SRC_MAX) {
        src_sel = HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX;
    } else {
        src_sel = clocks_src_sel_type_translation(module, clk_src);
        // Invalid clock source.
        if (src_sel == HAL_CLOCKS_CCRG_MCU_GROUP_CLK_SRC_MAX) {
            panic(PANIC_CLOCKS_SWITCH, __LINE__);
        }
    }

    uint32_t irq_sts = osal_irq_lock();
    if (g_current_clocks[module].clk_src != clk_src) {
        // Add vote.
        clocks_pll_src_t pll_src = clocks_pll_veto_type_translation(clk_src);
        clocks_pll_add_vote(pll_src, module);
        // Config the clock.
        hal_mcu_group_clock_switch((hal_clocks_ccrg_module_t)module, src_sel, clk_div);
        // Remove vote.
        pll_src = clocks_pll_veto_type_translation(g_current_clocks[module].clk_src);
        clocks_pll_remove_vote(pll_src, module);
        g_current_clocks[module].clk_src = clk_src;
    } else {
        // Config the clock.
        hal_mcu_group_clock_switch((hal_clocks_ccrg_module_t)module, src_sel, clk_div);
    }
    g_current_clocks[module].clk_div = clk_div;
    osal_irq_restore(irq_sts);
}

uint32_t clocks_get_module_frequency(clocks_ccrg_module_t module)
{
    clocks_clk_src_t clk_src = g_current_clocks[module].clk_src;
    uint8_t clk_div = g_current_clocks[module].clk_div;
    uint32_t clk_freq = 0;
    switch (clk_src) {
        case CLOCKS_CLK_SRC_RC:
            clk_freq = RC_CLK;
            break;
        case CLOCKS_CLK_SRC_32K:
            clk_freq = SYSTICK_CLK;
            break;
        case CLOCKS_CLK_SRC_TCXO_2X:
            clk_freq = TCXO_2X_CLK;
            break;
        case CLOCKS_CLK_SRC_TCXO:
            clk_freq = TCXO_1X_CLK;
            break;
        case CLOCKS_CLK_SRC_PAD_CLKIN:
        case CLOCKS_CLK_SRC_MAX:
            break;
        default:
            clk_freq = clocks_get_pll_frequency(clk_src);
            break;
    }
    if (clk_div > CLOCK_DIV_1) {
        clk_freq = clk_freq / clk_div;
    }
    return clk_freq;
}

static void master_inform_slave_adjust_freq(cores_t core, uint32_t freq)
{
    ipc_payload_adjust_slave_freq_t ipc_freq;
    ipc_freq.slave_freq = freq;
    (void)ipc_send_message(core, IPC_ACTION_SLAVE_ADJUST_FREQ, (ipc_payload *)&ipc_freq,
                           sizeof(ipc_payload_adjust_slave_freq_t), IPC_PRIORITY_HIGHEST, false);
}

void clocks_set_bt_freq(bcpu_clk_src_t bcpu_clk)
{
    if (!hal_cpu_is_bt_enabled() || bcpu_clk == g_bt_current_clk) {
        return;
    }
    master_inform_slave_adjust_freq(CORES_BT_CORE, (uint32_t)bcpu_clk);
}