//
//  deviceNode.cpp
//  murmur
//
//  Created by Julien on 29/04/13.
//
//

#include "deviceNode.h"

#define	SPRING_MIN_STRENGTH		0.005
#define SPRING_MAX_STRENGTH		0.1

#define	SPRING_MIN_WIDTH		1
#define SPRING_MAX_WIDTH		3

//--------------------------------------------------------------
DeviceNode::DeviceNode(Device* pDevice) : ofNode(),
mp_device(pDevice)
{
}

//--------------------------------------------------------------
void DeviceNode::setPositionNodeSoundInput(ofVec3f newPosition)
{
    msa::physics::Spring3D *spring = (msa::physics::Spring3D *) physics.getSpring( physics.numberOfSprings()-1 );
    msa::physics::Particle3D *p = spring->getTheOtherEnd();
    if (p)
        p->moveTo(newPosition);
}


//--------------------------------------------------------------
void  DeviceNode::setPositionNodeSurface(ofVec3f newPosition)
{
    msa::physics::Spring3D *spring = (msa::physics::Spring3D *) physics.getSpring( 0 );
    msa::physics::Particle3D *p = spring->getOneEnd();
    if (p)
        p->moveTo(newPosition);
}

//--------------------------------------------------------------
void DeviceNode::createChain(ofVec3f ptAttach, ofVec3f dir)
{
    if (mp_device)
    {
        // Physics setup
        physics.clear();
        physics.setGravity(ofVec3f(0, -0.00075, 0));

        // Normalize
        dir.normalize();
        
        ofVec3f posLed  = ptAttach;
        msa::physics::Particle3D* prev=0;
        for (int i=0; i<mp_device->getNbLEDs(); i++ )
        {
            // Create particles
            msa::physics::Particle3D *p = physics.makeParticle(posLed,0.0025f);
            
            // Attach first one
            if (i==0)
                p->makeFixed();
            
            // Spring
            if (prev){
                msa::physics::Spring3D* sp = physics.makeSpring(prev, p, 1.0f, mp_device->getDistLEDs());
                sp->setForceCap(1.0f);
            }
                
            prev = p;
            posLed += mp_device->getDistLEDs() * dir;
            //printf("mp_device->getDistLEDs()=%.3f\n", mp_device->getDistLEDs());
        }
        
        if (prev)
            prev->makeFixed();
        
    }
}

//--------------------------------------------------------------
void DeviceNode::customDraw()
{
    if (mp_device==0) return;
    
	physics.update();
    
    DevicePacket* pPacket=0;
    float   maxBoxSize = 0.055f;
    float   boxSize = 0.0f;
    int nbPackets = mp_device->getNbLEDs();

    ofVec3f A,B;
    for(int i=0; i<physics.numberOfSprings(); i++)
    {
        msa::physics::Spring3D *spring = (msa::physics::Spring3D *) physics.getSpring(i);
        msa::physics::Particle3D *a = spring->getOneEnd();
        msa::physics::Particle3D *b = spring->getTheOtherEnd();
        A = a->getPosition();
        B = b->getPosition();
        
        ofVec3f vec = b->getPosition() - a->getPosition();
        float dist = vec.length();
        float angle = acos( vec.z / dist ) * RAD_TO_DEG;
        if(vec.z <= 0 ) angle = -angle;
        float rx = -vec.y * vec.z;
        float ry =  vec.x * vec.z;
        
        glPushMatrix();
        glTranslatef(a->getPosition().x, a->getPosition().y, a->getPosition().z);
        glRotatef(angle, rx, ry, 0.0);
        float size  = ofMap(spring->getStrength(), SPRING_MIN_STRENGTH, SPRING_MAX_STRENGTH, SPRING_MIN_WIDTH, SPRING_MAX_WIDTH);
        
        boxSize = ofMap(mp_device->m_listPackets[nbPackets-1-i]->m_volume,0.0f,1.0f, 0.0f*maxBoxSize, maxBoxSize);
        ofSetColor(mp_device->m_listPackets[nbPackets-1-i]->m_color,255);

        ofBox(boxSize);
        glPopMatrix();

        ofPushStyle();
        ofSetColor(50,50,50,200);
        ofSetLineWidth(2.0f);
        ofLine(A.x,A.y,A.z, B.x,B.y,B.z);
        ofPopStyle();
    }
}
