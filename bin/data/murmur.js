// --------------------------------------------------------
println("-- murmur.js -- ");

// --------------------------------------------------------
include("Scripts/libraries/fieldkit/src/Header.js");
include("Scripts/libraries/fieldkit/src/Class.js");
include("Scripts/libraries/fieldkit/src/math/Math.js");
include("Scripts/libraries/fieldkit/src/math/Vec2.js");
include("Scripts/libraries/fieldkit/src/math/Vec3.js");

// --------------------------------------------------------
include("Scripts/libraries/murmur/Particle2D.js");
include("Scripts/libraries/murmur/Particle2DMass.js");
include("Scripts/libraries/murmur/Wave.js");
include("Scripts/libraries/murmur/VolumeAccum.js");

// --------------------------------------------------------
this.deviceManager = {};

// --------------------------------------------------------
function Device(id)
{
    this.id = id;
    this.accroche = {x:0,y:0};
    this.volume = 0;
    this.volumeAccum = new VolumeAccum();

    this.setAccroche = function(x,y)
    {
        this.accroche.x = x;
        this.accroche.y = y;
    }

    this.setColorHueSaturation = function(hue, saturation)
    {
        setDeviceColorHueSaturation(this.id, hue, saturation);    
    }

    this.setDeviceColor = function()
    {
        setDeviceColor(this.id);
    }

    this.setVolume = function(volume)
    {
        this.volume = volume;        
    }

    this.accumVolume = function(volume, fEvent)
    {
        this.volumeAccum.add(volume, fEvent);
    }

}

// --------------------------------------------------------
function getDevice(id)
{
    if (deviceManager[id]==null){
        deviceManager[id] = new Device(id);
        println(">>> new Device (js) : "+id);
        if (typeof deviceManager[id].onCreate == 'function'){
            println("   - calling 'onCreate'");
            deviceManager[id].onCreate();
        }
    }
    return deviceManager[id];
}


// --------------------------------------------------------
function shuffleArray(a)
{
	a.sort( function(){return Math.random()-0.5} );
}

// --------------------------------------------------------
 function size(arr) 
 {
    var size = 0;
    for (var key in arr) 
    {
        if (arr.hasOwnProperty(key)) size++;
    }
    return size;
 }; 



