/*
 * test_parser.hh
 *
 *  Created on: Mar 04, 2020
 *      Author: David Flanderka
 */

#ifndef TIME_PROFILLER_HH_
#define TIME_PROFILLER_HH_

#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>

using namespace std;

/**
 * Class allows to print time durations of selected code blocks.
 *
 * Example of usage - appropriate macros are the simplifiest way:
 * @code
   #define N_LOOPS 1e7

   START_TIMER("some_complex_function");
   TestingObject obj;
   for (uint i=0; i<N_LOOPS; ++i)
       obj.some_complex_function(param0, ...);
   END_TIMER("some_complex_function");
 * @endcode
 *
 * Measured blocs of code can be nested.
 */
class TimeProfiler
{
public:
	typedef typename std::chrono::time_point<std::chrono::high_resolution_clock> ChronoTime;

    /// return singleton instance.
	static TimeProfiler& instance()
    {
        static TimeProfiler tp;
        return tp;
    }

	/// Start of time measuring.
    void start_tag(std::string name)
    {
        std::cout << std::setfill(' ') << std::setw(2*(level_+1)) << "" << "tag_" << name << std::endl;
        start_times_[level_] = std::chrono::high_resolution_clock::now();
        level_++;
    }

	/// End of time measuring.
    void end_tag(std::string name)
    {
        ChronoTime start_time = start_times_[level_-1];
        ChronoTime end_time = std::chrono::high_resolution_clock::now();
        double time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
        std::cout << std::setfill(' ') << std::setw(2*(level_+1)) << "" << "total_time_of_tag_" << name << ": " << time << std::endl;
        level_--;
    }

    TimeProfiler(TimeProfiler const&)    = delete;
    void operator=(TimeProfiler const&)  = delete;
private:
    /// Holds actual level of timer tag
    unsigned int level_;

    /// Holds start times of individual levels (nested measured blocks).
    std::array< ChronoTime, 20 > start_times_;

    /// Forbidden contructor.
    TimeProfiler()
    : level_(0) {}
};

// Definitions of timer macros
#define START_TIMER(tag) \
    TimeProfiler::instance().start_tag(tag);
#define END_TIMER(tag) \
    TimeProfiler::instance().end_tag(tag);


#endif /* TIME_PROFILLER_HH_ */
