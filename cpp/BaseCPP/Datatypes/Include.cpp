// **************************************************************************
//
// File: Include.cpp
//
// Intrasoft Code Base
//
// Include File for Package Datatypes
//
// **************************************************************************

#ifdef ICB_DATATYPES_USE_STRING
#  include "IcbString.cpp"
#endif
#ifdef ICB_DATATYPES_USE_OBJECT
#  include "IcbObject.cpp"
#  include "IcbObjRefCnt.cpp"
#endif
#ifdef ICB_DATATYPES_USE_RESPROVIDER
#  include "IcbResProvider.cpp"
#endif
#ifdef ICB_DATATYPES_USE_EXCEPTION
#  include "IcbException.cpp"
#endif
#ifdef ICB_DATATYPES_USE_DATETIME
#  include "IcbDateTime.cpp"
#  include "IcbTimeZone.cpp"
#endif
#ifdef ICB_DATATYPES_USE_TIMESIMULATOR
#  include "IcbTimeSimulator.cpp"
#endif
#ifdef ICB_DATATYPES_USE_BINARY
#  include "IcbBinary.cpp"
#endif
#ifdef ICB_DATATYPES_USE_IP
#  include "IcbIP.cpp"
#endif
#ifdef ICB_DATATYPES_USE_VERSION
#  include "IcbVersion.cpp"
#endif
