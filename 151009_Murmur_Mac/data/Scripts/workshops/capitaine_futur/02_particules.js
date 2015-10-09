//--------------------------------------------------------------
// Déclaration des variables
// (bien noter le mot-clé «this»)
//
this.particules = []; // new Array
this.distance = 120;

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
	for (var i=0; i<this.particules.length; i++)
	{
		this.particules[i].update(dt);
	}

	// Test : certaines de particules sont-elles trop vieilles ?
	// Oui, on les enlève de notre liste
	for (var i=this.particules.length-1; i>=0; i--)
	{
		if (this.particules[i].isTooOld())
		{
			this.particules.splice(i,1);			
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
	of.EnableAlphaBlending();
	// Dessin des particules en parcourant la liste this.particules
	of.SetColor(255,255,255,100);
	for (var i=0; i<this.particules.length; i++)
	{
		//this.particules[i].draw();
	}

	var pi,pj;
	of.SetLineWidth(3);
	for (var i=0; i<this.particules.length; i++)
	{
		pi = this.particules[i];
		for (var j=i+1; j<this.particules.length; j++)
		{
			pj = this.particules[j];
			if ( of.Dist(pi.x,pi.y, pj.x,pj.y) <= this.distance )
			{
				of.Line( pi.x,pi.y, pj.x,pj.y );
			}

		}

	}
}
// ofNoise(x)
//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
//
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transporté, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//
function onNewPacket(deviceId,volume,x,y)
{
	// Si le volume est supérieur à une certaine valeur
	// on «émet» une particule en la sauvegardant dans le tableau this.particules
	// 
	// Particle2D est défini dans le fichier data/libraries/murmur/Particle2D.js
	//
	if (volume > 0.5)
	{
		var p = new Particle2D(x,y);
		p.x = x + (Math.random()-0.5)*100;
		p.y = y + (Math.random()-0.5)*100;
		p.ageMax = 10;
		this.particules.push( p );
	}
}

//--------------------------------------------------------------
// Appelé une seule fois lorsque le script est chargé
// Permet de créer des éléments d'interface (sliders et toggles)
//
function createUI()
{
	this.newSlider("distance", 40, 300, 120);
}

//--------------------------------------------------------------
// Fonction qui reçoit les évènements utilisateur lorsque
// les contrôles sont manipulés depuis l'interface
// 
function eventUI(name, value)
{
	if (name == "distance")
		this.distance = value;
}