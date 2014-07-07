/*
 * CMatrix.h
 *
 *  Created on: Jun 30, 2014
 *      Author: oliveris
 */

#ifndef CMATRIXF32_H_
#define CMATRIXF32_H_

#include <assert.h>
#include "diag/Trace.h"

#ifdef __cplusplus
extern "C" {
#undef __cplusplus
#include "arm_math.h"
#define __cplusplus 1
}
#endif


namespace freertosec {
namespace math {

template <int nRows, int nColumns>
class CMatrixF32 {
private:

#ifdef DEBUG
	uint32_t m_nId;
#endif

	/**
	 * Array of data arranged in row order. That is, the matrix element (i, j) is stored at:
	 * m_data[i*nColumns + j]
	 */
	float32_t m_data[nRows * nColumns];

	/**
	 * Matrix type defined by the DSP library.
	 */
	arm_matrix_instance_f32 m_matrix;

	/**
	 * Get the element at position (i,j).
	 *
	 * @param i specifies a row index.
	 * @param j specifies a column index
	 * @return the element at (i,j).
	 */
	inline
	float32_t &ElementAt(int i, int j) { return m_data[i*nColumns + j]; }

public:

	/**
	 * The default constructor.
	 */
	CMatrixF32() {
#ifdef DEBUG
		static uint32_t nId = 1;
		m_nId = nId++;
		trace_printf("create m<%i,%i>%i\n", nRows, nColumns, m_nId);
#endif

		m_matrix.numCols = nColumns;
		m_matrix.numRows = nRows;
		m_matrix.pData = m_data;
	}

	virtual ~CMatrixF32() {
#ifdef DEBUG
		trace_printf("dispose m<%i,%i>%i\n", nRows, nColumns, m_nId);
#endif
	}

	/**
	 * Copy-constructor.
	 */
	CMatrixF32(const CMatrixF32 &obj);


	/**
	 * Assignment operator that copies the matrix m into this object.
	 *
	 * @param m specifies the source matrix.
	 * @return a reference to this.
	 */
	inline
	CMatrixF32<nRows, nColumns> & operator=(const CMatrixF32<nRows, nColumns> &m);

	/**
	 * Assignment operator that initialize all element of this matrix with fValue.
	 *
	 * @param fValue specifies a float value.
	 * @return a reference to this.
	 */
	inline
	CMatrixF32<nRows, nColumns> & operator=(const float32_t fValue);

	/**
	 * Assignment operator that initialize this matrix with an array. This method is not safe because
	 * it assumes that pfValue is a pointer to an array of  (fRows x fColumn) size;
	 *
	 * @param pfValue specifies an array of nRows x nColumns size.
	 * @return a reference to this.
	 */
	inline
	CMatrixF32<nRows, nColumns> & operator=(const float32_t * const pfValue);

	inline
	operator const arm_matrix_instance_f32*() const { return &m_matrix; }

	inline
	operator arm_matrix_instance_f32*() { return &m_matrix; }

	inline
	float32_t*operator[](int n) {
		return &m_data[n * nColumns];
	}

	inline
	const CMatrixF32<nRows, nColumns> operator+(const CMatrixF32<nRows, nColumns> &m) const;

	inline
	const CMatrixF32<nRows, nColumns> operator-(const CMatrixF32<nRows, nColumns> &m) const;

	inline
	const CMatrixF32<nRows, nColumns> operator*(const float32_t fScale) const;

	inline
	CMatrixF32<nRows, nColumns> & operator+=(const CMatrixF32<nRows, nColumns> &m);
	inline
	CMatrixF32<nRows, nColumns> & operator-=(const CMatrixF32<nRows, nColumns> &m);

	inline
	CMatrixF32<nRows, nColumns> & operator*=(const float32_t fScale);

	inline
	bool operator==(const CMatrixF32<nRows, nColumns> &rhs) const;

	inline
	bool operator!=(const CMatrixF32<nRows, nColumns> &rhs) const;

	/**
	 * Given a squared input matrix (n x n)
	 * the algorithm returns a modified matrix and
	 * a permutation vector p (n x 1).
	 *
	 * @param p specifies the permutation vector.
	 * @return the number of permutation or -1 if the matrix is singular.
	 */
	int LUP(CMatrixF32<nRows, 1> &p);

	inline
	const CMatrixF32<nRows, nColumns> Inverse() const;

	inline
	CMatrixF32<nRows, nColumns> & Transpose();

