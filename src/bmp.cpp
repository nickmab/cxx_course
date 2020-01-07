#include <fstream>

#include "advanced_cxx/bmp.h"

namespace mabz {

#define i32 std::int32_t
#define ui8 std::uint8_t
// Like an (x, y) plane with the origin at the bottom left of the image.
void Bmp::SetRGBPixel(i32 x, i32 y, ui8 r, ui8 g, ui8 b)
{
	/* The rows are written from the bottom to the top in the data array,
	 * i.e. first pixel is bottom left, last pixel is top right of the image.
	 * Overflow of x or y coordinates is ignored - simply mod the value if out of range.
	 */
	x %= mDataHeader.mWidth;
	y %= mDataHeader.mHeight;

	// go up by y rows and across by x columns...
	const int startByte = 3 * (y * mDataHeader.mWidth + x);
	
	/* The 3 individual RGB bytes need to be written in reverse order because
	 * bmp is a little endian file format where the least significant bytes
	 * are written first - picture a hex number 0xRRGGBB - the blue byte is
	 * the "least significant" (i.e. represents the smallest numeric value).
	 */
	mData[startByte] = b;
	mData[startByte + 1] = g;
	mData[startByte + 2] = r;
}
#undef i32
#undef ui8

bool Bmp::WriteToFile(const char *filename) const
{
	std::ofstream outFile;
	outFile.open(filename, std::ios::out | std::ios::binary);
	if (!outFile.is_open())
	{
		return false;
	}
	else
	{
		outFile.write(reinterpret_cast<const char*>(&mFileHeader), sizeof(mFileHeader));
		outFile.write(reinterpret_cast<const char*>(&mDataHeader), sizeof(mDataHeader));
		outFile.write(reinterpret_cast<const char*>(mData.get()), mDataSize);

		if (outFile.fail())
		{
			return false;
		}
		else
		{
			outFile.close();
			return true;
		}
	}
}

PixelXYMapper::PixelXYMapper(
	double xCenter, 
	double yCenter, 
	double xDomainWidth, 
	int pixelWidth,
	int pixelHeight)

	: mPixelWidth(static_cast<double>(pixelWidth))
	, mPixelHeight(static_cast<double>(pixelHeight))
	, mXMinimum(xCenter - 0.5 * xDomainWidth)
	, mXDomainWidth(xDomainWidth)
	, mYMinimum(yCenter - 0.5 * xDomainWidth * mPixelHeight / mPixelWidth)
	, mYRangeHeight(xDomainWidth * mPixelHeight / mPixelWidth)
{}

// Does not check for X or Y out of range. So just don't make mistakes.
void PixelXYMapper::Convert(int xPixel, int yPixel, double& outXCoordinate, double& outYCoordinate) const
{
	outXCoordinate = mXMinimum + xPixel * mXDomainWidth / mPixelWidth;
	outYCoordinate = mYMinimum + yPixel * mYRangeHeight / mPixelHeight;
}

} /* namespace mabz */