// --------------------------------------------------------
println("-- murmur.js -- ");

// --------------------------------------------------------
include("Scripts/libraries/tweening/Tween.js");

// --------------------------------------------------------
include("Scripts/libraries/tweenjs/Tween.js");

// --------------------------------------------------------
include("Scripts/libraries/fieldkit/src/Header.js");
include("Scripts/libraries/fieldkit/src/Class.js");
include("Scripts/libraries/fieldkit/src/math/Math.js");
include("Scripts/libraries/fieldkit/src/math/Vec2.js");
include("Scripts/libraries/fieldkit/src/math/Vec3.js");

// --------------------------------------------------------
/*
include("Scripts/libraries/verlet-js/vec2.js");
include("Scripts/libraries/verlet-js/verlet.js");
include("Scripts/libraries/verlet-js/constraint.js");
include("Scripts/libraries/verlet-js/objects.js");
*/

// --------------------------------------------------------
include("Scripts/libraries/murmur/Particle2D.js");
include("Scripts/libraries/murmur/Particle2DMass.js");
//include("Scripts/libraries/murmur/GridVerlet.js");
include("Scripts/libraries/murmur/Wave.js");
//include("Scripts/libraries/murmur/Grid3D.js");
include("Scripts/libraries/murmur/SoundPlayer.js");
include("Scripts/libraries/murmur/VolumeAccum.js");
include("Scripts/libraries/murmur/Tunnel.js");

// --------------------------------------------------------
this.deviceManager = {};

// --------------------------------------------------------
function Device(id)
{
    this.id = id;
    this.volumeAccum = new VolumeAccum();
    
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



