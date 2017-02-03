
#include "ZeeDHist/parser.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

using std::cout;
using std::endl;
using std::runtime_error;

using namespace zeed_parser;

int isSeparator(char c)
{
    for (int i = 0; i < operationsCount; i++) {
        if ( c == operations[i] ) {
            return 1;
        }
    }

    if ((c == '(') || (c == ')')) {
        return 1;
    } else {
        return 0;
    }
}


binarynode::binarynode( string sin, int preproc)
{

    //    cout << "Got string "<< sin << endl;

    string s = preproc ?  preprocess(sin) : sin ;

    leftnode   = NULL;
    rightnode  = NULL;
    svalue     = "";
    type       = -1;  // not defined yet

    //int iChar = 0;       // navigation along the string

    // check if we start with (
    if (s[0] == '(') {
        // Find closing ")"

        int OpenCount  = 1;
        unsigned int i = 1;

        for ( ; i < s.length(); i++ ) {
            if (s[i] == '(') {
                OpenCount++;
            }

            if (s[i] == ')') {
                OpenCount--;
            }

            if (OpenCount == 0) {
                break;
            }
        }

        if (OpenCount == 0 ) {
            if ( i == s.length() - 1 ) {
                // cout << " ) at the end of line "<<endl;
                s = s.substr(1, i - 1);
            }
        } else {
            string err = "unbalanced parentheses in string ";
            err += s;
            throw runtime_error(err);
            // Error .. unbalanced parentheses.
        }
    }


    // Now search for operation following operations order, from low to high
    for (int iOrd = 1; iOrd <= highestOrder; iOrd++) {
        for ( int iOp = 0; iOp < operationsCount; iOp++ ) {

            //      cout << "bla" << operations[iOp] <<endl;
            if (operationsOrder[iOp] > iOrd) {
                break;
            }

            // Search for the operation:
            int location = 0;
            int OpenCount = 0;

            for (unsigned int i = 0; i < s.length(); i++) {
                //  cout << i << " " << s[i] << operations[iOp] << endl;
                // Check for opening parenthesis
                if (s[i] == '(') {
                    OpenCount++;
                } else if (s[i] == ')') {
                    OpenCount--;
                } else if ( (s[i] == operations[iOp]) && (OpenCount == 0)) {
                    location = i;
                    break;
                }
            }

            //      s.find(operation[iOp]);
            if (location > 0) {
                type = 1;
                // cout << " got operation" <<  operations[iOp] <<endl;
                operation = operations[iOp];
                //  Got operation with current low order
                leftnode  = new binarynode(s.substr(0, location), 0);
                rightnode = new binarynode(s.substr(location + 1, s.length()), 0);
                return;
            }
        }
    }

    // No operations, must be a dvalue or svalue
    if (isdigit(s[0])) {
        type = typeDValue;
        svalue = s;
        // cout << "got dval " << svalue <<endl;
    } else {
        type = typeSValue;
        svalue = s;
        // cout << "got sval " << svalue <<endl;
    }
}

binarynode::~binarynode()
{
    delete leftnode;
    delete rightnode;
}

void binarynode::print()
{

    if (type == typeOperation) {
        cout << "Operation:" << operation << endl;

        if (leftnode  != NULL) {
            cout << "Left: ";
            leftnode->print();
        }

        if (rightnode != NULL) {
            cout << "Right: ";
            rightnode->print();
        }
    } else if (type == typeDValue ) {
        cout << "DValue:" << svalue << endl;
    } else if (type == typeSValue ) {
        cout << "SValue:" << svalue << endl;
    } else {
        cout << "Invalid type=" << type << endl;
    }


}

string binarynode::eval()
{
    if (type == typeOperation ) {
        string left  = leftnode->eval();
        string right = rightnode->eval();

        // cout << "eval left,right:" << left <<","<<right<<endl;

        if ( (leftnode->getType() == typeDValue) && (rightnode->getType() == typeDValue)) {

            double  dleft  = strtod(left.c_str(), NULL);
            double  dright = strtod(right.c_str(), NULL);

            //      std::istringstream i1(left);
            //double dleft;
            //i1 >> dleft;
            //std::istringstream i2(left);
            //double dright;
            //i2 >> dright;
            double result = 0;

            if ( operation == '+') {
                result = dleft + dright;
            }

            if ( operation == '-') {
                result = dleft - dright;
            }

            if ( operation == '*') {
                result = dleft * dright;
            }

            if ( operation == '%') {
                result = dleft / dright;
            }

            if ( operation == '#') {
                result = dleft / dright;
            }


            std::ostringstream o;
            o << result;
            return o.str();
        } else {
            string out("(");
            out += left;
            out += operation;
            out += right;
            out += ")";
            return out;
        }
    } else if ( (type == typeSValue) || (type == typeDValue )) {
        return svalue;
    } else {
        return "Invalid type";
    }

}

