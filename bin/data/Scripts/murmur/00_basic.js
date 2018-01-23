// Allo-Halo
// 
// http://gaite-lyrique.net/atelier/allo-halo-courts-circuits-lumineux
//
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
	of.Background(255,0,0);
}

//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
// de la part d'un coquillage dont l'identifiant est «deviceId»
//
function onNewPacket(deviceId,volume,x,y)
{
}
