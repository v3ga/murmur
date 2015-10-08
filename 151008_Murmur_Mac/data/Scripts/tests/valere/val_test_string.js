//--------------------------------------------------------------
function createUI()
{
}

//--------------------------------------------------------------
function eventUI(name, value)
{
	if (name == "...")
	{
	}
}

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
this.volumeSaved = 0;
this.xSaved = 0;
this.ySaved = 0;

   
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
     this.R = of.Random(0,0);
	this.G = of.Random(0,255);
	this.B = of.Random(0,255);
    this.age = 0;
    
    this.Bx_01 = this.xSaved+of.Random(-50,50);
    this.By_01 = this.ySaved+of.Random(-50,50);
    this.Bx_02 = this.Bx_01+of.Random(-50,50);
    this.By_02 = this.By_01+of.Random(-50,50);
    this.Bx_03 = this.Bx_02+of.Random(-50,50);
    this.By_03 = this.By_02+of.Random(-50,50);

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
	of.SetColor(this.R,this.G,this.B, 255*(1-this.age/10.0));
	//of.Circle(this.xSaved, this.ySaved,this.volumeSaved*250)
    //of.SetLineWidth(3);
    //of.NoFill();
    
	of.Curve( this.xSaved, this.ySaved, this.volumeSaved+1*(this.Bx_01), this.volumeSaved+1*(this.By_01), this.volumeSaved+1*(this.Bx_02), this.volumeSaved+1*(this.By_02), this.volumeSaved+1*(this.Bx_03), this.volumeSaved+1*(this.By_03));
}

//--------------------------------------------------------------
function exit()
{
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
    
	if (volume > 0.5)
	{
		this.draw.push( new draw(x,y) );
	}
    
	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
}
