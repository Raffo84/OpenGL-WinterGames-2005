#ifndef IMAGEBASE_H
#define IMAGEBASE_H

// disable all warning concerning type conversions...
#ifdef _MSC_VER
#pragma warning (disable : 4541)
#pragma warning (disable : 4244)
#pragma warning (disable : 4305)
#endif

#include "AlignedMalloc.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <cmath>
#include <climits>
#include <iostream>
#include <cstdio>

#include "Image.h"
#include "Tools.h"

namespace CAMP {
	/** @brief	Templated base class for multi-modality image data
	*	@author	Wolfgang Wein, Hauke Heibel
	*
	*  \ingroup img
	*
	*  This class uses image data in various dimensions in the same way.
	*  Thus 2D images have one slice and frame, 3D images multiple slices and
	*  one frame, 2D movies one slice and multiple frames, etc.
	*  By using multiple channels the fusion of different image modalities may
	*  be represented in one image, similarily image gradient vectors can be
	*  used.
	*  For specific parameters and further computations on the image data, I
	*  use 32 bit floating-point values. Therefore it makes no sense to use
	*  the template type double with this class!
	*
	*  \todo Add missing comment for function like e.g. invert(), etc.
	*
	*  */
	template <class T> class ImageBase: public Image {
	public:
		ImageBase();

		/**
		 * Copy constructor for images.
		 * \param img Reference to the image to be copied.
		 */
		ImageBase(ImageBase& img);

		virtual ~ImageBase();

		/**
		* This function will initialize an image and allocate the data array 16 byte aligned. Thus for freeing the memory
		* we need special calls that are covered by the destroy() method.
		*
		* \param width The images width.
		* \param height The images height.
		* \param slices The number of slices, when creating a volume.
		* \param frames The number of frames.
		* \param channels The number of channels.
		* \return
		*/
		int init(const int width, const int height, const int slices = 1, const int frames = 1, const int channels = 1);

		int loadData(char *filename);

		int loadRawData(void *data);

		int saveData(char *filename);

		/// Clear image data buffer
		void clear(void);

		/**
		* Since we are allocationg aligned memory for the image data array you have always (!) to call
		* destroy to correctly free the memory.
		*/
		void destroy(void);

		void computeMinMax();

		void swapBytes();


		void swap(T*, int, int);

		void flip(void);
		void flipH(void);
		void flipV(void);


		void flip(bool x = true, bool y = false, bool z = false, bool t = false);

		void flipHorizontal();
		void flipVertical();

		/// Downsample the image a specific value in each dimension
		Image *downSample(const int nx, const int ny, const int nz = 1, const int nt = 1, const bool create = false);

		void rotate(int axis);

		/// Scale one image dimension to the power of two. Add in the other dimension a padding to get it to a power of 2.
		void scalePow2(void);

		void invert();

		void applyWindowLevel(bool invert = false);

		void getSliceImageUByte(ImageBase<byte> *result, int slice = 0, int direction = 0, bool powertwo = true);

		unsigned char *getSliceUByte(unsigned char *buffer = 0, int slice = 0, int channel=0, bool powertwo = true);

		unsigned char *getDataUByte(unsigned char *buffer = 0, bool powertwo = true);

		unsigned short *getDataUShort(unsigned short *buffer = 0, bool powertwo = true);

		/// Computes the center of mass for the volume. One channel/frame only!
		void computeMassCenter(float *result);

		Image *applyFilter2D( int datatype, float *filter, int windowSize, CAMP::Image *filteredImage = NULL);

		void fixEdges(int nPixel);

		Image *computeGradient2D(int datatype = 0, float *filter = NULL, bool border = false, Image *grad = NULL);

		Image *computeHessian2D(int datatype, float *filter = 0, Image *hessian = 0);

		Image *computeGradient3D(int datatype = 0, float *filter = NULL,
		bool border = false, bool fourchannels = false, Image *grad = NULL);

		/// @name Inline functions for member access

		/// Iterate, return as floating point values
		inline bool getNext(float *result);

		/// Iterate, writing the image data
		inline bool setNext(float value);

		inline void setCurrent(float value);

		/**
		* Return pointer to image data.
		* \return Pointer to image data.
		*/
		inline T *getData() { return m_data; }

		/**
		* Return pointer to a specifig line in a specific slice.
		* \param y Specifies the line.
		* \param z Specifies the slice.
		* \return Pointer to the specified line.
		*/
		inline T *getLinePtr(int y, int z = 0) { return m_data+z*m_slice_size+y*m_stride; }

		inline T* getPixelPtr(const int x, const int y);
		inline T* getVoxelPtr(const int x, const int y, const int z, const int t = 0, const int c = 0);

		inline T getPixelValue(const int x, const int y);
		inline T getVoxelValue(const int x, const int y, const int z = 0, const int t = 0, const int c = 0);

		inline void setPixel(const int x, const int y, T value);
		inline void setVoxel(const int x, const int y, const int z, T value);

		/// Set value at specified position
		inline void setVoxelFloat(const float value, const int x, const int y, const int z = 0, const int t = 0, const int c = 0);

		/// Return closest value (also called Nearest Neighbor Interpolation)
		inline float getClosest(const float x, const float y, const float z);

		/// Return voxel value as float data type
		inline float getVoxelFloat(const int x, const int y, const int z = 0, const int t = 0, const int c = 0);

		/// Return trilinearly interpolated value from the volume
		inline float getTrilinear(const float x, const float y, const float z);

		float getTricubic(const float x, const float y, const float z);

		/// Return closest value for all channels
		inline void getClosest(float *result, const float x, const float y, const float z);

		/// Return trilinearly interpolated value for all channels
		inline void getTrilinear(float *result, const float x, const float y, const float z);

		void getTricubic(float *result, const float x, const float y, const float z);

		/// Enables the so called container mode. Previously allocated data will get lost.
		void enableContainerMode(void) { destroy(); m_Protected = true; };
		void disableContainerMode(void) { m_Protected = false; };

		//@}

	protected:
		bool m_Protected;	///< If a image is defined to be protected the image can be used as a container, i.e. it is forbidden to delete any data.
		T *m_data;			///< The actual image data.
	};

