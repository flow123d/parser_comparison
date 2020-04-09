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
#include "muParserTest.h"

using namespace std;
using namespace mu;


class MuParserHandler : public BaseHandler {
public:
	MuParserHandler() : BaseHandler() {
	    complexLine = "2*x + y*3 + x*(z-y) + 2*_pi*z";
	    powerLine = "2*x + y^3 + x*(z-y) + 2*_pi*z";
	    funcSin = "sin(_pi*y)";
	}

    void init(int vec_size) override {
        create_data_vectors(vec_size);
    }

    double parse_vector_fast(std::string expr, std::string tag_name) override {
        value_type sum = 0.0;

        mu::Parser  parser;
        parser.DefineVar( _T("x"), &(x_v[0]) );
        parser.DefineVar( _T("y"), &(y_v[0]) );
        parser.DefineVar( _T("z"), &(z_v[0]) );
        parser.SetExpr(expr);

        START_TIMER(nBulkSize, tag_name);
        for (int j=0; j<nLoops; ++j) {
            parser.Eval(&(result_v[j]), nBulkSize);
        }
        END_TIMER(nBulkSize, tag_name);
        for (int i=0; i<nBulkSize; ++i) sum += result_v[i];

        return double(sum);
    }

};

#endif /* MUPARSER_HANDLER_HH_ */
