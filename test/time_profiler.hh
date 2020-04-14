/*
 * test_parser.hh
 *
 *  Created on: Mar 04, 2020
 *      Author: David Flanderka
 */

#ifndef TIME_PROFILLER_HH_
#define TIME_PROFILLER_HH_

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include <chrono>

using namespace std;

/**
 * Class allows to print time durations of selected code blocks.
 *
 * Output is printed to csv file. Data is arranged to table.
 * Example of usage - appropriate macros are the simplifiest way:
 * @code
   #define N_LOOPS 2000
   TestingObject obj;

   START_TIMER("abs(x)", N_LOOPS, 128); // expression, number of blocks, block size
   for (uint i=0; i<N_LOOPS; ++i)
       obj.some_complex_function(param0, ...);
   END_TIMER("abs(x)", N_LOOPS, 128);
   START_TIMER("sin(x)", N_LOOPS, 128);
   for (uint i=0; i<N_LOOPS; ++i)
       obj.other_function(param0, ...);
   END_TIMER("sin(x)", N_LOOPS, 128);
   // loop with different columns and rows

   obj.output("out_file");
 * @endcode
 */
class TimeProfiler
{
public:
    typedef typename std::chrono::time_point<std::chrono::high_resolution_clock> ChronoTime;

    struct DataTag {
    	std::string parser;
    	std::string expression;
    	int n_blocks;
    	int block_size;
        ChronoTime start_time;
        double time;
    };

    /// return singleton instance.
	static TimeProfiler& instance()
    {
        static TimeProfiler tp;
        return tp;
    }

	inline void set_parser(std::string parser)
	{
	    this->parser_ = parser;
	}

	/// Start of time measuring.
    void start_tag(std::string expression, int n_blocks, int block_size)
    {
        times[actual_tpoint_].parser = this->parser_;
        times[actual_tpoint_].expression = expression;
        times[actual_tpoint_].n_blocks = n_blocks;
        times[actual_tpoint_].block_size = block_size;
        times[actual_tpoint_].start_time = std::chrono::high_resolution_clock::now();
    }

	/// End of time measuring.
    void end_tag(std::string expression, int n_blocks, int block_size)
    {
        //BP_ASSERT( (times[actual_tpoint_].expression==expression)
        //        && (times[actual_tpoint_].n_blocks==n_blocks)
        //        && (times[actual_tpoint_].block_size==block_size) );
        ChronoTime start_time = times[actual_tpoint_].start_time;
        ChronoTime end_time = std::chrono::high_resolution_clock::now();
        times[actual_tpoint_].time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
        actual_tpoint_++;
    }

    /// Return data_tag of given idx
    inline DataTag data_tag(uint idx) const {
        return times[idx];
    }

    /// Return number of data tags
    inline unsigned int n_data_tags() const {
        return actual_tpoint_;
    }

    /// Reset data tags
    inline void reset_data_tags() {
        actual_tpoint_ = 0;
    }

    TimeProfiler(TimeProfiler const&)    = delete;
    void operator=(TimeProfiler const&)  = delete;
private:
    /// Forbidden contructor.
    TimeProfiler() : actual_tpoint_(0) {}

    DataTag times[100];    ///< Sets of timer data
    uint actual_tpoint_;   ///< Hold actual index of previous array
    std::string parser_;   ///< Hold parser name (it is not necessary to pass in each call)
};

// Definitions of timer macros
#define START_TIMER(expr, n_blocks, block_size) \
    TimeProfiler::instance().start_tag(expr, n_blocks, block_size);
#define END_TIMER(expr, n_blocks, block_size) \
    TimeProfiler::instance().end_tag(expr, n_blocks, block_size);


#endif /* TIME_PROFILLER_HH_ */
