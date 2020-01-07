#pragma once

#include <cstdint>
#include <memory>

namespace mabz {

#define i32 std::int32_t
#define i16 std::int16_t
#define ui8 std::uint8_t

#pragma pack(push, 2)
struct BmpFileHeader
{
	const char mBM[2]{'B', 'M'};
	i32 mFileSize{0};
	const i32 mReserved{0};
	i32 mDataOffset{0};

	BmpFileHeader() = default;
	BmpFileHeader(const BmpFileHeader&) = delete;
	BmpFileHeader(BmpFileHeader&&) = delete;
	~BmpFileHeader() = default;
};
#pragma pack(pop)

#pragma pack(push, 2)
struct BmpDataHeader
{
	const i32 mHeaderSize{40}; // size of this struct
	i32 mWidth{0};
	i32 mHeight{0};
	const i16 mPlanes{1};
	const i16 mBitsPerPixel{24};
	const i32 mCompression{0};
	i32 mDataSize{0};
	const i32 mHorizontalResolution{2400};
	const i32 mVorizontalResolution{2400};
	const i32 mColors{0};
	const i32 mImportantColors{0};

	BmpDataHeader() = default;
	BmpDataHeader(i32 width, i32 height, std::size_t dataSize) 
		: mWidth(width)
		, mHeight(height)
		, mDataSize(dataSize)
	{}
	BmpDataHeader(const BmpDataHeader&) = delete;
	BmpDataHeader(BmpDataHeader&&) = delete;
	~BmpDataHeader() = default;
};
#pragma pack(pop)

class Bmp
{
private:
	BmpFileHeader mFileHeader;
	BmpDataHeader mDataHeader;

	// This will be an array of uint8_t containing 3 entries for each RGB
	// pixel. So it will have dimension width x height x 3.
	// This 'agrees' with the mBitsPerPixel (== 24) property of the data header.
	const std::size_t mDataSize{0};
	std::unique_ptr<ui8[]> mData{nullptr};

public:
	Bmp(i32 width, i32 height)
		: mDataSize(3*width*height)
		, mDataHeader(width, height, mDataSize)
		, mData(new ui8[mDataSize]{0})
	{
		const std::size_t headersSize = sizeof(mFileHeader) + sizeof(mDataHeader);
		mFileHeader.mDataOffset = headersSize;
		mFileHeader.mFileSize = headersSize + mDataSize;
	}
	Bmp(const Bmp&) = delete;
	Bmp(Bmp&&) = delete;
	~Bmp() = default; // unique_ptr will auto-clean-up the memory.

	// Like an (x, y) plane with the origin at the bottom left of the image.
	void SetRGBPixel(i32 x, i32 y, ui8 r, ui8 g, ui8 b);

	i32 Width() const { return mDataHeader.mWidth; }
	i32 Height() const { return mDataHeader.mHeight; }
	
	bool WriteToFile(const char *filename) const;
};

#undef i32
#undef i16
#undef ui8

// Given an int (X, Y) pixel coordinate in a (W x H) bitmap, 
// map each pixel to an arbitrary cartesian (x, y) coordinate (e.g. for use
// plotting functions with arbitrary domain and range).
// Current implementation automatically keeps same scale/aspect ratio as the bitmap.
class PixelXYMapper
{
private:
	// cast on construction for easy division.
	const double mPixelWidth{0}; 
	const double mPixelHeight{0};

	// e.g. if going from (-1, 1) to (1, 4)
	// these would be calculated as -1, 2, 1, 3.
	const double mXMinimum{0};
	const double mXDomainWidth{0};
	const double mYMinimum{0};
	const double mYRangeHeight{0};

public:
	PixelXYMapper(
		double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight);

	// Simply a linear extrapolation based on distance from the midpoint of 
	// the range; does not bother to check whether the provided 'in' value is within source range!!
	void Convert(int xPixel, int yPixel, double& outXCoordinate, double& outYCoordinate) const;
};

} /* namespace mabz */
