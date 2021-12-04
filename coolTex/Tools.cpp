#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

#include "Tools.h"
#include "MedImage.h"
#include "Bitmap.h"
#include "libgen.h"
#include "dirent.h"

#pragma warning (disable: 4996)		// >> 'function' was declared deprecated <<

namespace CAMP {

void Tools::computeGradient2D(TF *data, TF *buffer, int width, int height, bool interactive) {
	int i, j, x, y;
	i = (interactive)? 2: 1;

	for (y = 0; y < height/i; y++) {
		for (x = 0; x < width/i; x++) {
			int u, v;
			TF sx = 0, sy = 0;
			if ((x == 0) || (x == width/i-1)
				|| (y == 0) || (y == height/i-1)) {
					buffer[2*(x+width*y)  ] = 0;
					buffer[2*(x+width*y)+1] = 0;
				} else {
					for (j = 0; j < 9; j++) {
						u = j % 3 - 1;
						v = j / 3 - 1;
						sx += Image::SOBEL2D[j] * data[x+u+width*(y+v)];
						sy += Image::SOBEL2D[j] * data[x+v+width*(y+u)];
					}
					buffer[2*(x+width*y)  ] = sx / 9.0f;
					buffer[2*(x+width*y)+1] = sy / 9.0f;
				}
		}
	}
}

void Tools::computeSimpleGradient3D(Image *image, float *grad_buffer) { 
	int i = 0, x, y, z;
	int slices = image->getSlices(), height = image->getHeight(), width = image->getWidth();
	for (z = 0; z < slices; ++z) {
		for (y = 0; y < height; ++y) {
			for (x = 0; x < width; ++x) {
				grad_buffer[i  ] = ((x+1 < width) ? (image->getVoxelFloat(x+1,y,z)) : 0) - ((x > 0) ? (image->getVoxelFloat(x-1,y,z)) : 0);
				grad_buffer[i+1] = ((y+1 < height) ? (image->getVoxelFloat(x,y+1,z)) : 0) - ((y > 0) ? (image->getVoxelFloat(x,y-1,z)) : 0);
				grad_buffer[i+2] = ((z+1 < slices) ? (image->getVoxelFloat(x,y,z+1)) : 0) - ((z > 0) ? (image->getVoxelFloat(x,y,z-1)) : 0);
				i += 3;
			}
		}
	}
}

void Tools::getByteImage(TF *data, TB *image, int width, int height, TF min, TF max, bool interactive) {
	if ((min == 0) && (max == 0)) {
		min = 1e10; max = -1e10;
		for (int i = 0; i < width*height; i++) {
			if (data[i] < min) min = data[i];
			if (data[i] > max) max = data[i];
			if (interactive) {
				if (i % width >= width/2-1) i += width/2;
				if (i / width >= height/2-1) i += width*height;
			}
		}
	}
	if (min == max) max += 1.0;
	for (int i = 0; i < width*height; i++) {
		int value = (int)(((data[i] - min) * 255.0) / (max - min));
		if (value < 0) value = 0;
		if (value > 255) value = 255;
		// the following two lines use a stride being a power of two
		int w2 = nextPowerTwo(width);
		int j = (w2 == width)? i: i % width + (i / width) * w2;
		image[j] = value;
		if (interactive) {
			if (i % width >= width/2-1) i += width/2;
			if (i / width >= height/2-1) i += width*height;
		}
	}
}


void Tools::getRGBAImage(float *data, unsigned char *buffer, int width, int height, Image *img, bool usewl) {
	int i, j; float min[3], max[3], val[3];
	// restrict to maximum of three channels
	int c = img->getChannels(); if (c > 3) c = 3;

	if (usewl) {
		for (j = 0; j < c; j++) { min[j] = img->getMin(j); max[j] = img->getMax(j); }
	} else {
		// compute min/max values of both intensities in the slices
		for (j = 0; j < c; j++) { min[j] = data[j]; max[j] = data[j]; }
		for (i = 0; i < width * height; i++) {
			for (j = 0; j < c; j++) {
				if (data[c*i+j] < min[j]) min[j] = data[c*i+j];
				if (data[c*i+j] > max[j]) max[j] = data[c*i+j];
			}
		}
		// avoid division by zero if channel is constant
		for (j = 0; j < c; j++) if (min[j] == max[j]) max[j] += 1.0f;
	}
	
	for (i = 0; i < width * height; i++) {
		for (j = 0; j < c; j++) {
			val[j] = ((data[c*i+j] - min[j]) * 255.0f) / (max[j] - min[j]);
			if (val[j] < 0.0f) val[j] = 0.0f;
			if (val[j] > 255.0f) val[j] = 255.0f;
		}
		buffer[4*i  ] = (unsigned char) val[0];
		buffer[4*i+1] = (unsigned char) val[1];
		buffer[4*i+2] = (unsigned char) val[c-1];
		buffer[4*i+3] = 0;
	}
}


void Tools::getRGBImage(TF *data, TB *buffer, int width, int height, TF min, TF max, TF *colormap, int colors, int mode) {
	if ((min == 0) && (max == 0)) {
		min = 1e10; max = -1e10;
		for (int i = 0; i < width*height; i++) {
			if (data[i] < min) min = data[i];
			if (data[i] > max) max = data[i];
		}
	}
	if (min == max) max += 1.0;
	for (int i = 0; i < width*height; i++) {
		TF value;
		if (mode) {
			if (data[i] < 0) value = 0.5f * (colors-1) * (1-data[i]/min);
			else value = 0.5f * (colors-1) * (1+data[i]/max);
		} else value = ((data[i] - min) * (colors-1)) / (max - min);
		if (value < 0.0) value = 0.0;
		if (value >= (colors-1)) value = (colors-1) - 0.001f;
		int intval = (int) value;
		value = value - intval;
		for (int j = 0; j < 3; j++)
			buffer[4*i+j] = (int) (255.0 * (colormap[3*intval+j] * (1-value)
			+ colormap[3*(intval+1)+j] * value));
		buffer[4*i+3] = 0;
	}
}


void Tools::getRGBImage(int *data, unsigned char *buffer, int width, int height, TF min, TF max, TF *colormap, int colors, int mode) {
	if ((min == 0) && (max == 0)) {
		min = 1e10; max = -1e10;
		for (int i = 0; i < width*height; i++) {
			if (static_cast<float>(data[i]) < min) min = static_cast<float>(data[i]);
			if (static_cast<float>(data[i]) > max) max = static_cast<float>(data[i]);
		}
	}
	if (min == max) max += 1.0;
	for (int i = 0; i < width*height; i++) {
		TF value;
		if (mode) {
			if (data[i] < 0) value = 0.5f * (colors-1) * (1-data[i]/min);
			else value = 0.5f * (colors-1) * (1+data[i]/max);
		} else value = (((float)data[i] - min) * (colors-1)) / (max - min);
		if (value < 0.0) value = 0.0;
		if (value >= (colors-1)) value = (colors-1) - 0.001f;
		int intval = (int) value;
		value = value - intval;
		for (int j = 0; j < 3; j++)
			buffer[4*i+j] = (int) (255.0 * (colormap[3*intval+j] * (1-value)
			+ colormap[3*(intval+1)+j] * value));
		buffer[4*i+3] = 0;
	}
}



void Tools::getByteImageGrad(TF *data, TB *image, int width, int height, TF min, TF max, bool interactive) {
	int x, y;
	int k = interactive? 2: 1;
	if ((min == 0) && (max == 0)) {
		min = 1e10; max = -1e10;
		for (y = 0; y < height/k; y++) {
			for (x = 0; x < width/k; x++) {
				TF val = sqr(data[2*(x+width*y)]) + sqr(data[2*(x+width*y)+1]);
				if (min > val) min = val;
				if (max < val) max = val;
			}
		}
		min = sqrt(min); max = sqrt(max);
	}
	if (min == max) max += 1.0;
	int w2 = nextPowerTwo(width);
	for (y = 0; y < height/k; y++) {
		for (x = 0; x < width/k; x++) {
			image[x+w2*y] = (TB) (255.0 * (sqrt(sqr(data[2*(x+width*y)])
				+ sqr(data[2*(x+width*y)+1]))-min) / (max-min));
		}
	}
}


void Tools::getByteImageGrad(Image *img, unsigned char *image) {
	int x, y;
	img->iterate();
	float min = 1e10, max = -1e10;
	for (y = 0; y < img->getHeight(); y++) {
		for (x = 0; x < img->getWidth(); x++) {
			float v1, v2;
			img->getNext(&v1); img->getNext(&v2);
			float val = sqr(v1) + sqr(v2);
			if (min > val) min = val;
			if (max < val) max = val;
		}
	}
	min = sqrt(min); max = sqrt(max);
	img->iterate();
	if (min == max) max += 1.0;
	int w2 = nextPowerTwo(img->getWidth());
	for (y = 0; y < img->getHeight(); y++) {
		for (x = 0; x < img->getWidth(); x++) {
			float v1, v2;
			img->getNext(&v1); img->getNext(&v2);			
			image[x+w2*y] = (TB) (255.0 * (sqrt(sqr(v1)
				+ sqr(v2))-min) / (max-min));
		}
	}
}


void Tools::smoothImageGrad(TF *data, int width, int height) {
	const int size = 2;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float sum1 = 0.0, sum2 = 0.0, count = 0.0;
			for (int yy = -size; yy <= size; yy++) {
				for (int xx = -size; xx <= size; xx++) {
					if ((y+yy >= 0) && (y+yy < height)
						&&(x+xx >= 0) && (x+xx < width)) {
							sum1 += data[2*(x+xx+width*(y+yy))];
							sum2 += data[2*(x+xx+width*(y+yy))+1];
							count += 1.0;
						}
				}
			}
			data[2*(x+width*y)  ] = sum1 / count;
			data[2*(x+width*y)+1] = sum2 / count;
		}
	}
}


