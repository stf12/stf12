/*
 * CMatrix.h
 *
 *  Created on: Jun 30, 2014
 *      Author: oliveris
 */

#ifndef CMATRIX_H_
#define CMATRIX_H_

#include <assert.h>
#include "diag/Trace.h"


namespace freertosec {
namespace math {

template <class T, int nRows, int nColumns>
class CMatrix {
private:

#ifdef DEBUG
	uint32_t m_nId;
#endif

	/**
	 * Array of data arranged in row order. That is, the matrix element (i, j) is stored at:
	 * m_data[i*nColumns + j]
	 */
	T m_data[nRows * nColumns];

public:

	/**
	 * Get the element at position (i,j).
	 *
	 * @param i specifies a row index.
	 * @param j specifies a column index
	 * @return the element at (i,j).
	 */
	inline
	T &ElementAt(int i, int j) { return m_data[i*nColumns + j]; }

public:

	/**
	 * The default constructor.
	 */
	CMatrix() {
#ifdef DEBUG
		static uint32_t nId = 1;
		m_nId = nId++;
//		trace_printf("create m<%i,%i>%i\n", nRows, nColumns, m_nId); //TODO - trace
#endif
	}

	virtual ~CMatrix() {
#ifdef DEBUG
//		trace_printf("dispose m<%i,%i>%i\n", nRows, nColumns, m_nId); //TODO - trace
#endif
	}

	/**
	 * Copy-constructor.
	 */
	CMatrix(const CMatrix &obj);


	/**
	 * Assignment operator that copies the matrix m into this object.
	 *
	 * @param m specifies the source matrix.
	 * @return a reference to this.
	 */
	inline
	CMatrix<T, nRows, nColumns> & operator=(const CMatrix<T, nRows, nColumns> &m);

	/**
	 * Assignment operator that initialize all element of this matrix with fValue.
	 *
	 * @param fValue specifies a float value.
	 * @return a reference to this.
	 */
	inline
	CMatrix<T, nRows, nColumns> & operator=(const T fValue);

	/**
	 * Assignment operator that initialize this matrix with an array. This method is not safe because
	 * it assumes that pfValue is a pointer to an array of  (fRows x fColumn) size;
	 *
	 * @param pfValue specifies an array of nRows x nColumns size.
	 * @return a reference to this.
	 */
	inline
	CMatrix<T, nRows, nColumns> & operator=(const T * const pfValue);

	inline
	T*operator[](int n) {
		return &m_data[n * nColumns];
	}

	inline
	const T*operator[](int n) const{
		return &m_data[n * nColumns];
	}

	inline
	const CMatrix<T, nRows, nColumns> operator+(const CMatrix<T, nRows, nColumns> &m) const;

	inline
	const CMatrix<T, nRows, nColumns> operator-(const CMatrix<T, nRows, nColumns> &m) const;

	inline
	const CMatrix<T, nRows, nColumns> operator*(const T xScale) const;

	inline
	CMatrix<T, nRows, nColumns> & operator+=(const CMatrix<T, nRows, nColumns> &m);
	inline
	CMatrix<T, nRows, nColumns> & operator-=(const CMatrix<T, nRows, nColumns> &m);

	inline
	CMatrix<T, nRows, nColumns> & operator*=(const T xScale);

	inline
	bool operator==(const CMatrix<T, nRows, nColumns> &rhs) const;

	inline
	bool operator!=(const CMatrix<T, nRows, nColumns> &rhs) const;

	/**
	 * Given a squared input matrix (n x n)
	 * the algorithm returns a modified matrix and
	 * a permutation vector p (n x 1).
	 *
	 * @param p specifies the permutation vector.
	 * @return the number of permutation or -1 if the matrix is singular.
	 */
	int LUP(CMatrix<T, nRows, 1> &p);

	inline
	const CMatrix<T, nRows, nColumns> Inverse() const;

	inline
	const CMatrix<T, nColumns, nRows> Transpose() const;

	inline
	CMatrix<T, nRows, nColumns> & Clone(CMatrix<T, nRows, nColumns> &m) const;

	inline
	void InitWithIdentity();

	inline
	void InitWithZero();

	inline
	void InitWithOne();

