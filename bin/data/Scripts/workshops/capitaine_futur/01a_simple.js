//--------------------------------------------------------------
// Déclaration des variables
// (bien noter le mot-clé «this»)
//
this.accrocheX = 0;
this.accrocheY = 0;
this.volume = 0;
this.diameter = 500;

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//  
function setup()
{
}

//--------------------------------------------------------------
// Appel avant la fonction draw(w,h)
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
	this.diameter = this.volume * 700;
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
	of.SetColor(255,255,255);
	of.Ellipse(this.accrocheX, this.accrocheY, this.diameter, this.diameter);
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
	this.accrocheX = x;
	this.accrocheY = y;
	this.volume = volume;
}
