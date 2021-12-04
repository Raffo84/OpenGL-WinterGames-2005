#ifndef DATATYPES_H
#define DATATYPES_H

#include "Defs.h"

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

namespace CAMP {	

	template <class T> class Matrix3;
	template <class T> class Matrix4;
	template <class T> class Quaternion;

	/**
	 * \ingroup dtypes
	 * A vector class.
	 */
	template <class T>
	class Vector3
	{
	public:
		/**
		 * Standard constructor.
		 * Be aware of the fact the vectors values are undefined after calling this constructor.
		 */
		Vector3(void);

		/**
		 * Constructor to explicitly pass three scalar values.
		 */
		Vector3(T x, T y, T z);

		/**
		 * Constructor to pass an array of scalar values.
		 */
		Vector3(T* initArray);

		/**
		 * Init the whole vector with the given scalar.
		 */
		Vector3(T initScalar);

		/**
		 * Copy Constructor.
		 */
		Vector3(const Vector3<T> &copy_me);

		/**
		 * Data accessing operator.
		 * @return Returns const scalar values.
		 */
		const T& operator[] (int i) const;

		/**
		 * Data accesing operator.
		 * @return Returns non const scalar values.
		 */
		T& operator[] (int i);

		/**
		 * Negation operator.
		 */
		inline Vector3<T> &operator -();

		/**
		 * Assignment operator, which maybe used to copy vectors. Note that there is no explicit copy constructor available.
		 */
		inline Vector3<T> &operator = (const Vector3<T>&);

		/**
		 * Assignment operator.
		 * @param scalar Takes a scalar value as parameter which will be assigned to all vector components.
		 */
		inline Vector3<T> &operator = (T scalar);

		/**
		 * Arithmetic addition.
		 */
		inline Vector3<T> &operator += (const Vector3<T>&);

		/**
		 * Arithmetic subtraction.
		 */
		inline Vector3<T> &operator -= (const Vector3<T>&);

		/**
		 * Arithmetic product with a scalar.
		 */
		inline Vector3<T> &operator *= (T);

		/**
		 * Arithmetic division by a scalar.
		 */
		inline Vector3<T> &operator /= (T);

		/**
		 * Test for equality of this vector and rhs within the given tolerance.
		 */
		inline bool equals(const Vector3<T> &rhs, const T tolerance) const;

		/**
		 * Function returning the vector's square of the vectors norm.
		 */
		inline T normSqr(void);

		/**
		 * Function returning the vector's norm.
		 */
		inline T norm(void);

		/**
		 * Function to convert this vector to the unit vector.
		 */
		inline Vector3<T> &normalize(void);

		/**
		 * Function returning the a vector's cross matrix.
		 * @param m Reference to a matrix to store \f$[\vec{v}]_x\f$.
		 * @return Reference to the matrix \f$[\vec{v}]_x\f$ which holds to the following equation:
		 * \f[
		 *		\forall \vec{u} : [\vec{v}]_x \cdot \vec{u} = \vec{v} \times \vec{u}
		 * \f]
		 */
		Matrix3<T>& getCrossMatrix(Matrix3<T>& m);

		/**
		 * Function returning a pointer to the vector's data.
		 */
		T* getArray(void);

		/********** Friends **********/

		/**
		 * Method for printing the vector tab delimited to a ostream.
		 */
		template <class U> friend std::ostream& operator<<(std::ostream&, const Vector3<T>&);

		/**
		 * Oparator to check for quality of two vectors.
		 */
		template <class U> friend inline bool operator ==(const Vector3<T>&, const Vector3<T>&);

		/**
		 * Operator to check for inequality of two vectors.
		 */
		template <class U> friend inline bool operator !=(const Vector3<T>&, const Vector3<T>&);

		/**
		 * Arithmetic addition of two vectors.
		 */
		template <class U> friend inline Vector3<T> operator +(const Vector3<T>&, const Vector3<T>&);

		/**
		 * Arithmetic subtraction of two vectors.
		 */
		template <class U> friend inline Vector3<T> operator -(const Vector3<T>&, const Vector3<T>&);

		/**
		 * Arithmetic division of a vector and a scalar.
		 */
		template <class U> friend inline Vector3<T> operator /(const Vector3<T>&, T);

