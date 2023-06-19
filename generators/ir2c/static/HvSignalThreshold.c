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

 #include "HvSignalThreshold.h"

 hv_size_t sThreshold_init(HeavyContextInterface *_c, SignalThreshold *o, float triggerThreshold, float triggerDebounceTime, float restThreshold, float restDebounceTime) {
  o->triggerThreshold = triggerThreshold;
  o->triggerDebounceTime = hv_millisecondsToSamples(_c, triggerDebounceTime);
  o->restThreshold = restThreshold;
  o->restDebounceTime = hv_millisecondsToSamples(_c, restDebounceTime);
  o->triggerDebounceState = false;
  o->restDebounceState = false;
  o->waitForTrigger = true;
  o->counter = 0;
  return 0;
}

void sThreshold_onMessage(HeavyContextInterface *_c, SignalThreshold *o, int letIn, const HvMessage *m, void (*sendMessage)(HeavyContextInterface *, int, const struct HvMessage *)) {
    switch(letIn){
        case 1:{
            if(msg_isFloat(m,0)) {
                if(msg_getFloat(m,0) == 0.0){
                    o->waitForTrigger = true;
                }
                else{
                    o->waitForTrigger = false;
                }
                o->counter = 0;
                o->triggerDebounceState = false;
                o->restDebounceState = false;
            }
        }
        default: break;
    }
}

static void sThreshold_sendMessage(HeavyContextInterface *_c, SignalThreshold *o, float outlet,
    void (*sendMessage)(HeavyContextInterface *, int, const HvMessage *)) {

    HvMessage *const m = HV_MESSAGE_ON_STACK(1);
    msg_initWithFloat(m, hv_getCurrentSample(_c) + HV_N_SIMD, 0);
    hv_scheduleMessageForObject(_c, m, sendMessage, outlet);
}

void sThreshold_process(HeavyContextInterface *_c, SignalThreshold *o, hv_bInf_t bIn,
    void (*sendMessage)(HeavyContextInterface *, int, const HvMessage *)) {

	if(o->triggerDebounceState == false && o->restDebounceState == false){
#if HV_SIMD_AVX || HV_SIMD_SSE
        const float *const b = (float *)&bIn;
#endif
	    for (int i = 0; i < HV_N_SIMD; i++){
#if HV_SIMD_AVX || HV_SIMD_SSE
	        float out = b[i & HV_N_SIMD_MASK];
#elif HV_SIMD_NEON
			float out = bIn[i & HV_N_SIMD_MASK];
#else // HV_SIMD_NONE
			float out = bIn;
#endif
	        if(o->waitForTrigger == true && out >= o->triggerThreshold){
	            o->triggerDebounceState = true;
	            sThreshold_sendMessage(_c, o, 0, sendMessage);
	            o->waitForTrigger = false;
	            break;
	        }
	        else if(o->waitForTrigger == false && out < o->restThreshold){
	            o->restDebounceState = true;
	            sThreshold_sendMessage(_c, o, 1, sendMessage);
	            o->waitForTrigger = true;
	            break;
	        }
	    }
	}
	else if(o->triggerDebounceState == true){
	    o->counter+= HV_N_SIMD;
	    if(o->counter >= o->triggerDebounceTime){
            o->counter = 0;
            o->triggerDebounceState = false;
	    }
	}

	else if(o->restDebounceState == true){
	    o->counter+= HV_N_SIMD;
	    if(o->counter >= o->restDebounceTime){
            o->counter = 0;
            o->restDebounceState = false;
	    }
	}
}
