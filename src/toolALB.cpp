//
//  toolALB.cpp
//  murmur
//
//  Created by Julien on 23/01/2015.
//
//

#include "toolALB.h"
#include "globals.h"
#include "surface.h"
#include "ofxJSONElement.h"

//--------------------------------------------------------------
toolALB::toolALB(toolManager* pParent) : tool("ALB", pParent)
{
	m_bRecording = false;
}

//--------------------------------------------------------------
void toolALB::createControlsCustom()
{
	if (mp_canvas)
	{
		ofxUIWidgetFontType fontType = OFX_UI_FONT_SMALL;
		float dim = 16;
		int widthDefault = 320;

	    mp_canvas->addWidgetDown( new ofxUILabel("ALB",OFX_UI_FONT_LARGE) );
    	mp_canvas->addWidgetDown( new ofxUISpacer(widthDefault, 2) );

		mp_btnRecord = new ofxUILabelButton("Record", false, 100,dim);
		mp_canvas->addWidgetDown( mp_btnRecord );

		mp_canvas->autoSizeToFitWidgets();
	}
}

//--------------------------------------------------------------
void toolALB::setup()
{
	OFAPPLOG->begin("toolALB::setup()");
	m_recorder.load(ofToDataPath("Exports/ALB/export.txt"));
	OFAPPLOG->println(" - found "+ofToString( m_recorder.getItems().size() )+" items");
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void toolALB::update()
{
	if (m_bRecording)
	{
//		string data = getData();
//		m_recorder.add( getData() , ofGetElapsedTimeMillis());
		Surface* pSurface = GLOBALS->getSurfaceMain();
		if (pSurface)
		{
			ofFbo& offscreen = pSurface->getOffscreen();
			if (m_imageOffscreen.getWidth() != offscreen.getWidth() || m_imageOffscreen.getHeight() != offscreen.getHeight() )
				m_imageOffscreen.allocate((int)offscreen.getWidth(),(int)offscreen.getHeight(),OF_IMAGE_COLOR);

			pSurface->getOffscreen().readToPixels( m_imageOffscreen );

			m_videoRecorder.addFrame( m_imageOffscreen.getPixelsRef() );
		}
	}
}

//--------------------------------------------------------------
void toolALB::handleEvents(ofxUIEventArgs& e)
{
	string name = e.getName();
	if (name == "Record")
	{
		if (mp_btnRecord->getValue())
		{
			m_bRecording = !m_bRecording;
			if (m_bRecording)
			{
				startRecording();
			}
			else
			{
				endRecording();
			}

			// Update UI
			mp_btnRecord->getLabelWidget()->setLabel( m_bRecording? "Stop" : "Record" );
		}
	
//		ofLog() << "m_bRecording=" << ofToString(m_bRecording);

	}
}

//--------------------------------------------------------------
void toolALB::startRecording()
{
	Surface* pSurface = GLOBALS->getSurfaceMain();
	if (pSurface)
	{
		ofResetElapsedTimeCounter();
		ofGetElapsedTimeMillis();


		ofFbo& offscreen = pSurface->getOffscreen();

		m_videoRecorder.setVideoCodec("mpeg4");
    	m_videoRecorder.setVideoBitrate("4000k");
	//vidRecorder.setAudioCodec("mp3");
	//vidRecorder.setAudioBitrate("192k");
		m_videoRecorder.setFfmpegLocation( ofFilePath::getAbsolutePath("Video/ffmpeg", true) );
	//ofLog() << ofFilePath::getAbsolutePath("Video/ffmpeg", true);
		m_videoRecorder.setup("Exports/ALB/video.mov", offscreen.getWidth(), offscreen.getHeight(), 30);
	}
}

//--------------------------------------------------------------
void toolALB::endRecording()
{
//	m_recorder.save("Exports/ALB/export.txt");
	m_videoRecorder.close();
}

//--------------------------------------------------------------
string toolALB::getData()
{
	DeviceManager* 		pDeviceManager 		= GLOBALS->mp_deviceManager;
	if (pDeviceManager)
	{
		vector<Device*>& listDevices = pDeviceManager->m_listDevices;
		int nbDevices=listDevices.size();

		ofxJSONElement json;
		for (int i=0;i<nbDevices;i++)
		{
			Device* pDevice = listDevices[i];

			// Device packets
			int nbPackets = pDevice->m_listPackets.size();
			DevicePacket* pDeviceLastPacket = pDevice->m_listPackets[nbPackets-1];

			// Data
			ofxJSONElement jsonDevice;
			jsonDevice["last_packet_volume"] = pDeviceLastPacket->m_volume;

			
			json[pDevice->getID()] = jsonDevice;
		}

		
		
		return json.getRawString(false);
	}
//	AnimationManager*	pAnimationManager	= GLOBALS->mp_
//	pDeviceManager->m_listDevices
}





