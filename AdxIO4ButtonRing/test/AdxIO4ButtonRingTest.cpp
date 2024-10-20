#include <gtest/gtest.h>
#include "./../src/AdxIO4ButtonRing.h"

TEST(AdxIO4ButtonRingTest, ExampleFunction)
{
    ASSERT_TRUE(true);
    AdxIO4ButtonRing obj;
    EXPECT_NO_THROW(obj.connect());
}