void Tools::smoothImageGrad(MedImage *img) {
	smoothImageGrad(img->getGradient2D(), img->getWidth(), img->getHeight());
}


void Tools::rigidTransform(float *vector, float *pars, int mode) {
	int i;
	float x = pars[3];
	float y = pars[4];
	float z = pars[5];
	if (mode == 0) for (i = 0; i < 3; i++) vector[i] += pars[i];
	float result[3];
	result[0] = cos(y)*cos(z)*vector[0]
	+ (sin(x)*sin(y)*cos(z)-cos(x)*sin(z))*vector[1]
	+ (cos(x)*sin(y)*cos(z)+sin(x)*sin(z))*vector[2];
	result[1] = cos(y)*sin(z)*vector[0]
	+ (sin(x)*sin(y)*sin(z)+cos(x)*cos(z))*vector[1]
	+ (cos(x)*sin(y)*sin(z)-sin(x)*cos(z))*vector[2];
	result[2] = -sin(y)*vector[0]
	+ sin(x)*cos(y)*vector[1]
	+ cos(x)*cos(y)*vector[2];
	for (i = 0; i < 3; i++) vector[i] = result[i];
}


void Tools::rigidTransformMat(float *vector, float *matrix, int mode) {
	float hom[4]; int i;
	for (i = 0; i < 3; i++) hom[i] = vector[i];
	hom[3] = (mode)? 0.0f: 1.0f;
	for (i = 0; i < 3; i++)
		vector[i] = matrix[i]*hom[0] + matrix[i+4]*hom[1]
		          + matrix[i+8]*hom[2] + matrix[i+12]*hom[3];
}


