/*
 *  timeline.cpp
 *  versatubes
 *
 *  Created by 2Roqs on 04/05/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#include "timeline.h"
#include "ofxXmlSettings.h"
#include "globals.h"
#include "testApp.h"

string Timeline::__EMPTY__ = "__EMPTY__";
string Timeline::__DATE_FORMAT__ = "%Y-%m-%d %H:%M:%S";

#define ____________TimelineEvent____________
//--------------------------------------------------------------
TimelineEvent::TimelineEvent()
{
	M_zeroAll();
}

//--------------------------------------------------------------
TimelineEvent::TimelineEvent(Poco::DateTime datetime_, string animationName_, string animationArgs_)
{
	M_zeroAll();

	m_datetime = datetime_;
	m_animationName = animationName_;
	m_animationArgs = animationArgs_;
}

//--------------------------------------------------------------
TimelineEvent::~TimelineEvent()
{
	//printf("~TimelineEvent()\n");
}

//--------------------------------------------------------------
void TimelineEvent::M_zeroAll()
{
	m_type = TYPE_ANIM_CHANGE;
	m_color.set(255.0,255.0);
	mp_eventNext = 0;
}


#define ____________Timeline____________
//--------------------------------------------------------------
Timeline::Timeline()
{
	m_isActive = true;

	m_cycleDuration					= 6.0*60.0;
	m_cycleDurationScene			= 6.0;
	m_cycleDurationEcoute			= 30.0;
	m_cycleDurationCompteur			= 6.0;
	m_cycleDurationContemplation	= 6.0*3.0;

}

//--------------------------------------------------------------
Timeline::~Timeline()
{
	M_empty();
}

//--------------------------------------------------------------
void Timeline::M_empty()
{
	vector<TimelineEvent*>::iterator it;
	for (it = m_listEvents.begin(); it != m_listEvents.end(); ++it){
		delete *it;
	}
}


//--------------------------------------------------------------
void Timeline::M_addEvent(Poco::DateTime datetime, string animationName, string animationArgs)
{
	TimelineEvent* pTimelineEvent = new TimelineEvent(datetime,animationName,animationArgs);
	m_listEvents.push_back(pTimelineEvent);
}

//--------------------------------------------------------------
TimelineEvent* Timeline::M_addEventColor(Poco::DateTime datetime, ofColor color)
{
	TimelineEvent* pTimelineEvent	= new TimelineEvent();

	pTimelineEvent->m_datetime		= datetime;
	pTimelineEvent->m_type			= TimelineEvent::TYPE_COLOR_CHANGE; 
	pTimelineEvent->m_color			= color;
	
	m_listEvents.push_back(pTimelineEvent);
	
	return pTimelineEvent;
}

//--------------------------------------------------------------
void Timeline::M_addColorToPalette(ofColor c){
	m_listColors.push_back( c );
}

//--------------------------------------------------------------
void Timeline::M_init(string filename, float interval)
{
	printf("Timeline::M_init(){\n");

	Poco::DateTime now = nowLocal();
	string nowPoco = Poco::DateTimeFormatter::format(now, __DATE_FORMAT__);
	printf("NOW=%s\n", nowPoco.c_str());

	// Parse animations from XML
	string strDateEnd = __EMPTY__;
	string strDateStart = __EMPTY__;
	int timezone=0;
	ofxXmlSettings *XML = new ofxXmlSettings(); 
	if (XML->loadFile(filename))
	{
		int nbTagDays = XML->getNumTags("day");
		if (nbTagDays>0){

			strDateStart = XML->getAttribute("day", "dateStart", __EMPTY__, 0);
			strDateEnd = XML->getAttribute("day", "dateEnd", __EMPTY__, nbTagDays-1);

			for(int i = 0; i < nbTagDays; i++)
			{
				printf("+ DAY %d\n", i);
				
				XML->pushTag("day", i);
				int nbTagEvents = XML->getNumTags("event");
				for(int j = 0; j < nbTagEvents; j++)
				{
					string date = XML->getAttribute("event", "date", __EMPTY__, j);
					if (date != "__EMPTY__"){

						Poco::DateTime datetime;
						Poco::DateTimeParser::parse(__DATE_FORMAT__, date, datetime, timezone);

						if (datetime>=now){
							string animationName = XML->getAttribute("event", "animation",		__EMPTY__, j);
							string animationArgs = XML->getAttribute("event", "animationArgs",	__EMPTY__, j);

							if (animationName != "__EMPTY__"){
								TimelineEvent* pTimelineEvent = new TimelineEvent(datetime,animationName);
								m_listEvents.push_back(pTimelineEvent);
							}
						}

						string datePoco = Poco::DateTimeFormatter::format(datetime, __DATE_FORMAT__);
						//printf("    - EVENT date=\"%s\", datePoco=\"%s\", year=%d [%s]\n", date.c_str(), datePoco.c_str(), datetime.year(), datetime>now ? "à venir" : "passé");
					}
				}

				XML->popTag();
			}
		}


		// add animations every (n) minutes
		if (strDateEnd!=__EMPTY__ && false /* Globals::instance()->mp_animationManager */){
			Poco::DateTime dateEnd;
			Poco::DateTimeParser::parse(__DATE_FORMAT__, strDateEnd, dateEnd, timezone);
			Poco::DateTime dateEvent = now;
//			M_addAnimationEvents(now,dateEnd);
		}
	}
	delete XML;


	printf("}\n");
}

