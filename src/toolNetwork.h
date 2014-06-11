//
//  toolNetwork.h
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#pragma once
#include "tool.h"
#include "oscReceiver.h"

class toolNetwork : public tool
{
	public:

		toolNetwork				(toolManager* parent);

	
		void					createControlsCustom();
		void					setup				();
		void					update				();
		void					handleEvents		(ofxUIEventArgs& e);


        oscReceiver         	m_oscReceiver;
        string              	m_oscReceiverIP;
        int                 	m_oscReceiverPort;
};
