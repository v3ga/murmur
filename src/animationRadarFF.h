//
//  animationRadarFF.h
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#pragma once
#include "animations.h"
#include "RadarFFElement.h"


class AnimationRadarFF : public Animation
{
	public:
		AnimationRadarFF			(string name);
		~AnimationRadarFF			();
 
		void					createUICustom			();
 
        virtual void			VM_update				(float dt);
        virtual void			VM_draw					(float w, float h);
        virtual void            onNewPacket             (DevicePacket*, string deviceId, float x, float y);

		virtual	void			onVolumAccumEvent		(string deviceId);

 		float					m_ageMax;

	private:
		vector<RadarFFElement*> m_elements;
 
		float					m_w,m_h;
 
 		float					m_speed;
 
		float					m_filled_radiusWidth;
		float					m_filled_bradiusRandom;
 
		float					m_dotted_radiusWidth;

		float					m_radial_radiusWidth;
};
