/*
 * bparser_handler.hh
 *
 *  Created on: Mar 19, 2020
 *      Author: David Flanderka
 */

#ifndef BPARSER_HANDLER_HH_
#define BPARSER_HANDLER_HH_

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <limits>
#include <ios>
#include <iomanip>
#include <numeric>

#include "base_handler.hh"
#include "include/assert.hh"
#include "include/parser.hh"
#include "test/test_tools.hh"

using namespace std;
using namespace bparser;


class BParserHandler : public BaseHandler {
public:
	BParserHandler() : BaseHandler() {
	    // rewrite power function to correct Bparser format
		powerLine = "2*x + pow(y, 3) + x*(z-y) + 2*pi*z";
        funcPower = "pow(y, 3)";
	}

    void create_data_vectors(int vec_size) {
        nBulkSize = vec_size;
        nLoops = 5000 * (2048 / vec_size);
        subsets.resize(vec_size/4);

        x_v.resize(nBulkSize);
        y_v.resize(nBulkSize);
        z_v.resize(nBulkSize);
        result_v.resize(nBulkSize);

        for (int i=0; i<nBulkSize; ++i) {
            x_v[i] = i;
            y_v[i] = (double)i/100 + 0.01;
            z_v[i] = (4.95 - (double)(i%10) * 1.1) * 0.2; // values oscillate in <-1; 1>
        }

        for (uint i=0; i<vec_size/4; ++i) {
            subsets[i] = i;
        }
    }

    double cpp_compute_constant() {
    	double sum = 0.0;
        START_TIMER(nBulkSize, "constant_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 0.5;
            }
        }
        END_TIMER(nBulkSize, "constant_cpp");
        for (int i=0; i<nBulkSize; ++i) {
        	sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_simple() {
    	double sum = 0.0;
        START_TIMER(nBulkSize, "simple_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = x_v[i] + y_v[i] + z_v[i];
            }
        }
        END_TIMER(nBulkSize, "simple_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER(nBulkSize, "complex_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 2*x_v[i] + y_v[i]*3 + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER(nBulkSize, "complex_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex_power() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER(nBulkSize, "power_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
            	result_v[i] = 2*x_v[i] + pow( y_v[i], 3.0 ) + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER(nBulkSize, "power_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double parse_vector_fast(std::string expr, std::string tag_name) override {
        double sum = 0.0;

        Parser p(nBulkSize);
        p.parse(expr);
        p.set_variable("x", {}, &x_v[0]);
        p.set_variable("y", {}, &y_v[0]);
        p.set_variable("z", {}, &z_v[0]);
        p.set_variable("_result_", {}, &result_v[0]);
        p.compile();
        p.set_subset(subsets);

        START_TIMER(nBulkSize, tag_name);
        for (int j=0; j<nLoops; ++j) {
            p.run();
        }
        END_TIMER(nBulkSize, tag_name);
        for (int i=0; i<nBulkSize; ++i) sum += result_v[i];

        return sum;
    }

    void run_expression_tests(int vec_size) {
        create_data_vectors(vec_size);

        this->cpp_compute_constant();
        this->parse_vector_fast(constantLine, "constant_parser");

        this->cpp_compute_simple();
        this->parse_vector_fast(simpleLine, "simple_parser");

        this->cpp_compute_complex();
        this->parse_vector_fast(complexLine, "complex_parser");

        this->cpp_compute_complex_power();
        this->parse_vector_fast(powerLine, "power_parser");
    }

    void run_function_tests(int vec_size) {
        create_data_vectors(vec_size);

        this->parse_vector_fast(funcPlus, "plus");
        this->parse_vector_fast(funcPower, "power");
        this->parse_vector_fast(funcAbs, "abs");
        this->parse_vector_fast(funcExp, "exp");
        this->parse_vector_fast(funcLog, "log");
        this->parse_vector_fast(funcSin, "sin");
        this->parse_vector_fast(funcAsin, "asin");
        //this->parse_vector_fast(funcTernary, "ternary");
        //this->parse_vector_fast(funcMax, "max");
    }

    // data members
    std::vector<double> x_v;
    std::vector<double> y_v;
    std::vector<double> z_v;
    std::vector<double> result_v;
    std::vector<uint> subsets;
};

#endif /* BPARSER_HANDLER_HH_ */
