#pragma once

namespace tdl
{
#define OUT

	typedef unsigned short      WORD;
	typedef unsigned long       DWORD;
	typedef unsigned char       BYTE;
	typedef unsigned short      UINT16;
	typedef long                LONG;

	/* Bitmap File Header */
#pragma pack(push, 1)
	struct BITMAPFILEHEADER
	{
		WORD    bfType;
		DWORD   bfSize;
		WORD    bfReserved1;
		WORD    bfReserved2;
		DWORD   bfOffBits;
	};
#pragma pack(pop)

	constexpr DWORD BM_HEADER_MARKER = (static_cast<WORD>('M' << 8) | 'B');

	/* Bitmap Info Header */
#pragma pack(push, 1)
	struct BITMAPINFOHEADER
	{
		DWORD      biSize;
		LONG       biWidth;
		LONG       biHeight;
		WORD       biPlanes;
		WORD       biBitCount;
		DWORD      biCompression;
		DWORD      biSizeImage;
		LONG       biXPelsPerMeter;
		LONG       biYPelsPerMeter;
		DWORD      biClrUsed;
		DWORD      biClrImportant;
	};
#pragma pack(pop)

	/* constants for the biCompression field */
	constexpr DWORD TDL_BI_RGB = 0L;	// No Compression
	constexpr DWORD TDL_BI_RLE8 = 1L;
	constexpr DWORD TDL_BI_RLE4 = 2L;
	constexpr DWORD TDL_BI_BITFIELDS = 3L;
	constexpr DWORD TDL_BI_JPEG = 4L;
	constexpr DWORD TDL_BI_PNG = 5L;

	/* Bitmap Palette Header */
#pragma pack(push, 1)
	struct RGBQUAD
	{
		BYTE    rgbBlue;
		BYTE    rgbGreen;
		BYTE    rgbRed;
		BYTE    rgbReserved;
	};
#pragma pack(pop)

	enum class eColor
	{
		Gray,
		RGB
	};

	enum class eBitType
	{
		BIT_8U,
		BIT_16U
	};

	bool WriteBmp(const char* filename, const unsigned char* pImage, const DWORD width, const DWORD height, const eColor color);

	bool WriteBmp(const char* filename, const unsigned short* pImage, const DWORD width, const DWORD height, const eColor color);
}
