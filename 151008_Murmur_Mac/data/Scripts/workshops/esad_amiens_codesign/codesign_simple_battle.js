//--------------------------------------------------------------
// Variables à utiliser tout au cours du sketch
this.volume = 0;
this.xAccroche = 0;
this.yAccroche = 0;

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
function setup()
{
	
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
	println("coucou")
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
	
	//bleu
	of.SetColor(25,10,170,90);
	of.Triangle(0,0,0,900,1200*this.volume,900*this.volume);
	
	
	//orange
	of.SetColor(250,100,17,90);
	of.Triangle(1200,0,1200,900,0*this.volume,900*this.volume);
	
	
	/*
	//bleu OK
	of.SetColor(25,10,170,90);
	of.Triangle(0,0,0,900,1200,900);
	
	//orange OK
	of.SetColor(250,100,17,90);
	of.Triangle(1200,0,1200,900,0,900);
	*/
	
	
	/*
	//triangle orange et bleu calé sur le "témoin"
	
	of.SetColor(25,10,170);
	of.Triangle(this.xAccroche,this.yAccroche,this.yAccroche,this.yAccroche,900*this.volume,this.xAccroche);
	
	of.SetColor(250,100,17);
	of.Triangle(this.yAccroche,this.xAccroche,this.xAccroche,this.xAccroche,1200*this.volume,this.yAccroche);
*/
	
	
	/*
	//un triangle qui fonctionne
	of.SetColor(25,10,170);
	of.Triangle(100,100,200,200,0,100);
	*/
	
	
	
	// of.SetRectMode(of.CENTER);
	// pour faire un rectMode, diviser
	
	// pour que la forme réagisse en fonction du son, je multiplie sa taille par le 		// volume avec valeur*this.volume.
	
	
	// of.SetColor(240,70,50,90);
	// of.Rect(this.xAccroche, this.yAccroche, 500*this.volume, 500*this.volume);
	// of.SetColor(250,170,150,80);
	// of.Rect(this.xAccroche, this.yAccroche, 50*this.volume, 50*this.volume);	
	
	
	/* 
	// ça fait des carrés de couleurs différentes
	for(i = 0 ; i < 5 ; i++){
	of.SetColor(of.Random(0,255),of.Random(0,255),of.Random(0,255));
	of.Rect(of.Random(0,w), of.Random(0,h), 100*this.volume, 100*this.volume);
	}
	*/
	

	
	/*
	for(i = 0 ; i < 100 ; i++){
	of.SetColor(of.Random(0,255));
	of.Triangle(of.Random(0,h),100*this.volume, of.Random(0,h), 100*this.volume,of.Random(0,h),100*this.volume);
	}
*/
	
	
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
}
