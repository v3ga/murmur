/*
 *  utils.h
 *  versatubes
 *
 *  Created by Julien on 03/05/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Timezone.h"


//--------------------------------------------------------------
bool ofLVOpenProgram(string pathAbsFile);

//--------------------------------------------------------------
Poco::DateTime nowLocal();

//--------------------------------------------------------------
string nowLocalFormatUnderscore();
