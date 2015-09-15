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
	m_bLoadVideo = false;
}

//--------------------------------------------------------------
AnimationVideo::~AnimationVideo()
{
}

//--------------------------------------------------------------
void AnimationVideo::setVideo(string name)
{
	m_player.loadMovie(name);
	m_player.play();
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
		setVideo("Video/A_drone_in_iceland_hap.mov");
	}

	ofBackground(0);
	
	ofRectangle rectSurface(0,0,w,h);
	ofRectangle rectVideo(0,0,m_player.getWidth(),m_player.getHeight());
	rectVideo.scaleTo(rectSurface);

//	m_player.draw(rectVideo.x,rectVideo.y,rectVideo.width,rectVideo.height);
	m_player.draw(0,0,w,h);
}

