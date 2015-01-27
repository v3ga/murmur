#pragma once

#define MURMUR_MULTI_WINDOWS	1

#include "ofMain.h"
#include "murmur.h"
#include "ofxUI.h"
#include "oscReceiver.h"
#include "oscSender.h"
#include "threadRasp.h"
#include "tool.h"

class Scene;
class SceneVisualisation;
class Surface;
class SurfaceNode;
class DeviceManager;
class DeviceInfoManager;
class DeviceEchoSimulator;
class DeviceNode;
class SoundInput;
class Animation;
class AnimationManager;

#if MURMUR_MULTI_WINDOWS
class ofxMultiGLFWWindow;
class GLFWwindow;
#endif

class testApp : public ofBaseApp
{
	public:
		void setup();
		void exit();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		// Multi-windows
		#if MURMUR_MULTI_WINDOWS
        ofxMultiGLFWWindow*		mp_glfw;
        vector<GLFWwindow*>*	mp_windows;
        int 					m_windowIndex;
		#endif

        // Javascript
		void                initJS();

		// OSC
		oscReceiver			m_oscReceiver;

        // UI
		toolManager			toolManager;
	
        bool                m_isUserControls;
		bool				m_isUpdateLayout;
//		void				updateControls();
        void                showControls(bool is=true);
        void                guiUpdateViewSimulation();
		void				guiMarkUpdateLayout(){m_isUpdateLayout=true;}
    
        map<Animation*, ofxUICanvas*> 	m_mapAnimationUI;
        map<ofxUIWidget*, bool> 		m_mapControlsState;

		void				guiUpdateListDevices	();
 
        // Application settings
        ofxXmlSettings      m_settings;
    
        // Configuration
        bool                isShowDevicePointSurfaces;
        bool                isAnimationSequence;
        bool                isFullscreen;
        bool                isViewSimulation;
        bool                isSimulation;
        bool                m_isViewAnimProperties;
		ofVec2f				m_windowSize;

		void				setViewSimulation(bool is=true);
 
        // Devices
        DeviceManager*      mp_deviceManager;
        DeviceEchoSimulator*mp_deviceSimulator;
        DeviceNode*         mp_deviceNodeSim;
        void                initDevices();
		void				initSimulators();
		void				selectDeviceWithIndex(int index);
        void                selectDevice(string id);
	
		vector<DeviceEchoSimulator*>	m_listDeviceSimulator;

		// Sound (simulation only)
		ofSoundStream		m_soundStreamInput;
 
        // Surfaces
        Surface*             mp_surfaceMain;
        SurfaceNode*         mp_surfaceNodeMain;
        void                 initSurfaces();
        float                m_diamCacheLEDs;
		SurfaceNode*		 getSurfaceNode(Surface*);
		void				 onSurfaceModified(Surface*);
		ofRectangle			 m_rectSurfaceOff;
	
        // Devices <> Surfaces
        void                attachDevicesToSurfaces();
        Surface*            getSurfaceForDevice(Device*);
        Surface*            getSurfaceForDeviceCurrent();
		AnimationManager*	getAnimationManagerForDevice(Device*);
		AnimationManager*	getAnimationManagerForDevice(string deviceId);
 
        // Scenes
        SceneVisualisation*  mp_sceneVisualisation;
        std::vector<Scene*>  m_listScenes;
    
        SceneVisualisation*  getSceneVisualisation(){return mp_sceneVisualisation;}
        void                 initScenes();
        void                 createDeviceNodeInScene(Device*);
    
        // Sound input (for simulator)
        void                 audioIn(float * input, int bufferSize, int nChannels);
    
};
