// Allo-Halo
// 
// http://gaite-lyrique.net/atelier/allo-halo-courts-circuits-lumineux
//

Device.prototype.colorA  			= {hue:0,saturation:0};
Device.prototype.colorB  			= {hue:0,saturation:0};
Device.prototype.color  			= {hue:0,saturation:0};
Device.prototype.colorPhase 		= Math.random()*360.0;
Device.prototype.colorPhaseSpeed 	= Math.random()*50.0+40; // degrés par seconde
Device.prototype.time 				= 0;

Device.prototype.onCreate = function()
{
	// Choisit les couleurs en fonction de l'id
	if (this.id == "deviceEchoSimulator01")
	{
		this.colorA = {hue:127, saturation:255};
		this.colorB = {hue:255, saturation:255};
	}
	else
	{
		this.colorA = {hue:Math.random()*230+25, saturation:255};
		this.colorB = {hue:Math.random()*230+25, saturation:255};
	}
}

Device.prototype.update = function(dt)
{
	this.time += dt;

	var angle = this.colorPhaseSpeed*this.time + this.colorPhase;
	var cos_angleRad = Math.sin( of.DegToRad(angle) );

	this.color.hue = of.Map( cos_angleRad,-1,1,this.colorA.hue,this.colorB.hue );
	this.color.saturation = of.Map( cos_angleRad,-1,1,this.colorA.saturation,this.colorB.saturation );

	this.setColorHueSaturation(this.color.hue, this.color.saturation);
}


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
	// Pour chaque device
	for (var deviceId in deviceManager)
	{
		var device = getDevice(deviceId);
		device.update(dt);
	}
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
}

//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
// de la part d'un coquillage dont l'identifiant est «deviceId»
//
function onNewPacket(deviceId,volume,x,y)
{
	var device = getDevice(deviceId);
	if (device){
		device.setAccroche(x,y);
		device.setVolume(volume);
	}
}
