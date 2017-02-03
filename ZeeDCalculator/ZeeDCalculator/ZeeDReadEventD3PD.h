#ifndef ZEED_CALC_EVENT_D3PD_H
#define ZEED_CALC_EVENT_D3PD_H

#include <TChain.h>
#include <vector>
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDCalculator/IZeeDReadEvent.h"
#include <TObject.h>

class ZeeDEvent;


/**
   @todo do we need calibrator and systematics for D3PD?
 */
class ZeeDReadEventD3PD : public TObject, public IZeeDReadEvent {

public:

    ZeeDReadEventD3PD();
    virtual ~ZeeDReadEventD3PD() {
    }

    virtual void ReadInfo(RawEvent* event);

private:

/*    void ReadElecInfo(RawEvent* event);
    void ReadEventInfo(RawEvent* event);

    TChain* fChain;
    int fEventCounter;
*/
    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

  /*  //Event information
    int EventNumber;
    int RunNumber;
    Int_t bcid;
    Int_t lbn;
    Int_t larError;
    Int_t el_n;
    Int_t timestamp;
    Int_t vxp_n;
    Double_t MET;
    Double_t MET_phi;
    
    //Electron information
    std::vector<double> *el_truth_pt;
    std::vector<double> el_truth_phi;
    std::vector<double> el_truth_eta;
    
    std::vector<int> el_originbkg;
    std::vector<int> el_origin;
    std::vector<int> el_truth_status; //MC only

    std::vector<unsigned int> el_index;
    std::vector<unsigned int> el_author;
    std::vector<unsigned int> el_OQ;
    std::vector<unsigned int> el_isEM;

    std::vector<double> *el_charge;
    std::vector<double> el_etas2;
    std::vector<double> el_trackpt;
    std::vector<double> el_tracketa;
    std::vector<double> el_trackphi;
    std::vector<double> el_trackz0pv;
    std::vector<double> el_trackd0pv;
    
    std::vector<double> Mee;
    std::vector<double> mT;

    std::vector<double> el_cl_eta;
    std::vector<double> el_cl_phi;

    std::vector<double> el_et;
    std::vector<double> el_deltaeta1;
    std::vector<double> el_deltaeta2;
    std::vector<double> el_deltaphi2;
    
    std::vector<double> SF_reco;
    std::vector<double> SF_trigger;
    std::vector<double> SF_tight;
    std::vector<double> SF_cMedfwdLoose;

    std::vector<double> el_Ecorrection;
    std::vector<double> el_Es0;
    std::vector<double> el_Es1;
    std::vector<double> el_Es2;
    std::vector<double> el_Es3;

    std::vector<double> el_Etcone30_pt_corrected;
    std::vector<double> el_ptcone30;
    std::vector<double> el_nucone30;
*/
};

#endif
