// **************************************************************************
//
/// @file: IcbArray.h
/// A template class for arrays
//
// Intrasoft Code Base - Package Datatypes
//
//
// **************************************************************************

#ifdef _DEBUG
#	undef THIS_FILE
#	define THIS_FILE __FILE__
#	define new DEBUG_NEW
#endif

// **************************************************************************
// *** TIcbArray ************************************************************
// **************************************************************************

/// A template class for arrays of any type T.
///
/// This class provides a resizable array. On insertion, the array grows 
/// exponentially. On removal, the array shrinks exponentially. In the case of
/// exponential growing, the capacity is the lowest power of 2 which is greater 
/// or equal to the number of elements to be stored - but at least 8 if the 
/// array is not empty. The array capacity can also be set explicitly.
///
/// Lookup can be done in running time O(1). Insert and delete operations at 
/// the end of the array can be done in amortized running time O(1). In the 
/// inside of the array, the running time is O(n).
/// The memory overhead is 12 bytes per array. Typically, 0-50% of the elements
/// are unitintialized.
///
/// The template functions IcbConstruct, IcbConstructCopy, IcbDestruct and 
/// IcbRelocate are used to create, delete and copy elements. Therefore, 
/// this class is suitable for complex element-types such as smart pointers
/// or MFC-like strings.

