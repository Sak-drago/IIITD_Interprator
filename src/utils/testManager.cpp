#include "utils/asserts.hpp"
#include <utils/testManager.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

#ifndef _WIN32
  #include <unistd.h>
  #include <sys/wait.h>
#endif

// - - - Internal state

static std::vector<TestGroup> groups;


void registerTest(TEST FUNCTION, const std::string& DESCRIPTION, u8 GROUP_ID)
{
  if (groups.size() <= GROUP_ID) groups.resize(GROUP_ID + 1);
  groups[GROUP_ID].push_back({ FUNCTION, DESCRIPTION });
}

typedef struct TestStats
{
  u32 total   = 0;
  u32 passed  = 0;
  u32 skipped = 0;
  u32 failed  = 0;
  u32 crashed = 0;
} TestStats;


// - - - no fork exec on windows, sadly
#ifndef _WIN32
static bool runTestForked(
  const TestEntry&  TEST, 
  std::string&      OUTPUT, 
  u8&               RESULT)
{
  i32 pipefd[2];
  RUNTIME_ASSERT_MESSAGE(pipe(pipefd) == 0, "[TEST MANAGER] : Failed to pipe");

  pid_t pid = fork();
  if (pid == 0)
  {
    // - - - child
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);

    RESULT = TEST.func();
    std::exit(RESULT);
  }

  // - - - parent
  close(pipefd[1]);

  char buffer[256 * 1024];
  u64 n;
  while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) OUTPUT.append(buffer, buffer + n);

  close(pipefd[0]);

  i32 status = 0;
  waitpid(pid, &status, 0);

  if (WIFSIGNALED(status))    return false;

  RESULT = WEXITSTATUS(status);
  return true;
}
#endif


void runTests()
{
  TestStats          stats;
  std::ostringstream fullLog;

  for (const auto& group : groups)
  {
    for (const auto& test : group)
    {
      stats.total++;

      std::string output;
      u8 result = 0;

      #ifndef _WIN32
        bool ok = runTestForked(test, output, result);
        if (!ok)
        {
          stats.crashed++;
          fullLog << "[CRASHED] " << test.description << "\n";
          continue;
        }
      #else

        // - - - WARNING: Windows: no crash isolation
        std::streambuf*     old = std::cout.rdbuf();
        std::ostringstream  capture;
        std::cout.rdbuf(capture.rdbuf());

        result = test.func();

        std::cout.rdbuf(old);
        output = capture.str();
      #endif

      fullLog << output;

      if (result == 0)              stats.passed++;
      else if (result == SKIP_TEST) stats.skipped++;
      else                          stats.failed++;
    }
  }

  // - - - Summary

  std::cout << "\nTotal tests: " << stats.total << "\n"
            << "Passed: "      << stats.passed << "\n"
            << "Skipped: "     << stats.skipped << "\n"
            << "Failed: "      << stats.failed << "\n";
  #ifndef _WIN32
    std::cout << "Crashed: "     << stats.crashed << "\n";
  #endif

  
  
  std::cout << "\n\n\n- - - FULL LOG - - - \n\n" << fullLog.str();
}