void Tools::hom2euler(float *pars, float *matrix) {
	pars[0] = matrix[12];
	pars[1] = matrix[13];
	pars[2] = matrix[14];
	pars[3] = atan2(-matrix[6], matrix[10]);
	pars[4] = asin(matrix[2]);
	pars[5] = atan2(-matrix[1], matrix[0]); // vorher 5
}


void Tools::euler2hom(float *pars, float *matrix) {
	float cx = cos(pars[3]), sx = sin(pars[3]),
		cy = cos(pars[4]), sy = sin(pars[4]),
		cz = cos(pars[5]), sz = sin(pars[5]);
	matrix[0] = cy*cz;
	matrix[1] = -cy*sz;
	matrix[2] = sy;
	matrix[3] = 0.0;
	matrix[4] = cx*sz+sx*sy*cz;
	matrix[5] = cx*cz-sx*sy*sz;
	matrix[6] = -sx*cy;
	matrix[7] = 0.0;
	matrix[8] = sx*sz-cx*sy*cz;
	matrix[9] = sx*cz+cx*sy*sz;
	matrix[10]= cx*cy;
	matrix[11]= 0.0;
	matrix[12]= pars[0];
	matrix[13]= pars[1];
	matrix[14]= pars[2];
	matrix[15]= 1.0;
}