template <class T> class TIcbArray
{
	// *** Inner Classes ****************************************************
private:
	/// Comparer class providing default sorting by the IcbCompare<T> template function.
	struct TCmpDefault {
		static int Compare(const T& oA, const T& oB) { return IcbCompare(oA, oB); }
	};

	/// Comparer class providing default case insensitive sorting by the IcbCompareNoCase<T> template function.
	struct TCmpNoCase {
		static int Compare(const T& oA, const T& oB) { return IcbCompareNoCase(oA, oB); }
	};

	// *** Attributes *******************************************************
private:
	/// Pointer to the elements of the array. 
	/// The array size is m_nCapacity, but only the first m_nSize elements
	/// are initialized and can be accessed.
	T* m_pData;	 

	/// The capacity of the array, never negative.
	int	m_nCapacity; 

	/// The size of the array, between 0 and m_nCapacity.
	int m_nSize;     
	
	// *** Methods **********************************************************
private:
	/// Computes a capacity with is big enough for storing a given number of elements.
	/// @param nNeeded the number of elements to be stored.
	/// @return        the capacity, a value >= nNeeded.
	static int DoComputeCapacity(int nNeeded)
	{
		if(nNeeded == 0) {
			return 0; 
		} else {
			int nCapacity = 8;
			while(nCapacity < nNeeded) nCapacity <<= 1;
			return nCapacity;
		}
	}

	/// Internal implementation of the quick sort sorting algorithm.
	/// This method sorts a subset of an array by using a custom comparer class.
	/// @param pData array to be sorted.
	/// @param nIdx1 the index of the first element of the subset.
	/// @param nIdx2 the index of the last element of the subset.
	/// @param bInverseOrder usually false, if true the array will be sorted in descending order.
	/// @param oCmp the comparator to be used. The comparator must implement the method Compare(),
	///             which takes two references to array elements and returns -1 / 0 / +1
	///             if the 1st element is smaller / equal / larger than the 2nd element.
	template <class CMP> static void DoQuickSort(T* pData, int nIdx1, int nIdx2, bool bInverseOrder, const CMP& oCmp)
	{
		if(nIdx1 >= nIdx2) 
			return;

		int nPivot = rand() % (nIdx2-nIdx1+1) + nIdx1;
		ASSERT(nPivot >= nIdx1);
		ASSERT(nPivot <= nIdx2);
		IcbSwap(pData[nIdx1], pData[nPivot]);
		nPivot = nIdx1;
		T oPivot = pData[nIdx1];
		for(int nActual = nIdx1+1; nActual <= nIdx2; nActual++) {
			int nCmp = oCmp.Compare(oPivot, pData[nActual]);
			if(bInverseOrder) 
				nCmp = -nCmp;
			if(nCmp > 0) {
				nPivot++;
				IcbSwap(pData[nPivot], pData[nActual]);
			}
		}
		IcbSwap(pData[nIdx1], pData[nPivot]);
		DoQuickSort(pData, nIdx1,    nPivot-1, bInverseOrder, oCmp);
		DoQuickSort(pData, nPivot+1, nIdx2,    bInverseOrder, oCmp);
	}

public:
	/// Constructs an empty array with no capacity.
	TIcbArray() 
	{
		m_pData     = NULL;
		m_nCapacity = 0;
		m_nSize     = 0;
	}

	/// Constructs an empty array with the given capacity.
	/// @param nCapacity the initial capacity.
	TIcbArray(int nCapacity) 
	{
		m_pData     = NULL;
		m_nCapacity = 0;
		m_nSize     = 0;

		SetCapacity(nCapacity);
	}

	/// Constructs a new array by copying size, capacity and elements from the given array.
	/// @param oCopy the array to be copied.
	TIcbArray(const TIcbArray& oCopy) 
	{
		m_pData     = NULL;
		m_nCapacity = 0;
		m_nSize     = 0;
		if(oCopy.m_nCapacity) {
			SetCapacity(oCopy.m_nCapacity);
		}
		if(oCopy.m_nSize) {
			m_nSize = oCopy.m_nSize;
			IcbConstructCopy(m_pData, m_nSize, oCopy.m_pData);
		}
	}

	/// Destructs the instance.
	~TIcbArray()
	{
		SetCapacity(0);
	}

	/// Assignment operator: Sets size, capacity and elements of this array.
	/// @param  oCopy the array to be assinged.
	/// @return a reference to the array itself.
	TIcbArray& operator = (const TIcbArray& oCopy)
	{
		SetCapacity(0);
		if(oCopy.m_nCapacity) {
			SetCapacity(oCopy.m_nCapacity);
		}
		if(oCopy.m_nSize) {
			m_nSize = oCopy.m_nSize;
			IcbConstructCopy(m_pData, m_nSize, oCopy.m_pData);
		}
		return *this;
	}

	/// Array operator: Returns a const reference to the element at the given index.
	/// @param nIdx the index of the element to be returned, must be between 0 and GetSize()-1.
	/// @return     the reference to the element with the given index.
	inline T& operator [] (int nIdx) { return GetAt(nIdx); }

	/// Array operator: Returns a const reference to the element at the given index.
	/// @param nIdx the index of the element to be returned, must be between 0 and GetSize()-1.
	/// @return     the reference to the element with the given index.
	inline const T& operator [] (int nIdx) const { return GetAt(nIdx); }

	/// Returns whether the array is empty.
	/// @return true if the array has a size of zero,
	///         false if the array has a size larger than zero.
	inline bool IsEmpty() const { return m_nSize == 0; }

	/// Returns the size of the array.
	/// @return the size of the array.
	inline int GetSize() const { return m_nSize; }

	/// Returns the capacity of the array.
	/// @return the capacity of the array.
	inline int GetCapacity() const { return m_nCapacity; }

	/// Returns a non-const reference to the array elements.
	/// @return a raw pointer to the array elements. The array size
	///         is GetCapacity(), but only the first GetSize() elements
	///         are initialized and can be accessed safely.
	inline T* GetData() { return m_pData; }

	/// Returns const reference to the array elements.
	/// @return a raw pointer to the array elements. The array size
	///         is GetCapacity(), but only the first GetSize() elements
	///         are initialized and can be accessed safely.
	inline const T* GetData() const { return m_pData; }

	/// Sets the size of the array.
	/// The capacity is increased to nSize if necessary. The capacity is never decreased.
	/// If the new size is bigger than the current size,  elements are created by calling 
	/// T's default constructor. If the new szie is smaller than the current size, elements 
	/// are deleted.
	/// @param nSize the new size of the array.
	inline void SetSize(int nSize)
	{
		ASSERT(nSize >= 0);

		if(nSize < 0 || nSize == m_nSize) {
			return; // invalid arguments or nothing to do
		}

		if(nSize > m_nCapacity) {
			SetCapacity(nSize);
		}

		if(nSize < m_nSize) {
			IcbDestruct(m_pData + nSize, m_nSize - nSize);
		} else {
			IcbConstruct(m_pData + m_nSize, nSize - m_nSize);
		}

		m_nSize = nSize;
	}

	/// Sets the capacity of the array.
	/// Existing elements are removed if the new capacity is smaller than the current size.
	/// @param nCapacity the new capacity of the array.
	void SetCapacity(int nCapacity)
	{	
		ASSERT(nCapacity >= 0);

		if(nCapacity < 0 || nCapacity == m_nCapacity) {
			return; // invalid arguments or nothing to do
		}

		T*  pData = nCapacity ? (T*) new char[nCapacity * sizeof(T)] : NULL;
		int nSize = m_nSize < nCapacity ? m_nSize : nCapacity;

		if(nSize < m_nSize) {
			IcbDestruct(m_pData + nSize, m_nSize - nSize);
		}
		if(nSize > 0) {
			IcbRelocate(pData, m_pData, nSize);
		}

		delete (char*) m_pData;

		m_pData     = pData;
		m_nSize     = nSize;
		m_nCapacity = nCapacity;
	}

	/// Returns a const reference to the element at the given index.
	/// @param nIdx the index of the element to be returned, must be between 0 and GetSize()-1.
	/// @return     the reference to the element with the given index.
	inline const T& GetAt(int nIdx) const
	{
		ASSERT(nIdx >= 0 && nIdx < m_nSize);

		return m_pData[nIdx];
	}

	/// Returns a non-const reference to the element at the given index.
	/// @param nIdx the index of the element to be returned, must be between 0 and GetSize()-1.
	/// @return     the reference to the element with the given index.
	inline T& GetAt(int nIdx) 
	{
		ASSERT(nIdx >= 0 && nIdx < m_nSize);

		return m_pData[nIdx];
	}

	/// Sets the element at the given index.
	/// @param nIdx the index of the element to be set, must be between 0 and GetSize()-1.
	/// @param oVal the new value for the element.
	inline void SetAt(int nIdx, const T& oVal)  
	{
		ASSERT(nIdx >= 0 && nIdx < m_nSize);

		m_pData[nIdx] = oVal;
	}

	/// Inserts a new element at the given index. 
	/// The capacity is increased exponentially if necessary.
	/// @param nIdx the index a which the element is to be inserted, must be between 0 and GetSize().
	/// @param oVal the new value for the element.
	inline void InsertAt(int nIdx, const T& oVal)
	{
		InsertAt(nIdx, &oVal, 1);
	}

	/// Inserts new elements at the given index. 
	/// The capacity is increased exponentially if necessary.
	/// @param nIdx   the index a which the elements are to be inserted, must be between 0 and GetSize().
	/// @param oArray an array containing the elements to be inserted.
	inline void InsertAt(int nIdx, const TIcbArray<T>& oArray)
	{
		InsertAt(nIdx, oArray.GetData(), oArray.GetSize());
	}

	/// Inserts new elements at the given index. 
	/// The capacity is increased exponentially if necessary.
	/// @param nIdx the index a which the elements are to be inserted, must be between 0 and GetSize().
	/// @param pBuf pointer to the value for the first new element.
	/// @param nNum the number of elements to be inserted.
	void InsertAt(int nIdx, const T* pBuf, int nNum)
	{
		ASSERT(nIdx >= 0 && nNum >= 0 && nIdx <= m_nSize);

		if(nIdx < 0 || nNum <= 0 || nIdx > m_nSize) {
			return; // invalid arguments or nothing to do
		}

		if(m_nSize + nNum > m_nCapacity) { // grow exponentially if capacity exhausted
			SetCapacity(DoComputeCapacity(m_nSize + nNum));
		}

		if(nIdx < m_nSize) {
			IcbRelocate(m_pData + nIdx + nNum, m_pData + nIdx, m_nSize - nIdx);
		}

		IcbConstructCopy(m_pData + nIdx, nNum, pBuf);

		m_nSize += nNum;
	}

	/// Removes elements at the given index.
	/// The capacity is decreased exponentially the new size is <= 33% of the capacity.
	/// @param nIdx the index of the first element to be removed.
	/// @param nNum the number of elements to be removed.
	void RemoveAt(int nIdx, int nNum = 1)
	{
		ASSERT(nIdx >= 0 && nNum >= 0 && nIdx + nNum <= m_nSize);

		if(nIdx < 0 || nNum <= 0 || nIdx + nNum > m_nSize) {
			return; // invalid arguments or nothing to do
		}

		IcbDestruct(m_pData + nIdx, nNum);

		if(nIdx + nNum < m_nSize) {
			IcbRelocate(m_pData + nIdx, m_pData + nIdx + nNum, m_nSize - nIdx - nNum);
		}

		m_nSize -= nNum;

		if(m_nSize * 3 <= m_nCapacity) { // shrink exponentially if usage <= 33%
			SetCapacity(DoComputeCapacity(m_nSize));
		}
	}

	/// Removes all elements and sets the capacity to zero.
	inline void RemoveAll() { SetCapacity(0); }

	/// Adds a new element to the end of the array.
	/// The capacity is increased exponentially if necessary.
	/// @param oVal the value of the element to be added.
	inline void Add(const T& oVal)
	{
		if(m_nSize + 1 > m_nCapacity) { // grow exponentially if capacity exhausted
			SetCapacity(DoComputeCapacity(m_nSize + 1));
		}

		IcbConstructCopy(m_pData + m_nSize, 1, &oVal);

		m_nSize += 1;
	}

	/// Adds news element to the end of the array.
	/// The capacity is increased exponentially if necessary.
	/// @param oArray an array containing the elements to be added.
	inline void Add(const TIcbArray<T>& oArray) { Add(oArray.GetData(), oArray.GetSize()); }

	/// Adds news element to the end of the array.
	/// The capacity is increased exponentially if necessary.
	/// @param pBuf pointer to the value for the first new element.
	/// @param nNum the number of elements to be added.
	void Add(const T* pBuf, int nNum)
	{
		ASSERT(nNum >= 0);

		if(nNum <= 0) {
			return; // invalid arguments or nothing to do
		}

		if(m_nSize + nNum > m_nCapacity) { // grow exponentially if capacity exhausted
			SetCapacity(DoComputeCapacity(m_nSize + nNum));
		}

		IcbConstructCopy(m_pData + m_nSize, nNum, pBuf);

		m_nSize += nNum;
	}

	/// Returns the index of the first element equal to the given value.
	/// Equality of values is determined by the IcbCompare<>() template method.
	/// @param oVal the value to be found.
	/// @param nPos the index, where the search is to be started
	/// @return     the index of the first element equal to the given value, or -1 if no element was found.
	int Find(const T& oVal, int nPos = 0) const
	{
		for(int nIdx=nPos; nIdx<m_nSize; nIdx++) 
			if(IcbCompare(m_pData[nIdx], oVal) == 0) 
				return nIdx;
		return -1;
	}

	/// Determines whether the array contains an element equal to the given value.
	/// Equality of values is determined by the IcbCompare<>() template method.
	/// @param oVal the value to be found.
	/// @return     true if at least one element equal to the given value exists, false otherwise.
	bool Contains(const T& oVal) const 
	{
		return Find(oVal) != -1;
	}

	/// Removes the first element element equal to the given value.
	/// If no element equal to the given value exists, the array is not changed.
	/// Equality of values is determined by the IcbCompare<>() template method.
	/// @param oVal the value to be removed.
	/// @return     true if the value was removed, false if the value was not found.
	bool Remove(const T& oVal) 
	{
		int nIdx = Find(oVal);
		if(nIdx != -1) {
			RemoveAt(nIdx);
			return true;
		} else {
			return false;
		}
	}

	/// Sorts the elements of the array using IcbCompare<>() template method.
	/// @param bInverseOrder true if the elements are to be sorted ascending,
	///                      false if the elements are to be sorted descending.
	inline void Sort(bool bInverseOrder = false)
	{
		Sort(TCmpDefault(), bInverseOrder);
	}

	/// Sorts the elements of the array using the given comparator.
	/// @param oCmp the comparator to be used. The comparator must implement the method Compare(),
	///             which takes two references to array elements and returns -1 / 0 / +1
	///             if the 1st element is smaller / equal / larger than the 2nd element.
	/// @param bInverseOrder true if the elements are to be sorted ascending,
	///                      false if the elements are to be sorted descending.
	template <class CMP> void Sort(const CMP& oCmp, bool bInverseOrder = false)
	{
		DoQuickSort(m_pData, 0, m_nSize-1, bInverseOrder, oCmp);
	}

	/// Sorts the elements of the array using the default case insensitive comparator.
	/// @param bInverseOrder true if the elements are to be sorted ascending,
	///                      false if the elements are to be sorted descending.
	inline void SortNoCase(bool bInverseOrder = false)
	{
		Sort(TCmpNoCase(), bInverseOrder);
	}

	/// Finds the given element or its insertion position in a sorted array.
	/// @param oVal    the element which is to be found or whose insertion position is to be determined.
	/// @param rbFound receives true if the element was found or false if it was not found.
	/// @return        the index of the element if it was found, or 
	///                the index at which the element is to be inserted if it was not found.
	inline int SortedFind(const T& oVal, bool& rbFound) const
	{
		return SortedFind(TCmpDefault(), oVal, rbFound);
	}

	/// Finds the given element or its insertion position in a sorted array.
	/// @param oCmp the comparator to be used. The comparator must implement the method Compare(),
	///             which takes two references to array elements and returns -1 / 0 / +1
	///             if the 1st element is smaller / equal / larger than the 2nd element.
	/// @param oVal    the element which is to be found or whose insertion position is to be determined.
	/// @param rbFound receives true if the element was found or false if it was not found.
	/// @return        the index of the element if it was found, or 
	///                the index at which the element is to be inserted if it was not found.
	///                Note: Unlike Find(), the return value is never -1, but it can be equal to GetSize(). 
	template <class CMP> inline int SortedFind(const CMP& oCmp, const T& oVal, bool& rbFound) const
	{
		int nIdx1 = 0;
		int nIdx2 = m_nSize - 1;

		while(nIdx1 <= nIdx2) {
			int nIdx = (nIdx1 + nIdx2) / 2;
			ASSERT(nIdx >= nIdx1 && nIdx <= nIdx2);
			ASSERT(nIdx >= 0     && nIdx <  m_nSize);

			int nCmp = oCmp.Compare(oVal, m_pData[nIdx]);
			if(nCmp == 0) {
				rbFound = true;
				return nIdx;
			} else if(nCmp < 0) {
				nIdx2 = nIdx - 1;
			} else {
				nIdx1 = nIdx + 1;
			}
		}

		ASSERT(nIdx1 == nIdx2 + 1);
		ASSERT(nIdx1 >= 0 && nIdx1 <= m_nSize);
		rbFound = false;
		return nIdx1;
	}

	/// Returns a the elements of a subrange in a new array.
	/// @param nIdx the index of the first element to be returned.
	/// @param nNum the number of elements to be returned.
	/// @return a new array containing nNum elements.
	TIcbArray GetSubArray(int nIdx, int nNum) const
	{
		ASSERT(nIdx >= 0 && nNum >= 0);

		int nNum2;
		if(nIdx + nNum <= m_nSize) {
			nNum2 = nNum;
		} else if(nIdx < m_nSize) {
			nNum2 = m_nSize - nIdx;
		} else {
			nNum2 = 0;
		}
		TIcbArray aResult;
		aResult.Add(m_pData+nIdx, nNum2);
		return aResult;
	}
};

