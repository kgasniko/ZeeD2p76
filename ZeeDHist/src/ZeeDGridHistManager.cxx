#include "ZeeDHist/ZeeDGridHistManager.h"


ZeeDGridHistManager::ZeeDGridHistManager(TString name, TString binGridName,  ZeeDQuantity<ZeeDArrayD> (*cFunction)(const ZeeDEvent*))
    : ZeeDHistManager(name),
      fBaseName(name),
      fHistManagers(NULL),
      fBinGrid(NULL),
      nRegisteredHM(0)
{
    fFunction = cFunction;
// Open bin grid
    fBinGrid = new ZeeDBinGrid(name);
    fBinGrid->ReadFromFile(binGridName);

    const TString format="%.2f";


    int nbins = this->GetNBins();
    int length_nbins = (TString("") += nbins).Length(); // five seconds of black magic
    for (Int_t i=0; i < nbins; i++) {

        TString name = "Bin";

	int length_i = (TString("") += i).Length();
	for (int izero = 0; izero < length_nbins-length_i; ++izero) name += '0';
        name += i;

        name += "|";

        std::vector<std::pair<Double_t, Double_t> > binBoundaries = fBinGrid->GetBinBoundaries(i);

        for( UInt_t n=0; n != binBoundaries.size(); ++n) {

            const Double_t low_edge = binBoundaries[n].first;
            const Double_t high_edge = binBoundaries[n].second;

            name += TString::Format(format, low_edge);
            name += "_";
            name += TString::Format(format, high_edge);

            name += "|";

        }

        fBinLabels.push_back(name);

    }

}

Int_t ZeeDGridHistManager::GetNBins()
{
    return fBinGrid->GetNumOfCells();
}

void ZeeDGridHistManager::AttachHM(ZeeDHistManager* histManager)
{
    fHistManagers.push_back(histManager);
}


void ZeeDGridHistManager::Initialize()
{
    for (UInt_t i=0; i<fHistManagers.size(); i++) {
        fHistManagers.at(i)->Initialize();
    }

    for (UInt_t i=0; i<fGridifiedHistManagers.size(); i++) {
        fGridifiedHistManagers.at(i)->Initialize();
    }

}

void ZeeDGridHistManager::UseEvent(const ZeeDEvent* event)
{
    // Distribute the event to all HM:
    for ( UInt_t i=0; i<fHistManagers.size(); i++) {
        fHistManagers.at(i)->UseEvent(event);
    }

    for ( UInt_t i=0; i<fGridifiedHistManagers.size(); i++) {
        fGridifiedHistManagers.at(i)->UseEvent(event);
    }

    // Evalueate the value:
    fValue = (*fFunction)(event);

}

void ZeeDGridHistManager::Fill()
{
    if( fValue.IsSet() == kFALSE ) return;

    ZeeDArrayD value = fValue.Get();

    Int_t idx = fBinGrid->GetIndex(&value);
    if ( (idx >=0) && (idx < this->GetNBins())) {

            for(UInt_t nHM = 0; nHM != nRegisteredHM; ++nHM) {

                fHistManagers.at(idx + nHM*this->GetNBins())->Fill();

            }

            for(UInt_t nHM = 0; nHM != fGridifiedHistManagers.size(); ++nHM) {

                fGridifiedHistManagers[nHM]->SetGridBin(idx);
                fGridifiedHistManagers[nHM]->Fill();

            }

    }
    else {
        //Error("ZeeDGridHistManager::Fill","Index outside array dimension");
    }


}

void ZeeDGridHistManager::Finalize()
{
    for (UInt_t i=0; i<fHistManagers.size(); i++) {
        fHistManagers.at(i)->Finalize();
    }
}
