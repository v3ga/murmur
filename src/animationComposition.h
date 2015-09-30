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
 


				void			setRenderNormal			(bool is=true);
		virtual void			add						(string name);
        virtual void            loadProperties          (string id);
		virtual	void			loadMidiSettings		();
		virtual	void			createUICustom			();
		virtual void			VM_enter				();
		virtual void			VM_update				(float dt);
		virtual void			VM_draw					(float w, float h);
		virtual void			VM_exit					();
 
		static	void			onSurfaceRenderOffscreen(Surface*, AnimationComposition* pThis);
				void			onSurfaceOffscreenChanged(ofVec2f& dim);

        virtual void            onNewPacket             (DevicePacket*, string deviceId, float x, float y);

		virtual	void			handleMidiMessages		();
		virtual void 			newMidiMessage			(ofxMidiMessage& eventArgs);

		virtual	void			guiEvent				(ofxUIEventArgs &e);
 
	protected:
		vector<Animation*>						m_animations;
		vector<AnimationCompoConfiguration>		m_compositions;

		ofFbo									m_fboAnimation1, m_fboAnimation2;
 
		bool									m_bRenderNormal; // without blending shaders
		ofShader								m_shader;
 
		void									allocateRenderBuffers();
		bool									m_bAllocateRenderBuffers;
};
