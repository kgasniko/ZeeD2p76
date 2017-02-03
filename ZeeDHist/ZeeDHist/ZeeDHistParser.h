#ifndef __ZEEDHISTPARSER_H
#define __ZEEDHISTPARSER_H

////////////////////////////////////////////////////////
// Name    : ZeeDHistParser.h
////////////////////////////////////////////////////////

#include "ZeeDHist/parser.h"
#include "ZeeDHist/ZeeDHistArray.h"

using namespace zeed_parser;

const int typeDval = 1;
const int typeHval = 2;

class ZeeDParserNodeResult {
public:
    int type;
    double dVal;
    ZeeDHistArray* hVal;
};

/** Interpret a string containing root file names, histogram
    subdirectories and arithmetic operations
    @author Alexander Glazov
    @date  2008/10/26
 */
class ZeeDHistParser : binarynode {
public:
    ZeeDHistParser( string s);
    virtual ~ZeeDHistParser();
    ZeeDHistArray* eval();
private:
    ZeeDParserNodeResult evalNode();

    /** Read the string, translate to file/histogram subdiretory name. */
    ZeeDHistArray* sValTohVal();

    double operate(double left, double right);
    ZeeDHistArray* operate(double left, ZeeDHistArray* right);
    ZeeDHistArray* operate(ZeeDHistArray* left, ZeeDHistArray* right);
};



#endif