	template<class T> ImageBase<T>::ImageBase(): Image() {
		m_data = NULL;
		m_Protected = false;
		// Determine data type. If anybody has a more elegant way to do this, let me know! (WW)
		//dynamic_cast<ImageBase<char> *> (this) m_datatype = BYTE;
		/*else if (dynamic_cast<ImageBase<unsigned char> *> (this)) m_datatype = UNSIGNED_BYTE;
		else if (dynamic_cast<ImageBase<short> *> (this)) m_datatype = SHORT;
		else if (dynamic_cast<ImageBase<unsigned short> *> (this)) m_datatype = UNSIGNED_SHORT;
		else if (dynamic_cast<ImageBase<int> *> (this)) m_datatype = INT;
		else if (dynamic_cast<ImageBase<unsigned int> *> (this)) m_datatype = UNSIGNED_INT;
		else if (dynamic_cast<ImageBase<float> *> (this)) m_datatype = FLOAT;
		else if (dynamic_cast<ImageBase<double> *> (this)) m_datatype = DOUBLE;
		if (m_datatype == 0) std::cout << "Warning: unknown template data type for ImageBase!" << std::endl;*/
	}


	template<class T> ImageBase<T>::ImageBase(ImageBase& img) {
		m_data = NULL;
		m_wl = NULL;
		m_debug = 1;
		m_stride = img.getStride();
		this->init(img.getWidth(), img.getHeight(), img.getSlices(), img.getFrames(), img.getChannels());

		std::memcpy(this->m_data, img.getData(), m_image_size*sizeof(T));
		for (int i=0; i<4; ++i) m_size[i] = img.getSize(i);
	}


	template<class T> ImageBase<T>::~ImageBase() {
		destroy();
	}


	template<class T>
	int ImageBase<T>::init(const int width, const int height, const int slices, const int frames, const int channels) {
		Image::init(width, height, slices, frames, channels);
		destroy();		
		if (m_image_size != 0 && !m_Protected) {

			m_data = static_cast<T*>(aligned_malloc(m_image_size * sizeof(T)));

			if (m_data == NULL) std::cout << "Error while initializing!\n";

			std::fill(m_data,m_data+m_image_size,static_cast<T>(0));
		}
		return 0;
	}


	template<class T>
	int ImageBase<T>::loadData(char *filename) {
#pragma warning (disable: 4996)		// >> 'fopen' was declared deprecated <<
		FILE *file = fopen(filename, "rb");
#pragma warning (default: 4996)
		if (!file) return 1;
		fread(m_data, sizeof(T)*m_dim[0]*m_channels, m_dim[1]*m_dim[2]*m_dim[3], file);
		fclose(file);
		return 0;
	}


	template<class T>
	int ImageBase<T>::loadRawData(void* data) {
		m_data = (T*)data;
		return 0;
	}


	template<class T>
	int ImageBase<T>::saveData(char *filename) {
#pragma warning (disable: 4996)		// >> 'fopen' was declared deprecated <<
		FILE *file = fopen(filename, "wb");
#pragma warning (default: 4996)
		if (!file) return 1;
		fwrite(m_data, sizeof(T)*m_dim[0]*m_channels, m_dim[1]*m_dim[2]*m_dim[3], file);
		fclose(file);
		return 0;
	}


	template<class T>
	void ImageBase<T>::clear(void) {
		if (!m_data) m_data = new T[m_image_size];
		std::fill(m_data,m_data+m_image_size,static_cast<T>(0));
		std::fill(m_wl,m_wl+4*m_channels,0.0f);
	}

	template<class T>
	void ImageBase<T>::destroy(void) {
		if (!m_Protected)
			aligned_free(m_data);		
	}


	template<class T> void ImageBase<T>::computeMinMax() {
		T min, max; int i;
		min = max = m_data[0];
		for (i = 1; i < m_image_size; i++) {
			if (min > m_data[i]) min = m_data[i];
			if (max < m_data[i]) max = m_data[i];
		}
		for (i = 0; i < m_channels; i++)
			setMinMax((float)min, (float)max, i);
	}


	template<class T> void ImageBase<T>::swapBytes() {
		unsigned char *data = (unsigned char *) m_data;
		if (sizeof(T) == 2) {
			for (int i = 0; i < m_image_size; i++) {
				unsigned char tmp = data[2*i];
				data[2*i] = data[2*i+1];
				data[2*i+1] = tmp;
			}
		} else if (sizeof(T) == 4) {
			for (int i = 0; i < m_image_size; i++) {
				unsigned char tmp;
				tmp = data[4*i]; data[4*i] = data[4*i+3]; data[4*i+3] = tmp;
				tmp = data[4*i+1]; data[4*i+1] = data[4*i+2]; data[4*i+2] = tmp;
			}
		}
	}


	template<class T> void ImageBase<T>::flip(bool x, bool y, bool z, bool t) {
		// Lazy stupid solution for now: create a new buffer

		T *buffer = static_cast<T*>(aligned_malloc(m_image_size * sizeof(T)));
		for (int tt = 0; tt < m_dim[3]; tt++) {
			for (int zz = 0; zz < m_dim[2]; zz++) {
				for (int yy = 0; yy < m_dim[1]; yy++) {
					for (int xx = 0; xx < m_dim[0]; xx++) {
						int xxx = x? m_dim[0]-xx-1: xx;
						int yyy = y? m_dim[1]-yy-1: yy;
						int zzz = z? m_dim[2]-zz-1: zz;
						int ttt = t? m_dim[3]-tt-1: tt;
						for (int cc = 0; cc < m_channels; cc++) {
							buffer[cc+xxx*m_channels+yyy*m_stride+zzz*m_slice_size+ttt*m_frame_size] =
							m_data[cc+xx*m_channels+yy*m_stride+zz*m_slice_size+tt*m_frame_size];
						}
					}
				}
			}
		}

		destroy();
		m_data = buffer;
	}


	template<class T> void ImageBase<T>::invert() {
		for (int i = 0; i < m_image_size; i++)
			for (int c = 0; c < m_channels; c++)
				m_data[c+m_channels*i] = (T)(m_wl[4*c+2]+m_wl[4*c+3]-(float)m_data[c+m_channels*i]);
	}


