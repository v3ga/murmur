// --------------------------------------------------
// Utils
var group01 = [
"chambreEcho_001",
"chambreEcho_002",
"chambreEcho_003",
"chambreEcho_004",
"chambreEcho_005"
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
function timeline_videolines()
{
    setAnimation("composition", "videolines.xml"); // attention ici le nom de la configuration et pas de la composition :-)
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