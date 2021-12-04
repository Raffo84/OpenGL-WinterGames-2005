template <class T>
Vector3<T>::Vector3(void)
{
	memset(m_data, 0, 3*sizeof(T));
}

template <class T>
Vector3<T>::Vector3(T x, T y, T z)
{
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
}

template <class T>
Vector3<T>::Vector3(T initScalar)
{
	m_data[0] = initScalar;
	m_data[1] = initScalar;
	m_data[2] = initScalar;
}

template <class T>
Vector3<T>::Vector3(T* initArray)
{
	memcpy(m_data, initArray, 3*sizeof(T));
}

template <class T>
Vector3<T>::Vector3(const Vector3<T> &copy_me)
{
	m_data[0] = copy_me[0];
	m_data[1] = copy_me[1];
	m_data[2] = copy_me[2];
}

template <class T>
const T& Vector3<T>::operator[] (int i) const
{
	return m_data[i];
}

template <class T>
T& Vector3<T>::operator[] (int i)
{
	return m_data[i];
}

template <class T>
inline Vector3<T> &Vector3<T>::operator -()
{
	m_data[0] *= -1;
	m_data[1] *= -1;
	m_data[2] *= -1;
	return *this;
}

template <class T>
inline Vector3<T> &Vector3<T>::operator = (const Vector3<T>& rhs)
{
	m_data[0] = rhs[0];
	m_data[1] = rhs[1];
	m_data[2] = rhs[2];
	return *this;
}

template <class T>
inline Vector3<T> &Vector3<T>::operator = (T s)
{
	m_data[0] = s;
	m_data[1] = s;
	m_data[2] = s;
	return *this;
}

template <class T>
inline Vector3<T> &Vector3<T>::operator += (const Vector3<T>& rhs)
{
	m_data[0] += rhs[0];
	m_data[1] += rhs[1];
	m_data[2] += rhs[2];
	return *this;
}

template <class T>
inline Vector3<T> &Vector3<T>::operator -= (const Vector3<T>& rhs)
{
	m_data[0] -= rhs[0];
	m_data[1] -= rhs[1];
	m_data[2] -= rhs[2];
	return *this;
}

template <class T>
inline Vector3<T> &Vector3<T>::operator *= (T s)
{
	m_data[0] *= s;
	m_data[1] *= s;
	m_data[2] *= s;
	return *this;
}

template <class T>
inline Vector3<T> &Vector3<T>::operator /= (T s)
{
	m_data[0] /= s;
	m_data[1] /= s;
	m_data[2] /= s;
	return *this;
}

template <class T>
inline bool Vector3<T>::equals(const Vector3<T>& rhs, const T tolerance) const
{
	const Vector3<T> &r_this = *this;
	for (int x=0; x<3; x++)
		if (abs(r_this[x]-rhs[x]) > tolerance) return false;
	return true;
}

template <class T>
inline T Vector3<T>::normSqr(void)
{
	return (m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2]);
}

template <class T>
inline T Vector3<T>::norm(void)
{
	return std::sqrt(m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2]);
}

template <class T>
inline Vector3<T> &Vector3<T>::normalize(void)
{
	*this /= this->norm();
	return *this;
}

template <class T>
Matrix3<T>& Vector3<T>::getCrossMatrix(Matrix3<T>& m)
{
	m[0][0] = 0;
	m[0][1] = -1*(*this)[2];
	m[0][2] = (*this)[1];
	m[1][0] = (*this)[2];
	m[1][1] = 0;
	m[1][2] = static_cast<T>(-1.0*(*this)[0]);
	m[2][0] = -1*(*this)[1];
	m[2][1] = (*this)[0];
	m[2][2] = 0;
	return m;
}

template <class T>
T* Vector3<T>::getArray(void)
{
	return m_data;
}

/********** Friend Implementation **********/
template <class T>
std::ostream& operator<<(std::ostream &s, const Vector3<T> &rhs)
{
	s.flags( std::ios_base::scientific);
	s << rhs[0] << "\n" << rhs[1] << "\n" << rhs[2] << std::endl;
	return s;
}

template <class T>
inline bool operator == (const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return (lhs[0]==rhs[0] && lhs[1]==rhs[1] && lhs[2]==rhs[2]);
}

template <class T>
inline bool operator != (const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return (lhs[0]!=rhs[0] || lhs[1]!=rhs[1] || lhs[2]!=rhs[2]);
}

template <class T>
inline Vector3<T> operator + (const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	Vector3<T> sum = lhs;
	sum += rhs;
	return sum;
}

template <class T>
inline Vector3<T> operator - (const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	Vector3<T> dif = lhs;
	dif -= rhs;
	return dif;
}

template <class T>
inline Vector3<T> operator / (const Vector3<T>& lhs, T s)
{
	Vector3<T> result = lhs;
	result/=s;
	return result;
}

template <class T>
inline Vector3<T> operator * (const Vector3<T>& lhs, T s)
{
	Vector3<T> result = lhs;
	result*=s;
	return result;
}

template <class T>
inline Vector3<T> operator * (T s, const Vector3<T>& rhs)
{
	Vector3<T> result = rhs;
	result*=s;
	return result;
}

template <class T>
inline T dot(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return (lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2]);
}

