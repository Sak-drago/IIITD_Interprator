#include <utils/asserts.hpp>
#include <utils/testManager.hpp>

#ifndef DEBUG
  #define DEBUG 
#endif


static u8 sanityTest()
{
  i32 a = 2 + 2;
  EXPECT_TO_BE(4, a);

  const char* str = "hello";
  EXPECT_C_STRING_TO_BE("hello", str);

  f32 f = 0.1f + 0.2f;
  EXPECT_FLOAT_TO_BE(0.3f, f, 0.0001f);

  LOG_ERROR("Hello world");
  i32* p = nullptr;
  EXPECT_TO_BE_NULL(p);

  return 0; 
}

int main()
{
  registerTest(sanityTest, "Basic sanity test");
  runTests();
}
