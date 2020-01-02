#ifndef ADVANCED_CXX_COLOR_MAP_H
#define ADVANCED_CXX_COLOR_MAP_H

#include <cstdint>
#include <iostream>
#include <vector>

namespace mabz { namespace color {

#define ui8 std::uint8_t

// Just getting started here, let's have a kind of lookup table where we define the 
// "score" that gets you into a certain colour zone and the corresponding colour of that zone.
class BasicTable
{
private:
	struct LookupTableEntry
	{
		unsigned mLowerBound{0};
		ui8 mRed{0};
		ui8 mGreen{0};
		ui8 mBlue{0};

		bool operator < (const LookupTableEntry& other) const { return mLowerBound < other.mLowerBound; }
	};

	const int mMaxNumberOfEntries{0};
	int mCurrentNumberOfEntries{0};
	std::vector<LookupTableEntry> mLookupTable{};

	// Once the table is fully populated with the colour zones, sort ascending for easy lookup.
	// returning false indicates invalidity of the table; only current invalidity is duplicate lower bounds.
	bool SortTable();
	bool mInitialised{false};

public:
	BasicTable() = delete;
	BasicTable(int size) 
		: mMaxNumberOfEntries(size)
		, mLookupTable(size)
	{}
	BasicTable(const BasicTable&) = delete;
	~BasicTable() = default;

	// false indicates failure - the table is already full.
	bool AddEntry(unsigned lowerBound, ui8 r, ui8 g, ui8 b);
	// false indicates failure - the table is not yet correctly initialised.
	// automatically returns black if the input value is below the min lower bound defined.
	bool GetColor(unsigned input, ui8& rOut, ui8& gOut, ui8& bOut) const;

	friend std::ostream& operator << (std::ostream&, const BasicTable&);
};

#undef ui8

} /* namespace color */
} /* namespace mabz */

#endif /* ADVANCED_CXX_COLOR_MAP_H */