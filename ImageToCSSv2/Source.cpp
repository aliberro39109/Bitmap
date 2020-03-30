#include <iostream>
#include <algorithm>
#include <chrono>
#include "Bitmap.h"

/*************************************************************
To do:
____Add Support for 2bit per pixel images
____Add RLE4 and RLE8 Support
____Fix getpixel
____Add a new function: Bitmap::CODE Bitmap::setPixel(UINT x,UINT y) which changes a pixel's value
	this function can return true if pixel has been changed or trigger an error which gives you a code
	that a pixel is not found.
____Add a new constructor so that we can make a custom image
____Add a new function: Bitmap::CODE Bitmap::convert_to(Bitmap from, UINT bpp), returns error if we cannot
	write to disk or any sort of other errors that we might encounter.
____Add a new function: Bitmap::CODE Bitmap::save(std::string filename), which saves the current data to a file
____Optimize the toHTML function
*************************************************************
____Add a new class for shapes like drawing
____Add a new class for image manipulation like cutting, croping, combining images,...
____Add a new class for filters like color inversion,...
____Add a new class for some KO.ol mathematical shapes (to be explained later:)...
*/
using namespace BMPCont;
using namespace std::chrono;
std::string getFileName(std::string filename)
{
	const size_t last_slash_idx = filename.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filename.erase(0, last_slash_idx + 1);
	}
	const size_t period_idx = filename.rfind('.');
	if (std::string::npos != period_idx)
	{
		filename.erase(period_idx);
	}
	return filename;
}

int main()
{
	std::string path;
	std::cout << "Give the full image name along with its path:\n";
	getline(std::cin, path); //read a string even with spaces
	path.erase(std::remove(path.begin(), path.end(), '\"'), path.end()); //remove quotes if user drags and drop image
	Bitmap bmp;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	int code = bmp.Load(path);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	std::cout << "It took " << time_span.count() << " seconds to load this file\n";
	if (code == bmp.CODE::LOADED)
		bmp.toHTML(getFileName(path));
	else
		std::cout << "ERROR";
	if (bmp.getLastError() == Bitmap::CODE::_NO_ERROR)
		std::cout << "No Error";
	else
		std::cout << bmp.getLastError();
	t1 = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(t1 - t2);
	std::cout << "\nAnd about " << time_span.count() << " seconds to convert it to html\n";
	std::cin.get();
	return 0;
}
