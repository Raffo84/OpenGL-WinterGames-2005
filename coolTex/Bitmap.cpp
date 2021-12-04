#include "Bitmap.h"

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace CAMP {

	int Bitmap::load(const char* file_name, bool stride)
	{
		if (!file_name) return -1;

		std::ifstream file(file_name, std::ios::in | std::ios::binary);
		
		if ( !file.good() )
			return -1;

		// Read the file and info headers
		CAMP::BITMAPFILEHEADER bfh;
		CAMP::BITMAPINFOHEADER bih;

		// because we don't know how the compiler packs the struct we have to read every single value
		// manually, which is a pain in the ass, but there is no way around it...
        file.read(reinterpret_cast<char*>(&bfh.bfType), sizeof(bfh.bfType));
		file.read(reinterpret_cast<char*>(&bfh.bfSize), sizeof(bfh.bfSize));
		file.read(reinterpret_cast<char*>(&bfh.bfReserved1), sizeof(bfh.bfReserved1));
		file.read(reinterpret_cast<char*>(&bfh.bfReserved2), sizeof(bfh.bfReserved2));
		file.read(reinterpret_cast<char*>(&bfh.bfOffBits), sizeof(bfh.bfOffBits));

		file.read(reinterpret_cast<char*>(&bih.biSize),sizeof(bih.biSize));
		file.read(reinterpret_cast<char*>(&bih.biWidth),sizeof(bih.biWidth));
		file.read(reinterpret_cast<char*>(&bih.biHeight),sizeof(bih.biHeight));
		file.read(reinterpret_cast<char*>(&bih.biPlanes),sizeof(bih.biPlanes));
		file.read(reinterpret_cast<char*>(&bih.biBitCount),sizeof(bih.biBitCount));
		file.read(reinterpret_cast<char*>(&bih.biCompression),sizeof(bih.biCompression));
		file.read(reinterpret_cast<char*>(&bih.biSizeImage),sizeof(bih.biSizeImage));
		file.read(reinterpret_cast<char*>(&bih.biXPelsPerMeter),sizeof(bih.biXPelsPerMeter));
		file.read(reinterpret_cast<char*>(&bih.biYPelsPerMeter),sizeof(bih.biYPelsPerMeter));
		file.read(reinterpret_cast<char*>(&bih.biClrUsed),sizeof(bih.biClrUsed));
		file.read(reinterpret_cast<char*>(&bih.biClrImportant),sizeof(bih.biClrImportant));

		// if we don't habe a bitmap, we are done...
		if (bfh.bfType != 0x4d42) return -1;	// == 19778 == "BM"
		
		m_BitCount = bih.biBitCount;

		if (bih.biBitCount == 8)								// we need to read the palette
		{			
			ImageBase<byte>::init(bih.biWidth,abs(bih.biHeight),1,1,bih.biBitCount >> 3);

			// the image's dimensions in meter
			if (bih.biXPelsPerMeter!=0)
				m_size[0] = static_cast<float>(m_dim[0])/static_cast<float>(bih.biXPelsPerMeter);
			if (bih.biYPelsPerMeter!=0)
				m_size[1] = static_cast<float>(m_dim[1])/static_cast<float>(bih.biYPelsPerMeter);

			if (bih.biClrUsed == 0)
				m_NumColors = 1 << bih.biBitCount; // 2^bih.biBitCount;
			else
				m_NumColors = bih.biClrUsed;

			m_Palette = new CAMP::RGBQUAD[m_NumColors];

			// read the palette...
			for (int color_idx = 0; color_idx<m_NumColors; ++color_idx)
			{
				file.read(reinterpret_cast<char*>(&(m_Palette[color_idx].rgbBlue)), 1);
				file.read(reinterpret_cast<char*>(&(m_Palette[color_idx].rgbGreen)), 1);
				file.read(reinterpret_cast<char*>(&(m_Palette[color_idx].rgbRed)), 1);
				file.read(reinterpret_cast<char*>(&(m_Palette[color_idx].rgbReserved)), 1);
			} // for (int color_idx = 0; color_idx<m_NumColors; ++color_idx)
		}
		else if (bih.biBitCount>=16)							// no palette given
		{			
			ImageBase<byte>::init(bih.biWidth,abs(bih.biHeight),1,1,bih.biBitCount >> 3);

			// the image's dimensions in meter
			if (bih.biXPelsPerMeter!=0)
				m_size[0] = static_cast<float>(m_dim[0])/static_cast<float>(bih.biXPelsPerMeter);
			if (bih.biYPelsPerMeter!=0)
				m_size[1] = static_cast<float>(m_dim[1])/static_cast<float>(bih.biYPelsPerMeter);			
		}
		else													// unsupported bitmap format
		{
			return -1;
		}		


		// now we are just going to read the pixel values
		// if a palette is present the rgb tupel corresponding to a specific pixel value x can be
		// found at m_Palette[x]...
		byte *buffer = new byte[bih.biSizeImage];
		file.read(reinterpret_cast<char*>(buffer), bih.biSizeImage);
		file.close();

		// this is pretty cryptic but does exactly this...
		// int local_stride = bih.biWidth * bih.biBitCount/8;
		// if (local_stride%4!=0) {
		//   local_stride += 4 - m_stride%4;
		// }
		int dword_aligned_stride = (((bih.biWidth * bih.biBitCount) + 31) & ~31) >> 3;
		
		if (bih.biHeight > 0)	// flip the image
		{			
			byte *current_src_line = buffer;
			byte *current_dst_line = m_data + (m_dim[1]-1)*m_stride;
			for (int row = 0; row<m_dim[1]; ++row)
			{
				std::memcpy(current_dst_line, current_src_line, m_stride);
				current_dst_line -= m_stride;
				current_src_line += dword_aligned_stride;
			}
		}
		else					// no flipping needed
		{
			byte *current_src_line = buffer;
			byte *current_dst_line = m_data;
			for (int row = 0; row<m_dim[1]; ++row)
			{
				std::memcpy(current_dst_line, current_src_line, m_stride);
				current_dst_line += m_stride;
				current_src_line += dword_aligned_stride;
			}
		}

		delete [] buffer;

		return 1;
	}

	int Bitmap::save(const char* file_name)
	{
		//#region setup bitmap file header and bitmap info header
		// Read the file and info headers
		BITMAPFILEHEADER bfh;
		BITMAPINFOHEADER bih;

		// should definitly not happen at this time...
		if (m_NumColors!=0 && !m_Palette) return -1;

		// NOTE: sizeof(bfh) + sizeof(bih) will NOT work, since most compilers 4 byte align their structs...
		int header_size = 54;		
		int palette_size = m_NumColors * 4; // 4 bytes per palette entry

		int numColors = 256;

		if (!m_Palette && (m_BitCount == 8 || m_channels == 1))
		{
			palette_size = numColors * 4;
		}		

		// missing palette information...
		if ( (m_BitCount==1 || m_BitCount==4) && !m_Palette) return -1;
				
		bfh.bfType = ('B' + ('M' << 8));
		bfh.bfSize = 0; // we'll compute that one later
		bfh.bfReserved1 = 0;
		bfh.bfReserved2 = 0;
		bfh.bfOffBits = header_size + palette_size;

		bih.biSize = sizeof(bih);
		bih.biWidth = m_dim[0];
		bih.biHeight = m_dim[1];
		bih.biPlanes = 1;
		bih.biBitCount = m_BitCount;
		bih.biCompression = 0;
		bih.biSizeImage = ((((m_dim[0] * m_BitCount) + 31) & ~31) / 8) * m_dim[1];
		bih.biXPelsPerMeter = 0;
		bih.biYPelsPerMeter = 0;
		bih.biClrUsed = numColors;
		bih.biClrImportant = 0;		
		
		// final corrections following...
		/*
		int file_alignment = (bfh.bfOffBits + bih.biSizeImage) % 4;
		if (file_alignment != 0)
			bih.biSizeImage += file_alignment;

		bfh.bfSize = bfh.bfOffBits + bih.biSizeImage;
		*/
		//#endregion

		if (!file_name) return -1;

		std::ofstream file(file_name, std::ios::out | std::ios::binary);
		
		if ( !file.good() ) return -1;

		
        file.write(reinterpret_cast<char*>(&bfh.bfType), sizeof(bfh.bfType));
		file.write(reinterpret_cast<char*>(&bfh.bfSize), sizeof(bfh.bfSize));
		file.write(reinterpret_cast<char*>(&bfh.bfReserved1), sizeof(bfh.bfReserved1));
		file.write(reinterpret_cast<char*>(&bfh.bfReserved2), sizeof(bfh.bfReserved2));
		file.write(reinterpret_cast<char*>(&bfh.bfOffBits), sizeof(bfh.bfOffBits));

		file.write(reinterpret_cast<char*>(&bih.biSize),sizeof(bih.biSize));
		file.write(reinterpret_cast<char*>(&bih.biWidth),sizeof(bih.biWidth));
		file.write(reinterpret_cast<char*>(&bih.biHeight),sizeof(bih.biHeight));
		file.write(reinterpret_cast<char*>(&bih.biPlanes),sizeof(bih.biPlanes));
		file.write(reinterpret_cast<char*>(&bih.biBitCount),sizeof(bih.biBitCount));
		file.write(reinterpret_cast<char*>(&bih.biCompression),sizeof(bih.biCompression));
		file.write(reinterpret_cast<char*>(&bih.biSizeImage),sizeof(bih.biSizeImage));
		file.write(reinterpret_cast<char*>(&bih.biXPelsPerMeter),sizeof(bih.biXPelsPerMeter));
		file.write(reinterpret_cast<char*>(&bih.biYPelsPerMeter),sizeof(bih.biYPelsPerMeter));
		file.write(reinterpret_cast<char*>(&bih.biClrUsed),sizeof(bih.biClrUsed));
		file.write(reinterpret_cast<char*>(&bih.biClrImportant),sizeof(bih.biClrImportant));

		char zero = 0x00;
		
		if (m_BitCount<16) {			
			if (m_Palette) {
				for (int color_idx=0; color_idx<numColors; ++color_idx)
				{
					file.write(reinterpret_cast<char*>(&(m_Palette[color_idx].rgbBlue)), 1);
					file.write(reinterpret_cast<char*>(&(m_Palette[color_idx].rgbGreen)), 1);
					file.write(reinterpret_cast<char*>(&(m_Palette[color_idx].rgbRed)), 1);
					file.write(reinterpret_cast<char*>(&(m_Palette[color_idx].rgbReserved)), 1);
				} // for (int color_idx=0; color_idx<numColors; ++color_idx)
			} else {
				char current_char;
				for (int color_idx=0; color_idx<numColors; ++color_idx)
				{
					current_char = static_cast<char>(color_idx);
					file.write(reinterpret_cast<char*>(&current_char), 1);
					file.write(reinterpret_cast<char*>(&current_char), 1);
					file.write(reinterpret_cast<char*>(&current_char), 1);
					file.write(reinterpret_cast<char*>(&zero), 1);
				}
			}
		}		
		
		int dword_aligned_stride = (((bih.biWidth * bih.biBitCount) + 31) & ~31) >> 3;
		int padding = dword_aligned_stride - (bih.biWidth * m_channels);

		byte *current_src_line = m_data + (bih.biHeight-1)*m_stride;
		for (int row=0; row<bih.biHeight; ++row)
		{
			// write the current line
			file.write(reinterpret_cast<char*>(current_src_line), m_stride);

			// dword align the current line
			for (int i=0; i<padding; ++i)
			{
				file.write(reinterpret_cast<char*>(&zero), 1);
			}

			// next line...
			current_src_line -= m_stride;
		} // for (int row=0; row<bih.biHeight; ++row)

		/*
		// four byte align the whole file...
		for (int i=0; i<file_alignment; ++i)
		{			
			file.write(reinterpret_cast<char*>(&zero), 1);
		}
		*/

		file.close();		

		return 1;
	}

	//void Bitmap::warpImage(int *offsets, int w_width, int w_height, bool luminance)
	//{	
	//	if (luminance) {
	//					
	//		// Enter here, if you want to convert the image to grayscale			
	//		int w_stride = w_width;									// when using bitmaps a pixel's size equals the nr of channels.
	//		int w_padding = 0;
	//	
	//		byte *warped_data = static_cast<byte*>(aligned_malloc(w_height*w_stride * sizeof(byte)));									
	//		std::fill(warped_data, warped_data+w_height*w_stride, 0);	// blacken the image

	//		byte *dst = warped_data;									// used for ptr-arithmetic
	//		byte *current_pixel;
	//		int current_offset;

	//		if (w_padding!=0) {
	//			for (int y=0; y<w_height; ++y) {
	//				for (int x=0; x<w_width; ++x) {
	//					current_offset = offsets[y*w_width+x];
	//					current_pixel = m_data + current_offset;
	//					if (current_offset!=-1) {											// found a pixel
	//													
	//						*dst = static_cast<byte>(0.11f*(*(current_pixel++)) +			// copy the pixel and compute the luminance value
	//								                 0.59f*(*(current_pixel++)) +			// luminance = 0.3*R+0.59*G+0.11*R
	//								                 0.3f*(*(current_pixel)) +				// our bitmaps are stored in BGR order
	//												 0.5f);									// force correct rounding behaviour							
	//						
	//					}
	//					++dst;																// go to the next pixel
	//				}
	//				dst+=w_padding;															// reached the line's end; skip padding bytes
	//			}
	//		} else {
	//			int size = w_width*w_height;
	//			for (int i=0; i<size; ++i) {
	//				current_offset = offsets[i];
	//				current_pixel = m_data + current_offset;
	//				if (current_offset!=-1) {

	//					*dst = static_cast<byte>(0.11f*(*(current_pixel++)) +				// copy the pixel and compute the luminance value
	//					                         0.59f*(*(current_pixel++)) +				// luminance = 0.3*R+0.59*G+0.11*R
	//					                         0.3f*(*(current_pixel)) +					// our bitmaps are stored in BGR order
	//											 0.5f);										// force correct rounding behaviour
	//				}
	//				++dst;
	//			}
	//		}

	//		this->init(w_width, w_height, 1, warped_data);

	//	} else {

	//		// Enter here, if we do NOT want to convert the image to grayscale
	//		int w_stride = w_width*m_channels;							// when using bitmaps a pixel's size equals the nr of channels.
	//		int w_padding = 0;
	//	
	//		byte *warped_data = static_cast<byte*>(aligned_malloc( w_height * w_stride * sizeof(byte)));
	//		std::fill(warped_data, warped_data+w_height*w_stride, 0);	// blacken the image

	//		byte *dst = warped_data;									// used for ptr-arithmetic

	//		int current_offset;

	//		if (w_padding!=0) {
	//			for (int y=0; y<w_height; ++y) {
	//				for (int x=0; x<w_width; ++x) {
	//					current_offset = offsets[y*w_width+x];				
	//					if (current_offset!=-1) {							// found a pixel
	//						memcpy(dst,m_data+current_offset,m_channels);	// copy it
	//					}
	//					dst+=m_channels;									// go to the next pixel
	//				}
	//				dst+=w_padding;											// reached the line's end; skip padding bytes
	//			}
	//		} else {
	//			int size = w_width*w_height;				
	//			for (int i=0; i<size; ++i) {
	//				current_offset = offsets[i];				
	//				if (current_offset!=-1) {							// found a pixel
	//					memcpy(dst,m_data+current_offset,m_channels);	// copy it
	//				}
	//				dst+=m_channels;									// go to the next pixel					
	//			}
	//		}

	//		this->init(w_width, w_height, m_channels, warped_data);
	//	}
	//}

	void Bitmap::init(int width, int height, int channels, byte *data)
	{	
		ImageBase<byte>::init(width,height,1,1,channels);
		
		m_BitCount = channels * 8;		
		
		if (data)
		{			
			destroy();
			m_data = data;
		}
	}	
}

