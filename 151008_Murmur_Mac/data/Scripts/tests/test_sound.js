//--------------------------------------------------------------
function setup()
{
    this.sndPlayer = new SoundPlayer( new Array("waves1.wav","waves2.wav","waves3.wav","waves4.wav","waves5.wav","waves6.wav","waves7.wav","waves8.wav") );
    this.t = 0;
}

//--------------------------------------------------------------
function createUI()
{
}

//--------------------------------------------------------------
function eventUI(name, value)
{
}

//--------------------------------------------------------------
function draw(w,h)
{
	of.Background(0,0,0);
}

//--------------------------------------------------------------
function update(dt)
{
	this.t += dt;
	if (this.t>1.5){
		this.sndPlayer.playRandom(false,1);
		this.t = 0;
		println("TOP");
	}
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
}
