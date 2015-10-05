//
//  animationLinesFF.h
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#pragma once

#include "animations.h"

class LineFFElement;
class AnimationLinesFF : public Animation
{
	public:
		AnimationLinesFF			(string name);
		~AnimationLinesFF			();
 
 
		virtual	void			createUICustom			();
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
		virtual void            onNewPacket             (DevicePacket*, string deviceId, float x, float y);
		virtual	void			onVolumAccumEvent		(string deviceId);

 
		vector<LineFFElement*>	m_lines;
 
		ofCamera				m_cam;
		
		float                   m_zMax;
		float					m_dirSpeed;
		float					m_rot,m_rotSpeed;
		float					m_radius;
 
		bool					m_bRadiusDirect;
 
		float					m_w,m_h;
 
		ofMesh					m_meshPlane;
 
		ofColor					m_lastPacketColor;
};
