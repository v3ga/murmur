//
//  ofAppLog.h
//  A_P_P_A_R_E_L
//
//  Created by Julien on 15/07/2014.
//
//

#pragma once
#include "ofMain.h"

#define OFAPPLOG ofAppLog::getInstance()

class ofAppLog
{
	public:

		static 	ofAppLog*	getInstance();
		static 	void		destroy		();
				void		begin		(string s);
				void		end			();
				void		println		(ofLogLevel level, const string & message);
				void		println		(const string & message);
	
				template <class T>
				ofAppLog* operator<<(const T& value)
				{
					ofLog() << space(m_level) << value;
					return this;
				}
	
	private:
	   	static ofAppLog* 	smp_instance;
		int					m_level;

		string				space		(int nb);
};

