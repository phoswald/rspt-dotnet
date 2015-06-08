// **************************************************************************
//
/// @file: IcbCore.h
/// Macro definitions and configuration
//
// Intrasoft Code Base - Package Core
//
// Contents:
// - Macro definitions for platform
// - Macro definitions for compile-time messages
// - Macro definitions for string and character types (ANSI and Unicode) 
// - Macro definitions for integer types (64-bit)
// - Some other macro definitions
//
// Configuration
// - ICB_SINGLETHREADED : does not assume multiple threads
// - ICB_NO_COMPMSG     : dont enable the ICB_COMPMSG_XXX macros
// - ICB_INSTANCECOUNT  : enable instance counting for some classes 
//
// **************************************************************************

// **************************************************************************
// *** Macro definitions for platform ***************************************
// **************************************************************************
//
// The following macros are defined here (under some conditions):
// ICB_DEBUG		  : defined if debug build
// ICB_UNICODE        : defined if unicode build
// ICB_WIN32          : defined if building for Windows 9X/NT/2000/XP/Vista/7
// ICB_WINCE          : defined if building for Pocket PC
// ICB_WINCE_ARM      : defined if building for Pocket PC and ARM CPU
// ICB_WINCE_X86      : defined if building for Pocket PC and X86 CPU
// ICB_MFC			  : defined if building with MFC
// ICB_MFC_60		  : defined if building with MFC, MS Visual C++ 6.0
// ICB_MFC_71         : defined if building with MFC, MS Visual C++ 2003
// ICB_MFC_80         : defined if building with MFC, MS Visual C++ 2005
// ICB_MFC_90         : defined if building with MFC, MS Visual C++ 2008
// ICB_64BIT          : defined if 64-bit build 

#ifdef _DEBUG
#	define ICB_DEBUG
#endif

#ifdef _UNICODE
#	define ICB_UNICODE
#endif

#if (defined WIN32) && (!defined _WIN32_WCE)
#	define ICB_WIN32
#endif
#if (defined WIN32) && (defined _WIN32_WCE)
#	define ICB_WINCE
#endif
#if (defined WIN32) && (defined _WIN32_WCE) && (defined _ARM_)
#	define ICB_WINCE_ARM
#endif
#if (defined WIN32) && (defined _WIN32_WCE) && (defined _X86_)
#	define ICB_WINCE_X86
#endif

#ifdef _MFC_VER
#	define ICB_MFC
#endif 
#if (_MFC_VER == 0x0600) // Visual Studio 6
#	define ICB_MFC_60
#endif
#if (_MFC_VER == 0x0700) // Visual Studio 2002
#	define ICB_MFC_70
#endif
#if (_MFC_VER == 0x0710) // Visual Studio 2003
#	define ICB_MFC_71
#endif
#if (_MFC_VER == 0x0800) // Visual Studio 2005
#	define ICB_MFC_80
#endif
#if (_MFC_VER == 0x0900) // Visual Studio 2008
#	define ICB_MFC_90
#endif
#ifdef _WIN64
#   define ICB_64BIT
#endif

#ifndef ICB_MFC
#	define ASSERT(X)
#	ifdef ICB_DEBUG
#		define DEBUG_NEW new
#	endif
#endif

// **************************************************************************
// *** Macro definitions for compile-time messages **************************
// **************************************************************************

// Example:
//  #pragma ICB_COMPMSG_INFO("Die Kompilierung läuft...")

#ifndef ICB_NO_COMPMSG
#	define ICB_DEFINE_STR(x)		#x
#	define ICB_DEFINE_INT(x)		ICB_DEFINE_STR(x)
#	ifdef ICB_DEBUG
#		define ICB_COMPMSG_RELEASE(desc)
#		define ICB_COMPMSG_DEBUG(desc)		message(__FILE__ "(" ICB_DEFINE_INT(__LINE__) "): ICB Debug: \'" desc "\'")
#	else
#		define ICB_COMPMSG_RELEASE(desc)	message(__FILE__ "(" ICB_DEFINE_INT(__LINE__) "): ICB Release: \'" desc "\'")
#		define ICB_COMPMSG_DEBUG(desc)
#	endif
#	define ICB_COMPMSG_INFO(desc)			message(__FILE__ "(" ICB_DEFINE_INT(__LINE__) "): ICB Info: \'" desc "\'")
#	define ICB_COMPMSG_WARNING(desc)		message(__FILE__ "(" ICB_DEFINE_INT(__LINE__) "): ICB Warning: \'" desc "\'")
#	define ICB_COMPMSG_ERROR(desc)			message(__FILE__ "(" ICB_DEFINE_INT(__LINE__) "): ICB Error: \'" desc "\'")
#else
#	define ICB_COMPMSG_RELEASE(desc)
#	define ICB_COMPMSG_DEBUG(desc)
#	define ICB_COMPMSG_INFO(desc)
#	define ICB_COMPMSG_WARNING(desc)
#	define ICB_COMPMSG_ERROR(desc)
#endif

// **************************************************************************
// *** Macro definitions for string and character types *********************
// **************************************************************************
//
// The following character/string types are available:
// BYTE     : unsigned char
// ACHAR    : 8-bit signed
// WCHAR    : 16-bit unsigned
// TCHAR    : ACHAR or WCHAR
// CString  : String of TCHAR, either from MFC or TIcbCoreString<TCHAR>
// CStringT : String of TCHAR, either from MFC or TIcbCoreString<TCHAR>
// CStringW : String of WCHAR, either from MFC or TIcbCoreString<WCHAR> 
// CStringA : String of ACHAR, either from MFC or TIcbCoreString<ACHAR>

typedef unsigned char  BYTE;
typedef char           ACHAR;
typedef wchar_t		   WCHAR;

#ifndef ICB_MFC
#  define CStringA		TIcbCoreString<ACHAR>
#  define CStringW		TIcbCoreString<WCHAR>
#  define CStringT		TIcbCoreString<TCHAR>
#endif
#if (defined ICB_MFC_60) && (!defined ICB_UNICODE)
#  define CStringA		CString
#  define CStringW		TIcbCoreString<WCHAR>
#  define CStringT		CString
#endif
#if (defined ICB_MFC_60) && (defined ICB_UNICODE)
#  define CStringA		TIcbCoreString<ACHAR>
#  define CStringW		CString
#  define CStringT		CString
#endif
#ifdef ICB_MFC_70
#  define CStringT		CString
#endif
#ifdef ICB_MFC_71
#  define CStringT		CString
#endif
#ifdef ICB_MFC_80
#  define CStringT		CString
#endif
#ifdef ICB_MFC_90
#  define CStringT		CString
#endif

// **************************************************************************
// *** Macro definitions for integer types **********************************
// **************************************************************************

typedef signed __int64    sint64;
typedef unsigned __int64  uint64;

// **************************************************************************
// **************************************************************************
// **************************************************************************

#ifdef ICB_INSTANCECOUNT
#	pragma ICB_COMPMSG_INFO("ICB_INSTANCECOUNT is defined. Instance counting for some classes is active.")
#	define ICB_IFDEF_INSTANCECOUNT(X) X
#else
#	define ICB_IFDEF_INSTANCECOUNT(X)
#endif
