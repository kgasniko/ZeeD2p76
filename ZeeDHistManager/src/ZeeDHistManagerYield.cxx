//#define USE_HUGE_2D_HIST 

#include "ZeeDHistManager/ZeeDHistManagerYield.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include <iomanip>

using std::cout;
using std::endl;

//-----------------------------------------------------
void ZeeDHistManagerYield::BookHistos()
{
  // Books histograms

  // get analysis options: list of all possible runs
  ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();
  double FirstRun = (*ZeeDAnaOptions)->FirstRunHistogram();
  double LastRun  = (*ZeeDAnaOptions)->LastRunHistogram();

  // read the valid run numbers and corresponding lumies from the text file
  // FIXME: this run list should be taken directly from the actual GRL file
  FILE* grl = fopen(ZeeDMisc::FindFile("RunLumi/RunLumi_v61-pro14.txt").Data(),"r");
  if (!grl) {
    cout << "YieldHistManager:\tCan't find list of all possible runs (RunLumi.txt)" << endl;
    exit(EXIT_FAILURE);
  } else {
    while (!feof(grl)) {
      int run;
      float lumi;
      fscanf(grl, "%d", &run);
      fscanf(grl, "%f", &lumi);
      if (run >= FirstRun && run <= LastRun) lumies[run] = lumi;
    }
  }
  fclose(grl);

  // create the histrogram
  std::set<double> bin_bounds_set;
  for (std::map<int, float>::iterator it = lumies.begin(); it != lumies.end(); ++it) {
    bin_bounds_set.insert(it->first - 0.5);
    bin_bounds_set.insert(it->first + 0.5);
  }
  std::vector<double> bin_bounds(bin_bounds_set.begin(), bin_bounds_set.end()); 


  int Nbins = bin_bounds.size() - 1;
  if (Nbins > 0) {
    AddTH1D("Nevents",	Nbins, &bin_bounds[0], "run", "");
    AddTH1D("Yield",	Nbins, &bin_bounds[0], "run", "");
    AddTH1D("NeventsW",	Nbins, &bin_bounds[0], "run", "");
    AddTH1D("YieldW",	Nbins, &bin_bounds[0], "run", "");

  } else {
    cout << "YieldHistManager: all the runs are out of bounds, do a uniform binning" << endl;
    int Nruns = LastRun - FirstRun + 1;
    AddTH1D("Nevents",  Nruns, FirstRun - 0.5, LastRun + 0.5, "run", "");
    AddTH1D("Yield",    Nruns, FirstRun - 0.5, LastRun + 0.5, "run", "");
    AddTH1D("NeventsW", Nruns, FirstRun - 0.5, LastRun + 0.5, "run", "");
    AddTH1D("YieldW",   Nruns, FirstRun - 0.5, LastRun + 0.5, "run", "");

  }
#ifdef USE_HUGE_2D_HIST 
  // FIXME: add sparse hists (THnSparse class) and do it properly
  AddTH2D("NeventsDetailed",  LastRun - FirstRun + 1, FirstRun - 0.5, LastRun + 0.5, 2000, -0.5, 1999.5, "run", "LB");
  AddTH2D("NeventsDetailedW", LastRun - FirstRun + 1, FirstRun - 0.5, LastRun + 0.5, 2000, -0.5, 1999.5, "run", "LB");
#endif

}

//-----------------------------------------------------
void ZeeDHistManagerYield::Fill()
{
  DEBUG_MES_START;
  DEBUG_MES(this->GetHMName());

  // Get event
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  // Event run, lb, weight
  Int_t Run = event->RunNumber().Get();
  Int_t LB = event->LB().Get();
  Double_t Weight = event->GetWeight();

  // Fill the histograms
  FillYieldHists(Run, LB, Weight);

}

//-----------------------------------------------------
void ZeeDHistManagerYield::FillYieldHists(Int_t Run,
#ifndef USE_HUGE_2D_HIST
   __attribute__((unused))
#endif
   Int_t LB,
   Double_t Weight)
{

  // ------------------======= Fill histograms =========------------------------
  // and get lumi for the given run
  float lumi = -1.;
  std::map<int, float>::iterator it = lumies.find(Run);
  if (it != lumies.end()) lumi = it->second;

  FillTH1(Run, 1.,		"Nevents");
  FillTH1(Run, Weight,		"NeventsW");
  FillTH1(Run, 1./lumi,		"Yield");
  FillTH1(Run, Weight/lumi,	"YieldW");

#ifdef USE_HUGE_2D_HIST 
  FillTH2(Run, LB, 1,		"NeventsDet");
  FillTH2(Run, LB, Weight,	"NeventsDetW");
#endif
}

