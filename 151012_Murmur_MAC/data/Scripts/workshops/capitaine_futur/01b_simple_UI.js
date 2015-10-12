//--------------------------------------------------------------
// Déclaration des variables
// (bien noter le mot-clé «this»)
//
this.accrocheX = 0;
this.accrocheY = 0;
this.volume = 0;
this.diameter = 500;
this.doDrawing = 1;
this.R = 0;

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
	if (this.doDrawing>0)
	{
		of.SetColor(this.R,120,255);
		of.Ellipse(this.accrocheX, this.accrocheY, this.diameter, this.diameter);
	}
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

//--------------------------------------------------------------
// Appelé une seule fois lorsque le script est chargé
// Permet de créer des éléments d'interface (sliders et toggles)
//
function createUI()
{
	this.newSlider("diameter", 0,1000,500);
	this.newSlider("R", 0,255,100);
	this.newToggle("doDrawing", 1);
}

//--------------------------------------------------------------
// Fonction qui reçoit les évènements utilisateur lorsque
// les contrôles sont manipulés depuis l'interface
// 
function eventUI(name, value)
{
	if (name == "diameter")
		this.diameter = value;
	else if (name == "doDrawing")
		this.doDrawing = value;
	else if (name == "R")
		this.R = value;
}

