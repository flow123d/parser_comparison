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
   #define N_LOOPS 1e7
   TestingObject obj;

   START_TIMER(i_column_0, "name_of_row");
   for (uint i=0; i<N_LOOPS; ++i)
       obj.some_complex_function(param0, ...);
   END_TIMER(i_column_0, "name_of_row");
   START_TIMER(i_column_0, "name_of_other_row");
   for (uint i=0; i<N_LOOPS; ++i)
       obj.other_function(param0, ...);
   END_TIMER(i_column_0, "name_of_other_row");
   // loop with different columns and rows

   TimeProfiler::instance().output("out_file");
 * @endcode
 *
 * Arguments of macros are indices to table. Please, fill 'full' table for correct output.
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

    /// Print data to csv file.
    void output(const std::string &file_name) {
        std::cout << " ... print to '" + file_name + ".csv' file.\n";
        std::string full_file_name = "output/" + file_name + ".csv";
        std::ofstream ofs(full_file_name.c_str(), std::ofstream::out);

        // header
        ofs << "\"parser\",\"expr\",\"n_blocks\",\"block_size\",\"time\"" << std::endl;

        // data
        for (uint i=0; i<actual_tpoint_; ++i ) {
            ofs << "\"" << times[i].parser << "\",\"" << times[i].expression << "\"," << times[i].n_blocks << ",";
            ofs << times[i].block_size << "," << times[i].time << std::endl;
        }
        ofs.close();

        // reset pointer to data array
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
