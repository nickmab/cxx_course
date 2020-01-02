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
	 * The 3 individual RGB bytes need to be written in reverse order because
	 * bmp is a little endian file format where the least significant bytes
	 * are written first - picture a hex number 0xRRGGBB - the blue byte is
	 * the "least significant" (i.e. represents the smallest numeric value).
	 * Overflow is ignored - will simply mod the number if out of range.
	 */
	x %= mDataHeader.mWidth;
	y %= mDataHeader.mHeight;
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

} /* namespace mabz */