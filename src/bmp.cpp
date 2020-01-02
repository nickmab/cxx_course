#include <fstream>

#include "advanced_cxx/bmp.h"

namespace mabz {

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