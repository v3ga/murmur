//
//  threadRasp.h
//  murmur
//
//  Created by Julien on 14/01/2014.
//
//

#pragma once

#include "ofMain.h"

class threadRasp : public ofThread
{
	public:
		threadRasp	();
	
		void threadedFunction();
		void setCommand(string command_);

		string	m_command;
};