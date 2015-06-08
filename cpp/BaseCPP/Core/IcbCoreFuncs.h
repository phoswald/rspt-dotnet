// **************************************************************************
//
/// @file: IcbCoreFuncs.h
/// Core functions and templates
//
// Intrasoft Code Base - Package Core
//
// Contents:
// - Functions for atomic operations
// - Function templates for in-place construction, destruction and copying
// - Function templates for comparing and hashing
// - Function templates for default-initialisation
// - Function templates for abs, min, max, swap, ...
//
// **************************************************************************

// **************************************************************************
// *** IcbAtomicXXX-Functions ***********************************************
// **************************************************************************

inline int IcbGetThisThreadID() { return (int) ::GetCurrentThreadId(); }

#ifdef ICB_SINGLETHREADED 
	__forceinline int  IcbAtomicInc(int volatile* i) { return ++(*i); }
	__forceinline int  IcbAtomicDec(int volatile* i) { return --(*i); }
	__forceinline bool IcbAtomicTestAndSet(int volatile* i, int t, int s) { if(*i != t) return false; *i = s; return true; }
#else // ICB_SINGLETHREADED
#ifdef ICB_MFC_60
#ifdef ICB_WIN32
	__forceinline int  IcbAtomicInc(int volatile* i) { return ::InterlockedIncrement((LONG*) i); }
	__forceinline int  IcbAtomicDec(int volatile* i) { return ::InterlockedDecrement((LONG*) i); }
	__forceinline bool IcbAtomicTestAndSet(int volatile* i, int t, int s) { return ::InterlockedCompareExchange((VOID**) i, (VOID*) s, (VOID*) t) == (VOID*) t; }
#else
	__forceinline int  IcbAtomicInc(int volatile* i) { return ::InterlockedIncrement((LONG*) i); }
	__forceinline int  IcbAtomicDec(int volatile* i) { return ::InterlockedDecrement((LONG*) i); }
	__forceinline bool IcbAtomicTestAndSet(int volatile* i, int t, int s) { return ::InterlockedCompareExchange((LONG*) i, s, t) == t; }
#endif
#endif // ICB_MFC_60
#ifdef ICB_MFC_71
	__forceinline int  IcbAtomicInc(int volatile* i) { return ::InterlockedIncrement((long volatile*) i); }
	__forceinline int  IcbAtomicDec(int volatile* i) { return ::InterlockedDecrement((long volatile*) i); }
	__forceinline bool IcbAtomicTestAndSet(int volatile* i, int t, int s) { return ::InterlockedCompareExchange((long volatile*) i, s, t) == t; }
#endif // ICB_MFC_71
#ifdef ICB_MFC_80
	__forceinline int  IcbAtomicInc(int volatile* i) { return ::InterlockedIncrement((long volatile*) i); }
	__forceinline int  IcbAtomicDec(int volatile* i) { return ::InterlockedDecrement((long volatile*) i); }
	__forceinline bool IcbAtomicTestAndSet(int volatile* i, int t, int s) { return ::InterlockedCompareExchange((long volatile*) i, s, t) == t; }
#endif // ICB_MFC_80
#ifdef ICB_MFC_90
	__forceinline int  IcbAtomicInc(int volatile* i) { return ::InterlockedIncrement((long volatile*) i); }
	__forceinline int  IcbAtomicDec(int volatile* i) { return ::InterlockedDecrement((long volatile*) i); }
	__forceinline bool IcbAtomicTestAndSet(int volatile* i, int t, int s) { return ::InterlockedCompareExchange((long volatile*) i, s, t) == t; }
#endif // ICB_MFC_90
#endif // ICB_SINGLETHREADED

// **************************************************************************
// *** IcbConstruct, IcbDestruct, IcbCopy, ... ******************************
// **************************************************************************

/// Calls the default constructor for nNum objects.
/// pBuf is uninitialized on entry, initialized on return.
template <class T> inline void IcbConstruct(T* pBuf, int nNum)
{
	while(nNum--) {
		::new((void*) pBuf++) T;
	}
}

/// Calls the copy-constructor for nNum objects.
/// pBuf is uninitialized on entry, initialized on return.
/// pSrc must be initialized.
template <class T> inline void IcbConstructCopy(T* pBuf, int nNum, const T* pSrc)
{
	while(nNum--) {
		::new((void*) pBuf++) T(*(pSrc++));
	}
}

/// Calls the destructor for nNum objects.
/// pBuf is initialized on entry, uninitialized on return.
template <class T> inline void IcbDestruct(T* pBuf, int nNum)
{
	while(nNum--) {
		(pBuf++)->~T();
	}
}

/// Moves nNum objects from pSrc to pDst.
/// pDst is uninitialized on entry, initialized on return.
/// pSrc is initialized on entry, uninitialized on return.
/// The memory areas may be overlapping
template <class T> inline void IcbRelocate(T* pDst, T* pSrc, int nNum)
{
	memmove(pDst, pSrc, nNum * sizeof(T));
}

