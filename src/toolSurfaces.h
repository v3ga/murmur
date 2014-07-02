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
#include "quadWarping.h"

class Device;
class toolSurfaces : public tool
{
	public:

		toolSurfaces			(toolManager* parent, Surface*);

	
		void					createControlsCustom		();
		Surface*				getSurfaceForDeviceCurrent	();
		Surface*				getSurfaceForDevice			(Device*);
		void 					onSurfaceModified			(Surface*);
		void					setView						(int which);
		void					saveData					();
		void					loadData					();
		void					setup						();
		void					update						();
		void					draw						();
		void					handleEvents				(ofxUIEventArgs& e);
		void					mousePressed				(int x, int y, int button);
		void					keyPressed					(int key);

	
	
		bool					loadMask					(string pathMask);
		void					updateMaskUI				(string pathMask);
	
		void					dragEvent					(ofDragInfo dragInfo);

		ofxUILabel*				mp_lblSurfaceActivity;
		Surface*				mp_surfaceMain;
		ofImage*				mp_mask;
		ofImage*				mp_maskUI;

		quadWarping				m_quadWarping;
		ofRectangle			 	m_rectScreen, m_rectSurfaceOff;

		int						m_view;
		bool					m_isDrawHandles;
	
		enum{
				VIEW_NORMAL				= 0,
				VIEW_QUADWARPING		= 1
		};
	
	private:

		string					getQuadWarpingPathFile		();

};
