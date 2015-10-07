/*
 *  animations.h
 *  versatubes
 *
 *  Created by 2Roqs on 30/04/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */
 
#pragma once

#include "ofMain.h"
#include "ofxJavascript.h"
#include "ofxTween.h"
#include "ofxXmlSettings.h"
#include "deviceInfo.h"
#include "ofxUI.h"
#include "ofxUIMovingGraphThreshold.h"
#include "volumeAccum.h"
#include "ofxJSONElement.h"
#include "ofxMidi.h"
#include "classProperty.h"
#include "midiInterface.h"

struct AnimationTheme
{
    public:
        int                     m_id;
        string                  m_name;
};

class AnimationInfo
{
    public:
        AnimationInfo               (string name){
                                        m_name = name;
                                    }
    
        string                      m_name;
        string                      m_args;
};

class AnimationSoundPlayer
{
    public:
        AnimationSoundPlayer        ();

        void                        	add(string name);
    
		void							setVolume(float v){m_volume=v;}
	    void                            playRandom(vector<int>& speakers);
		void							playRandom(int* speakers=0, int nbSpeakers=0);
		void							print(string title);

		float							m_volume;
    	int                             m_lastPlayedIndex;
    	std::vector<string>             m_listSoundNames; // those which are checked in interface

};


class Animation : public midiInterface
{
	public:
		Animation				(string name);
		Animation				(string name, string pathAbsScript);
		Animation				();
		virtual ~Animation		();

				void			M_zeroAll				();
		virtual	void			VM_setArgs				(string args);
		virtual	void			VM_enter				();
				void			M_update				(float dt);
		virtual void			VM_update				(float dt);
		virtual void			VM_drawBefore			(float w, float h){}
		virtual void			VM_draw					(float w, float h);
        virtual void            VM_doTransition         (int which, float t);
        virtual	void			VM_exit					();

		bool					M_loadScript			(const char* pathAbs);
		bool					M_reloadScript			();
		void					M_deleteScript			();
		
		bool					M_isScript				(){return mp_script!=0;}

		// Export to string
		virtual string			toString				(){return "";}
 
 		// Background
		bool					m_bDrawBackground;
		void					setDrawBackground		(bool is=true){m_bDrawBackground = is;}
 		void					drawBackground			(float grey){if (m_bDrawBackground) ofBackground(grey,255);}
		void					drawBackground			(float r, float g, float b, float a){if (m_bDrawBackground) ofBackground(r,g,b,a);}
 
		// Midi
		virtual	string			getMidiSettingsPath		(string suffix=""){ return "Config/midi/"+m_name+suffix+".xml"; }
		virtual	void			loadMidiSettings		();
		virtual	void			handleMidiMessages		();
		virtual void 			newMidiMessage			(ofxMidiMessage& eventArgs);

        // UI
        ofxUICanvas*            mp_UIcanvas;
        ofxUICanvas*            mp_UIcanvas01;

		ofxUIMovingGraphThreshold* mp_UIVolumeTh;
		ofxUILabel*				 mp_lblVolValues;
 
		ofxUITextInput*			mp_teConfigName;

        virtual void            setUICanvas             (ofxUICanvas* p){mp_UIcanvas=p;}
		void					addUISlider				(classProperty_float*);
		void					addUISlider				(classProperty_int*);
		void					addUItoggle				(classProperty_bool*);
        virtual void            createUI                ();
		virtual	void			createUIConfiguration	();
        virtual void            createUIMidi         	();
		virtual	void			createUIVolume			();
		virtual	void			createUISound			();
        virtual void            createUICustom          (){};
				ofxUICanvas*	getUI					();
        virtual ofxUICanvas*    showUI                  (bool is=true);
        virtual ofxUICanvas*    hideUI                  ();
        virtual void            guiEvent                (ofxUIEventArgs &e);
		virtual	bool			guiEventTogglesSound	(string name);
				void			updateUIVolume			();

		// Settings
		// When instanced from surface xml, animation can read settings from <animation node>
		virtual	void			readSurfaceSettings		(ofxXmlSettings& settings){}


		// Default properties
        virtual void            saveProperties          (string id);
        virtual void            loadProperties          (string id);
        string                  getPropertiesFilename   (string id, bool isExtension=true); // id from surface

		// Properties saved as configuration
 		vector<string>			m_configurations;
 				string			m_configurationCurrent;
				void			createDirConfiguration	();
		virtual	void			populateConfigurations	();
		virtual	void			saveConfiguration		(string filename);
		virtual	void			loadConfiguration		(string filename);
		virtual void			loadConfigurationPrev	();
		virtual	void			loadConfigurationNext	();
				int				getConfigurationIndex	(string filename);
 
		bool					m_bResetOnEnter;
 

		// Class properties
		classProperties			m_properties;
 