	template<class T>
	void ImageBase<T>::scalePow2(void)
	{
		if (!m_data) return;

		int width2 = m_dim[0];
		int height2 = m_dim[1];

		float tmp;

		tmp = std::log(static_cast<float>(m_dim[0]))/std::log(2.0f);
		if (tmp!=0.0f) width2 = (int)pow(2.0f,std::ceil(tmp));

		tmp = std::log(static_cast<float>(m_dim[1]))/std::log(2.0f);
		if (tmp!=0.0f) height2 = (int)pow(2.0f,std::ceil(tmp));

		//m_x_padding = std::abs(static_cast<int>(m_dim[0]-width2));
		//m_y_padding = std::abs(static_cast<int>(m_dim[1]-height2));

		int stride2 = width2*m_channels;
		int slice_size2 = height2*stride2;
		int img_size2 = m_dim[3]*m_dim[2]*slice_size2;

		T *newdata = static_cast<T*>(aligned_malloc(img_size2 * sizeof(T)));
		std::fill(newdata,newdata+img_size2,static_cast<T>(0));

		for (int z = 0; z < m_dim[2]; ++z) {
			for (int y = 0; y < m_dim[1]; ++y) {
				for (int x = 0; x < m_dim[0]*m_channels; ++x) {
					newdata[x+stride2*y+slice_size2*z] = m_data[x+m_channels*m_dim[0]*(y+m_dim[1]*z)];
				}
				// memmove(&newdata[z*slice_size2+y*stride2],&m_data[z*m_slice_size+y*m_stride],m_stride*sizeof(T));
			}
		}

		m_dim[0] = width2;
		m_dim[1] = height2;

		m_stride = stride2;
		m_slice_size = slice_size2;
		m_frame_size = m_dim[2]*slice_size2;
		m_image_size = img_size2;

		destroy();
		m_data = newdata;
		if (m_debug) std::cout << "Scaled to power of two, new width = "
		<< m_dim[0] << ", height = " << m_dim[1] << std::endl;
	}

	template<class T>
	Image *ImageBase<T>::downSample(const int nx, const int ny, const int nz, const int nt, const bool create) {
		if (m_data == NULL) return NULL;
		ImageBase<T> *newimage = NULL; T *newdata;
		float *newvalue = new float[m_channels];
		int newx = m_dim[0]/nx, newy = m_dim[1]/ny, newz = m_dim[2]/nz, newt = m_dim[3]/nt;
		if (create) {
			newimage = new ImageBase<T>();
			newimage->init(newx, newy, newz, newt, m_channels);
			newdata = newimage->getData();
		} else newdata = static_cast<T*>(aligned_malloc(m_channels*(m_dim[0]/nx)*(m_dim[1]/ny)*(m_dim[2]/nz)*(m_dim[3]/nt) * sizeof(T)));
		for (int t = 0; t < newt; t++) {
			for (int z = 0; z < newz; z++) {
				for (int y = 0; y < newy; y++) {
					for (int x = 0; x < newx; x++) {
						for (int c = 0; c < m_channels; c++) {
							newvalue[c] = 0.0;
							for (int tt = 0; tt < nt; tt++) {
								for (int zz = 0; zz < nz; zz++) {
									for (int yy = 0; yy < ny; yy++) {
										for (int xx = 0; xx < nx; xx++) {
											newvalue[c] += static_cast<float>(getVoxelValue(x*nx+xx, y*ny+yy, z*nz+zz, t*nt+tt, c));
										}
									}
								}
							}
							newvalue[c] /= (float)(nx*ny*nz*nt);
							newdata[c+m_channels*(x+newx*(y+newy*(z+newz*t)))] = (T) newvalue[c];
						}
					}
				}
			}
		}
		delete [] newvalue;
		if (create) {
			newimage->setPhysicalSize(m_size[0]*nx, m_size[1]*ny, m_size[2]*nz, m_size[3]*nt);
		} else {
			destroy();
			m_data = newdata;
			m_dim[0] = newx; m_dim[1] = newy; m_dim[2] = newz; m_dim[3] = newt;
			m_size[0] *= nx; m_size[1] *= ny; m_size[2] *= nz; m_size[3] *= nt;
			m_stride = newx * m_channels;
			m_slice_size = m_stride * newy;
			m_frame_size = m_slice_size * newz;
			m_image_size = m_frame_size * newt;
			if (m_debug) std::cout << "Image downsampled, new size "
				<< m_dim[0] << " x " << m_dim[1] << " x " << m_dim[2] << std::endl;
		}
		return newimage;
	}

	template<class T> void ImageBase<T>::rotate(int axis) {
		int addr = 0, addr2 = 0;

		T *data2 = static_cast<T*>(aligned_malloc(m_image_size * sizeof(T)));
		for (int z = 0; z < m_dim[2]; z++) {
			for (int y = 0; y < m_dim[1]; y++) {
				for (int x = 0; x < m_dim[0]; x++) {
					switch (axis) {
						case 0: addr = m_channels * (x + m_dim[0] * (z + m_dim[2] * (m_dim[1]-y-1))); break;
						case 1: addr = m_channels * ((m_dim[2]-z-1) + m_dim[2] * (y + m_dim[1] * x)); break;
						case 2: addr = m_channels * (y + m_dim[1] * (m_dim[0]-x-1 + m_dim[0] * z)); break;
					}
					for (int c = 0; c < m_channels; c++) {
						data2[addr+c] = m_data[addr2+c];
					}
					addr2 += m_channels;
				}
			}
		}
		destroy();
		m_data = data2;
		int i1 = 0, i2 = 1;
		switch (axis) {
			case 0: i1 = 1; i2 = 2; break;
			case 1: i1 = 0; i2 = 2; break;
			case 2: i1 = 0; i2 = 1; break;
		}
		int t = m_dim[i1]; m_dim[i1] = m_dim[i2]; m_dim[i2] = t;
		float s = m_size[i1]; m_size[i1] = m_size[i2]; m_size[i2] = s;
		if (m_debug) std::cout << "Rotated image around axis " << axis << std::endl;
	}

