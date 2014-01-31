//
//  animationTunnel.cpp
//  murmur
//
//  Created by Julien on 24/05/13.
//
//

#include "animationTunnel.h"
#include "globals.h"
#include "testApp.h"


TunnelElement::TunnelElement()
{
    m_t = 0.0f;
}


//--------------------------------------------------------------
Tunnel::Tunnel(AnimationTunnel* pAnimation)
{
	mp_animation = pAnimation;
}


//--------------------------------------------------------------
void Tunnel::VM_update(float dt)
{
    vector<TunnelElement*>::iterator it = m_listTunnelElement.begin();
    TunnelElement* pTunnelElement;
    for ( ; it!=m_listTunnelElement.end() ; )
    {
        pTunnelElement = *it;
        pTunnelElement->m_t += mp_animation->m_speedWave*dt;

        if (pTunnelElement->m_t >= 1.0f) {
            it = m_listTunnelElement.erase(it);
            delete pTunnelElement;
        } else {
            ++it;
        }
    }
}


//--------------------------------------------------------------
void Tunnel::VM_draw()
{
    ofTranslate(m_posAnchor.x,m_posAnchor.y,0);
    
    vector<TunnelElement*>::iterator it = m_listTunnelElement.begin();
    TunnelElement* pTunnelElement;
    ofVec2f posTunnel;
    for ( ; it!=m_listTunnelElement.end() ; ++it)
    {
        pTunnelElement = *it;

        glPushMatrix();
        float s = ofMap(pTunnelElement->m_t, 0,1,1,0.05);
        float f = 1.25f;
        ofRotateZ( pTunnelElement->m_t*mp_animation->m_ampRotation*sin( pTunnelElement->m_t*3.14*2 ) );

        ofNoFill();
        ofSetColor(255,(1.0-pTunnelElement->m_t)*255);

//        ofRect(-s*(w)/2,-s*(f*h)/2,s*(w),s*(f*h));
        
        glPopMatrix();
    }
	
}


//--------------------------------------------------------------
AnimationTunnel::AnimationTunnel(string name) : Animation(name)
{
    m_isCreateSpline = false;
    m_volumeAccum = 0.0f;
    m_volumeAccumTarget = 0.0f;

    m_volAccumTh = 0.5f;
    m_volAccumTrigger = 2.5f;
    
    m_zMax = -6500.0;
    m_ampRotation = 0;
    
    m_speedWave = 0.1f;
    
    m_volAccum.setTriggerInCb(sM_volTriggerIn, this);
    
    m_soundPlayer.add( "waves1.wav" );
    m_soundPlayer.add( "waves2.wav" );
    m_soundPlayer.add( "waves3.wav" );
    m_soundPlayer.add( "waves4.wav" );
    m_soundPlayer.add( "waves5.wav" );
    m_soundPlayer.add( "waves6.wav" );
    m_soundPlayer.add( "waves7.wav" );
    m_soundPlayer.add( "waves8.wav" );
    
}

//--------------------------------------------------------------
void AnimationTunnel::VM_enter()
{
    m_isCreateSpline = false;
}

//--------------------------------------------------------------
void AnimationTunnel::createSpline(float w, float h)
{
    if (!m_isCreateSpline && w>0 && h>0)
    {
        m_spline.clear();
        m_spline.setInterpolation(msa::kInterpolationCubic);
    
        int nbPoints = 2;
        float stepz = m_zMax / float(nbPoints-1);
    
        float x = 0;
        float y = 0;
        float z = 0.0f;
        float rndx=0, rndy=0;
        float bounding = 0;
        for (int i=0 ; i<nbPoints ; i++)
        {
            m_spline.push_back( ofVec3f(x,y+rndy,z) );
            rndx = ofRandom(-bounding/2,bounding/2);
            rndy = ofRandom(-bounding/2,bounding/2);
            z+=stepz;
        }
        
        m_isCreateSpline = true;
    }
}


//--------------------------------------------------------------
void AnimationTunnel::VM_update(float dt)
{
    m_t+=0.1*dt;
    if (m_t>=1.0f) m_t=0.0f;
}