int binarynode::getType()
{
    if (type != typeOperation ) {
        return type;
    } else {
        int left  = leftnode  -> getType();
        int right = rightnode -> getType();

        if ( (left == typeDValue) && (right == typeDValue)) {
            return typeDValue;
        } else {
            return typeSValue;
        }
    }
}

string binarynode::preprocess( string s)
{
    // Get rid of white spaces.
    // Special treatment for //fileName/( ) syntaxis.

    string nowhite = "";

    // first white spaces
    for (unsigned int i = 0; i < s.length(); i++) {
        if (s[i] != ' ') {
            nowhite += s[i];
        }
    }

    //     cout << "S, No white=" << s<<","<<nowhite <<endl;

    // Now search for //XXX/( ) patterns
    string expandFile = "";
    unsigned int iChar = 0;

    while ( iChar < nowhite.length() ) {
        if ( nowhite.substr(iChar, 2) == "//" ) {
            // locate closing /
            unsigned int i = iChar + 2;

            for (; i < nowhite.length(); ++i ) {
                if ( nowhite[i] == '/' ) {
                    break;
                }
            }

            if ( i < nowhite.length() ) {
                // Ok, found the closing '/'
                string fileName = nowhite.substr(iChar + 2, i - iChar - 2);
                // cout << "File name=" << fileName << endl;

                // Check if what follows is (

                i++;

                if (i == nowhite.length()) {
                    string err = "nothing follows file name ";
                    err += fileName;
                    throw runtime_error(err);
                }

                if (nowhite[i] == '(' ) {
                    // locate the closing )
                    int OpenCount = 0;
                    unsigned int j = i;

                    for ( ; j < nowhite.length(); ++j ) {
                        if (nowhite[j] == '(') {
                            ++OpenCount;
                        }

                        if (nowhite[j] == ')') {
                            --OpenCount;
                        }

                        if (OpenCount == 0) {
                            break;
                        }
                    }

                    if ( OpenCount == 0 ) {
                        // Ok
                        expandFile += expandFileName(fileName, nowhite.substr(i + 1, j - i - 1));
                        iChar = j + 1;

                    } else {
                        // Error .. unbalanced parentheses.
                        string err = "unbalanced parentheses in string ";
                        err += nowhite;
                        throw runtime_error(err);
                    }
                } else {
                    // Continue as usual
                    expandFile += nowhite[iChar] ;
                    ++iChar;
                }
            } else {
                string err = "un-closed file name for string ";
                err += nowhite;
                throw runtime_error(err);
            }
        } else {
            expandFile += nowhite[iChar] ;
            ++iChar;
        }
    }

    return expandFile;
}

string binarynode::expandFileName(string FileName, string s)
{

    // cout << " expandFileName: input: " << FileName << ","<<s<<endl;

    string out = "";
    unsigned int iChar = 0;
    // svalues: 1) start with non-digit  2) limitted by +-%*()

    while ( iChar < s.length() ) {
        if ( isSeparator( s[iChar] ) ) {
            out += s[iChar];
            iChar++;
        }

        // Start of S or D value. Find the end
        unsigned int i = iChar;

        for (; i < s.length(); ++i ) {
            if ( isSeparator( s[i] ) ) {
                break;
            }
        }

        string dsValue = s.substr(iChar, i - iChar);
        iChar = i;

        if (dsValue != "") {
            //            cout << "dsValue= "<< dsValue <<endl;
            if (isdigit(dsValue[0])) {
                // DValue, no modification
                out += dsValue;
            } else {
                // SValue, append file
                out += "//";
                out += FileName;
                out += '/';
                out += dsValue;
            }
        }

    }

    return out;
}

//

