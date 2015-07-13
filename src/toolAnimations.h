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

	
		void							setup						();
		void							drawUI						();
		void							createControlsCustom		();
		void							createControlsAnimations	(Surface*);
		void							handleEvents				(ofxUIEventArgs& e);
		bool							isHit						(int x, int y);
		static void						logJS						(void* pData, const string& message);
		bool							keyPressed					(int key);
		void							show						(bool is);
		void 							enableDrawCallback			(bool is);
		void							showPrevAnimation			();
		void							showNextAnimation			();
		bool							isSequenceActive			();

		void							updateDeviceAnimationTitle	();
		void							showAnimationPropsAll		(bool is=true);
		void							showAnimationCurrentProperties();

		void							enableWindowCallbacks		();
		void							disableWindowCallbacks		();

		void							initMidiAnimations			(Surface*);

		ofxUILabel* 					mp_lblAnimDirJs;
		ofxUILabel* 					mp_lblAnimTitle;
		ofxUITextArea*					mp_consoleJs;
		ofxUICanvas*					mp_animationUI;
		bool							m_isEnableDrawCallback;

        map<Animation*, ofxUICanvas*> 	m_mapAnimationUI;

		vector<string>					m_listLogJs;
		int								m_listLogJsMax;
};
