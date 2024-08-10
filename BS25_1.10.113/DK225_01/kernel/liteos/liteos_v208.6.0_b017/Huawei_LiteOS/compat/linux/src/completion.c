/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Completion Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#include "linux/completion.h"
#include "limits.h"
#include "los_task_pri.h"
#include "los_mp_pri.h"
#include "common/pendlist.h"

void init_completion(struct completion *x)
{
    UINT32 intSave;

    if (x == NULL) {
        PRINT_ERR("init_completion failed, input param is invalid\n");
        return;
    }

    SCHEDULER_LOCK(intSave);
    LOS_ListInit(&x->pendList);
    x->comCount = 0;
    x->state = COMPLETION_ONE;

    SCHEDULER_UNLOCK(intSave);
    return;
}

STATIC BOOL NoNeedWait(struct completion *x)
{
    if (x->state == COMPLETION_ALL) {
        return TRUE;
    }
    if (x->comCount > 0) {
        x->comCount--;
        return TRUE;
    }
    return FALSE;
}

unsigned long wait_for_completion_timeout(struct completion *x, unsigned long timeout)
{
    UINT32 intSave;
    UINT32 ret;

    if (x == NULL) {
        PRINT_ERR("wait_for_completion_timeout failed, input param is invalid\n");
        return timeout;
    }

    if (OS_INT_ACTIVE || (OsCurrTaskGet()->taskFlags & OS_TASK_FLAG_SYSTEM)) {
        PRINT_ERR("DO NOT call %s in system tasks or interrupt callback.\n", __FUNCTION__);
        return timeout;
    }

    SCHEDULER_LOCK(intSave);

    if (NoNeedWait(x) == TRUE) {
        SCHEDULER_UNLOCK(intSave);
        return timeout ? timeout : 1;
    }

    ret = BlockOnPendList(&x->pendList, (UINT32)timeout);
    SCHEDULER_UNLOCK(intSave);
    return (unsigned long)ret;
}

void complete(struct completion *x)
{
    UINT32 intSave;

    if (x == NULL) {
        PRINT_ERR("complete failed, input param is invalid\n");
        return;
    }

    SCHEDULER_LOCK(intSave);
    if (!LOS_ListEmpty(&x->pendList)) {
        WakeupFromPendList(&x->pendList, 1);
        SCHEDULER_UNLOCK(intSave);
        LOS_MpSchedule(OS_MP_CPU_ALL);
        LOS_Schedule();
        return;
    } else if (x->comCount != UINT_MAX) {
        x->comCount++;
    }

    SCHEDULER_UNLOCK(intSave);
    return;
}

void complete_all(struct completion *x)
{
    UINT32 intSave;

    if (x == NULL) {
        PRINT_ERR("complete_all failed, input param is invalid\n");
        return;
    }

    SCHEDULER_LOCK(intSave);
    x->state = COMPLETION_ALL;
    if (LOS_ListEmpty(&x->pendList)) {
        SCHEDULER_UNLOCK(intSave);
        return;
    }

    WakeupFromPendList(&x->pendList, ALL_NODES_OF_LIST);
    SCHEDULER_UNLOCK(intSave);
    LOS_MpSchedule(OS_MP_CPU_ALL);
    LOS_Schedule();

    return;
}
