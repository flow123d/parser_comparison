/*
 * test_parser.hh
 *
 *  Created on: Mar 02, 2020
 *      Author: David Flanderka
 */

#ifndef MUPARSER_HANDLER_HH_
#define MUPARSER_HANDLER_HH_

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
#include "parser/mpParser.h"
#include "parser/mpDefines.h"

using namespace std;
using namespace mup;


class MuParserHandler : public BaseHandler {
public:
	MuParserHandler() : BaseHandler() {
	    funcAsin = "sinh(z)";
	}

    void create_data_vectors(int vec_size) {
        nBulkSize = vec_size;
        nLoops = 5000 * (2048 / vec_size);

        x_v.resize(nBulkSize);
        y_v.resize(nBulkSize);
        z_v.resize(nBulkSize);
        r_vect.resize(nBulkSize);

        for (int i=0; i<nBulkSize; ++i) {
            x_v[i] = i;
            y_v[i] = (double)i/100 + 0.01;
            z_v[i] = (4.95 - (double)(i%10) * 1.1) * 0.2; // values oscillate in <-1; 1>
        }
    }

    double cpp_compute_constant() {
    	double sum = 0.0;
        START_TIMER(nBulkSize, "constant_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
            	r_vect[i] = 0.5;
            }
        }
        END_TIMER(nBulkSize, "constant_cpp");
        for (int i=0; i<nBulkSize; ++i) {
        	sum += r_vect[i];
        }

    	return sum;
    }

    double cpp_compute_simple() {
    	double sum = 0.0;
        START_TIMER(nBulkSize, "simple_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                r_vect[i] = x_v[i] + y_v[i] + z_v[i];
            }
        }
        END_TIMER(nBulkSize, "simple_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += r_vect[i];
        }

    	return sum;
    }

    double cpp_compute_complex() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER(nBulkSize, "complex_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                r_vect[i] = 2*x_v[i] + y_v[i]*3 + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER(nBulkSize, "complex_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += r_vect[i];
        }

    	return sum;
    }

    double cpp_compute_complex_power() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER(nBulkSize, "power_cpp");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
            	r_vect[i] = 2*x_v[i] + pow( y_v[i], 3.0 ) + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER(nBulkSize, "power_cpp");
        for (int i=0; i<nBulkSize; ++i) {
            sum += r_vect[i];
        }

    	return sum;
    }

    double parse_vector_fast(std::string expr, std::string tag_name) override {
        double sum = 0.0;
        ParserX   p;

        Value x(0.0);
        Value y(0.0);
        Value z(0.0);
        Value result(0.0);
        p.DefineVar("x", Variable(&x));
        p.DefineVar("y", Variable(&y));
        p.DefineVar("z", Variable(&z));
        p.SetExpr( _T(expr) );

        START_TIMER(nBulkSize, tag_name);
        for (int j=0; j<nLoops; ++j) {
        	for (int i=0; i<nBulkSize; ++i) {
                x = x_v[i];
                y = y_v[i];
                z = z_v[i];
                result = p.Eval();
                r_vect[i] = (double)result.GetFloat();
            }
        }
        END_TIMER(nBulkSize, tag_name);
        for (int i=0; i<nBulkSize; ++i) sum += r_vect[i];

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
        this->parse_vector_fast(funcAsin, "sinh");
        this->parse_vector_fast(funcTernary, "ternary");
        this->parse_vector_fast(funcMax, "max");
    }

    // data members
    std::vector<double> x_v;
    std::vector<double> y_v;
    std::vector<double> z_v;
    std::vector<double> r_vect;
};

#endif /* MUPARSER_HANDLER_HH_ */