	//can also be used for a 3D image - then, all slices are filtered with a 2D filter.
	template<class T> 
	Image *ImageBase<T>::applyFilter2D( int datatype, float *filter, int n, CAMP::Image *filteredImage )
	{
		if (datatype&1) {
			std::cout << "Error: unsigned data type, cannot compute filtered image!" << std::endl;
			return NULL;
		}
		if (filter == NULL) {
			std::cout << "Error: Cannot compute filtered image without filter!" << std::endl;
			return NULL;
		}
		if (filteredImage == NULL) {
			filteredImage = Tools::createImage(datatype);
			filteredImage->init(m_dim[0], m_dim[1], m_dim[2], m_dim[3], 1);
			filteredImage->setPhysicalSize(m_size[0], m_size[1], m_size[2], m_size[3]);
		}
		if (n%2==0) {
			std::cout << "Error: Window size must be an odd number!" << std::endl;
			return NULL;
		}

		int pOff = (n-1)/2;			//offset of pixels from middle pixel in filter
		float min = 1e16f, max = -1e16f;
		filteredImage->iterate();
		for (int z = 0; z < m_dim[2]; z++) {
			for (int y = 0; y < m_dim[1]; y++) {
				for (int x = 0; x < m_dim[0]; x++) {
					float sx = 0.0f;
					if ((x > pOff-1) && (x < m_dim[0]-pOff) && (y > pOff-1) && (y < m_dim[1]-pOff)) {
						for (int a = -pOff; a < pOff+1; a++) {
							for (int b = -pOff; b < pOff+1; b++) {
								if (filter[(a+pOff)*n + (b+pOff)]) {
									sx += filter[(a+pOff)*n + (b+pOff)] * m_data[x+b + m_dim[0]*(y+a) + m_dim[1]*m_dim[0]*z];
								}
							}
						}
					}
					// update min/max gradient value
					if (min > sx) min = sx; if (max < sx) max = sx;
					filteredImage->setNext(sx);
				}
			}
		}
		//fix the edges of the convolved image:
		filteredImage->fixEdges(pOff);
		filteredImage->setMinMax(min, max, 0);

		return filteredImage;
	}

	template<class T>
	void ImageBase<T>::fixEdges( int n )
	{
		//corners:
		for (int z = 0; z < m_dim[2]; z++) {
			for (int i = n; i > 1; i--) {
				m_data[z*m_dim[1]*m_dim[0] + (i-1)*m_dim[0] + (i-1)] = m_data[z*m_dim[1]*m_dim[0] + i*m_dim[0] + i];
				m_data[z*m_dim[1]*m_dim[0] + (m_dim[1]-1-(i-1))*m_dim[0] + (i-1)] = m_data[z*m_dim[1]*m_dim[0] + (m_dim[1]-1-1-(i-1))*m_dim[0] + i];
				m_data[z*m_dim[1]*m_dim[0] + (i-1)*m_dim[0] + (m_dim[0]-(i-1))] = m_data[z*m_dim[1]*m_dim[0] + i*m_dim[0] + (m_dim[0]-1-(i-1))];
				m_data[z*m_dim[1]*m_dim[0] + (m_dim[1]-(i-1))*m_dim[0] + (m_dim[0]-(i-1))] = m_data[z*m_dim[1]*m_dim[0] + (m_dim[1]-1-(i-1))*m_dim[0] + (m_dim[0]-1-(i-1))];
			}
		}
		//y-direction:
		for (int z = 0; z < m_dim[2]; z++) {
			for (int y=0; y<m_dim[1]; y++) {
				for (int j = n; j > 1; j--) {
					m_data[z*m_dim[1]*m_dim[0] + y*m_dim[0] + (j-1)] = m_data[z*m_dim[1]*m_dim[0] + y*m_dim[0] + j];
					m_data[z*m_dim[1]*m_dim[0] + y*m_dim[0] + (m_dim[0] - j)] = m_data[z*m_dim[1]*m_dim[0] + y*m_dim[0] + (m_dim[0] - j - 1)];
				}
			}
		}
		//x-direction:
		for (int z = 0; z < m_dim[2]; z++) {
			for (int x=0; x<m_dim[0]; x++) {
				for (int j = n; j > 1; j--) {
					m_data[z*m_dim[1]*m_dim[0] + (j-1)*m_dim[0] + x] = m_data[z*m_dim[1]*m_dim[0] + j*m_dim[0] + x];
					m_data[z*m_dim[1]*m_dim[0] + (m_dim[1]-j)*m_dim[0] + x] = m_data[z*m_dim[1]*m_dim[0] + (m_dim[1]-j-1)*m_dim[0] + x];
				}
			}
		}
	}

	template<class T> Image *ImageBase<T>::computeGradient2D(int datatype, float *filter, bool border, Image *grad) {
		if (datatype == 0) datatype = m_datatype;
		if (filter == NULL) filter = const_cast<float*>(SOBEL2D);
		if (datatype&1) {
			std::cout << "Error: unsigned data type, cannot compute gradient!" << std::endl;
			return NULL;
		}
		int nb = border? 0: 1;
		if (grad == NULL) {
			grad = Tools::createImage(datatype);
			grad->init(m_dim[0]-2*nb, m_dim[1]-2*nb, m_dim[2], 1, 2);
			grad->setPhysicalSize(m_size[0], m_size[1], m_size[2], m_size[3]);
		}
		grad->iterate();
		float min = 1e16, max = -1e16;
		for (int z = 0; z < m_dim[2]; z++) {
			for (int y = nb; y < m_dim[1]-nb; y++) {
				for (int x = nb; x < m_dim[0]-nb; x++) {
					float sx = 0.0f, sy = 0.0f;
					if ((!border) || ((x > 0) && (x < m_dim[0]-1)
					&& (y > 0) && (y < m_dim[1]-1))) {
						for (int i = 0; i < 9; i++) {
							if (filter[i]) {
								int u = i % 3 - 1;
								int v = i / 3 - 1;
								sx += filter[i] * m_data[x+u+m_dim[0]*(y+v)];
								sy += filter[i] * m_data[x+v+m_dim[0]*(y+u)];
							}
						}
					}
					// update min/max gradient value
					if (min > sx) min = sx; if (min > sy) min = sy;
					if (max < sx) max = sx; if (max < sy) max = sy;
					grad->setNext(sx);
					grad->setNext(sy);
				}
			}
		}
		grad->setMinMax(min, max, 0);
		grad->setMinMax(min, max, 1);
		return grad;
	}

