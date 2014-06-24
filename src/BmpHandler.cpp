#include "BmpHandler.h"

#include <boost/format.hpp>

using namespace utils;

#pragma pack(2)

// Bmp file header structure
struct BmpFileHeader
{
    unsigned short bfType;           /* Magic number for file */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
};
static_assert( sizeof(BmpFileHeader) == 14, "BmpFileHeader size mismatch" );

#pragma pack()

#  define BF_TYPE 0x4D42             /* "MB" */

// bitmap information specific to image data
struct BmpInfoHeader
{
	unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
};

#  define BI_RGB       0             /* No compression - straight BGR data */
#  define BI_RLE8      1             /* 8-bit run-length compression */
#  define BI_RLE4      2             /* 4-bit run-length compression */
#  define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

// color palette
// I found unsigned char to be equivalent to byte
struct RgbQuad
{
	unsigned char  rgbBlue;          /* Blue value */
    unsigned char  rgbGreen;         /* Green value */
    unsigned char  rgbRed;           /* Red value */
    unsigned char  rgbReserved;      /* Reserved */
};

BmpHandler::BmpHandler(const std::string & path)
	: width_(0), rowSize_(0), height_(0), offset_(0)
	, filePtr_(fopen(path.c_str(), "r+b"), &fclose)
{
	if (!filePtr_)
	{
		throw std::runtime_error((boost::format("Failed to open bmp file %s") % path).str());
	}

    BmpFileHeader fh;
    if (fread(&fh, sizeof(BmpFileHeader), 1, filePtr_.get()) != 1)
	{
		throw std::runtime_error((boost::format("Failed to load file header. File %s") % path).str());
	}

	BmpInfoHeader ih;
    if (fread(&ih, sizeof(BmpInfoHeader), 1, filePtr_.get()) != 1)
	{
		throw std::runtime_error((boost::format("Failed to load info header. File %s") % path).str());
	}

	width_ = ih.biWidth;
	height_ = ih.biHeight;
	offset_ = fh.bfOffBits;

	rowSize_ = width_;
	if (const size_t mod = width_ % 4)
	{
		rowSize_ += 4 - mod;
	}

	// to avoid alignment handling
	//if (width_ % 4 != 0 || height_ % 4 != 0)
	//{
	//	throw std::runtime_error((boost::format("Image size is not m*4. Size is %d x %d") % width_ % height_).str());
	//}

	// to use unsigned char
	if (ih.biClrImportant > 256)
	{
		throw std::runtime_error((boost::format("Colour range is bigger than expected 256. Current is %d") % ih.biClrImportant).str());
	}

	// paranoid check
	if (ih.biSizeImage != rowSize_ * height_)
	{
		throw std::runtime_error("Image size is corrupted. width * height != size from file.");
	}
}

size_t
BmpHandler::GetPositionOffset(const size_t x, const size_t y) const
{
	return (height_ - y - 1) * rowSize_ + x;
}

void 
BmpHandler::Get(const size_t x, const size_t y, ValueType * pBuff, const size_t cnt) const
{
	//const size_t currentOffset = (height_ - y - 1) * rowSize_ + x;
	const size_t currentOffset = GetPositionOffset(x, y);

	if (currentOffset + cnt > rowSize_ * height_)
	{
		throw std::runtime_error((boost::format("Incorrect points amount requested. x = %d y = %d cnt = %d. Image size is width = %d height = %d") 
			% x % y % cnt % width_ % height_).str());
	}

	fseek(filePtr_.get(), offset_ + currentOffset, SEEK_SET);

	fread(pBuff, sizeof(ValueType), cnt, filePtr_.get());
}

void 
BmpHandler::Put(const size_t x, const size_t y, const ValueType * pBuff, const size_t cnt)
{
	const size_t currentOffset = GetPositionOffset(x, y);

	if (currentOffset + cnt > rowSize_ * height_)
	{
		throw std::runtime_error((boost::format("Incorrect points amount to be put. x = %d y = %d cnt = %d. Image size is width = %d height = %d") 
			% x % y % cnt % width_ % height_).str());
	}

	fseek(filePtr_.get(), offset_ + currentOffset, SEEK_SET);

	fwrite(pBuff, sizeof(ValueType), cnt, filePtr_.get());
}

size_t
BmpHandler::GetWidth() const
{
	return width_;
}

size_t
BmpHandler::GetHeight() const
{
	return height_;
}
