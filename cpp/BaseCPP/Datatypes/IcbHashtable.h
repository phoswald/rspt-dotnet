// **************************************************************************
//
/// @file: IcbHashtable.h
/// A template class for hash tables (maps)
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
// *** TIcbHashtable ********************************************************
// **************************************************************************

/// This template class provides a hash table of types KEY and VAL.
/// 
/// Insert, lookup and delete operations can be done in running time O(1).
/// The memory overhead is 12+(4*range) bytes per hash table and 4 bytes 
/// per element.
///
/// Constructors, destructors and assignment operators for keys and values are 
/// properly called. The template function IcbCompare is used to compare keys
/// and IcbHashCode to compute hash keys. Therefore, this class is suitable 
/// for complex key- or value-types such as smart pointers or MFC-like strings.

template <class KEY, class VAL> class TIcbHashtable
{
	// *** Inner Classes ****************************************************
private:
	/// Structure for each element of the hash table.
	struct SNode
	{
		SNode*	m_pNext; ///< Pointer to the next element with the same hash value.
		KEY		m_oKey;	 ///< The key of the element.
		VAL		m_oVal;  ///< The value of the element.

		/// Constructs a new node with the provided values
		/// @param pNext pointer to the next element with the same hash value.
		/// @param oKey the key of this element
		SNode(SNode* pNext, const KEY& oKey) :                   m_pNext(pNext), m_oKey(oKey)               { }

		/// Constructs a new node with the provided values
		/// @param pNext pointer to the next element with the same hash value.
		/// @param oKey the key of this element
		/// @param oVal the value of this element
		SNode(SNode* pNext, const KEY& oKey, const VAL& oVal) :  m_pNext(pNext), m_oKey(oKey), m_oVal(oVal) { }
	};

	// *** Attributes *******************************************************
private:
	SNode**	m_pData;  ///< Points to an array with m_nRange pointers to elements, or NULL if m_nRange is zero. 
	int		m_nRange; ///< The range of the hash table. 
	int		m_nSize;  ///< The number of elements present in the hash table.
		
	// *** Methods **********************************************************
private:
	/// Calculates a range with a fill factor lower than 1.0 for a specified number of elements
	/// @param nSize number of elements
	/// @return the range value to use
	static int DoComputeRange(int nSize)
	{
		if(nSize == 0) {
			return 0; 
		} else {
			int nRange = 8;
			while(nRange < nSize) nRange <<= 1;
			return nRange+1;
		}
	}

	/// Does clone a single hash table bucket.
	/// @param pNode const pointer to the first node of the original bucket
	/// @return pointer to the first node of the new bucket
	static SNode* DoClone(const SNode* pNode)
	{
		SNode*  pResult = NULL;
		SNode** ppLink  = &pResult;
		while(pNode) {
			SNode* pTmp = new SNode(NULL, pNode->m_oKey, pNode->m_oVal);
			*ppLink = pTmp;
			pNode   = pNode->m_pNext;
			ppLink  = &(pTmp->m_pNext);
		}			
		return pResult;
	}

	/// Method to make this hash table a clone of a provided hash table
	/// @param sCopy the hash table that needs to be cloned
	void DoClone(const TIcbHashtable& sCopy)
	{
		m_pData  = sCopy.m_pData ? new SNode*[sCopy.m_nRange] : NULL;
		m_nRange = sCopy.m_nRange;
		m_nSize  = sCopy.m_nSize;

		for(int i=0; i<m_nRange; i++) {
			m_pData[i] = DoClone(sCopy.m_pData[i]);
		}
	}

	/// Find the node for a given key
	/// @param oKey the key that needs to be found
	/// @return pointer to the node with the given key, may be NULL if hash table is empty or the key is not found
	SNode* DoFind(const KEY& oKey) const
	{
		if(m_pData == NULL) {
			return NULL;
		}

		unsigned nHash = IcbHashCode(oKey);
		SNode* pNode = m_pData[nHash % m_nRange];
		while(pNode && IcbCompare(oKey, pNode->m_oKey) != 0) {
			pNode = pNode->m_pNext;
		}
		return pNode;
	}

	/// Find the node for a given key
	/// @param oKey the key that needs to be found
	/// @return the address of the pointer to the node with the given key, the referenced pointer may be NULL.
	SNode** DoFindPtr(const KEY& oKey) const
	{
		unsigned nHash = IcbHashCode(oKey);
		SNode** ppNode = &m_pData[nHash % m_nRange];
		while(*ppNode && IcbCompare(oKey, (*ppNode)->m_oKey) != 0) { 
			ppNode = &((*ppNode)->m_pNext);
		}
		return ppNode;
	}

	/// Iterates over all elements and provides pointers to keys and / or values as desired.
	/// @param pPos the iteration handle: initally NULL, the received value must
	///             be passed to the next iteration as long as the return value is true.
	/// @param pKey receives the pointer to the key of each element. May be NULL when calling this method.
	/// @param pVal receives the pointer to the value of each element. May be NULL when calling this method.
	/// @param ppVal receives the pointer to the value of each element. May be NULL when calling this method.
	/// @return     true if an element was found and returned, false if the iteration ends.
	bool DoEnum(const void*& pPos, KEY* pKey, VAL* pVal, VAL** ppVal) const
	{
		unsigned nIdxStart = 0;
		if(pPos) {
			SNode* pNode = (SNode*) pPos;
			if(pNode->m_pNext) {
				pNode = pNode->m_pNext;
				if(pKey) *pKey   = pNode->m_oKey;
				if(pVal) *pVal   = pNode->m_oVal;
				if(ppVal) *ppVal = &pNode->m_oVal;
				pPos  = pNode;
				return true;
			}
			nIdxStart = (IcbHashCode(pNode->m_oKey) % m_nRange)+1;
		}
		for(int nIdx=nIdxStart; nIdx<m_nRange; nIdx++) {
			SNode* pNode = m_pData[nIdx];
			if(pNode) {
				if(pKey) *pKey   = pNode->m_oKey;
				if(pVal) *pVal   = pNode->m_oVal;
				if(ppVal) *ppVal = &pNode->m_oVal;
				pPos = pNode;
				return true;
			}
		}
		pPos = NULL; 
		return false;
	}

	/// Iterates over all elements and provides the keys and / or values as desired.
	/// @param paoKeys receives a pointer to an array of the keys of all elements.
	/// @param paoVals receives a pointer to an array of the values of all elements.
	/// @param papVals receives a pointer to an array of pointers to the values of all elements.
	void DoGetEntries(TIcbArray<KEY>* paoKeys, TIcbArray<VAL>* paoVals, TIcbArray<const VAL*>* papVals) const
	{
		if(paoKeys) paoKeys->RemoveAll();
		if(paoVals) paoVals->RemoveAll();
		if(papVals) papVals->RemoveAll();
		for(int i=0; i<m_nRange; i++) {
			const SNode* pNode = m_pData[i];
			while(pNode) {
				if(paoKeys) paoKeys->Add(pNode->m_oKey);
				if(paoVals) paoVals->Add(pNode->m_oVal);
				if(papVals) papVals->Add(&pNode->m_oVal);
				pNode = pNode->m_pNext;
			}
		}
	}

public:
	/// Constructs an empty hash table with range zero.
	TIcbHashtable() 
	{
		m_pData  = NULL;
		m_nRange = 0;
		m_nSize  = 0;
	}

	/// Constructs a new hash table by copying range and elements from the given hash table.
	/// @param oCopy the hash table to be copied.
	TIcbHashtable(const TIcbHashtable& oCopy) 
	{
		DoClone(oCopy);
	}

	/// Destructs the instance.
	~TIcbHashtable()
	{
		RemoveAll();
	}

	/// Assignment operator: Sets range and elements of this hash table.
	/// @param  oCopy the hash table to be assinged.
	/// @return a reference to the hash table itself.
	TIcbHashtable& operator = (const TIcbHashtable& oCopy)
	{
		RemoveAll();
		DoClone(oCopy);
		return *this;
	}

	/// Sets the range and re-hashes all elements.
	/// @param nRange the new range of the hash table, must be >= 0.
	///               The range 0 is only allowed if the hash table is empty.
	void SetRange(int nRange)
	{
		ASSERT(nRange >= 0 || m_nSize == 0);

		if((m_nSize > 0 && nRange == 0) || nRange == m_nRange) {
			return; // invalid arguments or nothing to do
		}

		SNode** pData  = nRange ? new SNode*[nRange] : NULL;

		for(int i=0; i<nRange; i++) {
			pData[i] = NULL;
		}

		// move nodes from m_pData to pData
		if(m_nSize) {
			for(int i=0; i<m_nRange; i++) {
				while(m_pData[i]) {
					SNode* pNode = m_pData[i];
					m_pData[i] = pNode->m_pNext;
					unsigned nHash = IcbHashCode(pNode->m_oKey);
					unsigned nIdx  = nHash % nRange;
					pNode->m_pNext = pData[nIdx];
					pData[nIdx] = pNode;
				}
			}
		}

		delete[] m_pData;

		m_pData  = pData;
		m_nRange = nRange;
	}

	/// Returns whether the hash table is empty.
	/// @return true if the hash table contains no elements,
	///         false if the hash table contains one or more elements.
	bool IsEmpty() const { return m_nSize == 0; }

	/// Returns the number of elements in the hash table.
	/// @return the number of elements in the hash table.
	int GetSize() const { return m_nSize; }

	/// Checks, wether an element with the given key is contained in the hash table.
	/// @param oKey the key of the element.
	/// @return     true if there exists an element with the given key, false otherwise.
	bool Contains(const KEY& oKey) const
	{
		return DoFind(oKey) != NULL;
	}

	/// Returns the value of the element with the given key, if existing.
	/// @param oKey the key of the element.
	/// @param oVal receives the value of the element, if existing.
	/// @return     true if an element with the given key was found and oVal was assigned, 
	///             false if no element was found and oVal was not assigned.
	bool Get(const KEY& oKey, VAL& oVal) const
	{
		const SNode* pNode = DoFind(oKey);
		if(pNode) {
			oVal = pNode->m_oVal;
			return true;
		} else {
			return false;
		}
	}

	/// Returns a const pointer to the value of the element with the given key.
	/// @param oKey the key of the element.
	/// @return a const pointer to the value of the element with the given key,
	///         or NULL if no element was found.
	const VAL* GetRef(const KEY& oKey) const
	{
		const SNode* pNode = DoFind(oKey);
		if(pNode) {
			return &pNode->m_oVal;
		} else {
			return NULL;
		}
	}

	/// Returns a non-const pointer to the value of the element with the given key.
	/// @param oKey the key of the element.
	/// @return a non-const pointer to the value of the element with the given key,
	///         or NULL if no element was found.
	VAL* GetRef(const KEY& oKey)
	{
		SNode* pNode = DoFind(oKey);
		if(pNode) {
			return &pNode->m_oVal;
		} else {
			return NULL;
		}
	}

	/// Inserts a new key/value pair or replaces the value of the element with the given key.
	/// @param oKey the key of the element to be inserted or updated.
	/// @param oVal the value of the element to be inserted or updated.
	void Put(const KEY& oKey, const VAL& oVal)
	{
		if(m_nRange == 0) {
			SetRange(DoComputeRange(1));
		}

		SNode** ppNode = DoFindPtr(oKey);
		if(*ppNode == NULL) {
			// insert
			*ppNode = new SNode(*ppNode, oKey, oVal);
			m_nSize++;
		} else {
			// replace
			(*ppNode)->m_oVal = oVal;
		}

		if(m_nSize > m_nRange) { // rehash if load factor larger than 1.0
			SetRange(DoComputeRange(m_nSize));
			// ppNode cannot be used after this point!
		}
	}

	/// Returns a reference to the value of the existing or newly created element with the given key.
	/// If no element with the given key exists, a new element with an empty value is inserted.
	/// @param oKey the key of the element to be found or inserted.
	VAL& GetOrPut(const KEY& oKey)
	{
		if(m_nRange == 0) {
			SetRange(DoComputeRange(1));
		}

		SNode** ppNode = DoFindPtr(oKey);
		if(*ppNode == NULL) {
			// insert
			*ppNode = new SNode(*ppNode, oKey);
			m_nSize++;
		}
		SNode* pNode = *ppNode;

		if(m_nSize > m_nRange) { // rehash if load factor larger than 1.0
			SetRange(DoComputeRange(m_nSize));
			// ppNode cannot be used after this point!
		}

		return pNode->m_oVal;
	}

	/// Returns a reference to the value of the existing or newly created element with the given key.
	/// If no element with the given key exists, a new element with the given value is inserted.
	/// @param oKey     the key of the element to be found or inserted.
	/// @param oValDflt the value to be assigned when a new element is inserted.
	VAL& GetOrPut(const KEY& oKey, const VAL& oValDflt)
	{
		if(m_nRange == 0) {
			SetRange(DoComputeRange(1));
		}

		SNode** ppNode = DoFindPtr(oKey);
		if(*ppNode == NULL) {
			// insert
			*ppNode = new SNode(*ppNode, oKey, oValDflt);
			m_nSize++;
		}
		SNode* pNode = *ppNode;

		if(m_nSize > m_nRange) { // rehash if load factor larger than 1.0
			SetRange(DoComputeRange(m_nSize));
			// ppNode cannot be used after this point!
		}

		return pNode->m_oVal;
	}

	/// Removes the element with the given key, if existing.
	/// @param oKey the key of the element to be removed.
	void Remove(const KEY& oKey)
	{
		if(m_nRange == 0) {
			return;
		}

		SNode** ppNode = DoFindPtr(oKey);
		if(*ppNode) {
			// remove
			SNode* pNode = *ppNode;
			*ppNode = pNode->m_pNext;
			delete pNode;
			m_nSize--;
		}

		if(3 * m_nSize <= m_nRange) { // rehash if load factor smaller than 0.33
			SetRange(DoComputeRange(m_nSize));
			// ppNode cannot be used after this point!
		}
	}

	/// Removes all elements and sets the range to zero.
	void RemoveAll()
	{
		for(int i=0; i<m_nRange; i++) {
			SNode* pNode = m_pData[i];
			while(pNode) {
				SNode* pNext = pNode->m_pNext;
				delete pNode;
				pNode = pNext;
			}
		}
		delete[] m_pData;
		m_pData  = NULL;
		m_nRange = 0;
		m_nSize  = 0;
	}

	/// Iterates over the keys of all elements.
	/// @param pPos the iteration handle: initally NULL, the received value must
	///             be passed to the next iteration as long as the return value is true.
	/// @param oKey receives the key of each element.
	/// @return     true if an element was found and returned, false if the iteration ends.
	bool EnumKeys(const void*& pPos, KEY& oKey) const
	{
		return DoEnum(pPos, &oKey, NULL, NULL);
	}

	/// Iterates over the values of all elements.
	/// @param pPos the iteration handle: initally NULL, the received value must
	///             be passed to the next iteration as long as the return value is true.
	/// @param oVal receives the value of each element.
	/// @return     true if an element was found and returned, false if the iteration ends.
	bool EnumVals(const void*& pPos, VAL& oVal) const
	{
		return DoEnum(pPos, NULL, &oVal, NULL);
	}

	/// Iterates over the non-const pointers to the values of all elements.
	/// @param pPos the iteration handle: initally NULL, the received value must
	///             be passed to the next iteration as long as the return value is true.
	/// @param pVal receives the pointer to the value of each element.
	/// @return     true if an element was found and returned, false if the iteration ends.
	bool EnumValRefs(const void*& pPos, VAL*& pVal)
	{
		return DoEnum(pPos, NULL, NULL, &pVal);
	}

	/// Iterates over the const pointers to the values of all elements.
	/// @param pPos the iteration handle: initally NULL, the received value must
	///             be passed to the next iteration as long as the return value is true.
	/// @param pVal receives the pointer to the value of each element.
	/// @return     true if an element was found and returned, false if the iteration ends.
	bool EnumValRefs(const void*& pPos, const VAL*& pVal) const 
	{
		return DoEnum(pPos, NULL, NULL, (VAL**) &pVal); // TODO: can we get rid of this cast?
	}

	/// Iterates over the keys and values of all elements.
	/// @param pPos the iteration handle: initally NULL, the received value must
	///             be passed to the next iteration as long as the return value is true.
	/// @param oKey receives the key of each element.
	/// @param oVal receives the value of each element.
	/// @return     true if an element was found and returned, false if the iteration ends.
	bool EnumKeyVals(const void*& pPos, KEY& oKey, VAL& oVal) const
	{
		return DoEnum(pPos, &oKey, &oVal, NULL);
	}

	/// Iterates over the keys and non-const pointers to the values of all elements.
	/// @param pPos the iteration handle: initally NULL, the received value must
	///             be passed to the next iteration as long as the return value is true.
	/// @param oKey receives the key of each element.
	/// @param pVal receives the pointer to the value of each element.
	/// @return     true if an element was found and returned, false if the iteration ends.
	bool EnumKeyValRefs(const void*& pPos, KEY& oKey, VAL*& pVal)
	{
		return DoEnum(pPos, &oKey, NULL, &pVal);
	}

	/// Iterates over the keys and const pointers to the values of all elements.
	/// @param pPos the iteration handle: initally NULL, the received value must
	///             be passed to the next iteration as long as the return value is true.
	/// @param oKey receives the key of each element.
	/// @param pVal receives the pointer to the value of each element.
	/// @return     true if an element was found and returned, false if the iteration ends.
	bool EnumKeyValRefs(const void*& pPos, KEY& oKey, const VAL*& pVal) const 
	{
		return DoEnum(pPos, &oKey, NULL, (VAL**) &pVal); // TODO: can we get rid of this cast?
	}

	/// Returns the keys of all elements.
	/// @param aoKeys receives the keys of all elements.
	void GetKeys(TIcbArray<KEY>& aoKeys) const 
	{
		DoGetEntries(&aoKeys, NULL, NULL);
	}

	/// Returns the values of all elements.
	/// @param aoVals receives the values of all elements.
	void GetVals(TIcbArray<VAL>& aoVals) const 
	{
		DoGetEntries(NULL, &aoVals, NULL);
	}

	/// Returns const pointers to the values of all elements.
	/// @param apVals receives the pointers to the values of all elements.
	void GetValRefs(TIcbArray<const VAL*>& apVals) const 
	{
		DoGetEntries(NULL, NULL, &apVals);
	}

	/// Returns the keys and values of all elements.
	/// @param aoKeys receives the keys of all elements.
	/// @param aoVals receives the values of all elements.
	void GetKeyVals(TIcbArray<KEY>& aoKeys, TIcbArray<VAL>& aoVals) const 
	{
		DoGetEntries(&aoKeys, &aoVals, NULL);
	}

	/// Returns the keys and const pointers to the values of all elements.
	/// @param aoKeys receives the keys of all elements.
	/// @param apVals receives the pointers to the values of all elements.
	void GetKeyValRefs(TIcbArray<KEY>& aoKeys, TIcbArray<const VAL*>& apVals) const 
	{
		DoGetEntries(&aoKeys, &aoVals, NULL);
	}
};

// **************************************************************************

#ifdef _DEBUG
#   undef new
#endif
