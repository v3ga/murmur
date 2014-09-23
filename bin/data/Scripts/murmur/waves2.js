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
this.mapWaveManager = {};
this.waveMono = null;

//--------------------------------------------------------------
this.mode = 1; // 0 = one wave for all , 1 = one wave per device

//--------------------------------------------------------------
function WaveManager(anim_)
{
	this.anim = anim_;

	this.waves = Array();
    this.waveNbPoints = parseInt( Math.random()*10 )+3;
    this.sndPlayer = new SoundPlayer( new Array() );
    this.waveRotationSpeed = Math.random()*10.0-10.0;
	this.waveRotation =  0;

	this.update = function(dt)
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


	this.draw = function()
	{
	  for (var i=0 ; i<this.waves.length ; i++)
	  {
		  of.SetColor(255,255,255,255*this.waves[i].getAlpha());
		  this.waves[i].draw();
	  }

	  if (this.anim.drawConnections && this.waves.length>=2)
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
			  //println("wave "+i+" & wave "+(i+1));
			  for (var j=0;j<nbPoints;j++)
			  {
				  p1 = w1.points[j];
				  p2 = w2.points[j];
				//   println("p1:"+p1.x+"-"+p1.y);
				//   println("p2:"+p2.x+"-"+p2.y);
				if (p2.x>0)
				{

				  gl.Color4(1,1,1,w1a);gl.Vertex2(p1.x,p1.y);
				  gl.Color4(1,1,1,w2a);gl.Vertex2(p2.x,p2.y);
			  	}
			  }
			  gl.End();
		  }
	  }
	
	}

}

//--------------------------------------------------------------
function setup()
{
	this.waveMono = new WaveManager(this);
}

//--------------------------------------------------------------
function soundsChanged()
{
	if (this.mode > 0)
	{
		for (var deviceId in this.mapWaveManager)
			this.mapWaveManager[deviceId].sndPlayer.setSoundsName( getSoundsForAnimation(deviceId, "waves2.js") );
	}
	else{
			this.waveMono.sndPlayer.setSoundsName( getSoundsForAnimation(deviceId, "waves2.js") );
	}
}



//--------------------------------------------------------------
function createUI()
{
    this.newSlider("waveDirAmp",0,1,this.waveDirAmp);
    this.newSlider("vol. trigger in",0.0,1.0,0.1);
    this.newSlider("vol. trigger out",0.0,1.0,0.1);
    this.newToggle("multi",0);
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
	else
    if (name == "multi")
	{
		this.mode = value;
		println("eventUI, mode="+value);
	}
}

//--------------------------------------------------------------
function draw(w,h)
{
	of.Background(0,0,0);
	of.SetLineWidth(this.waveLineWidth);

	if (this.mode > 0)
	{
		for (var deviceId in this.mapWaveManager)
			this.mapWaveManager[deviceId].draw();
	}
	else
		this.waveMono.draw();

	of.SetLineWidth(1);
}

//--------------------------------------------------------------
function update(dt)
{
	if (this.mode > 0)
	{
		for (var deviceId in this.mapWaveManager)
		this.mapWaveManager[deviceId].update(dt);
	}
	else
		this.waveMono.update(dt);
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
	if (this.mode > 0)
	{
		if (this.mapWaveManager[deviceId] == null)
			this.mapWaveManager[deviceId] = new WaveManager(this);
	}

    this.volumeAccum = getDevice(deviceId).volumeAccum;
    getDevice(deviceId).accumVolume( volume, { f:volAccumEvent, obj:this, params:{x:x, y:y, deviceId:deviceId} } );
}

//--------------------------------------------------------------
function volAccumEvent(params)
{
    var w = new Wave(params.x,params.y,this.waveNbPoints);
    w.setDir( Math.random()*this.waveDirAmp-this.waveDirAmp/2, Math.random()*this.waveDirAmp-this.waveDirAmp/2 );
    w.setAgeMax(this.waveAgeMax);


	if (this.mode > 0)
	{
		with(this.mapWaveManager[params.deviceId])
		{
			sndPlayer.setSoundsName( getSoundsForAnimation(params.deviceId, "waves2.js") );
    		sndPlayer.playRandom(0,1,params.deviceId);
    		waves.push(w);
		}
	}
	else
	{
		with(this.waveMono)
		{
		    sndPlayer.soundsName = getSoundsForAnimation(params.deviceId, "waves2.js");
    		sndPlayer.playRandom(0,1,params.deviceId);
	    	waves.push(w);
		}
	}
}
