#include <iostream>
#include <cstdarg>
#include <cmath>
#include <limits.h>
#include <stdio.h>

#include "libgen.h"
#include "MedImage.h"
#include "Tools.h"

#pragma warning (disable: 4996)		// >> 'function' was declared deprecated <<

namespace CAMP {

MedImage::MedImage(int type): ImageBase<unsigned short>() {
	m_gradient2d = NULL;
	m_gradient3d = NULL;
	m_gradient3df = NULL;
	m_gradimage = NULL;
	m_gradimage_f = NULL;
	m_type = type;
	m_invert = false;
}


MedImage::~MedImage() {
	if (m_gradient2d) delete m_gradient2d;
	if (m_gradimage) delete m_gradimage;
	if (m_gradimage_f) delete m_gradimage_f;
}


int MedImage::load(const char *filename) {
	// filename might be modified by the basename/dirname methods, thus create a copy
	char filename2[255];
	sprintf(filename2, filename);
	FILE *file = fopen(filename, "r");
	if (!file) return 1;
	char type[256], rawfile[256], rawpath[256];
	int dims = 0, dimx = 1, dimy = 1, dimz = 1, header = 0, result = -1;
	float sx = 1.0, sy = 1.0, sz = 1.0, px = 0.0, py = 0.0, pz = 0.0;
	if (fscanf(file, "NDims = %i\n", &dims) != 1) { fclose(file); return 2; }
	if (fscanf(file, "DimSize = %i %i %i\n", &dimx, &dimy, &dimz) != dims) { fclose(file); return 2; }
	std::cout << "Dimensions " << dimx << " x " << dimy << " x " << dimz << std::endl;
	if (fscanf(file, "ElementSpacing = %f %f %f\n", &sx, &sy, &sz) != dims) { fclose(file); return 2; }
	std::cout << "Spacing " << sx << " x " << sy << " x " << sz << std::endl;
	fscanf(file, "Position = %f %f %f\n", &px, &py, &pz);
	fscanf(file, "ElementByteOrderMSB = %s\n", type);
	if (fscanf(file, "ElementType = %s\n", type) != 1) { fclose(file); return 2; }
	fscanf(file, "HeaderSize = %i\n", &header);
	if (fscanf(file, "ElementDataFile = %s\n", rawfile) != 1) { fclose(file); return 2; }
	sprintf(rawpath, "%s/%s", dirname(filename2), rawfile);
	std::cout << "Loading file " << rawpath << std::endl;
	fclose(file);
	if (strcmp(type, "MET_UCHAR") == 0) {
		result = load8(rawpath, dimx, dimy, dimz);
		// Assume 2D 8 bit images to be Ultrasound
		if (dims == 2) m_type = US;
	}
	else if (strcmp(type, "MET_SHORT") == 0) {
		result = load16(rawpath, dimx, dimy, dimz, 1, 1, 1);
		if (dims == 2) m_type = XRAY;
	}
	else if (strcmp(type, "MET_USHORT") == 0) {
		result = load16(rawpath, dimx, dimy, dimz);
		// We guess that 2D 16 bit images are X-Ray
		if (dims == 2) m_type = XRAY;
	}
	else if (strcmp(type, "MET_FLOAT") == 0) result = load32(rawpath, dimx, dimy, dimz);
	if (result == 0) {
		setPhysicalSize(sx, sy, sz);
		return 0;
	}
	return 3;
}


int MedImage::load16(char *filename, int dimx, int dimy, int dimz, int frames, int channels, int sign) {
	FILE *file = fopen(filename, "r");
	if (!file) return 1;
	init(dimx, dimy, dimz, frames, channels);
	if (m_gradient2d) { delete m_gradient2d; m_gradient2d = NULL; }
	if (m_gradimage) { delete m_gradimage; m_gradimage = NULL; }
	if (m_gradimage_f) { delete m_gradimage_f; m_gradimage_f = NULL; }
	fread(m_data, sizeof(TD)*dimx, dimy*dimz, file);
	fclose(file);
	sprintf(m_filename, filename);

	int min = 100000, max = -100000;
	if (sign) {
		// For signed 16 Bit values: Check range and shift accordingly
		signed short *data = (signed short *) m_data;
		for (int i = 0; i < dimx * dimy * dimz; i++) {
			if (data[i] < min) min = data[i];
			if (data[i] > max) max = data[i];
		}
		std::cout << "Signed range: " << min << "..." << max << std::endl;
		if (min < 0) {
			for (int i = 0; i < dimx * dimy * dimz; i++)
				m_data[i] = (unsigned short) (data[i] + min);
			max -= min;
			min = 0;
		}
	} else {
		for (int i = 0; i < dimx * dimy * dimz; i++) {
			if (m_data[i] < min) min = m_data[i];
			if (m_data[i] > max) max = m_data[i];
		}
	}
	setMinMax((float)min, (float)max);
	std::cout << "Volume loaded, min = " << min << ", max = " << max << std::endl;

#ifdef SUN
	// for Sun Machines: flip byte ordering!
	TB *buf = (TB*) m_data;
	for (int i = 0; i < dimx * dimy * dimz; i++) {
		TB tmp = buf[2*i];
		buf[2*i] = buf[2*i+1];
		buf[2*i+1] = tmp;
	}
#endif
	return 0;
}


int MedImage::load8(char *filename, int dimx, int dimy, int dimz, int frames, int channels) {
	FILE *file = fopen(filename, "r");
	if (!file) return 1;
	init(dimx, dimy, dimz, frames, channels);
	if (m_gradient2d) { delete m_gradient2d; m_gradient2d = NULL; }
	if (m_gradimage) { delete m_gradimage; m_gradimage = NULL; }
	if (m_gradimage_f) { delete m_gradimage_f; m_gradimage_f = NULL; }
	unsigned char *data8 = (unsigned char *) m_data;
	fread(data8, sizeof(unsigned char)*dimx, dimy*dimz, file);
	fclose(file);
	sprintf(m_filename, filename);

	int min = 100000, max = -100000;
	for (int i = dimx*dimy*dimz-1; i >= 0; i--) {
		if (data8[i] < min) min = data8[i];
		if (data8[i] > max) max = data8[i];
		m_data[i] = data8[i];	// expand it to 16 bit
	}
	setMinMax((float)min, (float)max);
	std::cout << "Volume loaded, min = " << min << ", max = " << max << std::endl;
	return 0;
}


int MedImage::load32(char *filename, int dimx, int dimy, int dimz, int frames, int channels) {
	FILE *file = fopen(filename, "r");
	if (!file) return 1;
	init(dimx, dimy, dimz, frames, channels);
	if (m_gradient2d) { delete m_gradient2d; m_gradient2d = NULL; }
	if (m_gradimage) { delete m_gradimage; m_gradimage = NULL; }
	if (m_gradimage_f) { delete m_gradimage_f; m_gradimage_f = NULL; }
	m_data = static_cast<unsigned short*>(aligned_malloc( dimx * dimy * dimz * sizeof(unsigned short)));	
	float *data32 = new float[dimx*dimy*dimz];
	fread(data32, sizeof(float)*dimx, dimy*dimz, file);
	fclose(file);
	sprintf(m_filename, filename);

#ifdef SUN
// for Sun Machines: flip byte ordering!
	TB *buf = (TB*) data32;
	for (int i = 0; i < dimx * dimy * dimz; i++) {
		TB tmp;
		tmp = buf[4*i]; buf[4*i] = buf[4*i+3]; buf[4*i+3] = tmp;
		tmp = buf[4*i+1]; buf[4*i+1] = buf[4*i+2]; buf[4*i+2] = tmp;
	}
#endif

	float min = 1e10, max = -1e10; int i;
	for (i = 0; i < dimx*dimy*dimz; i++) {
		if (data32[i] < min) min = data32[i];
		if (data32[i] > max) max = data32[i];
	}
	if (m_debug) std::cout << "Floating point range ["
	<< min << ", " << max << "]" << std::endl;
	if (max == min) max += 1.0;
	for (i = 0; i < dimx*dimy*dimz; i++)
		m_data[i] = (int)(65535.0*(data32[i]-min)/(max-min));
	setMinMax(0, 65535);
	delete data32;
	return 0;
}


int MedImage::reload() {
	FILE *file = fopen(m_filename, "r");
	if (!file) return 1;
	if (fread(m_data, sizeof(TD)*m_dim[0], m_dim[1]*m_dim[2], file) != (unsigned)m_dim[1]*m_dim[2]) {
		std::cout << "Error reading volume." << std::endl; return 2;
	}
	fclose(file);
	if (m_debug) std::cout << "Volume reloaded." << std::endl;
	return 0;
}


void MedImage::getSlice(TB *image, int nr) {
	if ((m_data == NULL) || (image == NULL)) return;
	nr = nr % m_dim[2]; if (nr < 0) nr += m_dim[2];
	for (int i = 0; i < m_dim[0] * m_dim[1]; i++) {
		int value = (int)(((((float)m_data[m_dim[0] * m_dim[1] * nr + i]
		- m_wl[2])) * 255.0) / (m_wl[3] - m_wl[2]));
		if (value < 0) value = 0;
		if (value > 255) value = 255;
		if (m_invert) value = 255 - value;
		// the following two lines use a stride being a power of two
		int w2 = Tools::nextPowerTwo(m_dim[0]);
		int j = (w2 == m_dim[0])? i: i % m_dim[0] + (i / m_dim[0]) * w2;
		image[j] = value;
	}
}


void MedImage::downSample2D() {
	if (m_gradient2d) { delete m_gradient2d; m_gradient2d = NULL; }
	if (m_gradimage) { delete m_gradimage; m_gradimage = NULL; }
	if (m_gradimage_f) { delete m_gradimage_f; m_gradimage_f = NULL; }
	downSample(2, 2);
}


void MedImage::downSample3D() {
	if (m_gradient2d) { delete m_gradient2d; m_gradient2d = NULL; }
	if (m_gradimage) { delete m_gradimage; m_gradimage = NULL; }
	if (m_gradimage_f) { delete m_gradimage_f; m_gradimage_f = NULL; }
	downSample(2, 2, 2);
}


void MedImage::flipY() {
	
	TD *newdata = static_cast<TD*>(aligned_malloc( m_dim[0]*m_dim[1]*m_dim[2] * sizeof(TD)));	
	for (int z = 0; z < m_dim[2]; z++) {
		for (int y = 0; y < m_dim[1]; y++) {
			for (int x = 0; x < m_dim[0]; x++) {
				newdata[x+y*m_dim[0]+z*m_dim[0]*m_dim[1]] =
					m_data[m_dim[0]-x-1+(m_dim[1]-y-1)*m_dim[0]+z*m_dim[0]*m_dim[1]];
			}
		}
	}
	destroy();
	m_data = newdata;
}


void MedImage::flipZ() {
	for (int z = 0; z < m_dim[2]/2; z++) {
		for (int y = 0; y < m_dim[1]; y++) {
			for (int x = 0; x < m_dim[0]; x++) {
				TD tmp = m_data[x+y*m_dim[0]+z*m_dim[0]*m_dim[1]];
				m_data[x+y*m_dim[0]+z*m_dim[0]*m_dim[1]] =
				m_data[x+y*m_dim[0]+(m_dim[2]-z-1)*m_dim[0]*m_dim[1]];
				m_data[x+y*m_dim[0]+(m_dim[2]-z-1)*m_dim[0]*m_dim[1]] = tmp;
			}
		}
	}
	if (m_debug) std::cout << "Image flipped on Z axis" << std::endl;
}


void MedImage::computeGradient2Dold(int slice) {
	if (m_data == NULL) return;
	ImageBase<float> *img = (ImageBase<float> *) computeGradient2D(FLOAT, NULL,	true);
	m_gradient2d = img->getData();
/*	if (m_gradient2d == NULL) m_gradient2d = new TF[2*m_dim[0]*m_dim[1]];
	int j, x, y;
	slice *= m_dim[0] * m_dim[1];
	for (y = 0; y < m_dim[1]; y++) {
		for (x = 0; x < m_dim[0]; x++) {
			int u, v, sx = 0, sy = 0;
			if ((x == 0) || (x == m_dim[0]-1)
			 || (y == 0) || (y == m_dim[1]-1)) {
				m_gradient2d[2*(x+m_dim[0]*y)  ] = 0.0;
				m_gradient2d[2*(x+m_dim[0]*y)+1] = 0.0;
			} else {
				for (j = 0; j < 9; j++) {
					u = j % 3 - 1;
					v = j / 3 - 1;
					sx += sobel2d[j] * m_data[x+u+m_dim[0]*(y+v)+slice];
					sy += sobel2d[j] * m_data[x+v+m_dim[0]*(y+u)+slice];
				}
				m_gradient2d[2*(x+m_dim[0]*y)  ] = (TF)sx / 9.0f;
				m_gradient2d[2*(x+m_dim[0]*y)+1] = (TF)sy / 9.0f;
			}
		}
	} */
}


void MedImage::computeGradient3Df(bool release) {
	if (m_data == NULL) return;
	
	if (m_gradimage_f) delete m_gradimage_f;
	m_gradimage_f = (ImageBase<float> *) computeGradient3D(FLOAT);
	m_gradient3df = m_gradimage_f->getData();
/*	if (m_gradimage_f == NULL) {
		m_gradimage_f = new ImageBase<float>();
		m_gradimage_f->init(m_dim[0]-2, m_dim[1]-2, m_dim[2]-2, 1, 3);
		m_gradient3df = m_gradimage_f->getData();
	}
	int i = 0, j, x, y, z;
	TF min = 100000.0, max = -100000.0;
	if (m_debug) std::cout << "Computing gradient..." << std::endl;
	for (z = 1; z < m_dim[2] - 1; z++) {
		for (y = 1; y < m_dim[1] - 1; y++) {
			for (x = 1; x < m_dim[0] - 1; x++) {
				int sx = 0, sy = 0, sz = 0;
				int u, v, w;
				for (j = 0; j < 27; j++) {

					// Hack to skip zeroes in the middle of the sobel filter.
					// Remove this line if the filter changes!
					if (j == 9) j = 18;

					u = j % 3 - 1;
					v = (j % 9) / 3 - 1;
					w = j / 9 - 1;
					sx += sobel3d[j] * m_data[x+w+m_dim[0]*(y+v)+m_dim[0]*m_dim[1]*(z+u)];
					sy += sobel3d[j] * m_data[x+u+m_dim[0]*(y+w)+m_dim[0]*m_dim[1]*(z+v)];
					sz += sobel3d[j] * m_data[x+u+m_dim[0]*(y+v)+m_dim[0]*m_dim[1]*(z+w)];
				}
				if (min > sx) min = (float)sx;
				if (min > sy) min = (float)sy;
				if (min > sz) min = (float)sz;
				if (max < sx) max = (float)sx;
				if (max < sy) max = (float)sy;
				if (max < sz) max = (float)sz;
				m_gradient3df[i  ] = ((TF)sx);// / 10000.0f;
				m_gradient3df[i+1] = ((TF)sy);// / 10000.0f;
				m_gradient3df[i+2] = ((TF)sz);// / 10000.0f;
//				TF norm = sqr(m_gradient3d[i])+sqr(m_gradient3d[i+1])+sqr(m_gradient3d[i+2]);
//				if (min > norm) min = norm;
//				if (max < norm) max = norm;
				i += 3;
			}
		}
	}
//	min = sqrt(min); max = sqrt(max);
//	m_gradient3d_max = max;
	if (m_debug) std::cout << "Done. Min = " << min << ", Max = " << max << std::endl; */
	if (release) {
		destroy();
		std::cout << "Image data released." << std::endl;
	}
}


void MedImage::computeGradient3Dold(bool release, bool fourchannels) {
	if (m_data == NULL) return;
	if (m_gradimage) delete m_gradimage;
	m_gradimage = (ImageBase<short> *)computeGradient3D(SHORT, NULL, false, fourchannels);
	m_gradient3d = m_gradimage->getData();
	
/*	if (m_gradimage == NULL) {
		m_gradimage = new ImageBase<short>();
		m_gradimage->init(m_dim[0]-2, m_dim[1]-2, m_dim[2]-2, 1, fourchannels? 4: 3);
		m_gradient3d = m_gradimage->getData();
	}
	int i = 0, j, x, y, z, bailout = 0;
	int min = 1000000, max = -1000000;
	if (m_debug) std::cout << "Computing gradient (short range "
	<< SHRT_MIN << ".." << SHRT_MAX << ")..." << std::endl;
	for (z = 1; z < m_dim[2] - 1; z++) {
		for (y = 1; y < m_dim[1] - 1; y++) {
			for (x = 1; x < m_dim[0] - 1; x++) {
				int sx = 0, sy = 0, sz = 0;
				int u, v, w;
				for (j = 0; j < 27; j++) {
					// Hack to skip zeroes in the middle of the sobel filter.
					// Remove this line if the filter changes!
					if (j == 9) j = 18;

					u = j % 3 - 1;
					v = (j % 9) / 3 - 1;
					w = j / 9 - 1;
					sx += sobel3d[j] * m_data[x+w+m_dim[0]*(y+v)+m_dim[0]*m_dim[1]*(z+u)];
					sy += sobel3d[j] * m_data[x+u+m_dim[0]*(y+w)+m_dim[0]*m_dim[1]*(z+v)];
					sz += sobel3d[j] * m_data[x+u+m_dim[0]*(y+v)+m_dim[0]*m_dim[1]*(z+w)];
				}
				// clamp gradient values to range of signed short
				if (sx < SHRT_MIN) { sx = SHRT_MIN; bailout++; }
				if (sy < SHRT_MIN) { sy = SHRT_MIN; bailout++; }
				if (sz < SHRT_MIN) { sz = SHRT_MIN; bailout++; }
				if (sx > SHRT_MAX) { sx = SHRT_MAX; bailout++; }
				if (sy > SHRT_MAX) { sy = SHRT_MAX; bailout++; }
				if (sz > SHRT_MAX) { sz = SHRT_MAX; bailout++; }
				// update minimum and maximum
				if (sx < min) min = sx; if (sy < min) min = sy; if (sz < min) min = sz;
				if (sx > max) max = sx; if (sy > max) max = sy; if (sz > max) max = sz;
				m_gradient3d[i  ] = sx;
				m_gradient3d[i+1] = sy;
				m_gradient3d[i+2] = sz;
				i += 3;
				// Put the original intensity into the fourth channel, if desired
				if (fourchannels) m_gradient3d[i++] = m_data[x+m_dim[0]*(y+m_dim[1]*z)];
			}
		}
	}
	// set the min/max values as range for the gradient volume
	for (i = 0; i < 3; i++) m_gradimage->setMinMax(min, max, i);
	// Inherit the range for intensity channel
	if (fourchannels) m_gradimage->setMinMax(m_wl[2], m_wl[3], 3);
	if (m_debug) std::cout << "Done. Range = [" << min << ".." << max << "], "
	<< bailout << " values exceeded." << std::endl; */
	if (release) {
		destroy();
		std::cout << "Image data released." << std::endl;
	}
}


void MedImage::computeSimpleGradient3D()
{
	if (m_data == NULL) return;
	if (m_gradimage == NULL) {
		m_gradimage = new ImageBase<short>();
		m_gradimage->init(m_dim[0], m_dim[1], m_dim[2], 1, 3);
		m_gradient3d = m_gradimage->getData();
	}
	int sx = 0, sy = 0, sz = 0; 
	int i = 0, x, y, z;
	for (z = 0; z < m_dim[2]; ++z) {
		for (y = 0; y < m_dim[1]; ++y) {
			for (x = 0; x < m_dim[0]; ++x) {

				sx = ((x+1 < m_dim[0]) ? ((int)getVoxelValue(x+1,y,z)) : 0) - ((x > 0) ? ((int)getVoxelValue(x-1,y,z)) : 0);
				sy = ((y+1 < m_dim[1]) ? ((int)getVoxelValue(x,y+1,z)) : 0) - ((y > 0) ? ((int)getVoxelValue(x,y-1,z)) : 0);
				sz = ((z+1 < m_dim[2]) ? ((int)getVoxelValue(x,y,z+1)) : 0) - ((z > 0) ? ((int)getVoxelValue(x,y,z-1)) : 0);

				// clamp gradient values to range of signed short
				if (sx < SHRT_MIN) { sx = SHRT_MIN; }
				if (sy < SHRT_MIN) { sy = SHRT_MIN; }
				if (sz < SHRT_MIN) { sz = SHRT_MIN; }
				if (sx > SHRT_MAX) { sx = SHRT_MAX; }
				if (sy > SHRT_MAX) { sy = SHRT_MAX; }
				if (sz > SHRT_MAX) { sz = SHRT_MAX; }
				m_gradient3d[i  ] = (short)sx;
				m_gradient3d[i+1] = (short)sy;
				m_gradient3d[i+2] = (short)sz;
				i += 3;
			}
		}
	}
}

}

#pragma warning (default: 4996)