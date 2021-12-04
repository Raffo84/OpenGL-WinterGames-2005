template <class T>
Matrix3<T>::Matrix3(void)
{
	std::memset(m_data, 0, 9*sizeof(T));
}

template <class T>
Matrix3<T>::Matrix3(T rx, T ry, T rz)
{		
    T cx       = cos(rx);
    T sx       = sin(rx);
    T cy       = cos(ry);
    T sy       = sin(ry);
    T cz       = cos(rz);
    T sz       = sin(rz);

    m_data[0]  =   cz*cy;
    m_data[1]  =  -sz*cy;
    m_data[2]  =   sy;
    m_data[3]  =  cz*sy*sx+sz*cx;
    m_data[4]  = -sz*sy*sx+cz*cx;
    m_data[5]  =  -cy*sx;
    m_data[6]  = -cz*sy*cx+sz*sx;
    m_data[7]  =  sz*sy*cx+cz*sx;
    m_data[8] =   cy*cx;
}

template <class T>
Matrix3<T>::Matrix3(T initScalar)
{
	m_data[0] = initScalar;
	m_data[1] = initScalar;
	m_data[2] = initScalar;
	m_data[3] = initScalar;
	m_data[4] = initScalar;
	m_data[5] = initScalar;
	m_data[6] = initScalar;
	m_data[7] = initScalar;
	m_data[8] = initScalar;
}

template <class T>
Matrix3<T>::Matrix3(T* initArray)
{
	memcpy(m_data, initArray, 9*sizeof(T));
}

template <class T>
const T* Matrix3<T>::operator[] (int i) const
{
	return &m_data[i*3];
}

template <class T>
T* Matrix3<T>::operator[] (int i)
{
	return &m_data[i*3];
}

