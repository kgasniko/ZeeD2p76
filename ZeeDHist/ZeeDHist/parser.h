#ifndef PARSER_H
#define PARSER_H

#include <string>


using std::string;

namespace zeed_parser {

const int  operationsCount = 5;
const int  highestOrder    = 2;
// % -- standard division,  # -- binary division
const char operations[]  = "+-*%#";
const int  operationsOrder[operationsCount] = {1, 1, 2, 2, 2};

const int typeOperation = 1;
const int typeDValue    = 2;
const int typeSValue    = 3;

class binarynode {
public:

    /** Main parsing code. */
    binarynode ( string s, int preproc = 1 );
    virtual ~binarynode();
    virtual void print();
    string eval();
    int getType();
protected:
    int type;
    binarynode* leftnode;
    binarynode* rightnode;
    char operation;
    string svalue;
private:
    string preprocess ( string s);

    /** Add FileName to all sValues. */
    string expandFileName( string FileName, string s);
};

// Functions
int IsSeparator(char c);

}
#endif

