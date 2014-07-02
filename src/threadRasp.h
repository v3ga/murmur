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
		void addCommand(string command_);

		vector<string>	m_commands;
};