template <class T> inline void IcbTryAdd(TIcbArray<T>* pary, const T& x)
{
	if(pary) pary->Add(x);
}

template <class T> inline void IcbTryClr(TIcbArray<T>* pary)
{
	if(pary) pary->RemoveAll();
}

template <class T> inline int IcbCompareArray(const TIcbArray<T>& aA, const TIcbArray<T>& aB)
{
	int nSizeA = aA.GetSize();
	int nSizeB = aB.GetSize();
	for(int i=0; i<nSizeA && i<nSizeB; i++) {
		int nCmp = IcbCompare(aA[i], aB[i]);
		if(nCmp != 0)
			return nCmp;
	}
	return nSizeA - nSizeB;
}

template <class T> __forceinline bool operator==(const TIcbArray<T>& aA, const TIcbArray<T>& aB) { return IcbCompareArray(aA, aB) == 0; }
template <class T> __forceinline bool operator!=(const TIcbArray<T>& aA, const TIcbArray<T>& aB) { return IcbCompareArray(aA, aB) != 0; }
template <class T> __forceinline bool operator< (const TIcbArray<T>& aA, const TIcbArray<T>& aB) { return IcbCompareArray(aA, aB) <  0; }
template <class T> __forceinline bool operator> (const TIcbArray<T>& aA, const TIcbArray<T>& aB) { return IcbCompareArray(aA, aB) >  0; }
template <class T> __forceinline bool operator<=(const TIcbArray<T>& aA, const TIcbArray<T>& aB) { return IcbCompareArray(aA, aB) <= 0; }
template <class T> __forceinline bool operator>=(const TIcbArray<T>& aA, const TIcbArray<T>& aB) { return IcbCompareArray(aA, aB) >= 0; }

