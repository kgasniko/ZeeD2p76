#ifndef __ZEEDCALCW_H
#define __ZEEDCALCW_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcW.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, George Sedov
//

// Root libraries
#include <TObject.h>
#include <TObjArray.h>
#include <TH2D.h>
#include "TFile.h"

#include "GaudiKernel/ToolHandle.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDEvent/ZeeDLepton.h"

class ZeeDSystematics;
class MissingET;
class ZeeDEvent;
class ZeeDBosonW;
class ZeeDElectron;
class ZeeDEvent;
class ZeeDEtMiss;

/** Class for calculation of W boson quantities */
class ZeeDCalcW : public TObject {

    public:

        ZeeDCalcW();
        ~ZeeDCalcW();

        /** Calculate W boson candidates for the given event */
        void Calculate(ZeeDEvent* event);

    private:

        /** Reads array of electrons from event */
        Bool_t ReadElectrons();

        /** Reads et miss object from event */
        void ReadEtMiss();
        std::vector<TObject*> pCor, pCorElec, pCorMuon;
        double sfBeg, sfEnd, size;
        ZeeDBosonW* CreateW (const ZeeDLepton* elec);

        /** Missing Et object */
        const MissingET* fMissingEtObj;

        /**  Array of electrons */
        TObjArray* fElectrons;

        /** Pointer to the current event */
        ZeeDEvent* fEventInProcess;

        /** Pointer to etMiss object */
        ZeeDEtMiss* fEtMiss;
        std::vector<std::vector<TH2D*> > toys, toysElec, toysMuon;
         /** Service helper */
        ZeeDSvcHelper fSvcHelper;
        /** Service handle to job options file */
        ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions;
        ZeeDSystematics* fSys;
};

#endif // ZeeDCalcW