void Tools::multMatrix(float *left, float *right, int result) {
	float tmp[16]; int i;
	for (i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			tmp[i*4+j] = 0;
			for (int k = 0; k < 4; k++) tmp[i*4+j] += left[k*4+j] * right[i*4+k];
		}
		if (result)
			for (i = 0; i < 16; i++) right[i] = tmp[i];
		else
			for (i = 0; i < 16; i++) left[i] = tmp[i];
}

Image *Tools::loadMetaImage(char *filename) {
	// If the extension indicates a bitmap, use the Bitmap class instead of MetaImage loading
	if (strcmp(filename+strlen(filename)-4, ".bmp") == 0) {
		Bitmap *bmp = new Bitmap();
		if (bmp->load(filename, false)) {
			bmp->computeMinMax();
			return bmp;
		}
		return NULL;
	}
	Image *img = NULL;
	char filename2[1024], rawpath[1024];
	std::string keyword, keyval;
	sprintf(filename2, filename);
	int dims = 0, channels = 1, header = 0;
	int i, dim[4]; bool msb = 0;
	float spacing[4], position[4];
	for (i = 0; i < 4; i++) { dim[i] = 1; spacing[i] = 1.0f; position[i] = 0.0f; }
		
	std::ifstream file;
	file.open(filename, std::ifstream::in); 
	if (!file.good()) { 
		return NULL; }
	
	std::string::iterator it;
	while (file.good()) {
		std::getline(file, keyword, '=');
		it=keyword.end();
 		keyword.erase(std::remove(keyword.begin(),keyword.end(),' '),it);
		if (keyword == "NDims") {
			file >> dims;
			std::cout << dims << " dimensions" << std::endl;
			std::getline(file, keyval, '\n');
		} else if (keyword == "DimSize") {
			for (i = 0; i < dims; i++) file >> dim[i];
			std::getline(file, keyval, '\n');
			std::cout << "Dimensions " << dim[0] << " x " << dim[1] << " x " << dim[2]
			<< " x " << dim[3] << std::endl;
		} else if (keyword == "ElementSpacing") {
			for (i = 0; i < dims; i++) file >> spacing[i];
			std::getline(file, keyval, '\n');
			std::cout << "Spacing " << spacing[0] << " x " << spacing[1] << " x " << spacing[2]
			<< " x " << spacing[3] << std::endl;
		} else if (keyword == "Position") {
			for (i = 0; i < dims; i++) file >> position[i];
			std::getline(file, keyval, '\n');
			std::cout << "Position " << position[0] << " x " << position[1] << " x " << position[2]
			<< " x " << position[3] << std::endl;
		} else if (keyword == "ElementByteOrderMSB") {
			std::getline(file, keyval, '\n');
			it=keyval.end();
			keyval.erase(std::remove(keyval.begin(),keyval.end(),' '),it);
			// remove remaining DOS line breaks '\r'
			// FIXME: might not work for MAC line breaks
			it=keyval.end();
			keyval.erase(std::remove(keyval.begin(),keyval.end(),'\r'),it);
			msb = ((keyval == "True") || (keyval == "1"));
			std::cout << "MSB: " << msb << std::endl;
		} else if (keyword == "ElementNumberOfChannels") {
			file >> channels;
			std::cout << channels << " channels" << std::endl;
			std::getline(file, keyval, '\n');
		} else if (keyword == "ElementType") {
			std::getline(file, keyval, '\n');
			it=keyval.end();
			keyval.erase(std::remove(keyval.begin(),keyval.end(),' '),it);
			// remove remaining DOS line breaks '\r'
			// FIXME: might not work for MAC line breaks
			it=keyval.end();
			keyval.erase(std::remove(keyval.begin(),keyval.end(),'\r'),it);
			if (keyval == "MET_UCHAR") img = new ImageBase<unsigned char>();
			else if (keyval == "MET_CHAR") img = new ImageBase<char>();
			else if (keyval == "MET_USHORT") img = new ImageBase<unsigned short>();
			else if (keyval == "MET_SHORT") img = new ImageBase<short>();
			else if (keyval == "MET_UINT") img = new ImageBase<unsigned int>();
			else if (keyval == "MET_INT") img = new ImageBase<int>();
			else if (keyval == "MET_FLOAT") img = new ImageBase<float>();
			else if (keyval == "MET_DOUBLE") img = new ImageBase<double>();
			else {
				std::cout << "Error: Unknown MetaImage data type!" << std::endl;
				return NULL;
			}
			std::cout << "Data Type " << img->getDataType() << std::endl;
		} else if (keyword == "HeaderSize") {
			file >> header;
			std::getline(file, keyval, '\n');
			if ((header) && (header != -1)) {
				std::cout << "Warning: " << header << " bytes header" << std::endl;
			}
		} else if (keyword == "ElementDataFile") {
#ifdef SUN
			msb = !msb;
#endif
			std::getline(file, keyval, '\n');
			it=keyval.end();
			keyval.erase(std::remove(keyval.begin(),keyval.end(),' '),it);
			// remove remaining DOS line breaks '\r'
			// FIXME: might not work for MAC line breaks
			it=keyval.end();
			keyval.erase(std::remove(keyval.begin(),keyval.end(),'\r'),it);
			sprintf(rawpath, "%s/%s", dirname(filename2), keyval.c_str());
			std::cout << "Loading file " << rawpath << std::endl;
			img->init(dim[0], dim[1], dim[2], dim[3], channels);
			img->loadData(rawpath);
			if (msb) img->swapBytes();
		} else {
			if (keyword != "")
				std::cout << "Warning: Unknown keyword \"" << keyword << "\"" << std::endl;
			std::getline(file, keyval, '\n');
		}
	}
	img->setPhysicalSize(spacing[0], spacing[1], spacing[2], spacing[3]);
	img->computeMinMax();
	std::cout << "Loaded Image, resolution " << dim[0] << " x " << dim[1] << " x " << dim[2]
	<< ", min " <<img->getMin() << ", max = " << img->getMax() << std::endl;

	file.close();
	return img;
	
	// This was the old version of the image loader...
/*	FILE *file = fopen(filename, "r");
	if (!file) return NULL;
	char type[256], rawfile[256], rawpath[256];
	if (fscanf(file, "NDims = %i\n", &dims) != 1) { fclose(file); return NULL; }
	if (fscanf(file, "DimSize = %i %i %i %i\n", &dimx, &dimy, &dimz, &dimt) != dims) { fclose(file); return NULL; }
	if (fscanf(file, "ElementSpacing = %f %f %f\n", &sx, &sy, &sz) != dims) { fclose(file); return NULL; }
	std::cout << "Spacing " << sx << " x " << sy << " x " << sz << std::endl;
	fscanf(file, "Position = %f %f %f\n", &px, &py, &pz);
	fscanf(file, "ElementByteOrderMSB = %s\n", type);
//	if (fscanf(file, "ElementNumberOfChannels = %i\n", &channels) != 1) channels = 1;
	std::cout << "Dimensions " << dimx << " x " << dimy << " x " << dimz
	<< " x " << dimt << " (" << channels << " channels)" << std::endl;
	if (fscanf(file, "ElementType = %s\n", type) != 1) { fclose(file); return NULL; }
	fscanf(file, "HeaderSize = %i\n", &header);
	if (fscanf(file, "ElementDataFile = %s\n", rawfile) != 1) { fclose(file); return NULL; }
	sprintf(rawpath, "%s/%s", dirname(filename2), rawfile);
	std::cout << "Loading file " << rawpath << std::endl;
	fclose(file);
	if (strcmp(type, "MET_UCHAR") == 0) img = new ImageBase<unsigned char>();
	else if (strcmp(type, "MET_SHORT") == 0) img = new ImageBase<short>();
	else if (strcmp(type, "MET_USHORT") == 0) img = new ImageBase<unsigned short>();
	else if (strcmp(type, "MET_FLOAT") == 0) img = new ImageBase<float>();
	img->init(dimx, dimy, dimz, 1, channels);
	if (img->loadData(rawpath) == 0) {
#ifdef SUN
		img->swapBytes();
#endif
		img->setPhysicalSize(sx, sy, sz);
		img->computeMinMax();
		std::cout << "Loaded Image, resolution " << dimx << " x " << dimy << " x " << dimz
		<< ", min " <<img->getMin() << ", max = " << img->getMax() << std::endl;
		return img;
	}
	// failed
	std::cout << "Loading image failed!" << std::endl;
	delete img;
	return NULL; */
}


