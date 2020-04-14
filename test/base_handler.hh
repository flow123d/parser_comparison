/*
 * test_parser.hh
 *
 *  Created on: Mar 17, 2020
 *      Author: David Flanderka
 */

#ifndef BASE_HANDLER_HH_
#define BASE_HANDLER_HH_

#include <cstring>
#include <string>
#include <vector>
#include "time_profiler.hh"

using namespace std;


class BaseHandler {
public:
	BaseHandler() {
	    constantLine = "0.5";
        simpleLine = "x + y + z";
        complexLine = "2*x + y*3 + x*(z-y) + 2*pi*z";
        powerLine = "2*x + y^3 + x*(z-y) + 2*pi*z";
	    constantLine_tag = constantLine;
        simpleLine_tag = simpleLine;
        complexLine_tag = complexLine;
        powerLine_tag = powerLine;

        funcPlus = "y+0.5";
        funcPower = "y^3";
        funcAbs = "abs(z)";
        funcExp = "exp(y)";
        funcLog = "log(y)";
        funcSin = "sin(pi*y)";
        funcAsin = "asin(z)";
        funcTernary = "z>0 ? x : y";
    	funcMax = "max(x,y,z)";
        funcPlus_tag = funcPlus;
        funcPower_tag = funcPower;
        funcAbs_tag = funcAbs;
        funcExp_tag = funcExp;
        funcLog_tag = funcLog;
        funcSin_tag = funcSin;
        funcAsin_tag = funcAsin;
        funcTernary_tag = funcTernary;
    	funcMax_tag = funcMax;

    	switch_off_func = true;
    }

    virtual ~BaseHandler() {}

    void create_data_vectors(int vec_size) {
        nBulkSize = vec_size;
        nLoops = 5000 * (2048 / vec_size);

        x_v.resize(nBulkSize);
        y_v.resize(nBulkSize);
        z_v.resize(nBulkSize);
        result_v.resize(nBulkSize);

        for (int i=0; i<nBulkSize; ++i) {
            x_v[i] = i;
            y_v[i] = (double)i/100 + 0.01;
            z_v[i] = (4.95 - (double)(i%10) * 1.1) * 0.2; // values oscillate in <-1; 1>
        }
    }

    virtual void init(int vec_size) =0;

    virtual double parse_vector_fast(std::string expr, std::string tag_name) =0;

    virtual void run_expression_tests(int vec_size) {
        init(vec_size);

        this->parse_vector_fast(constantLine, constantLine_tag);
        this->parse_vector_fast(simpleLine, simpleLine_tag);
        this->parse_vector_fast(complexLine, complexLine_tag);
        this->parse_vector_fast(powerLine, powerLine_tag);
    }

    void run_function_tests(int vec_size) {
        init(vec_size);

        this->parse_vector_fast(funcPlus, funcPlus_tag);
        this->parse_vector_fast(funcPower, funcPower_tag);
        this->parse_vector_fast(funcAbs, funcAbs_tag);
        this->parse_vector_fast(funcExp, funcExp_tag);
        this->parse_vector_fast(funcLog, funcLog_tag);
        this->parse_vector_fast(funcSin, funcSin_tag);
        this->parse_vector_fast(funcAsin, funcAsin_tag);
        if (this->switch_off_func) {
            this->parse_vector_fast(funcTernary, funcTernary_tag);
            this->parse_vector_fast(funcMax, funcMax_tag);
        }
    }

    /// Print data to csv file.
    void output(const std::string &file_name) {
        std::cout << " ... print to '" + file_name + ".csv' file.\n";
        std::string full_file_name = "output/" + file_name + ".csv";
        std::ofstream ofs(full_file_name.c_str(), std::ofstream::out);

        // header
        ofs << "\"parser\",\"expr\",\"n_blocks\",\"block_size\",\"time\"" << std::endl;

        // data
        for (uint i=0; i<TimeProfiler::instance().n_data_tags(); ++i ) {
            auto time = TimeProfiler::instance().data_tag(i);
            ofs << "\"" << time.parser << "\",\"" << time.expression << "\"," << time.n_blocks << ",";
            ofs << time.block_size << "," << time.time << std::endl;
        }
        ofs.close();

        // reset pointer to data array
        TimeProfiler::instance().reset_data_tags();
    }


    // data members
    int nLoops;
    int nBulkSize;
    std::vector<double> x_v;
    std::vector<double> y_v;
    std::vector<double> z_v;
    std::vector<double> result_v;

    // expressions passed by parsers (can be owerwrite in descendant)
    std::string constantLine;
    std::string simpleLine;
    std::string complexLine;
    std::string powerLine;

    // expressions strings to output (same for all parsers)
    std::string constantLine_tag;
    std::string simpleLine_tag;
    std::string complexLine_tag;
    std::string powerLine_tag;

    // functions passed by parsers (can be owerwrite in descendant)
    std::string funcPlus;
    std::string funcPower;
    std::string funcAbs;
    std::string funcExp;
    std::string funcLog;
    std::string funcSin;
    std::string funcAsin;
    std::string funcTernary;
	std::string funcMax;

    // functions strings to output (same for all parsers)
    std::string funcPlus_tag;
    std::string funcPower_tag;
    std::string funcAbs_tag;
    std::string funcExp_tag;
    std::string funcLog_tag;
    std::string funcSin_tag;
    std::string funcAsin_tag;
    std::string funcTernary_tag;
	std::string funcMax_tag;

	/// Allow switch-off test of ternary operator and max function unsupported in Bparser
	bool switch_off_func;
};

#endif /* BASE_HANDLER_HH_ */