        // SUPER DIRTY
        static std::map<JSObject*, Animation*> sm_mapJSObj_Anim;

        static ofxJSBOOL        jsNewSlider             (ofxJSContext* cx, ofxJSObject* obj, uintN argc, ofxJSValue* argv, ofxJSValue* retVal);
        static ofxJSBOOL        jsNewToggle             (ofxJSContext* cx, ofxJSObject* obj, uintN argc, ofxJSValue* argv, ofxJSValue* retVal);
	
		void					jsCallSoundChanged		();
 
 		// Device
		Device*					getDevice				(string deviceId);
		Device*					getDeviceCurrent		();
		bool					updateDevicePosition	(string deviceId, float x, float y); // return true if device point changed on surface
		virtual	void			onDevicePositionChanged	(string deviceId, float x, float y){}

		map<string, ofVec2f>	m_devicePositions;
 
		// Shader
		void					M_beginShader			();
		void					M_endShader				();
		void					M_loadShader			(string name);
		ofShader				m_shader;
	
        // Event packets
        virtual void            onNewPacket             (DevicePacket*, string deviceId, float xNorm, float yNorm);
 
		// Volume accumulutators
		void						accumulateVolume		(float volume, string deviceId);
		map<string, VolumeAccum*>	m_mapDeviceVolumAccum;
		static void					sOnVolumAccumEvent		(void*,VolumeAccum*);
		virtual void				onVolumAccumEvent		(string deviceId);
 
		float						m_volValuesMeanTh;
	
		// Sound player
		AnimationSoundPlayer		m_soundPlayer;
		vector<string>				m_listSoundNames;
		virtual void				registerSoundTags		(vector<string>& tags);
		int							getNbSoundNames			(){return m_listSoundNames.size();}
		virtual	void				playSound				(string deviceId);
	
        // Name + paths
		string						m_name;
		string						m_pathAbsScript;
		ofxJSScript*				mp_script;

		// TODO : SAFE with this ?
		JSObject*					mp_obj;

		bool						m_isAutoReloadScript;
		float						m_tReloadScript;
		Poco::Timestamp				m_timestampFileScript;
		
		bool						m_isAutoClear;

		// Color
		bool						m_isColor;
		vector<ofColor>				m_colors;
		map<string, ofColor>		m_colorDevice;
		void						loadColors			();
		ofColor						chooseRandomColor	();

		bool						m_isColorFromDevice;
};


class AnimationManager
{
	public:
								AnimationManager		();
								~AnimationManager		();
    
		void					M_setAnimation			(int index, string args="");
		void					M_setAnimation			(string name, string args="");
		void					M_setAnimation			(Animation*, string args="");

		void					M_setAnimationDirect	(int index, string args="");
		void					M_setAnimationDirect	(string name, string args="");
	
    
        void                    gotoAnimationNext       ();
        void                    gotoAnimationPrev       ();
        int                     getAnimationIndex       (Animation*);
        void                    setAnimationNext        ();
        void                    setAnimationTimelineNext();
    

		Animation*				M_getAnimation			(int index);
		Animation*				M_getAnimationByName	(string name);
		Animation*				M_getAnimationByThemeRnd(int theme);
		Animation*				M_getAnimationCurrent	(){return mp_animationCurrent;}
		string					M_getAnimationNameRnd	();
		void					M_addAnimation			(Animation*);
	

		bool					M_isAutoClear			();

		void					M_update				(float dt);
		void					M_drawCanvasBefore		(float w, float h);
		void					M_drawCanvas			(float w, float h);
		void					M_drawTransition		(float w, float h);

		void					M_setTransitionDuration	(float d){m_transitionDuration=d;}
        bool                    M_isTransiting          ();

		bool					M_editScript			();
		bool					M_reloadScript			();

        void                    M_readSettings          (ofxXmlSettings&);
 
		void					loadMidiSettings		();
		void					newMidiMessage			(ofxMidiMessage& midiMessage);
    
    
		vector<Animation*>		m_listAnimations;
		Animation*				mp_animationCurrent;
		Animation*				mp_animationNext;
		string					m_animationNextArgs;
    
    
        vector<AnimationInfo>   m_listAnimationsInfos;
        int                     m_animationInfoIndex;
    
		
		map<int,vector<Animation*>*>		m_listAnimationsTheme;

		int						m_state;
		void					M_changeState			(int newState);

		enum{
			STATE_PLAY				= 0,
			STATE_TRANSITION_IN		= 1,
			STATE_TRANSITION_OUT	= 2
		};

		// Transition
		ofxTween				m_transitionTween;
		float					m_transitionDuration;
		ofxEasingBack			easingback;
		ofxEasingLinear			easinglinear;
		
	private:
		void					M_changeAnimation		(Animation*,string args="");
		

};