#include "stdlib.h"
#include "assert.h"
#include <windows.h>
#include <stdio.h>
#include <list>
#include <algorithm>
#include "BitmapHandler.h"

using namespace std;

#define LOG_OUT(_x_) OutputDebugString(_x_) 
#define LOG_OUT_W(_x_)  OutputDebugStringW(_x_) 
#define LOG_OUT_A(_x_)  OutputDebugStringA(_x_) 

bool LoadBmp(const char* filename, byte** pImage);
bool SaveBmp(const char* filename, byte* pImage, int width, int height);

int main()
{
	int width = 100;
	int height = 100;
	int band = 1;
	unsigned char* pImage = new unsigned char[width * height * band];

	for (int i = 0; i < width * height * band; ++i)
	{
		pImage[i] = i % 256;
	}

	tdl::WriteBmp("testGray.bmp", pImage, width, height, tdl::eColor::Gray);

    delete[] pImage;
    
    unsigned short* pImage16bit = new unsigned short[width * height * band];

    for (int i = 0; i < width * height * band; ++i)
    {
        pImage16bit[i] = i % USHRT_MAX;
    }

    tdl::WriteBmp("testGray_16bit.bmp", pImage16bit, width, height, tdl::eColor::Gray);

    delete[] pImage16bit;

    return 0;
}

bool LoadBmp(const char* filename, byte** pImage)
{
    FILE* fp;

    // ��Ʈ�� ���� ����ü
    BITMAPFILEHEADER BMFH;					///< BitMap File Header.
    BITMAPINFOHEADER BMIH;					///< BitMap Info Header.

    fopen_s(&fp, filename, "rb");
    if (nullptr == fp)
    {
        LOG_OUT_A("fopen() error");
        return false;
    }

    // ������ ũ�⸸ŭ ���Ͽ��� �о��, �׸��� ��Ʈ����������� �־���
    fread(&BMFH, sizeof(BITMAPFILEHEADER), 1, fp);
    if (BMFH.bfType != 0x4d42)	// ��Ʈ�� ������ �ƴϸ� �����Ѵ�.
    {
        fclose(fp);
        LOG_OUT_A("not '.bmp' file !!!");
        return false;
    }

    fread(&BMIH, sizeof(BITMAPINFOHEADER), 1, fp);	//��������� �ִ� ũ���� ������ŭ �о
    if (BMIH.biBitCount != 24 || BMIH.biCompression != BI_RGB) //24��Ʈ���� üũ�ϰ�, ������ �ȵǾ� �ִ��� üũ�� ��
    {
        fclose(fp);
        return false;
    }

    INT Width = BMIH.biWidth;
    INT Height = BMIH.biHeight - 1;
    INT BytePerScanline = (Width * 3 + 3) & ~3;  // �е�
    INT size = BMFH.bfSize;

    *pImage = (BYTE*)malloc(size);

    fread(*pImage, size, 1, fp);  // ������ ������ ���� �о�´�.
    //*pImage += BytePerScanline * Height;

    // FILE*�� ����.
    fclose(fp);

    return true;
}

bool SaveBmp(const char* filename, byte* pImage, int width, int height)
{
    // DIB�� ������ �����Ѵ�.
    BITMAPINFO dib_define;
    dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    dib_define.bmiHeader.biWidth = width;
    dib_define.bmiHeader.biHeight = height;
    dib_define.bmiHeader.biPlanes = 1;
    dib_define.bmiHeader.biBitCount = 24;
    dib_define.bmiHeader.biCompression = BI_RGB;
    dib_define.bmiHeader.biSizeImage = (((width * 24 + 31) & ~31) >> 3) * height;
    dib_define.bmiHeader.biXPelsPerMeter = 0;
    dib_define.bmiHeader.biYPelsPerMeter = 0;
    dib_define.bmiHeader.biClrImportant = 0;
    dib_define.bmiHeader.biClrUsed = 0;

    // DIB ������ ��� ������ �����Ѵ�.
    BITMAPFILEHEADER dib_format_layout;
    ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
    dib_format_layout.bfType = *(WORD*)"BM";
    dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);// +dib_define.bmiHeader.biSizeImage;
    dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // ���� ����.
    FILE* fp = nullptr;
    fopen_s(&fp, filename, "wb");
    if (nullptr == fp)
    {
        LOG_OUT_A("fopen() error");
        return false;
    }

    // ���� �� ��� �� ������ ����.
    fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), fp);
    fwrite(&dib_define, 1, sizeof(BITMAPINFOHEADER), fp);
    fwrite(pImage, 1, dib_define.bmiHeader.biSizeImage, fp);
    fclose(fp);

    return true;
}