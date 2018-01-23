//
//  animationDebug.hpp
//  murmur
//
//  Created by Julien on 18/01/2018.
//
//

#pragma once

#include "animations.h"

class AnimationDebug : public Animation
{
	public:
		AnimationDebug			(string name);


		void            		createUICustom          ();
		void					VM_update				(float dt);
		void					VM_draw					(float w, float h);
        void            		guiEvent                (ofxUIEventArgs &e);

		void            		onNewPacket				(DevicePacket*, string deviceId, float xNorm, float yNorm);


	private:
		float					m_volume;
		float					m_pitch;

};
