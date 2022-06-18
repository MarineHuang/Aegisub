#include "gtest/gtest.h"
#include "utils.h"
#include "vector"

using namespace SQ;

/*
TEST(string, join) 
{
    std::vector<std::string> str_vec;
    str_vec.push_back("ab");
    str_vec.push_back("cd");         
    std::string str = SQ::Join(str_vec, "-");
    ASSERT_STREQ(str.c_str(), "ab-cd");
}
*/

TEST(string, split) 
{
  auto result = SplitString("a b c d e f", " ");
  EXPECT_EQ(result.size(), 6);

  // contain a space
  result = SplitString("ab cd ef  gh", " ");
  EXPECT_EQ(result.size(), 5);

  // contain a space
  result = SplitString("ab cd ef  gh", " ", true);
  EXPECT_EQ(result.size(), 4);

  result = SplitString("abcd", " ");
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0], "abcd");

  result = SplitString("eabc\nasbd", "\n");
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], "eabc");

  result = SplitString("a\nb\n", "\n");
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[1], "b");

  // two seps
  result = SplitString("ea,bc\nas,bd", ",\n");
  EXPECT_EQ(result.size(), 4);
  EXPECT_EQ(result[1], "bc");
}