// **************************************************************************

#ifdef _DEBUG
#   undef new
#endif

// **************************************************************************
// *** foreach **************************************************************
// **************************************************************************

/// Brings the foreach() statement to C++.
///
/// EXPR can be an anything: an array, a reference or a pointer to an array. 
/// For expressions of type TIcbArray<T> or TIcbArray<T>&, a copy will be made.
/// Expressions of type TIcbArray<T>* are more efficient since no copy is made. 
/// So, when passing variables or references (no matter if global, local or on 
/// the heap), adding '&' in front of the variable is an encouraged optimization. 
///
/// Example:
///   TIcbArray<CIcbString>  array; /* OR */
///   TIcbArray<CIcbString>* array; /* OR */
///   TIcbArray<CIcbString>& array;
///
///   foreach(CIcbString, element, array) {
///	      cout << element.GetLength();
///   }
///
///   foreach(CIcbString, element, func()) {
///	      cout << element.GetLength();
///   }
///

#define foreach(TYPE, INST, EXPR) \
	for(icb_iterator<TYPE> iter(EXPR) ; iter._ary && iter._pos++ < 0 ; ) \
		for( TYPE INST ; iter.next(INST) ; )

template <class T> struct icb_iterator
{
	const TIcbArray<T>* _ary;
	const bool          _own;
	int                 _pos;

	icb_iterator(const TIcbArray<T>& ary) : _ary(new TIcbArray<T>(ary)), _own(true),  _pos(-1) { }
	icb_iterator(const TIcbArray<T>* ary) : _ary(ary),                   _own(false), _pos(-1) { }

	~icb_iterator() { 
		if(_own) delete _ary;
	}

	bool next(T& t) {
		if(_pos >= _ary->GetSize())
			return false;
		t = _ary->GetAt(_pos++);
		return true;
	}
};

/*
#define foreach(type, inst, arr) \
	TIcbArray<type> foreach_arr = arr;\
	type inst;\
	for(int foreach_itervar=0;\
		(foreach_itervar<foreach_arr.GetSize()) && ((inst = foreach_arr[foreach_itervar]) || 1);\
		++foreach_itervar)

#define foreachp(type, inst, arr) \
	TIcbArray<type>* foreach_arrp = arr;\
	type inst;\
	if(foreach_arrp)\
		for(int foreach_itervarp=0;\
		(foreach_itervarp<foreach_arrp->GetSize()) && ((inst = foreach_arrp->operator [](foreach_itervarp)) || 1);\
		++foreach_itervarp)

#define foreach(type, inst, arr) \
	TIcbArray<type> list##inst = arr;\
	type inst;\
	for(int foreach_itervar=0;\
		(foreach_itervar<list##inst.GetSize()) && ((inst = list##inst[foreach_itervar]) || 1);\
		++foreach_itervar)
*/
