//
//  animationBox2D.cpp
//  murmur
//
//  Created by Julien on 20/05/13.
//
//

#include "animationBox2D.h"
#include "globals.h"

//--------------------------------------------------------------
AnimationBox2D::AnimationBox2D(string name) : Animation(name)
{
    m_volumeAccum = 0.0f;
    m_volumeAccumTarget = 0.0f;
    m_isBox2DCreated = false;
}

//--------------------------------------------------------------
void AnimationBox2D::createBox2D(float gx, float gy, float fps, bool isBounds, ofRectangle bounds)
{
    if (!m_isBox2DCreated)
    {
        m_box2d.init();
        m_box2d.setGravity(gx, gy);
        if (isBounds)
            m_box2d.createBounds(bounds);
        m_box2d.setFPS(fps);
    
        m_isBox2DCreated = true;
    }
}

//--------------------------------------------------------------
AnimationBox2D_circles::AnimationBox2D_circles(string name ) : AnimationBox2D(name)
{
    m_gravity = 0.0f;
    m_isCircleInflating = false;
    m_circleSize = 0.0f;
    m_volTrigger = 0.2f;
    m_sizeMin = 10;
    m_sizeMax = 30;
    m_nbObjects = 400;
}

//--------------------------------------------------------------
void AnimationBox2D_circles::VM_update(float dt)
{
    if (m_isBox2DCreated){
        m_box2d.setGravity(0.0f, m_gravity);
        m_box2d.update();
    }
    
}

//--------------------------------------------------------------
void AnimationBox2D_circles::VM_draw(float w, float h)
{
    createBox2D(0,0*10,30,true,ofRectangle(0, 0, w, h));
    
    ofBackground(0);
    ofPushStyle();
    ofxBox2dCircle circle;
	for(int i=0; i<m_listCircles.size(); i++)
    {
        circle = m_listCircles[i];
        
		ofFill();
		ofSetColor(255);
        ofEllipse(circle.getPosition().x, circle.getPosition().y, circle.getRadius()*2,circle.getRadius()*2);
    }

    if (m_isCircleInflating)
    {
		ofSetColor(255,0,0);
        ofEllipse(m_posAnchor.x, m_posAnchor.y, m_circleSize,m_circleSize);
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void AnimationBox2D_circles::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
    if (pDevicePacket==0) return;
 
	accumulateVolume(pDevicePacket->m_volume, deviceId);
	
    m_posAnchor.set(x,y);
    
	if (pDevicePacket->m_volume>m_volTrigger)
	{
		// playSound(deviceId);

		m_volumeAccumTarget += pDevicePacket->m_volume;

        m_circleSize = ofRandom(m_sizeMin,m_sizeMax);

        //if (m_volumeAccumTarget>=10.5f)
        {
            ofxBox2dCircle circle;
            circle.setPhysics(3.0, 0.53, 0.1);
            circle.setup(m_box2d.getWorld(), x, y, m_circleSize);
            circle.setVelocity( ofRandom(-0.5,0.5), ofRandom(-0.5,0.5) );
            
            m_listCircles.push_back(circle);

            if (m_listCircles.size() > (int)m_nbObjects) {
                m_listCircles.begin()->destroy();
                m_listCircles.erase(m_listCircles.begin());
            }
            
            m_volumeAccumTarget = 0;
            m_isCircleInflating = false;
        }
    }
	else
	{
		m_volumeAccumTarget = 0;
        m_isCircleInflating = false;
	}
}

//--------------------------------------------------------------
void AnimationBox2D_circles::createUICustom()
{
    if (mp_UIcanvas)
    {
        mp_UIcanvas->addSlider("gravity", -10.0f, 10.0f, &m_gravity);
        mp_UIcanvas->addSlider("vol. trigger", 0.0f, 1.0f, &m_volTrigger);
        mp_UIcanvas->addSlider("obj. size min", 10, 20, &m_sizeMin);
        mp_UIcanvas->addSlider("obj. size max", 10, 50, &m_sizeMax);
        mp_UIcanvas->addSlider("obj. number", 100, 500, &m_nbObjects);
    }
}

//--------------------------------------------------------------
void AnimationBox2D_circles::registerSoundTags(vector<string>& soundTags)
{
	soundTags.push_back("hit");
	soundTags.push_back("bubble");
}


