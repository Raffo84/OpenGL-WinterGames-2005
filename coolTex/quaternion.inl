template <class T>
Quaternion<T>::Quaternion(void)
{
}

template <class T>
Quaternion<T>::Quaternion(T w, T x, T y, T z)
{
	m_X = x;
	m_Y = y;
	m_Z = z;
	m_W = w;
}

template <class T>
Quaternion<T>::Quaternion(T initScalar)
{
	m_X = initScalar;
	m_Y = initScalar;
	m_Z = initScalar;
	m_W = initScalar;
}

template <class T>
Quaternion<T>::Quaternion(T x, T y, T z, bool reverse)
{
	static const T half = static_cast<T>(0.5);

	T ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
	
	if (reverse == true)
		y = -y;

	ti = x*half; tj = y*half; th = z*half;
	ci = cos(ti);  cj = cos(tj);  ch = cos(th);
	si = sin(ti);  sj = sin(tj);  sh = sin(th);
	cc = ci*ch; cs = ci*sh; sc = si*ch; ss = si*sh;

	m_X = cj*sc - sj*cs;
	m_Y = -cj*ss - sj*cc;
	m_Z = cj*cs - sj*sc;
	m_W = cj*cc + sj*ss;
}

template <class T>
inline Quaternion<T>& Quaternion<T>::operator = (const Quaternion<T>& rhs)
{
	m_X = rhs.m_X;
	m_Y = rhs.m_Y;
	m_Z = rhs.m_Z;
	m_W = rhs.m_W;
	return *this;
}

template <class T>
inline Quaternion<T>& Quaternion<T>::operator *= (const Quaternion<T>& rhs)
{
	Quaternion &r_this = *this;
	T w = r_this.m_W;
	T x = r_this.m_X;
	T y = r_this.m_Y;
	T z = r_this.m_Z;

	r_this.m_X = w*rhs.m_X + x*rhs.m_W + y*rhs.m_Z - z*rhs.m_Y;
	r_this.m_Y = w*rhs.m_Y - x*rhs.m_Z + y*rhs.m_W + z*rhs.m_X;
	r_this.m_Z = w*rhs.m_Z + x*rhs.m_Y - y*rhs.m_X + z*rhs.m_W;
	r_this.m_W = w*rhs.m_W - x*rhs.m_X - y*rhs.m_Y - z*rhs.m_Z;

	return r_this;
}

template <class T>
inline Quaternion<T>& Quaternion<T>::operator *= (T s)
{
	m_W *= s;
	m_X *= s;
	m_Y *= s;
	m_Z *= s;
	return *this;
}

template <class T>
inline Quaternion<T>& Quaternion<T>::operator /= (T s)
{
	m_W /= s;
	m_X /= s;
	m_Y /= s;
	m_Z /= s;
	return *this;
}

template <class T>
inline Quaternion<T>& Quaternion<T>::conjugate(void)
{
	m_X = -m_X;
	m_Y = -m_Y;
	m_Z = -m_Z;
	return *this;
}

template <class T>
inline Quaternion<T> Quaternion<T>::getConjugate(void)
{
	Quaternion<T> conj = *this;
	return conj.conjugate();
}

template <class T>
inline Quaternion<T>& Quaternion<T>::normalize(void)
{
	T norm_fac = static_cast<T>(1)/this->norm();
	*this *= norm_fac;
	return *this;
}

template <class T>
inline T Quaternion<T>::norm(void)
{
	return std::sqrt(m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W);
}

template <class T>
void Quaternion<T>::setRotation(T angle, Vector3<T> &axis)
{
	T norm_fac = static_cast<T>(1)/axis.norm();
	T sin_a = std::sin( angle * static_cast<T>(0.5) );
	T cos_a = std::cos( angle * static_cast<T>(0.5) );

	m_X    = (axis[0] * norm_fac) * sin_a;
	m_Y    = (axis[1] * norm_fac) * sin_a;
	m_Z    = (axis[2] * norm_fac) * sin_a;
	m_W    = cos_a;

	this->normalize();
}

template <class T>
Matrix3<T> &Quaternion<T>::getMatrix(Matrix3<T>& m)
{	
	this->normalize();

	T xx = m_X * m_X;
	T xy = m_X * m_Y;
	T xz = m_X * m_Z;
	T xw = m_X * m_W;

	T yy = m_Y * m_Y;
	T yz = m_Y * m_Z;
	T yw = m_Y * m_W;

	T zz = m_Z * m_Z;
	T zw = m_Z * m_W;

	m[0][0] = 1 - 2 * ( yy + zz );
	m[0][1] =     2 * ( xy + zw );
	m[0][2] =     2 * ( xz - yw );

	m[1][0] =	   2 * ( xy - zw );    
	m[1][1] = 1 - 2 * ( xx + zz );
	m[1][2] =     2 * ( yz + xw );

	m[2][0] =     2 * ( xz + yw );
	m[2][1] =     2 * ( yz - xw );
	m[2][2] = 1 - 2 * ( xx + yy );

	/*
	T ww = m_W * m_W;
	
	m[0][0] = ww + xx - yy - zz;
	m[0][1] = 2 * (xy - zw);
	m[0][2] = 2 * (xz + yw);

	m[1][0] = 2 * (xy + zw);
	m[1][1] = ww + yy - xx - zz;
	m[1][2] = 2 * (yz - xw);

	m[2][0] = 2 * (xz - yw);
	m[2][1] = 2 * (yz + xw);
	m[2][2] = ww + zz - xx - yy;
	*/

	return m;
}

/********** Friends **********/
template <class T>
std::ostream& operator<<(std::ostream &s, const Quaternion<T>& q)
{
	s.flags( std::ios_base::scientific);
	s << q.m_W << "\t" << q.m_X << "\t" << q.m_Y << "\t" << q.m_Z << std::endl;
	return s;
}

template <class T>
inline bool operator== (const Quaternion<T>& lhs, const Quaternion<T>& rhs)
{
	return (lhs.m_W == rhs.m_W  && lhs.m_X == rhs.m_X && lhs.m_Y == rhs.m_Y && lhs.m_Z == rhs.m_Z);
}

template <class T>
inline Quaternion<T> operator* (const Quaternion<T>& q, T s)
{
	Quaternion<T> p = q;
	p *= s;
	return p;
}

template <class T>
inline Quaternion<T> operator* (T s, const Quaternion<T>& q)
{
	Quaternion<T> p = q;
	p *= s;
	return p;
}

template <class T>
inline Quaternion<T> operator/ (const Quaternion<T>& q, T s)
{
	Quaternion<T> p = q;
	p /= s;
	return p;
}

template <class T>
inline Quaternion<T> operator* (const Quaternion<T>& q, const Quaternion<T>& r)
{
	Quaternion<T> p = q;
	p *= r;
	return p;	
}
