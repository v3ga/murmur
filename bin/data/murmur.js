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
include("Scripts/libraries/verlet-js/vec2.js");
include("Scripts/libraries/verlet-js/verlet.js");
include("Scripts/libraries/verlet-js/constraint.js");
include("Scripts/libraries/verlet-js/objects.js");


// --------------------------------------------------------
include("Scripts/libraries/murmur/Particle2D.js");
include("Scripts/libraries/murmur/Particle2DMass.js");
include("Scripts/libraries/murmur/GridVerlet.js");
include("Scripts/libraries/murmur/Wave.js");
include("Scripts/libraries/murmur/Grid3D.js");

// --------------------------------------------------------
THEME_UNKNOWN			= 0;
THEME_SURFACE			= 1;
THEME_CREATURE			= 2;
THEME_ARBORESCENCE		= 3;

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


function getDevice(id)
{
    if (deviceManager[id]==null){
        deviceManager[id] = new Device(id);
        println(">>> new Device (js) : "+id);
    }
    return deviceManager[id];
}

this.deviceManager = {};


// --------------------------------------------------------
function VolumeAccum()
{
    this.value = 0;
    
    /**/
    this.valueMax = 0.3;
    this.valueTh = 0.15;
    /**/
    
    this.STATE_WAVE_INSIDE = 1;
    this.STATE_WAVE_OUT = 2;

    this.valueTriggerIn = 0.15;
    this.valueTriggerOut = 0.15;
    
    this.state = this.STATE_WAVE_OUT;
    
    this.add = function(volume, fEvent)
    {
        if (this.state == this.STATE_WAVE_INSIDE)
        {
            if (volume <= this.valueTriggerOut)
            {
                this.state=this.STATE_WAVE_OUT;
            }
        }
        else if (this.state == this.STATE_WAVE_OUT)
        {
            this.value += volume;
            if (volume >= this.valueTriggerIn )
            {
                if (fEvent && fEvent.f && fEvent.obj)
                    fEvent.f.call(fEvent.obj, fEvent.params);
                this.state = this.STATE_WAVE_INSIDE;
                this.value=0;
            }
        }
            
//        if (volume > this.valueTh)
        
/*      if (volume > this.valueTh)
        {
            this.value += volume;
        }
        else
        {
            if (this.value >= this.valueMax)
            {
                if (fEvent && fEvent.f && fEvent.obj)
                    fEvent.f.call(fEvent.obj, fEvent.params);
            }
            this.value = 0;
        }
*/
    }
}

// --------------------------------------------------------
function SoundPlayer(soundsName)
{
    this.soundsName = soundsName;
    this.lastPlayedIndex = -1;

    this.playRandom = function(a,b)
    {
        if (soundsName.length>=2)
        {
		println( soundsName[0] );
            playSound( soundsName[0] );
/*
            var rndIndex = -1;
            do{
                rndIndex = Math.floor(Math.random() * soundsName.length);
            } while (rndIndex==this.lastPlayedIndex);
            this.lastPlayedIndex = rndIndex;
        
            playSound( soundsName[rndIndex],a,b );
        }
        else
*/
		}
    }
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



