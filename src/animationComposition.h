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

struct AnimationCompoConfiguration
{
		AnimationCompoConfiguration(string name, string anim1,string config1, string anim2, string config2)
		{
			m_name				= name;
		
			m_animationName1 	= anim1;
			m_animationConfig1	= config1;

			m_animationName2 	= anim2;
			m_animationConfig2	= config2;
		}
 
		string					m_name;

		string					m_animationName1;
		string					m_animationConfig1;
 
		string 					m_animationName2;
		string					m_animationConfig2;
};


class AnimationComposition : public Animation
{
	public:
		AnimationComposition							(string name);
		~AnimationComposition							();

				void			setRenderNormal			(bool is=true);
		virtual	Animation*		getAnimation			(string name);
		virtual void			addAnimation			(string name);
		virtual	void			setBlending				(string name);
		virtual	void			setComposition			(string name);
		virtual	void			readSurfaceSettings		(ofxXmlSettings& settings);
        virtual void            loadProperties          (string id);
		virtual	void			loadMidiSettings		();
		virtual	void			loadConfiguration		(string filename);
		virtual	void			createUICustom			();
		virtual void			VM_enter				();
		virtual void			VM_update				(float dt);
 		virtual void			VM_drawBefore			(float w, float h);
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
		vector<AnimationCompoConfiguration*>	m_compositions;
		string									m_compositionCurrent, m_compositionWanted;

		ofFbo									m_fboAnimation1, m_fboAnimation2;
 
		bool									m_bRenderNormal; // without blending shaders
		bool									m_bRenderNormalWanted;
		ofShader								m_shader;
 
		void									allocateRenderBuffers();
		bool									m_bAllocateRenderBuffers;
 
		ofxUIRadio*								mp_radioCompositions;
		ofxUIRadio*								mp_radioBlending;
		string									m_nameBlending;
 
		bool									isNameComposition(string name);
		bool									m_isLoadingConfiguration;
 
		string									m_strFragBlendFunctions;
		string									getShaderSourceWithBlendFunctions(string filename);
 
		vector<string>							m_blendings;
		bool									isNameBlending(string name);
};
