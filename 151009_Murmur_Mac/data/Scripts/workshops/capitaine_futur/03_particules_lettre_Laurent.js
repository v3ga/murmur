//--------------------------------------------------------------
// On étend les fonctionnalités de la class Particle2D
// en ajoutant un membre et deux fonctions à son prototype
// 
// 
Particle2D.prototype.text = "a";
//
Particle2D.prototype.setColor = function(color)
{
	this.color = color;
}

Particle2D.prototype.setText = function(text)
{
	this.text = text;
}
//
Particle2D.prototype.drawText = function(font)
{
	if(font)
	{
		font.drawString(this.text,this.x, this.y); // attention, «this» fait référence aux objets de type Particle2D, et non le sketch en cours
	}
}

//--------------------------------------------------------------
// Déclaration des variables
// (bien noter le mot-clé «this»)
//
this.heigth = 0;
this.weight = 0;
this.particules = [];
this.font = null;
this.text = "Du son à la lumière par le code";
this.textIndex = 0;

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//  
function setup()
{
	// Chargement d'une fonte dans le dossier
	// Pour l'instant, le dossier data est partagé par tous les scripts
	// 
	this.font = new ofxJSTrueTypeFont();
	this.font.loadFont("Scripts/workshops/capitaine_futur/data/DroidSansMono.ttf",24);
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
		var p = this.particules[i];

		if (p.x < 0 || p.x > this.width || p.y < 0 || p.y > this.height) {
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
  this.width = w;
  this.height = h;
	// Dessin du fond
	of.Background(0,0,0);

	// Dessin des particules en parcourant la liste this.particules
	for (var i=0; i<this.particules.length; i++)
	{
		of.SetColor.apply(of, this.particules[i].color);
		this.particules[i].drawText(this.font);
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
	// Si le volume est supérieur à une certaine valeur
	// on «émet» une particule en la sauvegardant dans le tableau this.particules
	// 
	// Particle2D est défini dans le fichier data/libraries/murmur/Particle2D.js
	//
	if (volume > 0.5)
	{
		var p = new Particle2D(x,y);
		var dict = [
			'waffle', 'so', 'sketchy', 'such', 'very',
			'I', 'want', 'don\'t', 'please', 'give', 'hurry'
		];
		var i = Math.floor(Math.random() * 100) % dict.length;

		//SetColor( r, g, b);

		p.setText(dict[i]);
		p.setColor([
	  	Math.floor(Math.random() * 255),
	  	Math.floor(Math.random() * 255),
	  	Math.floor(Math.random() * 255),
		]);

		this.particules.push( p );
		this.textIndex = (this.textIndex+1)%this.text.length;		
	}
}

//--------------------------------------------------------------
// Appelé une seule fois lorsque le script est chargé
// Permet de créer des éléments d'interface (sliders et toggles)
//
function createUI()
{
	// this.newSlider("diameter", 0,1000,500);
}

//--------------------------------------------------------------
// Fonction qui reçoit les évènements utilisateur lorsque
// les contrôles sont manipulés depuis l'interface
// 
function eventUI(name, value)
{
/*
	if (name == "diameter")
		this.diameter = value;
*/
}