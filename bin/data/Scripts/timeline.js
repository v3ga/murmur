// --------------------------------------------------
// Utils
var group01 = [
"deviceEchoSimulator01",
"deviceEchoSimulator02",
"deviceEchoSimulator03",
"deviceEchoSimulator04",
"deviceEchoSimulator05"
];

// --------------------------------------------------
function enableDeviceBPM_group(group, is)
{
    for (var i=0; i<group.length; i++)
         enableDeviceBPM(group[i], is);       
}

// --------------------------------------------------
function setDeviceBPM_group(group, bpm)
{
    for (var i=0; i<group.length; i++)
         setDeviceBPM(group[i], bpm);       
}

// --------------------------------------------------
function setDeviceMute_group(group, is)
{
    for (var i=0; i<group.length; i++)
	{
		println("setDeviceMute("+group[i]+","+is+")");
    	setDeviceMute(group[i], is);
	}
}



// --------------------------------------------------
function timeline_hello()
{
    println("hello!");

}

// --------------------------------------------------
function timeline_test()
{
    println("hello test!");

}

// --------------------------------------------------
function timeline_waves()
{
    setAnimation("waves", "test2.xml");
}


// --------------------------------------------------
function timeline_videolines()
{
    setAnimation("composition", "videolines.xml"); // attention ici le nom de la configuration et pas de la composition :-)
	setDeviceMute("deviceEchoSimulator01", true);
	setDeviceMute_group(group01, true);
}

// --------------------------------------------------
function timeline_videomurmur()
{
    setAnimation("video", "test2.xml"); // attention ici le nom de la configuration et pas de la composition :-)
}

// --------------------------------------------------
function timeline_clouds()
{
    setAnimation("video", "clouds.xml"); // attention ici le nom de la configuration et pas de la composition :-)
}

// --------------------------------------------------
function timeline_videoagents()
{
    setAnimation("composition", "videoagents.xml"); // attention ici le nom de la configuration et pas de la composition :-)
	setDeviceMute_group(group01, false);
}

// --------------------------------------------------
function timeline_radarwaves()
{
    setAnimation("composition", "test4.xml"); // attention ici le nom de la configuration et pas de la composition :-)
}

// --------------------------------------------------
function timeline_grid01()
{
    setAnimation("gridsFF", "test.xml");
    setDeviceReversePacketsDir("chambreEcho_002", true);
    enableDeviceBPM("chambreEcho_002", true);
    setDeviceBPM("chambreEcho_002", 127);
}

// --------------------------------------------------
function timeline_grid02()
{
    setAnimation("gridsFF", "test2.xml");

    enableDeviceBPM("chambreEcho_002", false);
}

// --------------------------------------------------
function timeline_grid03()
{
    setAnimation("gridsFF", "test3.xml");
}