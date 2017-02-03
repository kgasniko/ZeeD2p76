#ifndef ZEED_PDF_H
#define ZEED_PDF_H

#include <string>

namespace ZeeDPDF {

    enum Flavour {
      TBAR = -6, BBAR = -5, CBAR = -4, SBAR = -3,
      UBAR = -2, DBAR = -1, GLUON = 0, DOWN = 1,
      UP = 2, STRANGE = 3, CHARM = 4, BOTTOM = 5,
      TOP = 6, PHOTON = 7
    };

    void setFromPDFSet (const std::string& name, int subset = 0);
    void setToPDFSet   (const std::string& name, int subset = 0);
    
    bool isFromInited();
    bool isToInited();
    
    double getCorrectionFactor(Flavour partcode, double x, double Q);

}

#endif
