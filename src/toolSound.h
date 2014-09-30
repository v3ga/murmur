//
//  toolSound.h
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#pragma once

#include "tool.h"



class SoundPlayer;
class toolSound : public tool
{
	public:

		toolSound				(toolManager* parent);

	
		void					createControlsCustom	();
		void					update					();
		void					handleEvents			(ofxUIEventArgs& e);
		void 					show					(bool is);
		void					loadData				();
		void					saveData				();
		void					updateLayout			();
		string					getSoundInfosPathFile	();
	
		ofxUICanvas*			mp_canvasSound;
		ofxUILabel*				mp_lblSoundTitle;
		ofxUITextInput*			mp_teSoundTags;
		ofxUILabel*				mp_lblSoundInfos;
		ofxUILabel*				mp_lblSoundInfos2;

		SoundPlayer*			mp_soundCurrent;
	
		bool					m_isUpdateLayout;
};
