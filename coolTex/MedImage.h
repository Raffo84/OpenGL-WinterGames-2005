#ifndef	MEDIMAGE_H
#define	MEDIMAGE_H

#include <cstdlib>		// needed for definition of NULL
#include "ImageBase.h"

namespace CAMP {

#define		TB		unsigned char
#define		TD		unsigned short
#define		TF		float

inline TF sqr(TF x) { return x * x; }

/** @brief	Class for 2D and 3D 16-bit image data
*	@author	Wolfgang Wein
*
*  Handles both 2D and 3D data the same way, as 2D images are
*  just saved as a volume with one slice. The image data is saved as 16-bit
*  values, further computations like 2D/3D gradients produce floating-point
*  results. */
class MedImage: public ImageBase<unsigned short> {

public:

	/// Constructor, an optional type identifier can be set here
	MedImage(int type = 0);

	~MedImage();

	/// Load MetaImage
	/** The file with the text information has to be provided here. Depending
	*  on the read settings, one of the raw loading methods is called from
	*  here. */
	int load(const char *filename);

	/// Load 16 bit image data from a binary file.
	/** The filename pointer is saved as class member, in case the image is to be
	*  reloaded. The physical extensions of the image have to be set later. */
	int load16(char *filename, int dimx, int dimy, int dimz = 1,
				int frames = 1, int channels = 1, int sign = 0);

	/// Load 8 bit data, expand it to 16 bit
	int load8(char *filename, int dimx, int dimy, int dimz = 1, int frames = 1, int channels = 1);

	/// Load 32 bit floating-point data, shrink it to 16 bit
	int load32(char *filename, int dimx, int dimy, int dimz = 1, int frames = 1, int channels = 1);

	/// Reload the image data
	/** The image size is assumed to be unchanged. Therefore this function may
	*  only be called if the image has not been downsampled. */
	int reload();

	/// Specify if the 8 bit image should be inverted
	void setInvert(bool flag) { m_invert = flag; }

	/// Half the width and height of the image data for each slice
	void downSample2D();

	/// Half the width, height and number of slices of the image data
	void downSample3D();

	/// Flip the image data vertically
	void flipY();

	/// Flip the volume data with respect to the Z axis
	void flipZ();

	/// Returns a 8 bit image using the current window/level and invert setting
	void getSlice(TB *image, int nr = 0);

	/// Return pointer to the 2D gradient image
	TF *getGradient2D() { return m_gradient2d; }

	/// Return pointer to the 3D short gradient volume data
	short *getGradient3D() { return m_gradient3d; }
	
	/// Return pointer to the 3D gradient volume image instance
	ImageBase<short> *getGradient3DImage() { return m_gradimage; }
	
	/// Return pointer to the 3D float gradient volume data
	float *getGradient3Df() { return m_gradient3df; }
	
	/// Return pointer to the 3D float gradient volume image instance
	ImageBase<float> *getGradient3DfImage() { return m_gradimage_f; }

	/// Return maximum gradient vector magnitude. */
	float getGradient3DMax() { return m_gradient3d_max; }

	// void writePNG(char *filename, TB *image = NULL);

	/// Compute horizontal and vertical gradient images using a 2D Sobel filter
	void computeGradient2Dold(int slice = 0);

	/// Compute 3D gradient volume, release volume data if desired
	/** The second parameter denotes if the gradient volume should be created
	 *  with four channels, where the fourth one will be the original image
	 * intensity. This can be useful for future rendering techniques... */
	void computeGradient3Dold(bool release = false, bool fourchannels = false);

	/// Compute 3D gradient volume as float values
	void computeGradient3Df(bool release = false);

	/// Compute 3D gradient volume with differences of neighboring voxels
	void computeSimpleGradient3D();

	int getType() { return m_type; }

	/// @name MedImage Type Identifiers
	//@{
	enum {
		NA   = 0,	///< Not Available / Unknown
		CT   = 1,	///< Computed Tomography
		MR   = 2,	///< Magnetic Resonance
		PET  = 3,	///< Positron Emission Tomography
		XRAY = 8,	///< X-Ray MedImage
		US   = 9	///< Ultrasound MedImage
	};	
	//@}


	/// Return closest value from the gradient volume
	inline void getClosestGrad(TF x, TF y, TF z, TF *result) {
		m_gradimage->getClosest(result, x-1.0f, y-1.0f, z-1.0f);
//		int pos = 3*(((int)x-1)+(m_dim[0]-2)*((int)y-1)+(m_dim[0]-2)*(m_dim[1]-2)*((int)z-1));
//		for (int i = 0; i < 3; i++) result[i] = m_gradient3d[pos+i];
	}


	/// Return trilinearly interpolated value from the gradient volume
	inline void getTrilinearGrad(TF x, TF y, TF z, TF *result) {
		m_gradimage->getTrilinear(result, x-1.0f, y-1.0f, z-1.0f);
/*		int xx = (int)x, yy = (int)y, zz = (int)z;
		TF x2 = x - xx, y2 = y - yy, z2 = z - zz;
		for (int i = 0; i < 3; i++) result[i] =
			(m_gradient3d[3*((xx-1)+(m_dim[0]-2)*(yy-1)+(m_dim[0]-2)*(m_dim[1]-2)*(zz-1))+i]  * (1-x2) * (1-y2) * (1-z2)
			+ m_gradient3d[3*((xx  )+(m_dim[0]-2)*(yy-1)+(m_dim[0]-2)*(m_dim[1]-2)*(zz-1))+i] *   x2   * (1-y2) * (1-z2)
			+ m_gradient3d[3*((xx-1)+(m_dim[0]-2)*(yy  )+(m_dim[0]-2)*(m_dim[1]-2)*(zz-1))+i] * (1-x2) *   y2   * (1-z2)
			+ m_gradient3d[3*((xx-1)+(m_dim[0]-2)*(yy-1)+(m_dim[0]-2)*(m_dim[1]-2)*(zz  ))+i] * (1-x2) * (1-y2) *   z2
			+ m_gradient3d[3*((xx  )+(m_dim[0]-2)*(yy-1)+(m_dim[0]-2)*(m_dim[1]-2)*(zz  ))+i] *   x2   * (1-y2) *   z2
			+ m_gradient3d[3*((xx-1)+(m_dim[0]-2)*(yy  )+(m_dim[0]-2)*(m_dim[1]-2)*(zz  ))+i] * (1-x2) *   y2   *   z2
			+ m_gradient3d[3*((xx  )+(m_dim[0]-2)*(yy  )+(m_dim[0]-2)*(m_dim[1]-2)*(zz-1))+i] *   x2   *   y2   * (1-z2)
			+ m_gradient3d[3*((xx  )+(m_dim[0]-2)*(yy  )+(m_dim[0]-2)*(m_dim[1]-2)*(zz  ))+i] *   x2   *   y2   *   z2); */
	}


protected:

	float *m_gradient2d;		///< floating point slice gradient data
	short *m_gradient3d;		///< floating point gradient volume
	ImageBase<short> *m_gradimage; ///< image instance for short gradient volume
	ImageBase<float> *m_gradimage_f; ///< image instance for float gradient volume
	float *m_gradient3df;	///< signed short gradient volume
	float m_gradient3d_max;	///< maximum gradient vector magnitude
	char m_filename[256];	///< filename string

	int m_type;				///< image type

	bool m_invert;			///< invert flag for getSlice()

};

}

#endif
