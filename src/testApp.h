#pragma once

#include "ofMain.h"
#include "windowAnim.h"
#include "ofxUI.h"
#include "oscReceiver.h"
#include "oscSender.h"

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

    
        // Javascript
		void                initJS();

        // UI
        bool                m_isUserControls;
        void                initControls();
        void                showControls(bool is=true);
        void                guiEvent(ofxUIEventArgs &e);
        void                guiUpdateListDevices();
        void                guiUpdateDeviceAnimationTitle();
        void                guiUpdateDevice(Device*);
        void                guiUpdateViewSimulation();
        void                guiShowAnimationPropsAll(bool is=true);
    
        map<Animation*, ofxUICanvas*> m_mapAnimationUI;


        ofxUICanvas         *mp_gui1,*mp_gui2,*mp_gui3,*mp_gui4,*mp_guiAnimProps;
        ofxUIDropDownList*	mp_ddlDevices;
        ofxUILabel*         mp_lblAnimTitle;
        ofxUIToggle*        mp_tgViewSimu;

        ofxUILabel*         mp_lblDeviceTitle;
        ofxUISlider         *mp_sliderDeviceVolMax,*mp_sliderDeviceVolHistorySize,*mp_sliderDeviceVolHistoryTh,*mp_sliderDeviceTimeStandby,*mp_sliderDeviceNbLEDsStandby,*mp_sliderDeviceSpeedStandby;
        ofxUIToggle*        mp_toggleDeviceEnableStandby;
    
        // Application settings
        ofxXmlSettings      m_settings;
    
        // Configuration
        bool                isShowDevicePointSurfaces;
        bool                isAnimationSequence;
        bool                isFullscreen;
        bool                isViewSimulation;
        bool                isSimulation;
        bool                m_isViewAnimProperties;

        void                toggleView();
    
        // OSC
        oscReceiver         m_oscReceiver;
        string              m_oscReceiverIP;
        int                 m_oscReceiverPort;
    
        // Devices
        DeviceManager*      mp_deviceManager;
        DeviceEchoSimulator*mp_deviceSimulator;
        DeviceNode*         mp_deviceNodeSim;
        void                initDevices();
        void                selectDevice(string id);
    
        // Surfaces
        Surface*             mp_surfaceMain;
        SurfaceNode*         mp_surfaceNodeMain;
        void                 initSurfaces();
        float                m_diamCacheLEDs;
    
        // Devices <> Surfaces
        DeviceInfoManager*  mp_deviceInfoManager; // DEPRECATED
        void                attachDevicesToSurfaces();
        Surface*            getSurfaceForDevice(Device*);
        Surface*            getSurfaceForDeviceCurrent();
    
        // Scenes
        SceneVisualisation*  mp_sceneVisualisation;
        std::vector<Scene*>  m_listScenes;
    
        SceneVisualisation*  getSceneVisualisation(){return mp_sceneVisualisation;}
        void                 initScenes();
        void                 createDeviceNodeInScene(Device*);
    
        // Sound input (for simulator)
        void                 audioIn(float * input, int bufferSize, int nChannels);
    
};
