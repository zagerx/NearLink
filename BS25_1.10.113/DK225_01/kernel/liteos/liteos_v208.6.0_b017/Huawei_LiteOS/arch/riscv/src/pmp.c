/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description : LiteOS pmp module implementation.
 * Author: Huawei LiteOS Team
 * Create : 2022-12-20
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------- */
#include "arch/pmp.h"
#include "los_hwi.h"
STATIC INLINE UINT32 Ctz32(UINT32 val)
{
    if (val == 0) {
        return 32; /* The number of preamble 0 of zero is 32. */
    }

    return __builtin_ctz(val);
}

typedef UINT32 (*CSR_PMPREG_CONTROL)(BOOL isWrite, UINT32 regData);

#define PMP_CSR_FUNC_GET(csrReg)        csrReg##Control
#define PMP_CSR_FUNC_CONTROL(csrReg)    STATIC UINT32 csrReg##Control(BOOL isWrite, UINT32 regData)     \
                                        {                                                               \
                                            if (isWrite == 0) {                                         \
                                                return READ_CSR(csrReg);                                \
                                            } else {                                                    \
                                                WRITE_CSR(csrReg, regData);                             \
                                                return 0;                                               \
                                            }                                                           \
                                        }

PMP_CSR_FUNC_CONTROL(PMPCFG0)
PMP_CSR_FUNC_CONTROL(PMPCFG1)
PMP_CSR_FUNC_CONTROL(PMPCFG2)
PMP_CSR_FUNC_CONTROL(PMPCFG3)
#if (LOSCFG_PMPCFG_MAX_SUPPORT == 8)
PMP_CSR_FUNC_CONTROL(PMPCFG4)
PMP_CSR_FUNC_CONTROL(PMPCFG5)
PMP_CSR_FUNC_CONTROL(PMPCFG6)
PMP_CSR_FUNC_CONTROL(PMPCFG7)
#endif

PMP_CSR_FUNC_CONTROL(PMPADDR0)
PMP_CSR_FUNC_CONTROL(PMPADDR1)
PMP_CSR_FUNC_CONTROL(PMPADDR2)
PMP_CSR_FUNC_CONTROL(PMPADDR3)
PMP_CSR_FUNC_CONTROL(PMPADDR4)
PMP_CSR_FUNC_CONTROL(PMPADDR5)
PMP_CSR_FUNC_CONTROL(PMPADDR6)
PMP_CSR_FUNC_CONTROL(PMPADDR7)
PMP_CSR_FUNC_CONTROL(PMPADDR8)
PMP_CSR_FUNC_CONTROL(PMPADDR9)
PMP_CSR_FUNC_CONTROL(PMPADDR10)
PMP_CSR_FUNC_CONTROL(PMPADDR11)
PMP_CSR_FUNC_CONTROL(PMPADDR12)
PMP_CSR_FUNC_CONTROL(PMPADDR13)
PMP_CSR_FUNC_CONTROL(PMPADDR14)
PMP_CSR_FUNC_CONTROL(PMPADDR15)
#if (LOSCFG_PMP_MAX_SUPPORT == 32)
PMP_CSR_FUNC_CONTROL(PMPADDR16)
PMP_CSR_FUNC_CONTROL(PMPADDR17)
PMP_CSR_FUNC_CONTROL(PMPADDR18)
PMP_CSR_FUNC_CONTROL(PMPADDR19)
PMP_CSR_FUNC_CONTROL(PMPADDR20)
PMP_CSR_FUNC_CONTROL(PMPADDR21)
PMP_CSR_FUNC_CONTROL(PMPADDR22)
PMP_CSR_FUNC_CONTROL(PMPADDR23)
PMP_CSR_FUNC_CONTROL(PMPADDR24)
PMP_CSR_FUNC_CONTROL(PMPADDR25)
PMP_CSR_FUNC_CONTROL(PMPADDR26)
PMP_CSR_FUNC_CONTROL(PMPADDR27)
PMP_CSR_FUNC_CONTROL(PMPADDR28)
PMP_CSR_FUNC_CONTROL(PMPADDR29)
PMP_CSR_FUNC_CONTROL(PMPADDR30)
PMP_CSR_FUNC_CONTROL(PMPADDR31)
#endif

#ifdef LOSCFG_ARCH_LINX_M
PMP_CSR_FUNC_CONTROL(MEMATTR0)
PMP_CSR_FUNC_CONTROL(MEMATTR1)
PMP_CSR_FUNC_CONTROL(MEMATTR2)
PMP_CSR_FUNC_CONTROL(MEMATTR3)
PMP_CSR_FUNC_CONTROL(MEMATTR4)
PMP_CSR_FUNC_CONTROL(MEMATTR5)
PMP_CSR_FUNC_CONTROL(MEMATTR6)
PMP_CSR_FUNC_CONTROL(MEMATTR7)
PMP_CSR_FUNC_CONTROL(MEMATTR8)
PMP_CSR_FUNC_CONTROL(MEMATTR9)
PMP_CSR_FUNC_CONTROL(MEMATTR10)
PMP_CSR_FUNC_CONTROL(MEMATTR11)
PMP_CSR_FUNC_CONTROL(MEMATTR12)
PMP_CSR_FUNC_CONTROL(MEMATTR13)
PMP_CSR_FUNC_CONTROL(MEMATTR14)
PMP_CSR_FUNC_CONTROL(MEMATTR15)
PMP_CSR_FUNC_CONTROL(MEMATTR16)
PMP_CSR_FUNC_CONTROL(MEMATTR17)
PMP_CSR_FUNC_CONTROL(MEMATTR18)
PMP_CSR_FUNC_CONTROL(MEMATTR19)
PMP_CSR_FUNC_CONTROL(MEMATTR20)
PMP_CSR_FUNC_CONTROL(MEMATTR21)
PMP_CSR_FUNC_CONTROL(MEMATTR22)
PMP_CSR_FUNC_CONTROL(MEMATTR23)
PMP_CSR_FUNC_CONTROL(MEMATTR24)
PMP_CSR_FUNC_CONTROL(MEMATTR25)
PMP_CSR_FUNC_CONTROL(MEMATTR26)
PMP_CSR_FUNC_CONTROL(MEMATTR27)
PMP_CSR_FUNC_CONTROL(MEMATTR28)
PMP_CSR_FUNC_CONTROL(MEMATTR29)
PMP_CSR_FUNC_CONTROL(MEMATTR30)
PMP_CSR_FUNC_CONTROL(MEMATTR31)
#endif

