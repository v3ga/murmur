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
#include "ofxAubio.h"
#include "pitchAnalysis.h"

#if SOUNDINPUT_USE_FFT
#include "ofxFFTLive.h"
#endif

class Sample;
class SoundInput
{
    public:
 
		~SoundInput							();
	
        virtual void        setup           (int deviceId, int nChannels);
		virtual	void		setup			(int nChannels); // only for simulators
        virtual void        update          ();
        virtual void        audioIn         (float * input, int bufferSize, int nChannels);
 
        virtual void        drawVolume      (float x, float y);
	
		void				useRawVolume	(bool is=true){m_useRawVol=is;}
		void				mute			(bool is=true){m_isMute=is;}
 
        vector <float>&     getVolHistory   (){return m_volHistory;}
		float				getVolHistoryLast();
        float               getHeightDraw   (){return m_heightDraw;}
		void				setVolume		(float vol);
    
        void                setVolEmpiricalMax(float v){m_volEmpiricalMax=v;}
        void                setVolHistorySize(int nb=400);
        void                setVolHistoryValue(int i, float value);
    
        float               getVolEmpiricalMax(){return m_volEmpiricalMax;}

        void                updateVolHistoryMean();
        float               getVolHistoryMean();
        float               getVolHistoryMeanFiltered();
		int					getNbChannels(){return m_nbChannels;}
		int					getBufferSize(){return m_bufferSize;}
 
		vector <float>&     getPitchHistory   (){return m_pitchAnalysis.m_historyNorm;}
		void				drawPitch		  (float x, float y);

	
		void				stopInput		(bool is=true){m_isStopInput=is;}
		void				setSample		(Sample*);
		void				setSampleVolume	(float v){m_sampleVolume = v;}
		float				getSampleVolume	(){return m_sampleVolume;}
 
		bool				getUseRawVol	(){return m_useRawVol;}
 
		void				setInputVolumeModulate(float f){m_inputVolumeModulate=f;}
 
		void				setPitchMin		(float v){m_pitchAnalysis.setMin(v);}
		void				setPitchMax		(float v){m_pitchAnalysis.setMax(v);}
 
		float				getPitchMin		(){return m_pitchAnalysis.m_pitchMin;}
		float				getPitchMax		(){return m_pitchAnalysis.m_pitchMax;}
 
    private:
        //ofSoundStream*       mp_soundStreamInput;
        ofSoundStream       m_soundStreamInput;
#if SOUNDINPUT_USE_FFT
        ofxFFTLive          m_fft;
#endif

		bool				m_isMute;
		bool				m_isStopInput;
		float				m_inputVolumeModulate; // multiply the value retrived in audio in by this value


		// Volume
        vector <float>      m_mono;
        vector <float>      m_left;
        vector <float>      m_right;
        vector <float>      m_volHistory;
        int                 m_volHistoryNb;
        float               m_volHistoryMean, m_volHistoryMeanFiltered;
		int 				m_sampleRate;
	

		bool				m_useRawVol;
        float               m_smoothedVol;
        float               m_scaledVol;
        float               m_volEmpiricalMax;
    
        int                 m_nbChannels;
        int                 m_bufferSize;

		// Pitch
        ofxAubioPitch 		m_pitch;
		pitchAnalysis		m_pitchAnalysis;

		// View
        float               m_heightDraw;
	
		// Sample playback
		Sample*				mp_sample;
		float*				m_sampleData;
		float				m_sampleVolume;
	
    
        void                initAudioBuffers	();
		void				initPitch			();
        void                setPitchHistorySize(int nb=400);
		void				processAudio		(float * input, int bufferSize, int nChannels);
};