		/**
		 * Arithmetic multiplication of a vector and a scalar.
		 */
		template <class U> friend inline Vector3<T> operator *(const Vector3<T>&, T);

		/**
		 * Arithmetic multiplication of a scalar and a vector.
		 */
		template <class U> friend inline Vector3<T> operator *(T, const Vector3<T>&);

		/**
		 * Dot product of two vectors.
		 */
		template <class U> friend inline T dot(const Vector3<T>&, const Vector3<T>&);		

		/**
		 * Cross product of two vectors.
		 */
		template <class U> friend inline Vector3<T> cross(const Vector3<T>&, const Vector3<T>&);

	private:
		/**
		 * Array, which stores the three components of this vector.
		 */
		T m_data[3];
	};

	/**
	 * \ingroup dtypes
	 * A vector class.
	 */
	template <class T>
	class Vector4
	{
	public:
		/**
		 * Standard constructor.
		 * Be aware of the fact the vectors values are undefined after calling this constructor.
		 */
		Vector4(void);

		/**
		 * Constructor to explicitly pass three scalar values.
		 */
		Vector4(T x, T y, T z, T w);

		/**
		 * Constructor to pass an array of scalar values.
		 */
		Vector4(T* initArray);

		/**
		 * Constructor to assign one scalar to the whole vector.
		 * \param scalar Initial scalar value.
		 */
		Vector4(const T scalar);

		/**
		 * Data accessing operator.
		 * @return Returns const scalar values.
		 */
		const T& operator[] (int i) const;

		/**
		 * Data accesing operator.
		 * @return Returns non const scalar values.
		 */
		T& operator[] (int i);

		/**
		 * Negation operator.
		 */
		inline Vector4<T> &operator -();

		/**
		 * Assignment operator, which maybe used to copy vectors. Note that there is no explicit copy constructor available.
		 */
		inline Vector4<T> &operator = (const Vector4<T>&);

		/**
		 * Assignment operator.
		 * @param scalar Takes a scalar value as parameter which will be assigned to all vector components.
		 */
		inline Vector4<T> &operator = (T scalar);

		/**
		 * Arithmetic addition.
		 */
		inline Vector4<T> &operator += (const Vector4<T>&);

		/**
		 * Arithmetic subtraction.
		 */
		inline Vector4<T> &operator -= (const Vector4<T>&);

		/**
		 * Arithmetic product with a scalar.
		 */
		inline Vector4<T> &operator *= (T);

		/**
		 * Arithmetic division by a scalar.
		 */
		inline Vector4<T> &operator /= (T s);

		/**
		 * Test for equality of this vector and rhs within the given tolerance.
		 */
		inline bool equals(const Vector4<T> &rhs, const T tolerance) const;

		/**
		 * Function returning the vector's square of the vectors norm.
		 */
		inline T normSqr(void);

		/**
		 * Function returning the vector's norm.
		 */
		inline T norm(void);

		/**
		 * Function to convert this vector to the unit vector.
		 */
		inline Vector4<T> &normalize(void);

		/**
		 * Function returning a pointer to the vector's data.
		 */
		T* getArray(void);

		/********** Friends **********/

		/**
		 * Method for printing the vector tab delimited to a ostream.
		 */
		template <class U> friend std::ostream& operator<<(std::ostream&, const Vector4<T>&);

		/**
		 * Oparator to check for quality of two vectors.
		 */
		template <class U> friend inline bool operator == (const Vector4<T>&, const Vector4<T>&);

		/**
		 * Operator to check for inequality of two vectors.
		 */
		template <class U> friend inline bool operator != (const Vector4<T>&, const Vector4<T>&);

		/**
		 * Arithmetic addition of two vectors.
		 */
		template <class U> friend inline Vector4<T> operator + (const Vector4<T>&, const Vector4<T>&);

		/**
		 * Arithmetic subtraction of two vectors.s
		 */
		template <class U> friend inline Vector4<T> operator - (const Vector4<T>&, const Vector4<T>&);

		/**
		 * Arithmetic division of a vector and a scalar.
		 */
		template <class U> friend inline Vector4<T> operator / (const Vector4<T>&, T);

		/**
		 * Arithmetic multiplication of a vector and a scalar.
		 */
		template <class U> friend inline Vector4<T> operator *(const Vector4<T>&, T);

