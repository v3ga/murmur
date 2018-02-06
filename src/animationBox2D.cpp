//
//  animationBox2D.cpp
//  murmur
//
//  Created by Julien on 20/05/13.
//
//

#include "animationBox2D.h"
#include "globals.h"
#include "surface.h"


//--------------------------------------------------------------
AnimationBox2D::AnimationBox2D(string name) : Animation(name)
{
    m_volumeAccum = 0.0f;
    m_volumeAccumTarget = 0.0f;
	m_pitchLast = 0.0f;
    m_isBox2DCreated = false;
	
	m_isLeftWall 	= true;
	m_isRightWall	= true;
	m_isTopWall		= true;
	m_isBottomWall	= true;

	m_bHandlePitch = true;
}

//--------------------------------------------------------------
void AnimationBox2D::createBox2D(float gx, float gy, float fps, bool isBounds, ofRectangle bounds)
{
    if (!m_isBox2DCreated)
    {
        m_box2d.init();
        m_box2d.setGravity(gx, gy);
        if (isBounds)
			createBounds(bounds);
        m_box2d.setFPS(fps);
    
        m_isBox2DCreated = true;
    }
}

//--------------------------------------------------------------
void AnimationBox2D::createBounds(ofRectangle bounds)
{
	if (!m_box2d.bWorldCreated) return;
	
	if (m_box2d.ground)
		m_box2d.world->DestroyBody( m_box2d.ground );

	b2BodyDef bd;
	bd.position.Set(0, 0);
	m_box2d.ground = m_box2d.world->CreateBody(&bd);
	
	b2PolygonShape shape;
	
	ofRectangle rec(bounds.x/OFX_BOX2D_SCALE, bounds.y/OFX_BOX2D_SCALE, bounds.width/OFX_BOX2D_SCALE, bounds.height/OFX_BOX2D_SCALE);
	
	//right wall
	if (m_isRightWall)
	{
		shape.SetAsEdge(b2Vec2(rec.x+rec.width, rec.y), b2Vec2(rec.x+rec.width, rec.y+rec.height));
		m_box2d.ground->CreateFixture(&shape, 0.0f);
	}
	
	//left wall
	if (m_isLeftWall)
	{
		shape.SetAsEdge(b2Vec2(rec.x, rec.y), b2Vec2(rec.x, rec.y+rec.height));
		m_box2d.ground->CreateFixture(&shape, 0.0f);
	}
	
	// top wall
	if (m_isTopWall)
	{
		shape.SetAsEdge(b2Vec2(rec.x, rec.y), b2Vec2(rec.x+rec.width, rec.y));
		m_box2d.ground->CreateFixture(&shape, 0.0f);
	}
	
	// bottom wall
	if (m_isBottomWall)
	{
		shape.SetAsEdge(b2Vec2(rec.x, rec.y+rec.height), b2Vec2(rec.x+rec.width, rec.y+rec.height));
		m_box2d.ground->CreateFixture(&shape, 0.0f);
	}

}

//--------------------------------------------------------------
void AnimationBox2D::guiEvent(ofxUIEventArgs &e)
{
	Animation::guiEvent(e);

	string name = e.getName();
	if (name == "left wall" || name == "right wall" || name == "top wall" || name == "bottom wall")
	{
	}
}

//--------------------------------------------------------------
AnimationBox2D_circles::AnimationBox2D_circles(string name ) : AnimationBox2D(name)
{
	OFAPPLOG->begin("AnimationBox2D_circles() constructor");
	
    m_gravity = 0.0f;
    m_isCircleInflating = false;
    m_circleSize = 0.0f;
    m_volTrigger = 0.2f;
    m_sizeMin = 10;
    m_sizeMax = 30;
	m_sizeScale = 1.0f;
    m_nbObjects = 400;

	mp_sliderGravity = 0;
	
	loadColors();
	loadPropertiesDefinition();

	m_properties.add( new classProperty_float("gravity", 		getPropDef("gravity:min", -10.0f),		getPropDef("gravity:max", 10.0f), 	&m_gravity) );
	m_properties.add( new classProperty_float("obj. number", 	getPropDef("obj:number:min", 50), 		getPropDef("obj:number:max", 500), 	&m_nbObjects) );

	m_properties.add( new classProperty_float("obj. size min", 	getPropDef("obj:sizemin:min", 10), 		getPropDef("obj:sizemin:max", 20),	&m_sizeMin) );
	m_properties.add( new classProperty_float("obj. size max", 	getPropDef("obj:sizemax:min", 10),		getPropDef("obj:sizemax:max", 10),	&m_sizeMax) );
	m_properties.add( new classProperty_float("obj. scale", 	getPropDef("obj:scale:min", 0.0f),		getPropDef("obj:scale:max", 1.0f),	&m_sizeScale) );

	m_properties.add( new classProperty_bool("left wall", 	&m_isLeftWall) );
	m_properties.add( new classProperty_bool("right wall", 	&m_isRightWall) );
	m_properties.add( new classProperty_bool("top wall", 	&m_isTopWall) );
	m_properties.add( new classProperty_bool("bottom wall", &m_isBottomWall) );
	
	OFAPPLOG->end();
}

