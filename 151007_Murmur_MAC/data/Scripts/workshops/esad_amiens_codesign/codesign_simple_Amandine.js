//--------------------------------------------------------------
// Variables à utiliser tout au cours du sketch
this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;
this.data = null;
this.myFont = null;
this.mesNoeuds = new Array();
this.radVal = 100;
this.strVal = 5;
this.myAttractor = null;

//--------------------------------------------------------------
function Noeud(x,y) 
{
	this.x = x;
	this.y = y;
	
	this.xDepart = x;
	this.yDepart = y;
		
	this.attractor = new Attractor();
		
	  // vitesse
	this.velocityx = 0;	
	this.velocityy = 0;	

	this.accx = 0;	
	this.accy = 0;	

	
	  // ralentissement
	this.damping = 0.95;

	  // calcule de la nouvelle position
	  this.update = function(dt) 
	  {
		this.attractor.setPosition(this.xDepart, this.yDepart);
        this.attractor.strength = 15;
        this.attractor.radius = 500;
		this.attractor.attract(this);
	  
		this.velocityx += this.accx;
		this.velocityy += this.accy;

		this.velocityx *= (1-this.damping);
		this.velocityy *= (1-this.damping);
		
		this.x += this.velocityx;
		this.y += this.velocityy;

	  }
}

function Attractor()
{
	this.x=0; 
	this.y=0; 

	this.radius = 500;
	this.strength = 1;  
	
	this.ramp = 0.6;

	this.setPosition = function(x,y)
	{
		this.x=x; 
		this.y=y; 
	}
	
	this.attract = function(noeud)
	{
		var dx = this.x - noeud.x;
		var dy = this.y - noeud.y;
		var d = Math.sqrt(dx*dx+dy*dy);
	
		// calcule de la force
		var st;
		var fo;
	
		if (d > 0 && d < this.radius) {
		
			var st = Math.pow(d / this.radius, 1/this.ramp); //force global
			var fo = st * 9 * this.strength * (1 / (st + 1) + ((st - 3) / 4)) / d; //force appliqué au noeud après le calcul

		// apply force to node velocity
//			noeud.velocityx += dx * fo;
//			noeud.velocityy += dy * fo;

			noeud.accx += dx * fo;
			noeud.accy += dy * fo;

			}


	}
		
}


//--------------------------------------------------------------

function setupNoeuds()
{
	var f = new File("../../../data/Scripts/workshops/esad_amiens_codesign/data/text.txt");
	f.open("read", "text");
	this.data = f.readln();
	f.close();
	println(this.data);

	var margin = 270;
	var posX=margin; 
	var posY=20;
	for (i = 0; i < 350; i++) {
		mesNoeuds.push( new Noeud(posX, 150+posY) );
		posX+=26;
    
		if (posX>960) {
		posX=margin; 
		posY+=50;
		}
	}		
}

// Appel au démarrage de l'animation
//
function setup()
{	
	this.myFont = new ofxJSTrueTypeFont();
	this.myFont.loadFont("Scripts/workshops/esad_amiens_codesign/data/DroidSansMono.ttf",24);
	this.setupNoeuds();
	this.myAttractor = new Attractor();
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
		for(i=0; i<this.mesNoeuds.length;i++)
		{
			this.mesNoeuds[i].update(dt);
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
//	of.SetColor(255,100,0,10);
//	of.Circle((this.xAccroche-50), (this.yAccroche-50), 200*this.volume);
	
	of.SetColor(255,255,255,255);
	
	if (this.myFont)
	{
//			this.myFont.drawString("HEY",40,40); // ne marche pas ?? ? ? ?

		var c;
		for(i=0; i<this.mesNoeuds.length;i++)
		{
			c = this.data.charAt(i);
			this.myFont.drawString(c,this.mesNoeuds[i].x, this.mesNoeuds[i].y);
		
			//of.Ellipse(this.mesNoeuds[i].x, this.mesNoeuds[i].y, 10,10);	
	
		}
	}
	
    for (var i = 0; i < mesNoeuds.length; i++) 
	{
        myAttractor.strength = -this.strVal;
        myAttractor.attract(this.mesNoeuds[i]);
    }	
	    
	
}


//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transporté, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//
function onNewPacket(deviceId,volume,x,y)
{
	this.volume = volume;
	this.xAccroche = x;
	this.yAccroche = y;
	
	this.myAttractor.setPosition(x,y);
	this.myAttractor.radius = volume*400;
}
