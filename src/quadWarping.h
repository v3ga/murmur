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
		void			setup	(quadWarping* pParent=0, ofTrueTypeFont* pFont=0);
		void			update	();
		void			draw	(int id=-1);
		void			onPress(int x, int y, int button);
		void			onDragOver(int x, int y, int button);
		void			onDragOutside(int x, int y, int button);
		void 			onRelease(int x, int y, int button);		// called when mouse releases while over object
		void 			onReleaseOutside(int x, int y, int button);		// called when mouse releases outside of object after being pressed on object
	
		bool			m_isSelected;

	private:
		ofVec2f			dragDelta;
		ofVec2f			posNormalized;
		quadWarping*	mp_parent;
		ofTrueTypeFont*	mp_font;

};

class quadWarping
{
	public:
								quadWarping			();

		void					disableMouseEvents	();
		void					enableMouseEvents	();
		void					setup				(ofTrueTypeFont* pFont);
		void					update				();
		void					draw				();
		void					save				(string pathFile);
		void					load				(string pathFile);
		bool					hasSelectedHandle	();
		void					moveSelectedHandle	(ofVec2f delta);
		void					unselectHandle		();
		void					resetHandles		();
		void					selectHandle		(quadWarpingHandle* p);
		void					windowResized		(int w, int h);
		void					windowResized		(int wold, int hold,int w, int h);

		float*					findTransformMatrix			(const ofRectangle& src);
		ofMatrix4x4				getTransformMatrix			(const ofRectangle& src, bool bInverse=false);
		ofVec2f					getPointInSquareNormalized	(ofVec2f p);

		quadWarpingHandle		m_handles[4];

	private:
		quadWarpingHandle*		mp_handleSelected;
		float					m_matrixTransform[16];
	
};