template <class T>
inline Matrix3<T> &Matrix3<T>::operator- ()
{
	for (int i=0; i<9; i++)
		m_data[i] *= -1;
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::operator = (const Matrix3<T>& rhs)
{
	memcpy(m_data, rhs[0], 9*sizeof(T));
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::operator = (T s)
{
	for (int i=0; i<9; i++)
		m_data[i] = s;
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::operator *= (const Matrix3<T>& rhs)
{
	Matrix3<T> tmp = *this;
	m_data[0] = tmp[0][0]*rhs[0][0]+tmp[0][1]*rhs[1][0]+tmp[0][2]*rhs[2][0];
	m_data[1] = tmp[0][0]*rhs[0][1]+tmp[0][1]*rhs[1][1]+tmp[0][2]*rhs[2][1];
	m_data[2] = tmp[0][0]*rhs[0][2]+tmp[0][1]*rhs[1][2]+tmp[0][2]*rhs[2][2];
	m_data[3] = tmp[1][0]*rhs[0][0]+tmp[1][1]*rhs[1][0]+tmp[1][2]*rhs[2][0];
	m_data[4] = tmp[1][0]*rhs[0][1]+tmp[1][1]*rhs[1][1]+tmp[1][2]*rhs[2][1];
	m_data[5] = tmp[1][0]*rhs[0][2]+tmp[1][1]*rhs[1][2]+tmp[1][2]*rhs[2][2];
	m_data[6] = tmp[2][0]*rhs[0][0]+tmp[2][1]*rhs[1][0]+tmp[2][2]*rhs[2][0];
	m_data[7] = tmp[2][0]*rhs[0][1]+tmp[2][1]*rhs[1][1]+tmp[2][2]*rhs[2][1];
	m_data[8] = tmp[2][0]*rhs[0][2]+tmp[2][1]*rhs[1][2]+tmp[2][2]*rhs[2][2];
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::operator += (const Matrix3<T>& rhs)
{
	m_data[0] += rhs[0][0];
	m_data[1] += rhs[0][1];
	m_data[2] += rhs[0][2];
	m_data[3] += rhs[1][0];
	m_data[4] += rhs[1][1];
	m_data[5] += rhs[1][2];
	m_data[6] += rhs[2][0];
	m_data[7] += rhs[2][1];
	m_data[8] += rhs[2][2];
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::operator -= (const Matrix3<T>& rhs)
{
	m_data[0] -= rhs[0][0];
	m_data[1] -= rhs[0][1];
	m_data[2] -= rhs[0][2];
	m_data[3] -= rhs[1][0];
	m_data[4] -= rhs[1][1];
	m_data[5] -= rhs[1][2];
	m_data[6] -= rhs[2][0];
	m_data[7] -= rhs[2][1];
	m_data[8] -= rhs[2][2];
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::operator *= (T s)
{
	m_data[0] *= s;
	m_data[1] *= s;
	m_data[2] *= s;
	m_data[3] *= s;
	m_data[4] *= s;
	m_data[5] *= s;
	m_data[6] *= s;
	m_data[7] *= s;
	m_data[8] *= s;
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::operator /= (T s)
{
	m_data[0] /= s;
	m_data[1] /= s;
	m_data[2] /= s;
	m_data[3] /= s;
	m_data[4] /= s;
	m_data[5] /= s;
	m_data[6] /= s;
	m_data[7] /= s;
	m_data[8] /= s;
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::multRight(const Matrix3<T>& rhs)
{
	Matrix3<T> tmp = *this;
	m_data[0] = tmp[0][0]*rhs[0][0]+tmp[0][1]*rhs[1][0]+tmp[0][2]*rhs[2][0];
	m_data[1] = tmp[0][0]*rhs[0][1]+tmp[0][1]*rhs[1][1]+tmp[0][2]*rhs[2][1];
	m_data[2] = tmp[0][0]*rhs[0][2]+tmp[0][1]*rhs[1][2]+tmp[0][2]*rhs[2][2];
	m_data[3] = tmp[1][0]*rhs[0][0]+tmp[1][1]*rhs[1][0]+tmp[1][2]*rhs[2][0];
	m_data[4] = tmp[1][0]*rhs[0][1]+tmp[1][1]*rhs[1][1]+tmp[1][2]*rhs[2][1];
	m_data[5] = tmp[1][0]*rhs[0][2]+tmp[1][1]*rhs[1][2]+tmp[1][2]*rhs[2][2];
	m_data[6] = tmp[2][0]*rhs[0][0]+tmp[2][1]*rhs[1][0]+tmp[2][2]*rhs[2][0];
	m_data[7] = tmp[2][0]*rhs[0][1]+tmp[2][1]*rhs[1][1]+tmp[2][2]*rhs[2][1];
	m_data[8] = tmp[2][0]*rhs[0][2]+tmp[2][1]*rhs[1][2]+tmp[2][2]*rhs[2][2];
	return *this;
}

template <class T>
inline Matrix3<T> &Matrix3<T>::multLeft(const Matrix3<T>& lhs)
{
	Matrix3<T> rhs = *this;
	m_data[0] = lhs[0][0]*rhs[0][0]+lhs[0][1]*rhs[1][0]+lhs[0][2]*rhs[2][0];
	m_data[1] = lhs[0][0]*rhs[0][1]+lhs[0][1]*rhs[1][1]+lhs[0][2]*rhs[2][1];
	m_data[2] = lhs[0][0]*rhs[0][2]+lhs[0][1]*rhs[1][2]+lhs[0][2]*rhs[2][2];
	m_data[3] = lhs[1][0]*rhs[0][0]+lhs[1][1]*rhs[1][0]+lhs[1][2]*rhs[2][0];
	m_data[4] = lhs[1][0]*rhs[0][1]+lhs[1][1]*rhs[1][1]+lhs[1][2]*rhs[2][1];
	m_data[5] = lhs[1][0]*rhs[0][2]+lhs[1][1]*rhs[1][2]+lhs[1][2]*rhs[2][2];
	m_data[6] = lhs[2][0]*rhs[0][0]+lhs[2][1]*rhs[1][0]+lhs[2][2]*rhs[2][0];
	m_data[7] = lhs[2][0]*rhs[0][1]+lhs[2][1]*rhs[1][1]+lhs[2][2]*rhs[2][1];
	m_data[8] = lhs[2][0]*rhs[0][2]+lhs[2][1]*rhs[1][2]+lhs[2][2]*rhs[2][2];
	return *this;
}

template <class T>
inline void Matrix3<T>::mult(Vector3<T> *dst, const Vector3<T>& rhs) const
{
	Vector3<T> &dest = *dst;
	const Matrix3<T> &r_this = *this;
	dest[0] = r_this[0][0]*rhs[0] + r_this[0][1]*rhs[1] + r_this[0][2]*rhs[2];
	dest[1] = r_this[1][0]*rhs[0] + r_this[1][1]*rhs[1] + r_this[1][2]*rhs[2];
	dest[2] = r_this[2][0]*rhs[0] + r_this[2][1]*rhs[1] + r_this[2][2]*rhs[2];
}

template <class T>
inline bool Matrix3<T>::equals(const Matrix3<T>& rhs, const T tolerance) const
{
	const Matrix3<T> &r_this = *this;
	for (int x=0; x<3; x++)
		for (int y=0; y<3; y++)
			if (abs(r_this[x][y]-rhs[x][y]) > tolerance) return false;
	return true;
}

template <class T>
T Matrix3<T>::frobeniusNorm(void) const
{
	const Matrix3<T> &m = *this;
	return std::sqrt(m[0][0]*m[0][0] + m[0][1]*m[0][1] + m[0][2]*m[0][2] +
					 m[1][0]*m[1][0] + m[1][1]*m[1][1] + m[1][2]*m[1][2] +
					 m[2][0]*m[2][0] + m[2][1]*m[2][1] + m[2][2]*m[2][2]);	
}

template <class T>
T Matrix3<T>::det(void) const
{
	const Matrix3<T> &r_this = *this;
	// More or less directly from "Advanced Engineering Mathematics"
	// (E. Kreyszig), 6th edition.

	double a11 = r_this[0][0];
	double a12 = r_this[0][1];
	double a13 = r_this[0][2];
	double a21 = r_this[1][0];
	double a22 = r_this[1][1];
	double a23 = r_this[1][2];
	double a31 = r_this[2][0];
	double a32 = r_this[2][1];
	double a33 = r_this[2][2];

	double M11 = a22 * a33 - a32 * a23;
	double M21 = -(a12 * a33 - a32 * a13);
	double M31 = a12 * a23 - a22 * a13;

	return static_cast<T>(a11 * M11 + a21 * M21 + a31 * M31);
}

template <class T>
T Matrix3<T>::trace(void) const
{
	const Matrix3<T> &r_this = *this;

	return (r_this[0][0] + r_this[1][1] + r_this[2][2]);
}

template <class T>
void Matrix3<T>::setRow(int row, Vector3<T>& row_vec)
{
	memcpy((*this)[row],row_vec.getArray(),3*sizeof(T));
}

template <class T>
void Matrix3<T>::setCol(int col, Vector3<T>& col_vec)
{
	Matrix3<T> &r_this = *this;
	r_this[0][col] = col_vec[0];
	r_this[1][col] = col_vec[0];
	r_this[2][col] = col_vec[0];
}

template <class T>
Vector3<T> Matrix3<T>::getRow(int row) const
{
	return Vector3<T>(const_cast<float*>(m_data+row*3));
}

template <class T>
Vector3<T> Matrix3<T>::getCol(int col) const
{
	const Matrix3<T> &r_this = *this;
	return Vector3<T>(r_this[0][col],r_this[1][col],r_this[2][col]);
}

template <class T>
Matrix3<T> &Matrix3<T>::invert(void)
{
	Matrix3<T> &r_this = *this;
	Matrix3<T> inverse;

	T opt1 = r_this[2][0] * r_this[0][1];
	T opt2 = r_this[2][0] * r_this[0][2];
	T opt3 = r_this[1][0] * r_this[0][1];
	T opt4 = r_this[1][0] * r_this[0][2];
	T opt5 = r_this[0][0] * r_this[1][1];
	T opt6 = r_this[0][0] * r_this[1][2];
	T opt7 = (opt1 * r_this[1][2] - opt2 * r_this[1][1] - opt3 * r_this[2][2] + opt4 * r_this[2][1] + opt5 * r_this[2][2] - opt6 * r_this[2][1]);
	T opt8 = static_cast<T>(0.1e1) / opt7;

	inverse[0][0] = (r_this[1][1] * r_this[2][2] - r_this[1][2] * r_this[2][1]) * opt8;
	inverse[0][1] = -(r_this[0][1] * r_this[2][2] - r_this[0][2] * r_this[2][1]) * opt8;
	inverse[0][2] = (r_this[0][1] * r_this[1][2] - r_this[0][2] * r_this[1][1]) * opt8;
	inverse[1][0] = -(-r_this[2][0] * r_this[1][2] + r_this[1][0] * r_this[2][2]) * opt8;
	inverse[1][1] = (-opt2 + r_this[0][0] * r_this[2][2]) * opt8;
	inverse[1][2] = -(-opt4 + opt6) * opt8;
	inverse[2][0] = (-r_this[2][0] * r_this[1][1] + r_this[1][0] * r_this[2][1]) * opt8;
	inverse[2][1] = -(-opt1 + r_this[0][0] * r_this[2][1]) * opt8;
	inverse[2][2] = (-opt3 + opt5) * opt8;

	*this = inverse;
	return *this;
}

template <class T>
Matrix3<T> Matrix3<T>::getInverse(void)
{
	Matrix3<T> inverse = *this;
	inverse.invert();
	return inverse;
}

template <class T>
Matrix3<T> &Matrix3<T>::transpose(void)
{
	Matrix3<T> &r_this = *this;
	T tmp;
	for (int x=0; x<3; x++) {
		for (int y=x; y<3; y++)
		{
			tmp = r_this[x][y];
			r_this[x][y] = r_this[y][x];
			r_this[y][x] = tmp;
		}
	}
	return r_this;
}

template <class T>
Matrix3<T> Matrix3<T>::getTransposed(void)
{
	Matrix3<T> transposed = *this;
	transposed.transpose();
	return transposed;
}

template <class T>
void Matrix3<T>::makeIdentity(void)
{
	*this = 0;
	m_data[0] = m_data[4] = m_data[8] = 1;
}

template <class T>
Quaternion<T> Matrix3<T>::getQuaternion(void)
{
	CAMP::Quaternion<T> quat = CAMP::Quaternion<T>();
	T q[4];
	T qoff[6];
	int c = 0;

	q[0] = (1 + m_data[0] + m_data[4] + m_data[8]) / 4;
	q[1] = (1 + m_data[0] - m_data[4] - m_data[8]) / 4;
	if (q[c] < q[1])
		c = 1;
	q[2] = (1 - m_data[0] + m_data[4] - m_data[8]) / 4;
	if (q[c] < q[2])
		c = 2;
	q[3] = (1 - m_data[0] - m_data[4] + m_data[8]) / 4;
	if (q[c] < q[3])
		c = 3;


	qoff[0] = (m_data[7] - m_data[5]) / 4;
	qoff[1] = (m_data[2] - m_data[6]) / 4;
	qoff[2] = (m_data[3] - m_data[1]) / 4;
	qoff[3] = (m_data[1] + m_data[3]) / 4;
	qoff[4] = (m_data[2] + m_data[6]) / 4;
	qoff[5] = (m_data[7] + m_data[5]) / 4;

	if (c==0)
	{
		quat.m_W = sqrt(q[c]);
		quat.m_X = qoff[0] / quat.m_W;
		quat.m_Y = qoff[1] / quat.m_W;
		quat.m_Z = qoff[2] / quat.m_W;
	} else if (c==1)
	{
		quat.m_X = sqrt(q[c]);
		quat.m_W = qoff[0] / quat.m_X;
		quat.m_Y = qoff[3] / quat.m_X;
		quat.m_Z = qoff[4] / quat.m_X;
	} else if (c==2)
	{
		quat.m_Y = sqrt(q[c]);
		quat.m_W = qoff[1] / quat.m_Y;
		quat.m_X = qoff[3] / quat.m_Y;
		quat.m_Z = qoff[5] / quat.m_Y;
	} else if (c==3)
	{
		quat.m_Z = sqrt(q[c]);
		quat.m_W = qoff[2] / quat.m_Z;
		quat.m_X = qoff[4] / quat.m_Z;
		quat.m_Y = qoff[5] / quat.m_Z;
	}

	return quat;
}

template <class T>
void Matrix3<T>::setRotation(T angle, Vector3<T>& axis)
{
	Vector3<T> axis_norm = axis;
	axis_norm.normalize();

	T c = std::cos(angle);
	T s = std::sin(angle);

	// One minus c (short name for legibility of formula)
	T omc = (static_cast<T>(1) - c);

	if (axis.normSqr() != 1) axis.normalize();

	T x = axis[0];
	T y = axis[1];
	T z = axis[2];
	T xs = x * s;
	T ys = y * s;
	T zs = z * s;
	T xyomc = x * y * omc;
	T xzomc = x * z * omc;
	T yzomc = y * z * omc;

	m_data[0] = x*x*omc + c;
	m_data[1] = xyomc - zs;
	m_data[2] = xzomc + ys;

	m_data[3] = xyomc + zs;
	m_data[4] = y*y*omc + c;
	m_data[5] = yzomc - xs;

	m_data[6] = xzomc - ys;	
	m_data[7] = yzomc + xs;
	m_data[8] = z*z*omc + c;
}

template <class T>
void Matrix3<T>::setScale(T scale_factor)
{
	this->makeIdentity();
	(*this)[0][0] = scale_factor;
	(*this)[1][1] = scale_factor;
}

template <class T>
bool Matrix3<T>::getCholeskyFactor(Matrix3<T>*m, T tolerance)
{
	if (tolerance < 0) return false;

	Matrix3<T>& a = *this;
	Matrix3<T>& lower = *m;

	// is the matrix positive definite??
	double opt0 = a[0][0];
	if (opt0 <= tolerance)
	{
		opt0 = tolerance;
		if (tolerance == 0) return false;
	}

	double opt1 = sqrt(opt0);
	double opt2 = 0.1e1 / opt1;
	double opt3 = pow(a[0][1], 0.2e1);
	double opt4 = a[1][1] * opt0 - opt3;
	double opt5 = 0.1e1 / opt0;

	// is the matrix positive definite??
	double opt6;
	if (opt4*opt5 <= tolerance)
	{
		opt6 = tolerance;
		if (tolerance == 0) return false;
	} else {
		opt6 = sqrt(opt4 * opt5);
	}
	double opt7 = pow(a[0][2], 0.2e1);
	double opt8 = pow(a[1][2], 0.2e1);

	// is the matrix positive definite??
	double opt9 = (a[2][2] * a[0][0] * a[1][1] - a[2][2] * opt3 - opt7 * a[1][1] - opt8 * a[0][0] + 0.2e1 * a[1][2] * a[0][2] * a[0][1]) / opt4;	
	if (opt9 <= tolerance) 
	{
		opt9 = tolerance;
		if (tolerance == 0) return false;
	}
	double opt10 = sqrt(opt9);	

	lower[0][0] = opt1;
	lower[0][1] = 0.0e0;
	lower[0][2] = 0.0e0;
	lower[1][0] = opt2 * a[0][1];
	lower[1][1] = opt6;
	lower[1][2] = 0.0e0;
	lower[2][0] = opt2 * a[0][2];
	lower[2][1] = (a[1][2] * a[0][0] - a[0][2] * a[0][1]) / opt6 * opt5;
	lower[2][2] = opt10;

	return true;
}

template <class T>
T* Matrix3<T>::getArray(void)
{
	return m_data;
}

/********** Friend Implementation **********/
template <class T>
std::ostream& operator<<(std::ostream &s, const Matrix3<T>& rhs)
{
	s.flags( std::ios_base::scientific);
	s << rhs[0][0] << "\t" << rhs[0][1] << "\t" << rhs[0][2] << "\t" << std::endl;
	s << rhs[1][0] << "\t" << rhs[1][1] << "\t" << rhs[1][2] << "\t" << std::endl;
	s << rhs[2][0] << "\t" << rhs[2][1] << "\t" << rhs[2][2] << "\t" << std::endl;
	return s;
}

template <class T>
std::fstream& operator<<(std::fstream &matrix_file, Matrix3<T> &matrix)
{
	matrix_file.flags( std::ios_base::scientific);
	matrix_file << matrix[0][0] << "\t" << matrix[0][1] << "\t" << matrix[0][2] << "\t" << std::endl;
	matrix_file << matrix[1][0] << "\t" << matrix[1][1] << "\t" << matrix[1][2] << "\t" << std::endl;
	matrix_file << matrix[2][0] << "\t" << matrix[2][1] << "\t" << matrix[2][2] << "\t" << std::endl;
	return matrix_file;
}

template <class T>
std::fstream& operator>>(std::fstream &matrix_file, Matrix3<T> &matrix)
{
	if (!matrix_file.good()) return matrix_file;
	
	std::string tmp;

	for (int x=0; x<3; x++) {
		for (int y=0; y<2; y++) {
			std::getline(matrix_file, tmp, '\t');
			matrix[x][y] = static_cast<T>(std::atof(tmp.c_str()));
		}
		std::getline(matrix_file, tmp, '\n');
		matrix[x][2] = static_cast<T>(std::atof(tmp.c_str()));
	}

	return matrix_file;
}

template <class T>
inline bool operator == (const Matrix3<T>& lhs, const Matrix3<T>& rhs)
{
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			if (lhs[i][j] != rhs[i][j]) return false;
	return true;
}

template <class T>
inline bool operator != (const Matrix3<T>& lhs, const Matrix3<T>& rhs)
{
	return !(lhs==rhs);
}

template <class T>
inline Matrix3<T> operator + (const Matrix3<T>& lhs, const Matrix3<T>& rhs)
{
	Matrix3<T> result = lhs;
	lhs += rhs;
	return result;
}

template <class T>
inline Matrix3<T> operator - (const Matrix3<T>& lhs, const Matrix3<T>& rhs)
{
	Matrix3<T> result = lhs;
	result -= rhs;
	return result;
}

template <class T>
inline Matrix3<T> operator * (const Matrix3<T>& lhs, const Matrix3<T>& rhs)
{
	Matrix3<T> result = lhs;
	result *= rhs;
	return result;
}

template <class T>
inline Vector3<T> operator * (const Matrix3<T>& lhs, const Vector3<T>& rhs)
{
	Vector3<T> result;	
	lhs.mult(&result, rhs);
	return result;
}

template <class T>
inline Matrix3<T> operator / (const Matrix3<T>& lhs, T s)
{
	Matrix3<T> result = lhs;	
	result/= s;
	return result;
}

template <class T>
inline Matrix3<T> operator * (const Matrix3<T>& lhs, T s)
{
	Matrix3<T> result = lhs;	
	result*= s;
	return result;
}

template <class T>
inline Matrix3<T> operator * (T s, const Matrix3<T>& rhs)
{
	Matrix3<T> result = rhs;	
	result*= s;
	return result;
}

template <class T>
inline Vector3<T> homTransform (const Matrix3<T>& lhs, const Vector3<T>& rhs, const T plane = 1.0)
{
	Vector3<T> result;
	lhs.mult(&result, rhs);
	result /= result[2];
	if (plane!=1.0) result *= plane;	
	return result;
}


template <class T>
Matrix4<T>::Matrix4(void)
{
	memset(m_data, 0, 16*sizeof(T));
}

template <class T>
Matrix4<T>::Matrix4(T tx, T ty, T tz, T rx, T ry, T rz)
{		
    T cx       = cos(rx);
    T sx       = sin(rx);
    T cy       = cos(ry);
    T sy       = sin(ry);
    T cz       = cos(rz);
    T sz       = sin(rz);
    //T AD      =   A * D;
    //T BD      =   B * D;

    m_data[0]  =   cz*cy;
    m_data[1]  =  -sz*cy;
    m_data[2]  =   sy;
    m_data[4]  =  cz*sy*sx+sz*cx;
    m_data[5]  = -sz*sy*sx+cz*cx;
    m_data[6]  =  -cy*sx;
    m_data[8]  = -cz*sy*cx+sz*sx;
    m_data[9]  =  sz*sy*cx+cz*sx;
    m_data[10] =   cy*cx;

	m_data[3] = tx;
	m_data[7] = ty;
	m_data[11] = tz;

	m_data[12] = m_data[13] = m_data[14] = static_cast<T>(0);
	m_data[15] = static_cast<T>(1);
}

template <class T>
Matrix4<T>::Matrix4(T initScalar)
{
	m_data[0] = initScalar;
	m_data[1] = initScalar;
	m_data[2] = initScalar;
	m_data[3] = initScalar;
	m_data[4] = initScalar;
	m_data[5] = initScalar;
	m_data[6] = initScalar;
	m_data[7] = initScalar;
	m_data[8] = initScalar;
	m_data[9] = initScalar;
	m_data[10] = initScalar;
	m_data[11] = initScalar;
	m_data[12] = initScalar;
	m_data[13] = initScalar;
	m_data[14] = initScalar;
	m_data[15] = initScalar;
}

template <class T>
Matrix4<T>::Matrix4(T* initArray)
{
	memcpy(m_data, initArray, 16*sizeof(T));
}

template <class T>
Matrix4<T>::Matrix4(const Vector3<T> &translation, const  Quaternion<T> &rotation)
{
	this->setTranslation(translation);
	this->setRotation(rotation);
}

template <class T>
const T* Matrix4<T>::operator[] (int i) const
{
	return &m_data[i*4];
}

template <class T>
T* Matrix4<T>::operator[] (int i)
{
	return &m_data[i*4];
}

template <class T>
inline Matrix4<T> &Matrix4<T>::operator- ()
{
	for (int i=0; i<16; i++)
		m_data[i] *= -1;
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::operator = (const Matrix4<T>& rhs)
{
	memcpy(m_data, rhs[0], 16*sizeof(T));
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::operator = (T s)
{
	for (int i=0; i<16; i++)
		m_data[i] = s;
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::operator *= (const Matrix4<T>& rhs)
{
	Matrix4<T> tmp = *this;
	m_data[0]  = tmp[0][0]*rhs[0][0]+tmp[0][1]*rhs[1][0]+tmp[0][2]*rhs[2][0]+tmp[0][3]*rhs[3][0];
	m_data[1]  = tmp[0][0]*rhs[0][1]+tmp[0][1]*rhs[1][1]+tmp[0][2]*rhs[2][1]+tmp[0][3]*rhs[3][1];
	m_data[2]  = tmp[0][0]*rhs[0][2]+tmp[0][1]*rhs[1][2]+tmp[0][2]*rhs[2][2]+tmp[0][3]*rhs[3][2];
	m_data[3]  = tmp[0][0]*rhs[0][3]+tmp[0][1]*rhs[1][3]+tmp[0][2]*rhs[2][3]+tmp[0][3]*rhs[3][3];
	m_data[4]  = tmp[1][0]*rhs[0][0]+tmp[1][1]*rhs[1][0]+tmp[1][2]*rhs[2][0]+tmp[1][3]*rhs[3][0];
	m_data[5]  = tmp[1][0]*rhs[0][1]+tmp[1][1]*rhs[1][1]+tmp[1][2]*rhs[2][1]+tmp[1][3]*rhs[3][1];
	m_data[6]  = tmp[1][0]*rhs[0][2]+tmp[1][1]*rhs[1][2]+tmp[1][2]*rhs[2][2]+tmp[1][3]*rhs[3][2];
	m_data[7]  = tmp[1][0]*rhs[0][3]+tmp[1][1]*rhs[1][3]+tmp[1][2]*rhs[2][3]+tmp[1][3]*rhs[3][3];
	m_data[8]  = tmp[2][0]*rhs[0][0]+tmp[2][1]*rhs[1][0]+tmp[2][2]*rhs[2][0]+tmp[2][3]*rhs[3][0];
	m_data[9]  = tmp[2][0]*rhs[0][1]+tmp[2][1]*rhs[1][1]+tmp[2][2]*rhs[2][1]+tmp[2][3]*rhs[3][1];
	m_data[10] = tmp[2][0]*rhs[0][2]+tmp[2][1]*rhs[1][2]+tmp[2][2]*rhs[2][2]+tmp[2][3]*rhs[3][2];
	m_data[11] = tmp[2][0]*rhs[0][3]+tmp[2][1]*rhs[1][3]+tmp[2][2]*rhs[2][3]+tmp[2][3]*rhs[3][3];
	m_data[12] = tmp[3][0]*rhs[0][0]+tmp[3][1]*rhs[1][0]+tmp[3][2]*rhs[2][0]+tmp[3][3]*rhs[3][0];
	m_data[13] = tmp[3][0]*rhs[0][1]+tmp[3][1]*rhs[1][1]+tmp[3][2]*rhs[2][1]+tmp[3][3]*rhs[3][1];
	m_data[14] = tmp[3][0]*rhs[0][2]+tmp[3][1]*rhs[1][2]+tmp[3][2]*rhs[2][2]+tmp[3][3]*rhs[3][2];
	m_data[15] = tmp[3][0]*rhs[0][3]+tmp[3][1]*rhs[1][3]+tmp[3][2]*rhs[2][3]+tmp[3][3]*rhs[3][3];
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::operator += (const Matrix4<T>& rhs)
{
	m_data[0]  += rhs[0][0];
	m_data[1]  += rhs[0][1];
	m_data[2]  += rhs[0][2];
	m_data[3]  += rhs[0][3];
	m_data[4]  += rhs[1][0];
	m_data[5]  += rhs[1][1];
	m_data[6]  += rhs[1][2];
	m_data[7]  += rhs[1][3];
	m_data[8]  += rhs[2][0];
	m_data[9]  += rhs[2][1];
	m_data[10] += rhs[2][2];
	m_data[11] += rhs[2][3];
	m_data[12] += rhs[3][0];
	m_data[13] += rhs[3][1];
	m_data[14] += rhs[3][2];
	m_data[15] += rhs[3][3];
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::operator -= (const Matrix4<T>& rhs)
{
	m_data[0]  -= rhs[0][0];
	m_data[1]  -= rhs[0][1];
	m_data[2]  -= rhs[0][2];
	m_data[3]  -= rhs[0][3];
	m_data[4]  -= rhs[1][0];
	m_data[5]  -= rhs[1][1];
	m_data[6]  -= rhs[1][2];
	m_data[7]  -= rhs[1][3];
	m_data[8]  -= rhs[2][0];
	m_data[9]  -= rhs[2][1];
	m_data[10] -= rhs[2][2];
	m_data[11] -= rhs[2][3];
	m_data[12] -= rhs[3][0];
	m_data[13] -= rhs[3][1];
	m_data[14] -= rhs[3][2];
	m_data[15] -= rhs[3][3];
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::operator *= (T s)
{
	m_data[0]  *= s;
	m_data[1]  *= s;
	m_data[2]  *= s;
	m_data[3]  *= s;
	m_data[4]  *= s;
	m_data[5]  *= s;
	m_data[6]  *= s;
	m_data[7]  *= s;
	m_data[8]  *= s;
	m_data[9]  *= s;
	m_data[10] *= s;
	m_data[11] *= s;
	m_data[12] *= s;
	m_data[13] *= s;
	m_data[14] *= s;
	m_data[15] *= s;
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::operator /= (T s)
{
	m_data[0]  /= s;
	m_data[1]  /= s;
	m_data[2]  /= s;
	m_data[3]  /= s;
	m_data[4]  /= s;
	m_data[5]  /= s;
	m_data[6]  /= s;
	m_data[7]  /= s;
	m_data[8]  /= s;
	m_data[9]  /= s;
	m_data[10] /= s;
	m_data[11] /= s;
	m_data[12] /= s;
	m_data[13] /= s;
	m_data[14] /= s;
	m_data[15] /= s;
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::multRight(const Matrix4<T>& rhs)
{
	Matrix4<T> tmp = *this;
	m_data[0]  = tmp[0][0]*rhs[0][0]+tmp[0][1]*rhs[1][0]+tmp[0][2]*rhs[2][0]+tmp[0][3]*rhs[3][0];
	m_data[1]  = tmp[0][0]*rhs[0][1]+tmp[0][1]*rhs[1][1]+tmp[0][2]*rhs[2][1]+tmp[0][3]*rhs[3][1];
	m_data[2]  = tmp[0][0]*rhs[0][2]+tmp[0][1]*rhs[1][2]+tmp[0][2]*rhs[2][2]+tmp[0][3]*rhs[3][2];
	m_data[3]  = tmp[0][0]*rhs[0][3]+tmp[0][1]*rhs[1][3]+tmp[0][2]*rhs[2][3]+tmp[0][3]*rhs[3][3];
	m_data[4]  = tmp[1][0]*rhs[0][0]+tmp[1][1]*rhs[1][0]+tmp[1][2]*rhs[2][0]+tmp[1][3]*rhs[3][0];
	m_data[5]  = tmp[1][0]*rhs[0][1]+tmp[1][1]*rhs[1][1]+tmp[1][2]*rhs[2][1]+tmp[1][3]*rhs[3][1];
	m_data[6]  = tmp[1][0]*rhs[0][2]+tmp[1][1]*rhs[1][2]+tmp[1][2]*rhs[2][2]+tmp[1][3]*rhs[3][2];
	m_data[7]  = tmp[1][0]*rhs[0][3]+tmp[1][1]*rhs[1][3]+tmp[1][2]*rhs[2][3]+tmp[1][3]*rhs[3][3];
	m_data[8]  = tmp[2][0]*rhs[0][0]+tmp[2][1]*rhs[1][0]+tmp[2][2]*rhs[2][0]+tmp[2][3]*rhs[3][0];
	m_data[9]  = tmp[2][0]*rhs[0][1]+tmp[2][1]*rhs[1][1]+tmp[2][2]*rhs[2][1]+tmp[2][3]*rhs[3][1];
	m_data[10] = tmp[2][0]*rhs[0][2]+tmp[2][1]*rhs[1][2]+tmp[2][2]*rhs[2][2]+tmp[2][3]*rhs[3][2];
	m_data[11] = tmp[2][0]*rhs[0][3]+tmp[2][1]*rhs[1][3]+tmp[2][2]*rhs[2][3]+tmp[2][3]*rhs[3][3];
	m_data[12] = tmp[3][0]*rhs[0][0]+tmp[3][1]*rhs[1][0]+tmp[3][2]*rhs[2][0]+tmp[3][3]*rhs[3][0];
	m_data[13] = tmp[3][0]*rhs[0][1]+tmp[3][1]*rhs[1][1]+tmp[3][2]*rhs[2][1]+tmp[3][3]*rhs[3][1];
	m_data[14] = tmp[3][0]*rhs[0][2]+tmp[3][1]*rhs[1][2]+tmp[3][2]*rhs[2][2]+tmp[3][3]*rhs[3][2];
	m_data[15] = tmp[3][0]*rhs[0][3]+tmp[3][1]*rhs[1][3]+tmp[3][2]*rhs[2][3]+tmp[3][3]*rhs[3][3];
	return *this;
}

template <class T>
inline Matrix4<T> &Matrix4<T>::multLeft(const Matrix4<T>& lhs)
{
	Matrix4<T> rhs = *this;
	m_data[0]  = lhs[0][0]*rhs[0][0]+lhs[0][1]*rhs[1][0]+lhs[0][2]*rhs[2][0]+lhs[0][3]*rhs[3][0];
	m_data[1]  = lhs[0][0]*rhs[0][1]+lhs[0][1]*rhs[1][1]+lhs[0][2]*rhs[2][1]+lhs[0][3]*rhs[3][1];
	m_data[2]  = lhs[0][0]*rhs[0][2]+lhs[0][1]*rhs[1][2]+lhs[0][2]*rhs[2][2]+lhs[0][3]*rhs[3][2];
	m_data[3]  = lhs[0][0]*rhs[0][3]+lhs[0][1]*rhs[1][3]+lhs[0][2]*rhs[2][3]+lhs[0][3]*rhs[3][3];
	m_data[4]  = lhs[1][0]*rhs[0][0]+lhs[1][1]*rhs[1][0]+lhs[1][2]*rhs[2][0]+lhs[1][3]*rhs[3][0];
	m_data[5]  = lhs[1][0]*rhs[0][1]+lhs[1][1]*rhs[1][1]+lhs[1][2]*rhs[2][1]+lhs[1][3]*rhs[3][1];
	m_data[6]  = lhs[1][0]*rhs[0][2]+lhs[1][1]*rhs[1][2]+lhs[1][2]*rhs[2][2]+lhs[1][3]*rhs[3][2];
	m_data[7]  = lhs[1][0]*rhs[0][3]+lhs[1][1]*rhs[1][3]+lhs[1][2]*rhs[2][3]+lhs[1][3]*rhs[3][3];
	m_data[8]  = lhs[2][0]*rhs[0][0]+lhs[2][1]*rhs[1][0]+lhs[2][2]*rhs[2][0]+lhs[2][3]*rhs[3][0];
	m_data[9]  = lhs[2][0]*rhs[0][1]+lhs[2][1]*rhs[1][1]+lhs[2][2]*rhs[2][1]+lhs[2][3]*rhs[3][1];
	m_data[10] = lhs[2][0]*rhs[0][2]+lhs[2][1]*rhs[1][2]+lhs[2][2]*rhs[2][2]+lhs[2][3]*rhs[3][2];
	m_data[11] = lhs[2][0]*rhs[0][3]+lhs[2][1]*rhs[1][3]+lhs[2][2]*rhs[2][3]+lhs[2][3]*rhs[3][3];
	m_data[12] = lhs[3][0]*rhs[0][0]+lhs[3][1]*rhs[1][0]+lhs[3][2]*rhs[2][0]+lhs[3][3]*rhs[3][0];
	m_data[13] = lhs[3][0]*rhs[0][1]+lhs[3][1]*rhs[1][1]+lhs[3][2]*rhs[2][1]+lhs[3][3]*rhs[3][1];
	m_data[14] = lhs[3][0]*rhs[0][2]+lhs[3][1]*rhs[1][2]+lhs[3][2]*rhs[2][2]+lhs[3][3]*rhs[3][2];
	m_data[15] = lhs[3][0]*rhs[0][3]+lhs[3][1]*rhs[1][3]+lhs[3][2]*rhs[2][3]+lhs[3][3]*rhs[3][3];
	return *this;
}

template <class T>
inline void Matrix4<T>::mult(Vector4<T> *dst, const Vector4<T>& rhs) const
{
	Vector4<T> &dest = *dst;
	const Matrix4<T> &r_this = *this;
	dest[0] = r_this[0][0]*rhs[0] + r_this[0][1]*rhs[1] + r_this[0][2]*rhs[2] + r_this[0][3]*rhs[3];
	dest[1] = r_this[1][0]*rhs[0] + r_this[1][1]*rhs[1] + r_this[1][2]*rhs[2] + r_this[1][3]*rhs[3];
	dest[2] = r_this[2][0]*rhs[0] + r_this[2][1]*rhs[1] + r_this[2][2]*rhs[2] + r_this[2][3]*rhs[3];
	dest[3] = r_this[3][0]*rhs[0] + r_this[3][1]*rhs[1] + r_this[3][2]*rhs[2] + r_this[3][3]*rhs[3];
}

template <class T>
inline bool Matrix4<T>::equals(const Matrix4<T>& rhs, const T tolerance) const
{
	const Matrix4<T> &r_this = *this;
	for (int x=0; x<4; x++)
		for (int y=0; y<4; y++)
			if (abs(r_this[x][y]-rhs[x][y]) > tolerance) return false;
	return true;
}

template <class T>
T Matrix4<T>::det(void) const
{
	const Matrix4<T> &r_this = *this;
	// Directly from optimized maple output.

	double tmp1 = r_this[0][0] * r_this[1][1];
	double tmp2 = r_this[2][2] * r_this[3][3];
	double tmp4 = r_this[2][3] * r_this[3][2];
	double tmp6 = r_this[0][0] * r_this[2][1];
	double tmp7 = r_this[1][2] * r_this[3][3];
	double tmp9 = r_this[1][3] * r_this[3][2];
	double tmp11 = r_this[0][0] * r_this[3][1];
	double tmp12 = r_this[1][2] * r_this[2][3];
	double tmp14 = r_this[1][3] * r_this[2][2];
	double tmp16 = r_this[1][0] * r_this[0][1];
	double tmp19 = r_this[1][0] * r_this[2][1];
	double tmp20 = r_this[0][2] * r_this[3][3];
	double tmp22 = r_this[0][3] * r_this[3][2];
	double tmp24 = r_this[1][0] * r_this[3][1];
	double tmp25 = r_this[0][2] * r_this[2][3];
	double tmp27 = r_this[0][3] * r_this[2][2];
	double tmp29 = tmp1 * tmp2 - tmp1 * tmp4 - tmp6 * tmp7 + tmp6 * tmp9 + tmp11 * tmp12 - tmp11 * tmp14 - tmp16 *
		tmp2 + tmp16 * tmp4 + tmp19 * tmp20 - tmp19 * tmp22 - tmp24 * tmp25 + tmp24 * tmp27;
	double tmp30 = r_this[2][0] * r_this[0][1];
	double tmp33 = r_this[2][0] * r_this[1][1];
	double tmp36 = r_this[2][0] * r_this[3][1];
	double tmp37 = r_this[0][2] * r_this[1][3];
	double tmp39 = r_this[0][3] * r_this[1][2];
	double tmp41 = r_this[3][0] * r_this[0][1];
	double tmp44 = r_this[3][0] * r_this[1][1];
	double tmp47 = r_this[3][0] * r_this[2][1];
	double tmp50 = tmp30 * tmp7 - tmp30 * tmp9 - tmp33 * tmp20 + tmp33 * tmp22 + tmp36 * tmp37 - tmp36 * tmp39 -
		tmp41 * tmp12 + tmp41 * tmp14 + tmp44 * tmp25 - tmp44 * tmp27 - tmp47 * tmp37 + tmp47 * tmp39;
	return (tmp29 + tmp50);
}

template <class T>
T Matrix4<T>::trace(void) const
{
	const Matrix4<T> &r_this = *this;

	return (r_this[0][0] + r_this[1][1] + r_this[2][2] + r_this[3][3]);
}

template <class T>
void Matrix4<T>::setRow(int row, Vector4<T>& row_vec)
{
	memcpy((*this)[row],row_vec.getArray(),4*sizeof(T));
}

template <class T>
void Matrix4<T>::setCol(int col, Vector4<T>& col_vec)
{
	Matrix4<T> &r_this = *this;
	r_this[0][col] = col_vec[0];
	r_this[1][col] = col_vec[0];
	r_this[2][col] = col_vec[0];
	r_this[3][col] = col_vec[0];
}

template <class T>
Vector4<T> Matrix4<T>::getRow(int row) const
{
	return Vector4<T>(const_cast<float*>(m_data+row*4));
}

template <class T>
Vector4<T> Matrix4<T>::getCol(int col) const
{
	const Matrix4<T> &r_this = *this;
	return Vector4<T>(r_this[0][col],r_this[1][col],r_this[2][col],r_this[3][col]);
}

template <class T>
Matrix4<T> &Matrix4<T>::invert(void)
{
	Matrix4<T> &r_this = *this;
	Matrix4<T> result;

	// check for affine matrix (common case)
	if (r_this[0][3] == 0.0 && r_this[1][3] == 0.0 &&
		r_this[2][3] == 0.0 && r_this[3][3] == 1.0)
	{
		// check for identity matrix (we already checked last column)
		if (r_this[0][0] == 1.0 && 
			r_this[0][1] == 0.0 &&
			r_this[0][2] == 0.0 &&
			r_this[1][0] == 0.0 &&
			r_this[1][1] == 1.0 &&
			r_this[1][2] == 0.0 &&
			r_this[2][0] == 0.0 &&
			r_this[2][1] == 0.0 &&
			r_this[2][2] == 1.0)
		{
			return *this;
		}

		/* More or less directly from:
		 * Kevin Wu, "Fast Matrix Inversion",  Graphics Gems II
		 *
		 * Computes the inverse of a 3D affine matrix; i.e. a matrix with a dimen-
		 * sionality of 4 where the right column has the entries (0, 0, 0, 1).
		 *
		 * This procedure treats the 4 by 4 matrix as a block matrix and
		 * calculates the inverse of one submatrix for a significant perform-
		 * ance improvement over a general procedure that can invert any non-
		 * singular matrix:
		 *          --        --          --          --
		 *          |          | -1       |    -1      |
		 *          | A      0 |          |   A      0 |
		 *    -1    |          |          |            |
		 *   M   =  |          |     =    |     -1     |
		 *          | C      1 |          | -C A     1 |
		 *          |          |          |            |
		 *          --        --          --          --
		 *
		 *  where     M is a 4 by 4 matrix,
		 *            A is the 3 by 3 upper left submatrix of M,
		 *            C is the 1 by 3 lower left submatrix of M.
		 */
		
		double det_1;
		double pos, neg, temp;

		#define ACCUMULATE     \
		if (temp >= 0.0)  \
			pos += temp;     \
		else               \
			neg += temp

		/*
		 * Calculate the determinant of submatrix A and determine if the
		 * the matrix is singular as limited by doubleing-point data 
		 * representation.
		 */
		pos = neg = 0.0;
		temp =  r_this[0][0] * r_this[1][1] * r_this[2][2];
		ACCUMULATE;
		temp =  r_this[0][1] * r_this[1][2] * r_this[2][0];
		ACCUMULATE;
		temp =  r_this[0][2] * r_this[1][0] * r_this[2][1];
		ACCUMULATE;
		temp = -r_this[0][2] * r_this[1][1] * r_this[2][0];
		ACCUMULATE;
		temp = -r_this[0][1] * r_this[1][0] * r_this[2][2];
		ACCUMULATE;
		temp = -r_this[0][0] * r_this[1][2] * r_this[2][1];
		ACCUMULATE;
		det_1 = pos + neg;

		#undef ACCUMULATE

		/* Is the submatrix A singular? */
		if ((det_1 == 0.0) || (abs(det_1 / (pos - neg)) < 1.0e-15))
		{
			/* Matrix M has no inverse */
			return *this;
		}
		else
		{
			/* Calculate inverse(A) = adj(A) / det(A) */
			det_1 = 1.0 / det_1;
			result[0][0] = (T) ((r_this[1][1] * r_this[2][2] -
							r_this[1][2] * r_this[2][1]) * det_1);
			result[1][0] = (T) (- (r_this[1][0] * r_this[2][2] -
							r_this[1][2] * r_this[2][0]) * det_1);
			result[2][0] = (T) ((r_this[1][0] * r_this[2][1] -
							r_this[1][1] * r_this[2][0]) * det_1);
			result[0][1] = (T) (- (r_this[0][1] * r_this[2][2] -
							r_this[0][2] * r_this[2][1]) * det_1);
			result[1][1] = (T) ((r_this[0][0] * r_this[2][2] -
							r_this[0][2] * r_this[2][0]) * det_1);
			result[2][1] = (T) (- (r_this[0][0] * r_this[2][1] -
							r_this[0][1] * r_this[2][0]) * det_1);
			result[0][2] = (T) ((r_this[0][1] * r_this[1][2] -
							r_this[0][2] * r_this[1][1]) * det_1);
			result[1][2] = (T) (- (r_this[0][0] * r_this[1][2] -
							r_this[0][2] * r_this[1][0]) * det_1);
			result[2][2] = (T) ((r_this[0][0] * r_this[1][1] -
							r_this[0][1] * r_this[1][0]) * det_1);

			/* Calculate -C * inverse(A) */
			result[3][0] = - (r_this[3][0] * result[0][0] +
							r_this[3][1] * result[1][0] +
							r_this[3][2] * result[2][0]);
			result[3][1] = - (r_this[3][0] * result[0][1] +
							r_this[3][1] * result[1][1] +
							r_this[3][2] * result[2][1]);
			result[3][2] = - (r_this[3][0] * result[0][2] +
							r_this[3][1] * result[1][2] +
							r_this[3][2] * result[2][2]);

			/* Fill in last column */
			result[0][3] = result[1][3] = result[2][3] = 0.0;
			result[3][3] = 1.0;
		}
	}
	else
	{ // non-affine matrix
		double max, sum, tmp, inv_pivot;
		int p[4];
		int i, j, k;

		// algorithm from: Schwarz, "Numerische Mathematik"
		result = *this;

		for (k = 0; k < 4; k++)
		{ 
			max = 0.0;
			p[k] = 0;
	     
			for (i = k; i < 4; i++)
			{ 
				sum = 0.0;
				for (j = k; j < 4; j++)
					sum += abs(result[i][j]);
				if (sum > 0.0)
				{
                    tmp = abs(result[i][k]) / sum;
					if (tmp > max)
					{
                        max = tmp;  
                        p[k] = i;
					}
				}
			}
	     
			if (max == 0.0)
			{
				return *this;
			}
	     
			if (p[k] != k)
			{
				for (j = 0; j < 4; j++)
				{
                    tmp = result[k][j];  
					result[k][j] = result[p[k]][j];  
					result[p[k]][j] = (T) tmp;
				}
			}
	     
			inv_pivot = 1.0 / result[k][k];
			for (j = 0; j < 4; j++)
			{
				if (j != k)
				{
					result[k][j] = (T) (- result[k][j] * inv_pivot);
					for (i = 0; i < 4; i++)
					{
						if (i != k)
							result[i][j] += result[i][k] * result[k][j];
					}
				}
			}
	     
			for (i = 0; i < 4; i++)
				result[i][k] *= (T) inv_pivot;
			result[k][k] = (T) inv_pivot;
		}
	   
		for (k = 2; k >= 0; k--)
		{
			if (p[k] != k)
			{
				for (i = 0; i < 4; i++)
				{
                    tmp = result[i][k];
					result[i][k] = result[i][p[k]]; 
					result[i][p[k]] = (T) tmp;
				}
			}
		}
	}
	*this = result;
	return *this;
}

template <class T>
Matrix4<T> Matrix4<T>::getInverse(void)
{
	Matrix4<T> inverse = *this;
	inverse.invert();
	return inverse;
}

template <class T>
Matrix4<T> &Matrix4<T>::transpose(void)
{
	Matrix4<T> &r_this = *this;
	T tmp;
	for (int x=0; x<4; x++) {
		for (int y=x; y<4; y++)
		{
			tmp = r_this[x][y];
			r_this[x][y] = r_this[y][x];
			r_this[y][x] = tmp;
		}
	}
	return r_this;
}

template <class T>
Matrix4<T> Matrix4<T>::getTransposed(void)
{
	Matrix4<T> transposed = *this;
	transposed.transpose();
	return transposed;
}

template <class T>
Quaternion<T> Matrix4<T>::getQuaternion(void)
{
	CAMP::Quaternion<T> quat = CAMP::Quaternion<T>();
	T q[4];
	T qoff[6];
	int c = 0;

	q[0] = (1 + m_data[0] + m_data[5] + m_data[10]) / 4;
	q[1] = (1 + m_data[0] - m_data[5] - m_data[10]) / 4;
	if (q[c] < q[1])
		c = 1;
	q[2] = (1 - m_data[0] + m_data[5] - m_data[10]) / 4;
	if (q[c] < q[2])
		c = 2;
	q[3] = (1 - m_data[0] - m_data[5] + m_data[10]) / 4;
	if (q[c] < q[3])
		c = 3;


	qoff[0] = (m_data[9] - m_data[6]) / 4;
	qoff[1] = (m_data[2] - m_data[8]) / 4;
	qoff[2] = (m_data[4] - m_data[1]) / 4;
	qoff[3] = (m_data[1] + m_data[4]) / 4;
	qoff[4] = (m_data[2] + m_data[8]) / 4;
	qoff[5] = (m_data[9] + m_data[6]) / 4;

	if (c==0)
	{
		quat.m_W = sqrt(q[c]);
		quat.m_X = qoff[0] / quat.m_W;
		quat.m_Y = qoff[1] / quat.m_W;
		quat.m_Z = qoff[2] / quat.m_W;
	} else if (c==1)
	{
		quat.m_X = sqrt(q[c]);
		quat.m_W = qoff[0] / quat.m_X;
		quat.m_Y = qoff[3] / quat.m_X;
		quat.m_Z = qoff[4] / quat.m_X;
	} else if (c==2)
	{
		quat.m_Y = sqrt(q[c]);
		quat.m_W = qoff[1] / quat.m_Y;
		quat.m_X = qoff[3] / quat.m_Y;
		quat.m_Z = qoff[5] / quat.m_Y;
	} else if (c==3)
	{
		quat.m_Z = sqrt(q[c]);
		quat.m_W = qoff[2] / quat.m_Z;
		quat.m_X = qoff[4] / quat.m_Z;
		quat.m_Y = qoff[5] / quat.m_Z;
	}

	return quat;
}

template <class T>
void Matrix4<T>::setRotation(const Quaternion<T> &rotation)
{
	T xx = rotation.m_X * rotation.m_X;
	T xy = rotation.m_X * rotation.m_Y;
	T xz = rotation.m_X * rotation.m_Z;
	T xw = rotation.m_X * rotation.m_W;

	T yy = rotation.m_Y * rotation.m_Y;
	T yz = rotation.m_Y * rotation.m_Z;
	T yw = rotation.m_Y * rotation.m_W;

	T zz = rotation.m_Z * rotation.m_Z;
	T zw = rotation.m_Z * rotation.m_W;

	m_data[0] = 1 - 2 * ( yy + zz );
	m_data[1] =     2 * ( xy + zw );
	m_data[2] =     2 * ( xz - yw );
	m_data[4] =	   2 * ( xy - zw );    
	m_data[5] = 1 - 2 * ( xx + zz );
	m_data[6] =     2 * ( yz + xw );
	m_data[8] =     2 * ( xz + yw );
	m_data[9] =     2 * ( yz - xw );
	m_data[10] = 1 - 2 * ( xx + yy );
	m_data[15] = 1;
}

template <class T>
void Matrix4<T>::setTranslation(const Vector3<T> &translation)
{
	m_data[3] = translation[0];
	m_data[7] = translation[1];
	m_data[11] = translation[2];
	m_data[15] = 1;
}

template <class T>
void Matrix4<T>::makeIdentity(void)
{
	*this = 0;
	m_data[0] = m_data[5] = m_data[10] = m_data[15] = 1;
}

template <class T>
T* Matrix4<T>::getArray(void)
{
	return m_data;
}

/********** Friend Implementation **********/
template <class T>
std::ostream& operator<<(std::ostream &s, const Matrix4<T>& rhs)
{
	s.flags( std::ios_base::scientific);
	s << rhs[0][0] << "\t" << rhs[0][1] << "\t" << rhs[0][2] << "\t" << rhs[0][3] << "\t" << std::endl;
	s << rhs[1][0] << "\t" << rhs[1][1] << "\t" << rhs[1][2] << "\t" << rhs[1][3] << "\t" << std::endl;
	s << rhs[2][0] << "\t" << rhs[2][1] << "\t" << rhs[2][2] << "\t" << rhs[2][3] << "\t" << std::endl;
	s << rhs[3][0] << "\t" << rhs[3][1] << "\t" << rhs[3][2] << "\t" << rhs[3][3] << "\t" << std::endl;
	return s;
}

template <class T>
std::ifstream& operator>>(std::ifstream &matrix_file, Matrix4<T> &matrix)
{
	if (!matrix_file.good()) return matrix_file;
	
	std::string tmp;

	for (int x=0; x<4; x++) {
		for (int y=0; y<3; y++) {
			std::getline(matrix_file, tmp, '\t');
			matrix[x][y] = static_cast<T>(std::atof(tmp.c_str()));
		}
		std::getline(matrix_file, tmp, '\n');
		matrix[x][3] = static_cast<T>(std::atof(tmp.c_str()));
	}

	return matrix_file;
}

template <class T>
inline bool operator == (const Matrix4<T>& lhs, const Matrix4<T>& rhs)
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			if (lhs[i][j] != rhs[i][j]) return false;
	return true;
}

template <class T>
inline bool operator != (const Matrix4<T>& lhs, const Matrix4<T>& rhs)
{
	return !(lhs==rhs);
}

template <class T>
inline Matrix4<T> operator + (const Matrix4<T>& lhs, const Matrix4<T>& rhs)
{
	Matrix4<T> result = lhs;
	lhs += rhs;
	return result;
}

template <class T>
inline Matrix4<T> operator - (const Matrix4<T>& lhs, const Matrix4<T>& rhs)
{
	Matrix4<T> result = lhs;
	lhs -= rhs;
	return lhs;
}

template <class T>
inline Matrix4<T> operator * (const Matrix4<T>& lhs, const Matrix4<T>& rhs)
{
	Matrix4<T> result = lhs;
	result *= rhs;
	return result;
}

template <class T>
inline Vector4<T> operator * (const Matrix4<T>& lhs, const Vector4<T>& rhs)
{
	Vector4<T> result;	
	lhs.mult(&result, rhs);
	return result;
}

template <class T>
inline Matrix4<T> operator / (const Matrix4<T>& lhs, T s)
{
	Matrix4<T> result = lhs;	
	result/= s;
	return result;
}

template <class T>
inline Matrix4<T> operator * (const Matrix4<T>& lhs, T s)
{
	Matrix4<T> result = lhs;	
	result*= s;
	return result;
}

template <class T>
inline Matrix4<T> operator * (T s, const Matrix4<T>& rhs)
{
	Matrix4<T> result = rhs;	
	result*= s;
	return result;
}
