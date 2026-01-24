#include <utils/profiler.hpp>
#include <iostream>
#include <cmath>

void slowFunction() 
{
  PROFILE; // Automatically uses function name
  for (int i = 0; i < 1000000; ++i) 
  {
    int j = std::sqrt(i);
    j = j + 1;
  }
}

int main() 
{
  LOG_INFO("Starting Profiler Test...");

  {
    PROFILE_SCOPE("Entire Main Loop");

    slowFunction();

    {
      PROFILE_SCOPE("Manual Scope Block");
      for (int i = 0; i < 100; ++i) 
      {
        // Simulate minor work
      }
    }

  }

  LOG_INFO("Profiling complete. Check result.json for output.");
  return 0;
}
