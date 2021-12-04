#include "Image.h"
#include <algorithm>
#include <iostream>

namespace CAMP {

Image::Image() {
	m_datatype = 0;
	m_wl = 0;
	m_debug = 1;
	m_size[0] = 1.0; // width
	m_size[1] = 1.0; // height
	m_size[2] = 1.0; // slices
	m_size[3] = 1.0; // frames
	m_stride = 0;
	m_slice_size = 0;
	m_frame_size = 0;
	m_image_size = 0;
	m_iteration = 0;
}


Image::~Image() {
	if (m_wl) delete[] m_wl;
}

int Image::init(const int width, const int height, const int slices, const int frames, const int channels) {
	m_dim[0] = width;
	m_dim[1] = height;
	m_dim[2] = slices;
	m_dim[3] = frames;
	m_channels = channels;
	m_stride = width*channels;
	m_slice_size = m_stride * height;
	m_frame_size = m_slice_size*slices;
	m_image_size = m_frame_size*frames;
	if (m_wl) delete[] m_wl;
	if (channels!=0) {
		m_wl = new float[4*channels];
		std::fill(m_wl,m_wl+4*channels,0.0f);
	}
	return 0;
}


void Image::setWindowLevel(const float window, const float level, const int channel) {
	m_wl[4*channel  ] = window;
	m_wl[4*channel+1] = level;
	m_wl[4*channel+2] = level - window * 0.5f;
	m_wl[4*channel+3] = level + window * 0.5f;
}


void Image::setMinMax(const float min, const float max, const int channel) {
	m_wl[4*channel  ] = max - min;
	m_wl[4*channel+1] = (max + min) * 0.5f;
	m_wl[4*channel+2] = min;
	m_wl[4*channel+3] = max;
}


void Image::setPhysicalSize(const float s0, const float s1, const float s2, const float s3) {
	m_size[0] = s0;
	m_size[1] = s1;
	m_size[2] = s2;
	m_size[3] = s3;
}


const float Image::SOBEL2D[9] = {
	1, 2, 1, 0, 0, 0, -1, -2, -1
};

const float Image::SOBEL3D[27] = {
	 1, 1, 1, 1, 2, 1, 1, 1, 1,
	 0, 0, 0, 0, 0, 0, 0, 0, 0,
	-1,-1,-1,-1,-2,-1,-1,-1,-1
};

}