int Tools::saveMetaImage(char *filename, Image *img) {
	char filename2[255];
	char *types[8] = {"CHAR", "UCHAR", "SHORT", "USHORT", "INT", "UINT", "FLOAT"};
	sprintf(filename2, "%s.mhd", filename);
	FILE *file = fopen(filename2, "w");
	if (!file) return 1;
	int dims = (img->getDim(3) != 1)? 4: ((img->getDim(2) != 1)? 3: 2);
	fprintf(file, "NDims = %i\n", dims);
	switch (dims) {
		case 2: fprintf(file, "DimSize = %i %i\n", img->getDim(0), img->getDim(1));
				fprintf(file, "ElementSpacing = %f %f\n", img->getSize(0), img->getSize(1));
				fprintf(file, "Position = 0 0\n"); break;
		case 3: fprintf(file, "DimSize = %i %i %i\n", img->getDim(0),
				img->getDim(1), img->getDim(2));
				fprintf(file, "ElementSpacing = %f %f %f\n", img->getSize(0),
				img->getSize(1), img->getSize(2));
				fprintf(file, "Position = 0 0 0\n"); break;
		case 4: fprintf(file, "DimSize = %i %i %i %i\n", img->getDim(0),
				img->getDim(1), img->getDim(2), img->getDim(3));
				fprintf(file, "ElementSpacing = %f %f %f %f\n", img->getSize(0),
				img->getSize(1), img->getSize(2), img->getSize(3));
				fprintf(file, "Position = 0 0 0 0\n"); break;
	}
	fprintf(file, "ElementByteOrderMSB = False\n");
	if (img->getChannels() > 1) fprintf(file, "ElementNumberOfChannels = %i\n", img->getChannels());
	fprintf(file, "ElementType = MET_%s\n", types[img->getDataType()&255]);
	sprintf(filename2, "%s.raw", filename);
	fprintf(file, "ElementDataFile = %s\n", filename2);
	fclose(file);
#ifdef SUN
		img->swapBytes();
#endif
	img->saveData(filename2);
#ifdef SUN
		img->swapBytes();
#endif
	return 0;
}


