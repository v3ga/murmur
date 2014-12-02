//
//  toolScene.h
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#pragma once

#include "tool.h"
#include "sceneVisualisation.h"

class toolScene : public tool
{
	public:

		toolScene				(toolManager* parent, SceneVisualisation*);
	
		void					createControlsCustom();

		SceneVisualisation*		getScene			(){return mp_sceneVisualisation;}
		SurfaceNode*			getSurfaceNode		(Surface* pSurface);
		void					handleEvents		(ofxUIEventArgs& e);
		void					onDeviceChanged		();

	private:
		void					setDeviceCurrentNodeDrawLights	();

		SceneVisualisation*		mp_sceneVisualisation;
		bool					m_isDrawDeviceLightCurrent;
};
