#include "BitmapHandler.h"
#include <stdio.h>
//#include <Windows.h>

namespace tdl
{
	bool WriteBmp(const char* filename, const unsigned char* pImage, const DWORD width, const DWORD height, const eColor color)
	{
		// Set Ouput File Object
		FILE* fp = nullptr;
		fopen_s(&fp, filename, "wb");
		if (nullptr == fp)
		{
#ifdef _DEBUG
			printf("fopen() error");
#endif
			return false;
		}

		// Set Header Info according to color type 
		DWORD bitCount;
		DWORD widthStep;
		DWORD offBits;
		DWORD printedWidthStep;
		DWORD printedImageSize;
		DWORD fileSize;

		switch (color)
		{
		case eColor::Gray:
			bitCount = 8;
			widthStep = width;
			offBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
			break;
		default:	// case eColor::RGB:
			bitCount = 24;
			widthStep = width * 3;
			offBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
			break;
		}
		printedWidthStep = (widthStep + 3) & ~3;
		printedImageSize = height * printedWidthStep;
		fileSize = offBits + printedImageSize;

		// Write Bitmap File Header
		BITMAPFILEHEADER bitmapFileHeader;
		bitmapFileHeader.bfType = BM_HEADER_MARKER;
		bitmapFileHeader.bfSize = fileSize;
		bitmapFileHeader.bfReserved1 = 0U;
		bitmapFileHeader.bfReserved2 = 0U;
		bitmapFileHeader.bfOffBits = offBits;
		fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);

		// Write Bitmap Info Header
		BITMAPINFOHEADER bitmapInfoHeader;
		bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapInfoHeader.biWidth = width;
		bitmapInfoHeader.biHeight = height;
		bitmapInfoHeader.biPlanes = 1;
		bitmapInfoHeader.biBitCount = bitCount;
		bitmapInfoHeader.biCompression = TDL_BI_RGB;
		bitmapInfoHeader.biSizeImage = printedImageSize;
		bitmapInfoHeader.biXPelsPerMeter = 0;
		bitmapInfoHeader.biYPelsPerMeter = 0;
		bitmapInfoHeader.biClrImportant = 0;
		bitmapInfoHeader.biClrUsed = 0;
		fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

		// Write Palette Header
		if (color == eColor::Gray)
		{
			RGBQUAD paletteHeader[256];
			for (int i = 0; i < 256; ++i)
			{
				paletteHeader[i].rgbBlue = (BYTE)i;
				paletteHeader[i].rgbGreen = (BYTE)i;
				paletteHeader[i].rgbRed = (BYTE)i;
				paletteHeader[i].rgbReserved = 0;
			}
			fwrite(&paletteHeader, sizeof(RGBQUAD), 256, fp);
		}

		// Write Printed Image Data 
		unsigned char* pPrintedImage = new unsigned char[printedImageSize] {0U, };
		for (size_t r = 0; r < height; ++r)
		{
			int rowIdx = r * widthStep;
			int printedRowIdx = r * printedWidthStep;

			for (size_t c = 0; c < widthStep; ++c)
			{
				pPrintedImage[printedRowIdx + c] = pImage[rowIdx + c];
			}
		}
		fwrite(pPrintedImage, printedImageSize, 1, fp);
		delete[] pPrintedImage;

		fclose(fp);
		return true;
	}

	bool WriteBmp(const char* filename, const unsigned short* pImage, const DWORD width, const DWORD height, const eColor color)
	{
		int imageSize;
		switch (color)
		{
		case eColor::Gray:
			imageSize = width * height;
			break;
		default:	// case eColor::RGB:
			imageSize = width * height * 3;
			break;
		}

		unsigned char* pImage8bit = new unsigned char[imageSize];

		for (int i = 0; i < imageSize; ++i)
		{
			pImage8bit[i] = static_cast<unsigned char>(pImage[i] >> 8);
		}

		bool returnValue = WriteBmp(filename, pImage8bit, width, height, color);

		delete[] pImage8bit;

		return returnValue;
	}
}