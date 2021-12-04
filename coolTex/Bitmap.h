#ifndef BITMAP_H
#define BITMAP_H

#include "DataTypes.h"
#include "ImgTypes.h"
#include "ImageBase.h"

namespace CAMP {

	/**
	 * \ingroup img
	 * 
	 * \brief Class which models windows bitmaps.
	 *
	 * \author Hauke Heibel
     *
	 * Currently 8, 16, 24 and 32 bit bitmaps should be supported. While loading bitmaps are
	 * flipped to get the (0,0) coordinate at the top left.
	 *
	 * \todo I am not quite sure, but not deleting the palette array might be a source for a memory leak.
	 */
	class Bitmap : public ImageBase<byte>
	{
	public:
		/**
		 * Standard constructor which initialized an empty bitmap.
		 */
		Bitmap() : ImageBase<byte>()
		{
			// The rest is set properly in the super constructor.
			m_BitCount = 0;

			m_NumColors = 0;
			m_Palette = NULL;

			ImageBase<byte>::init(0,0,0,0,0);
		};		

		/**
		 * Copy constructor.
		 * \param &to_be_copied Reference to the bitmap that shall be copied.
		 */
		Bitmap(Bitmap &to_be_copied) : ImageBase<byte>(to_be_copied)
		{						
			m_BitCount = to_be_copied.getBitCount();
			
			m_NumColors = to_be_copied.getNumColors();
			m_Palette = new RGBQUAD[m_NumColors];

			std::memcpy(m_Palette, to_be_copied.getPalette(), m_NumColors*sizeof(RGBQUAD));
		};

		/**
		 * Standard destructor for bitmaps.
		 */
		~Bitmap() {
			if (m_Protected) return;
		};

		/**
		* Function to load an image.
		*/
		int load(const char* file_name, bool stride = true);

		/**
		* Saves the bitmap ;).
		*/
		int save(const char* filename);

		/**
		* Method to warp the image by using precomputed offsets. The i-th's pixel's offset of the warped (!) image
		* is stored at index i in the offsets array. The offsets array has a length of width*height of the warped
		* image...the offsets array contains a -1 if at this index does no according pixel exist. These pixels will
		* occur black in the image.
		*/
		//void warpImage(int *offsets, int w_width, int w_height, bool luminance=false);

	    /**
		* Initialize the image with the given parameteres. Previously allocated pixel data
		* will get lost if the image is not protected.
		*/
		void init(int width, int height, int channels = 3, byte *data = NULL);
				
		/**
		 * Returns the number of bits per pixel.
		 * \return Bits per pixel.
		 */
		inline int getBitCount(void) { return m_BitCount; }

		/**
		 * Returns the number of colors defined in the palette.
		 * \return Number of palette colors.
		 */
		inline int getNumColors(void) { return m_NumColors; }

		/**
		 * Returns the address of the palette.
		 * \return Palette's address.
		 */
		inline CAMP::RGBQUAD *getPalette(void) { return m_Palette; }

	protected:		
		int m_BitCount; ///< Additional bit count, which becomes important if working with 1 or 4 bit bitmaps.
		
		int m_NumColors; ///< Number of colors defined in the palette.
		CAMP::RGBQUAD *m_Palette; ///< Array of RGBQUADs which actually defines the palette.

	};

}

#endif

