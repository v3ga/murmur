/*
 *  utils.cpp
 *  versatubes
 *
 *  Created by Julien on 03/05/12.
 *  Copyright 2012 2Roqs. All rights reserved.
 *
 */

#include "utils.h"

//--------------------------------------------------------------
bool ofLVOpenProgram(string pathAbsFile)
{
        #ifdef TARGET_OSX
                CFStringRef      cfPathAbsFile = CFStringCreateWithCString(NULL,pathAbsFile.c_str(),CFStringGetSystemEncoding());
                if (cfPathAbsFile != NULL)
                {
                        CFURLRef cfUrl = CFURLCreateWithFileSystemPath (kCFAllocatorDefault,cfPathAbsFile,kCFURLPOSIXPathStyle,false); 
                        if (cfUrl != NULL)
                        {
                                LSOpenCFURLRef(cfUrl, NULL);
                                return true;
                        }
                }

                return false;
        #endif
        
        #ifdef TARGET_WIN32
                // http://msdn.microsoft.com/en-us/library/bb762153%28VS.85%29.aspx
                HWND hwnd = WindowFromDC(wglGetCurrentDC());
                return ((int)ShellExecute(hwnd, "open", pathAbsFile.c_str(),0,0,SW_SHOWNORMAL))>32;
        #endif

        return false;
}

//--------------------------------------------------------------
Poco::DateTime nowLocal()
{
	Poco::DateTime nowLocal;
	nowLocal.makeLocal(Poco::Timezone::tzd());
	return nowLocal;
}

//--------------------------------------------------------------
string nowLocalFormatUnderscore()
{
	Poco::DateTime now = nowLocal();
	return Poco::DateTimeFormatter::format(now, "%H_%M_%S");
}