//--------------------------------------------------------------
void Timeline::M_update()
{
/*
	if (!m_isActive) 
		return;
*/

	Poco::DateTime now = nowLocal();
	if (m_listEvents.size()>0)
	{
		TimelineEvent* pNextEvent = m_listEvents[0];
//		printf("now : %s - event:%s\n",dateNow.c_str(),dateEvent.c_str());
		if (pNextEvent->m_datetime <= now)
		{
			string dateEvent = Poco::DateTimeFormatter::format(pNextEvent->m_datetime, __DATE_FORMAT__);
			int typeEvent = pNextEvent->m_type;

			bool bTriggerAnim = true;
			// ========================================================================
			// ANIM CHANGE
			// ========================================================================
/*
			if (typeEvent == TimelineEvent::TYPE_ANIM_CHANGE)
			{
				// Scene index changing now
				// Look for active client ? = sound activity + network activity 
				Animation* pAnimation = Globals::instance()->mp_animationManager->M_getAnimationByName(pNextEvent->m_animationName);
				if (pAnimation->m_theme == Animation::THEME_SCENE)
				{
					// Change the index
					Globals::instance()->mp_osc->M_changeIndexClientActive();	
					
					// Is new active ? 
					OSCClient* pClientActive = Globals::instance()->mp_osc->M_getClientActive();
					
					// Anyone ? 
					int nbOscClients = Globals::instance()->mp_osc->M_getClientNb();
					
					// Not active or no clients... -> do something
					if ( (nbOscClients==0) || (pClientActive && pClientActive->M_isClientActive() == false))
					{
						// Dont trigger anim Scene if no sound on it
						bTriggerAnim = false;
					
						if (pClientActive)
							printf(">>> WARNING client %s INACTIVE...\n", pClientActive->m_ip.c_str());
						else
							printf(">>> WARNING NO CLIENTS...\n");
						
						printf(">>> WONT TRIGGER ANIM SCENE \n");

						// Replace next animation with theme=contemplation
						// Look for next event of type Animation
						TimelineEvent* pEventAfter = 0;
						for (int i=1;i<m_listEvents.size();i++){
							if (m_listEvents[i]->m_type == TimelineEvent::TYPE_ANIM_CHANGE){
								pEventAfter = m_listEvents[i];
								break;
							}
						}

						if (pEventAfter)
						{
							string animationNamePrev = pEventAfter->m_animationName;
						
							Animation* pAnimationContemplation = Globals::instance()->mp_animationManager->M_getAnimationByThemeRnd(Animation::THEME_CONTEMPLATION);
							if (pAnimationContemplation){
								pEventAfter->m_animationName = pAnimationContemplation->m_name;
								printf(">>> CHANGING next animation [%s] to [%s]\n", animationNamePrev.c_str(), pEventAfter->m_animationName.c_str());
							}
						}
					}
				}

				// Active ? fire an event to animation manager
				if (m_isActive && bTriggerAnim==true)
				{ 
					Globals::instance()->mp_animationManager->M_setAnimation(pNextEvent->m_animationName, pNextEvent->m_animationArgs);
					printf("ANIM CHANGE %s [date=%s]\n", pNextEvent->m_animationName.c_str(), dateEvent.c_str());
				}
			}
			
			// ========================================================================
			// COLOR CHANGE
			// ========================================================================
			else 
			if (typeEvent == TimelineEvent::TYPE_COLOR_CHANGE)
			{
				//Globals::instance()->mp_animationManager->M_setAnimation(pNextEvent->m_animationName, pNextEvent->m_animationArgs);
				if (pNextEvent->mp_eventNext){
					string dateEventNext = Poco::DateTimeFormatter::format(pNextEvent->mp_eventNext->m_datetime, __DATE_FORMAT__);

					Globals::instance()->mp_app->versatubesSetColorNext(pNextEvent->m_color,pNextEvent->mp_eventNext->m_datetime);
					printf("COLOR CHANGE [date=%s] [next=%s] [color=(%.1f,%.1f,%.1f)]\n", dateEvent.c_str(), dateEventNext.c_str(), (float)pNextEvent->m_color.r,(float)pNextEvent->m_color.g, (float)pNextEvent->m_color.b);
				}
			}
*/

			m_listEvents.erase(m_listEvents.begin());
			delete pNextEvent;
		}
	}
}

