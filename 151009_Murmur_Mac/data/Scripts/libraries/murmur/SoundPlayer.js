// --------------------------------------------------------
function SoundPlayer(soundsName)
{
    this.soundsName = soundsName;
    this.lastPlayedIndex = -1;

	this.setSoundsName = function(obj)
	{
		this.soundsName = new Array();
		for (var p in obj){
			this.soundsName.push(obj[p]);
		}
	}

    this.playRandom = function(isLoop,volume,deviceId)
    {
		// println( "soundsName.length = "+this.soundsName.length );
        if (this.soundsName.length>=2)
        {
            //playSound( soundsName[0],isLoop,volume );

            var rndIndex = -1;
            do{
                rndIndex = Math.floor(Math.random() * this.soundsName.length);
            } while (rndIndex==this.lastPlayedIndex);
            this.lastPlayedIndex = rndIndex;
        
			// println( "playing random = "+this.soundsName[rndIndex] );
            playSound( this.soundsName[rndIndex],isLoop,volume,deviceId );
        }
		else
		if (soundsName.length==1)
		{
            playSound( this.soundsName[0],isLoop,volume,deviceId );
		}
    }
}