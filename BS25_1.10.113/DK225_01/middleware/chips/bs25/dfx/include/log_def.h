/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  common logging producer interface - need to change name of log.h in all the protocol core files
 * Author: @CompanyNameTag
 * Create:
 */

#ifndef _LOG_DEF_H_
#define _LOG_DEF_H_

#ifdef BTC_SYS_PART
#include "log_def_btc.h"
#endif
#ifdef BTH_SYS_PART
#include "log_def_bth.h"
#endif
#ifdef SUPPORT_GLP_CLIENT
#include "log_def_glp.h"
#endif
#include "log_def_pf.h"
#endif
