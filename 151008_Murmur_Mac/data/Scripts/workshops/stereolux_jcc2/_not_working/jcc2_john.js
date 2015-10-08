//--------------------------------------------------------------
// Appel au démarrage de l'animation
//
function setup()
{
    this.listParticles = Array();

    this.letters =  ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                     "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"];

    this.a = 0;
}

//--------------------------------------------------------------
// Appel avant le dessin
//
// dt est le temps écoulé depuis le dernier dessin (en seconde)
//
function update(dt)
{
    var p;
   	// Important de parcourir la liste en arrière à cause du «splice»
   	for (var i=this.listParticles.length-1; i>=0 ; i-- )
   	{
   		// Référence sur la particule
   		p = this.listParticles[i];
   		// Mise à jour de la particule
   		p.update(dt);
   		// Trop vieille ? On l'enlève du tableau !
   		if (p.isTooOld()){
   			this.listParticles.splice(i,1);
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
    //of.Background(0,0,0);
	of.SetColor(0,0,0,50);
	of.Rect(0,0,w,h);	
   	var p;
    for (var i=this.listParticles.length-1; i>=0 ; i-- )
   	{
   		p = this.listParticles[i];
        //println(p.color);
        r = (p.color >> 16) & 0xFF;
        g = (p.color >> 8) & 0xFF;
        b = p.color & 0xFF;
   		of.SetColor(r,g,b,255*(1.0-p.age/p.ageMax)); // alpha qui dépend de l'age de la particule
   		p.draw();

       if(i < this.listParticles.length-1) {
           of.Line(previous_p.x, previous_p.y, p.x, p.y);
       }
       previous_p = p;
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
    if (volume>=0.5) {
        n = Math.floor(Math.random()*this.letters.length);
        l = this.letters[n];
        if (typeof l != 'undefined')
            this.listParticles.push( new Letter2D(l, x, y, volume, a) );
        a = (a + 1) % 360;
    }

    p = 21129; //16777215

}
