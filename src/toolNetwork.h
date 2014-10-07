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

		static	void			logMessageOSC		(const ofxOscMessage& message, bool isIn=true);
		void					logMessage			(const ofxOscMessage& message, bool isIn=true);


        oscReceiver         	m_oscReceiver;
        string              	m_oscReceiverIP;
        int                 	m_oscReceiverPort;
	
		bool					m_isLogEnable;
		bool					m_isLogMessagesIn, m_isLogMessagesOut;
	
	protected:
		ofxUITextArea*			mp_consoleOsc;

		vector<string>			m_logMessagesOSC;
		int						m_logMessagesOSCMax;
		string					m_logConsole;
	
		static toolNetwork*		smp_this;

};
