//
//  classProperty.cpp
//  murmur
//
//  Created by Julien on 17/06/2015.
//
//

#include "classProperty.h"


//--------------------------------------------------------------
classProperty_float::classProperty_float(string name, float min, float max, float* pValue) : classProperty(name)
{
	m_min 	= min;
	m_max 	= max;
	mp_variable	= pValue;
	m_type	= FLOAT;
	m_ownsVariable = false;
}

//--------------------------------------------------------------
classProperty_float::classProperty_float(string name, float min, float max) : classProperty(name)
{
	m_min 	= min;
	m_max 	= max;
	mp_variable	= new float();
	m_type	= FLOAT;
	m_ownsVariable = true;
}

//--------------------------------------------------------------
classProperty_float::~classProperty_float()
{
	if (m_ownsVariable){
		delete mp_variable;
	}
}


//--------------------------------------------------------------
void classProperty_float::setValueFromMidiMessage(ofxMidiMessage& m)
{
	*mp_variable = ofMap(m.value,0,127, m_min, m_max);
}


//--------------------------------------------------------------
classProperty_bool::classProperty_bool(string name, bool* pValue) : classProperty(name)
{
	mp_variable	= pValue;
	m_type	= BOOL;
}


//--------------------------------------------------------------
classProperties::classProperties()
{
}

//--------------------------------------------------------------
classProperties::~classProperties()
{
	map<string, classProperty*>::iterator it;
	for (it = m_list.begin() ; it != m_list.end()  ; ++it)
	{
		delete it->second;
	}
	m_list.clear();
}

//--------------------------------------------------------------
void classProperties::add(classProperty* p)
{
	m_list[p->m_name] = p;
}

//--------------------------------------------------------------
classProperty* classProperties::get(string name)
{
	if (m_list.find(name) != m_list.end())
		return m_list[name];
	return 0;
}

//--------------------------------------------------------------
classProperty_float* classProperties::getFloat(string name)
{
	classProperty* pProp = get(name);
	if (pProp)
		return static_cast<classProperty_float*>( pProp );
	return 0;
}

//--------------------------------------------------------------
classProperty_bool* classProperties::getBool(string name)
{
	classProperty* pProp = get(name);
	if (pProp)
		return static_cast<classProperty_bool*>( pProp );
	return 0;
}
