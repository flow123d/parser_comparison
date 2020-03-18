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
    }

    virtual ~BaseHandler() {
	    //Profiler::uninitialize();
    }

    virtual double parse_vector_fast(std::string expr) =0;

    // data members
    int nLoops;
    int nBulkSize;

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
};

#endif /* BASE_HANDLER_HH_ */
