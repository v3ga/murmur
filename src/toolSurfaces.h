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
#include "midiInterface.h"
#include "classProperty.h"

class Device;
class GLFWwindow;
class toolSurfaces : public tool, public midiInterface
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
		bool					keyPressed					(int key);
		void					windowResized				(int w, int h);
		void					windowResized				(int wold, int hold, int w, int h);
		void					setWindowCurrent			(GLFWwindow* p){mp_windowCurrent=p;}

		virtual string			getMidiSettingsPath			(string suffix=""){return "Gui/tools/midi/Surfaces.xml";}
		void					loadMidiSettings			(){midiInterface::setClassProperties(&m_properties) ; midiInterface::loadMidiSettings();}
	
	
		bool					loadMask					(string pathMask);
		void					updateMaskUI				(string pathMask);
	
		void					dragEvent					(ofDragInfo dragInfo);

	 	classProperties			m_properties;

		ofxUILabel*				mp_lblSurfaceActivity;
		Surface*				mp_surfaceMain;
		ofImage*				mp_mask;
		ofImage*				mp_maskUI;
		float					m_alpha;

		quadWarping				m_quadWarping;
		ofRectangle			 	m_rectScreen, m_rectSurfaceOff;

		int						m_view;
		bool					m_isDrawHandles;
	
		enum{
				VIEW_NORMAL				= 0,
				VIEW_QUADWARPING		= 1
		};
	
	private:
	
		GLFWwindow*				mp_windowCurrent;

		string					getQuadWarpingPathFile		();
		void					computeRectSurfaceOff		();

};
