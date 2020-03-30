#include "Bitmap.h"
namespace BMPCont
{
	Bitmap::Bitmap()
	{
		memset(&mBitmapFileHeader, 0, sizeof(struct _BITMAPFILEHEADER));
		memset(&mBitmapInfoHeader, 0, sizeof(struct _BITMAPINFOHEADER));
	}
	Bitmap::Bitmap(std::string& filename)
	{
		memset(&mBitmapFileHeader, 0, sizeof(struct _BITMAPFILEHEADER));
		memset(&mBitmapInfoHeader, 0, sizeof(struct _BITMAPINFOHEADER));
		Load(filename);
	}
	Bitmap::CODE Bitmap::getLastError()
	{
		return error;
	}
	void Bitmap::setLastError(CODE c)
	{
		error = c;
	}
	Bitmap::~Bitmap()
	{
		delete[] colors; //free up memory;
	}
	void Bitmap::Read1()
	{
		int row_size = ((BMPinfo.BitsPerPixel*BMPinfo.Width + 31) / 32) * 4;
		unsigned char* data = new unsigned char[row_size];
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
			hNeg = true;
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_size, file);
			for (int j = 0; j < BMPinfo.Width; j++, k++)
			{
				unsigned int bit = (*(data + j / 8) &(1 << (7 - j % 8))) >> (7 - j % 8);
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);

