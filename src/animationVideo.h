//
//  animationVideo.h
//  murmur
//
//  Created by Julien on 10/09/2015.
//
//

#pragma once
#include "animations.h"
#include "ofxHapPlayer.h"

class AnimationVideo : public Animation
{
	public:
		AnimationVideo			(string name);
		~AnimationVideo			();
 

 
 		virtual void           	createUICustom          ();
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
		virtual	void			VM_enter				();
		virtual	void			VM_exit					();
        virtual void            guiEvent                (ofxUIEventArgs &e);

		void					setVideo				(string name);
 
		ofxHapPlayer			m_player;
 		bool					m_bLoadVideo;
		bool					m_bHasLoadedVideo;
 
 
		ofxUITextInput*			mp_teVideoPath;
		string					m_videoPath;
 
};

