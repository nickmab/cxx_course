#ifndef ADVANCED_CXX_BMP_H
#define ADVANCED_CXX_BMP_H

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

	// void SetRGBPixel();

	bool WriteToFile(const char *filename) const;
};

#undef i32
#undef i16
#undef ui8

} /* namespace mabz */

#endif /* ADVANCED_CXX_BMP_H */