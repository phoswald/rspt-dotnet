// **************************************************************************
//
// File: Include.h
//
// Intrasoft Code Base
//
// Include File for Package Core
//
// Configuration
// - ICB_EARLYINIT : If IncludeEarly.cpp is to be used
// - ICB_CORE_USE_CORESTRING
// - ICB_CORE_USE_DEBUG
// - ICB_CORE_USE_TRACE
// - ICB_CORE_USE_ASSERT
// - ICB_CORE_USE_HEAPDOCTOR
//
// **************************************************************************

#include "IcbCore.h"
#ifdef ICB_CORE_USE_CORESTRING
#  include "IcbCoreString.h"
#endif
#include "IcbCoreFuncs.h"
#ifdef ICB_CORE_USE_DEBUG
#  include "Debug/Include.h"
#endif
#ifdef ICB_CORE_USE_TRACE
#  include "Trace/Include.h"
#endif
#ifdef ICB_CORE_USE_ASSERT
#  include "Assert/Include.h"
#endif
#ifdef ICB_CORE_USE_HEAPDOCTOR
#  include "HeapDoctor/IcbHeapDoctor.h"
#endif 
