// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#define WINVER 0x0600         // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINNT 0x0600   // Change this to the appropriate value to target other versions of Windows.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#define _WIN32_IE      0x0700 // Change this to the appropriate value to target other versions of IE.
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers

//#include <stdio.h>
//#include <tchar.h>
#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#define ICB_PACKAGE_CORE
#define ICB_PACKAGE_DATATYPES
#define ICB_DATATYPES_USE_ARRAY
#define ICB_DATATYPES_USE_HASHTABLE
#include "..\BaseCPP\Include.h"
