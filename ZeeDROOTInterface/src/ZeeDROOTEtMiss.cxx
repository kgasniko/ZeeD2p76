#include "ZeeDROOTInterface/ZeeDROOTEtMiss.h"
#include "ZeeDEvent/ZeeDEtMiss.h"

ZeeDROOTEtMiss::ZeeDROOTEtMiss() : 
  fEt(0),
  fEtX(0),
  fEtY(0),
  fPhi(0),
  fMET_SoftTermETX(0),
  fMET_SoftTermETY(0),
  fMET_SoftTermETSUM(0)
{}

ZeeDROOTEtMiss::~ZeeDROOTEtMiss()
{}

Int_t ZeeDROOTEtMiss::fillOut (ZeeDEtMiss* const etMiss) const
{
    // Fills ZeeDEtMiss with the information from ZeeDROOTEtMiss
    etMiss->fEt  = fEt;
    etMiss->fEtX = fEtX;
    etMiss->fEtY = fEtY;
    etMiss->fPhi = fPhi;

	etMiss->fCorRecoilEt  = fCorRecoilEt;
    etMiss->fCorRecoilEtX = fCorRecoilEtX;
    etMiss->fCorRecoilEtY = fCorRecoilEtY;
    etMiss->fCorRecoilPhi = fCorRecoilPhi;
	etMiss->fCorRecoilSumet = fCorRecoilSumet;

    etMiss-> fMET_SoftTermETX   =  fMET_SoftTermETX;
    etMiss-> fMET_SoftTermETY   =  fMET_SoftTermETY;
    etMiss-> fMET_SoftTermETSUM =  fMET_SoftTermETSUM;

    UInt_t wSize(0);

    // Jet
    wSize = fMETCompositionJet_wet.size();
    etMiss->GetMETCompositionJet_wet().reserve( wSize );
    etMiss->GetMETCompositionJet_wpx().reserve( wSize );
    etMiss->GetMETCompositionJet_wpy().reserve( wSize );
    etMiss->GetMETCompositionJet_statusWord().reserve( wSize );
    etMiss->GetMETCompositionJet_index().reserve( wSize );

    for(UInt_t i = 0; i < wSize; i++)
      {
	etMiss->GetMETCompositionJet_wet().push_back( fMETCompositionJet_wet.at(i));
	etMiss->GetMETCompositionJet_wpx().push_back( fMETCompositionJet_wpx.at(i));
	etMiss->GetMETCompositionJet_wpy().push_back( fMETCompositionJet_wpy.at(i));
	etMiss->GetMETCompositionJet_statusWord().push_back(fMETCompositionJet_statusWord.at(i) );
	etMiss->GetMETCompositionJet_index().push_back(fMETCompositionJet_index.at(i) );
      }

    // Electron
    wSize = fMETCompositionElectron_wet.size();
    etMiss->GetMETCompositionElectron_wet().reserve( wSize );
    etMiss->GetMETCompositionElectron_wpx().reserve( wSize );
    etMiss->GetMETCompositionElectron_wpy().reserve( wSize );
    etMiss->GetMETCompositionElectron_statusWord().reserve( wSize );
    etMiss->GetMETCompositionElectron_index().reserve( wSize );

    for(UInt_t i = 0; i < wSize; i++)
      {
	etMiss->GetMETCompositionElectron_wet().push_back( fMETCompositionElectron_wet.at(i) );
	etMiss->GetMETCompositionElectron_wpx().push_back( fMETCompositionElectron_wpx.at(i) );
	etMiss->GetMETCompositionElectron_wpy().push_back( fMETCompositionElectron_wpy.at(i) );
	etMiss->GetMETCompositionElectron_statusWord().push_back( fMETCompositionElectron_statusWord.at(i) );
	etMiss->GetMETCompositionElectron_index().push_back( fMETCompositionElectron_index.at(i) );
      }

    // Photon
    wSize = fMETCompositionPhoton_wet.size();
    etMiss->GetMETCompositionPhoton_wet().reserve( wSize );
    etMiss->GetMETCompositionPhoton_wpx().reserve( wSize );
    etMiss->GetMETCompositionPhoton_wpy().reserve( wSize );
    etMiss->GetMETCompositionPhoton_statusWord().reserve( wSize );
    etMiss->GetMETCompositionPhoton_index().reserve( wSize );

    for(UInt_t i = 0; i < wSize; i++)
      {
	etMiss->GetMETCompositionPhoton_wet().push_back( fMETCompositionPhoton_wet.at(i) );
	etMiss->GetMETCompositionPhoton_wpx().push_back( fMETCompositionPhoton_wpx.at(i) );
	etMiss->GetMETCompositionPhoton_wpy().push_back( fMETCompositionPhoton_wpy.at(i) );
	etMiss->GetMETCompositionPhoton_statusWord().push_back( fMETCompositionPhoton_statusWord.at(i) );
	etMiss->GetMETCompositionPhoton_index().push_back( fMETCompositionPhoton_index.at(i) );
      }

    // Tau
    wSize = fMETCompositionTau_wet.size();
    etMiss->GetMETCompositionTau_wet().reserve( wSize );
    etMiss->GetMETCompositionTau_wpx().reserve( wSize );
    etMiss->GetMETCompositionTau_wpy().reserve( wSize );
    etMiss->GetMETCompositionTau_statusWord().reserve( wSize );
    etMiss->GetMETCompositionTau_index().reserve( wSize );

    for(UInt_t i = 0; i < wSize; i++)
      {
	etMiss->GetMETCompositionTau_wet().push_back( fMETCompositionTau_wet.at(i) );
	etMiss->GetMETCompositionTau_wpx().push_back( fMETCompositionTau_wpx.at(i) );
	etMiss->GetMETCompositionTau_wpy().push_back( fMETCompositionTau_wpy.at(i) );
	etMiss->GetMETCompositionTau_statusWord().push_back( fMETCompositionTau_statusWord.at(i) );
	etMiss->GetMETCompositionTau_index().push_back( fMETCompositionTau_index.at(i) );
      }


    // Muon
    wSize = fMETCompositionMuon_wet.size();
    etMiss->GetMETCompositionMuon_wet().reserve( wSize );
    etMiss->GetMETCompositionMuon_wpx().reserve( wSize );
    etMiss->GetMETCompositionMuon_wpy().reserve( wSize );
    etMiss->GetMETCompositionMuon_statusWord().reserve( wSize );
    etMiss->GetMETCompositionMuon_index().reserve( wSize );

    for(UInt_t i = 0; i < wSize; i++)
      {
	etMiss->GetMETCompositionMuon_wet().push_back( fMETCompositionMuon_wet.at(i) );
	etMiss->GetMETCompositionMuon_wpx().push_back( fMETCompositionMuon_wpx.at(i) );
	etMiss->GetMETCompositionMuon_wpy().push_back( fMETCompositionMuon_wpy.at(i) );
	etMiss->GetMETCompositionMuon_statusWord().push_back( fMETCompositionMuon_statusWord.at(i) );
	etMiss->GetMETCompositionMuon_index().push_back( fMETCompositionMuon_index.at(i) );
      }


    return 0;
}

