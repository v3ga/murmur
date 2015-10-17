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
		virtual	void			loadConfiguration		(string filename);

		void					setVideo				(string name);
		void					setDisplayMode			(string name);
 
		ofxHapPlayer			m_player;
 		bool					m_bLoadVideo;
		bool					m_bHasLoadedVideo;
 
 
		ofxUITextInput*			mp_teVideoPath;
		string					m_videoPath;
 
		vector<string>			m_displayMode;
		string					m_displayModeCurrent;

		float					m_alpha;
 
		int						m_nbFramesBeforeDrawVideo;
		int						m_nbFrames;
		bool					m_bCanDrawVideo;
};

