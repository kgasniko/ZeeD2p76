#ifndef __ZeeDHistManagerControlCF_H
#define __ZeeDHistManagerControlCF_H

///////////////////////////////////////////////
// Name ZeeDHistManagerControlCF
// Basic histograms for Boson candidates
// Created by SG, 13 Feb 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

class ZeeDBosonZ;
class ZeeDSystematics;

/** ZeeDHistManagerControlCF */
class ZeeDHistManagerControlCF : public ZeeDHistManager {
public:

ZeeDHistManagerControlCF(TString name)
: ZeeDHistManager(name),
fSvcHelper("ZeeDAnalysisCutHistManagerZ"),
fAnaOptions(fSvcHelper.GetAnaSvc())
{}

// Book all histograms:
virtual void BookHistos();

// Arrange loop over Z,W bosons:
using ZeeDHistManager::Fill;
virtual void Fill();

private:
ZeeDSystematics* fSys;

ZeeDSvcHelper fSvcHelper;
ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions; //!

};

#endif //ZeeDHistManagerControlCF