/*
/// Copies nNum objects from pSrc to pDst.
/// pSrc must be initialized.
/// pDst must be initialized.
template <class T> inline void IcbCopy(T* pDst, const T* pSrc, int nNum)
{
	while(nNum--) {
		*(pDst++) = *(pSrc++);
	}
}

template <class B, class A> inline B& IcbReinterpret(A& o) 
{ 
	return * (B*) (&o); 
}
*/

template <class T> inline T* IcbSafeCast(T* t) 
{ 
	return t; 
}

// **************************************************************************
// *** IcbCompare, IcbHashCode, ... *****************************************
// **************************************************************************

/// Compares two objects.
/// @param oA the 1st object to compare.
/// @param oB the 2nd object to compare.
/// @return negative if oA is less than, positive if oA is greater than or zero if oA is equal to oB. 
template <class T> inline int IcbCompare(const T& oA, const T& oB)
{
	return (oA == oB) ? 0 : ((oA < oB) ? -1 : 1);
}

/// Compares two objects in a case insensitive manner.
/// @param oA the 1st object to compare.
/// @param oB the 2nd object to compare.
/// @return negative if oA is less than, positive if oA is greater than or zero if oA is equal to oB. 
template <class T> inline int IcbCompareNoCase(const T& oA, const T& oB)
{
	ASSERT(0);
	return 0;
}

/// Compares two strings (ANSI version).
/// @param oA the 1st strings to compare.
/// @param oB the 2nd strings to compare.
/// @return negative if oA is less than, positive if oA is greater than or zero if oA is equal to oB. 
template <> inline int IcbCompare<CStringA>(const CStringA& oA, const CStringA& oB)
{
	return strcmp(oA, oB);
}

/// Compares two strings (Unicode version).
/// @param oA the 1st strings to compare.
/// @param oB the 2nd strings to compare.
/// @return negative if oA is less than, positive if oA is greater than or zero if oA is equal to oB. 
template <> inline int IcbCompare<CStringW>(const CStringW& oA, const CStringW& oB)
{
	return wcscmp(oA, oB);
}

/// Compares two strings in a case insensitive manner (ANSI version).
/// @param oA the 1st strings to compare.
/// @param oB the 2nd strings to compare.
/// @return negative if oA is less than, positive if oA is greater than or zero if oA is equal to oB. 
template <> inline int IcbCompareNoCase<CStringA>(const CStringA& oA, const CStringA& oB)
{
	return _stricmp(oA, oB);
}

/// Compares two strings in a case insensitive manner (Unicode version).
/// @param oA the 1st strings to compare.
/// @param oB the 2nd strings to compare.
/// @return negative if oA is less than, positive if oA is greater than or zero if oA is equal to oB. 
template <> inline int IcbCompareNoCase<CStringW>(const CStringW& oA, const CStringW& oB)
{
	return _wcsicmp(oA, oB);
}

/// Computes the hash code of the given object.
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the object whose hash code is to be computed.
/// @return the computed hash code.
template <class T> inline unsigned IcbHashCode(const T& oA);

/// Computes the hash code of the given bool.
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the bool whose hash code is to be computed.
/// @return the computed hash code.
template <> inline unsigned IcbHashCode<bool>(const bool& oA)
{
	return (unsigned) oA ? 1 : 0;
}

/// Computes the hash code of the given 32-bit integer.
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the integer whose hash code is to be computed.
/// @return the computed hash code.
template <> inline unsigned IcbHashCode<int>(const int& oA)
{
	return (unsigned) oA;
}

/// Computes the hash code of the given 64-bit integer.
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the integer whose hash code is to be computed.
/// @return the computed hash code.
template <> inline unsigned IcbHashCode<__int64>(const __int64& oA)
{
	return (unsigned)  oA ^
		   (unsigned) (oA >> 32);
}

/// Computes the hash code of the given raw pointer.
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the string whose hash code is to be computed.
/// @return the computed hash code.
template <> inline unsigned IcbHashCode<const void*>(const void* const& oA)
{
	return IcbHashCode<INT_PTR>((INT_PTR) oA); 
}

/// Computes the hash code of the given string (ANSI version).
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the string whose hash code is to be computed.
/// @return the computed hash code.
template <> inline unsigned IcbHashCode<const ACHAR*>(const ACHAR* const& oA)
{
	const ACHAR* p = oA;
	unsigned nHash = 0;
	while (*p)
		nHash = (nHash<<5) + nHash + *p++;
	return nHash;
}

/// Computes the hash code of the given string (Unicode version).
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the string whose hash code is to be computed.
/// @return the computed hash code.
template <> inline unsigned IcbHashCode<const WCHAR*>(const WCHAR* const& oA)
{
	const WCHAR* p = oA;
	unsigned nHash = 0;
	while (*p)
		nHash = (nHash<<5) + nHash + *p++;
	return nHash;
}

