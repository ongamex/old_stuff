/******************************************************************************/
/*                                                                            */
/*  File: darray.h                                                            */
/*  Author: bkenwright@xbdev.net                                              */
/*  Date: 19-01-06                                                            */
/*                                                                            */
/******************************************************************************/
/*
*/
/******************************************************************************/

#ifndef DARRAY_H
#define DARRAY_H

#include <Windows.h>
#include "common.h"

template <class Type>
class DArray
{
public:
	DArray() : m_max(0), m_val(NULL) {};
	~DArray(){ delete[] m_val; };

	int Size() const {return m_max;}

	void Resize(int size)
	{
		DBG_ASSERT(size>=0 && size<100000);
		if (size==m_max) return;

		Type* temp = m_val;
		m_val = NULL;
		if (size>0)
		{
			m_val = new Type[size];
		}

		int num = min(size, m_max);
		for (int i=0; i<num; i++)
		{
			m_val[i] = temp[i];
		}
		if (temp)
		{
			delete[] temp;
		}

		m_max = size;
	}

	void Push(Type& pushData)
	{
		DBG_ASSERT(m_max>=0 && m_max<100000);

		Type* oldVal = m_val;
		int   oldMax = m_max;
		m_val = NULL;
		m_val = new Type[m_max + 1];
		m_max++;

		for (int i=0; i<oldMax; i++)
		{
			m_val[i] = oldVal[i];
		}

		DBG_ASSERT(m_max>0);
		m_val[m_max - 1] = pushData;

		delete[] oldVal;
	}

	Type& Pop()
	{
		DBG_ASSERT(m_max>0);
		DBG_ASSERT(m_val);

		return m_val[m_max-1];

		Resize(m_max-1);
	}

	void Remove(int indx)
	{
		DBG_ASSERT(m_max>0);
		DBG_ASSERT(indx>=0 && indx<m_max);

		for (int i=indx; i<(m_max-1); i++)
		{
			m_val[i] = m_val[i+1];
		}

		Resize(m_max-1);
		
	}



	Type& operator[](int indx) const
	{
		DBG_ASSERT(indx>=0 && indx< m_max);
		return m_val[indx];
	}

private:

	DArray(DArray& rhs){}

	DArray& operator=(DArray& rhs)
	{
		return *this;
	}

	int		m_max;
	Type*	m_val;
};


#endif //DARRAY_H




