//--------------------------------------------------------------
// Déclaration des variables
// (bien noter le mot-clé «this»)
//
this.accrocheX = 0;
this.accrocheY = 0;
this.volume = 0;

this.oldVolume = [];
this.time = 0;

this.number_of_lines = 21;
this.noise_velocity = 0.6;
this.noise_angle = 0.2;


//--------------------------------------------------------------
// Appel au démarrage de l'animation
//  
function setup()
{
	of.Background(0,0,0);
	for(var i=0;i<100;i++){
		this.oldVolume.push(0);
	}
}

//--------------------------------------------------------------
// Appel avant la fonction draw(w,h)
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{


	this.oldVolume.push(this.volume);
	this.oldVolume.splice(0,1);
	this.time = this.time +dt*this.noise_velocity;
}


function createUI()
{
	this.newSlider("Number of lines", 1,40,21);
	this.newSlider("Noise angle (rad)",0,0.5,0.2);
	this.newSlider("Noise propagation velocity",0,2,0.6);
}

function eventUI(name, value)
{
	if (name == "Number of lines")
		this.number_of_lines = Math.floor(value);
	else if (name == "Noise angle (rad)")
		this.noise_angle = value;
	else if (name == "Noise propagation velocity")
		this.noise_velocity = value;
}

function draw_line(arr, max_dist,ligne){
	var npoints = arr.length;
	
	var r1=0;
	var r2=0;
	var t1=0;
	var t2=0;
	var rand_theta = 0;
	
	for(var i=0;i<arr.length-1;i++){
		of.SetColor(255,255,255,255*((npoints - (i+10))/npoints));
		of.SetLineWidth(10*((npoints-(i+10))/npoints));
		//position of line in radial coordinates
		r1 = i/npoints*max_dist;
		r2 = (i+1)/npoints*max_dist;
		rand_theta = this.noise_angle*ofNoise(ligne,this.time+i/npoints);
		t1 = rand_theta+ i/npoints*arr[arr.length - 1 -i];
		t2 = rand_theta+ (i+1)/npoints*arr[arr.length - 1 -(i+1)];
		of.Line(r1*Math.cos(t1),r1*Math.sin(t1),r2*Math.cos(t2),r2*Math.sin(t2));
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
	
	//of.Background(0,0,0);
	of.EnableAlphaBlending();
	of.SetColor(0,0,0,30);
	of.Rect(0,0,w,h);
	
	var nlines = this.number_of_lines;
	
	for(var ktemp = 0;ktemp<nlines;ktemp++){
		
		of.PushMatrix();
		of.Translate(this.accrocheX,this.accrocheY,0);
		of.RotateZ(ktemp/nlines*360);
		draw_line(this.oldVolume,h*0.45,ktemp);
		of.PopMatrix();
	}
	of.SetColor(0,0,0,255);
	of.Ellipse(this.accrocheX,this.accrocheY,20,20);
	
	
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