/// Computes the hash code of the given string (ANSI version).
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the string whose hash code is to be computed.
/// @return the computed hash code.
template <> inline unsigned IcbHashCode<CStringA>(const CStringA& oA)
{
	return IcbHashCode<const ACHAR*>(oA);
}

/// Computes the hash code of the given string (Unicode version).
/// The hash code is supposed to be an evenly distributed 32-bit value
/// @param oA the string whose hash code is to be computed.
/// @return the computed hash code.
template <> inline unsigned IcbHashCode<CStringW>(const CStringW& oA)
{
	return IcbHashCode<const WCHAR*>(oA);
}

// **************************************************************************
// *** IcbDefault ***********************************************************
// **************************************************************************

/// Returns the default value for the type T.
/// @return the default value, which is false/zero/NULL/"" for bools/numbers/pointers/strings.  
template <class T> inline T IcbDefault() 
{ 
	return T(); 
}

template <> inline bool IcbDefault<bool>() 
{ 
	return false; 
}

template <> inline int IcbDefault<int>() 
{ 
	return 0; 
}

template <> inline double IcbDefault<double>() 
{ 
	return 0.0; 
}

// **************************************************************************
// *** IcbSwap, IcbMin, IcbMax, ... *****************************************
// **************************************************************************

template <class T> inline void IcbSwap(T& oA, T& oB)
{
	T oTemp = oA;
	oA = oB;
	oB = oTemp;
}

template<class T> inline T IcbAbs(T n) 
{
	if (n<0) {
		return -n; 
	} else {
		return n;
	}
}

template<class T> inline T IcbMax(T n, T m) 
{
	if (n<m) {
		return m; 
	} else {
		return n;
	}
}

template<class T> inline T IcbMin(T n, T m) 
{
	if (n>m) {
		return m; 
	} else {
		return n;
	}
}

template<class T> inline int IcbSgn(T n) 
{
	if (n==0) {
		return 0; 
	} else if (n>0) {
		return 1; 
	} else {
		return -1;
	}
}

//template<class T> inline T IcbMed(T n, T m, T o) 
//{
//	if (n < m) {
//		if (o < n) {
//			return n;
//		} else if (o > m) {
//			return m;
//		} else {
//			return o;
//		}
//	} else {
//		if (o > n) {
//			return n;
//		} else if (o < m) {
//			return m;
//		} else {
//			return o;
//		}
//	}
//}

template<class T> inline int IcbRound(T n) 
{
	return (int) (IcbAbs(n)+0.5) * IcbSgn(n);
}

inline double IcbRound(double n, double granularity) 
{
	return (int) (IcbAbs(n)/granularity+0.5) * granularity * IcbSgn(n);
}

// **************************************************************************
// *** IcbOr, IcbAnd, IcbCond ***********************************************
// **************************************************************************

inline bool IcbOr(bool x1, bool x2) {
	return x1 || x2;
}

inline bool IcbOr(bool x1, bool x2, bool x3) {
	return IcbOr(x1, x2) || x3;
}

inline bool IcbOr(bool x1, bool x2, bool x3, bool x4) {
	return IcbOr(x1, x2, x3) || x4;
}

inline bool IcbOr(bool x1, bool x2, bool x3, bool x4, bool x5) {
	return IcbOr(x1, x2, x3, x4) || x5;
}

inline bool IcbOr(bool x1, bool x2, bool x3, bool x4, bool x5, bool x6) {
	return IcbOr(x1, x2, x3, x4, x5) || x6;
}

inline bool IcbOr(bool x1, bool x2, bool x3, bool x4, bool x5, bool x6, bool x7) {
	return IcbOr(x1, x2, x3, x4, x5, x6) || x7;
}

inline bool IcbAnd(bool x1, bool x2) {
	return x1 && x2;
}

inline bool IcbAnd(bool x1, bool x2, bool x3) {
	return IcbAnd(x1, x2) && x3;
}

inline bool IcbAnd(bool x1, bool x2, bool x3, bool x4) {
	return IcbAnd(x1, x2, x3) && x4;
}

inline bool IcbAnd(bool x1, bool x2, bool x3, bool x4, bool x5) {
	return IcbAnd(x1, x2, x3, x4) && x5;
}

template <class T> inline T IcbCond(bool c, T r1, T r2) {
	return c ? r1 : r2;
}

// **************************************************************************
// *** IcbTrySet, IcbTryGet *************************************************
// **************************************************************************

template <class T> inline void IcbTrySet(T* px, const T& x)
{
	if(px) *px = x;
}

template <class T> inline T IcbTryGet(T* px, const T& dflt)
{
	return px ? *px : dflt;
}

template <class T> inline void IcbTryInc(T* px, const T& x)
{
	if(px) *px += x;
}

template <class T> inline void IcbTryDec(T* px, const T& x)
{
	if(px) *px -= x;
}
