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

    this.setValueTriggerIn = function(value)
    {
        this.valueTriggerIn = value;
    }

}