	inline
	void InitWithArray(const T * const pfData);

};

//**********************************
// Copy-constructor implementation *
//**********************************

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns>::CMatrix(const CMatrix &obj) {
	memcpy(m_data, obj.m_data, sizeof(m_data));


#ifdef DEBUG
	m_nId = obj.m_nId + 10000;
//	trace_printf("cc m<%i,%i>%i\n", nRows, nColumns, m_nId); //TODO - trace
#endif
}


//****************************************
// Member unary operators implementation *
//****************************************

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator=(const CMatrix<T, nRows, nColumns> &m) {
	memcpy(m_data, m.m_data, sizeof(m_data));

#ifdef DEBUG
//	trace_printf("copy m<%i,%i>%i to m<%i,%i>%i\n", nRows, nColumns, m.m_nId, nRows, nColumns, m_nId); //TODO - trace
#endif

	return *this;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator=(const T fValue) {
	for(int i=0; i<nRows*nColumns; ++i)
		m_data[i] = fValue;

	return *this;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator=(const T * const pfValue) {
	memcpy(m_data, pfValue, sizeof(m_data));

	return *this;
}



template <class T, int nRows, int nColumns>
const CMatrix<T, nRows, nColumns> CMatrix<T, nRows, nColumns>::operator+(const CMatrix<T, nRows, nColumns> &m) const {
	CMatrix<T, nRows, nColumns> res;

	for(int i=0; i<nRows*nColumns; ++i)
		res.m_data[i] = m_data[i] + m.m_data[i];

	return res;
}

template <class T, int nRows, int nColumns>
const CMatrix<T, nRows, nColumns> CMatrix<T, nRows, nColumns>::operator-(const CMatrix<T, nRows, nColumns> &m) const {
	CMatrix<T, nRows, nColumns> res;

	for(int i=0; i<nRows*nColumns; ++i)
		res.m_data[i] = m_data[i] - m.m_data[i];

	return res;
}

template <class T, int nRows, int nColumns>
const CMatrix<T, nRows, nColumns> CMatrix<T, nRows, nColumns>::operator*(const T xScale) const{
	CMatrix<T, nRows, nColumns> res;

	for(int i=0; i<nRows*nColumns; ++i)
		res.m_data[i] *= xScale;

	return res;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator+=(const CMatrix<T, nRows, nColumns> &m) {

	for (int i=0; i<nRows*nColumns; ++i)
		m_data[i] += m.m_data[i];

	return *this;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator-=(const CMatrix<T, nRows, nColumns> &m) {

	for (int i=0; i<nRows*nColumns; ++i)
		m_data[i] -= m.m_data[i];

	return *this;
}


template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator*=(const T xScale) {

	for (int i=0; i<nRows*nColumns; ++i)
		m_data[i] *= xScale;

	return *this;
}

template <class T, int nRows, int nColumns>
bool CMatrix<T, nRows, nColumns>::operator==(const CMatrix<T, nRows, nColumns> &rhs) const {
	return !memcmp(m_data, rhs.m_data, sizeof(m_data));
}

template <class T, int nRows, int nColumns>
bool CMatrix<T, nRows, nColumns>::operator!=(const CMatrix<T, nRows, nColumns> &rhs) const {
	return ! operator==(rhs);
}


//*******************************
// Member method implementation *
//*******************************

template <class T, int nRows, int nColumns>
const CMatrix<T, nRows, nColumns> CMatrix<T, nRows, nColumns>::Inverse() const {
	assert(nRows == nColumns);

	CMatrix<T, nRows, nColumns> res;
	CMatrix<T, nRows, nColumns> A = *this;
	CMatrix<T, nRows, nColumns> B;
	CMatrix<T, nRows, 1> P;
	int k;

	if (A.LUP(P) != -1) {
		for (int i=0; i<nColumns; ++i) {
			B = (T)0;
			B.ElementAt(i, 0) = (T)1;
			//iNEMO_MatBackSubs(A, B, P, pDest, i);
			for (k=0; k<nColumns; ++k) {
				for(int h=k+1; h<nColumns; ++h) {
					B[P[h][0]][0] -= A[P[h][0]][k] * B[P[k][0]][0];
				}
			}

			res[nColumns-1][i] = B[P[nColumns-1][0]][0] / A[P[nColumns-1][0]][nColumns-1];

			for (k=nColumns-2; k>=0; --k) {
				T sum = 0;
				for (int j=k+1; j<nColumns; ++j) {
					sum += A[P[k][0]][j] * res[j][i];
				}
				T temp = B.ElementAt(P.ElementAt(k,0),0) - sum;
				res.ElementAt(k,i) = temp / A.ElementAt(P.ElementAt(k,0),k);
			}
		}
	}

	return res;
}

template <class T, int nRows, int nColumns>
const CMatrix<T, nColumns, nRows> CMatrix<T, nRows, nColumns>::Transpose() const {
	CMatrix<T, nColumns, nRows> res;

	for (int i=0; i<nColumns; ++i) {
		for (int j=0; j<nRows; ++j) {
			res.ElementAt(i, j) = m_data[j*nColumns + i];
		}
	}

	return res;
}

template <class T, int nRows, int nColumns>
int CMatrix<T, nRows, nColumns>::LUP(CMatrix<T, nRows, 1> &p) {
	assert(nColumns == nRows);

	int	i, j, k;
	int	iMax;
	int	retNumPerm = 0;
//	short int sTmp;
	T sTmp;
	T fP1, fP2; /* Pivot Variables */

	// Initialize the vector p.
	for (i=0; i < nColumns; ++i)
		p[i][0] = (T) (i);

	/* Partial Pivoting */
	for (k=0; k < nColumns; ++k)
	{
		for (i=k, iMax=k, fP1=0.0f; i < nColumns; ++i)
		{
			/* Local ABS */
			if (ElementAt(p[i][0], k) > 0)
				fP2 = ElementAt(p[i][0], k);
			else
				fP2 = - ElementAt(p[i][0], k);
			if (fP2 > fP1)
			{
				fP1 = fP2;
				iMax = i;
			}
		}
		/* Row exchange, update permutation vector */
		if (k != iMax)
		{
			retNumPerm++;
			sTmp = p[k][0];
			p[k][0] = p[iMax][0];
			p[iMax][0] = sTmp;
		}

		/* Suspected Singular Matrix */
		if (ElementAt(p[k][0], k) == 0.0f)
			return (-1);

		for (i=k+1; i < nColumns; ++i)
		{
			/* Calculate Mat [i][j] */
			ElementAt(p[i][0], k) = ElementAt(p[i][0], k) / ElementAt(p[k][0], k);

			/* Elimination */
			for (j=k+1; j < nColumns; ++j)
			{
				ElementAt(p[i][0], j) -= ElementAt(p[i][0], k) * ElementAt(p[k][0], j);
			}


		}
	}
	return (retNumPerm);
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::Clone(CMatrix<T, nRows, nColumns> &m) const {
	memcpy(m.m_data, m_data, sizeof(m_data));
	return m;
}

template <class T, int nRows, int nColumns>
void CMatrix<T, nRows, nColumns>::InitWithIdentity() {
	for (int i=0; i<nRows; ++i) {
		for (int j=0; j<nColumns; ++j) {
			ElementAt(i, j) = i == j ? 1.0 : 0.0;
		}
	}
}

template <class T, int nRows, int nColumns>
void CMatrix<T, nRows, nColumns>::InitWithZero() {
	for (int i=0; i<nRows*nColumns; ++i) {
		m_data[i] = 0.0;
	}
}

template <class T, int nRows, int nColumns>
void CMatrix<T, nRows, nColumns>::InitWithOne() {
	for (int i=0; i<nRows*nColumns; ++i) {
		m_data[i] = 1.0;
	}
}

template <class T, int nRows, int nColumns>
void CMatrix<T, nRows, nColumns>::InitWithArray(const T * const pfData) {
	memcpy(m_data, pfData, sizeof(m_data));
}



//******************************
// Non-member binary operators *
//******************************


template<class T, int nRows1, int nColumns1Rows2, int nColumns2>
inline
const CMatrix<T, nRows1, nColumns2> operator*(const CMatrix<T, nRows1, nColumns1Rows2> &m1, const CMatrix<T, nColumns1Rows2, nColumns2> &m2) {
	CMatrix<T, nRows1, nColumns2> res;

	res.InitWithZero();
	for (int i=0; i<nRows1; ++i) {
		for (int j=0; j<nColumns2; ++j) {
			for (int k=0; k<nColumns1Rows2; ++k) {
				res[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}

	return res;
}


} /* namespace math */
} /* namespace freertosec */


#endif /* CMATRIX_H_ */
