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

class SoundPlayer : public ofxSoundPlayerMultiOutput/*ofSoundPlayer*/
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

        float                           m_soundMainVolumeMin;
        float                           m_soundMainVolumeMax;
		int*							mp_soundMainSpeakers;
		int								m_nbSoundMainSpeakers;
	


    private:
        static SoundManager*			smp_instance;
        vector< SoundPlayer* >          m_listSoundPlayer;
        SoundPlayer*                    mp_soundMain;
    
        float                           m_soundMainVolume;
};