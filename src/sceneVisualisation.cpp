//
//  sceneVisualisation.cpp
//  murmur
//
//  Created by Julien on 29/04/13.
//
//

#include "sceneVisualisation.h"


//--------------------------------------------------------------
SceneVisualisation::SceneVisualisation()
{
	m_bDrawSurfaces		= true;
 	m_bDrawDevices		= true;
 	m_bDrawSilhouettes	= true;
 	m_bDrawGrid			= true;
}

//--------------------------------------------------------------
SceneVisualisation::~SceneVisualisation()
{
    vector<SurfaceNode*>::iterator itSurface = m_listSurfaceNodes.begin();
    for(; itSurface != m_listSurfaceNodes.end();)
    {
        delete *itSurface;
        itSurface = m_listSurfaceNodes.erase(itSurface);
    }

    vector<DeviceNode*>::iterator itDevice = m_listDeviceNodes.begin();
    for(; itDevice != m_listDeviceNodes.end();)
    {
        delete *itDevice;
        itDevice = m_listDeviceNodes.erase(itDevice);
    }

    vector<SilhouetteNode*>::iterator itSilhouette = m_listSilhouetteNodes.begin();
    for(; itSilhouette != m_listSilhouetteNodes.end();)
    {
        delete *itSilhouette;
        itSilhouette = m_listSilhouetteNodes.erase(itSilhouette);
    }


}

//--------------------------------------------------------------
void SceneVisualisation::loadSettings()
{
	m_settings.loadFile("Config/scene.xml");
}


//--------------------------------------------------------------
ofVec3f SceneVisualisation::getPositionForSilhouette(string deviceId_)
{
	m_settings.pushTag("scene");
		m_settings.pushTag("silhouettes");
		int nbSilhouettes = m_settings.getNumTags("silhouette");
		bool isFound = false;
		float x=0.0f, y=0.0f, z=0.0f;
		for (int i=0 ; i<nbSilhouettes; i++)
		{
			//m_settings.pushTag("silhouette", i)
			string deviceId = m_settings.getAttribute("silhouette", "device", "unknow", i);
			if (deviceId == deviceId_)
			{
				m_settings.pushTag("silhouette", i);
					x = m_settings.getAttribute("position", "x", 0.0f);
					y = m_settings.getAttribute("position", "y", 0.0f);
					z = m_settings.getAttribute("position", "z", 4.0f);
				m_settings.popTag();
			
				isFound = true;
			}
			if(isFound)
				break;
		}
	
	
		m_settings.popTag();
	m_settings.popTag();

	if (isFound){
		return ofVec3f(x,y,z);
	}

	return ofVec3f(0.0f,0.0f,4.0f);
}

//--------------------------------------------------------------
void SceneVisualisation::createDeviceNode( Device* pDevice, SurfaceNode* pSurface)
{
    DeviceNode* pDeviceNode = new DeviceNode( pDevice );
    addDeviceNode(pDeviceNode, pSurface);
}

//--------------------------------------------------------------
void SceneVisualisation::addDeviceNode(DeviceNode* pDeviceNode, SurfaceNode* pSurfaceNode)
{
    m_listDeviceNodes.push_back(pDeviceNode);
    
    // Add a new silhouette
    SilhouetteNode* pSilhouette = new SilhouetteNode( &Data::instance()->m_imgSilhouette );
//    pSilhouette->setPosition(-2.0f,0,4.0f);
	pSilhouette->setPosition( getPositionForSilhouette(pDeviceNode->getDeviceId()) );
    m_listSilhouetteNodes.push_back(pSilhouette);
    
    // Create chain
    ofVec3f posSilhouette = pSilhouette->getGlobalPosition();
    pDeviceNode->createChain(pSurfaceNode->getGlobalPositionDevicePointSurface(pDeviceNode->getDevice()), ofVec3f(0.0f,0.0f,1.0f));
    pDeviceNode->setPositionNodeSoundInput( ofVec3f(posSilhouette.x, posSilhouette.y+0.9f*pSilhouette->getHeight(), posSilhouette.z-0.1f ) );
}

//--------------------------------------------------------------
DeviceNode*	SceneVisualisation::getDeviceNodeFor(Device* pDevice)
{
	vector<DeviceNode*>::iterator it = m_listDeviceNodes.begin();
	for ( ; it != m_listDeviceNodes.end(); ++it)
	{
		if ((*it)->getDevice() == pDevice)
			return (*it);
	}
	return 0;
}

//--------------------------------------------------------------
void SceneVisualisation::addSurfaceNode(SurfaceNode* pSurfaceNode)
{
    m_listSurfaceNodes.push_back(pSurfaceNode);
}

//--------------------------------------------------------------
SurfaceNode* SceneVisualisation::getSurfaceNode(Surface* pSurface)
{
    vector<SurfaceNode*>::iterator it;
    for (it = m_listSurfaceNodes.begin() ; it != m_listSurfaceNodes.end() ; ++it)
    {
        if ( (*it)->getSurface() ==  pSurface)
            return *it;
    }
    return 0;
}


//--------------------------------------------------------------
DeviceNode* SceneVisualisation::getDeviceNode(Device* pDevice)
{
    vector<DeviceNode*>::iterator it;
    for (it = m_listDeviceNodes.begin() ; it != m_listDeviceNodes.end() ; ++it)
    {
        if ( (*it)->getDevice() ==  pDevice)
            return *it;
    }
    return 0;
}


