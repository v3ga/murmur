//
//  soundManager.h
//  murmur
//
//  Created by Julien on 16/05/13.
//
//

#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxSoundPlayerMultiOutput.h"

class SoundInfo
{
	public:

		void					setTagsString(string tags);
		string					getTagsString();
		bool					hasTag(string tag);
		vector<string>			m_tags;
};

class SoundPlayer : public ofxSoundPlayerMultiOutput
{
    public:
        SoundPlayer                     (string name){m_name = name;}
        string                          m_name;
};



class SoundManager
{
    public:
        SoundManager					();
        ~SoundManager					();
	
		static void						destroy();
    
    
        static SoundManager*			instance();
        void                            setup(ofxXmlSettings& settings);
        void                            update();
        void                            playSound(string name, bool isLoop=true, float volume=1.0f,int* speakers=0,int nbSpeakers=0);
        void                            setVolume(string name, float volume);
        void                            setVolumeAll(float volume);
        void                            stopAll();
        void                            activateSoundMain(bool is=true);
        void                            setVolumeSoundMain(float vol);
        void                            setVolumeSoundMainNormalized(float t);
        SoundPlayer*                    getSoundPlayer(string name);
		vector<string> 					getListSoundsName();
		SoundInfo*						getSoundInfo(string name);
		void							addSoundInfo(string name, SoundInfo*);
		vector<string>					getListSoundsNameWithTag(vector<string>& tags);
		int								getDriverSelected(){return m_driver;}

		int								m_driver; // relative to FMOD
        float                           m_soundMainVolumeMin;
        float                           m_soundMainVolumeMax;
		int*							mp_soundMainSpeakers;
		int								m_nbSoundMainSpeakers;
	


    private:
        static SoundManager*			smp_instance;
        vector< SoundPlayer* >          m_listSoundPlayer;
		map<string, SoundInfo*>			m_mapSoundInfos; // associates file name with Sound info struct
        SoundPlayer*                    mp_soundMain;
    
        float                           m_soundMainVolume;
};