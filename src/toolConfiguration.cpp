//
//  toolConfiguration.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolConfiguration.h"
#include "globals.h"
#include "testApp.h"

//--------------------------------------------------------------
toolConfiguration::toolConfiguration(toolManager* parent) : tool("Configuration", parent)
{
	isShowDevicePointSurfaces	= false;
	isViewSimulation			= false;
	m_isLaunchMadMapper			= false;
	m_isLaunchDevices			= false;

	mp_tgViewSimu				= 0;
}

//--------------------------------------------------------------
toolConfiguration::~toolConfiguration()
{
	vector<threadRasp*>::iterator it;
	for (it = m_listThreadLaunchDevices.begin(); it!=m_listThreadLaunchDevices.end(); ++it)
	{
		(*it)->waitForThread(true);
		delete (*it);
	}
}
//--------------------------------------------------------------
void toolConfiguration::createControlsCustom()
{
	if (mp_canvas)
	{
		float dim = 16;
		int widthDefault = 320;

	    mp_canvas->addToggle("Show device points", isShowDevicePointSurfaces, dim, dim);
    	mp_tgViewSimu = mp_canvas->addToggle("View simulation", isViewSimulation, dim, dim);

    	mp_canvas->addWidgetDown(new ofxUIToggle("launch madmapper @ start", false, dim, dim));
    	mp_canvas->addWidgetDown(new ofxUIToggle("launch murmur @ rasp", false, dim, dim));

		mp_canvas->autoSizeToFitWidgets();
		mp_canvas->setVisible(false);
	}
}

//--------------------------------------------------------------
void toolConfiguration::setup()
{
	if (!GLOBALS->mp_app->isSimulation)
	{
		if (m_isLaunchMadMapper)
		{
			launchMadMapper();
		}
	
		// Launch Murmur on Raspberry
		if (m_isLaunchDevices)
		{
			launchDevices();
		}
	}
}

//--------------------------------------------------------------
void toolConfiguration::launchDevices()
{
	printf("[testApp::launchDevices]\n");
	int nbLaunchDevices = GLOBALS->mp_app->m_settings.getNumTags("murmur:launchDevices:ip");
	for (int i=0;i<nbLaunchDevices;i++)
	{
		string strIPMurmur 	= GLOBALS->mp_app->m_settings.getValue("murmur:launchDevices:ip", "10.23.108.114", i);
		string strSSH_kill 	= "ssh pi@" + strIPMurmur + " 'sudo pkill -f murmurRaspberry'";
		string strSSH_run 	= "ssh pi@" + strIPMurmur + " /home/pi/Dev/C/openFrameworks/examples/myapps/murmurRaspberry/bin/run_murmur.sh";
		
		threadRasp* pThreadLaunchDevice = new threadRasp();
		pThreadLaunchDevice->addCommand(strSSH_kill);
		pThreadLaunchDevice->addCommand(strSSH_run);
		pThreadLaunchDevice->startThread();
		
		m_listThreadLaunchDevices.push_back(pThreadLaunchDevice);
	}
}

//--------------------------------------------------------------
void toolConfiguration::launchMadMapper()
{
	string pathFileMM = GLOBALS->mp_app->m_settings.getValue("murmur:madmapper", "default.map");
	ofFile file(ofToDataPath("Config/madmapper/"+pathFileMM));
	if (file.exists())
	{
		string command = "open "+file.getAbsolutePath();
		system(command.c_str());
	   printf("Launching MadMapper with %s\n", file.getAbsolutePath().c_str());
	}
	else
	{
	   printf("Error launching MadMapper with %s\n", file.getAbsolutePath().c_str());
	}
}


//--------------------------------------------------------------
void toolConfiguration::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();

	if (name == "View simulation")
    {
        isViewSimulation = ((ofxUIToggle *) e.widget)->getValue();
    }
    else if (name == "Show device points")
    {
        GLOBALS->mp_app->isShowDevicePointSurfaces = ((ofxUIToggle *) e.widget)->getValue();
    }
	else if (name == "launch madmapper @ start")
	{
		m_isLaunchMadMapper = ((ofxUIToggle *) e.widget)->getValue();
	}
	else
	if (name == "launch murmur @ rasp")
	{
		m_isLaunchDevices = ((ofxUIToggle *) e.widget)->getValue();
	}

}
