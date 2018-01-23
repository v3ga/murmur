//
//  pitchAnalysis.cpp
//  murmurPitchDetection
//
//  Created by Julien on 18/01/2018.
//
//

#include "pitchAnalysis.h"

//--------------------------------------------------------------
pitchAnalysis::pitchAnalysis()
{
	setConfidenceTh(0.6);
	setHistorySize(200);
	setClamp(100,600);
	m_pitch = 0.0f;
}

//--------------------------------------------------------------
void pitchAnalysis::setHistorySize(float size)
{
	m_historySize = size;
	m_history.clear();
	m_history.assign(m_historySize, 0.0f);

	m_historyNorm.clear();
	m_historyNorm.assign(m_historySize, 0.0f);
}

//--------------------------------------------------------------
float pitchAnalysis::getPitchNorm(float value)
{
	return ofMap(value, m_pitchMin, m_pitchMax, 0.0f, 1.0f, true);
}

//--------------------------------------------------------------
float pitchAnalysis::getHistoryValue(int index)
{
	return m_history[index];
}


//--------------------------------------------------------------
void pitchAnalysis::update(float pitch, float confidence, float dt)
{
	if (dt == 0.0f)
		dt = 1.0 / 60.0f;

	if (confidence >= m_confidenceTh)
	{
		m_pitch 		= pitch;
		m_pitchTarget	= pitch;
	}
	else
	{
		m_pitchTarget = m_pitchMin;
	}

	m_pitch += (m_pitchTarget-m_pitch) * dt * 0.2;
	m_pitch = ofClamp(m_pitch,m_pitchMin,m_pitchMax);


	m_history.insert(m_history.begin(), m_pitch);
	m_historyNorm.insert(m_historyNorm.begin(), getPitchNorm(m_pitch));
	if (m_history.size() > m_historySize)
	{
		m_history.erase( m_history.end()-1, m_history.end());
		m_historyNorm.erase( m_historyNorm.end()-1, m_historyNorm.end());
	}
}
