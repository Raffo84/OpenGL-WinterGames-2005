#ifndef TOOLS_H
#define TOOLS_H

#include <cstdlib>						// needed for definition of NULL
#include <vector>
#include <string>

namespace CAMP {

class Image;
class MedImage;

/**
 * \ingroup ctools
 * \ingroup img
 *
 * \brief Some neat little tools. Both mathematical and imaging tools are available.
 * 
 * \todo Dudes, comment the source.  
 *
 */
class Tools {
public:
	static void computeGradient2D(float *data, float *buffer, int width, int height, bool interactive = false);
	
	static void computeSimpleGradient3D(Image *image, float *grad_buffer);
	
	static void getByteImage(float *data, unsigned char *image, int width, int height, float min = 0, float max = 0, bool interactive = false);
	static void getRGBImage(float *data, unsigned char *buffer, int width, int height, float min = 0, float max = 0, float *colormap = NULL, int colors = 0, int mode = 0);
	static void getRGBImage(int *data, unsigned char *buffer, int width, int height, float min = 0, float max = 0, float *colormap = NULL, int colors = 0, int mode = 0);
	static void getRGBAImage(float *data, unsigned char *buffer, int width, int height, Image *img, bool usewl = true);

	
	static void getByteImageGrad(float *data, unsigned char *image, int width, int height, float min = 0, float max = 0, bool interactive = false);
	static void getByteImageGrad(Image *img, unsigned char *image);
	
	static void smoothImageGrad(float *data, int width, int height);
	static void smoothImageGrad(MedImage *img);

	inline static float sprod(float *x, float *y) { return x[0]*y[0]+x[1]*y[1]+x[2]*y[2]; }
	inline static float sqr(float x) { return x*x; }
	inline static double sqr(double x) { return x*x; }
	static void rigidTransform(float *vector, float *pars, int mode = 0);
	static void rigidTransformMat(float *vector, float *matrix, int mode = 0);
	static void euler2hom(float *pars, float *matrix);
	static void hom2euler(float *pars, float *matrix);

	/// Multiplies two matrices, if result=1, store result in right matrix, left otherwise
	static void multMatrix(float *left, float *right, int result = 0);
		
	/**
	 * Returns the next bigger power of two.
	 * \param v The number to compute the next power of two. 
	 * \return The next power of two x for which \f$x \geq v\f$ and \f$\sqrt{x} \in \mathbf{N}\f$ hold.
	 */
	static inline int nextPowerTwo(int v)
	{
		int i = (v & -v); // get the lowest bitmark...
		while(i < v) i <<= 1;
		return i;
	}

	/**
	 * Checks wether the given number is a power of two.
	 * \param v The number to check for being a power of two.
	 * \return <code>true</code> if the parameter is a power of two, otherwise <code>false</code>
	 */
	static inline bool isPow2(int v)
	{
		return ((v & -v) == v);
	}

	static Image *loadMetaImage(char *filename);
	static int saveMetaImage(char *filename, Image *img);
	static Image *createImage(int datatype);
	static void addFilesInDir(std::string path, std::vector<std::string> &fileList, bool recursive = false);

	/// create a Gaussian filter with window size [n1, n2] and std. deviation sigma
	static float *createGaussFilter( int n1, int n2, float sigma );

	/// determines the eigenvalues and eigenvectors of the 2nd derivative at each pixel of a 2D image
	static void eigenAnalysis2D(Image *input, Image *output1, Image *output2 = 0, Image* output3 = 0);

};

}

#endif
