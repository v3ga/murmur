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


        ofFloatImage            m_imgSoundInput;
        ofVec2f                 m_anchor,m_anchorNorm;
        float                   m_volume;
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
        virtual void            onNewPacket             (DevicePacket*, string deviceId, float xNorm, float yNorm);
		virtual	void			createUICustom			();

        ofFloatImage            m_imgSoundInput;
        ofVec2f                 m_anchor,m_anchorNorm;
        float                   m_volume;
		float					m_waveIntensity;
		bool					m_isBlend;
	
	private:
		map<string, ShaderWave*>	m_mapShaderWaves;
};