/**
 * Related instructions of registers are specified in register_config.h and they are not explained here.
 */
#ifdef LOSCFG_ARCH_LINX_M
/* the PMP region address must be at-least 32-byte aligned */
#define PMP_ADDR_ALIGN_MIN         32U
#define PMP_ADDR_ALIGN_SIZE        ((CACHE_LINE_SIZE > PMP_ADDR_ALIGN_MIN) ? CACHE_LINE_SIZE : PMP_ADDR_ALIGN_MIN)
#define PMP_TOR_MIN_SIZE           PMP_ADDR_ALIGN_SIZE
#define PMP_NAPOT_MIN_SIZE         PMP_ADDR_ALIGN_SIZE

/*
 * The mem attr of the PMP region is device non-bufferable,
 * system MMU is disable and specify the PMP region is secure.
 */
#define MEM_ATTR_DEFAULT_VALUE     0
#else
#define PMP_ADDR_ALIGN_SIZE        4
#define PMP_TOR_MIN_SIZE           4
#define PMP_NAPOT_MIN_SIZE         8

#define GET_SECTL_PMP_NUM(secIndx)          ((UINT8)(secIndx) << 1)
#define CLEAR_SECTL_BIT(secIdx)             (~((0x3U << GET_SECTL_PMP_NUM(secIdx)) & UINT32_CUT_MASK))

#define GET_MEMATTRREG_NUM(memAttrIndx)     ((UINT8)(memAttrIndx) >> 3)
#define GET_MEMATTR_PMP_NUM(memAttrIndx)    (((UINT8)(memAttrIndx) & 0x7) << 2)
#define CLEAR_MEMATTR_BIT(memAttrIndx)      (~((0xfU << GET_MEMATTR_PMP_NUM(memAttrIndx)) & UINT32_CUT_MASK))

#define MEM_ATTR_DEFAULT_VALUE              MEM_ATTR_DEV_NON_BUF
#endif

#ifdef LOSCFG_ARCH_RISCV_EPMP
#define EPMP_MML_IS_ENABLE                  ((READ_CSR(MSECCFG) & MSECCFG_MML_ENABLE) == MSECCFG_MML_ENABLE)
#else
#define EPMP_MML_IS_ENABLE                  FALSE
#endif

#define GET_PMPREG_NUM(pmpIdx)                                ((UINT8)((UINT8)(pmpIdx) & 0x3) << 3)
#define CLEAR_PMPCFG_BIT(pmpIdx, cfgValue)                    ((UINT32)(cfgValue) & \
                                                              ~((0xff << GET_PMPREG_NUM(pmpIdx)) & UINT32_CUT_MASK))
#define GET_PMPCFG_VALID_VALUE(pmpIdx, cfgOldValue, cfgValue) (((UINT32)(cfgValue) << GET_PMPREG_NUM(pmpIdx)) | \
                                                              CLEAR_PMPCFG_BIT(pmpIdx, cfgOldValue))
#define GET_PMPCFG_NUM(pmpIdx)                                ((UINT8)(pmpIdx) >> 2)

#define GET_PMP_NAPOT_SIZE(value) (((UINT32)(value) >> 1) - 1)
#define SET_PMP_PROT_ADDR(value) ((UINT32)(value) >> 2)
#define GET_PMP_PROT_ADDR(value) (((UINT32)(value) << 2) & UINT32_CUT_MASK)

#define GET_NAPOT_PMP_REG_PROT_SIZE(value) (1U << (Ctz32((value) + 1) + 3))
#define IS_ALIGN(addr, boundary) ((UINT32)(addr) & ((UINT32)(boundary) - 1))

LITE_OS_SEC_DATA UINT64 g_pmpMaxAddr = LOSCFG_PMP_MAX_ADDRESS;
LITE_OS_SEC_DATA UINT64 g_pmpMinAddr = LOSCFG_PMP_MIN_ADDRESS;
LITE_OS_SEC_DATA STATIC BOOL g_pmpTopFlag = FALSE;
STATIC CSR_PMPREG_CONTROL g_pmpAddrFunc[LOSCFG_PMP_MAX_SUPPORT] = {
    PMP_CSR_FUNC_GET(PMPADDR0), PMP_CSR_FUNC_GET(PMPADDR1), PMP_CSR_FUNC_GET(PMPADDR2), PMP_CSR_FUNC_GET(PMPADDR3),
    PMP_CSR_FUNC_GET(PMPADDR4), PMP_CSR_FUNC_GET(PMPADDR5), PMP_CSR_FUNC_GET(PMPADDR6), PMP_CSR_FUNC_GET(PMPADDR7),
    PMP_CSR_FUNC_GET(PMPADDR8), PMP_CSR_FUNC_GET(PMPADDR9), PMP_CSR_FUNC_GET(PMPADDR10), PMP_CSR_FUNC_GET(PMPADDR11),
    PMP_CSR_FUNC_GET(PMPADDR12), PMP_CSR_FUNC_GET(PMPADDR13), PMP_CSR_FUNC_GET(PMPADDR14), PMP_CSR_FUNC_GET(PMPADDR15),
#if (LOSCFG_PMP_MAX_SUPPORT == 32)
    PMP_CSR_FUNC_GET(PMPADDR16), PMP_CSR_FUNC_GET(PMPADDR17), PMP_CSR_FUNC_GET(PMPADDR18), PMP_CSR_FUNC_GET(PMPADDR19),
    PMP_CSR_FUNC_GET(PMPADDR20), PMP_CSR_FUNC_GET(PMPADDR21), PMP_CSR_FUNC_GET(PMPADDR22), PMP_CSR_FUNC_GET(PMPADDR23),
    PMP_CSR_FUNC_GET(PMPADDR24), PMP_CSR_FUNC_GET(PMPADDR25), PMP_CSR_FUNC_GET(PMPADDR26), PMP_CSR_FUNC_GET(PMPADDR27),
    PMP_CSR_FUNC_GET(PMPADDR28), PMP_CSR_FUNC_GET(PMPADDR29), PMP_CSR_FUNC_GET(PMPADDR30), PMP_CSR_FUNC_GET(PMPADDR31)
#endif
};

