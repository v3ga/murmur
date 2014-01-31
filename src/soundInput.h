//
//  soundInput.h
//  murmur
//
//  Created by Julien on 15/04/13.
//
//

#pragma once
#define SOUNDINPUT_USE_FFT 0

#include "ofMain.h"

#if SOUNDINPUT_USE_FFT
#include "ofxFFTLive.h"
#endif


class SoundInput
{
    public:
 
	
		~SoundInput							();
	
        virtual void        setup           (int deviceId, int nChannels);
		virtual	void		setup			(int nChannels); // only for simulators
        virtual void        update          ();
        virtual void        audioIn         (float * input, int bufferSize, int nChannels);
    
        virtual void        drawVolume      (float x, float y);
	
		void				mute			(bool is=true){m_isMute=is;}
    
        vector <float>&     getVolHistory   (){return m_volHistory;}
        float               getHeightDraw   (){return m_heightDraw;}
    
        void                setVolEmpiricalMax(float v){m_volEmpiricalMax=v;}
        void                setVolHistorySize(int nb=400);
        void                setVolHistoryValue(int i, float value);
    
        float               getVolEmpiricalMax(){return m_volEmpiricalMax;}

        void                updateVolHistoryMean();
        float               getVolHistoryMean();
        float               getVolHistoryMeanFiltered();
    
    private:
        ofSoundStream*       mp_soundStreamInput;
#if SOUNDINPUT_USE_FFT
        ofxFFTLive          m_fft;
#endif

		bool				m_isMute;

        vector <float>      m_mono;
        vector <float>      m_left;
        vector <float>      m_right;
        vector <float>      m_volHistory;
        int                 m_volHistoryNb;
        float               m_volHistoryMean, m_volHistoryMeanFiltered;
    

        float               m_smoothedVol;
        float               m_scaledVol;
        float               m_volEmpiricalMax;
    
        int                 m_nbChannels;
        int                 m_bufferSize;
    
        float               m_heightDraw;

    
        void                initAudioBuffers();
};
