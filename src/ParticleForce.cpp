//
//  ParticleForce.cpp
//  murmur
//
//  Created by Julien on 02/10/2015.
//
//

#include "ParticleForce.h"

//--------------------------------------------------------------
ParticleForce::ParticleForce(float anchorx, float anchory)
{
	m_anchor.set(anchorx, anchory);
	m_volume = m_volumeTarget = 0.0f;
}


//--------------------------------------------------------------
void ParticleForce::update(float dt)
{
    m_volume += (m_volumeTarget-m_volume)*0.4f;
}
