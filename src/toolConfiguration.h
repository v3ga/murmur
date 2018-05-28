//
//  toolConfiguration.h
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#pragma once

#include "tool.h"
#include "threadRasp.h"

class toolConfiguration : public tool
{
	public:

		toolConfiguration			(toolManager* parent);
		~toolConfiguration			();

	
		void					createControlsCustom();
		void					setup				();
		void					launchMadMapper		();
		void					launchDevices		();
		void					setFullscreen		(bool is=true);
		void					showToolWindow		(bool is);
		void					toggleFullscreen	();
		void					setViewSimulation	(bool is=true);
		void					toggleViewSimulation();
		void					updateUI			();
		void					handleEvents		(ofxUIEventArgs& e);


		bool					isShowDevicePointSurfaces;
		bool					isViewSimulation;
		bool					m_isLaunchMadMapper;
		bool					m_isLaunchDevices;
		threadRasp				m_threadRasp;
		vector<threadRasp*>		m_listThreadLaunchDevices;

        ofxUIToggle*        	mp_tgViewSimu;
        ofxUIToggle*        	mp_tgFullscreen;
        ofxUIToggle*        	mp_tgHideToolWindow;
	
		// bool					m_isFullscreen;

};
