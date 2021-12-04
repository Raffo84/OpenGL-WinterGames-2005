#ifndef IMAGE_H
#define IMAGE_H

#include <cstdlib>	// needed for definition of NULL

// The following definitions are intended for a nice doxygen structure.
// TODO:: Doxygen offers the possibility of moving such definitions into a
//        separate file. Maybe this would be a bit more concise.

/**
 * \defgroup img Imaging
 *
 * This group of objects is intended to offer representations for arbitrary imaging modalities including
 * basic methods and functions to work on these images.
 */

/** \defgroup dtypes Mathematical Datatypes */

/** \defgroup ctools Common Tools */

namespace CAMP {

/** \addtogroup img */

/*@{*/

typedef unsigned int uint; ///< Lazy coder's typedef...
typedef unsigned char byte; ///< Lazy coder's typedef...
typedef unsigned short word; ///< Lazy coder's typedef...
typedef unsigned short ushort; ///< Lazy coder's typedef...
typedef unsigned long dword; ///< Lazy coder's typedef...

/*@}*/

/**
 *  \ingroup img
 *  @brief	Base class for 2D-5D image data
 *	@author	Wolfgang Wein
 *
 *  This is the not templated interface class for any image data. All methods
 *  which do not refer to image data directly, are implemented here. All
 *  methods, whose signature does not contain the data type, are defined as
 *  virtual, so that they can be used without knowing the template type. Refer
 *  to the subclass ImageBase<T> for further details. */
class Image {
public:
	/**
	 * Image standard constructor. Initializes almost all image parameters besides the actual image data, which
	 * depends on the data type.
	 */
	Image();

	/**
	 * Standard destructor for images.
	 * \return 
	 */
	virtual ~Image();

	/**
	 * Load image from file
	 * \param *filename The filename of the image to load.
	 * \return 
	 */
	virtual int load(const char *filename) { return 0; }

	/// Load raw data from file into already initialized image
	virtual int loadData(char *filename) = 0;

	/// Load raw data from pointer into already initialized image
	virtual int loadRawData(void* data) = 0;
	
	/// Saves the raw data to a file
	virtual int saveData(char *filename) = 0;

	/// Initialize image data buffer
	virtual int init(const int width, const int height,
	const int slices = 1, const int frames = 1, const int channels = 1);

	/// Clear image data buffer
	virtual void clear(void) = 0;

	/// Destroy image data buffer and free memory
	virtual void destroy(void) = 0;
	
	/// Downsample the image a specific value in each dimension
	virtual Image *downSample(const int nx, const int ny, const int nz = 1, const int nt = 1, const bool create = false) = 0;

	/// Rotate the volume 90 degrees around one of the axis
	virtual void rotate(int axis) = 0;
	
	/// Scale one image dimension to the power of two. Add in the other dimension a padding to get it to a power of 2.
	virtual void scalePow2(void) = 0;

	/// Invert the image, using the min/max values as borders
	virtual void invert() = 0;

	/// Clamp image values to window/level range
	virtual void applyWindowLevel(bool invert = false) = 0;
	
	/// Iterate, return as floating point values
	virtual bool getNext(float *result) = 0;
	
	/// Iterate, writing the image data
	virtual bool setNext(float value) = 0;

	/// Set value at current iteration
	virtual void setCurrent(float value) = 0;

	/// Set value at specified position
	virtual void setVoxelFloat(const float value, const int x, const int y, const int z = 0, const int t = 0, const int c = 0) = 0;

	/// Return closest value (also called Nearest Neighbor Interpolation)
	virtual float getClosest(const float x, const float y, const float z) = 0;

	/// Return closest value (also called Nearest Neighbor Interpolation)
	virtual float getVoxelFloat(const int x, const int y, const int z = 0, const int t = 0, const int c = 0) = 0;

	/// Return trilinearly interpolated value from the volume
	virtual float getTrilinear(const float x, const float y, const float z) = 0;

	/// Tri-Cubic interpolation for one channel. Uses 64 neighbored values, therefore very slow!
	virtual float getTricubic(const float x, const float y, const float z) = 0;
		
	/// Return closest value for all channels
	virtual void getClosest(float *result, const float x, const float y, const float z) = 0;
	
	/// Return trilinearly interpolated value for all channels
	virtual void getTrilinear(float *result, const float x, const float y, const float z) = 0;

	/// Tri-Cubic interpolation. Uses 64 neighbored values, therefore very slow!
	virtual void getTricubic(float *result, const float x, const float y, const float z) = 0;

	/// Set minimum and maximum value according to the image data
	virtual void computeMinMax() = 0;

	/// Convert from little to big endian and reverse
	virtual void swapBytes() = 0;

	/// Flip the data in arbitrary dimensions
	virtual void flip(bool x = true, bool y = false, bool z = false, bool t = false) = 0;