		/**
		 * Arithmetic multiplication of a scalar and a vector.
		 */
		template <class U> friend inline Vector4<T> operator *(T, const Vector4<T>&);

		/**
		 * Dot product of two vectors.
		 */
		template <class U> friend inline T dot(const Vector4<T>&, const Vector4<T>&);		

	private:
		/**
		 * Array, which stores the four components of this vector.
		 */
		T m_data[4];
	};

	/**
	 * \ingroup dtypes
	 * A matrix class.
	 */
	template <class T>
	class Matrix3 {
	public:
		/**
		 * Standard constructor; be aware of the fact, that the matrices values are undefined after the call.
		 */
		Matrix3(void);

		/**
		 * Generate a matrix from euler angles (rotation order z-y-x)
		 */
		Matrix3(T rx, T ry, T rz);

		/**
		 * Constructor to pass one scalar value, which all nine entries of the matrix are initialized with.
		 */
		Matrix3(T initScalar);

		/**
		 * Constructor to pass an array of scalar values, which has to consits of exactly nine entries.
		 */
		Matrix3(T* initArray);

		/**
		 * Data accessing operator.
		 * @return Returns const scalar values.
		 */
		const T* operator[] (int i) const;

		/**
		 * Data accesssing operator.
		 * @return Returns non const scalar values.
		 */
		T* operator[] (int i);

		/**
		 * Negation operator.
		 */
		inline Matrix3<T> &operator- ();

		/**
		 * Assignement operator, which may be used to copy matrices. Note that there is no copy
		 * constructor available.
		 */
		inline Matrix3<T> &operator = (const Matrix3<T>&);

		/**
		 * Assignment operator.
		 * @param scalar Takes a scalar value as parameter, which will be assigned to all matrix components.
		 */
		inline Matrix3<T> &operator = (T scalar);

		/**
		 * Arithmetic matrix product.
		 * @see multRight()
		 */
		inline Matrix3<T> &operator *= (const Matrix3<T>&);

		/**
		 * Arithmetic matrix addition.
		 */
		inline Matrix3<T> &operator += (const Matrix3<T>&);

		/**
		 * Arithmetic matrix subtraction.
		 */
		inline Matrix3<T> &operator -= (const Matrix3<T>&);

		/**
		 * Arithmetic product of this matrix with a scalar.
		 */
		inline Matrix3<T> &operator *= (T);

		/**
		 * Arithmetic division of this matrix by a scalar.
		 */
		inline Matrix3<T> &operator /= (T);

		/**
		 * Arithmetic matrix product.
		 * @param m Reference to a matrix.
		 * @return Returns reference to this, where
		 * \f[
		 *      this
		 *		= \left(\begin{array}{ccc}
		 *			this_{00} & this_{01} & this_{02} \\
		 *			this_{10} & this_{11} & this_{12} \\
		 *			this_{20} & this_{21} & this_{22} \\
		 *        \end{array}\right)
		 *		  \cdot
		 *		  \left(\begin{array}{ccc}
		 *			m_{00} & m_{01} & m_{02} \\
		 *			m_{10} & m_{11} & m_{12} \\
		 *			m_{20} & m_{21} & m_{22} \\
		 *        \end{array}\right)
		 * \f]
		 */
		inline Matrix3<T> &multRight(const Matrix3<T> &m);

		/**
		 * Arithmetic matrix product.
		 * @param m Reference to a matrix.
		 * @return Returns reference to this, where
		 * \f[
		 *      this
		 *		= \left(\begin{array}{ccc}
		 *			m_{00} & m_{01} & m_{02} \\
		 *			m_{10} & m_{11} & m_{12} \\
		 *			m_{20} & m_{21} & m_{22} \\
		 *        \end{array}\right)
		 *		  \cdot
		 *		  \left(\begin{array}{ccc}
		 *			this_{00} & this_{01} & this_{02} \\
		 *			this_{10} & this_{11} & this_{12} \\
		 *			this_{20} & this_{21} & this_{22} \\
		 *        \end{array}\right)
		 * \f]
		 */
		inline Matrix3<T> &multLeft(const Matrix3<T> &m);

		/**
		 * Vector matrix product.
		 * @param *dst Pointer to a vector to store the result in.
		 * @param &rhs The right hand side vector of the matrix multiplication.		 
		 */
		inline void mult(Vector3<T> *dst, const Vector3<T> &rhs) const;

