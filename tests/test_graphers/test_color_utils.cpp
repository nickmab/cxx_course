#include <gtest/gtest.h>

#include <advanced_cxx/graphers/color_utils.h>

namespace {

namespace nscol = mabz::graphers::color;

class ColorTest : public testing::Test 
{
private:
	// nothing yet.	

public:
	// nothing yet.
};

TEST_F(ColorTest, TestRGBToHSV)
{
    {
	    nscol::RGB rgb{20, 55, 99};

	    // try the instance method...
	    nscol::HSV hsv = rgb.ToHSV();
	    EXPECT_NEAR(hsv.mHue, 213, 0.5);
	    EXPECT_NEAR(hsv.mSaturation, 0.798, 0.0005);
	    EXPECT_NEAR(hsv.mValue, 0.388, 0.0005);
	}
  
	{
  		// try the static method
  		nscol::HSV hsv = nscol::RGB::ToHSV(150, 19, 208);
  		EXPECT_NEAR(hsv.mHue, 282, 0.5);
	    EXPECT_NEAR(hsv.mSaturation, 0.909, 0.0005);
	    EXPECT_NEAR(hsv.mValue, 0.816, 0.0005);
	}
}

} /* anon namespace */
