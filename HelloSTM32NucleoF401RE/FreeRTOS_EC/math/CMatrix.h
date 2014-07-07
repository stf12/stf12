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

	uint32_t m_nId;

	/**
	 * Array of data arranged in row order. That is, the matrix element (i, j) is stored at:
	 * m_data[i*nColumns + j]
	 */
	T m_data[nRows * nColumns];

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
		static uint32_t nId = 1;
		m_nId = nId++;

		trace_printf("create m<%i,%i>%i\n", nRows, nColumns, m_nId);
	}

	virtual ~CMatrix() {trace_printf("dispose m<%i,%i>%i\n", nRows, nColumns, m_nId);};

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
	 * Assignment operator that initialize all element of this matrix with xValue.
	 *
	 * @param xValue specifies a float value.
	 * @return a reference to this.
	 */
	inline
	CMatrix<T, nRows, nColumns> & operator=(const T xValue);

	/**
	 * Assignment operator that initialize this matrix with an array. This method is not safe because
	 * it assumes that pfValue is a pointer to an array of  (fRows x fColumn) size;
	 *
	 * @param pxValues specifies an array of nRows x nColumns size.
	 * @return a reference to this.
	 */
	inline
	CMatrix<T, nRows, nColumns> & operator=(const T * const pxValues);

	inline
	T*operator[](int n) {
		return &m_data[n * nColumns];
	}

	inline
	CMatrix<T, nRows, nColumns> & operator+(const CMatrix<T, nRows, nColumns> &m) const;

	inline
	CMatrix<T, nRows, nColumns> & operator-(const CMatrix<T, nRows, nColumns> &m) const;

	inline
	CMatrix<T, nRows, nColumns> & operator*(T xScale);

	/**
	 * Given a squared input matrix (n x n)
	 * the algorithm returns a modified matrix and
	 * a permutation vector p (n x 1).
	 *
	 * @param p specifies the permutation vector.
	 * @return the number of permutation or -1 if the matrix is singular.
	 */
//	int LUP(CMatrix<T, nRows, 1> &p);

	inline
	CMatrix<T, nRows, nColumns> & Inverse() const;

	inline
	CMatrix<T, nRows, nColumns> & Transpose();

	inline
	CMatrix<T, nRows, nColumns> & Clone(CMatrix<T, nRows, nColumns> &m) const;

	inline
	void InitWithIdentity();

	inline
	void InitWithZero();

	inline
	void InitWithOne();

	inline
	void InitWithArray(const float32_t * const pfData);

};

//**********************************
// Copy-constructor implementation *
//**********************************

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns>::CMatrix(const CMatrix &obj) {
	memcpy(m_data, obj.m_data, sizeof(m_data));

	m_nId = obj.m_nId + 10000;
	trace_printf("cc m<%i,%i>%i\n", nRows, nColumns, m_nId);
}


//****************************************
// Member unary operators implementation *
//****************************************

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator=(const CMatrix<T, nRows, nColumns> &m) {
	memcpy(m_data, m.m_data, sizeof(m_data));

	trace_printf("copy m<%i,%i>%i to m<%i,%i>%i\n", nRows, nColumns, m.m_nId, nRows, nColumns, m_nId);

	return *this;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator=(const T xValue) {
	for(int i=0; i<nRows*nColumns; ++i)
		m_data[i] = xValue;

	return *this;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator=(const T * const pxValues) {
	memcpy(m_data, pxValues, sizeof(m_data));

	return *this;
}



template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator+(const CMatrix<T, nRows, nColumns> &m) const {
	CMatrix<T, nRows, nColumns> res;

	for (int i=0; i<nRows*nColumns; ++i)
		res.m_data[i] = m_data[i] + m.m_data[i];

	return res;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator-(const CMatrix<T, nRows, nColumns> &m) const {
	CMatrix<T, nRows, nColumns> res;

	for (int i=0; i<nRows*nColumns; ++i)
		res.m_data[i] = m_data[i] - m.m_data[i];

	return res;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::operator*(T xScale) {
	for (int i=0; i<nRows*nColumns; ++i)
		m_data[i] *= xScale;

	return *this;
}


//*******************************
// Member method implementation *
//*******************************

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::Inverse() const {
	assert(nRows == nColumns);

	CMatrix<T, nRows, nColumns> res;
	//arm_status status = arm_mat_inverse_f32(&m_matrix, &res.m_matrix);
	//TODO: STF - tbd.

	return res;
}

template <class T, int nRows, int nColumns>
CMatrix<T, nRows, nColumns> & CMatrix<T, nRows, nColumns>::Transpose() {

	//TODO: STF - tbd.

	return *this;
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
void CMatrix<T, nRows, nColumns>::InitWithArray(const float32_t * const pfData) {
	memcpy(m_data, pfData, sizeof(m_data));
}



//******************************
// Non-member binary operators *
//******************************

template<class T, int nRows1, int nColumns1Rows2, int nColumns2>
CMatrix<T, nRows1, nColumns2> & operator*(const CMatrix<T, nRows1, nColumns1Rows2> &m1, const CMatrix<T, nColumns1Rows2, nColumns2> &m2) {
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


#endif /* CMatrix_H_ */
