//
//  toolAnimations.h
//  murmur
//
//  Created by Julien on 05/06/2014.
//
//

#pragma once
#include "tool.h"

class Animation;
class Surface;
class toolAnimations : public tool
{
	public:

		toolAnimations			(toolManager* parent);

	
		void							createControlsCustom		();
		void							createControlsAnimations	(Surface*);
		void							handleEvents				(ofxUIEventArgs& e);
		void							logJS						(void* pData, const string& message);
		void							keyPressed					(int key);
		void							show						(bool is);
		void 							enableDrawCallback			(bool is);
	

		void							updateDeviceAnimationTitle	();
		void							showAnimationPropsAll		(bool is=true);

		ofxUILabel* 					mp_lblAnimDirJs;
		ofxUILabel* 					mp_lblAnimTitle;
		ofxUITextArea*					mp_consoleJs;
		ofxUICanvas*					mp_animationUI;

        map<Animation*, ofxUICanvas*> 	m_mapAnimationUI;

		vector<string>					m_listLogJs;
		int								m_listLogJsMax;
};