#ifdef LOSCFG_ARCH_LINX_M
STATIC CSR_PMPREG_CONTROL g_memAttrFunc[LOSCFG_PMP_MAX_SUPPORT] = {
    PMP_CSR_FUNC_GET(MEMATTR0), PMP_CSR_FUNC_GET(MEMATTR1), PMP_CSR_FUNC_GET(MEMATTR2), PMP_CSR_FUNC_GET(MEMATTR3),
    PMP_CSR_FUNC_GET(MEMATTR4), PMP_CSR_FUNC_GET(MEMATTR5), PMP_CSR_FUNC_GET(MEMATTR6), PMP_CSR_FUNC_GET(MEMATTR7),
    PMP_CSR_FUNC_GET(MEMATTR8), PMP_CSR_FUNC_GET(MEMATTR9), PMP_CSR_FUNC_GET(MEMATTR10), PMP_CSR_FUNC_GET(MEMATTR11),
    PMP_CSR_FUNC_GET(MEMATTR12), PMP_CSR_FUNC_GET(MEMATTR13), PMP_CSR_FUNC_GET(MEMATTR14), PMP_CSR_FUNC_GET(MEMATTR15),
    PMP_CSR_FUNC_GET(MEMATTR16), PMP_CSR_FUNC_GET(MEMATTR17), PMP_CSR_FUNC_GET(MEMATTR18), PMP_CSR_FUNC_GET(MEMATTR19),
    PMP_CSR_FUNC_GET(MEMATTR20), PMP_CSR_FUNC_GET(MEMATTR21), PMP_CSR_FUNC_GET(MEMATTR22), PMP_CSR_FUNC_GET(MEMATTR23),
    PMP_CSR_FUNC_GET(MEMATTR24), PMP_CSR_FUNC_GET(MEMATTR25), PMP_CSR_FUNC_GET(MEMATTR26), PMP_CSR_FUNC_GET(MEMATTR27),
    PMP_CSR_FUNC_GET(MEMATTR28), PMP_CSR_FUNC_GET(MEMATTR29), PMP_CSR_FUNC_GET(MEMATTR30), PMP_CSR_FUNC_GET(MEMATTR31)
};
#endif

STATIC CSR_PMPREG_CONTROL g_pmpCfgFunc[LOSCFG_PMPCFG_MAX_SUPPORT] = {
    PMP_CSR_FUNC_GET(PMPCFG0), PMP_CSR_FUNC_GET(PMPCFG1), PMP_CSR_FUNC_GET(PMPCFG2), PMP_CSR_FUNC_GET(PMPCFG3),
#if (LOSCFG_PMPCFG_MAX_SUPPORT == 8)
    PMP_CSR_FUNC_GET(PMPCFG4), PMP_CSR_FUNC_GET(PMPCFG5), PMP_CSR_FUNC_GET(PMPCFG6), PMP_CSR_FUNC_GET(PMPCFG7)
#endif
};

LITE_OS_SEC_TEXT STATIC INLINE BOOL PmpIsLockBypass(VOID)
{
#ifdef LOSCFG_ARCH_RISCV_EPMP
    if (READ_CSR(MSECCFG) & MSECCFG_RLB_ENABLE) {
        return TRUE;
    }
#endif
    return FALSE;
}

/*****************************************************************************
 Function   : PmpCfgRead
 Description: Read pmp config register.
 Input      : number -- pmp config serial number.
 Return     : Pmp config register value.
 *****************************************************************************/
LITE_OS_SEC_TEXT STATIC UINT8 PmpCfgRead(UINT8 number)
{
    CSR_PMPREG_CONTROL pmpFunc = NULL;
    UINT32 pmpCfgRegVal = 0;

    if (GET_PMPCFG_NUM(number) >= LOSCFG_PMPCFG_MAX_SUPPORT) {
        return 0;
    }

    pmpFunc = g_pmpCfgFunc[GET_PMPCFG_NUM(number)];
    LOS_ASSERT(pmpFunc != NULL);

    pmpCfgRegVal = pmpFunc(FALSE, 0);

    return (UINT8)(pmpCfgRegVal >> GET_PMPREG_NUM(number)) & UINT8_CUT_MASK;
}

LITE_OS_SEC_TEXT STATIC UINT32 PmpIsLocked(UINT8 number)
{
    UINT8 pmpCfg;

    pmpCfg = PmpCfgRead(number);
    if (pmpCfg & PMP_RGN_LOCK) {
        return TRUE;
    }
    return FALSE;
}

LITE_OS_SEC_TEXT STATIC UINT32 PmpAddrRead(UINT8 pmpAddrIdx)
{
    CSR_PMPREG_CONTROL pmpFunc = NULL;
    if (pmpAddrIdx >= LOSCFG_PMP_MAX_SUPPORT) {
        return 0;
    }
    pmpFunc = g_pmpAddrFunc[pmpAddrIdx];
    LOS_ASSERT(pmpFunc != NULL);
    return pmpFunc(FALSE, 0);
}

