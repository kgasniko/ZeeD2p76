#include "ZeeDExternal/ZeeDPDF.h"
#include "LHAPDF/LHAPDF.h"
#include <vector>

namespace ZeeDPDF {

bool fromPDFInited = false;
bool toPDFInited = false;
const int fromPDFNum = 1;
const int toPDFNum = 2;

bool initialized = false;

void init() {
    if (!initialized) {
        LHAPDF::initLHAPDF();
	LHAPDF::setVerbosity(LHAPDF::DEFAULT);
	LHAPDF::setPDFPath(".");
	initialized = true;
    }
}

void setFromPDFSet (const std::string& name, int subset){
    init();
    LHAPDF::initPDFSet(fromPDFNum, name, subset);
    fromPDFInited = true;
}

void setToPDFSet   (const std::string& name, int subset){
    init();
    LHAPDF::initPDFSet(toPDFNum,name, subset);
    toPDFInited = true;
}

bool isFromInited() {
    return fromPDFInited;
}

bool isToInited() {
    return toPDFInited;
}

double getCorrectionFactor(ZeeDPDF::Flavour fl, double x, double Q) {
    if (!fromPDFInited || !toPDFInited) return 0.0;
    double from = LHAPDF::xfx(fromPDFNum,x,Q,fl);
    double to   = LHAPDF::xfx(  toPDFNum,x,Q,fl);
    
    return to/from;
}

}
