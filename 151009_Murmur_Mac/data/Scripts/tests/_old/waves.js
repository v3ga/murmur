//--------------------------------------------------------------
// Configuration
this.waveNbPoints = 12;
this.waveAgeMax = 10; // seconds
this.drawConnections = true;
this.waveRotationSpeed = 10.0; // degrees per second
this.waveLineWidth = 2;
this.waveDirAmp = 0.5;

//--------------------------------------------------------------
this.volumeAccum = 0;
this.volumeAccumTarget = 0;
this.waveRotation =  0;

//--------------------------------------------------------------
function getTheme(){ return THEME_CREATURE; }

//--------------------------------------------------------------
function setup()
{
	this.waves = Array();
    this.waveNbPoints = parseInt( Math.random()*10 )+3;
    this.sndPlayer = new SoundPlayer( new Array("waves1.wav","waves2.wav","waves3.wav","waves4.wav","waves5.wav","waves6.wav","waves7.wav","waves8.wav") );
    this.waveRotationSpeed = Math.random()*10.0-10.0;
}

//--------------------------------------------------------------
function createUI()
{
    this.newSlider("waveDirAmp",0,1,this.waveDirAmp);
    this.newSlider("vol. trigger in",0.0,1.0,0.1);
    this.newSlider("vol. trigger out",0.0,1.0,0.1);
}

//--------------------------------------------------------------
function eventUI(name, value)
{
    if (name == "waveDirAmp")
    {
        this.waveDirAmp = value;
    }
    else if (name == "vol. trigger in")
    {
        if (this.volumeAccum)
            this.volumeAccum.valueTriggerIn = value;
    }
    else if (name == "vol. trigger out")
    {
        if (this.volumeAccum)
            this.volumeAccum.valueTriggerOut = value;
    }
}

//--------------------------------------------------------------
function draw(w,h)
{
	of.Background(0,0,0);
	of.SetLineWidth(this.waveLineWidth);

	for (var i=0 ; i<this.waves.length ; i++)
	{
		of.SetColor(255,255,255,255*this.waves[i].getAlpha());
		this.waves[i].draw();
	}

	if (this.drawConnections && this.waves.length>=2)
	{
		var w1,w2;
		var w1a,w2a;
		var nbPoints;
		var p1,p2;
		for (var i=0 ; i<this.waves.length-1 ; i++)
		{
			w1 = this.waves[i];
			w2 = this.waves[i+1];
			w1a = w1.getAlpha();
			w2a = w2.getAlpha();
			nbPoints = w1.points.length;
			gl.Begin(GL_LINES);
			for (var j=0;j<nbPoints;j++)
			{
				p1 = w1.points[j];
				p2 = w2.points[j];

				gl.Color4(1,1,1,w1a);gl.Vertex2(p1.x,p1.y);
				gl.Color4(1,1,1,w2a);gl.Vertex2(p2.x,p2.y);
			}
			gl.End();
		}
	}

	of.SetLineWidth(1);
}

//--------------------------------------------------------------
function update(dt)
{
	this.waveRotation += this.waveRotationSpeed*dt;

	var w;
	for (var i=this.waves.length-1; i>=0 ; i-- )
	{
		w = this.waves[i];
        w.setPhase( of.DegToRad(this.waveRotation) );
		w.update(dt);
		if (w.isOld()){
			this.waves.splice(i,1);
		}
	}

}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
    this.volumeAccum = getDevice(deviceId).volumeAccum;
    getDevice(deviceId).accumVolume( volume, { f:volAccumEvent, obj:this, params:{x:x, y:y} } );
}

//--------------------------------------------------------------
function volAccumEvent(params)
{
    this.sndPlayer.playRandom(0,1);

    var w = new Wave(params.x,params.y,this.waveNbPoints);
    w.setDir( Math.random()*this.waveDirAmp-this.waveDirAmp/2, Math.random()*this.waveDirAmp-this.waveDirAmp/2 );
    w.setAgeMax(this.waveAgeMax);
    this.waves.push(w);
}