LITE_OS_SEC_TEXT STATIC VOID PmpAddrWrite(UINT8 pmpAddrIdx, UINT32 pmpAddrVal)
{
    CSR_PMPREG_CONTROL pmpFunc = NULL;
    if (pmpAddrIdx >= LOSCFG_PMP_MAX_SUPPORT) {
        return;
    }
    pmpFunc = g_pmpAddrFunc[pmpAddrIdx];
    LOS_ASSERT(pmpFunc != NULL);
    (VOID)pmpFunc(TRUE, pmpAddrVal);
}

LITE_OS_SEC_TEXT STATIC UINT32 PmpTorLowerBoundRead(UINT8 pmpAddrIdx)
{
    UINT8  pmpAddrMatch;
    UINT32 pmpPreAddrVal;
    UINT32 pmpPreSize;
    UINT32 pmpPreBaseAddr;
    UINT32 napotSize;

    /* If PMP entry 0’s A field is set to TOR, zero is used for the lower bound */
    if (pmpAddrIdx == 0) {
        return 0;
    }

    pmpAddrMatch = PmpCfgRead(pmpAddrIdx - 1) & PMP_RGN_ADDR_MATCH_MASK;
    switch (pmpAddrMatch) {
        case PMP_RGN_ADDR_MATCH_OFF:
            /* FALLTHROUGH */
        case PMP_RGN_ADDR_MATCH_TOR:
            pmpPreAddrVal = PmpAddrRead(pmpAddrIdx - 1);
            return GET_PMP_PROT_ADDR(pmpPreAddrVal);
        case PMP_RGN_ADDR_MATCH_NAPOT:
            pmpPreAddrVal = PmpAddrRead(pmpAddrIdx - 1);
            PRINT_INFO("pre region addr value = 0x%x\n", pmpPreAddrVal);
            pmpPreSize = GET_NAPOT_PMP_REG_PROT_SIZE(pmpPreAddrVal);
            PRINT_INFO("pre region size = 0x%x\n", pmpPreSize);
            napotSize = GET_PMP_NAPOT_SIZE(pmpPreSize);    /* get the pre-region size */
            pmpPreBaseAddr = GET_PMP_PROT_ADDR(pmpPreAddrVal) & (~napotSize);    /* get the pre-region base address */
            PRINT_INFO("pre region base address = 0x%x\n", pmpPreBaseAddr);
            /* get the pre-region end address, i.e. the lower bound of this TOR region */
            return (pmpPreBaseAddr + pmpPreSize);
        default:
            return 0;
    }
}

LITE_OS_SEC_TEXT STATIC UINT32 PmpCheckNextPmpEntry(const PMP_ENTRY_INFO *pmpInfo)
{
    UINT8 pmpcfgMode = PmpCfgRead(pmpInfo->ucNumber) & PMP_RGN_ADDR_MATCH_MASK;
    if ((pmpcfgMode == PMP_RGN_ADDR_MATCH_OFF) || (pmpcfgMode == PMP_RGN_ADDR_MATCH_TOR)) {
        if (pmpInfo->ucAddressMatch == PMP_RGN_ADDR_MATCH_NAPOT) {
            return LOS_ERRNO_PMP_TOR_INVALID_MATCH_MODE;
        }
    } else if (pmpcfgMode == PMP_RGN_ADDR_MATCH_NAPOT) {
        if (pmpInfo->ucAddressMatch != PMP_RGN_ADDR_MATCH_NAPOT) {
            return LOS_ERRNO_PMP_NAPOT_INVALID_MATCH_MODE;
        }
    }

    return LOS_OK;
}

LITE_OS_SEC_TEXT STATIC UINT32 PmpBaseAddrAndRegionSizeCheck(UINT8 number,
                                                             UINT8 addrMatch,
                                                             UINT32 baseAddr,
                                                             UINT32 regionSize)
{
    UINT32 prevAddr;
    /* Base address is not in the range of [LOSCFG_PMP_MIN_ADDRESS, LOSCFG_PMP_MAX_ADDRESS] */
    if ((baseAddr < g_pmpMinAddr) || (baseAddr > g_pmpMaxAddr)) {
        return LOS_ERRNO_PMP_BASE_ADDRESS_NOT_IN_RANGE;
    }

    /* the PMP region start address and end address must be 4 aligned. */
    if (IS_ALIGN(baseAddr, PMP_ADDR_ALIGN_SIZE) != 0) {
        return LOS_ERRNO_PMP_INVALID_BASE_ADDRESS;
    }

    if (addrMatch == PMP_RGN_ADDR_MATCH_NAPOT) {
        if ((regionSize < PMP_NAPOT_MIN_SIZE) && (((UINT64)baseAddr + regionSize) > g_pmpMaxAddr)) {
            return LOS_ERRNO_PMP_NAPOT_ENCODING_NON_MATCHED;
        }

        /* check the naturally aligned power-of-2 regions (NAPOT) size */
        if (((regionSize - 1) & regionSize) != 0) {
            return LOS_ERRNO_PMP_INVALID_CAPACITY;
        }

        /* the NAPOT type and size should be matched in NAPOT range encoding. */
        if (((regionSize - 1) & baseAddr) != 0) {
            return LOS_ERRNO_PMP_NAPOT_ENCODING_NON_MATCHED;
        }
    }

    /* check TOR base address equals to the pre-region end address */
    if (addrMatch == PMP_RGN_ADDR_MATCH_TOR) {
        prevAddr = PmpTorLowerBoundRead(number);
        if (baseAddr < (prevAddr + PMP_TOR_MIN_SIZE)) {
            return LOS_ERRNO_PMP_TOR_LOWER_BOUND_NONMATCHED;
        }
    }

    return LOS_OK;
}

