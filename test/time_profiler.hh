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

private:
    struct DataTag {
        ChronoTime start_time;
        double time;
    };
public:
    /// return singleton instance.
	static TimeProfiler& instance()
    {
        static TimeProfiler tp;
        return tp;
    }

	/// Start of time measuring.
    void start_tag(uint npoints, std::string name)
    {
        std::map<std::string, uint>::iterator row_it = row_indices_.find(name);
        if (row_it == row_indices_.end()) { // add new row
            uint row_idx = row_indices_.size();
            row_indices_[name] = row_idx;
            row_it = row_indices_.find(name);
        }

        std::map<uint, uint>::iterator col_it = col_indices_.find(npoints);
        if (col_it == col_indices_.end()) { // add new row
            uint col_idx = col_indices_.size();
            col_indices_[npoints] = col_idx;
            col_it = col_indices_.find(npoints);
        }

        times[col_it->second][row_it->second].start_time = std::chrono::high_resolution_clock::now();
    }

	/// End of time measuring.
    void end_tag(uint npoints, std::string name)
    {
        std::map<std::string, uint>::iterator row_it = row_indices_.find(name);
        std::map<uint, uint>::iterator col_it = col_indices_.find(npoints);
        ChronoTime start_time = times[col_it->second][row_it->second].start_time;
        ChronoTime end_time = std::chrono::high_resolution_clock::now();
        times[col_it->second][row_it->second].time = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
    }

    /// Print data to csv file.
    void output(const std::string &file_name) {
    	std::cout << " ... print to '" + file_name + ".csv' file.\n";
    	std::string full_file_name = "output/" + file_name + ".csv";
        std::ofstream ofs(full_file_name.c_str(), std::ofstream::out);

        // header
        ofs << "\"\"";
        for (std::map<uint, uint>::iterator col_it = col_indices_.begin(); col_it != col_indices_.end(); ++col_it )
            ofs << "," << col_it->first;
        ofs << std::endl;

        // data
        for (std::map<std::string, uint>::iterator row_it = row_indices_.begin(); row_it != row_indices_.end(); ++row_it ) {
            ofs << "\"" << row_it->first << "\"";
            for (std::map<uint, uint>::iterator col_it = col_indices_.begin(); col_it != col_indices_.end(); ++col_it )
                ofs << "," << times[col_it->second][row_it->second].time;
		    ofs << std::endl;
        }
        ofs.close();

        // clean maps
        row_indices_.clear();
        col_indices_.clear();
    }

    TimeProfiler(TimeProfiler const&)    = delete;
    void operator=(TimeProfiler const&)  = delete;
private:
    /// Forbidden contructor.
    TimeProfiler() {}

    DataTag times[10][10];
    std::map<std::string, uint> row_indices_;
    std::map<uint, uint> col_indices_;
};

// Definitions of timer macros
#define START_TIMER(npoints, tag) \
    TimeProfiler::instance().start_tag(npoints, tag);
#define END_TIMER(npoints, tag) \
    TimeProfiler::instance().end_tag(npoints, tag);


#endif /* TIME_PROFILLER_HH_ */
