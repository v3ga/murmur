/*
 *  device.h
 *  murmur
 *
 *  Created by Julien on 10/04/13.
 *  Copyright 2013 2Roqs. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "oscSender.h"
#include "soundInput.h"
#include "Sample.h"
#include "ofxXmlSettings.h"

// ======================================================================================
class DevicePacket
{
    public:
        DevicePacket         ();

        void                  copy							(DevicePacket* pPacket);
		void				  computeColor					(const ofColor& deviceColor, bool isColor, bool isInvert);
		void				  computeColor					(const float* deviceColor, bool isColor, bool isInvert);
		void				  computeColor2					(const float* deviceColor, const float* deviceColor2, float t, bool isColor, bool isInvert);
		void				  computeColorForVolume			(const float* deviceColor, bool isColor, bool isInvert, float volume);
		void				  computeColor2ForVolume		(const float* deviceColor, const float* deviceColor2, float t, bool isColor, bool isInvert, float volume);
	
        float                 m_volume;
		float				  m_pitch;
		ofColor		  	      m_color;
};


// ======================================================================================
class Device
{
	public:
		Device              (string id, int nbLEDs, float distLEDs);
        virtual ~Device ();
    
        // Identifiant du device + IP
        string              m_id;
        string              m_ip;
        string              getID(){return m_id;}
        string              getIP(){return m_ip;}
    
        // LEDs
        int                 m_nbLEDs;
        float               m_distLEDs;	// meters
        float               m_length;
        void                set     (string id, int nbLEDs, float distLEDs);
    
        int                 getNbLEDs(){return m_nbLEDs;}
        float               getDistLEDs(){return m_distLEDs;}
        float               getLength(){return m_length;}
    
        // Properties
        void                setSoundInputVolumeMax    		(float v);
        void                setSoundInputVolumeMaxOSC 		(float v);

		void				setSoundInputVolumeMaxMin 		(float v);
		void				setSoundInputVolumeMaxMax 		(float v);
		void				setSoundInputVolumeMaxMinOSC	(float v);
		void				setSoundInputVolumeMaxMaxOSC 	(float v);
 
        void                setSoundInputVolHistorySize(int nb);
        void                setSoundInputVolHistorySizeOSC(int nb);

        float               getSoundInputVolumeMax();
        float               getSoundInputVolumeMaxMin();
        float               getSoundInputVolumeMaxMax();
        float               getSoundInputVolHistorySize();

        // Activity / Standby / Standup
        bool                m_isEnableStandbyMode;
        bool                m_isActive;
        float               m_timeInactivity;
        float               m_volHistoryTh;
        float               m_durationPreStandby;
	
		bool				m_isEnableStandup;
		float				m_standupTh;
 
		// Ping
		float				m_volHistoryPingTh;
 		bool				m_resetVolHistoryPing;
 
		void				setVolHistoryPingTh			(float v);
		void				setVolHistoryPingThOSC		(float v);

		virtual void		resetVolHistoryPing			();
		virtual void		resetVolHistoryPingOSC		();
 
        enum{
            EStandby_active              = 0,
            EStandby_pre_standby         = 1,
            EStandby_standby             = 2,
            EStandby_standup             = 3
        };
    
        int                 m_stateStandby;
        float               m_stateStandbyDuration;
        string              m_stateStandbyStr;
        float               m_timeStandby;
 
		void				turnoff(bool bReboot=false);
    
        void                enableStandbyMode(bool is=true);
        void                checkForActivity(float dt);
		bool				isStandUp(){return m_stateStandby == EStandby_standup;}
 
        void                setSoundInputVolHistoryTh(float th);
        void                setSoundInputVolHistoryThOSC(float th);
 
		void				setEnablePitch(bool is=true);
		void				setEnablePitchOSC(bool is);

		void				setSoundInputPitchMin(float min);
		void				setSoundInputPitchMinOSC(float min);
		void				setSoundInputPitchMax(float max);
		void				setSoundInputPitchMaxOSC(float max);
 
		

	
        void                setEnableStandbyMode(bool is);
        void                setEnableStandbyModeOSC(bool is);
    
        float               getSoundInputVolHistoryTh(){return m_volHistoryTh;}
        bool                getEnableStandbyMode(){return m_isEnableStandbyMode;}
 
		void				resetStandBy();
		void               	setTimeStandby(float v);
        void               	setTimeStandbyOSC(float v);
        float               getTimeStandby(){return m_timeStandby;}
		float				getSampleVolStandby(){return m_sampleVolStandby;}

		bool				getEnableStandup			(){return m_isEnableStandup;}
		void				setEnableStandup			(bool is);
		void				setEnableStandupOSC			(bool is);

		void				setSampleVolumeStandby		(float v);
		void				setSampleVolumeStandbyOSC	(float v);
	
		float				getStandupVol				(){return m_standupTh;}
		void				setStandupVol				(float v);
		void				setStandupVolOSC			(float v);
 
		virtual	void		mute(bool is=true){if(mp_soundInput) mp_soundInput->mute(is);}

		// Pitch
		

		// BPM
		int					m_bpm;
		bool				m_bpmEnable;
 
		float				m_bpmPeriod;
		float				m_bpmTime;
		float				m_bpmSoundValue;
 
		int					getBPM						(){return m_bpm;}
		void				setBPM						(int v);
		void				setBPMOSC					(int v);
		void				computeBPM					();
		void				updateBPM					(float dt);
 
		bool				isBPMEnabled				(){return m_bpmEnable;}
		void				setBPMEnable				(bool is);
		void				setBPMEnableOSC				(bool is);
 
		// Sample (stand by)
		Sample*				mp_sampleStandBy;
		float				m_sampleVolStandby;
		string				m_sampleNameStandby;
 
        // Saving
        void                loadXML(string dir);
        void                saveXML(string dir);

		void				loadXMLSurface		(ofxXmlSettings&);
		void				loadXMLSoundInput	(ofxXmlSettings&);
		void				loadXMLSoundOutput	(ofxXmlSettings&);
		void				loadXMLColor		(ofxXmlSettings&);
		void				loadXMLPackets		(ofxXmlSettings&);
		virtual	void		loadXMLData			(ofxXmlSettings&);
		void 				loadXMLPing			(ofxXmlSettings&);
		void 				loadXMLGenerative	(ofxXmlSettings&);
		void				loadXMLBPM			(ofxXmlSettings&);

 
        // Attach point (used only on server side)
        // normalized
        ofVec2f             m_pointSurface;
        void                setPointSurface(float xNorm, float yNorm);

        // OSC
        oscSender           m_oscSender;
 
		// Color of LEDs
		void				enableColor					(bool is=true);
		void				enableColorOSC				(bool is);
		void				setColorHueSaturation		(float h, float s);
		void				setColorHue					(float h);
		void				setColorSaturation			(float s);
		void				setColorHueSaturationOSC	(float h, float s);
		void				setColorSpeedOscillation	(float speed);

		void				setColor2HueSaturation		(float h, float s);
		void				setColor2HueSaturationOSC	(float h, float s);

		bool				m_isEnableColor;
		ofColor				m_color;
		float				m_colorHsv[3];
		int					m_colorMode;
		float				m_colorSpeedOscillation; // phase for cos oscillation , unit is degrees/s
		ofColor				m_colorOscillation1,m_colorOscillation2;

		ofColor				m_color2;
		float				m_colorHsv2[3];

		enum
		{
			colorMode_grey				= 0,	// set color directly
			colorMode_manual_hsb		= 1,	// set color directly
			colorMode_oscillation_hsb	= 2		// oscillation between two m_colorOscillation1 & m_colorOscillation2
		};
	
        // Packets
        vector<DevicePacket*>   m_listPackets;
        virtual void        sampleSoundInput();
        virtual void        createPackets(int nb);
        virtual void        deletePackets();
        virtual void        sendPacketsOSC();
				void		invertPacketsVolume		(bool is);
				void		invertPacketsVolumeOSC	(bool is);
				void		reversePacketsDir		(bool is);
				void		reversePacketsDirOSC	(bool is);

	 			bool		m_isSendMessagesOSC; // used on deviceEcho
				void		setSendMessagesOSC(bool is){m_isSendMessagesOSC=is;}
 
        virtual void        onReceivePacketBegin();
        virtual void        onReceivePacket(DevicePacket*);
        virtual void        onReceivePacketEnd();
    
        DevicePacket*       getLastPacket(){return m_listPackets[m_nbLEDs-1];}
 		bool				m_isUpdatingPacket;
		bool				m_isInvertPacketsVolume;
		bool				m_isReverseDirPackets;
	
        // Sound input
        SoundInput*         mp_soundInput;
		virtual	void		startSoundInput				(int nbChannels);
        virtual void        startSoundInput 			(int deviceId, int nbChannels);
        virtual void        audioIn         			(float * input, int bufferSize, int nChannels);
        virtual float       getWidthSoundInputVolume	();
        virtual float       getHeightSoundInputVolume	();
        virtual void        drawSoundInputVolume		(float x, float y);
        virtual void        drawSoundInputPitch			(float x, float y);
				void		setSoundInputUseRawVolume	(bool is);
				bool		getSoundInputUseRawVolume	(){return m_soundInputUseRawVol;}
				void		setSoundInputUseRawVolumeOSC(bool is);
				void		setSoundInputMute			(bool is);
				void		setSoundInputMuteOSC		(bool is);
				bool		getSoundInputMute			(){return m_soundInputMute;}
				float		getSoundInputPitchMin		(){return m_soundInputPitchMin;}
				float		getSoundInputPitchMax		(){return m_soundInputPitchMax;}

		// Sound playing
		vector<int>			m_listSpeakerIds;
		void				clearListSpeakers	();
		void				addSpeakerId		(int id);
 
        // > only used on server side
        float               m_soundInputVolHistorySize;
        float               m_soundInputVolEmpiricalMax;
        float               m_soundInputVolEmpiricalMaxMin, m_soundInputVolEmpiricalMaxMax;
		bool				m_soundInputUseRawVol;
		bool				m_soundInputMute;
		float				m_soundInputPitchMin, m_soundInputPitchMax;
		bool				m_bEnablePitch;
 
		// Mute graphics
		// > only used in software
		bool				m_bGenerative;
		void				setGenerative				(bool is);
		void				setGenerativeOSC			(bool is);
		bool				isGenerative				(){return m_bGenerative;}
        
        // Interface
        virtual void        update(float dt);

    private:
        int                 m_indexPacketReceived;
        string              getPathXML(string dir);
    
};



// ======================================================================================
class DeviceManager
{
    public:
        DeviceManager       ();
        ~DeviceManager      ();

        void                addDevice           (Device*);
		int					getDevicesNb		(){return m_listDevices.size();}
        Device*             getDeviceById       (string id);
        void                getDevicesListId    (vector<string>&);
        void                setDeviceCurrent    (Device*);
        Device*             setDeviceCurrent    (string deviceId);
        Device*             getDeviceCurrent    (){return mp_deviceCurrent;}
        void                saveDevicesXML      (string dir);
		void				turnoffDevices		(bool bReboot = false);
		int					indexOf				(Device*);
 		int					indexOfCurrent		(){return indexOf(mp_deviceCurrent);}
	
        vector<Device*>     m_listDevices;
    
    private:
        Device*             mp_deviceCurrent;

        vector<Device*>::iterator itDevices;
        void                deleteDevices       ();
};