Image *Tools::createImage(int datatype){
	switch (datatype) {
	case Image::BYTE: return new ImageBase<char>(); break;
	case Image::UNSIGNED_BYTE: return new ImageBase<unsigned char>(); break;
	case Image::SHORT: return new ImageBase<short>(); break;
	case Image::UNSIGNED_SHORT: return new ImageBase<unsigned short>(); break;
	case Image::INT: return new ImageBase<int>(); break;
	case Image::UNSIGNED_INT: return new ImageBase<unsigned int>(); break;
	case Image::FLOAT: return new ImageBase<float>(); break;
	case Image::DOUBLE: return new ImageBase<double>(); break;
	}
	return NULL;
}


void Tools::addFilesInDir(std::string path, std::vector<std::string> &fileList, bool recursive)
{
	DIR* dir = opendir(path.c_str());
	// Check, whether current path is a directory or file
	if (dir)
	{
		struct dirent *entry;
		std::string fullpath = path;
		if( path[3] != '\0')
		{
			// Windows is able to handle slashes "/" as well
			// (along with standard backslashes "\\"),
			// so we don't need to add a backslash here
			fullpath +="/";
		}
		while ((entry = readdir(dir)) != 0)
		{
			if (entry->d_name[0] != '.')
			{
				// Check, whether we want to recursively get files or just the files
				// in the current path
				if (recursive)
                    addFilesInDir(((std::string)fullpath+entry->d_name), fileList, recursive);
				else
				{
					// Check, whether current path is a directory or file
					DIR* tmp_dir = opendir((fullpath+entry->d_name).c_str());
					if (tmp_dir)
						closedir(tmp_dir);
					else
                        fileList.push_back(fullpath+entry->d_name);
				}
			}
		}
		closedir(dir);
	}
	else
	{
		fileList.push_back(path);
	}
}

