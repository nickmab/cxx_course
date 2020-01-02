#include "advanced_cxx/color_map.h"

#include <algorithm>
#include <cassert>

namespace mabz { namespace color {

// Once we fill up the lookup table, sort it such that the entries are ascending 
// by the LBound threshold so we can easily lookup the colour later.
bool BasicTable::SortTable()
{
	assert (mCurrentNumberOfEntries == mMaxNumberOfEntries);

	std::sort(mLookupTable.begin(), mLookupTable.end());
	bool containsDuplicates = false;
	
	std::vector<LookupTableEntry>::const_iterator entry = mLookupTable.begin();
	unsigned prevLowerBound = entry->mLowerBound;
	++entry;
	for ( ; entry != mLookupTable.end(); ++entry)
	{
		containsDuplicates |= entry->mLowerBound == prevLowerBound;
		prevLowerBound = entry->mLowerBound;
	}
	mInitialised = !containsDuplicates;
	return mInitialised;
}

#define ui8 std::uint8_t

bool BasicTable::AddEntry(unsigned lowerBound, ui8 r, ui8 g, ui8 b)
{
	if (mInitialised || mCurrentNumberOfEntries >= mMaxNumberOfEntries)
	{
		return false;
	}
	else
	{
		LookupTableEntry& entry = mLookupTable[mCurrentNumberOfEntries];
		entry.mLowerBound = lowerBound;
		entry.mRed = r;
		entry.mGreen = g;
		entry.mBlue = b;
		
		if (++mCurrentNumberOfEntries == mMaxNumberOfEntries)
		{
			return SortTable();
		}
		else
		{
			return true;
		}
	}
}

bool BasicTable::GetColor(unsigned input, ui8& rOut, ui8& gOut, ui8& bOut) const
{
	if (!mInitialised)
	{
		return false;
	}
	else
	{
		std::vector<LookupTableEntry>::const_iterator entry = mLookupTable.begin();
		if (input < entry->mLowerBound)
		{
			rOut = gOut = bOut = 0;
		}
		else
		{
			const LookupTableEntry* found = &(*entry);
			++entry;
			
			for ( ; entry != mLookupTable.end(); ++entry)
			{
				if (input >= entry->mLowerBound)
				{
					found = &(*entry);
				}
				else
				{
					break;
				}
			}

			rOut = found->mRed;
			gOut = found->mGreen;
			bOut = found->mBlue;
		}
		return true;
	}
}

#undef ui8

std::ostream& operator << (std::ostream& os, const BasicTable& bt)
{
	std::cout << "BasicTable mMaxNumberOfEntries( " << bt.mMaxNumberOfEntries << ") "
			  << "mCurrentNumberOfEntries( " << bt.mCurrentNumberOfEntries << ") "
			  << "mInitialised( " << bt.mInitialised << ") "
			  << std::endl;

	for (auto& entry : bt.mLookupTable)
	{
		std::cout << "mLowerBound(" << (int)entry.mLowerBound << ") "
				  << "mRed(" << (int)entry.mRed << ") "
				  << "mGreen(" << (int)entry.mGreen << ") "
				  << "mBlue(" << (int)entry.mBlue << ") ";
				  
		if (&entry != &bt.mLookupTable[bt.mMaxNumberOfEntries - 1])
		{
			std::cout << std::endl;
		}
	}

	return os;
}

} /* namespace color */
} /* namespace mabz */