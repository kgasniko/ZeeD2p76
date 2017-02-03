#include "ZeeDInlineZ/ZeeDInlineZ.h"

// std includes
#include <iostream>

// Root includes
#include <TFile.h>
#include <TMath.h>
#include <TROOT.h>

// Athena includes
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/ITHistSvc.h"

// ZeeD analysis includes
#include "ZeeDArrays/ZeeDGridCell.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"

// #include <stdlib>
// #include <math>
/////////////////////////////////////////////////////////////////////////////

using std::cout;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
const double ZeeDInlineZ::m_ZMass             = 91.1876;
const double ZeeDInlineZ::m_ZWidth            = 2.4952;
const double ZeeDInlineZ::m_PI                = TMath::Pi();
const double ZeeDInlineZ::m_EMass             = 0.0005;
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
extern "C"
{
    void photosinit_();
    void setphotosparam_(double in_z[], double in_elecm[], double in_elecp[]);
    void getphotos_();
    void getphotosparams_(double out_z[], double out_elecm[], double out_elecp[],
                          double out_phot1[], double out_phot2[] );
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ZeeDInlineZ::ZeeDInlineZ()
{
    // StatusCode sc = StatusCode::SUCCESS;

    // ServiceHandle<ITHistSvc>* histSvc = fSvcHelper.GetTHistSvc();
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    m_InFileName    = (*ZeeDAnaOptions)->ZeeDInlineZ_InFileName();
    m_SeedGen       = (*ZeeDAnaOptions)->ZeeDInlineZ_SeedGen();
    m_ESmear        = (*ZeeDAnaOptions)->ZeeDInlineZ_ESmear();
    m_UseRadiativ   = (*ZeeDAnaOptions)->ZeeDInlineZ_UseRadiation();

    TFile* fileH1 = new TFile(ZeeDMisc::FindFile(m_InFileName));

    // Read/create new histos in memory:
    m_HZRap = (TH1D*)fileH1->Get("ZBosEtaGen");
    m_HZPt  = (TH1D*)fileH1->Get("ZBosPtGen");

    // Change association of the histograms so that they are not deleted
    // when file is closed:
    m_HZRap ->SetDirectory(gROOT);
    m_HZPt  ->SetDirectory(gROOT);

    fileH1->Close();
    delete fileH1;

    m_Rndm = new TRandom3(m_SeedGen);
    gRandom  -> SetSeed(m_SeedGen);

    m_Elec1       = new TLorentzVector(0., 0., 0., 0.);
    m_Elec2       = new TLorentzVector(0., 0., 0., 0.);
    m_Elec1_Smear = new TLorentzVector(0., 0., 0., 0.);
    m_Elec2_Smear = new TLorentzVector(0., 0., 0., 0.);
    m_Z           = new TLorentzVector(0., 0., 0., 0.);
    m_Z_Rad       = new TLorentzVector(0., 0., 0., 0.);
    m_Elec1_Rad   = new TLorentzVector(0., 0., 0., 0.);
    m_Elec2_Rad   = new TLorentzVector(0., 0., 0., 0.);


    // Read bin grid with Acceptance
    m_binGrid = new ZeeDBinGrid();
    m_binGrid ->ReadFromFile(ZeeDMisc::FindFile("Inline/Inline_Acceptance.txt")); // source for Inline Acceptance

    // Printing Bins
    m_binGrid->Print();

    if (m_UseRadiativ == true) {
        PhotosInitc();
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
ZeeDInlineZ::~ZeeDInlineZ()
{
    // Destructor
    delete m_Rndm;
    delete m_Elec1;
    delete m_Elec2;
    delete m_Elec1_Smear;
    delete m_Elec2_Smear;
    delete m_Z;
    delete m_binGrid;
    delete m_Z_Rad;
    delete m_Elec1_Rad;
    delete m_Elec2_Rad;


}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ZeeDInlineZ::CulcZ()
{

    TLorentzVector v1(1., 1., 1., 1.); // 1 electron
    TLorentzVector v2(1., 1., 1., 1.); // 2 electron
    TLorentzVector z(1., 0., 0., 2.); // Z bozon
    double mu, m, r;
    double ZRap = m_HZRap  -> GetRandom();
    double ZPt = m_HZPt -> GetRandom();

    mu = m_Rndm -> BreitWigner(m_ZMass , m_ZWidth); // mass of Z of Gv
    m = m_EMass; // mass of electron of Gv

    double px, py, pz, pt, y, E, q;

    y = ZRap;
    pt = ZPt;
    double phiZ = m_Rndm -> Rndm() * 2 * m_PI ;

    px = pt * cos(phiZ);
    py = pt * sin(phiZ);

    q = (4 * exp(2 * y)) / ((exp(2 * y) + 1) * (exp(2 * y) + 1));
    E = sqrt((mu * mu + pt * pt) / (q));
    pz = E * ((exp(2 * y) - 1) / (exp(2 * y) + 1));

    z[0] = px;
    z[1] = py;
    z[2] = pz;
    z[3] = E;

    *m_Z = z; // Acsess value Gen. Z to m_Z

    TVector3 b; // boost vector
    b = z.BoostVector(); // calculate of boost vector


    // calculate electron momentum vector
    r = mu / 2;  // r*r - square of electron pulse in system of centrum mass

    // give random angle of electron direction
    double vec[3];
    double u1 = 0;
    double u2 = 0;
    double s  = 0;

    while ( true ) {
        u1 = 2.* m_Rndm -> Rndm() - 1.;
        u2 = 2.* m_Rndm -> Rndm() - 1.;

        s = u1 * u1 + u2 * u2;

        if (s < 1.) {
            break;
        }
    }

    double co2 = 2. * sqrt(1. - s);
    vec[0] = u1 * co2;
    vec[1] = u2 * co2;
    vec[2] = 1. - 2.*s;


    // give 4 pulse of 1 electrom in system of centrum mass
    v1[0] = r * vec[0]; // cos(phi) * sin(teta) ;
    v1[1] = r * vec[1]; // sin(phi) * sin(teta) ;
    v1[2] = r * vec[2]; // cos(teta) ;
    v1[3] = sqrt((r * r) + (m * m)) ;

    // give 4 pulse of 2 electrom in system of centrum mass
    v2[0] = -v1[0];
    v2[1] = -v1[1];
    v2[2] = -v1[2];
    v2[3] = v1[3];


    // boost v1 and v2 in laboratory system
    v1.Boost(b);
    v2.Boost(b);

    *m_Elec1 = v1;
    *m_Elec2 = v2;

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
TLorentzVector ZeeDInlineZ::GetESmear(TLorentzVector* elec)
{
    // Calculation Smearing for electron

    // give energi diviation
    double E = (*elec)[3]; // Energy of elevrton
    double E_new, Psq, Psqnew, m;
    E_new = m_Rndm -> Gaus(E, (m_ESmear * E)) * 0.9955; // Energy after diviation

    Psq = (*elec)[0] * (*elec)[0] + (*elec)[1] * (*elec)[1] + (*elec)[2] * (*elec)[2];
    m = m_EMass; // mass of electron
    Psqnew = (E_new * E_new) - m * m;
    TLorentzVector NewElec;

    NewElec[0] = (*elec)[0] * sqrt(Psqnew / Psq );
    NewElec[1] = (*elec)[1] * sqrt(Psqnew / Psq );
    NewElec[2] = (*elec)[2] * sqrt(Psqnew / Psq );
    NewElec[3] = E_new;

    return NewElec;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
bool ZeeDInlineZ::CalcIsEM(TLorentzVector* Elec )
{
    // Calculate efficiency of detectors for Z generator
    double value = m_binGrid -> GetValue( Elec->Eta() ); // Value of search cells
    double rmd = (m_Rndm -> Rndm() );

    return ( rmd < value ) ? true : false;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void  ZeeDInlineZ::SetPhotosParams()
{

    double Z[5], Elecm[5], Elecp[5];

    for(int i = 0; i < 4 ; ++i ) {

        Z[i]     = (*m_Z)[i];
        Elecm[i] = (*m_Elec1)[i];
        Elecp[i] = (*m_Elec2)[i];
    }

    Z[4]     = m_ZMass;
    Elecm[4] = m_EMass;
    Elecp[4] = m_EMass;

    setphotosparam_(Z, Elecm, Elecp);

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ZeeDInlineZ::CalcEvent()
{

    Reset();
    this->CulcZ();

    m_IsEM1 = CalcIsEM(m_Elec1);
    m_IsEM2 = CalcIsEM(m_Elec2);

    if (m_UseRadiativ == true) {

        // Set our parametrs to Photos Package

        SetPhotosParams();

        // Calculate radiation
        GetPhotos();

        GetPhotosParams(m_Z_Rad, m_Elec1_Rad, m_Elec2_Rad );

        *m_Elec1_Smear =  GetESmear(m_Elec1_Rad);
        *m_Elec2_Smear =  GetESmear(m_Elec2_Rad);

    } else {
        *m_Elec1_Smear =  GetESmear(m_Elec1);
        *m_Elec2_Smear =  GetESmear(m_Elec2);
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
TLorentzVector ZeeDInlineZ::GetGenElec(int IElec)
{

    if (m_UseRadiativ == true) {
        return IElec == 0 ? *m_Elec1_Rad : *m_Elec2_Rad;
    } else {
        return IElec == 0 ? *m_Elec1 : *m_Elec2;
    }

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
TLorentzVector ZeeDInlineZ::GetGenElecSmear(int IElec)
{
    return IElec == 0 ? *m_Elec1_Smear : *m_Elec2_Smear;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
bool ZeeDInlineZ::GetIsEM(int IElec)
{
    bool ISEM = false;

    if ((IElec == 0) && (m_IsEM1 == true)) {
        ISEM = true;
    }

    if ((IElec == 1) && (m_IsEM2 == true)) {
        ISEM = true;
    }

    return ISEM;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
TLorentzVector ZeeDInlineZ::GetGenElecNoRad(int IElec)
{
    return IElec == 0 ? *m_Elec1 : *m_Elec2;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
TLorentzVector ZeeDInlineZ::GetGenElecRad(int IElec)
{
    return IElec == 0 ? *m_Elec1_Rad : *m_Elec2_Rad;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
inline void ZeeDInlineZ::PhotosInitc()
{
    photosinit_();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ZeeDInlineZ::GetPhotos()
{
    getphotos_();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ZeeDInlineZ::GetPhotosParams(TLorentzVector* out_z, TLorentzVector* out_elecm,
                                  TLorentzVector* out_elecp)
{
    double Z[5], Elecm[5], Elecp[5], phot1[5], phot2[5];

    getphotosparams_(Z, Elecm, Elecp, phot1, phot2);

    for(int i = 0; i < 4 ; ++i ) {
        (*out_z)[i]      = Z[i];
        (*out_elecm)[i]  = Elecm[i];
        (*out_elecp)[i]  = Elecp[i];
    }

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void ZeeDInlineZ::Reset()
{
    m_Elec1       -> SetPxPyPzE(0., 0., 0., 0.);
    m_Elec2       -> SetPxPyPzE(0., 0., 0., 0.);
    m_Elec1_Smear -> SetPxPyPzE(0., 0., 0., 0.);
    m_Elec2_Smear -> SetPxPyPzE(0., 0., 0., 0.);
    m_Z           -> SetPxPyPzE(0., 0., 0., 0.);
    m_Z_Rad       -> SetPxPyPzE(0., 0., 0., 0.);
    m_Elec1_Rad   -> SetPxPyPzE(0., 0., 0., 0.);
    m_Elec2_Rad   -> SetPxPyPzE(0., 0., 0., 0.);

}
