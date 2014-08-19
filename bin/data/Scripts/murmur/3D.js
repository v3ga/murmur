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
	
	
	
	of.Background(0,0,0);
	of.NoFill();
	of.DrawBox (this.xSaved,this.ySaved,200,100,this.volumeSaved*100,50);
	of.DrawCylinder (this.xSaved-200,this.ySaved+200,this.volumeSaved*150,100);
	of.DrawCone (this.xSaved+200,this.ySaved,200,this.volumeSaved*100,100);
	of.DrawIcoSphere (this.xSaved+100,this.ySaved+100,this.volumeSaved*150,100);
	of.DrawPlane  (this.xSaved-50,this.ySaved+100,this.volumeSaved*250,200);
	of.DrawSphere   (this.xSaved,this.ySaved,this.volumeSaved*550);
	
	
	
	
	
	
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