		/**
		 * Test for equality of this matrix and rhs within the given tolerance.
		 */
		inline bool equals(const Matrix3<T> &rhs, const T tolerance) const;

		/**
		 * Returns the value of the frobenius norm
		 */
		T frobeniusNorm(void) const;

		/**
		 * Returns the determinant of this matrix.
		 */
		T det(void) const;

		/**
		 * Returns the trace of this matrix.
		 */
		T trace(void) const;

		/**
		 * Set the row to the specified vector.
		 */
		void setRow(int, Vector3<T>&);

		/**
		 * Set the column to the specified vector.
		 */
		void setCol(int, Vector3<T>&);

		/**
		 * Retrieve the given row vector.
		 */
		Vector3<T> getRow(int) const;

		/**
		 * Retrieve the given column vector.
		 */
		Vector3<T> getCol(int) const;

		/**
		 * Inverts this matrix in-situ.
		 * @return Reference to this matrix.
		 */
		Matrix3<T> &invert(void);

		/**
		 * Returns the inverse of this matrix.		 
		 */
		Matrix3<T> getInverse(void);

		/**
		 * Tranposes this matrix in-situ.
		 * @return Reference to this matrix.
		 */
		Matrix3<T> &transpose(void);

		/**
		 * Returns the transposed of this matrix.
		 */
		Matrix3<T> getTransposed(void);

		/**
		 * Sets this matrix to the identity.
		 */
		void makeIdentity(void);

		/**
		 * Returns the quaternion of the matrix.
		 */
		Quaternion<T> getQuaternion(void);

		/**
		 * Set the components of this matrix such, that presents a rotation matrix
		 * around the given axis, by the given angle.
		 */
		void setRotation(T angle, Vector3<T> &axis);

		/**
		 * Sets this matrix to the scale matrix with the given value.
		 */
		void setScale(T scale_factor);

		/**
		 * Decompose this matrix such, that \f$this = \mbox{U}^{T}\cdot\mbox{U}\f$
		 * where \f$\mbox{U}^{T}\f$ is a lower triangular matrix and \f$\mbox{U}\f$
		 * is an upper triangular matrix.
		 * @param m Pointer of the matrix to store U in.
		 * @param tolerance If you really know that your matrix is symmetrix positive definite, you can specify a
		 *   <i>tolerance</i> (>0) to which diagonal elements will be set, if they get below it.
		 * @return <i>false</i> if this matrix is not symmetric positive definite, otherwise <i>true</i>
		 */
		bool getCholeskyFactor(Matrix3<T>*m, T tolerance = 0);

		/**
		 * Return the array containing the elements of this matrix.
		 */
		T* getArray(void);

		/********** Friends **********/

		/**
		 * Function to print a matrix to an ostream.
         */
		template <class U> friend std::ostream& operator<<(std::ostream&, const Matrix3<T>&);

		/**
		 * Writing a tab delimeted matrix
		 */
		template <class U> friend std::fstream& operator<<(std::fstream &matrix_file, Matrix3<T> &matrix);

		/**
		 * Method for reading matrices out of a tab delimite file.
		 */
		template <class U> friend std::fstream& operator>>(std::fstream&, Matrix3<T>&);		

		/**
		 * Tests for exact equality of two matrices.
		 * @see equals()
		 */
		template <class U> friend inline bool operator == (const Matrix3<T>&, const Matrix3<T>&);

		/**
		 * Tests two matrices for exact inequality.
		 */
		template <class U> friend inline bool operator != (const Matrix3<T>&, const Matrix3<T>&);

		/**
		 * Arithmetic addition of two matrices.
		 */
		template <class U> friend inline Matrix3<T> operator + (const Matrix3<T>&, const Matrix3<T>&);

		/**
		 * Artihmetic difference of two matrices.
		 */
		template <class U> friend inline Matrix3<T> operator - (const Matrix3<T>&, const Matrix3<T>&);

		/**
		 * Arithmetic matrix multiplication.
		 */
		template <class U> friend inline Matrix3<T> operator * (const Matrix3<T>&, const Matrix3<T>&);

		/**
		 * Arithmetic multiplication of matrix and vector.
		 */
		template <class U> friend inline Vector3<T> operator * (const Matrix3<T>&, const Vector3<T>&);

