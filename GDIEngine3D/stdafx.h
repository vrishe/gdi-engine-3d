// stdafx.h: header file which is used for listing of standard headers or those not 
//			 changes it's contents in a frequently manner

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES

// WinAPI Library Headers:
#include <Windows.h>
#include <WinError.h>
#include <tchar.h>

// C Library Headers:
#include <cmath>
#include <ctime>

// STL Headers:
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <map>

// Custom Library headers:
#include "Math.h"

#define __foreach(it, i, c) for(it i = c.begin(), _end = c.end(); i != _end; i++)