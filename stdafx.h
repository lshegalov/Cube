// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>


// TODO: reference additional headers your program requires here
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <cstdint>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <string>

#include <iostream>
#include <iomanip>

#include <locale> 


#include <ctype.h>
#ifdef _MSC_BUILD
#include <direct.h>
#include <strstream> 
#endif

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <memory.h> 
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#ifdef __GNUC__
#include <sys/stat.h>
#include <limits.h>
#include <strings.h>
#include <unistd.h>

#define _mkdir mkdir
#define sprintf_s sprintf
#define strcat_s strcat
#define _stricmp strcasecmp
#define strcpy_s strcpy
#define vsprintf_s vsprintf
#endif

#define ERROR_MSG_LEN 2000

#define TEST_SIDE 21

struct Results
{
    int nNumberOfSteps = 0;

    long lNumberOfPolygons = 0;

    int nTotalNumberOfSides = 0;

    int nTotalNumberOfEdges = 0;

    double dAverageNumberOfEdgesPerPolygons = 0;

    std::map<int, size_t> distributionPerEdges;

    double dAverageNumberOfSidesPerPolygons = 0;

    std::map<int, size_t> distributionPerSides;

    // per # of steps
    std::map<int, double> dMeanNumberOfSidesPerPolygons;

    // per # of steps
    std::map<int, double> dMeanNumberOfEdgesPerPolygons;
};