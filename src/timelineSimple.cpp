//
//  timelineSimple.cpp
//  murmur
//
//  Created by Julien on 28/01/2015.
//
//

#include "timelineSimple.h"
#include "ofxXmlSettings.h"
#include "ofAppLog.h"

//--------------------------------------------------------------
timelineSimple::timelineSimple()
{
	m_timeStart = 0.0f;
	m_eventIndex = 0;
	m_isStarted = false;
}

//--------------------------------------------------------------
timelineSimple::~timelineSimple()
{
	vector<timelineSimpleEvent*>::iterator it = m_events.begin();
	for ( ; it != m_events.end(); ++it)
		delete *it;
 	m_events.clear();
}

//--------------------------------------------------------------
void timelineSimple::start()
{
	m_timeStart = ofGetElapsedTimeMillis();
	m_eventIndex = 0;
	m_isStarted = true;

	// Reset events
	vector<timelineSimpleEvent*>::iterator it = m_events.begin();
	for ( ; it != m_events.end(); ++it)
		(*it)->m_timeTrigger=0;

	startThread();
}

//--------------------------------------------------------------
void timelineSimple::update()
{
	unsigned long long timeNow = ofGetElapsedTimeMillis();
	unsigned long long timeElapsed = timeNow-m_timeStart;
	if (m_events.size()>0 && m_eventIndex < m_events.size())
	{
		timelineSimpleEvent* pTimeEvent = m_events[m_eventIndex];
		if (timeElapsed>=pTimeEvent->m_time)
		{
			pTimeEvent->trigger(timeElapsed);
			 m_eventIndex++;
		}
	}
}

//--------------------------------------------------------------
void timelineSimple::threadedFunction()
{
	while( isThreadRunning() != 0 ){
		update();
		ofSleepMillis(2);
	}
}


//--------------------------------------------------------------
void timelineSimple::addEvent(timelineSimpleEvent* p)
{
	m_events.push_back(p);
}

//--------------------------------------------------------------
void timelineSimple::addEvent(string id, unsigned long long time, timelineSimpleEvent::timelineSimpleCb pCb, void* pCbObj)
{
	addEvent( new timelineSimpleEvent(id,time,pCb,pCbObj) );
}

//--------------------------------------------------------------
void timelineSimple::setEventsCallback(timelineSimpleEvent::timelineSimpleCb pCb, void* pCbObj)
{
	vector<timelineSimpleEvent*>::iterator it = m_events.begin();
	for ( ; it != m_events.end(); ++it)
	{
		timelineSimpleEvent* pEvent = *it;
		pEvent->mpf_cb = pCb;
		pEvent->mp_cbObj = pCbObj;
	}
}

//--------------------------------------------------------------
void timelineSimple::load(string file)
{
	OFAPPLOG->begin("timelineSimple::load("+file+")");
	ofxXmlSettings data;
	if (data.loadFile(file))
	{
		int nbEvents = data.getNumTags("event");
		OFAPPLOG->println("-nbEvents = "+ofToString(nbEvents));
		for (int i=0;i<nbEvents;i++)
		{
		   string id 		= data.getAttribute("event",	"id", 		"???",	i);
		   float time 		= data.getAttribute("event",	"time", 	0.0f,	i);
		   string action 	= data.getAttribute("event",	"action",	"",		i);
		 
		   OFAPPLOG->println("-id = "+id);
		   if (time>0.0)
		   {
				unsigned timeLong = (unsigned long long) (time*1000.0f);
				addEvent(id, timeLong,0,0);
			}
		}
	}
	OFAPPLOG->end();
}

