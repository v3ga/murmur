//--------------------------------------------------------------
function createUI()
{
}

//--------------------------------------------------------------
function eventUI(name, value)
{
	if (name == "...")
	{
	}
}

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
this.volumeSaved = 0;
this.xSaved = 0;
this.ySaved = 0;

function setup()
{
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
}

//--------------------------------------------------------------
// Appel pour le dessin sur la surface
//
// (w,h) sont les dimensions de la surface en pixels
//
// fonction de dessin (et autres)
// https://code.google.com/p/ofxjavascript/wiki/ofxJSGlobalFunctions
//
function draw(w,h)
{
	
	
	of.BackgroundGradient (255,0,0,0,0,255,OF_GRADIENT_BAR);
	of.SetColor(255,0,0);
	of.Circle(this.xSaved+100, this.ySaved,this.volumeSaved*250)
	of.SetColor(0,0,255);
	of.Circle(this.xSaved-100, this.ySaved,this.volumeSaved*250)
	
	
	of.SetColor(255,255,255);
//of.NoFill();
	//of.SetLineWidth (10);
	
	
	of.SetPolyMode(OF_POLY_WINDING_ODD);
	of.BeginShape();
	of.Vertex(this.xSaved-200,this.ySaved);
	of.Vertex(this.xSaved-150,this.ySaved-150);
	of.Vertex(this.xSaved-50,this.ySaved-200);
	of.Vertex(this.xSaved+100,this.ySaved-100);
	
	of.NextContour(false);
	
	of.Vertex(this.xSaved+150*this.volumeSaved,this.ySaved*this.volumeSaved);
	of.Vertex(this.xSaved+100*this.volumeSaved,this.ySaved+150*this.volumeSaved);
	of.Vertex(this.xSaved*this.volumeSaved,this.ySaved+50*this.volumeSaved);
	of.Vertex(this.xSaved-100*this.volumeSaved,this.ySaved*this.volumeSaved);
	
	of.NextContour(false); 
	
	of.Vertex(this.xSaved+200,this.ySaved);
	of.Vertex(this.xSaved+150,this.ySaved+150);
	of.Vertex(this.xSaved+50,this.ySaved+200);
	of.Vertex(this.xSaved-100,this.ySaved+100);
	
	of.NextContour(false); 
	
	of.Vertex(this.xSaved-40*(this.volumeSaved*2),this.ySaved-40*(this.volumeSaved*2));
	of.Vertex(this.xSaved,this.ySaved-50*(this.volumeSaved*4));
	of.Vertex(this.xSaved+40*(this.volumeSaved*2),this.ySaved-40*(this.volumeSaved*2));
	of.Vertex(this.xSaved+50*(this.volumeSaved*2),this.ySaved);
	of.Vertex(this.xSaved+40*(this.volumeSaved*2),this.ySaved+40*(this.volumeSaved*2));
	of.Vertex(this.xSaved,this.ySaved+50*(this.volumeSaved*4));
	of.Vertex(this.xSaved-40*(this.volumeSaved*2),this.ySaved+40*(this.volumeSaved*2));
	of.Vertex(this.xSaved-50*(this.volumeSaved*2),this.ySaved);
	
	of.EndShape();
	
	
}

//--------------------------------------------------------------
function exit()
{
}

//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
//
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transporté, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//
function onNewPacket(deviceId,volume,x,y)
{
	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
}
