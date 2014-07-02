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

void threadRasp::addCommand(string command_)
{
	m_commands.push_back( command_ );
}

void threadRasp::threadedFunction()
{
	if (m_commands.size()==0) return;
	
    printf("Launching Raspberry thread with : \n");
	vector<string>::iterator it;
	for (it = m_commands.begin() ; it != m_commands.end() ; ++it){
		string command = (*it);
		printf("  - %s\n", command.c_str());

		system(command.c_str());
		sleep( 1000 );
	}
    printf("Stopping Raspberry thread\n");
}


