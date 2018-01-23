//
//  pitchAnalysis.hpp
//  murmurPitchDetection
//
//  Created by Julien on 18/01/2018.
//
//

#pragma once
#include "ofMain.h"

class pitchAnalysis
{
	public:
		pitchAnalysis		();

		void				setConfidenceTh	(float th){m_confidenceTh=th;}
		void				setClamp		(float min, float max){setMin(min);setMax(max);}
		void				setMin			(float min){m_pitchMin = min;}
		void				setMax			(float max){m_pitchMax = max;}
		void				setHistorySize	(float size);
		int					getHistorySize	(){return m_historySize;}
		float				getPitchNorm	(float value);
		float				getHistoryValue	(int index);
		float				getValue		(){return m_pitch;}
		void				update			(float pitch, float confidence, float dt);
	
		float				m_pitchMin, m_pitchMax;
		float				m_confidenceTh;
 
		vector<float>		m_history;
		vector<float>		m_historyNorm;
		int					m_historySize;

	private:
		float				m_pitch, m_pitchTarget;
};

