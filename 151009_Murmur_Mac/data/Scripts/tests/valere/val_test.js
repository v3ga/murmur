this.sizeCircle = 250;
this.bold = 1;
//--------------------------------------------------------------
function createUI()
{
    this.newSlider("size Circle",0,500,this.sizeCircle);
	this.newSlider("Line Width",0,50,this.bold);
    this.newToggle("test",0);
}

//--------------------------------------------------------------
function eventUI(name, value)
{
	    if (name == "size Circle")
    {
        this.sizeCircle = value;
    }
	else if (name == "Line Width")
    {
        this.bold = value;
    }
	else if (name == "test")
	{
		of.NoFill() || of.Fill();
		
	}
}

//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
//println("son" + this.mapson);

//this.volumeSaved = 0;
this.xSaved = 0;
this.ySaved = 0;

// this.tableaucolor = new Array();
this.tableaucolor = ["0xff98b3", "0x98edff", "0x98ff9a", "0xffd798", "0x85a7ff"];
/*
this.c = {
r : 255,
g : 233,
b : 125,
unevaleur : 12345
}
*/
function getRandomColor()
{
    var nb = this.tableaucolor.length;
    var index = parseInt( of.Clamp( of.Random(0, nb), 0, nb-1) );
    return parseInt( this.tableaucolor[index] );
}


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
//    var colorRandom = this.getRandomColor();
//    println(colorRandom);
	this.mapson = of.Map(this.volumeSaved, 0.0,0.9,2.00,15.00);
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
	 
	of.SetCircleResolution(this.mapson);
	 //of.Background(0,0,0);
	of.SetHexColor( this.getRandomColor() );
	// of.SetColor(40);
	of.SetLineWidth(this.bold);
	
	
	
	of.NoFill();
	
	
	of.Circle(this.xSaved, this.ySaved,this.volumeSaved*this.sizeCircle)
	//of.SetHexColor( this.getRandomColor() );
	//of.Circle(this.xSaved-30, this.ySaved,this.volumeSaved*500)
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
	this.volumeSaved = volume;
	this.xSaved = x;
	this.ySaved = y;
	//println(this.volumeSaved);
	
}