	inline
	CMatrixF32<nRows, nColumns> & Clone(CMatrixF32<nRows, nColumns> &m) const;

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

template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns>::CMatrixF32(const CMatrixF32 &obj) {
	memcpy(m_data, obj.m_data, sizeof(m_data));
	m_matrix.numRows = nRows;
	m_matrix.numCols = nColumns;
	m_matrix.pData = m_data;

#ifdef DEBUG
	m_nId = obj.m_nId + 10000;
	trace_printf("cc m<%i,%i>%i\n", nRows, nColumns, m_nId);
#endif
}


//****************************************
// Member unary operators implementation *
//****************************************

template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns> & CMatrixF32<nRows, nColumns>::operator=(const CMatrixF32<nRows, nColumns> &m) {
	memcpy(m_data, m.m_data, sizeof(m_data));

#ifdef DEBUG
	trace_printf("copy m<%i,%i>%i to m<%i,%i>%i\n", nRows, nColumns, m.m_nId, nRows, nColumns, m_nId);
#endif

	return *this;
}

template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns> & CMatrixF32<nRows, nColumns>::operator=(const float32_t fValue) {
	for(int i=0; i<nRows*nColumns; ++i)
		m_data[i] = fValue;

	return *this;
}

template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns> & CMatrixF32<nRows, nColumns>::operator=(const float32_t * const pfValue) {
	memcpy(m_data, pfValue, sizeof(m_data));

	return *this;
}



template <int nRows, int nColumns>
const CMatrixF32<nRows, nColumns> CMatrixF32<nRows, nColumns>::operator+(const CMatrixF32<nRows, nColumns> &m) const {
	CMatrixF32<nRows, nColumns> res;

	arm_status status = arm_mat_add_f32(&m_matrix, &m.m_matrix, &res.m_matrix);

	return res;
}

template <int nRows, int nColumns>
const CMatrixF32<nRows, nColumns> CMatrixF32<nRows, nColumns>::operator-(const CMatrixF32<nRows, nColumns> &m) const {
	CMatrixF32<nRows, nColumns> res;

	arm_status status = arm_mat_sub_f32(&m_matrix, &m.m_matrix, &res.m_matrix);

	return res;
}

template <int nRows, int nColumns>
const CMatrixF32<nRows, nColumns> CMatrixF32<nRows, nColumns>::operator*(const float32_t fScale) const{
	CMatrixF32<nRows, nColumns> res;

	arm_status status = arm_mat_scale_f32(&m_matrix, fScale, &res.m_matrix);

	return res;
}

template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns> & CMatrixF32<nRows, nColumns>::operator+=(const CMatrixF32<nRows, nColumns> &m) {
//	CMatrixF32<nRows, nColumns> res;

	arm_status status = arm_mat_add_f32(&m_matrix, &m.m_matrix, &m_matrix);

	return *this;
}

template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns> & CMatrixF32<nRows, nColumns>::operator-=(const CMatrixF32<nRows, nColumns> &m) {
//	CMatrixF32<nRows, nColumns> res;

	arm_status status = arm_mat_sub_f32(&m_matrix, &m.m_matrix, &m_matrix);

	return *this;
}


template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns> & CMatrixF32<nRows, nColumns>::operator*=(const float32_t fScale) {
//	CMatrixF32<nRows, nColumns> res;

	arm_status status = arm_mat_scale_f32(&m_matrix, fScale, &m_matrix);
//	*this = res;

	return *this;
}

template <int nRows, int nColumns>
bool CMatrixF32<nRows, nColumns>::operator==(const CMatrixF32<nRows, nColumns> &rhs) const {
	return !memcmp(m_data, rhs.m_data, sizeof(m_data));
}

template <int nRows, int nColumns>
bool CMatrixF32<nRows, nColumns>::operator!=(const CMatrixF32<nRows, nColumns> &rhs) const {
	return ! operator==(rhs);
}


//*******************************
// Member method implementation *
//*******************************

template <int nRows, int nColumns>
const CMatrixF32<nRows, nColumns> CMatrixF32<nRows, nColumns>::Inverse() const {
	assert(nRows == nColumns);

	CMatrixF32<nRows, nColumns> res;
	arm_status status = arm_mat_inverse_f32(&m_matrix, &res.m_matrix);

	return res;
}

template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns> & CMatrixF32<nRows, nColumns>::Transpose() {
	CMatrixF32<nRows, nColumns> res = *this;

	arm_status status = arm_mat_trans_f32(&m_matrix, &res.m_matrix);

	memcpy(m_data, res.m_data, sizeof(m_data));

	return *this;
}

template <int nRows, int nColumns>
int CMatrixF32<nRows, nColumns>::LUP(CMatrixF32<nRows, 1> &p) {
	assert(nColumns == nRows);

	int	i, j, k;
	int	iMax;
	int	retNumPerm = 0;
//	short int sTmp;
	float32_t sTmp;
	float32_t fP1, fP2; /* Pivot Variables */

	// Initialize the vector p.
	for (i=0; i < nColumns; ++i)
		p[i][0] = (float32_t) (i);

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

template <int nRows, int nColumns>
CMatrixF32<nRows, nColumns> & CMatrixF32<nRows, nColumns>::Clone(CMatrixF32<nRows, nColumns> &m) const {
	memcpy(m.m_data, m_data, sizeof(m_data));
	return m;
}

template <int nRows, int nColumns>
void CMatrixF32<nRows, nColumns>::InitWithIdentity() {
	for (int i=0; i<nRows; ++i) {
		for (int j=0; j<nColumns; ++j) {
			ElementAt(i, j) = i == j ? 1.0 : 0.0;
		}
	}
}

template <int nRows, int nColumns>
void CMatrixF32<nRows, nColumns>::InitWithZero() {
	for (int i=0; i<nRows*nColumns; ++i) {
		m_data[i] = 0.0;
	}
}

template <int nRows, int nColumns>
void CMatrixF32<nRows, nColumns>::InitWithOne() {
	for (int i=0; i<nRows*nColumns; ++i) {
		m_data[i] = 1.0;
	}
}

template <int nRows, int nColumns>
void CMatrixF32<nRows, nColumns>::InitWithArray(const float32_t * const pfData) {
	memcpy(m_data, pfData, sizeof(m_data));
}



//******************************
// Non-member binary operators *
//******************************


template<int nRows1, int nColumns1Rows2, int nColumns2>
inline
const CMatrixF32<nRows1, nColumns2> operator*(const CMatrixF32<nRows1, nColumns1Rows2> &m1, const CMatrixF32<nColumns1Rows2, nColumns2> &m2) {
	CMatrixF32<nRows1, nColumns2> res;

	arm_status status = arm_mat_mult_f32(m1, m2, res);

	return res;
}


} /* namespace math */
} /* namespace freertosec */


#endif /* CMATRIXF32_H_ */