	template<class T> Image *ImageBase<T>::computeHessian2D(int datatype, float *filter, Image *hessian) {
		if (datatype == 0) datatype = m_datatype;
		if (filter == NULL) filter = const_cast<float*>(SOBEL2D);
		if (datatype&1) {
			std::cout << "Error: unsigned data type, cannot compute hessian!" << std::endl;
			return NULL;
		}
		if (hessian == NULL) {
			hessian = Tools::createImage(datatype);
			hessian->init(m_dim[0], m_dim[1], m_dim[2], 1, 3); //3 channels for xx, xy, yy
			hessian->setPhysicalSize(m_size[0], m_size[1], m_size[2], m_size[3]);
		}
		if (m_channels == 1) { //original image - compute gradient first
			Image *grad = computeGradient2D(m_datatype, filter, true);
			grad->computeHessian2D(m_datatype, filter, hessian);
			return hessian;
		}

		hessian->iterate();
		float min1 = 1e16, min2 = 1e16, min3 = 1e16, max1 = -1e16, max2 = -1e16, max3 = -1e16;
		for (int z = 0; z < m_dim[2]; z++) {
			for (int y = 0; y < m_dim[1]; y++) {
				for (int x = 0; x < m_dim[0]; x++) {
					float sxx = 0.0f, sxy = 0.0f, syy = 0.0f;
					if (((x > 0) && (x < m_dim[0]-1)
					&& (y > 0) && (y < m_dim[1]-1))) {
						for (int i = 0; i < 9; i++) {
							if (filter[i]) {
								int u = i % 3 - 1;
								int v = i / 3 - 1;
								//channel 1:
								sxx += filter[i] * m_data[(x+u)*m_channels+m_channels*m_dim[0]*(y+v)];
								sxy += filter[i] * m_data[(x+v)*m_channels+m_channels*m_dim[0]*(y+u)];
								//channel 2:
								syy += filter[i] * m_data[1 + (x+v)*m_channels+m_channels*m_dim[0]*(y+u)];
							}
						}
					}
					// update min/max hessian value
					if (min1 > sxx) min1 = sxx; if (min2 > sxy) min2 = sxy;if (min3 > syy) min3 = syy;
					if (max1 < sxx) max1 = sxx; if (max2 < sxy) max2 = sxy; if (max3 < syy) max3 = syy;
					hessian->setNext(sxx);
					hessian->setNext(sxy);
					hessian->setNext(syy);
				}
			}
		}					
		hessian->setMinMax(min1, max1, 0);
		hessian->setMinMax(min2, max2, 1);
		hessian->setMinMax(min3, max3, 2);
		return hessian;
	}

	template<class T> Image *ImageBase<T>::computeGradient3D(int datatype, float *filter, bool border, bool fourchannels, Image *grad) {
		if (datatype == 0) datatype = m_datatype;
		if (filter == NULL) filter = const_cast<float*>(SOBEL3D);
		if (datatype&1) {
			std::cout << "Error: unsigned data type, cannot compute gradient!" << std::endl;
			return NULL;
		}
		// define maximum range for 8 and 16 bit data types
		float clampmin, clampmax;
		switch (datatype) {
			case BYTE:  clampmin = CHAR_MIN; clampmax = CHAR_MAX; break;
			case SHORT: clampmin = SHRT_MIN; clampmax = SHRT_MAX; break;
			default: clampmin = 0.0f; clampmax = 0.0f;
		}
		int nb = border? 0: 1;
		int nc = fourchannels? 4: 3;
		int bailout = 0;
		if (grad == NULL) {
			grad = Tools::createImage(datatype);
			grad->init(m_dim[0]-2*nb, m_dim[1]-2*nb, m_dim[2]-2*nb, 1, nc);
			grad->setPhysicalSize(m_size[0], m_size[1], m_size[2], m_size[3]);
		}
		grad->iterate();
		float min = 1e16, max = -1e16;
		if (m_debug) std::cout << "Computing 3D gradient..." << std::endl;
		for (int z = nb; z < m_dim[2]-nb; z++) {
			for (int y = nb; y < m_dim[1]-nb; y++) {
				for (int x = nb; x < m_dim[0]-nb; x++) {
					float sx = 0.0f, sy = 0.0f, sz = 0.0f;
					if ((!border) || ((x > 0) && (x < m_dim[0]-1) && (y > 0)
					&& (y < m_dim[1]-1) && (z > 0) && (z < m_dim[2]-1))) {
						for (int i = 0; i < 27; i++) {
							if (filter[i]) {
								int u = i % 3 - 1;
								int v = (i % 9) / 3 - 1;
								int w = i / 9 - 1;
								sx += filter[i] * m_data[x+w+m_dim[0]*((y+v)+m_dim[1]*(z+u))];
								sy += filter[i] * m_data[x+u+m_dim[0]*((y+w)+m_dim[1]*(z+v))];
								sz += filter[i] * m_data[x+u+m_dim[0]*((y+v)+m_dim[1]*(z+w))];
							}
						}
					}
					// fit values to template type range
					if (clampmin) {
						if (sx < clampmin) { sx = clampmin; bailout++; }
						if (sy < clampmin) { sy = clampmin; bailout++; }
						if (sz < clampmin) { sz = clampmin; bailout++; }
						if (sx > clampmax) { sx = clampmax; bailout++; }
						if (sy > clampmax) { sy = clampmax; bailout++; }
						if (sz > clampmax) { sz = clampmax; bailout++; }
					}
					grad->setNext(sx);
					grad->setNext(sy);
					grad->setNext(sz);
					// set original image intensity as fourth channel
					if (fourchannels) grad->setNext(m_data[x+m_dim[0]*(y+m_dim[1]*z)]);
					// update min/max gradient value
					if (min > sx) min = sx; if (min > sy) min = sy; if (min > sz) min = sz;
					if (max < sx) max = sx; if (max < sy) max = sy; if (max < sz) max = sz;
				}
			}
		}
		// set the min/max values as range for the gradient volume
		for (int i = 0; i < 3; i++) grad->setMinMax(min, max, i);
		// Inherit the range for intensity channel
		if (fourchannels) grad->setMinMax(m_wl[2], m_wl[3], 3);
		if (m_debug) std::cout << "Done. Range = [" << min << ".." << max << "]" << std::endl;
		if (bailout) std::cout << "Warning: " << bailout << " values exceeded." << std::endl;
	//	<< bailout << " values exceeded." << std::endl;
		return grad;
	}



