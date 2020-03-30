This app has been deployed to github since version 2.0

The features regarding it are:
* Read Bitmaps of type (1,4,8,16,32) bits per pixel.
* Exports pixels to an html file.
* Very good error reporting.
* No external dependencies needed
* Code isn't OS dependent, so it is easily migratable to other OS platforms

The library has been added to a namespace called __BMPCont__ to resolve ambiguity if another library has the same name while the library is named __Bitmap__

To start using the library:

```cpp
#include "Bitmap.h"
using namespace BMPCont;
int main()
{
    Bitmap bmp;
    int code = bmp.Load("ourImage.bmp"); //Loads the image, can be done directly from the constructor
    if(code == bmp.CODE::LOADED)
        bmp.toHTML("outputedHTMLName"); //Outputs the html name, we can also check if any sort of error ever happens
    else
        std::cout << "Error";
	if (bmp.getLastError() == Bitmap::CODE::_NO_ERROR)
		std::cout << "No Error";
	else
		std::cout << bmp.getLastError();
    std::cin.get();
    return 0;
}
```
Important thing that you might observe is that I have allergies when it comes to the __using namespace std__
Upcoming features:
* Add Support for 2bit per pixel images
* Add RLE4 and RLE8 Support
* Fix getpixel
* Add a new function: Bitmap::CODE Bitmap::setPixel(UINT x,UINT y) which changes a pixel's value
	this function can return true if pixel has been changed or trigger an error which gives you a code
	that a pixel is not found.
* Add a new constructor so that we can make a custom image
* Add a new function: Bitmap::CODE Bitmap::convert_to(Bitmap from, UINT bpp), returns error if we cannot
	write to disk or any sort of other errors that we might encounter.
* Add a new function: Bitmap::CODE Bitmap::save(std::string filename), which saves the current data to a file
* Optimize the toHTML function
* Add a new class for shapes like drawing
* Add a new class for image manipulation like cutting, croping, combining images,...
* Add a new class for filters like color inversion,...
* Add a new class for some KO.ol mathematical shapes (to be explained later:)...
* Lastly, I'm planing on making other libraries for the frequently used image types (like jpeg, png,...)