	/// Apply a 2D filter to the image
	virtual Image *applyFilter2D( int datatype, float *filter, int windowSize, CAMP::Image *filteredImage = NULL) = 0;

	/// Fixes the edges of an image after convolution
	virtual void fixEdges( int nPixels ) = 0;

	/// Compute the 2D gradient image
	virtual Image *computeGradient2D(int datatype = 0, float *filter = NULL,
	bool border = false, Image *grad = NULL) = 0;

	/// Computes the 3 channels of a hessian matrix for a 2D image:
	virtual Image *computeHessian2D(int datatype, float *filter = 0, Image *hessian = 0) = 0;
	
	/// Compute the 3D gradient volume of this image.
	/** If datatype is omitted, the same as the Image is used. powertwo indicates
	 *  that the resulting volume is created with a power of two number of voxels
	 *  in each dimension. fourchannels=true creates fourth channel with the
	 *  original image intensity in it. */
	virtual Image *computeGradient3D(int datatype = 0, float *filter = NULL, bool border = false, bool fourchannels = false, Image *grad = NULL) = 0;
	
	virtual unsigned char *getDataUByte(unsigned char *buffer = 0, bool powertwo = true) = 0;
	virtual unsigned short *getDataUShort(unsigned short *buffer = 0, bool powertwo = true) = 0;
	
	virtual unsigned char *getSliceUByte(unsigned char *buffer = 0, int slice = 0, int channel=0, bool powertwo = true) = 0;

	/// Compute the center of mass (mass being image intensity)
	virtual void computeMassCenter(float *result) = 0;

	/// Specify the window and level setting
	/** Does not directly affect the image data. */
	void setWindowLevel(const float window, const float level, const int channel = 0);

	/// Another expression for the window/level setting
	/** window = max-min, level = (max+min)/2 */	
	void setMinMax(const float min, const float max, const int channel = 0);

	/// Set physical image size
	void setPhysicalSize(const float s0, const float s1, const float s2 = 1.0, const float s3 = 1.0);

	/// start iteration
	void iterate() { m_iteration = 0; }

	/// Return number indicating the type of the image data
	int getDataType() { return m_datatype; }
	
	/// Return width of the image data
	inline int getWidth() { return m_dim[0]; }
	
	/// Return height of the image data
	inline int getHeight() { return m_dim[1]; }

	/// Return number of slices
	inline int getSlices() { return m_dim[2]; }

	/// Return number of frames
	inline int getFrames() { return m_dim[3]; }
	
	/// Return number of channels
	inline int getChannels() { return m_channels; }

	/// Get width/height/slices with dimension index
	inline int getDim(const int nr) { return m_dim[nr]; }

	/// Get whole image size
	inline int getDim() { return m_image_size; }

	/// Get whole image size
	inline int getImageSize() { return m_image_size; }

	/// Get stride
	inline int getStride() { return m_stride; }

	/// Get the a slices size
	inline int getSliceSize() { return m_slice_size; }

	/// Return physical size
	inline float getSize(const int nr) { return m_size[nr]; }

	inline float getWindow(const int channel = 0) { return m_wl[4*channel]; }
	inline float getLevel(const int channel = 0) { return m_wl[4*channel+1]; }
	inline float getMin(const int channel = 0) { return m_wl[4*channel+2]; }
	inline float getMax(const int channel = 0) { return m_wl[4*channel+3]; }

	/// @name Image data type constants
	/* They are the equivalent to the corresponding OpenGL constants! */
	//@{
	enum {
		BYTE			= 0x1400,
		UNSIGNED_BYTE	= 0x1401,
		SHORT			= 0x1402,
		UNSIGNED_SHORT	= 0x1403,
		INT				= 0x1404,
		UNSIGNED_INT	= 0x1405,
		FLOAT			= 0x1406,
		DOUBLE			= 0x140A
	};
	//@}

	static const float SOBEL2D[9]; ///< 3x3 sobel operator for 2D (edge detection)
	static const float SOBEL3D[27]; ///< 3x3x3 sobel operator for 3D (edge detection)

protected:
	int m_dim[4];		///< Image dimensions (width, height, slices, frames)
	float m_size[4];	///< Physical pixel/voxel size/time step
	int m_channels;		///< Number of channels
	float *m_wl;		///< Window/level/min/max parameters for each channel
	int m_debug;		///< Nonzero value causes more debug output
	int m_stride;		///< Size of one image row in bytes
	int m_slice_size;	///< Size of one slice
	int m_frame_size;	///< Size of one frame (resp. volume)
	int m_image_size;	///< Size of the whole image	
	int m_iteration;	///< Position of iteration
	int m_datatype;		///< Image data type
};

}

#endif
