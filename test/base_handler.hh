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
	    //Profiler::initialize();

	    constantLine = "0.5";
        simpleLine = "x + y + z";
        complexLine = "2*x + y*3 + x*(z-y) + 2*pi*z";
        powerLine = "2*x + y^3 + x*(z-y) + 2*pi*z";

        funcPlus = "y+0.5";
        funcPower = "y^3";
        funcAbs = "abs(z)";
        funcExp = "exp(y)";
        funcLog = "log(y)";
        funcSin = "sin(pi*y)";
        funcAsin = "asin(z)";
        funcTernary = "z>0 ? x : y";
    	funcMax = "max(x,y,z)";

    	switch_off_func = true;
    }

    virtual ~BaseHandler() {
	    //Profiler::uninitialize();
    }

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

        this->parse_vector_fast(constantLine, "constant_parser");
        this->parse_vector_fast(simpleLine, "simple_parser");
        this->parse_vector_fast(complexLine, "complex_parser");
        this->parse_vector_fast(powerLine, "power_parser");
    }

    void run_function_tests(int vec_size) {
        init(vec_size);

        this->parse_vector_fast(funcPlus, "plus");
        this->parse_vector_fast(funcPower, "power");
        this->parse_vector_fast(funcAbs, "abs");
        this->parse_vector_fast(funcExp, "exp");
        this->parse_vector_fast(funcLog, "log");
        this->parse_vector_fast(funcSin, "sin");
        this->parse_vector_fast(funcAsin, "asin");
        if (this->switch_off_func) {
            this->parse_vector_fast(funcTernary, "ternary");
            this->parse_vector_fast(funcMax, "max");
        }
    }


    // data members
    int nLoops;
    int nBulkSize;
    std::vector<double> x_v;
    std::vector<double> y_v;
    std::vector<double> z_v;
    std::vector<double> result_v;

    std::string constantLine;
    std::string simpleLine;
    std::string complexLine;
    std::string powerLine;

    std::string funcPlus;
    std::string funcPower;
    std::string funcAbs;
    std::string funcExp;
    std::string funcLog;
    std::string funcSin;
    std::string funcAsin;
    std::string funcTernary;
	std::string funcMax;

	/// Allow switch-off test of ternary operator and max function unsupported in Bparser
	bool switch_off_func;
};

#endif /* BASE_HANDLER_HH_ */
