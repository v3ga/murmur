this.volume = 0;
this.volumeFiltered = 0;

this.width = 0;
this.height = 0;

//--------------------------------------------------------------
function getTheme(){ return THEME_CREATURE; }

//--------------------------------------------------------------
function createParticles(w,h)
{
    if (this.listParticles == null)
    {
        this.nbParticles = 200;
        this.listParticles = Array();
        for (var i=0;i<this.nbParticles;i++)
        {
            var radius =  Math.random()*100+50;
            var angle = Math.random()*3.14*2;
            var x = w/2 + radius * Math.cos(angle);
            var y = h/2 + radius * Math.sin(angle);
            
            this.listParticles.push( new Particle2DMass(x,y,5,1,Math.random()*20+10) );
        }
    }
}

//--------------------------------------------------------------
function setup()
{
    this.listParticles = null;
//    playSound("emotion.wav", 1, 0.6);
//    setVolumeSound("emotion.wav", 1);
}

//--------------------------------------------------------------
function draw(w,h)
{
    // Store width / height
    this.width = w;
    this.height = w;
    
    // Create particles (if the list is null)
    // We do this here because we need (w,h) which is not available in setup() <-- TODO ?
    this.createParticles(w,h);
    
    // Render
	of.Background(0,0,0);
    if (this.listParticles)
    {
        of.SetColor(255);
        for (var i=0;i<this.nbParticles;i++)
            this.listParticles[i].run();
    }

 }

//--------------------------------------------------------------
function update(dt)
{
    this.volumeFiltered += (this.volume-this.volumeFiltered)*0.2;
    //setVolumeSound("emotion.wav", of.Map( this.volumeFiltered, 0,1, 0.1,1));
    setVolumeSoundMainNormalized( this.volumeFiltered );
}

//--------------------------------------------------------------
function onNewPacket(deviceId,volume,x,y)
{
    // Store volume in member class
    this.volume = volume;
    
    // Update center of particles every time
    // + diameter of repulsion = function (volumeFiltered)
    if (this.listParticles)
    {
        for (var i=0;i<this.nbParticles;i++){
            this.listParticles[i].setPointRef(x,y);
            this.listParticles[i].setDiamRepulsion( this.volumeFiltered * 0.45*Math.min(this.width, this.height)  );
        }
    }
}
