//
//  animationWaveShader.h
//  murmur
//
//  Created by Julien on 16/05/13.
//
//

#pragma once
#include "animations.h"


class ShaderWave
{
	public:
	
		ShaderWave				();

		void					update				(float dt);
		void					setSize				(int w);
		void					clearSoundInput		();

       	ofFloatImage            m_imgSoundInput;
		ofVec2f                	m_anchor,m_anchorNorm;
        float                  	m_volume;

		ofFloatColor			m_colorWhite;
		ofFloatColor			m_colorDevice;
		ofFloatColor			m_color, m_colorTarget;
		float					m_fColor;
};

class AnimationShaderWave : public Animation
{
    public:
    
        AnimationShaderWave(string name);
		~AnimationShaderWave();
 
        virtual	void			VM_enter				();
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
        virtual	void			VM_exit					();
		virtual	void			onVolumAccumEvent		(string deviceId);
        virtual void            onNewPacket             (DevicePacket*, string deviceId, float xNorm, float yNorm);
		virtual	void			createUICustom			();
		virtual	void			guiEvent				(ofxUIEventArgs &e);
				void			setSizeVolumeTexture	();

        ofFloatImage            m_imgSoundInput;
        ofVec2f                 m_anchor,m_anchorNorm;
        float                   m_volume;
		float					m_waveIntensity;
		bool					m_isBlend;
		int						m_waveVolumeTexture;
	
	private:
		map<string, ShaderWave*>	m_mapShaderWaves;
		map<string, bool>			m_mapVolMeanAbove;


};