//--------------------------------------------------------------
void AnimationBox2D_circles::onVolumAccumEvent(string deviceId)
{
	Animation::onVolumAccumEvent(deviceId);
	m_colorDevice[deviceId] = chooseRandomColor();
}



//--------------------------------------------------------------
void AnimationBox2D_circles::VM_update(float dt)
{
	updateUIVolume();

	Surface* pSurfaceMain = GLOBALS->getSurfaceMain();
	if (pSurfaceMain)
	{
		createBox2D(0,0*10,30,true,ofRectangle(0, 0, pSurfaceMain->getWidthPixels(), pSurfaceMain->getHeightPixels()));
		createBounds( ofRectangle(0.0f,0.0f,pSurfaceMain->getWidthPixels(),pSurfaceMain->getHeightPixels()) ); // TEMP, should be relative to surface
	}


    if (m_isBox2DCreated)
	{
		if (hasPitch())
		{
			if (m_bPitchManual)
				m_pitchLast = m_pitchManualValue;
		
			m_gravity = ofMap(m_pitchLast, 0.0,1.0, m_properties.getFloat("gravity")->m_max, m_properties.getFloat("gravity")->m_min);
			
			if (mp_sliderGravity)
				mp_sliderGravity->setValue(m_gravity);
		}
	
        m_box2d.setGravity(0.0f, m_gravity);
        m_box2d.update();
    }
    
}

//--------------------------------------------------------------
void AnimationBox2D_circles::VM_draw(float w, float h)
{
	drawBackground(0);
    ofPushStyle();
    ofxBox2dCircle circle;
	for(int i=0; i<m_listCircles.size(); i++)
    {
        circle = m_listCircles[i];
        
		ofFill();
		if (m_isColor || m_isColorFromDevice)
		{
			ofSetColor( m_listCirclesColor[i] );
		}
		else
		{
			ofSetColor(255);
		}
        ofEllipse(circle.getPosition().x, circle.getPosition().y, circle.getRadius()*2*m_sizeScale,circle.getRadius()*2*m_sizeScale);
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
 
	accumulateVolumeAndPitch(pDevicePacket->m_volume, pDevicePacket->m_pitch, deviceId);
	
    m_posAnchor.set(x,y);
    
	if (pDevicePacket->m_volume>m_volTrigger && m_isBox2DCreated)
	{
		// playSound(deviceId);

		m_volumeAccumTarget += pDevicePacket->m_volume;
		m_pitchLast = pDevicePacket->m_pitch;
		if (m_bPitchManual)
			m_pitchLast = m_pitchManualValue;

		if (hasPitch())
		{
			m_circleSize = ofMap(pDevicePacket->m_volume,0.0,1.0,m_sizeMin,m_sizeMax);
		}
		else
		{
	        m_circleSize = ofRandom(m_sizeMin,m_sizeMax);
		}
		

        if (true)
        {
            ofxBox2dCircle circle;
            circle.setPhysics(3.0, 0.53, 0.1);
            circle.setup(m_box2d.getWorld(), x, y, m_circleSize);
            circle.setVelocity( ofRandom(-0.5,0.5), ofRandom(-0.5,0.5) );
            
            m_listCircles.push_back(circle);
			if (m_isColorFromDevice)
			{
				m_listCirclesColor.push_back( pDevicePacket->m_color );
			}
			else
			if (m_isColor)
			{
				m_listCirclesColor.push_back( m_colorDevice[deviceId] );
			}
			else
			{
				m_listCirclesColor.push_back( ofColor(255) );
			}


            if (m_listCircles.size() > (int)m_nbObjects) {
                m_listCircles.begin()->destroy();
                m_listCircles.erase(m_listCircles.begin());
				
				m_listCirclesColor.erase(m_listCirclesColor.begin());
				
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
        mp_UIcanvas->addToggle("color", 							&m_isColor);
        mp_UIcanvas->addToggle("colorFromDevice", 					&m_isColorFromDevice);

		mp_sliderGravity = addUISlider( m_properties.getFloat("gravity") );
        mp_UIcanvas->addSlider("vol. trigger", 		0.0f, 1.0f, 	&m_volTrigger);

		addUISlider( m_properties.getFloat("obj. size min") );
		addUISlider( m_properties.getFloat("obj. size max") );
		addUISlider( m_properties.getFloat("obj. scale") );
		addUISlider( m_properties.getFloat("obj. number") );

		addUItoggle( m_properties.getBool("left wall"));
		addUItoggle( m_properties.getBool("right wall"));
		addUItoggle( m_properties.getBool("top wall"));
		addUItoggle( m_properties.getBool("bottom wall"));
    }
}

//--------------------------------------------------------------
void AnimationBox2D_circles::registerSoundTags(vector<string>& soundTags)
{
	soundTags.push_back("hit");
	soundTags.push_back("bubble");
}