		/**
		 * Arithmetic division of a matrix and a scalar.
		 */
		template <class U> friend inline Matrix3<T> operator / (const Matrix3<T>&, T);

		/**
		 * Arithmetic multiplication of a matrix and a scalar.
		 */
		template <class U> friend inline Matrix3<T> operator * (const Matrix3<T>&, T);

		/**
		 * Arithmetic multiplication of a scalar and a matrix.
		 */
		template <class U> friend inline Matrix3<T> operator * (T, const Matrix3<T>&);

		/**
		 * Homogenous transformation; brings the result back to plane given as the parameter.
		 **/
		template <class U> friend inline Vector3<T> homTransform (const Matrix3<T>&, const Vector3<T>&, const T plane = 1.0);

	private:
		/**
		 * Array which stores the matrices values.
		 */
		T m_data[9];
	};

	/**
	 * \ingroup dtypes
	 * A matrix class.
	 */
	template <class T>
	class Matrix4 {
	public:
		/**
		 * Standard constructor; be aware of the fact, that the matrices values are undefined after the call.
		 */
		Matrix4(void);

		/**
		 * Generate a matrix from euler angles including a translation.
		 */
		Matrix4(T tx, T ty, T tz, T pitch, T yaw, T roll);

		/**
		 * Constructor to pass one scalar value, which all 16 entries of the matrix are initialized with.
		 */
		Matrix4(T initScalar);

		/**
		 * Constructor to pass an array of scalar values, which has to consits of exactly nine entries.
		 */
		Matrix4(T* initArray);

		/**
		 * Generate a matrix from a translation and rotation.
		 */
		Matrix4(const Vector3<T> &translation, const  Quaternion<T> &rotation);

		/**
		 * Data accessing operator.
		 * @return Returns const scalar values.
		 */
		const T* operator[] (int i) const;

		/**
		 * Data accesssing operator.
		 * @return Returns non const scalar values.
		 */
		T* operator[] (int i);

		/**
		 * Negation operator.
		 */
		inline Matrix4<T> &operator- ();

		/**
		 * Assignement operator, which may be used to copy matrices. Note that there is no copy
		 * constructor available.
		 */
		inline Matrix4<T> &operator = (const Matrix4<T>&);

		/**
		 * Assignment operator.
		 * @param scalar Takes a scalar value as parameter, which will be assigned to all matrix components.
		 */
		inline Matrix4<T> &operator = (T scalar);

		/**
		 * Arithmetic matrix product.
		 * @see multRight()
		 */
		inline Matrix4<T> &operator *= (const Matrix4<T>&);

		/**
		 * Arithmetic matrix addition.
		 */
		inline Matrix4<T> &operator += (const Matrix4<T>&);

		/**
		 * Arithmetic matrix subtraction.
		 */
		inline Matrix4<T> &operator -= (const Matrix4<T>&);

		/**
		 * Arithmetic product of this matrix with a scalar.
		 */
		inline Matrix4<T> &operator *= (T);

		/**
		 * Arithmetic division of this matrix by a scalar.
		 */
		inline Matrix4<T> &operator /= (T);

		/**
		 * Arithmetic matrix product.
		 * @param m Reference to a matrix.
		 * @return Returns reference to this, where
		 * \f[
		 *      this
		 *		= \left(\begin{array}{cccc}
		 *			this_{00} & this_{01} & this_{02} & this_{03} \\
		 *			this_{10} & this_{11} & this_{12} & this_{13}  \\
		 *			this_{20} & this_{21} & this_{22} & this_{23}  \\
		 *			this_{30} & this_{31} & this_{32} & this_{33}  \\
		 *        \end{array}\right)
		 *		  \cdot
		 *		  \left(\begin{array}{cccc}
		 *			m_{00} & m_{01} & m_{02} & m_{03} \\
		 *			m_{10} & m_{11} & m_{12} & m_{13}  \\
		 *			m_{20} & m_{21} & m_{22} & m_{23}  \\
		 *			m_{30} & m_{31} & m_{32} & m_{33}  \\
		 *        \end{array}\right)
		 * \f]
		 */
		inline Matrix4<T> &multRight(const Matrix4<T> &m);

