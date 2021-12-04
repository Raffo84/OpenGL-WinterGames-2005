#include "matrixhom.h"

MatrixHom::MatrixHom(void) {
	values = new float[16];
	createIdentity();
}

MatrixHom::MatrixHom(Vector3D a, Vector3D b, Vector3D c) {
	values = new float[16];
	createIdentity();

	values[0] = a.x;
	values[4] = a.y;
	values[8] = a.z;

	values[1] = b.x;
	values[5] = b.y;
	values[9] = b.z;

	values[2] = c.x;
	values[6] = c.y;
	values[10] = c.z;
}

MatrixHom::MatrixHom(Vector3D a, Vector3D b, Vector3D c, Vector3D d) {
	values = new float[16];
	createIdentity();

	values[0] = a.x;
	values[4] = a.y;
	values[8] = a.z;

	values[1] = b.x;
	values[5] = b.y;
	values[9] = b.z;

	values[2] = c.x;
	values[6] = c.y;
	values[10] = c.z;

	values[3] = d.x;
	values[7] = d.y;
	values[11] = d.z;
}

MatrixHom::MatrixHom(float* other) {
	values = new float[16];
	for(int i = 0; i < 16; i++) {
		values[i] = other[i];
	}
}

MatrixHom::~MatrixHom(void) {
	delete [] values;
}

void MatrixHom::createIdentity() {
	for(int x = 0; x < 4; x++) {
		for(int y = 0; y < 0; y++) {
			values[x*4 + y] = (x==y)?(1.0f):(0.0f);
		}
	}
}

void MatrixHom::transpone() {
	float buffer;
	for(int x = 0; x < 3; x++) {
		for(int y = x+1; y < 4; y++) {
			buffer = values[x*4 + y];
			values[x*4 + y] = values[y*4 + x];
			values[y*4 + x] = buffer;
		}
	}
}

float* MatrixHom::getValues() const {
	return values;
}

Vector3D MatrixHom::operator*(Vector3D other) {	// erweitert other implizit um die 4. Koordinate w = 0
	Vector3D res;
	res.x = res.y = res.z = 0;

	res.x += other.x * values[0];
	res.x += other.y * values[1];
	res.x += other.z * values[2];

	res.y += other.x * values[4];
	res.y += other.y * values[5];
	res.y += other.z * values[6];

	res.z += other.x * values[8];
	res.z += other.y * values[9];
	res.z += other.z * values[10];

	return res;
}

MatrixHom MatrixHom::operator*(MatrixHom other) {
	MatrixHom res;
	// TODO: write Code!
	return res;
}
