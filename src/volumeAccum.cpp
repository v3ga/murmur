//
//  volumeAccum.cpp
//  murmur
//
//  Created by Julien on 23/05/13.
//
//

#include "volumeAccum.h"

VolumeAccum::VolumeAccum()
{
    m_state = STATE_WAVE_OUT;
    m_valueTriggerIn = 0.3f;
    m_valueTriggerOut = 0.1f;
}

void VolumeAccum::setTriggerInCb(cbTriggerIn pFunc, void* pUserData)
{
    mpf_funcTriggerIn = pFunc;
    mp_userData = pUserData;
}

void VolumeAccum::add(float volume)
{
    if (m_state == STATE_WAVE_INSIDE)
    {
        if (volume <= m_valueTriggerOut)
        {
            m_state=STATE_WAVE_OUT;
        }
    }
    else if (m_state == STATE_WAVE_OUT)
    {
        m_value += volume;
        if (volume >= m_valueTriggerIn )
        {
            if (mpf_funcTriggerIn && mp_userData)
                (*mpf_funcTriggerIn)(mp_userData, this);
            
            m_state = STATE_WAVE_INSIDE;
            m_value=0;
        }
    }
    
}
