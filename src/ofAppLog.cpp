//
//  ofAppLog.cpp
//  A_P_P_A_R_E_L
//
//  Created by Julien on 15/07/2014.
//
//

#include "ofAppLog.h"

//--------------------------------------------------------------
ofAppLog* ofAppLog::smp_instance = 0;

//--------------------------------------------------------------
void ofAppLog::destroy()
{
	delete smp_instance;
}

//--------------------------------------------------------------
void ofAppLog::begin(string s)
{
	println(s);
	println("{");
	m_level++;
}

//--------------------------------------------------------------
void ofAppLog::end()
{
	m_level--;
	println("}");
	println("");
}

//--------------------------------------------------------------
void ofAppLog::println(ofLogLevel level, const string & message)
{
	ofLog(level) << message;
}

//--------------------------------------------------------------
void ofAppLog::println(const string & message)
{
	ofLog() << space(m_level) << message;
}




//--------------------------------------------------------------
ofAppLog* ofAppLog::getInstance()
{
	if(smp_instance==0)
		smp_instance = new ofAppLog();
	return smp_instance;
}

//--------------------------------------------------------------
string ofAppLog::space(int nb)
{
	string s;
	for (int i=0;i<nb*2;i++) s+=" ";
	return s;
}


