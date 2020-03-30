#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
namespace BMPCont
{
	typedef unsigned short WORD;
	typedef unsigned long DWORD;
	typedef unsigned int UINT;
	typedef unsigned short UINT16;
#define BITMAP_SIGNATURE 0x4d42
#define BITMAPFILEHEADERSIZE 14
#define BITMAPINFOHEADERSIZE 40
#define BITMAPV4HEADERSIZE  108
#define BITMAPV5HEADERSIZE  124
	//As we know that structs add padding if they were not in alignment, so this would cause problems for us when we're Reading/Writing
#pragma pack(push,1)		//pushes the current packing mode internally, and sets packing to 1, no padding
	struct _BITMAPFILEHEADER {
		short int bfType;			//Signature
		int bfSize;					//Size of the BMP in bytes
		int  bfReserved;			//Reserved; actual value depends on the application that creates the image
		UINT bfOffBits;				//The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.
	};
	struct _BITMAPINFOHEADER {
		UINT hSize;					//Size of this header
		int bWidth;					//Bitmap width in pixels
		int bHeight;				//Bitmap height in pixels
		UINT16 colPlanes;			//Number of color planes (must be 1)
		UINT16 bpp;					//Number of bits per pixel. Typical values are 1, 4, 8, 16, 24 and 32
		UINT compMethod;			//Compression method being used
		UINT imgSize;				//Image size
		UINT horRes;				//Horizontal resolution of the image
		UINT verRes;				//Vertical resolution of the image
		UINT nColPalette;			//Number of colors in the color palette, or 0 to default to 2^n
		UINT nColImport;			//Number of important colors used
	};
	enum _CompressionMethod
	{
		_BI_RGB = 0,				//No compression
		_BI_RLE8,					//Can be used only with 8-bit/pixel bitmaps
		_BI_RLE4,					//Can be used only with 4-bit/pixel bitmaps
		_BI_BITFIELDS,				//BITMAPV2INFOHEADER: RGB bit field masks, BITMAPV3INFOHEADER + : RGBA
		_BI_JPEG,					//BITMAPV4INFOHEADER+: JPEG image for printing
		_BI_PNG,					//BITMAPV4INFOHEADER+: PNG image for printing
		_BI_ALPHABITFIELDS,			//only Windows CE 5.0 with .NET 4.0 or later
		_BI_CMYK = 11,				//only Windows Metafile CMYK
		_BI_CMYKRLE8,				//only Windows Metafile CMYK
		_BI_CMYKRLE4				//only Windows Metafile CMYK
	};
	struct _BITMAPINFO
	{
		int Width;
		int Height;
		int FileSize;
		int ImageSize;
		int BitsPerPixel;
		UINT pixelArrayOffset;
	};
	struct RGBA
	{
		UINT Red;
		UINT Green;
		UINT Blue;
		UINT Alpha;
		RGBA()
		{
			Red = Green = Blue = Alpha = 0;
		}
		RGBA(UINT red, UINT green, UINT blue, UINT alpha)
		{
			Red = red; Green = green; Blue = blue; Alpha = alpha;
		}
		~RGBA() {}
	};
	struct BGRA
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t alpha;
	};
#pragma back(pop) //restores the previous packing
	class Bitmap
	{
	public:
		bool loaded = false;
		_BITMAPINFO BMPinfo;
		Bitmap();
		Bitmap(std::string& file);
		RGBA getPixel(int x, int y);
		~Bitmap();
		enum CODE
		{
			NOFILELOADED,
			FILECANNOTOPEN,
			CORRUPTBITMAP,
			UNKNOWNHEADER,
			ERRORCOLORPLANES,
			ERRORPNGCOMPRESSION,
			ERRORJPEGCOMPRESSION,
			ERRORMALLOC,
			NOPALETTE,
			UNKNOWN32BPP,
			INVALIDCOORDINATES,
			CANTCREATEFILE,
			UNKNOWNBPP,
			LOADED,
			HTMLEXPORTED,
			_NO_ERROR
		};
		CODE Load(std::string& filename);
		CODE Save(std::string& filename);
		CODE getLastError();
		CODE toHTML(std::string output); //Saves the data as a HTML file
		void setLastError(CODE c);
	private:
		FILE* file;
		CODE error = CODE::_NO_ERROR;
		_BITMAPFILEHEADER mBitmapFileHeader;
		_BITMAPINFOHEADER mBitmapInfoHeader;
		std::string _filename;
		RGBA* colors;
		void Read1();
		void Read4();
		void Read8();
		void Read16_555rgb();
		void Read16_555rgba();
		void Read16_565rgb();
		void Read24();
		void Read32_rgb();
		void Read32_rgba();
		unsigned int maskR, maskG, maskB, maskA;
		std::string digitToString(int n);
		std::string floatToString(float n);
		BGRA *palette = NULL;
	};
}