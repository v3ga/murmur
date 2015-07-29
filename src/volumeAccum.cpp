//
//  volumeAccum.cpp
//  murmur
//
//  Created by Julien on 23/05/13.
//
//

#include "volumeAccum.h"

//--------------------------------------------------------------
VolumeAccum::VolumeAccum()
{
    m_state = STATE_WAVE_OUT;
    m_valueTriggerIn = 0.3f;
    m_valueTriggerOut = 0.1f;
	
	m_valueHistoryMax = 300;
	m_valueMean = 0.0f;
	m_value = 0.0f;
	
	m_stateTime = 0.0f;
	
}

//--------------------------------------------------------------
void VolumeAccum::setTriggerInCb(cbTriggerIn pFunc, void* pUserData)
{
    mpf_funcTriggerIn = pFunc;
    mp_userData = pUserData;
}

//--------------------------------------------------------------
void VolumeAccum::add(float volume)
{
	// Add value
	if( m_valueHistory.size() >= m_valueHistoryMax ){
		m_valueHistory.erase(m_valueHistory.end()-1, m_valueHistory.end());
	}
	m_valueHistory.insert( m_valueHistory.begin(), volume );

	// Compute mean value
	int nb = m_valueHistory.size();
	if (nb>0)
	{
		m_valueMean = 0.0f;
		for (int i=0;i<nb;i++)
		{
			m_valueMean += m_valueHistory[i];
		}
		m_valueMean /= (float)nb;
	}

	// State
    if (m_state == STATE_WAVE_INSIDE)
    {
        if (volume <= m_valueTriggerOut)
        {
            m_state=STATE_WAVE_OUT;
			m_stateTime=0.0f;
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
			m_stateTime=0.0f;
        }
    }
    
}

//--------------------------------------------------------------
void VolumeAccum::update(float dt)
{
	m_stateTime += dt;
}

//--------------------------------------------------------------
void VolumeAccum::reset()
{
	m_state = STATE_WAVE_OUT;
	m_stateTime=0.0f;
	m_value = 0.0f;
}