//--------------------------------------------------------------
void Timeline::M_setActive(bool is)
{
	m_isActive = is;
	if (is)
	{

		if (m_listEvents.size()>0)
		{
			Poco::DateTime now = nowLocal();
			vector<TimelineEvent*> listEventsDelete;

			string strNow = Poco::DateTimeFormatter::format(now, __DATE_FORMAT__);
			printf("Timeline::M_setActive() - NOW:%s\n",strNow.c_str());

			int index = 0;
			TimelineEvent* pNextEvent = m_listEvents[index];
			while(m_listEvents.size()>0 && pNextEvent->m_datetime <= now)
			{
				pNextEvent = m_listEvents[++index];
			}
			
			if (index > 1){
				index = index-1;
				for (int i=0;i<=index;i++){
					string strDate = Poco::DateTimeFormatter::format(m_listEvents[i]->m_datetime, __DATE_FORMAT__);
					printf("  - deleting event [date=%s]\n",strDate.c_str());
					delete m_listEvents[i];
				}
				m_listEvents.erase(m_listEvents.begin(), m_listEvents.begin()+index+1);
			}

		}
	}
}

//--------------------------------------------------------------
void Timeline::M_programFor(int nbHours)
{
	M_empty();
	M_getPropertiesJS();
	
	Poco::DateTime now = nowLocal();
	Poco::DateTime end = now + Poco::Timespan(0, nbHours, 0, 0, 0);
	m_deltaTime = end - now; 

	m_dateStart = now;
	m_dateEnd = end;

	M_addAnimationEvents(now,end);

	//M_sortEventByDate();
}

//--------------------------------------------------------------
void Timeline::M_programUntil(string strDateEnd)
{
	int timezone=0;
	Poco::DateTime dateEnd;
	Poco::DateTimeParser::parse(__DATE_FORMAT__, strDateEnd, dateEnd, timezone);

	Poco::DateTime now = nowLocal()+Poco::Timespan(5,0);
	m_deltaTime = dateEnd - now; 


	M_empty();
	M_getPropertiesJS();
	M_addAnimationEvents(nowLocal(),dateEnd);
}


//--------------------------------------------------------------
void Timeline::M_programColorChangeEvery(int nbSecondsInterval, string strDateEnd)
{
	int timezone=0;
	Poco::DateTime dateEnd;
	Poco::DateTimeParser::parse(__DATE_FORMAT__, strDateEnd, dateEnd, timezone);

	
	Poco::DateTime dateEventColor = nowLocal()+Poco::Timespan(5,0);
	ofColor c;
	TimelineEvent* pEventPrev=0;
	while(dateEventColor<dateEnd)
	{
		// TODO : pick colors in palette
		//c.set(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255),255);
		int indexColor = (int) ofRandom(0,m_listColors.size());
		if (indexColor>=m_listColors.size())
			indexColor = m_listColors.size()-1;


		
		TimelineEvent* pEvent = M_addEventColor(dateEventColor, m_listColors[indexColor] );		
		if (pEventPrev){
			pEventPrev->mp_eventNext = pEvent;
		}
		pEventPrev = pEvent;

		dateEventColor	+= Poco::Timespan(0,0,0,nbSecondsInterval,0);
//	Timespan(int days, int hours, int minutes, int seconds, int microseconds);
	}
}


