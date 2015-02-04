//
//  AnimationTunnelALB.cpp
//  murmur
//
//  Created by Julien on 03/02/2015.
//
//

#include "AnimationTunnelALB.h"

//--------------------------------------------------------------
TunnelElementALB::TunnelElementALB(ofMesh* pMesh, ofVec3f pos, ofVec3f dir, float dirSpeed, int leftOrRight)
{
	mp_mesh = pMesh;
	m_pos = pos;
	m_dir = dir;
	m_dirSpeed = dirSpeed;
	m_leftOrRight = leftOrRight;
}


//--------------------------------------------------------------
void TunnelElementALB::setRotation(ofMatrix4x4& m)
{
	m_rot = m;
}

//--------------------------------------------------------------
void TunnelElementALB::update(float dt)
{
	m_pos += m_dir*m_dirSpeed*dt;
}

//--------------------------------------------------------------
void TunnelElementALB::draw()
{
	if (mp_mesh)
	{
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(m_pos);
		ofMultMatrix( ofMatrix4x4::getTransposedOf(m_rot) );
		if (m_leftOrRight)
			ofRotateZ(180.0f);
		ofSetColor(255,255);
//		mp_mesh->drawVertices();
		mp_mesh->draw();
		ofPopMatrix();
		ofPopStyle();
	}
}

//--------------------------------------------------------------
AnimationTunnelALB::AnimationTunnelALB(string name) : Animation(name)
{
	mp_meshes[0] = 0;
	mp_meshes[1] = 0;
	
	m_dirAngle	 = 2;
	m_dirSpeed	 = 500;

	m_h1Mesh	 = 20;
	m_h2Mesh	 = 5;
	m_wMesh		 = 10;


	m_dirSpeed		= 300;
	m_emitLeft		= true;
	m_emitRight		= true;
	
	//m_volAccum.setTriggerInCb(sM_volTriggerIn, this);

}

//--------------------------------------------------------------
AnimationTunnelALB::~AnimationTunnelALB()
{
	deleteElements();
	deleteMeshes();
}

//--------------------------------------------------------------
void AnimationTunnelALB::deleteElements()
{
	vector<TunnelElementALB*>::iterator it = m_elements.begin();
	for ( ; it != m_elements.end(); ++it)
	{
		delete *it;
	}
	m_elements.clear();
}

//--------------------------------------------------------------
void AnimationTunnelALB::createMeshes()
{
	deleteMeshes();


	float h1 = m_h1Mesh;
	float h2 = m_h2Mesh;
	float d	 = m_wMesh;

	mp_meshes[0] = new ofMesh();
	
	mp_meshes[0]->addVertex(ofVec3f(0,h1,0));
	mp_meshes[0]->addVertex(ofVec3f(-h1,h2,0));
	mp_meshes[0]->addVertex(ofVec3f(-h1,-h2,0));
	mp_meshes[0]->addVertex(ofVec3f(0,-h1,0));

	mp_meshes[0]->addVertex(ofVec3f(0,-h1,d));
	mp_meshes[0]->addVertex(ofVec3f(-h1,-h2,d));
	mp_meshes[0]->addVertex(ofVec3f(-h1,h2,d));
	mp_meshes[0]->addVertex(ofVec3f(0,h1,d));

	mp_meshes[0]->addIndex(0);
	mp_meshes[0]->addIndex(7);
	mp_meshes[0]->addIndex(1);

	mp_meshes[0]->addIndex(7);
	mp_meshes[0]->addIndex(1);
	mp_meshes[0]->addIndex(6);

	mp_meshes[0]->addIndex(1);
	mp_meshes[0]->addIndex(6);
	mp_meshes[0]->addIndex(2);

	mp_meshes[0]->addIndex(6);
	mp_meshes[0]->addIndex(5);
	mp_meshes[0]->addIndex(2);


	mp_meshes[0]->addIndex(2);
	mp_meshes[0]->addIndex(4);
	mp_meshes[0]->addIndex(3);

	mp_meshes[0]->addIndex(2);
	mp_meshes[0]->addIndex(5);
	mp_meshes[0]->addIndex(4);


	mp_meshes[0]->setMode(OF_PRIMITIVE_TRIANGLES);

	mp_meshes[1] = 0;
}

//--------------------------------------------------------------
void AnimationTunnelALB::modifyMeshes()
{
	if (mp_meshes[0] == 0)
		createMeshes();

	float h1 = m_h1Mesh;
	float h2 = m_h2Mesh;
	float d	 = m_wMesh;

	mp_meshes[0]->setVertex(0,ofVec3f(0,h1,0));
	mp_meshes[0]->setVertex(1,ofVec3f(-h1,h2,0));
	mp_meshes[0]->setVertex(2,ofVec3f(-h1,-h2,0));
	mp_meshes[0]->setVertex(3,ofVec3f(0,-h1,0));

	mp_meshes[0]->setVertex(4,ofVec3f(0,-h1,d));
	mp_meshes[0]->setVertex(5,ofVec3f(-h1,-h2,d));
	mp_meshes[0]->setVertex(6,ofVec3f(-h1,h2,d));
	mp_meshes[0]->setVertex(7,ofVec3f(0,h1,d));
}

//--------------------------------------------------------------
void AnimationTunnelALB::deleteMeshes()
{
	delete mp_meshes[0];
	delete mp_meshes[1];

	mp_meshes[0] = 0;
	mp_meshes[1] = 0;
}


//--------------------------------------------------------------
void AnimationTunnelALB::VM_enter()
{
	if (mp_meshes[0] == 0)
		createMeshes();
}

