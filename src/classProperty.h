//
//  classProperty.h
//  murmur
//
//  Created by Julien on 17/06/2015.
//
//

#pragma once
#include "ofMain.h"
#include "ofxMidi.h"

//--------------------------------------------------------------
class classProperty
{


	public:
		classProperty(string name)
		{
			m_name = name;
		}

		virtual ~classProperty(){}
 
	
		string			m_name;
		int				m_type;
		virtual void	setValueFromMidiMessage	(ofxMidiMessage& ){}

	enum{
		FLOAT		= 0,
		BOOL		= 1
		
	};

};

//--------------------------------------------------------------
class classProperty_float : public classProperty
{
	public:
		classProperty_float(string name, float min, float max, float* pValue);
		classProperty_float(string name, float min, float max);
		~classProperty_float();

		void	setValueFromMidiMessage	(ofxMidiMessage& );

		float*			mp_variable;
		float			m_min;
		float			m_max;
 
		bool			m_ownsVariable;
};

//--------------------------------------------------------------
class classProperty_bool : public classProperty
{
	public:
		classProperty_bool(string name, bool* pValue);

		bool*			mp_variable;
};


//--------------------------------------------------------------
class classProperties
{
	public:
		classProperties		();
		~classProperties	();

		void					add						(classProperty*);
		classProperty*			get						(string name);
		classProperty_float*	getFloat				(string name);
		classProperty_bool* 	getBool					(string name);

	 	map<string, classProperty*> m_list;
};