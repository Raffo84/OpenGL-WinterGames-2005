#ifndef IMGTYPES_H
#define IMGTYPES_H

namespace CAMP {	

#ifndef DOXYGEN_SHOULD_SKIP_THIS

	// Types used in the image class. Copied directly from windows.
	typedef long BOOL;
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned long DWORD;
	typedef long LONG;

	/* constants for the biCompression field */
	#define BI_RGB        0L
	#define BI_RLE8       1L
	#define BI_RLE4       2L
	#define BI_BITFIELDS  3L
	#define BI_JPEG       4L
	#define BI_PNG        5L

	typedef struct tagRGBTRIPLE {
		BYTE    rgbtBlue;
		BYTE    rgbtGreen;
		BYTE    rgbtRed;
	} RGBTRIPLE;

	//Colour palette
	typedef struct tagRGBQUAD {
		BYTE    rgbBlue;
		BYTE    rgbGreen;
		BYTE    rgbRed;
		BYTE    rgbReserved;
	} RGBQUAD;

	//File information header
	//provides general information about the file
	typedef struct tagBITMAPFILEHEADER { 
		WORD    bfType; 
		DWORD   bfSize; 
		WORD    bfReserved1; 
		WORD    bfReserved2; 
		DWORD   bfOffBits; 
	} BITMAPFILEHEADER;

	//Bitmap information header
	//provides information specific to the image data
	typedef struct tagBITMAPINFOHEADER{
		DWORD  biSize; 
		LONG   biWidth; 
		LONG   biHeight; 
		WORD   biPlanes; 
		WORD   biBitCount; 
		DWORD  biCompression; 
		DWORD  biSizeImage; 
		LONG   biXPelsPerMeter; 
		LONG   biYPelsPerMeter; 
		DWORD  biClrUsed; 
		DWORD  biClrImportant; 
	} BITMAPINFOHEADER;

	typedef struct tagBITMAPINFO { 
		BITMAPINFOHEADER bmiHeader; 
		RGBQUAD          bmiColors[1];
	} BITMAPINFO;

#endif

}

#endif
