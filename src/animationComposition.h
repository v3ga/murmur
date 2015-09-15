//
//  animationComposition.h
//  murmur
//
//  Created by Julien on 25/06/2015.
//
//

#pragma once
#include "animations.h"
#include "surface.h"

class AnimationCompoConfiguration
{
	public:
	
		string					m_animationName1, m_animationName2;
};


class AnimationComposition : public Animation
{
	public:
		AnimationComposition							(string name);


		virtual void			add						(string name);
        virtual void            loadProperties          (string id);
		virtual	void			createUICustom			();
		virtual void			VM_enter				();
		virtual void			VM_update				(float dt);
		virtual void			VM_draw					(float w, float h);
		virtual void			VM_exit					();
 
		static	void			onSurfaceRenderOffscreen(Surface*, AnimationComposition* pThis);


        virtual void            onNewPacket             (DevicePacket*, string deviceId, float x, float y);

	protected:
		vector<Animation*>						m_animations;
		vector<AnimationCompoConfiguration>		m_configurations;

		ofFbo									m_fboAnimation1, m_fboAnimation2;
 
		ofShader								m_shader;
};