		/**
		 * Arithmetic matrix product.
		 * @param m Reference to a matrix.
		 * @return Returns reference to this, where
		 * \f[
		 *      this
		 *		= \left(\begin{array}{cccc}
		 *			m_{00} & m_{01} & m_{02} & m_{03} \\
		 *			m_{10} & m_{11} & m_{12} & m_{13}  \\
		 *			m_{20} & m_{21} & m_{22} & m_{23}  \\
		 *			m_{30} & m_{31} & m_{32} & m_{33}  \\
		 *        \end{array}\right)
		 *		  \cdot
		 *		  \left(\begin{array}{cccc}
		 *			this_{00} & this_{01} & this_{02} & this_{03} \\
		 *			this_{10} & this_{11} & this_{12} & this_{13}  \\
		 *			this_{20} & this_{21} & this_{22} & this_{23}  \\
		 *			this_{30} & this_{31} & this_{32} & this_{33}  \\
		 *        \end{array}\right)
		 * \f]
		 */
		inline Matrix4<T> &multLeft(const Matrix4<T> &m);

		/**
		 * Vector matrix product.
		 * @param *dst Pointer to a vector to store the result in.
		 * @param &rhs The right hand side vector of the matrix multiplication.		 
		 */
		inline void mult(Vector4<T> *dst, const Vector4<T> &rhs) const;

		/**
		 * Test for equality of this matrix and rhs within the given tolerance.
		 */
		inline bool equals(const Matrix4<T> &rhs, const T tolerance) const;

		/**
		 * Returns the determinant of this matrix.
		 */
		T det(void) const;

		/**
		 * Returns the trace of this matrix.
		 */
		T trace(void) const;

		/**
		 * Set the row to the specified vector.
		 */
		void setRow(int, Vector4<T>&);

		/**
		 * Set the column to the specified vector.
		 */
		void setCol(int, Vector4<T>&);

		/**
		 * Retrieve the given row vector.
		 */
		Vector4<T> getRow(int) const;

		/**
		 * Retrieve the given column vector.
		 */
		Vector4<T> getCol(int) const;

		/**
		 * Inverts this matrix in-situ.
		 * @return Reference to this matrix.
		 */
		Matrix4<T> &invert(void);

		/**
		 * Returns the inverse of this matrix.		 
		 */
		Matrix4<T> getInverse(void);

		/**
		 * Tranposes this matrix in-situ.
		 * @return Reference to this matrix.
		 */
		Matrix4<T> &transpose(void);

		/**
		 * Returns the transposed of this matrix.
		 */
		Matrix4<T> getTransposed(void);

		/**
		 * Returns the quaternion of the upper left 3x3 part of the matrix.
		 */
		Quaternion<T> getQuaternion(void);

		/**
		 * Sets the given rotation.
		 */
		void setRotation(const Quaternion<T> &rotation);

		/**
		 * Sets the given translation. Beware that the lower right coordinate will be set to 1.
		 */
		void setTranslation(const Vector3<T> &translation);

		/**
		 * Sets this matrix to the identity.
		 */
		void makeIdentity(void);

		/**
		 * Return the array containing the elements of this matrix.
		 */
		T* getArray(void);

		/********** Friends **********/

		/**
		 * Function to print a matrix to an ostream.
         */
		template <class U> friend std::ostream& operator<<(std::ostream&, const Matrix4<T>&);

		/**
		 * Method for reading matrices out of a tab delimite file.
		 */
		template <class U> friend std::ifstream& operator>>(std::ifstream&, Matrix4<T>&);

		/**
		 * Tests for exact equality of two matrices.
		 * @see equals()
		 */
		template <class U> friend inline bool operator == (const Matrix4<T>&, const Matrix4<T>&);

		/**
		 * Tests two matrices for exact inequality.
		 */
		template <class U> friend inline bool operator != (const Matrix4<T>&, const Matrix4<T>&);

		/**
		 * Arithmetic addition of two matrices.
		 */
		template <class U> friend inline Matrix4<T> operator + (const Matrix4<T>&, const Matrix4<T>&);

		/**
		 * Artihmetic difference of two matrices.
		 */
		template <class U> friend inline Matrix4<T> operator - (const Matrix4<T>&, const Matrix4<T>&);

		/**
		 * Arithmetic matrix multiplication.
		 */
		template <class U> friend inline Matrix4<T> operator * (const Matrix4<T>&, const Matrix4<T>&);

