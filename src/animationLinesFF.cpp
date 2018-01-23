//
//  animationLinesFF.cpp
//  murmur
//
//  Created by Julien on 22/09/2015.
//
//

#include "LineFFElement.h"
#include "animationLinesFF.h"

//--------------------------------------------------------------
AnimationLinesFF::AnimationLinesFF(string name) : Animation(name)
{
	m_w = m_h = -1.0;
	m_dirSpeed = 800;
	m_rotSpeed = 0.0f;
	
	
	mp_propDirSpeed	  = new classProperty_float("dirSpeed", 100.0f,1000.0f, &m_dirSpeed);
	mp_propRotSpeed	  = new classProperty_float("rotSpeed", 0.0f,360.0f, &m_rotSpeed);
	
	
	
	m_properties.add( mp_propDirSpeed );
	m_properties.add( new classProperty_float("zMax", 100.0f,2000.0f, &m_zMax) );
	m_properties.add( new classProperty_float("radius", 0.0f,200.0f, &m_radius) );
	m_properties.add( new classProperty_bool("radiusDirect", &m_bRadiusDirect) );
	m_properties.add( new classProperty_float("rot", 0.0f,360.0f, &m_rot) );
	m_properties.add( mp_propRotSpeed );

	m_meshPlane = ofMesh::box(2.0f, 400.0f, 1.0f);

	mp_sliderDirSpeed = 0;
	mp_sliderRotSpeed = 0;
	
	m_bHandlePitch = true;

	if (m_bHandlePitch)
	{
		mp_propDirSpeed->enableTarget();
		mp_propRotSpeed->enableTarget();

		mp_propDirSpeed->m_variableTarget = m_dirSpeed;
		mp_propRotSpeed->m_variableTarget = m_rotSpeed;
	}
}

//--------------------------------------------------------------
AnimationLinesFF::~AnimationLinesFF()
{
	deleteElements();
}

//--------------------------------------------------------------
void AnimationLinesFF::createUICustom()
{
    if (mp_UIcanvas)
    {
        mp_UIcanvas->addToggle("colorFromDevice", &m_isColorFromDevice);


		mp_sliderDirSpeed = addUISlider( m_properties.getFloat("dirSpeed") );
		addUISlider( m_properties.getFloat("zMax") );
		addUISlider( m_properties.getFloat("radius") );
		addUItoggle( m_properties.getBool("radiusDirect") );
		addUISlider( m_properties.getFloat("rot") );
		mp_sliderRotSpeed = addUISlider( m_properties.getFloat("rotSpeed") );
    }
}

//--------------------------------------------------------------
void AnimationLinesFF::VM_enter()
{
	setDrawBackground();
	if (m_bResetOnEnter)
		deleteElements();
}


//--------------------------------------------------------------
void AnimationLinesFF::VM_update(float dt)
{
	updateUIVolume();
	if (hasPitch())
	{
		mp_sliderDirSpeed->setValue( m_dirSpeed );
		mp_sliderRotSpeed->setValue( m_rotSpeed );
	}

	if (mp_propDirSpeed) mp_propDirSpeed->update(dt);
	if (mp_propRotSpeed) mp_propRotSpeed->update(dt);


	vector<LineFFElement*>::iterator it = m_lines.begin();
	for (; it != m_lines.end();)
	{
		(*it)->update(dt);
		if ( (*it)->m_pos.z >= 1000.0f){
			m_lines.erase(it);
		}
		else
			++it;
	}
}

//--------------------------------------------------------------
void AnimationLinesFF::VM_draw(float w, float h)
{
	m_w = w;
	m_h = h;

	m_cam.setPosition(0,0,1000);
	m_cam.begin(ofRectangle(0,0,w,h));

	drawBackground(0);

	ofPushStyle();
	vector<LineFFElement*>::iterator it = m_lines.begin();
	for (; it != m_lines.end(); ++it)
		(*it)->draw();

	m_cam.end();
	ofPopStyle();
}

//--------------------------------------------------------------
void AnimationLinesFF::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
	accumulateVolumeAndPitch(pDevicePacket->m_volume, pDevicePacket->m_pitch, deviceId);
	m_lastPacketColor = pDevicePacket->m_color;
	if (pDevicePacket->m_volume >= m_volValuesMeanTh)
	{
		if (hasPitch())
		{
		
		
			if (mp_propDirSpeed) mp_propDirSpeed->setTarget( ofMap( pDevicePacket->m_pitch, 0.0f,1.0f, 350.0f, 1000.0f, true) );
			if (mp_propRotSpeed) mp_propRotSpeed->setTarget( ofMap( pDevicePacket->m_pitch, 0.0f,1.0f, 360.0f, 40.0f,true) );
		
			// ofLog() << "pDevicePacket->m_pitch="<< pDevicePacket->m_pitch<< ";mp_propDirSpeed->m_variableTarget = " << mp_propDirSpeed->m_variableTarget;
		}

		emitLine(deviceId);
	}
}

//--------------------------------------------------------------
void AnimationLinesFF::emitLine(string deviceId)
{
	Device* pDevice = getDevice(deviceId);
	if (pDevice)
	{
		if (m_w>0 && m_h>0)
		{
			ofVec3f posAnchorScreen(pDevice->m_pointSurface.x*m_w,pDevice->m_pointSurface.y*m_h, 0);
			ofVec3f posAnchorWorld = m_cam.screenToWorld(posAnchorScreen,ofRectangle(0,0,m_w,m_h));
			ofVec3f dir = (posAnchorWorld - m_cam.getPosition()).normalize();

			ofVec3f posFar = m_cam.getPosition()+m_zMax*dir;
;
			LineFFElement* pLine = new LineFFElement(this, posFar,-dir);
			pLine->m_radius 	= m_radius;
			pLine->m_rot 		= m_rot;
			pLine->m_color		= m_isColorFromDevice ? m_lastPacketColor : ofColor(255);
			
			m_lines.push_back(pLine);
		}
	}
}

//--------------------------------------------------------------
void AnimationLinesFF::deleteElements()
{
	vector<LineFFElement*>::iterator it = m_lines.begin();
	for (; it != m_lines.end(); ++it)
		delete *it;
	m_lines.clear();
}