	template<class T> void ImageBase<T>::applyWindowLevel(bool invert) {
		for (int i = 0; i < m_image_size; i++) {
			if (m_data[i] < m_wl[2]) m_data[i] = (T) m_wl[2];
			if (m_data[i] > m_wl[3]) m_data[i] = (T) m_wl[3];
			m_data[i] = (invert)? (T)m_wl[3] - m_data[i]: m_data[i] - (T)m_wl[2];
		}
		if (m_debug) std::cout << "Window/Level setting applied" << std::endl;
	}


	template<class T> inline bool ImageBase<T>::getNext(float *result) {
		if (m_iteration < m_image_size) {
			result[0] = (float)m_data[m_iteration++];
			return true;
		}
		return false;
	}


	template<class T> inline bool ImageBase<T>::setNext(float value) {
		if (m_iteration < m_image_size) {
			m_data[m_iteration++] = (T)value;
			return true;
		}
		return false;
	}

	template<class T> void ImageBase<T>::getSliceImageUByte(ImageBase<byte> *result, int slice, int direction, bool powertwo)
	{
		int nx = (powertwo)? Tools::nextPowerTwo(m_dim[0]): m_dim[0];
		int ny = (powertwo)? Tools::nextPowerTwo(m_dim[1]): m_dim[1];

		result->init(nx, ny, 1, 1, m_channels);
		byte *buffer = result->getData();

		// Note: the window level is supposed to be the same for every channel...

		slice = slice % m_dim[2];
		for (int i = 0; i < m_slice_size; i++) {
			float value = (int)(((((float)m_data[m_dim[0] * m_dim[1] * slice + i]
			- m_wl[2])) * 255.0f) / (m_wl[3] - m_wl[2]));
			if (value < 0.0f) value = 0.0f;
			if (value > 255.0f) value = 255.0f;
			// the following two lines use a stride being a power of two
			int j = (nx == m_dim[0])? i: i % m_dim[0] + (i / m_dim[0]) * nx;
			if (value!=0)
				buffer[j] = (unsigned char) value;
		}
	}

	template<class T> inline void ImageBase<T>::setCurrent(float value) {
		m_data[m_iteration-1] = (T)value;
	}


	template<class T> unsigned char *ImageBase<T>::getSliceUByte(unsigned char *buffer, int slice, int channel, bool powertwo) {
		int nx = (powertwo)? Tools::nextPowerTwo(m_dim[0]): m_dim[0];
		int ny = (powertwo)? Tools::nextPowerTwo(m_dim[1]): m_dim[1];
		// if no pointer is provided, create new one
		if (buffer == NULL) buffer = new unsigned char[nx*ny];
		if (buffer == NULL) return NULL;
		slice = slice % m_dim[2];
		// with count only the relevant values for a specific channel are visited
		int count = channel;
		for (int i = 0; i < m_dim[0] * m_dim[1]; i++) {
			float value = (int)(((((float)m_data[m_dim[0] * m_dim[1] * slice + count]
			- m_wl[4*channel+2])) * 255.0f) / (m_wl[4*channel+3] - m_wl[4*channel+2]));
			count += m_channels;
			if (value < 0.0f) value = 0.0f;
			if (value > 255.0f) value = 255.0f;
			// the following two lines use a stride being a power of two
			int j = (nx == m_dim[0])? i: i % m_dim[0] + (i / m_dim[0]) * nx;
			buffer[j] = (unsigned char) value;
		}
		return buffer;
	}


	template<class T> unsigned char *ImageBase<T>::getDataUByte(unsigned char *buffer, bool powertwo) {
		int nx = (powertwo)? Tools::nextPowerTwo(m_dim[0]): m_dim[0];
		int ny = (powertwo)? Tools::nextPowerTwo(m_dim[1]): m_dim[1];
		int nz = (powertwo)? Tools::nextPowerTwo(m_dim[2]): m_dim[2];
		// if no pointer is provided, create new one
		if (buffer == NULL) buffer = new unsigned char[nx*ny*nz*m_channels];
		if (buffer == NULL) return NULL;
		int i;
		// clear buffer
		for (i = 0; i < nx*ny*nz*m_channels; i++) buffer[i] = 0;
		i = 0;
		// fill it with image data
		for (int z = 0; z < m_dim[2]; z++) {
			for (int y = 0; y < m_dim[1]; y++) {
				for (int x = 0; x < m_dim[0]; x++) {
					for (int c = 0; c < m_channels; c++) {
						float value = 255.0f * (static_cast<float>(m_data[i++])-m_wl[4*c+2])/(m_wl[4*c+3]-m_wl[4*c+2]);
						if (value < 0.0f) value = 0.0f;
						if (value > 255.0f) value = 255.0f;
						buffer[m_channels*(x+nx*(y+ny*z))+c] = static_cast<unsigned char>(value);
					}
				}
			}
		}
		return buffer;
	}


	template<class T> unsigned short *ImageBase<T>::getDataUShort(unsigned short *buffer, bool powertwo) {
		int nx = (powertwo)? Tools::nextPowerTwo(m_dim[0]): m_dim[0];
		int ny = (powertwo)? Tools::nextPowerTwo(m_dim[1]): m_dim[1];
		int nz = (powertwo)? Tools::nextPowerTwo(m_dim[2]): m_dim[2];
		// if no pointer is provided, create new one
		if (buffer == NULL) buffer = new unsigned short[nx*ny*nz*m_channels];
		if (buffer == NULL) return NULL;
		int i;
		// clear buffer
		for (i = 0; i < nx*ny*nz*m_channels; i++) buffer[i] = 0;
		i = 0;
		// fill it with image data
		for (int z = 0; z < m_dim[2]; z++) {
			for (int y = 0; y < m_dim[1]; y++) {
				for (int x = 0; x < m_dim[0]; x++) {
					for (int c = 0; c < m_channels; c++) {
						float value = 65535.0f * (static_cast<float>(m_data[i++])-m_wl[4*c+2])/(m_wl[4*c+3]-m_wl[4*c+2]);
						if (value < 0.0f) value = 0.0f;
						if (value > 65535.0f) value = 65535.0f;
						buffer[m_channels*(x+nx*(y+ny*z))+c] = static_cast<unsigned short>(value);
					}
				}
			}
		}
		return buffer;
	}


