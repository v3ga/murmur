//
//  animationGridsFF.h
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#pragma once
#include "animations.h"
#include "blur.h"

class AnimationShaderWave;
class AnimationGridsFF : public Animation
{
	public:
		AnimationGridsFF			(string name);
		~AnimationGridsFF			();
 
 
		virtual	void			createUICustom			();
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
		virtual void			onNewPacket				(DevicePacket* pDevicePacket, string deviceId, float x, float y);
		virtual	void			guiEvent				(ofxUIEventArgs &e);
 
 	protected:
 
		void					computeMesh				(float w, float h);
		void					loadShader				();

 		ofVboMesh				m_meshGrid;
		float					m_meshGridRatio; // relative to screen
		bool					m_bCreateMeshGrid;
		int						m_meshGridColumns, m_meshGridRows;

		ofVec3f					m_pos;
		bool					m_bSquare;
 
		float					m_w, m_h;
 
		ofShader				m_shader;
		bool					m_bReloadShader;
 
		ofFbo					m_fbo;
		ofImage					m_img;
		blur					m_blur;
 
		bool					m_bDebug;
 
		float					m_colorFromDeviceLuminance;
		int						m_speed;
		float					m_amplitude;
		float					m_rotationX;
		float					m_blurAmount;
		int						m_blurNbPasses;

		AnimationShaderWave*	mp_animShaderWave;
};
