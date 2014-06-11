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
		void					handleEvents		(ofxUIEventArgs& e);


		bool					isShowDevicePointSurfaces;
		bool					isViewSimulation;
		bool					m_isLaunchMadMapper;
		bool					m_isLaunchDevices;
		threadRasp				m_threadRasp;
		vector<threadRasp*>		m_listThreadLaunchDevices;

        ofxUIToggle*        	mp_tgViewSimu;

};