	template<class T> void ImageBase<T>::computeMassCenter(float *result) {
		double sx = 0.0, sy = 0.0, sz = 0.0, count = 0.0;
		for (int z = 0; z < m_dim[2]; z++) {
			for (int y = 0; y < m_dim[1]; y++) {
				for (int x = 0; x < m_dim[0]; x++) {
					double value = (double)m_data[x+m_dim[0]*(y+m_dim[1]*z)] - m_wl[2];
					sx += value * x;
					sy += value * y;
					sz += value * z;
					count += value;
				}
			}
		}
		result[0] = (float) (sx / count);
		result[1] = (float) (sy / count);
		result[2] = (float) (sz / count);
	}


	template<class T>
	inline T* ImageBase<T>::getPixelPtr(const int x, const int y) {
		assert(x*m_channels + y*m_stride >= 0); // bounds check
		assert(x*m_channels + y*m_stride < m_image_size); // bounds check

		return m_data + x*m_channels + y*m_stride;
	}

	template<class T>
	inline T* ImageBase<T>::getVoxelPtr(const int x, const int y, const int z, const int t, const int c) {
		assert(m_data + c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size >= m_data); // bounds check
		assert(c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size < m_image_size); // bounds check

		return m_data + c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size;
	}

	template<class T>
	inline T ImageBase<T>::getVoxelValue(const int x, const int y, const int z, const int t, const int c) {
		assert(m_data + c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size >= m_data); // bounds check
		assert(c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size < m_image_size); // bounds check

		return m_data[c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size];
	}

	template<class T>
	inline T ImageBase<T>::getPixelValue(const int x, const int y) {
		assert(y*m_stride+x*m_channels >= 0); // bounds check
		assert(y*m_stride+x*m_channels < m_image_size); // bounds check
				
		return m_data[y*m_stride+x*m_channels];
	}

	template<class T>
	inline void ImageBase<T>::setPixel(const int x, const int y, T value)
	{
		assert(y*m_stride + x*m_channels >= 0); // bounds check
		assert(y*m_stride + x*m_channels < m_image_size); // bounds check

		m_data[y*m_stride+x*m_channels] = value;
	}

	template<class T>
	inline void ImageBase<T>::setVoxel(const int x, const int y, const int z, T value)
	{
		assert(m_data + x*m_channels + y*m_stride + z*m_slice_size >= m_data); // bounds check
		assert(x*m_channels + y*m_stride + z*m_slice_size < m_image_size); // bounds check

		m_data[x*m_channels + y*m_stride + z*m_slice_size] = value;
	}

	template<class T>
	inline void ImageBase<T>::setVoxelFloat(const float value, const int x, const int y, const int z, const int t, const int c)
	{
		assert(m_data + c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size >= m_data); // bounds check
		assert(c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size < m_image_size); // bounds check

		m_data[c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size] = static_cast<T>(value);
	}

	// Fix (by Hauke): This will not return the closest... static_cast<int>(a) will allways truncate a
	template<class T>
	inline float ImageBase<T>::getClosest(const float x, const float y, const float z) {		
		return static_cast<float>(m_data[static_cast<int>(z)*m_slice_size+static_cast<int>(y)*m_stride+static_cast<int>(x)*m_channels]);
	}

	template<class T>
	inline float ImageBase<T>::getVoxelFloat(const int x, const int y, const int z, const int t, const int c) {		
		assert(m_data + c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size >= m_data); // bounds check
		assert(c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size < m_image_size); // bounds check

		return static_cast<float>(m_data[c + x*m_channels + y*m_stride + z*m_slice_size + t*m_frame_size]);
	}

	template<class T>
	inline float ImageBase<T>::getTrilinear(const float x, const float y, const float z) {
		int xx = static_cast<int>(x), yy = static_cast<int>(y), zz = static_cast<int>(z);
		float x2 = x - xx, y2 = y - yy, z2 = z - zz;
		return static_cast<float>(m_data[xx  +m_dim[0]*((yy  )+m_dim[1]*(zz  ))] * (1-x2) * (1-y2) * (1-z2)
			+ m_data[xx+1+m_dim[0]*((yy  )+m_dim[1]*(zz  ))] *   x2   * (1-y2) * (1-z2)
			+ m_data[xx  +m_dim[0]*((yy+1)+m_dim[1]*(zz  ))] * (1-x2) *   y2   * (1-z2)
			+ m_data[xx  +m_dim[0]*((yy  )+m_dim[1]*(zz+1))] * (1-x2) * (1-y2) *   z2
			+ m_data[xx+1+m_dim[0]*((yy  )+m_dim[1]*(zz+1))] *   x2   * (1-y2) *   z2
			+ m_data[xx  +m_dim[0]*((yy+1)+m_dim[1]*(zz+1))] * (1-x2) *   y2   *   z2
			+ m_data[xx+1+m_dim[0]*((yy+1)+m_dim[1]*(zz  ))] *   x2   *   y2   * (1-z2)
			+ m_data[xx+1+m_dim[0]*((yy+1)+m_dim[1]*(zz+1))] *   x2   *   y2   *   z2);
	}

	template<class T>
	float ImageBase<T>::getTricubic(const float x, const float y, const float z) {
		float result;
		getTricubic(&result, x, y, z);
		return result;
	}


	template<class T>
	inline void ImageBase<T>::getClosest(float *result, const float x, const float y, const float z) {
	//	assert( ((int)x)+m_dim[0]*(((int)y)+m_dim[1]*((int)z)) ==  static_cast<int>(z)*m_slice_size+static_cast<int>(y)*m_stride+static_cast<int>(x)*m_channels);
		for (int i = 0; i < m_channels; i++) result[i] = (float)
		m_data[i+static_cast<int>(z)*m_slice_size+static_cast<int>(y)*m_stride+static_cast<int>(x)*m_channels];
	}