//--------------------------------------------------------------
ofVec3f AnimationTunnel::getTangentAt(float t)
{
    return (m_spline.sampleAt(t+0.01f)-m_spline.sampleAt(t)).normalized();
}

//--------------------------------------------------------------
void AnimationTunnel::computeBasisAt(float* result, float t)
{
    ofVec3f k = getTangentAt(t);
    ofVec3f i = k.crossed( ofVec3f(0,1,0) );
    ofVec3f j = k.crossed( i );

    result[0] = i.x;
    result[1] = i.y;
    result[2] = i.z;
    result[3] = 0;
    
    result[4] = j.x;
    result[5] = j.y;
    result[6] = j.z;
    result[7] = 0;
    
    result[8] = k.x;
    result[9] = k.y;
    result[10] = k.z;
    result[11] = 0;
    
    result[12] = 0;
    result[13] = 0;
    result[14] = 0;
    result[15] = 1.0f;
}


//--------------------------------------------------------------
void AnimationTunnel::VM_draw(float w, float h)
{
    //createSpline(w,h);
    glEnable(GL_DEPTH_TEST);
    

    ofBackground(0,0,0,255);
    ofTranslate(m_posAnchor.x,m_posAnchor.y,0);
    
    vector<TunnelElement*>::iterator it = m_listTunnelElement.begin();
    TunnelElement* pTunnelElement;
    ofVec2f posTunnel;
    for ( ; it!=m_listTunnelElement.end() ; ++it)
    {
        pTunnelElement = *it;

        glPushMatrix();
        float s = ofMap(pTunnelElement->m_t, 0,1,1,0.05);
        float f = 1.25f;
        ofRotateZ( pTunnelElement->m_t*m_ampRotation*sin( pTunnelElement->m_t*3.14*2 ) );

        ofNoFill();
        ofSetColor(255,(1.0-pTunnelElement->m_t)*255);

        ofRect(-s*(w)/2,-s*(f*h)/2,s*(w),s*(f*h));
        
        glPopMatrix();
    }
    glDisable(GL_DEPTH_TEST);
 }

//--------------------------------------------------------------
void AnimationTunnel::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
    if (pDevicePacket==0) return;
    m_posAnchor.set(x,y);
    m_volAccum.add(pDevicePacket->m_volume);
}


//--------------------------------------------------------------
void AnimationTunnel::sM_volTriggerIn(void* pUserData)
{
    AnimationTunnel* pThis = (AnimationTunnel*) pUserData;
    pThis->m_listTunnelElement.push_back( new TunnelElement() );
    pThis->m_soundPlayer.playRandom();
}

//--------------------------------------------------------------
void AnimationTunnel::createUICustom()
{

    if (mp_UIcanvas)
    {
        mp_UIcanvas->addSlider("vol. trigger in", 0.0f, 1.0f, &m_volAccum.m_valueTriggerIn);
        mp_UIcanvas->addSlider("vol. trigger out", 0.0f, 1.0f, &m_volAccum.m_valueTriggerOut);
        mp_UIcanvas->addSlider("z tunnel", -7000.0f, -2000.0f, m_zMax);
        mp_UIcanvas->addSlider("rot. amplitude", 0.0f, 20.0f, m_ampRotation);
        mp_UIcanvas->addSlider("speed wave", 0.05f, 0.3f, m_speedWave);
    }
}

//--------------------------------------------------------------
void AnimationTunnel::guiEvent(ofxUIEventArgs &e)
{
    string name = e.widget->getName();
    if (name == "z tunnel")
    {
        m_isCreateSpline = false;
        m_zMax = ((ofxUISlider*) e.widget)->getScaledValue();
    }
    else if (name == "rot. amplitude")
    {
        m_ampRotation = ((ofxUISlider*) e.widget)->getScaledValue();
    }
    else if (name == "speed wave")
    {
        m_speedWave = ((ofxUISlider*) e.widget)->getScaledValue();
    }
}