		/**
		 * Arithmetic multiplication of matrix and vector.
		 */
		template <class U> friend inline Vector4<T> operator * (const Matrix4<T>&, const Vector4<T>&);

		/**
		 * Arithmetic division of a matrix and a scalar.
		 */
		template <class U> friend inline Matrix4<T> operator / (const Matrix4<T>&, T);

		/**
		 * Arithmetic multiplication of a matrix and a scalar.
		 */
		template <class U> friend inline Matrix4<T> operator * (const Matrix4<T>&, T);

		/**
		 * Arithmetic multiplication of a scalar and a matrix.
		 */
		template <class U> friend inline Matrix4<T> operator * (T, const Matrix4<T>&);

	private:
		/**
		 * Array which stores the matrices values.
		 */
		T m_data[16];
	};

	/**
	* \ingroup dtypes
	* A quaternion class.
	*/
	template <class T>
	class Quaternion {
	public:
		/**
		 * Standard constructor. Note that the quaternions values are undefined after
		 * calling the standard constructor.
		 */
		Quaternion(void);

		/**
		 * Constructor to explicitly set the quaternions values.
		 */
		Quaternion(T w, T x, T y, T z);

		/**
		 * init all values with initscalar
		 */
		Quaternion(T initScalar);

		/**
		 * Enter euler angles in radians to create the appropriate quaternion.
		 * If reverse == false, rotation is first around x, second around y, third around z.
		 * if reverse == true, rotation is first around z, second around y, third around x.
		 */
		Quaternion(T x, T y, T z, bool reverse=false);

		/**
		 * Assignment operator for quaternions.
		 */
		inline Quaternion<T>& operator = (const Quaternion<T>&);

		/**
		 * Arithmetic multiplication operator for two quaternions.
		 */
		inline Quaternion<T>& operator *= (const Quaternion<T>&);

		/**
		 * Arithmetic multiplication operator for quaternion and scalar.
		 */
		inline Quaternion<T>& operator *= (T);

		/**
		 * Arithmetic division operator for quaternion and scalar.
		 */
		inline Quaternion<T>& operator /= (T);

		/**
		 * Conjugates the quaternion in situ.
		 */
		inline Quaternion<T>& conjugate(void);

		/**
		 * Returns the conjugate quaternion.
		 */
		inline Quaternion<T> getConjugate(void);

		/**
		 * Returns the unit quaternion.
		 */
		inline Quaternion<T>& normalize(void);	

		/**
		 * Returns the norm of the quaternion.
		 */
		inline T norm(void);	

		/**
		 * Method to set the quaternion to such values that it represents a rotation around the
		 * given axis by the given angle.
		 */
		void setRotation(T angle, Vector3<T> &axis);

		/**
		 * Returns the matrix, that performs the same transformations as the quaternion.
		 */
		Matrix3<T> &getMatrix(Matrix3<T>&);

		/********** Friends **********/

		/**
		 * Method to print a quaternion to an ostream.
		 */
		template <class U> friend std::ostream& operator<<(std::ostream &s, const Quaternion<T>&);

		/**
		 * Method to test for exact equality of two quaternions.
		 */
		template <class U> friend inline bool operator== (const Quaternion<T>&, const Quaternion<T>&);

		/**
		 * Arithmetic multiplication of a quaternion and a scalar.
		 */
		template <class U> friend inline Quaternion<T> operator* (const Quaternion<T>&, T);

		/**
		 * Arithmetic multiplication of a scalar and a quaternion.
		 */
		template <class U> friend inline Quaternion<T> operator* (T, const Quaternion<T>&);

		/**
		 * Arithmetic division of a quaternion by a scalar.
		 */
		template <class U> friend inline Quaternion<T> operator/ (const Quaternion<T>&, T);

		/**
		 * Arithmetic multiplication of two quaternions.
		 */
		template <class U> friend inline Quaternion<T> operator* (const Quaternion<T>&, const Quaternion<T>&);	

	public:
		/**
		 * Values in which the quaternion is stored.
		 */
		T m_X,m_Y,m_Z,m_W;
	};

	/********** Implementations **********/
	#include "vector.inl"
	#include "matrix.inl"
	#include "quaternion.inl"

}

#endif

