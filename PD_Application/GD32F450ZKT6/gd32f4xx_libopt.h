/*!
    \file    gd32f4xx_libopt.h
    \brief   library optional for gd32f4xx

    \version 2024-01-17, V2.6.4, demo for GD32F4xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef GD32F4XX_LIBOPT_H
#define GD32F4XX_LIBOPT_H

#ifndef GD32F450
    #define GD32F450
#endif

#if defined (GD32F450) || defined (GD32F405) || defined (GD32F407) || defined (GD32F470) || defined (GD32F425) || defined (GD32F427)
    #include "gd32f4xx_rcu.h"
    #include "gd32f4xx_adc.h"
    #include "gd32f4xx_can.h"
    #include "gd32f4xx_crc.h"
    #include "gd32f4xx_ctc.h"
    #include "gd32f4xx_dac.h"
    #include "gd32f4xx_dbg.h"
    #include "gd32f4xx_dci.h"
    #include "gd32f4xx_dma.h"
    #include "gd32f4xx_exti.h"
    #include "gd32f4xx_fmc.h"
    #include "gd32f4xx_fwdgt.h"
    #include "gd32f4xx_gpio.h"
    #include "gd32f4xx_syscfg.h"
    #include "gd32f4xx_i2c.h"
    #include "gd32f4xx_iref.h"
    #include "gd32f4xx_pmu.h"
    #include "gd32f4xx_rtc.h"
    #include "gd32f4xx_sdio.h"
    #include "gd32f4xx_spi.h"
    #include "gd32f4xx_timer.h"
    #include "gd32f4xx_trng.h"
    #include "gd32f4xx_usart.h"
    #include "gd32f4xx_wwdgt.h"
    #include "gd32f4xx_misc.h"
#endif

#if defined (GD32F450) || defined (GD32F470)
    #include "gd32f4xx_enet.h"
    #include "gd32f4xx_exmc.h"
    #include "gd32f4xx_ipa.h"
    #include "gd32f4xx_tli.h"
#endif

#if defined (GD32F407) || defined (GD32F427)
    #include "gd32f4xx_enet.h"
    #include "gd32f4xx_exmc.h"
#endif

#endif /* GD32F4XX_LIBOPT_H */