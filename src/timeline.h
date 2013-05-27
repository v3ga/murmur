/*
 *  timeline.h
 *  versatubes
 *
 *  Created by 2Roqs on 04/05/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "animations.h"
#include "utils.h"

class TimelineEvent
{
	public:
		TimelineEvent			();
		~TimelineEvent			();
	
		TimelineEvent			(Poco::DateTime datetime, string animationName="", string animationArgs="");

		void					M_zeroAll	();
		Poco::DateTime			m_datetime;
		string					m_animationName;
		string					m_animationArgs;

		ofColor					m_color;
		int						m_type;
		
		TimelineEvent*			mp_eventNext;

		enum{
			TYPE_ANIM_CHANGE		= 0,
			TYPE_COLOR_CHANGE		= 1
		};
};


class Timeline
{
	public:
		Timeline				();
		~Timeline				();


		vector<TimelineEvent*>	m_listEvents;

		void					M_empty						();
		void					M_sortEventByDate			();
		void					M_addEvent					(Poco::DateTime datetime, string animationName="", string animationArgs="");
		void					M_addEvent					(Poco::DateTime datetime, Animation*, string animationArgs="");
		TimelineEvent*			M_addEventColor				(Poco::DateTime datetime, ofColor c);
		void					M_init						(string filename, float interval);
		void					M_update					();
		void					M_setActive					(bool is=true);
		void					M_programFor				(int nbHours);
		void					M_programUntil				(string strDateEnd);
		void					M_programColorChangeEvery	(int nbSecondsInterval,string strDateEnd);
		int						M_getDurationSeconds		(){return m_deltaTime.totalSeconds();}
		int						M_getElapsedSeconds			();
		int						M_getRemainingSeconds		();
		void					M_getPropertiesJS			();

		void					M_addColorToPalette			(ofColor c); // TODO : pas à sa place ici ...

		void					M_print						();
		static bool				sM_compareEvent				(TimelineEvent* pA,TimelineEvent* pB);

		bool					m_isActive;
		Poco::Timespan			m_deltaTime;
		Poco::DateTime			m_dateStart,m_dateEnd;
	
		// Cycle variables
		float					m_cycleDuration;
		float					m_cycleDurationScene;
		float					m_cycleDurationEcoute;
		float					m_cycleDurationCompteur;
		float					m_cycleDurationContemplation;
		
		// Color palettes
		vector<ofColor>			m_listColors;

		// Constants
		static string			__EMPTY__;
		static string			__DATE_FORMAT__;
		
	private:
		void					M_addAnimationEvents	(Poco::DateTime begin, Poco::DateTime end);

};


