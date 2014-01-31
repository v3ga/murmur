//
//  threadRasp.cpp
//  murmur
//
//  Created by Julien on 14/01/2014.
//
//

#include "threadRasp.h"

threadRasp::threadRasp() : ofThread()
{
}

void threadRasp::setCommand(string command_)
{
	m_command = command_;
}

void threadRasp::threadedFunction()
{
	if (m_command=="") return;
	
    printf("Launching Raspberry thread with %s\n", m_command.c_str());
	system(m_command.c_str());
    printf("Stopping Raspberry thread\n");
}


