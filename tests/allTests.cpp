#include "allTests.h"
#include "../library/include/asserts.h"
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
  #ifdef __gnu_linux__

  const char* executables[] = 
  {
    "tokenizerTests"
  };

  int numExe = sizeof(executables) / sizeof(executables[0]);

  for (int i = 0; i < numExe; ++i)
  {
    pid_t pid = fork();
    FORGE_ASSERT_MESSAGE(pid >= 0, "Fork cannot fail");

    if (pid == 0)
    {
      FORGE_LOG_INFO("Executing test : %s\n", executables[i]);
      execl(executables[i], executables[i], (char*) NULL);

      FORGE_ASSERT_MESSAGE(true, "If you see this message, the executable doesnt exist");
    }

    else 
    {
      int status;
      waitpid(pid, &status, 0);
    }
  }

  FORGE_LOG_INFO("All tests completed");

  #elif 
    FORGE_LOG_INFO("I dont know how to fork, exec and wait on windows");
  #endif
  return 0;
}