//--------------------------------------------------------------
void SceneVisualisation::setup()
{
    m_cam.setNearClip(0.01f);
    m_cam.setFarClip(20.0f);
    m_camRadiusTarget = m_camRadius = 25.0f;
    m_camAngleTarget = m_camAngle = 45.0f;
    updateCamPosition();
}

//--------------------------------------------------------------
void SceneVisualisation::update(float dt)
{
    m_camAngle += (m_camAngleTarget-m_camAngle)*0.1f;
    m_camRadius += (m_camRadiusTarget-m_camRadius)*0.1f;
    
    updateCamPosition();
}


//--------------------------------------------------------------
void SceneVisualisation::draw()
{
	ofBackground(70,70,70,255);

	// Cam orientation
	m_cam.setPosition(m_camPosition);
	m_cam.setFarClip(1000.0f);
    SurfaceNode* pSurfaceNodeMain = m_listSurfaceNodes[0];
    if (pSurfaceNodeMain){
        m_cam.lookAt(pSurfaceNodeMain->getPositionCenter());
	}
    else{
        m_cam.lookAt(ofVec3f(0,0,0));
	}

	m_cam.begin();
	ofEnableDepthTest();





	


	// Surface
	//glEnable(GL_DEPTH_TEST);
	
//	if(ofGetGLRenderer()) ofGetGLRenderer()->disableTextureTarget(GL_TEXTURE_2D);

	// LAME Patch
	// Silhouettes nodes
		if (m_listSilhouetteNodes.size()>0)
		{
			ofPushStyle();
			ofPushMatrix();
			ofEnableAlphaBlending();
			
			ofTranslate(0,10000);
			m_listSilhouetteNodes[0]->draw();


	    	ofDisableBlendMode();
			ofPopMatrix();
			ofPopStyle();
		}
	

	// Grid
	if (m_bDrawGrid)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofPushMatrix();
		ofFill();
    	ofSetColor(120,120,120,255);
    	ofRotateZ(90);
    	ofDrawGridPlane(10.0f, 20.0f);
    	ofPopMatrix();
    	ofDisableAlphaBlending();
		ofPopStyle();
	}
	
	if (m_bDrawSurfaces)
	{
		ofPushStyle();
		ofSetColor(255,255,255);
    	vector<SurfaceNode*>::iterator itSurface = m_listSurfaceNodes.begin();
    	for(; itSurface != m_listSurfaceNodes.end(); ++itSurface)
        	(*itSurface)->draw();
		ofPopStyle();

	}

	
	// Silhouettes nodes
	if (m_bDrawSilhouettes)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
    	vector<SilhouetteNode*>::iterator itSilhouette = m_listSilhouetteNodes.begin();
    	for(; itSilhouette != m_listSilhouetteNodes.end(); ++itSilhouette)
        	(*itSilhouette)->draw();
    	ofDisableBlendMode();
		ofPopStyle();
	}

	
	// Device nodes


	if (m_bDrawDevices)
	{
		ofPushStyle();
	    vector<DeviceNode*>::iterator itDevice = m_listDeviceNodes.begin();
    	for(; itDevice != m_listDeviceNodes.end(); ++itDevice)
        	(*itDevice)->draw();
		ofPopStyle();
	}
	ofDisableDepthTest();

	//glDisable(GL_DEPTH_TEST);
    m_cam.end();

}

//--------------------------------------------------------------
void SceneVisualisation::updateCamPosition()
{
    m_camAngleTarget = min( max(30.0f,m_camAngleTarget), 150.0f);
    m_camRadiusTarget = min( max(1.75f,m_camRadiusTarget), 15.0f);

    m_camPosition.set( m_camRadius*cos( ofDegToRad(m_camAngle) ), 6.0f, m_camRadius*sin( ofDegToRad(m_camAngle) ));
    // printf("m_camPosition = %.2f,%.2f,%.2f", m_camPosition.x,m_camPosition.y,m_camPosition.z);
}

//--------------------------------------------------------------
void SceneVisualisation::keyPressed(int key)
{
    if (key == OF_KEY_LEFT)
    {
        m_camAngleTarget += 3.0f;
    }
    else if (key == OF_KEY_RIGHT)
    {
        m_camAngleTarget -= 3.0f;
    }
    if (key == OF_KEY_UP)
    {
        m_camRadiusTarget -= 0.5f;
    }
    else if (key == OF_KEY_DOWN)
    {
        m_camRadiusTarget += 0.5f;
    }

    updateCamPosition();
}


//--------------------------------------------------------------
void SceneVisualisation::mousePressed(int x, int y)
{
    m_mouseDrag = true;
    m_mousePointDragStart.set((float)x, (float)y);
    m_mousePointDrag = m_mousePointDragStart;
    m_camAngleTargetDragStart = m_camAngleTarget;
    m_camRadiusTargetDragStart = m_camRadiusTarget;
}

//--------------------------------------------------------------
void SceneVisualisation::mouseDragged(int x, int y)
{
    m_mousePointDrag.set((float)x, (float)y);
    m_camAngleTarget = m_camAngleTargetDragStart +(m_mousePointDrag.x - m_mousePointDragStart.x)*0.1f;
    m_camRadiusTarget = m_camRadiusTargetDragStart +(m_mousePointDrag.y - m_mousePointDragStart.y)*0.05f;
    updateCamPosition();
}

//--------------------------------------------------------------
void SceneVisualisation::mouseReleased(int x, int y)
{
    m_mouseDrag = false;
}


