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
	
        float                   m_value;			// accumulated value
        float                   m_valueTriggerIn;
        float                   m_valueTriggerOut;

 		vector<float>			m_valueHistory;		// history of volumes
		int						m_valueHistoryMax;	// number of history values
		float					m_valueMean;		// mean value for history values

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