	template<class T>
	inline void ImageBase<T>::getTrilinear(float *result, const float x, const float y, const float z) {
		int xx = static_cast<int>(x), yy = static_cast<int>(y), zz = static_cast<int>(z);
		float x2 = x - xx, y2 = y - yy, z2 = z - zz;
		assert((xx < m_dim[0]-1) && (yy < m_dim[1]-1) && (zz < m_dim[2]-1));
		for (int i = 0; i < m_channels; i++) result[i] =
			(float)m_data[i+m_channels*(xx  +m_dim[0]*((yy  )+m_dim[1]*(zz  )))] * (1-x2) * (1-y2) * (1-z2)
			+ (float)m_data[i+m_channels*(xx+1+m_dim[0]*((yy  )+m_dim[1]*(zz  )))] *   x2   * (1-y2) * (1-z2)
			+ (float)m_data[i+m_channels*(xx  +m_dim[0]*((yy+1)+m_dim[1]*(zz  )))] * (1-x2) *   y2   * (1-z2)
			+ (float)m_data[i+m_channels*(xx  +m_dim[0]*((yy  )+m_dim[1]*(zz+1)))] * (1-x2) * (1-y2) *   z2
			+ (float)m_data[i+m_channels*(xx+1+m_dim[0]*((yy  )+m_dim[1]*(zz+1)))] *   x2   * (1-y2) *   z2
			+ (float)m_data[i+m_channels*(xx  +m_dim[0]*((yy+1)+m_dim[1]*(zz+1)))] * (1-x2) *   y2   *   z2
			+ (float)m_data[i+m_channels*(xx+1+m_dim[0]*((yy+1)+m_dim[1]*(zz  )))] *   x2   *   y2   * (1-z2)
			+ (float)m_data[i+m_channels*(xx+1+m_dim[0]*((yy+1)+m_dim[1]*(zz+1)))] *   x2   *   y2   *   z2;
	}


	template<class T>
	void ImageBase<T>::getTricubic(float *result, const float x, const float y, const float z) {
		int xx = static_cast<int>(x), yy = static_cast<int>(y), zz = static_cast<int>(z);
		float weight[12], h[3]; int i;
		h[0] = x - xx; h[1] = y - yy; h[2] = z - zz;
		xx -= 1; yy -= 1; zz -= 1;
		for (i = 0; i < 3; i++) {
			weight[4*i] = h[i]*(h[i]*(2.0f-h[i])-1.0f);
			weight[4*i+1] = 1.0f+h[i]*h[i]*(h[i]-2);
			weight[4*i+2] = h[i]*(1.0f+h[i]*(1.0f-h[i]));
			weight[4*i+3] = h[i]*h[i]*(h[i]-1.0f);
		}
		for (i = 0; i < m_channels; i++) result[i] = 0.0f;
		for (int w = 0; w < 4; w++) {
			for (int v = 0; v < 4; v++) {
				for (int u = 0; u < 4; u++) {
					float factor = weight[u]*weight[4+v]*weight[8+w];
					for (i = 0; i < m_channels; i++)
						result[i] += (float)m_data[i+m_channels*(xx+u+m_dim[0]*((yy+v)+m_dim[1]*(zz+w)))]*factor;
				}
			}
		}
	}

	// **********************************************************************************************************
	// es soll so viel wie moeglich von unserer Implementierung uebernommen werden, daher wird fuer das
	// flippen extra die Image Struktur erstellt
	
	struct myImage;
	struct mypoint;

	void horizontalFlip(myImage bild);
	void verticalFlip(myImage bild);


	#define POSITION(x, y, breite) y * breite + x 



	struct mypoint
	{
		int m_x;
		int m_y;
		mypoint(int x, int y)	{m_x = x; m_y = y;}
	};

	struct  myImage
	{
		int m_breite;
		int m_hoehe;
		byte *m_pData;
		myImage(int b, int h, byte* data)	{m_breite = b; m_hoehe = h; m_pData = data; }
	};

	


	template<class T> void ImageBase<T>::swap(T* pData, int iIndex1, int iIndex2)
	{
		int iTemp;

		iTemp = pData[iIndex1];
		pData[iIndex1] = pData[iIndex2];
		pData[iIndex2] = iTemp;

		return;
	}

	template<class T> void ImageBase<T>::flip(void)
	{
		flipH();
		flipV();

		return;
	}

	template<class T> void ImageBase<T>::flipH(void)
	{
		int iH, iW;
		int i, n;

		iW = m_dim[0];
		iH = m_dim[1];

		for(i=0; i<iW; i++)
			for(n=0; n<iH/2; n++)
				swap(m_data, n*iW+i, (iH-(n+1))*iW+i);
		
		
		return;
	}

	template<class T> void ImageBase<T>::flipV(void)
	{
		int iH, iW;
		int i, n;

		iW = m_dim[0];
		iH = m_dim[1];

		for(i=0; i<iH; i++)
			for(n=0; n<iW/2; n++)
				swap(m_data, i*iW+n, (i+1)*iW-(n+1));
		
		
		return;
	}





	template<class T>
	void ImageBase<T>::flipHorizontal()
	{
		myImage bild(m_dim[0], m_dim[1], m_data);
		for(int l = 0; l < bild.m_breite / 2; l++)
		{	
			for(int ll = 0; ll < bild.m_hoehe; ll++)
			{
				mypoint a(l,ll);
				mypoint b(bild.m_breite -1 - l,ll);
				
				// swap
				int dummy = bild.m_pData[a.m_y * bild.m_breite + a.m_x];
				bild.m_pData[POSITION(a.m_x, a.m_y, bild.m_breite)] = bild.m_pData[POSITION(b.m_x, b.m_y, bild.m_breite)];
				bild.m_pData[POSITION(b.m_x, b.m_y, bild.m_breite)] = dummy;

			}
		}
	}

	template<class T>
	void ImageBase<T>::flipVertical()
	{
		myImage bild(m_dim[0], m_dim[1], m_data);
		for(int l = 0; l < bild.m_hoehe / 2; l++)
		{
			for(int ll = 0; ll < bild.m_breite; ll++)
			{
				mypoint a(ll,l);
				mypoint b(ll,bild.m_hoehe - 1 - l);
				int dummy = bild.m_pData[a.m_y * bild.m_breite + a.m_x];
				bild.m_pData[POSITION(a.m_x, a.m_y, bild.m_breite)] = bild.m_pData[POSITION(b.m_x, b.m_y, bild.m_breite)];
				bild.m_pData[POSITION(b.m_x, b.m_y, bild.m_breite)] = dummy;
			}
		}
	}


	


}

\

// enable all warning concerning type conversions...
#ifdef WIN32
#pragma warning( default: 4541 )
#pragma warning ( default: 4244)
#pragma warning ( default: 4305)
#endif

#endif
