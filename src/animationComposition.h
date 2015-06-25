//
//  animationComposition.h
//  murmur
//
//  Created by Julien on 25/06/2015.
//
//

#pragma once
#include "animations.h"

class AnimationComposition : public Animation
{
	public:
		AnimationComposition			(string name);


		virtual void			add						(string name);
        virtual void            loadProperties          (string id);
		virtual	void			createUICustom			();
		virtual void			VM_enter				();
		virtual void			VM_update				(float dt);
		virtual void			VM_draw					(float w, float h);


        virtual void            onNewPacket             (DevicePacket*, string deviceId, float x, float y);

	protected:
		vector<Animation*>		m_animations;

};
