//
//  toolScene.cpp
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#include "toolScene.h"

//--------------------------------------------------------------
toolScene::toolScene(toolManager* parent, SceneVisualisation* scene) : tool("Scene", parent)
{
	mp_sceneVisualisation = scene;
}
	
//--------------------------------------------------------------
void toolScene::createControlsCustom()
{
	if (mp_canvas)
	{
	
	}
}

//--------------------------------------------------------------
SurfaceNode* toolScene::getSurfaceNode(Surface* pSurface)
{
	if (mp_sceneVisualisation)
		return mp_sceneVisualisation->getSurfaceNode(pSurface);
	return 0;
}


//--------------------------------------------------------------
void toolScene::handleEvents(ofxUIEventArgs& e)
{
    string name = e.widget->getName();
}
