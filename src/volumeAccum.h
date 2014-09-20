//
//  volumeAccum.h
//  murmur
//
//  Created by Julien on 23/05/13.
//
//

#pragma once
#include "ofMain.h"

//--------------------------------------------------------------
class VolumeAccum
{
    public:
        typedef void (*cbTriggerIn)(void*,VolumeAccum*);

    
        VolumeAccum             ();
	
		string					m_deviceId;
    
        float                   m_value;
        float                   m_valueTriggerIn;
        float                   m_valueTriggerOut;

        int                     m_state;

        cbTriggerIn             mpf_funcTriggerIn;
        void*                   mp_userData;
    
        void setTriggerInCb     (cbTriggerIn, void*);
        void add                (float volume);
    
        enum
        {
            STATE_WAVE_INSIDE = 1,
            STATE_WAVE_OUT = 2
        };

};