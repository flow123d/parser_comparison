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
        complexLine = "2 * x + y * 3 + 2 * pi * z";
        powerLine = "";

        funcPlus = "0.5*y";
	}

    void create_data_vectors(int vec_size) {
        nBulkSize = vec_size;
        nLoops = 5000 * (2048 / vec_size);

        x_v = Value(nBulkSize, 0);
        y_v = Value(nBulkSize, 0);
        z_v = Value(nBulkSize, 0);
        r_vect.resize(nBulkSize);

        for (int i=0; i<nBulkSize; ++i) {
            x_v.At(i) = (float_type)i;
            y_v.At(i) = (float_type)(double)i/100 + 0.01;
            z_v.At(i) = (float_type)(4.95 - (double)(i%10) * 1.1) * 0.2; // values oscillate in <-1; 1>
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
                r_vect[i] = (double)x_v.At(i).GetFloat() + (double)y_v.At(i).GetFloat() + (double)z_v.At(i).GetFloat();
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
                r_vect[i] = 2*(double)x_v.At(i).GetFloat() + (double)y_v.At(i).GetFloat()*3 + + 2*pi*(double)z_v.At(i).GetFloat();
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

        p.DefineVar("x", Variable(&x_v));
        p.DefineVar("y", Variable(&y_v));
        p.DefineVar("z", Variable(&z_v));
        p.SetExpr( _T(expr) );

        START_TIMER("parse_vector_fast");
        for (int j=0; j<nLoops; ++j) {
            Value result = p.Eval();

            sum += (double)result.At(0).GetFloat();
        }
        END_TIMER("parse_vector_fast");

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
    }

    void run_function_tests(int vec_size) {
        create_data_vectors(vec_size);

        START_TIMER("multiplication_function");
        this->parse_vector_fast(funcPlus);
        END_TIMER("multiplication_function");

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
    Value x_v;
    Value y_v;
    Value z_v;
    std::vector<double> r_vect;
};

#endif /* MUPARSER_HANDLER_HH_ */
