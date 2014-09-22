// --------------------------------------------------------
function SoundPlayer(soundsName)
{
    this.soundsName = soundsName;
    this.lastPlayedIndex = -1;

    this.playRandom = function(isLoop,volume,deviceId)
    {
        if (soundsName.length>=2)
        {
            //playSound( soundsName[0],isLoop,volume );

            var rndIndex = -1;
            do{
                rndIndex = Math.floor(Math.random() * soundsName.length);
            } while (rndIndex==this.lastPlayedIndex);
            this.lastPlayedIndex = rndIndex;
        
            playSound( soundsName[rndIndex],isLoop,volume,deviceId );
        }
    }
}