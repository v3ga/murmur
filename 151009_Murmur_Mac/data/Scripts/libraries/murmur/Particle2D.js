// «Classe» définie en js
// L'inclusion de ce fichier doit figurer dans le fichier murmur.js

function Particle2D(x,y)
{

	// ------------------------------------------------
	// Membres
	this.x = x;
	this.y = y;
	this.age = 0;
	this.ageMax = Math.random()*3+1; // secondes
	this.diameter = Math.random()*50+10;

	this.vMax = 150;

	this.vx = (Math.random()-0.5)*this.vMax; // pixels / seconde
	this.vy = (Math.random()-0.5)*this.vMax; // pixels / seconde

	// ------------------------------------------------
	this.update = function(dt)
	{
		// Mise à jour de la position
		this.x += this.vx*dt;
		this.y += this.vy*dt;

		// On incrémente son age
		this.age+=dt;
	}

	// ------------------------------------------------
	// dessin grâce à l'api de ofxJavascript
	// voir >>> https://code.google.com/p/ofxjavascript/wiki/ofxJSGlobalFunctions
	this.draw = function()
	{
		of.Ellipse(this.x, this.y, this.diameter, this.diameter);
	}

	// ------------------------------------------------
	// Nous renseigne si la particule est trop vieille
	this.isTooOld = function()
	{
		return (this.age>=this.ageMax);
	}


}