// creates a Gauss filter as in MatLab (function fspecial)
float *Tools::createGaussFilter( int n1, int n2, float sigma )
{
	float *res = new float[n1*n2];
	float sum = 0.0f;
	for (int i=0; i<n1; i++) {
		for (int j = 0; j<n2; j++) {
            res[i*n2+j] = exp(- ((float)((i*i) + (j*j))) / (2.0f * sigma * sigma));
			sum += res[i*n2+j];
		}
	}
	for (int i=0; i<n1*n2; i++) { res[i] /= sum; }
	return res;
}
// assume: input has 3 channels: for each pixel p: pxx, pxy(=pyx), pyy
// output1 will have 2 channels: for each pixel lambda1, lambda2, 
// output2 will have 2 channels: u1, u2 for the Max Eigenvector u=(u1,u2), where
// Hessain H = VDVt with D = (lambda1    0   )
//							 (    0   lambda2)
// and V = (u,v), where u=(u1,u2)t, v=(v1,v2)t.
// lambda1 will always be the max Eigenvalue.
void Tools::eigenAnalysis2D(Image *input, Image *output1, Image *output2, Image *output3)
{
	if (input->getChannels() != 3)
		return;
	float xx, xy, yy;
	if (output2 != 0) output2->iterate();
	if (output2 != 0) output3->iterate();
	for (input->iterate(), output1->iterate(); input->getNext(&xx), input->getNext(&xy), input->getNext(&yy);) {
		float dxy = xx - yy;
		float sxy = xx + yy;

		float S = sqrt(dxy*dxy + 4.0f*xy*xy);

		float eMax = (sxy + S) / 2.0f;
		float eMin = (sxy - S) / 2.0f;

		output1->setNext(eMax);
		output1->setNext(eMin);

		if (output2 != 0) {
			float u1 = ( -dxy - S) / 2.0f;
			float u2 = -xy;

			//normalize
			float n = sqrt( u1 * u1 + u2 * u2 );
			if (n > 1e-16) {
				u1 = u1 / n;
				u2 = u2 / n;
			}
			else {
				u1 = 0.0f;
				u2 = 0.0f;
			}

			output2->setNext(u1);
			output2->setNext(u2);

		if (output3 != 0) {
				//BETTER and FASTER way?
				//v from constraints utv=0 & |v|=1:
				float v2 = sqrt(u1 * u1 + u2 * u2) / u1;
				float v1 = u2 * v2 / u1;

				//normalize
				float n = sqrt(v1 * v1 + v2 * v2);
				if (n > 1e-16) {
					v1 = v1 / n;
					v2 = v2 / n;
				}
				else {
					v1 = 0.0f;
					v2 = 0.0f;
				}

				output3->setNext(v1);
				output3->setNext(v2);
			}
		}
	}

	output1->computeMinMax();
	if (output2 != 0) output2->computeMinMax();
	if (output3 != 0) output3->computeMinMax();
}


}

#pragma warning (default: 4996)