#ifdef LOSCFG_ARCH_RISCV_EPMP
LITE_OS_SEC_TEXT STATIC UINT32 EpmpGetPermissionType(PmpAccInfo accInfo)
{
    /* Only the following permissions are supported():
    * user   R	 W	 X  kernel	R	W	X	  mode:L      R	  W	  X	   rule:
    * 0      #	 #	 #          #	#	#	       0      0	  0	  0	    NA
    * 1      #	 #	 √          #	#	#	       0      0	  0	  1	    u-mode-only
    * 2      √	 #	 #          √	√	#	       0      0	  1	  0	    shared data region
    * 3      √	 √	 #          √	√	#	       0      0	  1	  1	    shared data region
    * 4      √	 #	 #          #	#	#	       0      1	  0	  0	    u-mode-only
    * 5      √	 #	 √          #	#	#	       0      1	  0	  1	    u-mode-only
    * 6      √	 √	 #          #	#	#	       0      1	  1	  0	    u-mode-only
    * 7      √	 √	 √          #	#	#	       0      1	  1	  1	    u-mode-only
    *
    * 8      #	 #	 #          #	#	#		   1      0	  0	  0	    NA
    * 9      #	 #	 #          #	#	√		   1      0	  0	  1	    m-mode-only
    * 10     #	 #	 √          #	#	√		   1      0	  1	  0	    share code region
    * 11     #	 #	 √          √	#	√		   1      0	  1	  1	    share code region
    * 12     #	 #	 #          √	#	#		   1      1	  0	  0	    m-mode-only
    * 13     #	 #	 #          √	#	√		   1      1	  0	  1	    m-mode-only
    * 14     #	 #	 #          √	√	#		   1      1	  1	  0	    m-mode-only
    * 15     √	 #	 #          √	#	#		   1      1	  1	  1	    shared data region
    */

    BOOL userRead = ((accInfo.readAcc == E_MEM_U_R_P_NR) || (accInfo.readAcc == E_MEM_U_R_P_R));
    BOOL userWrite = ((accInfo.writeAcc == E_MEM_U_W_P_NW) || (accInfo.writeAcc == E_MEM_U_W_P_W));
    BOOL userExc = ((accInfo.excuteAcc == E_MEM_U_X_P_NX) || (accInfo.excuteAcc == E_MEM_U_X_P_X));
    BOOL kernelRead = ((accInfo.readAcc == E_MEM_U_NR_P_R) || (accInfo.readAcc == E_MEM_U_R_P_R));
    BOOL kernelWrite = ((accInfo.writeAcc == E_MEM_U_NW_P_W) || (accInfo.writeAcc == E_MEM_U_W_P_W));
    BOOL kernelExc = ((accInfo.excuteAcc == E_MEM_U_NX_P_X) || (accInfo.excuteAcc == E_MEM_U_X_P_X));

    /* U-mode-only */
    if (!kernelRead && !kernelWrite && !kernelExc) {
        if (!userRead && userWrite) {
            return UNKNOWN_MARK_RULE;
        }
        return U_MODE_ONLY_RULE;
    }

    /* M-mode-only */
    if (!userRead && !userWrite && !userExc) {
        if ((!(kernelRead && !kernelExc)) && kernelWrite) {
            return UNKNOWN_MARK_RULE;
        }
        return M_MODE_ONLY_RULE;
    }

    /* Shared-Region */
    if ((accInfo.excuteAcc == E_MEM_U_X_P_X) && (accInfo.writeAcc == E_MEM_U_NW_P_NW) && !userRead) {
        return SHARE_CODE_REGION_RULE;
    }

    /* Shared-Region */
    if ((accInfo.excuteAcc == E_MEM_U_NX_P_NX) && (accInfo.readAcc == E_MEM_U_R_P_R)) {
        if (userWrite && !kernelWrite) {
            return UNKNOWN_MARK_RULE;
        }
        return SHARE_DATA_REGION_RULE;
    }

    return UNKNOWN_MARK_RULE;
}

LITE_OS_SEC_TEXT STATIC UINT32 EpmpAccpGet(PmpAccInfo accInfo, UINT32 mode)
{
    UINT32 pmpCfgAcc = 0;
    BOOL userRead = ((accInfo.readAcc == E_MEM_U_R_P_NR) || (accInfo.readAcc == E_MEM_U_R_P_R));
    BOOL userWrite = ((accInfo.writeAcc == E_MEM_U_W_P_NW) || (accInfo.writeAcc == E_MEM_U_W_P_W));
    BOOL userExc = ((accInfo.excuteAcc == E_MEM_U_X_P_NX) || (accInfo.excuteAcc == E_MEM_U_X_P_X));
    BOOL kernelRead = ((accInfo.readAcc == E_MEM_U_NR_P_R) || (accInfo.readAcc == E_MEM_U_R_P_R));
    BOOL kernelWrite = ((accInfo.writeAcc == E_MEM_U_NW_P_W) || (accInfo.writeAcc == E_MEM_U_W_P_W));
    BOOL kernelExc = ((accInfo.excuteAcc == E_MEM_U_NX_P_X) || (accInfo.excuteAcc == E_MEM_U_X_P_X));
    switch (mode) {
        case M_MODE_ONLY_RULE:
            pmpCfgAcc = (kernelRead << PMPCFG_R_BIT) | (kernelWrite << PMPCFG_W_BIT) |
                        (kernelExc << PMPCFG_X_BIT) | PMP_RGN_LOCK;
            break;
        case U_MODE_ONLY_RULE:
            pmpCfgAcc = (userRead << PMPCFG_R_BIT) | (userWrite << PMPCFG_W_BIT) | (userExc << PMPCFG_X_BIT);
            break;
        case SHARE_CODE_REGION_RULE:
            pmpCfgAcc = (1U << PMPCFG_W_BIT) | (kernelRead << PMPCFG_X_BIT) | PMP_RGN_LOCK;
            break;
        case SHARE_DATA_REGION_RULE:
            if (userWrite || kernelWrite) {
                pmpCfgAcc = (1U << PMPCFG_W_BIT) | (userWrite << PMPCFG_X_BIT);
            } else {
                pmpCfgAcc = (1U << PMPCFG_R_BIT) | (1U << PMPCFG_W_BIT) | (1U << PMPCFG_X_BIT) | PMP_RGN_LOCK;
            }
            break;
        default:
            break;
    }
    return pmpCfgAcc;
}
#endif

