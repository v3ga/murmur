//
//  toolSurfaces.h
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#pragma once

#include "tool.h"
#include "surface.h"

class Device;
class toolSurfaces : public tool
{
	public:

		toolSurfaces			(toolManager* parent, Surface*);

	
		void					createControlsCustom		();
		Surface*				getSurfaceForDeviceCurrent	();
		Surface*				getSurfaceForDevice			(Device*);
		void 					onSurfaceModified			(Surface*);
		void					update						();
		void					handleEvents				(ofxUIEventArgs& e);
	
		bool					loadMask					(string pathMask);
		void					updateMaskUI				(string pathMask);
	
		void					dragEvent					(ofDragInfo dragInfo);

		ofxUILabel*				mp_lblSurfaceActivity;
		Surface*				mp_surfaceMain;
		ofImage*				mp_mask;
		ofImage*				mp_maskUI;
};
