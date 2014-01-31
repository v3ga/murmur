function Sinewave(w,y,freq, nombrePoints)
{
    this.y = y;
    this.freq = freq;
    this.nbPoints = nombrePoints;
    this.points_x = new Array(this.nbPoints);
    this.points_y = new Array(this.nbPoints);
    this.angle = 0;
    this.width = w;
    this.offset = of.Random(0,Math.PI);
    this.offsetSpeed = of.Random(Math.PI/10,Math.PI/2);
    this.amp = of.Random(200,300);
    this.freqMin = of.Random(1,5);
    this.freqMax= this.freqMin+of.Random(0,4);
    
    
    this.modulateFreq = function(t)
    {
            this.freq = this.freqMin+t*(this.freqMax-this.freqMin);
    }

    this.update = function(dt)
    {
        this.offset   += this.offsetSpeed*dt;
        
        var step_x = this.width / (this.nbPoints-1);
        var step_angle = 2*Math.PI/this.nbPoints;
        this.angle=0;
        for (var i=0;i<this.nbPoints;i++)
        {
           var r = 20+this.amp*0.5*(1+Math.sin(this.offset+this.freq*this.angle));
           this.points_x[i] = r*Math.cos(this.angle);     
         this.points_y[i] = r*Math.sin(this.angle);     

//           this.points_x[i] = i*step_x;     
//            this.points_y[i] = y+this.amp*Math.sin(this.offset+this.freq*this.angle);     
           this.angle+=step_angle;
        }
    }
    
    this.draw = function()
    {
        of.SetColor(255,255,255,200);
        for (var i=0;i<this.nbPoints;i++)
        {
            of.Line(this.points_x[i],this.points_y[i],this.points_x[(i+1)%this.nbPoints],this.points_y[(i+1)%this.nbPoints]);
        }
    }
}


//--------------------------------------------------------------
function ligneSon(y, hauteurson){
        this.y = y;
        this.hauteurSonActuelle = hauteurson;
        
        this.draw = function(){
                of.SetColor(128, 255, 255);
                of.SetLineWidth(10);
                of.NoFill();
                //of.Bezier (0, 200, 0, 200, 400, 600, 400, 600); 
                for(var i=0; i<=10; i++){
                    of.Bezier((this.width/10)*i, 300, (this.width/10)*i, 300, (this.width/10)*(i+1), 500, (this.width/10)*(i+1), 500);
                }//this.y*this.hauteurSonActuelle
        }
        
        this.update = function(dt){
        }
}
//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
function setup()
{
    this.sines = null;
//    this.sine = null;
    this.volumeFiltre = 0;
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
    this.volumeFiltre += (this.volumeActuel-this.volumeFiltre)*0.3*dt;

    for (var i=0;i<this.sines.length;i++)
    {
       // this.sines[i].freq = 1+this.volumeFiltre *10;
        this.sines[i].modulateFreq(this.volumeFiltre);
        this.sines[i].update(dt);

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

        if (this.sines == null)
        {
            this.sines = new Array();
            for (var i=0;i<10;i++)
            {
                this.sines.push( new Sinewave(this.width,this.height/2+of.Random(-50,50), 3, parseInt(of.Random(4,8))) );
            }

        }


	//of.Background(0,0,0);
       of.SetColor(0, 0, 0, 20);
       of.Rect(0, 0, w, h);
    of.Translate(this.xCentral, this.yCentral,0);
    for (var i=0;i<this.sines.length;i++)
    {
        this.sines[i].freq = 1+this.volumeFiltre *10;
        this.sines[i].draw();

    }       

	
      //  of.Translate(this.xCentral, this.yCentral, 0);
        //this.premierTest(0, 0, (this.volumeActuel*1000), (this.volumeActuel*100));
}


//--------------------------------------------------------------
// Appel lorsque la surface reçoit un nouveau «packet» lumineux
// deviceId est l'identifiant de la chambre d'Echo
// volume est la valeur du volume transporté, entre 0 et 1
// (x,y) est la position du point d'accroche sur la surface
//
function onNewPacket(deviceId,volume,x,y)
{
    //println(volumeActuel);
    this.volumeActuel = volume;
    this.xCentral = x;
    this.yCentral = y;
    
    if(this.volumeActuel > 0.1){
      
          // this.ligneSon(this.yCentral/2, this.volumeActuel);
    }
}

  