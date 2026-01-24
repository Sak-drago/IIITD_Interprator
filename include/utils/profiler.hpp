/**
 * @file profiler
 * @author Just Somebody
 * @date 10-01-2026
 * @brief Basic profiling utilities to see performance of code blocks.
 */

#pragma once

#ifdef PROFILE_ENABLED

#include <fstream>
#include <string>
#include <utils/dataTypes.hpp>
#include <utils/asserts.hpp>

namespace Utils 
{
  /// @brief A packet of data for profiling 
  typedef struct ProfileResult 
  {
    std::string name      = "Unknown";
    u64         start     = 0;
    u64         end       = 0;
    u64         threadId  = 0;
  } ProfileResult;

  /// @brief Measures time passed in microseconds
  class Timer 
  {
  private:
    u64         startTime;
    u64         endTime;
    std::string name;

  public:
    /**
     * @brief Create and start the timer
     * @param name The identifier for the profile result
     */
    Timer(const std::string& name);

    /// @brief Destructor that records the profile result
    ~Timer();

    /// @brief Starts the timer 
    void start();

    /**
     * @brief Calculate how long has the timer been running in microseconds
     * @return Time elapsed in microseconds
     */
    u64 elapsed();

    /**
     * @brief Get profile result 
     * @return A ProfileResult struct containing timing data
     */
    ProfileResult elapsedResult();
  };

  /// @brief The main profiler class, implemented as a singleton, master over all timers
  class Profiler 
  {
  private:
    std::ofstream resultJsonFile;
    bool          firstProfile;

    Profiler();
    void writeHeader();
    void writeFooter();

  public:
    /**
     * @brief Singleton access
     * @return Reference to the Profiler instance
     */
    static Profiler& Instance();

    /**
     * @brief Write the result of a single timer 
     * @param result The profile data to write to the JSON file
     */
    void writeProfile(const ProfileResult& result);

    /// @brief Destructor that closes the file and writes the footer
    ~Profiler();
    
    // - - -  Disable copying
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;
  };
  #define PROFILE_SCOPE(name) Utils::Timer timer##__LINE__(name)
  #define PROFILE PROFILE_SCOPE(__func__)
}

#else 
  #define PROFILE_SCOPE(name)
  #define PROFILE 
#endif
