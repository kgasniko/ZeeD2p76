#include "ZeeDHist/ZeeDHistParser.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <TROOT.h>

using std::cout;
using std::endl;
using std::runtime_error;


ZeeDHistParser::ZeeDHistParser(string s)
    : binarynode(s)
{}

ZeeDHistParser::~ZeeDHistParser()
{}

ZeeDParserNodeResult ZeeDHistParser::evalNode()
{
    ZeeDParserNodeResult result = {0, 0, NULL};


    if (type == typeOperation ) {
        ZeeDParserNodeResult leftRes  = static_cast<ZeeDHistParser*>(leftnode)->evalNode();
        ZeeDParserNodeResult rightRes = static_cast<ZeeDHistParser*>(rightnode)->evalNode();

        // Cases
        if ( (leftRes.type == typeDval) && (rightRes.type == typeDval) ) {
            result.type = typeDval;
            result.dVal = operate(leftRes.dVal, rightRes.dVal);
        } else if ( (leftRes.type == typeHval) && (rightRes.type == typeHval) ) {
            result.type = typeHval;
            result.hVal = operate(leftRes.hVal, rightRes.hVal);
        } else if ( (leftRes.type == typeDval) && (rightRes.type == typeHval) ) {
            result.type = typeHval;
            result.hVal = operate(leftRes.dVal, rightRes.hVal);
        } else if ( (leftRes.type == typeHval) && (rightRes.type == typeDval) ) {
            result.type = typeHval;
            result.hVal = operate(rightRes.dVal, leftRes.hVal);
        }

        // Cleanup
        if (leftRes.hVal  != NULL) {
            delete leftRes.hVal;
        }

        if (rightRes.hVal != NULL) {
            delete rightRes.hVal;
        }
    }

    else if (type == typeDValue ) {
        result.type = typeDval;
        result.dVal = strtod(svalue.c_str(), NULL);
    }

    else if (type == typeSValue ) {
        result.type = typeHval;
        result.hVal = sValTohVal();
    }

    else {
    }


    return result;

}

ZeeDHistArray* ZeeDHistParser::sValTohVal()
{

    unsigned int iChar = 0;
    // normally we start with file name
    //    cout << "sValTOhVal: " << svalue <<endl;


    if (svalue.substr(iChar, 2) == "//") {
        unsigned int i = iChar + 2;

        for ( ; i < svalue.length(); i++ ) {
            if (svalue[i] == '/') {
                break;
            }
        }

        string fileName = svalue.substr(iChar + 2, i - iChar - 2);
        TFile* RootFile = new TFile(fileName.c_str());

        if (RootFile == NULL ) {
            cout << " File not found " << fileName << endl;
            throw runtime_error("ZeeDHistParser: wrong file name");
        }

        // now get directory name:
        string dirName = svalue.substr(i + 1, svalue.length());

        //        cout << "file=" << fileName <<" dir=" << dirName<<endl;


        ZeeDHistArray* Result = new ZeeDHistArray(RootFile, dirName);

        if (Result == NULL) {
            cout << " Wrong directory name" << dirName << endl;
            throw runtime_error("ZeeDHistParser: wrong directory name");
        }

        delete RootFile;
        return Result;
    } else {
        cout << " Wrong svalue " << svalue << endl;
        throw runtime_error("ZeeDHistParser: unexpected variable");
    }


}

double  ZeeDHistParser::operate(double left, double right)
{
    double result = 0;

    if ( operation == '+') {
        result = left + right;
    } else if ( operation == '-') {
        result = left - right;
    } else if ( operation == '*') {
        result = left * right;
    } else if ( operation == '%') {
        result = left / right;
    } else if ( operation == '#') {
        result = left / right;
    } else {
        cout << "Wrong operation " << operation << endl;
    }

    return result;

}

ZeeDHistArray* ZeeDHistParser::operate(double left, ZeeDHistArray* right)
{
    ZeeDHistArray* result = new ZeeDHistArray(*right);

    if ( operation == '+') {
        *result +=  left;
    } else if ( operation == '-') {
        *result -=  left;
    } else if ( operation == '*') {
        *result *=  left;
    } else if ( operation == '%') {
        *result /=  left;
    } else if ( operation == '#') {
        *result /=  left;
    } else {
        cout << "Wrong operation " << operation << endl;
    }

    return result;
}

ZeeDHistArray* ZeeDHistParser::operate(ZeeDHistArray* left, ZeeDHistArray* right)
{
    ZeeDHistArray* result = new ZeeDHistArray(*left);

    if ( operation == '+') {
        *result +=  *right;
    } else if ( operation == '-') {
        *result -=  *right;
    } else if ( operation == '*') {
        *result *=  *right;
    } else if ( operation == '%') {
        *result /=  *right;
    }

    else if ( operation == '#') {
        *result = *result / *right;
    } else {
        cout << "Wrong operation " << operation << endl;
    }

    return result;
}

ZeeDHistArray* ZeeDHistParser::eval()
{
    //    this->print();

    ZeeDParserNodeResult tmp = this->evalNode();

    if (tmp.type == typeDval) {
        return NULL;
    } else {
        return tmp.hVal;
    }

}



