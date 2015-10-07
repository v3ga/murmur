//
//  floatRelax.h
//  murmur
//
//  Created by Julien on 07/10/2015.
//
//

#pragma once

class floatRelax
{
	public:
			float	m_value;
			float	m_valueTarget;
			float	m_relaxation;
 
			floatRelax()
			{
				m_value 		= 0.0f;
				m_valueTarget 	= 0.0f;
				m_relaxation 	= 0.96f;
			}
 
			floatRelax(float v,float target, float rel=0.8f)
			{
				m_value 		= v;
				m_valueTarget 	= target;
				m_relaxation 	= rel;
			}
 
			void set(float target)
			{
				m_valueTarget = target;
			}
 
			void update	(float dt)
			{
				m_value += (m_valueTarget-m_value)*m_relaxation*dt;
			}

};
