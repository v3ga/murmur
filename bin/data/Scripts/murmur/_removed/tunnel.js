this.wSurface = 0.0;
this.hSurface = 0.0;
this.soundPlayer = null;

//--------------------------------------------------------------
function createUI()
{
    this.newSlider("vol. trigger in",0.0,1.0,0.1);
    this.newSlider("vol. trigger out",0.0,1.0,0.1);
    this.newSlider("ageMax",1.0,10.0, 8.0);
    this.newSlider("speed",50.0,250.0, 100.0);
    this.newSlider("speedRotate",0.0,180.0, 44.0);
    this.newToggle("drawConnections",0);
    this.newSlider("drawLinesWidth",1.0,5.0, 1.0);
}

//--------------------------------------------------------------
function eventUI(name, value)
{
    if (name == "vol. trigger in")
    {
        if (this.volumeAccum)
            this.volumeAccum.valueTriggerIn = value;
    }
    else if (name == "vol. trigger out")
    {
        if (this.volumeAccum)
            this.volumeAccum.valueTriggerOut = value;
    }
	else if (name == "ageMax")
	{
		this.tunnelAgeMax = value;
		for (var deviceId in this.mapTunnels)
			this.mapTunnels[deviceId].setAgeMax(value);
	}
	else if (name == "speed")
	{
		this.tunnelSpeed = value;
		for (var deviceId in this.mapTunnels)
			this.mapTunnels[deviceId].setSpeed(value);
	}
	else if (name == "speedRotate")
	{
		this.tunnelSpeedRotate = value;
		for (var deviceId in this.mapTunnels)
			this.mapTunnels[deviceId].setSpeedRotate(value);
	}
	else if (name == "drawConnections")
	{
		for (var deviceId in this.mapTunnels)
			this.mapTunnels[deviceId].setDrawConnections(value);
	}
	else if (name == "drawLinesWidth")
	{
		this.tunnelDrawLinesWidth = value;
	}

}


//--------------------------------------------------------------
function setup()
{
	this.mapTunnels = new Array();
	this.soundPlayer = new SoundPlayer( new Array("waves1.wav","waves2.wav","waves3.wav","waves4.wav","waves5.wav","waves6.wav","waves7.wav","waves8.wav") );
}

//--------------------------------------------------------------
function update(dt)
{
	for (var deviceId in this.mapTunnels)
		this.mapTunnels[deviceId].update(dt);
}

//--------------------------------------------------------------
function draw(w,h)
{
	this.wSurface = w;
	this.hSurface = h;

	of.Background(0,0,0);
//	if (this.tunnelDrawLinesWidth)
//		of.SetLineWidth(this.tunnelDrawLinesWidth);

	for (var deviceId in this.mapTunnels){
		this.mapTunnels[deviceId].draw(w,h);
	}

	of.SetLineWidth(1.0);
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
	// Create a tunnel for the device
	if (this.mapTunnels[deviceId] == null)
		this.mapTunnels[deviceId] = new Tunnel();

	// Accumulate volume to trigger events
    this.volumeAccum = getDevice(deviceId).volumeAccum;
    getDevice(deviceId).accumVolume( volume, { f:volAccumEvent, obj:this, params:{x:x, y:y, deviceId:deviceId} } );
}

//--------------------------------------------------------------
function volAccumEvent(params)
{
	if (this.mapTunnels[params.deviceId]!=null && this.wSurface>0.0)
	{
	    var tunnelFrame = new TunnelFrame(params.x,params.y);
		
		tunnelFrame.setA(0,0);
		tunnelFrame.setB(this.wSurface,0);
		tunnelFrame.setC(this.wSurface,this.hSurface);
		tunnelFrame.setD(0,this.hSurface);

		tunnelFrame.setAgeMax(this.tunnelAgeMax);
		tunnelFrame.setSpeed(this.tunnelSpeed);
		tunnelFrame.setSpeedRotate(this.tunnelSpeedRotate);

    	//this.mapTunnels[params.deviceId].sndPlayer.playRandom(0,1);
    	this.mapTunnels[params.deviceId].addFrame(tunnelFrame);
		this.soundPlayer.playRandom(0,1);
		
		 // println("> new nb="+this.mapTunnels.length+" @x="+params.x+";y="+params.y);
	}
}

