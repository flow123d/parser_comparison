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
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
            	r_vect[i] = 0.5;
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
        	sum += r_vect[i];
        }

    	return sum;
    }

    double cpp_compute_simple() {
    	double sum = 0.0;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                r_vect[i] = x_v[i] + y_v[i] + z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += r_vect[i];
        }

    	return sum;
    }

    double cpp_compute_complex() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
                r_vect[i] = 2*x_v[i] + y_v[i]*3 + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += r_vect[i];
        }

    	return sum;
    }

    double cpp_compute_complex_power() {
    	double sum = 0.0;
    	double pi = 3.141592653589793238462643;
        START_TIMER("cpp_compute");
        for (int j=0; j<nLoops; ++j) {
            for (int i=0; i<nBulkSize; ++i) {
            	r_vect[i] = 2*x_v[i] + pow( y_v[i], 3.0 ) + x_v[i]*(z_v[i]-y_v[i]) + 2*pi*z_v[i];
            }
        }
        END_TIMER("cpp_compute");
        for (int i=0; i<nBulkSize; ++i) {
            sum += r_vect[i];
        }

    	return sum;
    }

    double parse_vector_fast(std::string expr) override {
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

        START_TIMER("parse_vector_fast");
        for (int j=0; j<nLoops; ++j) {
        	for (int i=0; i<nBulkSize; ++i) {
                x = x_v[i];
                y = y_v[i];
                z = z_v[i];
                result = p.Eval();
                r_vect[i] = (double)result.GetFloat();
            }
        }
        END_TIMER("parse_vector_fast");
        for (int i=0; i<nBulkSize; ++i) sum += r_vect[i];

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

        START_TIMER("ternary_function");
        this->parse_vector_fast(funcTernary);
        END_TIMER("ternary_function");

        START_TIMER("max_function");
        this->parse_vector_fast(funcMax);
        END_TIMER("max_function");
    }

    // data members
    std::vector<double> x_v;
    std::vector<double> y_v;
    std::vector<double> z_v;
    std::vector<double> r_vect;
};

#endif /* MUPARSER_HANDLER_HH_ */
