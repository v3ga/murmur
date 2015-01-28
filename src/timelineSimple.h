//
//  timelineSimple.h
//  murmur
//
//  Created by Julien on 28/01/2015.
//
//

#pragma once
#include "ofMain.h"

class timelineSimpleEvent
{
	public:
		typedef	void	(*timelineSimpleCb)(void*, timelineSimpleEvent*);

		timelineSimpleEvent			(string id, unsigned long long time, timelineSimpleCb pCb, void* pCbObj)
		{
			m_id		= id;
			m_time 		= time;
			mpf_cb 		= pCb;
			mp_cbObj 	= pCbObj;
			
			m_timeTrigger = 0;
		}

		void trigger(unsigned long long timeTrigger)
		{
			m_timeTrigger = timeTrigger;
			if (mpf_cb){
				(*mpf_cb)(mp_cbObj, this);
			}
		}

		string						m_id;
		unsigned long long			m_time;		// millis
		unsigned long long			m_timeTrigger;		// millis
		void*						mp_cbObj;
		timelineSimpleCb			mpf_cb;
};


class timelineSimple
{
	public:
		timelineSimple			();
		~timelineSimple			();

		void					start		();
		void					update		();

		void					addEvent			(timelineSimpleEvent*);
		void					addEvent			(string id, unsigned long long time, timelineSimpleEvent::timelineSimpleCb pCb, void* pCbObj);
		void					setEventsCallback	(timelineSimpleEvent::timelineSimpleCb pCb, void* pCbObj);
		void					load				(string file);

		unsigned long long				m_timeStart;
		vector<timelineSimpleEvent*>	m_events;
		int								m_eventIndex;
};


