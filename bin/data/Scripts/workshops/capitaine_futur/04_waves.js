//--------------------------------------------------------------
// Déclaration des variables
// (bien noter le mot-clé «this»)
//
this.waves = [];
this.waveDirAmp = 0.5;
this.waveAgeMax = 6; // seconds
this.volumeAccum = new VolumeAccum();

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
	// Mise à jour des paramètres de toutes les particules émises
	for (var i=0; i<this.waves.length; i++)
	{
		this.waves[i].update(dt);
	}

	// Test : certaines de particules sont-elles trop vieilles ?
	// Oui, on les enlève de notre liste
	for (var i=this.waves.length-1; i>=0; i--)
	{
		if (this.waves[i].isTooOld())
		{
			this.waves.splice(i,1);			
		}
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
	// Dessin du fond
	of.Background(0,0,0);

	// Active le alpha blending
	of.EnableAlphaBlending();

	// Largeur des lignes à (n) pixels
	of.SetLineWidth(3);

	// Dessin des particules en parcourant la liste this.particules
	for (var i=0; i<this.waves.length; i++)
	{

		of.SetColor(255,255,255, 255*(1.0-this.waves[i].age/this.waves[i].ageMax));
		this.waves[i].draw();
	}
	of.SetLineWidth(1);
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
	// Si le volume accumulé a dépassé une certaine valeur
	// alors on déclenche l'appel à la fonction «volAccumEvent» en lui passant des 
	// paramètres propres au «device» qui a provoqué cet évènement
	this.volumeAccum.add( volume,  { f:volAccumEvent, obj:this, params:{x:x, y:y, deviceId:deviceId} });
}

//--------------------------------------------------------------
function volAccumEvent(params)
{
	var dim = parseInt(Math.random()*10+3);

    var wave = new Wave(params.x,params.y, dim);
	wave.radiusSpeed = 100.0;

    wave.setDir( Math.random()*this.waveDirAmp-this.waveDirAmp/2, Math.random()*this.waveDirAmp-this.waveDirAmp/2 );
    wave.setAgeMax(this.waveAgeMax);

    this.waves.push( wave );
}