Int_t ZeeDROOTEtMiss::fillIn (const ZeeDEtMiss* const etMiss)
{
    // Fills ZeeDROOTEtMiss with the information from ZeeDEtMiss

    fEt  = etMiss->fEt;
    fEtX = etMiss->fEtX;
    fEtY = etMiss->fEtY;
    fPhi = etMiss->fPhi;

    fCorRecoilEt  = etMiss->fCorRecoilEt;
    fCorRecoilEtX = etMiss->fCorRecoilEtX;
    fCorRecoilEtY = etMiss->fCorRecoilEtY;
    fCorRecoilPhi = etMiss->fCorRecoilPhi;
	fCorRecoilSumet = etMiss->fCorRecoilSumet;

    fMET_SoftTermETX   = etMiss-> fMET_SoftTermETX; 
    fMET_SoftTermETY   = etMiss-> fMET_SoftTermETY;
    fMET_SoftTermETSUM = etMiss-> fMET_SoftTermETSUM; 
    
    UInt_t wSize(0);

    //Jet
    wSize = etMiss->GetMETCompositionJet_wet().size();
    fMETCompositionJet_wet       .reserve( wSize );
    fMETCompositionJet_wpx       .reserve( wSize );       
    fMETCompositionJet_wpy       .reserve( wSize );       
    fMETCompositionJet_statusWord.reserve( wSize );
    fMETCompositionJet_index     .reserve( wSize );
    for(vectorF::const_iterator it = etMiss->GetMETCompositionJet_wet().begin(), stop = etMiss->GetMETCompositionJet_wet().end(); it!=stop; ++it )
      {
	Int_t position( std::distance( etMiss->GetMETCompositionJet_wet().begin(), it ) );
	fMETCompositionJet_wet       .push_back( etMiss->GetMETCompositionJet_wet()[ position ] );
	fMETCompositionJet_wpx       .push_back( etMiss->GetMETCompositionJet_wpx()[ position ] );
	fMETCompositionJet_wpy       .push_back( etMiss->GetMETCompositionJet_wpy()[ position ] );
	fMETCompositionJet_statusWord.push_back( etMiss->GetMETCompositionJet_statusWord()[ position ] );
	fMETCompositionJet_index     .push_back( etMiss->GetMETCompositionJet_index()[ position ] );     
      }

    //Electron
    wSize = etMiss->GetMETCompositionElectron_wet().size();
    fMETCompositionElectron_wet       .reserve( wSize );
    fMETCompositionElectron_wpx       .reserve( wSize );       
    fMETCompositionElectron_wpy       .reserve( wSize );       
    fMETCompositionElectron_statusWord.reserve( wSize );
    fMETCompositionElectron_index     .reserve( wSize );
    for(vectorF::const_iterator it = etMiss->GetMETCompositionElectron_wet().begin(), stop = etMiss->GetMETCompositionElectron_wet().end(); it!=stop; ++it )
      {
	Int_t position( std::distance( etMiss->GetMETCompositionElectron_wet().begin(), it ) );
	fMETCompositionElectron_wet       .push_back( etMiss->GetMETCompositionElectron_wet()[ position ] );
	fMETCompositionElectron_wpx       .push_back( etMiss->GetMETCompositionElectron_wpx()[ position ] );
	fMETCompositionElectron_wpy       .push_back( etMiss->GetMETCompositionElectron_wpy()[ position ] );
	fMETCompositionElectron_statusWord.push_back( etMiss->GetMETCompositionElectron_statusWord()[ position ] );
	fMETCompositionElectron_index     .push_back( etMiss->GetMETCompositionElectron_index()[ position ] );     
      }

    //Photon
    wSize = etMiss->GetMETCompositionPhoton_wet().size();
    fMETCompositionPhoton_wet       .reserve( wSize );
    fMETCompositionPhoton_wpx       .reserve( wSize );       
    fMETCompositionPhoton_wpy       .reserve( wSize );       
    fMETCompositionPhoton_statusWord.reserve( wSize );
    fMETCompositionPhoton_index     .reserve( wSize );
    for(vectorF::const_iterator it = etMiss->GetMETCompositionPhoton_wet().begin(), stop = etMiss->GetMETCompositionPhoton_wet().end(); it!=stop; ++it )
      {
	Int_t position( std::distance( etMiss->GetMETCompositionPhoton_wet().begin(), it ) );
	fMETCompositionPhoton_wet       .push_back( etMiss->GetMETCompositionPhoton_wet()[ position ] );
	fMETCompositionPhoton_wpx       .push_back( etMiss->GetMETCompositionPhoton_wpx()[ position ] );
	fMETCompositionPhoton_wpy       .push_back( etMiss->GetMETCompositionPhoton_wpy()[ position ] );
	fMETCompositionPhoton_statusWord.push_back( etMiss->GetMETCompositionPhoton_statusWord()[ position ] );
	fMETCompositionPhoton_index     .push_back( etMiss->GetMETCompositionPhoton_index()[ position ] );     
      }

    //Tau
    wSize = etMiss->GetMETCompositionTau_wet().size();
    fMETCompositionTau_wet       .reserve( wSize );
    fMETCompositionTau_wpx       .reserve( wSize );       
    fMETCompositionTau_wpy       .reserve( wSize );       
    fMETCompositionTau_statusWord.reserve( wSize );
    fMETCompositionTau_index     .reserve( wSize );
    for(vectorF::const_iterator it = etMiss->GetMETCompositionTau_wet().begin(), stop = etMiss->GetMETCompositionTau_wet().end(); it!=stop; ++it )
      {
	Int_t position( std::distance( etMiss->GetMETCompositionTau_wet().begin(), it ) );
	fMETCompositionTau_wet       .push_back( etMiss->GetMETCompositionTau_wet()[ position ] );
	fMETCompositionTau_wpx       .push_back( etMiss->GetMETCompositionTau_wpx()[ position ] );
	fMETCompositionTau_wpy       .push_back( etMiss->GetMETCompositionTau_wpy()[ position ] );
	fMETCompositionTau_statusWord.push_back( etMiss->GetMETCompositionTau_statusWord()[ position ] );
	fMETCompositionTau_index     .push_back( etMiss->GetMETCompositionTau_index()[ position ] );     
      }

    //Muon
    wSize = etMiss->GetMETCompositionMuon_wet().size();
    fMETCompositionMuon_wet       .reserve( wSize );
    fMETCompositionMuon_wpx       .reserve( wSize );       
    fMETCompositionMuon_wpy       .reserve( wSize );       
    fMETCompositionMuon_statusWord.reserve( wSize );
    fMETCompositionMuon_index     .reserve( wSize );
    for(vectorF::const_iterator it = etMiss->GetMETCompositionMuon_wet().begin(), stop = etMiss->GetMETCompositionMuon_wet().end(); it!=stop; ++it )
      {
	Int_t position( std::distance( etMiss->GetMETCompositionMuon_wet().begin(), it ) );
	fMETCompositionMuon_wet       .push_back( etMiss->GetMETCompositionMuon_wet()[ position ] );
	fMETCompositionMuon_wpx       .push_back( etMiss->GetMETCompositionMuon_wpx()[ position ] );
	fMETCompositionMuon_wpy       .push_back( etMiss->GetMETCompositionMuon_wpy()[ position ] );
	fMETCompositionMuon_statusWord.push_back( etMiss->GetMETCompositionMuon_statusWord()[ position ] );
	fMETCompositionMuon_index     .push_back( etMiss->GetMETCompositionMuon_index()[ position ] );     
      }
    return 0;
}

