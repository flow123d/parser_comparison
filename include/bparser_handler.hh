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
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 0.5;
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
        	sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_simple() {
    	double sum = 0.0;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = x_v[i] + y_v[i] + z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                result_v[i] = 2*x_v[i] + y_v[i]*3 + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double cpp_compute_complex_power() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
            	result_v[i] = 2*x_v[i] + pow( y_v[i], 3.0 ) + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += result_v[i];
        }

    	return sum;
    }

    double parse_vector_fast(std::string expr) override {
        double sum = 0.0;

        Parser p(nBulkSize);
        p.parse(expr);
        p.set_variable("x", {}, &x_v[0]);
        p.set_variable("y", {}, &y_v[0]);
        p.set_variable("z", {}, &z_v[0]);
        p.set_variable("_result_", {}, &result_v[0]);
        p.compile();
        p.set_subset(subsets);

        START_TIMER("parse_vector_fast");
        for (int j=0; j<nLoops; ++j) {
            p.run();
        }
        END_TIMER("parse_vector_fast");
        for (int i=0; i<nBulkSize; ++i) sum += result_v[i];

        return sum;
    }

    void run_expression_tests(int vec_size) {
        create_data_vectors(vec_size);

        START_TIMER("constant_expresions");
        this->cpp_compute_constant();
        this->parse_vector_fast(constantLine);
        END_TIMER("constant_expresions");

        START_TIMER("simple_expresions");
        this->cpp_compute_simple();
        this->parse_vector_fast(simpleLine);
        END_TIMER("simple_expresions");

        START_TIMER("complex_expresions");
        this->cpp_compute_complex();
        this->parse_vector_fast(complexLine);
        END_TIMER("complex_expresions");

        START_TIMER("power_expresions");
        this->cpp_compute_complex_power();
        this->parse_vector_fast(powerLine);
        END_TIMER("power_expresions");
    }

    void run_function_tests(int vec_size) {
        create_data_vectors(vec_size);

        START_TIMER("plus_function");
        this->parse_vector_fast(funcPlus);
        END_TIMER("plus_function");

        START_TIMER("power_function");
        this->parse_vector_fast(funcPower);
        END_TIMER("power_function");

        START_TIMER("abs_function");
        this->parse_vector_fast(funcAbs);
        END_TIMER("abs_function");

        START_TIMER("exp_function");
        this->parse_vector_fast(funcExp);
        END_TIMER("exp_function");

        START_TIMER("log_function");
        this->parse_vector_fast(funcLog);
        END_TIMER("log_function");

        START_TIMER("sin_function");
        this->parse_vector_fast(funcSin);
        END_TIMER("sin_function");

        START_TIMER("asin_function");
        this->parse_vector_fast(funcAsin);
        END_TIMER("asin_function");

        /*START_TIMER("ternary_function");
        this->parse_vector_fast(funcTernary);
        END_TIMER("ternary_function");*/

        /*START_TIMER("max_function");
        this->parse_vector_fast(funcMax);
        END_TIMER("max_function");*/
    }

    // data members
    std::vector<double> x_v;
    std::vector<double> y_v;
    std::vector<double> z_v;
    std::vector<double> result_v;
    std::vector<uint> subsets;
};

#endif /* BPARSER_HANDLER_HH_ */
