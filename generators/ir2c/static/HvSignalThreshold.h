/**
 * Copyright (c) 2014-2018 Enzien Audio Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _HEAVY_SIGNAL_THRESHOLD_H_
#define _HEAVY_SIGNAL_THRESHOLD_H_

#define __HV_DELAY_MAX_MESSAGES 8

#include "HvHeavyInternal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SignalThreshold {
  float triggerThreshold;
  hv_uint32_t triggerDebounceTime;
  float restThreshold;
  hv_uint32_t restDebounceTime;
  bool triggerDebounceState;
  bool restDebounceState;
  bool waitForTrigger;
  hv_uint32_t counter;
} SignalThreshold;

hv_size_t sThreshold_init(HeavyContextInterface *_c, SignalThreshold *o, float triggerThreshold, float triggerDebounceTime, float restThreshold, float restDebounceTime);

void sThreshold_onMessage(HeavyContextInterface *_c, SignalThreshold *o, int letIn, const HvMessage *m, void (*sendMessage)(HeavyContextInterface *, int, const struct HvMessage *));

void sThreshold_process(HeavyContextInterface *_c, SignalThreshold *o, hv_bInf_t bIn,
    void (*sendMessage)(HeavyContextInterface *, int, const HvMessage *));

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _HEAVY_SIGNAL_THRESHOLD_H_