//--------------------------------------------------------------
void AnimationTunnelALB::VM_update(float dt)
{
	updateUIVolume();

	//m_dirLeft.set	( ofVec3f( cos(ofDegToRad(90+m_dirAngle)), 0, sin(ofDegToRad(90+m_dirAngle)) ) );
  	//m_dirRight.set	( ofVec3f( cos(ofDegToRad(90-m_dirAngle)), 0, sin(ofDegToRad(90-m_dirAngle)) ) );
	
	vector<TunnelElementALB*>::iterator it = m_elements.begin();
	for ( ; it!=m_elements.end();)
	{
		TunnelElementALB* p = *it;
		p->update(dt);
		if (p->m_pos.z>=1000)
		{
			delete p;
			m_elements.erase(it);
		}
		else ++it;
	}
}


//--------------------------------------------------------------
void AnimationTunnelALB::VM_draw(float w, float h)
{
	m_w = w;
	m_h = h;

	ofBackground(0);
	m_cam.setPosition(0,0,1000);
	m_cam.begin(ofRectangle(0,0,w,h));

	// ofDrawAxis(50);
	
	int nbElements = m_elements.size();
	for (int i=0;i<nbElements;i++)
	{
		m_elements[i]->draw();
	}

/*	ofPushMatrix();
	ofTranslate(m_posFar);
	ofDrawAxis(100);
	ofPopMatrix();
*/
	
	m_cam.end();
}

//--------------------------------------------------------------
void AnimationTunnelALB::onNewPacket(DevicePacket* pDevicePacket, string deviceId, float x, float y)
{
    if (pDevicePacket==0) return;


    m_posAnchor.set(x,y);
	m_posFar.set(x,y,300);
	
	if (pDevicePacket->m_volume>=m_volAccum.m_valueTriggerIn)
	{
		TunnelElementALB* pTunnelElement=0;
		
		ofVec3f posAnchorWorld = m_cam.screenToWorld(ofVec3f(m_posAnchor.x,m_posAnchor.y,0),ofRectangle(0,0,m_w,m_h));
		ofVec3f dir = (posAnchorWorld - m_cam.getPosition()).normalize();
		// m_posFar = /*m_cam.getPosition()+*/1000.0*dir;
		m_posFar = m_cam.getPosition()+1500.0*dir;


//	m_dirLeft.set	( ofVec3f( cos(ofDegToRad(90+m_dirAngle)), 0, sin(ofDegToRad(90+m_dirAngle)) ) );
  	//m_dirRight.set	( ofVec3f( cos(ofDegToRad(90-m_dirAngle)), 0, sin(ofDegToRad(90-m_dirAngle)) ) );
m_dirLeft = (m_cam.screenToWorld(ofVec3f(m_w/2,m_h/2,0))-m_posFar).normalized();
m_dirRight =(m_cam.screenToWorld(ofVec3f(m_w/2,m_h/2,0))-m_posFar).normalized();
ofMatrix4x4 m;

ofVec3f k = m_dirLeft;
ofVec3f i = ofVec3f(1.0f,0.0f,0.0f);
ofVec3f j = k.crossed(i);

m.set(
i.x, j.x, k.x, 0,
i.y, j.y, k.y, 0,
i.z, j.z, k.z, 0,
0, 0, 0, 1

);


		//m_dirLeft

		if (m_emitRight)
		{
			pTunnelElement = new TunnelElementALB(mp_meshes[0], m_posFar, m_dirRight.normalize(), m_dirSpeed,1);
			pTunnelElement->setRotation(m);
			m_elements.push_back( pTunnelElement );
		}
		if (m_emitLeft)
		{
			pTunnelElement = new TunnelElementALB(mp_meshes[0], m_posFar, m_dirLeft.normalize(), m_dirSpeed,0);
			pTunnelElement->setRotation(m);
			m_elements.push_back( pTunnelElement );
		}
	}
}

//--------------------------------------------------------------
void AnimationTunnelALB::sM_volTriggerIn(void* pUserData,VolumeAccum*)
{
}

//--------------------------------------------------------------
void AnimationTunnelALB::createUICustom()
{
    if (mp_UIcanvas)
    {
        mp_UIcanvas->addSlider("vol. th", 	0.0f, 1.0f, 	&m_volAccum.m_valueTriggerIn);
//        mp_UIcanvas->addSlider("z point", 	0, 500.0f, 		&m_posFar.z);
        mp_UIcanvas->addSlider("h1", 		10, 50, 		&m_h1Mesh);
        mp_UIcanvas->addSlider("h2", 		1, 10, 			&m_h2Mesh);
        mp_UIcanvas->addSlider("w", 		1, 30, 			&m_wMesh);
        mp_UIcanvas->addSlider("speed", 	100.0f, 500.0f, &m_dirSpeed);
//        mp_UIcanvas->addSlider("angle", 	0, 10, 			&m_dirAngle);
        mp_UIcanvas->addToggle("left", 						&m_emitLeft);
        mp_UIcanvas->addToggle("right", 					&m_emitRight);
    }
}

//--------------------------------------------------------------
void AnimationTunnelALB::guiEvent(ofxUIEventArgs &e)
{
	if (e.getName() == "h1" || e.getName() == "h2" || e.getName() == "w")
		modifyMeshes();
	else
	if (e.getName() == "speed")
	{
		int nbElements = m_elements.size();
		for (int i=0;i<nbElements;i++)
			m_elements[i]->m_dirSpeed = m_dirSpeed;
	}
}