LITE_OS_SEC_TEXT UINT32 PmpAccessCheck(PmpAccInfo accInfo)
{
#ifdef LOSCFG_ARCH_RISCV_EPMP
    if (EPMP_MML_IS_ENABLE) {
        if (EpmpGetPermissionType(accInfo) == UNKNOWN_MARK_RULE) {
            return LOS_ERRNO_PMP_REGION_INVALID_ACCESS;
        }
        return LOS_OK;
    }
#endif

    if ((accInfo.readAcc > E_MEM_RD_ACC_RD) || (accInfo.writeAcc > E_MEM_WR_ACC_WR) ||
        (accInfo.excuteAcc > E_MEM_EX_ACC_EX)) {
        return LOS_ERRNO_PMP_REGION_INVALID_ACCESS;
    }
    return LOS_OK;
}

#ifdef LOSCFG_ARCH_LINX_M
/*****************************************************************************
 Function   : PmpGetMemAttrType
 Description: get memory attribute type
 Input      : memAttr   -- inner memory attribute or outer memory attribute
 Return     : unknown or device type or normal type
 *****************************************************************************/
LITE_OS_SEC_TEXT STATIC UINT32 PmpGetMemAttrType(UINT32 memAttr)
{
    if ((memAttr == MEM_ATTR_DEV_NON_BUF) || (memAttr == MEM_ATTR_DEV_BUF)) {
        return MEM_ATTRTYPE_DEV;
    } else if ((memAttr == MEM_ATTR_NORM_NON_CA_NON_BUF) || (memAttr == MEM_ATTR_NORM_NON_CA_BUF) ||
               (memAttr == MEM_ATTR_WT_NA) || (memAttr == MEM_ATTR_WT_RA) ||
               (memAttr == MEM_ATTR_WB_RD_ALLOC) || (memAttr == MEM_ATTR_WT_RWA) ||
               (memAttr == MEM_ATTR_WB_RW_ALLOC)) {
        return MEM_ATTRTYPE_NORMAL;
    }
    return MEM_ATTRTYPE_UNKNOWN;
}
#else
LITE_OS_SEC_TEXT STATIC UINT32 PmpGetMemAttrType(UINT32 memAttr)
{
    if (memAttr == MEM_ATTR_DEV_NON_BUF) {
        return MEM_ATTRTYPE_DEV;
    }

    if ((memAttr == MEM_ATTR_NORM_NON_CA_NON_BUF) || (memAttr == MEM_ATTR_NORM_NON_CA_BUF) ||
               (memAttr == MEM_ATTR_WB_RD_ALLOC) || (memAttr == MEM_ATTR_WB_RW_ALLOC)) {
        return MEM_ATTRTYPE_NORMAL;
    }
    return MEM_ATTRTYPE_UNKNOWN;
}
#endif

