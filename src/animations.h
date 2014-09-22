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
#include "volumeAccum.h"


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


class Animation
{
	public:
		Animation				(string name);
		Animation				(string name, string pathAbsScript);
		Animation				();
		virtual ~Animation		();

				void			M_zeroAll				();
		virtual	void			VM_setArgs				(string args);
		virtual	void			VM_enter				();
		virtual void			VM_update				(float dt);
		virtual void			VM_draw					(float w, float h);
        virtual void            VM_doTransition         (int which, float t);
        virtual	void			VM_exit					();

		void					M_setTheme				(int theme){m_theme=theme;}

		bool					M_loadScript			(const char* pathAbs);
		bool					M_reloadScript			();
		void					M_deleteScript			();
		
		bool					M_isScript				(){return mp_script!=0;}

    
        // UI
        ofxUICanvas*            mp_UIcanvas;
	

        virtual void            setUICanvas             (ofxUICanvas* p){mp_UIcanvas=p;}
        virtual void            createUI                ();
		virtual	void			createUISound			();
        virtual void            createUICustom          (){};
				ofxUICanvas*	getUI					();
        virtual ofxUICanvas*    showUI                  (bool is=true);
        virtual ofxUICanvas*    hideUI                  ();
        virtual void            guiEvent                (ofxUIEventArgs &e);
		virtual	bool			guiEventTogglesSound	(string name);

        virtual void            saveProperties          (string id);
        virtual void            loadProperties          (string id);
        string                  getPropertiesFilename   (string id); // id from surface
    
        // SUPER DIRTY
        static std::map<JSObject*, Animation*> sm_mapJSObj_Anim;

        static ofxJSBOOL        jsNewSlider             (ofxJSContext* cx, ofxJSObject* obj, uintN argc, ofxJSValue* argv, ofxJSValue* retVal);
        static ofxJSBOOL        jsNewToggle             (ofxJSContext* cx, ofxJSObject* obj, uintN argc, ofxJSValue* argv, ofxJSValue* retVal);

 
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
	
		// Sound player
		AnimationSoundPlayer		m_soundPlayer;
		vector<string>				m_listSoundNames;
	
        // Name + paths
		string					m_name;
		string					m_pathAbsScript;
		ofxJSScript*			mp_script;

		// TODO : SAFE with this ?
		JSObject*				mp_obj;

		bool					m_isAutoReloadScript;
		float					m_tReloadScript;
		Poco::Timestamp			m_timestampFileScript;
		
		bool					m_isAutoClear;
		
		// Theme
		int						m_theme;
		enum{
            THEME_UNKNOWN			= 0,
            THEME_SURFACE			= 1,
            THEME_CREATURE			= 2,
            THEME_ARBORESCENCE		= 3
		};
};


class AnimationManager
{
	public:
								AnimationManager		();
								~AnimationManager		();
    
		void					M_setAnimation			(int index, string args="");
		void					M_setAnimation			(string name, string args="");
		void					M_setAnimation			(Animation*, string args="");
		void					M_setAnimationDirect	(string name, string args="");
    
        void                    gotoAnimationNext       ();
        void                    gotoAnimationPrev       ();
        int                     getAnimationIndex       (Animation*);
        void                    setAnimationNext        ();
        void                    setAnimationTimelineNext();
    

		Animation*				M_getAnimation			(int index);
		Animation*				M_getAnimationByName	(string name);
		Animation*				M_getAnimationByThemeRnd(int theme);
		string					M_getAnimationNameRnd	();
		void					M_addAnimation			(Animation*);
		void					M_groupAnimationByTheme	();

		bool					M_isAutoClear			();

		void					M_update				(float dt);
		void					M_drawCanvas			(float w, float h);
		void					M_drawTransition		(float w, float h);

		void					M_setTransitionDuration	(float d){m_transitionDuration=d;}
        bool                    M_isTransiting          ();

		bool					M_editScript			();
		bool					M_reloadScript			();

        void                    M_readSettings          (ofxXmlSettings&);
    
    
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