template <class T>
inline Vector3<T> cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	Vector3<T> result = lhs;
	result[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
	result[1] = rhs[0] * lhs[2] - rhs[2] * lhs[0];
	result[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
	return result;
}


// -----------------------------------------------
// the Vector4
template <class T>
Vector4<T>::Vector4(void)
{
	std::memset(m_data, 0, 4*sizeof(T));
}

template <class T>
Vector4<T>::Vector4(const T scalar)
{
	m_data[0] = m_data[1] = m_data[2] = m_data[3] = scalar;
}

template <class T>
Vector4<T>::Vector4(T x, T y, T z, T w)
{
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
	m_data[3] = w;
}

template <class T>
Vector4<T>::Vector4(T* initArray)
{
	memcpy(m_data, initArray, 4*sizeof(T));
}

template <class T>
const T& Vector4<T>::operator[] (int i) const
{
	return m_data[i];
}

template <class T>
T& Vector4<T>::operator[] (int i)
{
	return m_data[i];
}

template <class T>
inline Vector4<T> &Vector4<T>::operator -()
{
	m_data[0] *= -1;
	m_data[1] *= -1;
	m_data[2] *= -1;
	m_data[3] *= -1;
}

template <class T>
inline Vector4<T> &Vector4<T>::operator = (const Vector4<T>& rhs)
{
	m_data[0] = rhs[0];
	m_data[1] = rhs[1];
	m_data[2] = rhs[2];
	m_data[3] = rhs[3];
	return *this;
}

template <class T>
inline Vector4<T> &Vector4<T>::operator = (T s)
{
	m_data[0] = s;
	m_data[1] = s;
	m_data[2] = s;
	m_data[3] = s;
	return *this;
}

template <class T>
inline Vector4<T> &Vector4<T>::operator += (const Vector4<T>& rhs)
{
	m_data[0] += rhs[0];
	m_data[1] += rhs[1];
	m_data[2] += rhs[2];
	m_data[3] += rhs[3];
	return *this;
}

template <class T>
inline Vector4<T> &Vector4<T>::operator -= (const Vector4<T>& rhs)
{
	m_data[0] -= rhs[0];
	m_data[1] -= rhs[1];
	m_data[2] -= rhs[2];
	m_data[3] -= rhs[3];
	return *this;
}

template <class T>
inline Vector4<T> &Vector4<T>::operator *= (T s)
{
	m_data[0] *= s;
	m_data[1] *= s;
	m_data[2] *= s;
	m_data[3] *= s;
	return *this;
}

template <class T>
inline Vector4<T> &Vector4<T>::operator /= (T s)
{
	m_data[0] /= s;
	m_data[1] /= s;
	m_data[2] /= s;
	m_data[3] /= s;
	return *this;
}

template <class T>
inline bool Vector4<T>::equals(const Vector4<T>& rhs, const T tolerance) const
{
	const Vector4<T> &r_this = *this;
	for (int x=0; x<4; x++)
		if (abs(r_this[x]-rhs[x]) > tolerance) return false;
	return true;
}

template <class T>
inline T Vector4<T>::normSqr(void)
{
	return (m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2] + m_data[3]*m_data[3]);
}

template <class T>
inline T Vector4<T>::norm(void)
{
	return SQRT_FUNCTION(m_data[0]*m_data[0] + m_data[1]*m_data[1] + m_data[2]*m_data[2] + m_data[3]*m_data[3]);
}

template <class T>
inline Vector4<T> &Vector4<T>::normalize(void)
{
	*this /= this->lenght();
	return *this;
}

template <class T>
T* Vector4<T>::getArray(void)
{
	return m_data;
}

/********** Friend Implementation **********/
template <class T>
std::ostream& operator<<(std::ostream &s, const Vector4<T> &rhs)
{
	s.flags( std::ios_base::scientific);
	s << rhs[0] << "\t" << rhs[1] << "\t" << rhs[2] << "\t" << rhs[3] << std::endl;
	return s;
}

template <class T>
inline bool operator == (const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return (lhs[0]==rhs[0] && lhs[1]==rhs[1] && lhs[2]==rhs[2] && lhs[3]==rhs[3]);
}

template <class T>
inline bool operator != (const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return (lhs[0]!=rhs[0] || lhs[1]!=rhs[1] || lhs[2]!=rhs[2] || lhs[3]!=rhs[3]);
}

template <class T>
inline Vector4<T> operator + (const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	Vector4<T> sum = lhs;
	sum += rhs;
	return sum;
}

template <class T>
inline Vector4<T> operator - (const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	Vector4<T> dif = lhs;
	dif -= rhs;
	return dif;
}

template <class T>
inline Vector4<T> operator / (const Vector4<T>& lhs, T s)
{
	Vector4<T> result = lhs;
	result/=s;
	return result;
}

template <class T>
inline Vector4<T> operator * (const Vector4<T>& lhs, T s)
{
	Vector4<T> result = lhs;
	result*=s;
	return result;
}

template <class T>
inline Vector4<T> operator * (T s, const Vector4<T>& rhs)
{
	Vector4<T> result = rhs;
	result*=s;
	return result;
}

template <class T>
inline T dot(const Vector4<T>& lhs, const Vector4<T>& rhs)
{
	return (lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2] + lhs[3]*rhs[3]);
}