				colors[loc].Red = palette[bit].red;
				colors[loc].Green = palette[bit].green;
				colors[loc].Blue = palette[bit].blue;
				colors[loc].Alpha = 0xFF;
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	void Bitmap::Read4()
	{
		int row_size = ((BMPinfo.BitsPerPixel*BMPinfo.Width + 31) / 32) * 4;
		unsigned char* data = new unsigned char[row_size];
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
			hNeg = true;
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_size, file);
			for (int j = 0; j < BMPinfo.Width; j++, k++)
			{
				unsigned int bit;
				if (j % 2 == 0)
					bit = (int)(*(data + j / 2) >> 4);
				else
					bit = (int)(*(data + j / 2) & 0B00001111);
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);
				colors[loc].Red = palette[bit].red;
				colors[loc].Green = palette[bit].green;
				colors[loc].Blue = palette[bit].blue;
				colors[loc].Alpha = 0xFF;
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	void Bitmap::Read8()
	{
		int row_size = ((BMPinfo.BitsPerPixel*BMPinfo.Width + 31) / 32) * 4;
		unsigned char* data = new unsigned char[row_size];
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
			hNeg = true;
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_size, file);
			for (int j = 0; j < BMPinfo.Width; j++, k++)
			{
				unsigned int byte = (int)*(data + j);
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);
				colors[loc].Red = palette[byte].red;
				colors[loc].Green = palette[byte].green;
				colors[loc].Blue = palette[byte].blue;
				colors[loc].Alpha = 0xFF;
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	void Bitmap::Read16_555rgb()
	{
		int row_size = BMPinfo.Width * 2; //1 pixel per 2 bytes (WORD)
		unsigned char* data = new unsigned char[row_size];
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
		{
			hNeg = true; //This is used incase we have the image starting from the top to the bottom
		}
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_size, file);
			for (int j = 0; j < BMPinfo.Width * 2; j += 2, k++)
			{
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);
				unsigned short wrd = (int)*((unsigned short*)(data + j));
				colors[loc].Blue = (wrd & 0b11111) << 3;
				colors[loc].Green = (wrd >> 5 & 0b11111) << 3;
				colors[loc].Red = (wrd >> 10 & 0b11111) << 3;
				colors[loc].Alpha = 0xFF;
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	void Bitmap::Read16_555rgba()
	{
		int row_size = BMPinfo.Width * 2; //1 pixel per 2 bytes (WORD)
		unsigned char* data = new unsigned char[row_size];
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
		{
			hNeg = true; //This is used incase we have the image starting from the top to the bottom
		}
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_size, file);
			for (int j = 0; j < BMPinfo.Width * 2; j += 2, k++)
			{
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);
				unsigned short wrd = (int)*((unsigned short*)(data + j));
				colors[loc].Blue = (wrd & 0b11111) << 3;
				colors[loc].Green = (wrd >> 5 & 0b11111) << 3;
				colors[loc].Red = (wrd >> 10 & 0b11111) << 3;
				colors[loc].Alpha = (wrd >> 15) == 1 ? 0xFF : 0;
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	void Bitmap::Read16_565rgb()
	{
		int row_size = BMPinfo.Width * 2; //1 pixel per 2 bytes (WORD)
		unsigned char* data = new unsigned char[row_size];
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
		{
			hNeg = true; //This is used incase we have the image starting from the top to the bottom
		}
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_size, file);
			for (int j = 0; j < BMPinfo.Width * 2; j += 2, k++)
			{
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);
				unsigned short wrd = (int)*((unsigned short*)(data + j));
				colors[loc].Blue = (wrd & 0b11111) << 3;
				colors[loc].Green = (wrd >> 5 & 0b111111) << 2;
				colors[loc].Red = (wrd >> 11 & 0b11111) << 3;
				colors[loc].Alpha = 0xFF;
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	void Bitmap::Read24()
	{
		int row_padded = BMPinfo.Width * 3 + (4 - (BMPinfo.Width * 3) % 4) % 4;
		unsigned char* data = new unsigned char[row_padded]; //padding since we store the pixels as multiple of 4
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
		{
			hNeg = true; //This is used incase we have the image starting from the top to the bottom
		}
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_padded, file);
			for (int j = 0; j < BMPinfo.Width * 3; j += 3, k++)
			{
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);
				colors[loc].Blue = (unsigned char)data[j];
				colors[loc].Green = (unsigned char)data[j + 1];
				colors[loc].Red = (unsigned char)data[j + 2];
				colors[loc].Alpha = 0xFF;
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	void Bitmap::Read32_rgb()
	{
		int row_size = BMPinfo.Width * 4; //1 pixel per 4 bytes (WORD)
		unsigned char* data = new unsigned char[row_size];
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
		{
			hNeg = true; //This is used incase we have the image starting from the top to the bottom
		}
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_size, file);
			for (int j = 0; j < BMPinfo.Width * 4; j += 4, k++)
			{
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);
				unsigned long dw = (int)*((unsigned long*)(data + j));
				colors[loc].Blue = (dw & 0b11111111);
				colors[loc].Green = (dw >> 8 & 0b11111111);
				colors[loc].Red = (dw >> 16 & 0b11111111);
				colors[loc].Alpha = 0xFF;
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	void Bitmap::Read32_rgba()
	{
		int row_size = BMPinfo.Width * 4; //1 pixel per 4 bytes (WORD)
		unsigned char* data = new unsigned char[row_size];
		colors = new RGBA[BMPinfo.Width*BMPinfo.Height];
		bool hNeg = false;
		if (mBitmapInfoHeader.bHeight < 0)
		{
			hNeg = true; //This is used incase we have the image starting from the top to the bottom
		}
		for (int i = 0, k = 0; i < BMPinfo.Height; i++)
		{
			fread(data, sizeof(unsigned char), row_size, file);
			for (int j = 0; j < BMPinfo.Width * 4; j += 4, k++)
			{
				int loc = k + (hNeg ? (BMPinfo.Height - i - 1) * (BMPinfo.Width) : 0);
				unsigned long dw = (int)*((unsigned long*)(data + j));
				colors[loc].Blue = (dw & 0b11111111);
				colors[loc].Green = (dw >> 8 & 0b11111111);
				colors[loc].Red = (dw >> 16 & 0b11111111);
				colors[loc].Alpha = (dw >> 24 & 0b11111111);
			}
			if (hNeg)
				k = 0;
		}
		delete[] data;
	}
	RGBA Bitmap::getPixel(int x, int y)
	{
		if (x < 0 || y < 0 || x >= BMPinfo.Width || y >= BMPinfo.Height)
		{
			error = CODE::INVALIDCOORDINATES;
			return RGBA(0, 0, 0, 0);
		}
		if (!loaded) //check if a file has been loaded before sending any pixel information
		{
			error = CODE::NOFILELOADED;
			return RGBA(0, 0, 0, 0);
		}
		UINT loc = x + (BMPinfo.Height - y - 1) * (BMPinfo.Width);
		return RGBA(colors[loc].Red, colors[loc].Green, colors[loc].Blue, colors[loc].Alpha);
	}
	std::string Bitmap::digitToString(int n)
	{
		std::string x = "";
		while (n != 0)
		{
			int r = n % 10;
			n /= 10;
			x.append(std::to_string(r));
		}
		std::reverse(x.begin(), x.end());
		if (x == "")
			return "0";
		return x;
	}
	std::string Bitmap::floatToString(float n)
	{
		std::ostringstream ss;
		ss << n;
		return ss.str();
	}
	Bitmap::CODE Bitmap::toHTML(std::string output)
	{
		if (!loaded)
		{
			return error = CODE::NOFILELOADED;
		}
		FILE* out = fopen((output + ".html").c_str(), "w");
		if (out == NULL)
		{
			return error = CODE::CANTCREATEFILE;
		}
		fputs("<html><head><title>", out);
		fputs(output.c_str(), out);
		fputs("</title><style>", out);
		std::string style = "", html = "";
		std::vector<std::string> v; //for optimization
		unsigned long k = 0;
		for (int i = 0; i < BMPinfo.Height; i++)
		{
			html.append("<div>");
			for (int j = 0; j < BMPinfo.Width; j++)
			{
				html.append("<span class='a");
				RGBA pixel = getPixel(j, i);
				std::string r = digitToString(pixel.Red), g = digitToString(pixel.Green), b = digitToString(pixel.Blue), a = floatToString(float(pixel.Alpha) / 255.0);
				html.append(r + g + b);
				html.append("'></span>");
				if (std::find(v.begin(), v.end(), (std::string)(r + g + b + a)) == v.end())
				{
					style.append(".a" + r + g + b + "{background-color:rgba(" + r + "," + g + "," + b + "," + a + ");height:1px;width:1px;display:table-cell;}");
					v.push_back(r + g + b + a);
				}
				k += 3;
			}
			html.append("</div>");
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		fputs(style.c_str(), out);
		fputs(std::string("</style></head><body><div style='width:" + digitToString(BMPinfo.Width) + "px;height:" + digitToString(BMPinfo.Height) + "px;'>").c_str(), out);
		fputs(html.c_str(), out);
		fputs("</div></body></html>", out);
		fclose(out);
		return CODE::HTMLEXPORTED;
	}
	Bitmap::CODE Bitmap::Load(std::string& filename)
	{
		loaded = false;
		_filename = filename;
		file = fopen(filename.c_str(), "rb");
		if (file == NULL)
		{
			return error = CODE::FILECANNOTOPEN;
		}
		fread((void*)&mBitmapFileHeader, BITMAPFILEHEADERSIZE, 1, file);
		if (mBitmapFileHeader.bfType != BITMAP_SIGNATURE)
		{
			fclose(file);
			return error = CODE::CORRUPTBITMAP;
		}
		fread((void*)&mBitmapInfoHeader, BITMAPINFOHEADERSIZE, 1, file);
		/********************Errors that we might face**********************************/
		if (mBitmapInfoHeader.hSize != BITMAPINFOHEADERSIZE && mBitmapInfoHeader.hSize != BITMAPV4HEADERSIZE && mBitmapInfoHeader.hSize != BITMAPV5HEADERSIZE)
		{
			fclose(file);
			return error = CODE::UNKNOWNHEADER;
		}
		if (mBitmapInfoHeader.colPlanes != 1)
		{
			fclose(file);
			return error = CODE::ERRORCOLORPLANES;
		}
		if (mBitmapInfoHeader.compMethod == _CompressionMethod::_BI_PNG)
		{
			fclose(file);
			return error = CODE::ERRORPNGCOMPRESSION;
		}
		if (mBitmapInfoHeader.compMethod == _CompressionMethod::_BI_PNG)
		{
			fclose(file);
			return error = CODE::ERRORJPEGCOMPRESSION;
		}
		BMPinfo.BitsPerPixel = mBitmapInfoHeader.bpp;
		BMPinfo.FileSize = mBitmapFileHeader.bfSize;
		BMPinfo.Height = abs(mBitmapInfoHeader.bHeight);
		BMPinfo.ImageSize = mBitmapInfoHeader.imgSize;
		BMPinfo.pixelArrayOffset = mBitmapFileHeader.bfOffBits;
		BMPinfo.Width = mBitmapInfoHeader.bWidth;

		//Load the Color Header
		if (mBitmapInfoHeader.bpp <= 8)
		{
			fseek(file, BITMAPFILEHEADERSIZE + mBitmapInfoHeader.hSize, SEEK_SET);
			//Below we will force to have a color palette, since after conversion, some converters forget to add nColPalette value
			if (mBitmapInfoHeader.bpp == 1) mBitmapInfoHeader.nColPalette = 2;
			if (mBitmapInfoHeader.bpp == 4) mBitmapInfoHeader.nColPalette = 16;
			if (mBitmapInfoHeader.bpp == 8) mBitmapInfoHeader.nColPalette = 256;
			palette = (BGRA*)malloc(mBitmapInfoHeader.nColPalette * sizeof(BGRA)); //its bad to use delete on malloc
			if (!palette)
			{
				fclose(file);
				return error = CODE::ERRORMALLOC;
			}
			else if (BITMAPFILEHEADERSIZE + mBitmapInfoHeader.hSize + mBitmapInfoHeader.nColPalette * 4 < mBitmapFileHeader.bfOffBits)
			{
				return error = CODE::NOPALETTE;
			}
			else
				fread((void*)palette, sizeof(BGRA), mBitmapInfoHeader.nColPalette, file);
		} //16bpp color tables can be used as compression method, but we won't supported

		//Load Image Data
		if (mBitmapInfoHeader.compMethod == _CompressionMethod::_BI_BITFIELDS) //Get bit Masks for RGBA
		{
			fseek(file, 0x36, SEEK_SET);
			fread(&maskR, sizeof(maskR), 1, file);
			fread(&maskG, sizeof(maskR), 1, file);
			fread(&maskB, sizeof(maskR), 1, file);
			fread(&maskA, sizeof(maskR), 1, file);
		}
		fseek(file, mBitmapFileHeader.bfOffBits, SEEK_SET);
		switch (mBitmapInfoHeader.bpp)
		{
		case 32:	//32 bit Colors
			if (mBitmapInfoHeader.compMethod == _CompressionMethod::_BI_RGB) // 32bpp RGB
				Read32_rgb();
			else if (mBitmapInfoHeader.compMethod == _CompressionMethod::_BI_BITFIELDS
				&& maskR == 0xFF0000 && maskG == 0xFF00 && maskB == 0xFF && maskA == 0xFF000000) //32 RGBA
				Read32_rgba();
			break;
		case 24:	//24 bit Colors
			Read24();
			break;
		case 16:	//16 bit Colors
			if (mBitmapInfoHeader.compMethod == _CompressionMethod::_BI_RGB) // 16 bpp 5:5:5:1 RGB
				Read16_555rgb();
			else if (mBitmapInfoHeader.compMethod == _CompressionMethod::_BI_BITFIELDS && maskR == 0xF800 && maskG == 0x7E0 && maskB == 0x1F) //16 bpp 5:6:5
				Read16_565rgb();
			else // 16 bpp 5:5:5:1 RGBA
				Read16_555rgba();
			break;
		case 8:		//256 Colors
			Read8();
			break;
		case 4:		//16 Colors
			Read4();
			break;
		case 1:		//Monochrome
			Read1();
			break;
		default:
			return error = CODE::UNKNOWNBPP;
			break; //no need for break but just being fancy with C++
		}
		loaded = true;
		fclose(file);
		return CODE::LOADED;
	}
	Bitmap::CODE Bitmap::Save(std::string& filename)
	{
		//Not Finished
		return CODE::_NO_ERROR;
		FILE* f = fopen("m.bmp", "w");
		_BITMAPFILEHEADER header;
		header.bfType = BITMAP_SIGNATURE;
		header.bfReserved = 0B01000010010011010101000000100001; //BMP!
		header.bfOffBits = BITMAPFILEHEADERSIZE + BITMAPINFOHEADERSIZE;
		_BITMAPINFOHEADER inHeader;
		inHeader.bHeight = 360;
		inHeader.bpp = 24;
		inHeader.bWidth = 360;
		inHeader.colPlanes = 1;
		inHeader.compMethod = 0;
		inHeader.verRes = 2835;
		inHeader.horRes = 2835;
		inHeader.hSize = 40;
		inHeader.imgSize = (inHeader.bWidth * 3 + (4 - (inHeader.bWidth * 3) % 4) % 4)*inHeader.bHeight;
		inHeader.nColImport = 0;
		inHeader.nColPalette = 0;
		header.bfSize = inHeader.hSize + BITMAPFILEHEADERSIZE + BITMAPINFOHEADERSIZE;
		//	fwrite((char*)header, sizeof(_BITMAPFILEHEADER), 1, f);
	}
}