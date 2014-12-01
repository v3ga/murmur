// Allo-Halo
// 
// http://gaite-lyrique.net/atelier/allo-halo-courts-circuits-lumineux
//

Device.prototype.colorA  			= {hue:0,saturation:0};
Device.prototype.colorB  			= {hue:0,saturation:0};
Device.prototype.color  			= {hue:0,saturation:0};
Device.prototype.colorPhase 		= Math.random()*360.0;
Device.prototype.colorPhaseSpeed 	= Math.random()*20.0+10; // degrés par seconde
Device.prototype.time 				= 0;

Device.prototype.onCreate = function()
{
	// Choisit les couleurs en fonction de l'id
	if (this.id == "deviceEchoSimulator01" || this.id == "chambreEcho_002")
	{
		this.colorA = {hue:60, saturation:255};
		this.colorB = {hue:255, saturation:255};
	}
	else if (this.id == "deviceEchoSimulator02")
	{
		this.colorA = {hue:60, saturation:40};
		this.colorB = {hue:120, saturation:200};
	}
	else if (this.id == "deviceEchoSimulator03")
	{
		this.colorA = {hue:60, saturation:100};
		this.colorB = {hue:120, saturation:255};
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
		getDevice(deviceId).update(dt);
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
	for (var deviceId in deviceManager)
	{
		var device = getDevice(deviceId);

		of.PushMatrix();
		of.Translate(device.accroche.x, device.accroche.y,0);
		device.setDeviceColor();
		of.Rect(0-150,0-150,300,300);
		of.PopMatrix();
	}
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