LITE_OS_SEC_TEXT STATIC UINT32 PmpAttrAndSectlCheck(const PMP_ENTRY_INFO *pmpInfo)
{
    if (PmpGetMemAttrType((UINT32)(pmpInfo->memAttr)) == MEM_ATTRTYPE_UNKNOWN) {
        return LOS_ERRNO_PMP_ATTR_INVALID;
    }

    if ((pmpInfo->seCtl != SEC_CONTRLO_SECURE_NOMMU) && (pmpInfo->seCtl != SEC_CONTRLO_NOSECURE_NOMMU) &&
        (pmpInfo->seCtl != SEC_CONTRLO_SECURE_MMU) && (pmpInfo->seCtl != SEC_CONTRLO_NOSECURE_MMU)) {
        return LOS_ERRNO_PMP_SECTL_INVALID;
    }

    if (PmpAccessCheck(pmpInfo->accPermission) != LOS_OK) {
        return LOS_ERRNO_PMP_REGION_INVALID_ACCESS;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT STATIC UINT32 PmpCheckParame(PMP_ENTRY_INFO *pmpInfo)
{
    UINT32 ret;
    UINT8  pmpcfgNext;

    if (pmpInfo == NULL) {
        return LOS_ERRNO_PMP_PTR_NULL;
    }

    /* NA4 is not supported in the LinxCore M-profile */
    if (pmpInfo->ucAddressMatch == PMP_RGN_ADDR_MATCH_NA4) {
        return LOS_ERRNO_PMP_INVALID_MATCH;
    }

    /* Number exceeds the maximum value supported by PMO */
    if (pmpInfo->ucNumber >= LOSCFG_PMP_MAX_SUPPORT) {
        return LOS_ERRNO_PMP_INVALID_NUMBER;
    }

    /* Check the entry whether locked or not */
    if (!PmpIsLockBypass() && PmpIsLocked(pmpInfo->ucNumber)) {
        return LOS_ERRNO_PMP_ENTRY_IS_LOCKED;
    }

    g_pmpTopFlag = FALSE;
    /* If address matching mode equals to TOR, check the next PMP entry */
    if ((pmpInfo->ucNumber + 1) < LOSCFG_PMP_MAX_SUPPORT) {
        pmpcfgNext = PmpCfgRead(pmpInfo->ucNumber + 1);
        if (((pmpcfgNext & PMP_RGN_LOCK) && !PmpIsLockBypass()) &&
            ((pmpcfgNext & PMP_RGN_ADDR_MATCH_MASK) == PMP_RGN_ADDR_MATCH_TOR)) {
            ret = PmpCheckNextPmpEntry(pmpInfo);
            if (ret != LOS_OK) {
                return ret;
            }
            g_pmpTopFlag = TRUE;
        }
    }

    if (g_pmpTopFlag != TRUE) {
        ret = PmpBaseAddrAndRegionSizeCheck(pmpInfo->ucNumber, pmpInfo->ucAddressMatch,
                                            pmpInfo->uwBaseAddress, pmpInfo->uwRegionSize);
        if (ret != LOS_OK) {
            return ret;
        }
    }

    if (pmpInfo->ucAddressMatch != PMP_RGN_ADDR_MATCH_OFF) {
        return PmpAttrAndSectlCheck(pmpInfo);
    } else {
        pmpInfo->memAttr = MEM_ATTR_DEFAULT_VALUE;
    #ifdef LOSCFG_ARCH_LINXCORE_131
        pmpInfo->seCtl = SEC_CONTRLO_SECURE_NOMMU;
    #endif
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT STATIC UINT8 PmpAccpGet(PmpAccInfo accPermission)
{
#ifdef LOSCFG_ARCH_RISCV_EPMP
    if (EPMP_MML_IS_ENABLE) {
        return (UINT8)EpmpAccpGet(accPermission, EpmpGetPermissionType(accPermission));
    }
#endif
    UINT32 pmpCfgAcc = 0;

    if (accPermission.readAcc == E_MEM_RD_ACC_RD) {
        pmpCfgAcc |= (1U << PMPCFG_R_BIT);
    }

    if (accPermission.writeAcc == E_MEM_WR_ACC_WR) {
        pmpCfgAcc |= (1U << PMPCFG_W_BIT);
    }

    if (accPermission.excuteAcc == E_MEM_EX_ACC_EX) {
        pmpCfgAcc |= (1U << PMPCFG_X_BIT);
    }

    return (UINT8)pmpCfgAcc;
}

LITE_OS_SEC_TEXT STATIC VOID PmpCfgWrite(UINT8 pmpCfgIdx, UINT8 pmpCfgVal)
{
    CSR_PMPREG_CONTROL pmpFunc = NULL;
    UINT32 cfgVal;

    if (GET_PMPCFG_NUM(pmpCfgIdx) >= LOSCFG_PMPCFG_MAX_SUPPORT) {
        return;
    }

    pmpFunc = g_pmpCfgFunc[GET_PMPCFG_NUM(pmpCfgIdx)];
    LOS_ASSERT(pmpFunc != NULL);

    cfgVal = pmpFunc(FALSE, 0);
    cfgVal = GET_PMPCFG_VALID_VALUE(pmpCfgIdx, cfgVal, pmpCfgVal);
    (VOID)pmpFunc(TRUE, cfgVal);
}

/*****************************************************************************
 Function    : PmpNapotAddrGet
 Description : get the pmp address (NAPOT) of the physical memory protection entry
 Input       : base -- base addr
 Input       : size -- protection size
 Output      : pmp address (NAPOT)
 Return      : OS_SUCCESS
 *****************************************************************************/
LITE_OS_SEC_TEXT STATIC UINT32 PmpNapotAddrGet(UINT32 base,  UINT32 size)
{
    return (UINT32)SET_PMP_PROT_ADDR(base + GET_PMP_NAPOT_SIZE(size));
}

#ifdef LOSCFG_ARCH_LINX_M
LITE_OS_SEC_TEXT STATIC VOID PmpMemattrWrite(UINT8 pmpMemAttrIdx, PmpMemAttrInfo pmpMemAttrVal)
{
    CSR_PMPREG_CONTROL pmpFunc = NULL;
    if (pmpMemAttrIdx >= LOSCFG_PMP_MAX_SUPPORT) {
        return;
    }
    pmpFunc = g_memAttrFunc[pmpMemAttrIdx];
    LOS_ASSERT(pmpFunc != NULL);

    (VOID)pmpFunc(TRUE, (UINT32)(pmpMemAttrVal | (pmpMemAttrVal << MEMATTR_OATTR_BIT)));
}

LITE_OS_SEC_TEXT STATIC VOID PmpSectlWrite(UINT8 pmpSecIdx, UINT8 pmpCfgVal)
{
    CSR_PMPREG_CONTROL pmpFunc = NULL;
    UINT32 memattr;
    if (pmpSecIdx >= LOSCFG_PMP_MAX_SUPPORT) {
        return;
    }
    pmpFunc = g_memAttrFunc[pmpSecIdx];
    LOS_ASSERT(pmpFunc != NULL);
    memattr = pmpFunc(FALSE, 0);
    if ((pmpCfgVal == (UINT8)SEC_CONTRLO_NOSECURE_NOMMU) || (pmpCfgVal == (UINT8)SEC_CONTRLO_NOSECURE_MMU)) {
        memattr |= 1U << MEMATTR_NS_BIT;
    }
    if ((pmpCfgVal == (UINT8)SEC_CONTRLO_SECURE_MMU) || (pmpCfgVal == (UINT8)SEC_CONTRLO_NOSECURE_MMU)) {
        memattr |= 1U << MEMATTR_ME_BIT;
    }
    (VOID)pmpFunc(TRUE, memattr);
}
#else
LITE_OS_SEC_TEXT STATIC VOID PmpMemattrWrite(UINT8 pmpMemAttrIdx, PmpMemAttrInfo pmpMemAttrVal)
{
    UINT32 memAttrVal;

    if (GET_MEMATTRREG_NUM(pmpMemAttrIdx) == 0) {
        memAttrVal  = READ_CUSTOM_CSR(MEMATTRL);
        memAttrVal &= CLEAR_MEMATTR_BIT(pmpMemAttrIdx);
        memAttrVal |= ((UINT32)pmpMemAttrVal << GET_MEMATTR_PMP_NUM(pmpMemAttrIdx));
        WRITE_CUSTOM_CSR_VAL(MEMATTRL, memAttrVal);
    } else {
        memAttrVal  = READ_CUSTOM_CSR(MEMATTRH);
        memAttrVal &= CLEAR_MEMATTR_BIT(pmpMemAttrIdx);
        memAttrVal |= ((UINT32)pmpMemAttrVal << GET_MEMATTR_PMP_NUM(pmpMemAttrIdx));
        WRITE_CUSTOM_CSR_VAL(MEMATTRH, memAttrVal);
    }
}

LITE_OS_SEC_TEXT STATIC VOID PmpSectlWrite(UINT8 pmpSecIdx, UINT8 pmpCfgVal)
{
    UINT32 sectl;

    sectl  = READ_CUSTOM_CSR(SECTL);
    sectl &= CLEAR_SECTL_BIT(pmpSecIdx);
    sectl |= (pmpCfgVal << GET_SECTL_PMP_NUM(pmpSecIdx));
    WRITE_CUSTOM_CSR_VAL(SECTL, sectl);
}
#endif

/*****************************************************************************
 Function   : ArchRegionDisable
 Description: disable region by number
 Input      : number -- mpu region number want to disable
 Return     : LOS_OK or Error Information
 *****************************************************************************/
LITE_OS_SEC_TEXT UINT32 ArchRegionDisable(UINT8 number)
{
    UINT32 intSave;
    if (number >= LOSCFG_PMP_MAX_SUPPORT) {
        return LOS_ERRNO_PMP_INVALID_NUMBER;
    }

    if (!PmpIsLockBypass() && PmpIsLocked(number)) {
        return LOS_ERRNO_PMP_ENTRY_IS_LOCKED;
    }

    intSave = LOS_IntLock();

    PmpSectlWrite(number, SEC_CONTRLO_SECURE_NOMMU);

    PmpCfgWrite(number, PMP_RGN_ADDR_MATCH_OFF);

    LOS_IntRestore(intSave);

    return LOS_OK;
}


LITE_OS_SEC_TEXT STATIC VOID PmpRegionConfig(const PMP_ENTRY_INFO *pmpInfo)
{
    UINT8 pmpXCfg = 0;
    UINT32 pmpXAddr = UINT32_CUT_MASK;

    switch ((pmpInfo->ucAddressMatch & PMP_RGN_ADDR_MATCH_MASK)) {
        case PMP_RGN_ADDR_MATCH_OFF:
            pmpXAddr = SET_PMP_PROT_ADDR(pmpInfo->uwBaseAddress);
            break;
        case PMP_RGN_ADDR_MATCH_NAPOT:
            pmpXAddr = PmpNapotAddrGet(pmpInfo->uwBaseAddress, pmpInfo->uwRegionSize);
            break;
        case PMP_RGN_ADDR_MATCH_TOR:
            pmpXAddr = SET_PMP_PROT_ADDR(pmpInfo->uwBaseAddress);
            break;
        case PMP_RGN_ADDR_MATCH_NA4:
            /* The PMP entry size granularity must be the multiple of cache line size, not support yet! */
            return;
        default:
            break;
    }

    /* write pmp address register */
    if (g_pmpTopFlag != TRUE) {
        PmpAddrWrite(pmpInfo->ucNumber, pmpXAddr);
    }

    /* write pmp memattr/memattrh register */
    PmpMemattrWrite(pmpInfo->ucNumber, pmpInfo->memAttr);

    /* write pmp sectl register */
    PmpSectlWrite(pmpInfo->ucNumber, pmpInfo->seCtl);

    /* write pmp cfg register */
    if (pmpInfo->ucAddressMatch != PMP_RGN_ADDR_MATCH_OFF) {
        pmpXCfg |= (pmpInfo->ucAddressMatch) | (PmpAccpGet(pmpInfo->accPermission));
    }

    if (!EPMP_MML_IS_ENABLE && (pmpInfo->bLocked)) {
        pmpXCfg |= PMP_RGN_LOCK;
    }

    PmpCfgWrite(pmpInfo->ucNumber, pmpXCfg);
}

#ifdef LOSCFG_ARCH_LINXCORE_131
/*****************************************************************************
 Function   : ArchGetSectl
 Description: Get Sectl region
 Input      : None
 Return     : Sectl register value
 *****************************************************************************/
LITE_OS_SEC_TEXT UINT32 ArchGetSectl(VOID)
{
    return READ_CUSTOM_CSR(SECTL);
}
#endif

/*****************************************************************************
 Function   : ArchProtectionRegionSet
 Description: set protection region
 Input      : pmpInfo --- PMP parameters to be set.
              The base address must be in the range of RAM
 Return     : LOS_OK or the index of pmpInfo which set up failed
 *****************************************************************************/
LITE_OS_SEC_TEXT UINT32 ArchProtectionRegionSet(PMP_ENTRY_INFO *pmpInfo)
{
    UINT32 ret;
    UINT32 intSave;

    intSave = LOS_IntLock();
    ret = PmpCheckParame(pmpInfo);
    if (ret != LOS_OK) {
        PRINT_ERR("set protection region error, ret: [0x%x]\n", ret);
        LOS_IntRestore(intSave);
        return ret;
    }
    PmpRegionConfig(pmpInfo);
    LOS_IntRestore(intSave);

    return LOS_OK;
}

LITE_OS_SEC_TEXT_INIT UINT32 ArchProtectionUnitInit(UINT64 minAddr, UINT64 maxAddr)
{
    UINT32 intSave;
    intSave = LOS_IntLock();

    g_pmpMinAddr = minAddr;
    g_pmpMaxAddr = maxAddr;

    LOS_IntRestore(intSave);
    return LOS_OK;
}
