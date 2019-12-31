#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include <advanced_cxx/cmplx_num.h>

using namespace mabz;
using namespace std;

class CmplxTest : public ::testing::Test {
    protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.

    CmplxTest() {
        // You can do set-up work for each test here.
    }

    ~CmplxTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Class members declared here can be used by all tests in the test suite
    // for Foo.
};

TEST_F(CmplxTest, AbsFunction) {
    Cmplx c(2, -2);
    ASSERT_DOUBLE_EQ(c.Abs(), 2.82842712474619);
}

TEST_F(CmplxTest, AssignOp) {
    Cmplx c1(-0.5, 0.1);
    Cmplx c2(0, 0);
    c2 = c1;
    ASSERT_DOUBLE_EQ(c2.Re(), -0.5);
    ASSERT_DOUBLE_EQ(c2.Im(), 0.1);
}

TEST_F(CmplxTest, DerefOp) {
    Cmplx c = *Cmplx(2.0, -0.5);
    ASSERT_DOUBLE_EQ(c.Re(), 2.0);
    ASSERT_DOUBLE_EQ(c.Im(), 0.5);
}

TEST_F(CmplxTest, BoolOpNonZero) {
    Cmplx c(0, -0.5);
    ASSERT_TRUE(c);
}

TEST_F(CmplxTest, BoolOpZero) {
    Cmplx c;
    ASSERT_FALSE(c);
}

TEST_F(CmplxTest, EqualityOpEQ) {
    Cmplx c1(0.3, 9);
    Cmplx c2(0.3, 9);
    ASSERT_TRUE(c1 == c2);
}

TEST_F(CmplxTest, EqualityOpNE) {
    Cmplx c1(0.3, 9);
    Cmplx c2(0.3, -9);
    ASSERT_FALSE(c1 == c2);
}

TEST_F(CmplxTest, InequalityOpEQ) {
    Cmplx c1(3.5, 0);
    Cmplx c2(3.5, 0);
    ASSERT_FALSE(c1 != c2);
}

TEST_F(CmplxTest, InequalityOpNE) {
    Cmplx c1(0, 1);
    Cmplx c2(0, -1);
    ASSERT_TRUE(c1 != c2);
}

TEST_F(CmplxTest, PutsToStreamPositive) {
    Cmplx c(1.2, 2.3);
    stringstream s;
    s << c;
    ASSERT_EQ(s.str().substr(0, 12), string("(1.2 + 2.3i)"));
}

TEST_F(CmplxTest, PutsToStreamNegative) {
    Cmplx c(-1.2, -2.3);
    stringstream s;
    s << c;
    ASSERT_EQ(s.str().substr(0, 13), string("(-1.2 - 2.3i)"));
}

TEST_F(CmplxTest, Addition) {
    Cmplx c1(3.5, 0);
    Cmplx c2(-3.5, 0);
    Cmplx c3(0, 0.2);
    Cmplx c4(0, -0.2);
    ASSERT_EQ(c1 + c2 + c3 + c4, Cmplx());
}

TEST_F(CmplxTest, Subtraction) {
    Cmplx c1(3.5, 0);
    Cmplx c2(-3.5, 0);
    Cmplx c3(0, 0.2);
    Cmplx c4(0, -0.2);
    ASSERT_EQ(-c1 - c2 - c3 - c4, Cmplx());
}