//--------------------------------------------------------------
void Timeline::M_addAnimationEvents(Poco::DateTime begin, Poco::DateTime end)
{
	Poco::DateTime dateEvent = begin;
/*	while(dateEvent<end)
	{
		// ---- CYCLE ----
		// Scene

		Animation* pAnimationScene = Globals::instance()->mp_animationManager->M_getAnimationByName("typo_scene.js");
		if (pAnimationScene){
			M_addEvent(dateEvent,pAnimationScene->m_name);
			dateEvent += Poco::Timespan(m_cycleDurationScene,0);
		}
		
		// Ecoute
		Animation* pAnimationEcoute = Globals::instance()->mp_animationManager->M_getAnimationByThemeRnd( Animation::THEME_ECOUTE );
		if (pAnimationEcoute){
			M_addEvent(dateEvent,pAnimationEcoute->m_name);
			dateEvent += Poco::Timespan(m_cycleDurationEcoute,0);
		}

		// Contemplation
		int nbSecondsContemplation = m_cycleDuration - (m_cycleDurationScene+m_cycleDurationEcoute);
		Poco::DateTime dateContemplation	= dateEvent;
		Poco::DateTime dateContemplationEnd = dateContemplation+Poco::Timespan(nbSecondsContemplation,0);
		
		float durationCompteur			= m_cycleDurationCompteur;
		float durationAnimContemplation	= m_cycleDurationContemplation;
		Animation* pAnimationCompteur	= Globals::instance()->mp_animationManager->M_getAnimationByName( "typo_counter_remaining.js" );
		while(dateContemplation<dateContemplationEnd)
		{
			// Compteur
			M_addEvent(dateContemplation,pAnimationCompteur->m_name);
			dateContemplation += Poco::Timespan(m_cycleDurationCompteur,0);
			
			
			// Contemplation
			Animation* pAnimationContemplation	= Globals::instance()->mp_animationManager->M_getAnimationByThemeRnd( Animation::THEME_CONTEMPLATION );
			M_addEvent(dateContemplation,pAnimationContemplation->m_name);
			dateContemplation += Poco::Timespan(m_cycleDurationContemplation,0);

			// Add to date event
			dateEvent	+= Poco::Timespan(m_cycleDurationCompteur+m_cycleDurationContemplation,0);
		}

		// ---- CYCLE FIN ----
	}
*/
}


//--------------------------------------------------------------
void Timeline::M_getPropertiesJS()
{
	// Javascript properties
	ofxJSValue retVal;
	ofxJSValue args[1];
	
	args[0] = string_TO_ofxJSValue(string("d")); ofxJSCallFunctionNameGlobal("getTimelineCycle", args, 1, retVal);
	m_cycleDuration = ofxJSValue_TO_float(retVal);

	args[0] = string_TO_ofxJSValue(string("dScene")); ofxJSCallFunctionNameGlobal("getTimelineCycle", args, 1, retVal);
	m_cycleDurationScene = ofxJSValue_TO_float(retVal);

	args[0] = string_TO_ofxJSValue(string("dEcoute")); ofxJSCallFunctionNameGlobal("getTimelineCycle", args, 1, retVal);
	m_cycleDurationEcoute = ofxJSValue_TO_float(retVal);

	args[0] = string_TO_ofxJSValue(string("dCompteur")); ofxJSCallFunctionNameGlobal("getTimelineCycle", args, 1, retVal);
	m_cycleDurationCompteur = ofxJSValue_TO_float(retVal);

	args[0] = string_TO_ofxJSValue(string("dContemplation")); ofxJSCallFunctionNameGlobal("getTimelineCycle", args, 1, retVal);
	m_cycleDurationContemplation = ofxJSValue_TO_float(retVal);
}

//--------------------------------------------------------------
void Timeline::M_print()
{
	printf("----------------------------------------\n");
	vector<TimelineEvent*>::iterator it;
	for (it = m_listEvents.begin(); it != m_listEvents.end(); ++it){
		string datePoco = Poco::DateTimeFormatter::format((*it)->m_datetime, __DATE_FORMAT__);
		printf("+ EVENT type=%d date=\"%s\" [%s]\n", (*it)->m_type, datePoco.c_str(), (*it)->m_animationName.c_str());
	}
	printf("----------------------------------------\n");
}


//--------------------------------------------------------------
int Timeline::M_getElapsedSeconds()
{
	Poco::DateTime now = nowLocal();
	if (now > m_dateStart){
		Poco::Timespan delta = now-m_dateStart;
		return delta.totalSeconds();
	}
	return 0;
}

//--------------------------------------------------------------
int Timeline::M_getRemainingSeconds()
{
	int total = M_getDurationSeconds();
	if (total>0){
		return total-M_getElapsedSeconds();
	}
	return 0;
}

//--------------------------------------------------------------
void Timeline::M_sortEventByDate()
{
	sort(m_listEvents.begin(), m_listEvents.end(), Timeline::sM_compareEvent);
}

//--------------------------------------------------------------
bool Timeline::sM_compareEvent(TimelineEvent* pA,TimelineEvent* pB)
{
	return (pA->m_datetime < pB->m_datetime);
}


