// **************************************************************************
//
// File: Include.cpp
//
// Intrasoft Code Base
//
// Include File for Package Core
//
// **************************************************************************

#ifdef ICB_CORE_USE_CORESTRING
#  include "IcbCoreString.cpp"
#endif
#ifdef ICB_CORE_USE_DEBUG
#  include "Debug/Include.cpp"
#endif
#if (defined ICB_CORE_USE_TRACE) && (!defined ICB_EARLYINIT)
#  include "Trace/Include.cpp"
#endif
#ifdef ICB_CORE_USE_ASSERT
#  include "Assert/Include.cpp"
#endif
