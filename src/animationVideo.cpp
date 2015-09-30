//
//  animationVideo.cpp
//  murmur
//
//  Created by Julien on 10/09/2015.
//
//

#include "animationVideo.h"

//--------------------------------------------------------------
AnimationVideo::AnimationVideo(string name) : Animation(name)
{
	m_bLoadVideo 		= false;
	m_bHasLoadedVideo	= false;
	mp_teVideoPath		= 0;
}

//--------------------------------------------------------------
AnimationVideo::~AnimationVideo()
{
}

//--------------------------------------------------------------
void AnimationVideo::createUICustom()
{
	if (mp_UIcanvas)
	{
		mp_teVideoPath = new ofxUITextInput("teVideoPath", "", 200, 16);
		mp_teVideoPath->setAutoUnfocus(true);
		mp_teVideoPath->setAutoClear(false);
	
		mp_UIcanvas->addWidgetDown(mp_teVideoPath);
		
		ofxUILabelButton* pBtnLoad = new ofxUILabelButton("btnLoadVideo", 100, false, OFX_UI_FONT_SMALL);
		mp_UIcanvas->addWidgetRight(pBtnLoad);
		pBtnLoad->getLabelWidget()->setLabel("Load");
	}
}

//--------------------------------------------------------------
void AnimationVideo::setVideo(string name)
{
	m_bHasLoadedVideo = m_player.loadMovie(name);
	if (m_bHasLoadedVideo)
		m_player.play();
}

//--------------------------------------------------------------
void AnimationVideo::VM_enter()
{
	if (m_bHasLoadedVideo)
		m_player.play();
}

//--------------------------------------------------------------
void AnimationVideo::VM_exit()
{
	if (m_bHasLoadedVideo)
		m_player.stop();
}

//--------------------------------------------------------------
void AnimationVideo::VM_update(float dt)
{
	m_player.update();
}

//--------------------------------------------------------------
void AnimationVideo::VM_draw(float w, float h)
{
	if (!m_bLoadVideo)
	{
		m_bLoadVideo = true;
		setVideo(m_videoPath);
	}

	
	if (m_bHasLoadedVideo)
	{
		ofRectangle rectSurface(0,0,w,h);
		ofRectangle rectVideo(0,0,m_player.getWidth(),m_player.getHeight());
		rectVideo.scaleTo(rectSurface);

		m_player.draw(0,0,w,h);
	}
	else
	{
		ofBackground(0);
	}
}

//--------------------------------------------------------------
void AnimationVideo::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);

	string name = e.getName();
	int kind = e.getKind();

	if (kind == OFX_UI_WIDGET_TEXTINPUT)
	{
		if (name == "teVideoPath")
		{
			if (mp_teVideoPath->getTextString() != "")
			{
				m_videoPath = "Video/"+mp_teVideoPath->getTextString();
				
				ofLog() << m_videoPath;

				m_bLoadVideo = false;
				m_bHasLoadedVideo = false;
			}
		}
	}
	else
	if (kind == OFX_UI_WIDGET_LABELBUTTON)
	{
	   if (name == "btnLoadVideo")
	   {
			if (e.getButton()->getValue()>0)
			{
				ofFileDialogResult result = ofSystemLoadDialog("Load video", true, ofToDataPath("Video",true));
				//ofLog() << " path= " << result.getPath();
				//ofLog() << " name= " << result.getName();
				
				if (mp_teVideoPath && result.getName() != "")
				{
					m_videoPath = "Video/"+result.getName();
					mp_teVideoPath->setTextString( result.getName() );
					m_bLoadVideo = false;
					m_bHasLoadedVideo = false;
				}
			}
	   }
	}
}


