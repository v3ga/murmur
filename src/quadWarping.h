//
//  quadWarping.h
//  murmur
//
//  Created by Julien on 01/07/2014.
//
//

#pragma once

#include "ofxMSAInteractiveObject.h"

class quadWarping;
class quadWarpingHandle : public ofxMSAInteractiveObject
{
	public:
		quadWarpingHandle		();
	
	
		bool			hitTest	(int x, int y);
		void			setup	(quadWarping* pParent=0);
		void			draw	();
		void			onPress(int x, int y, int button);
		void			onDragOver(int x, int y, int button);
		void			onDragOutside(int x, int y, int button);
		void 			onRelease(int x, int y, int button);		// called when mouse releases while over object
		void 			onReleaseOutside(int x, int y, int button);		// called when mouse releases outside of object after being pressed on object
	
		bool			m_isSelected;
	private:
		ofVec2f			dragDelta;
		quadWarping*	mp_parent;
};

class quadWarping
{
	public:

		void					disableMouseEvents	();
		void					enableMouseEvents	();
		void					setup				();
		void					draw				();
		void					save				(string pathFile);
		void					load				(string pathFile);
		void					moveSelectedHandle	(ofVec2f delta);
		void					selectHandle		(quadWarpingHandle* p);

		float*					findTransformMatrix	(const ofRectangle& src);

		quadWarpingHandle		m_handles[4];

	private:
		quadWarpingHandle*		mp_handleSelected;
		float					m_matrixTransform[16];
	
};