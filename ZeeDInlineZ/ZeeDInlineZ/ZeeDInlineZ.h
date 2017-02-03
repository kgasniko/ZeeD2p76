#ifndef __ZEEDINLINEZ
#define __ZEEDINLINEZ

////////////////////////////////////////////////////////
// Name    : ZeeDInlineZ
////////////////////////////////////////////////////////

// Std includes
#include <string>

// Root includes
#include <TLorentzVector.h>
#include <TH1.h>
#include <TRandom3.h>

// ZeeD analysis includes
#include "ZeeDArrays/ZeeDBinGrid.h"

#include "ZeeDTools/ZeeDSvcHelper.h"

// Root headers
#include <TObjArray.h>


/** It read histograms, Build random Z use this histogram ,Generate 2
    electron 4-vector from Z decey.

    @author Mikhail Karnevskiy.
    @date 21.10.2008
 */

class ZeeDInlineZ {

public:
    ZeeDInlineZ();
    ~ZeeDInlineZ();

    TLorentzVector* m_Elec1, *m_Elec2, *m_Elec1_Smear, *m_Elec2_Smear, *m_Z;
    TLorentzVector* m_Elec1_Rad, *m_Elec2_Rad, *m_Z_Rad ;

    /** Method for full generate event include Radiations, Smiaring and Efficiency */
    void CalcEvent();

    //  Gets methods

    /** Get Generat electron (with/without radiation checked flag) */
    TLorentzVector GetGenElec(int IElec);

    /** Get smearing electron (with/without radiation checked flag) */
    TLorentzVector GetGenElecSmear(int IElec);
    TLorentzVector GetGenZ() {
        return *m_Z;
    }
    TLorentzVector GetGenZRad() {
        return *m_Z_Rad;
    }

    /** Get radiativ electron */
    TLorentzVector GetGenElecRad(int IElec);

    /** Get No radiativ electron */
    TLorentzVector GetGenElecNoRad(int IElec);

    /** Get ISEM for generate elestron */
    bool GetIsEM(int IElec);

private:

    bool m_IsEM1, m_IsEM2;

    /** Generations of z boson and two electron 4-vector */
    void CulcZ();

    /** Get smearing for electron */
    TLorentzVector GetESmear(TLorentzVector* elec);

    /** Calc EM */
    bool CalcIsEM(TLorentzVector* Elec);

    /** Reset all internal var. */
    void Reset();

    // Methods for ForTran interface

    /** Inicialization of Photos */
    inline void PhotosInitc();

    /** Set parametrs for Photos (set 4-vector of two electrons and z boson) */
    void SetPhotosParams();

    /** Get all parameter from Photos (4-vectors of all particle) */
    void GetPhotosParams(TLorentzVector* out_z, TLorentzVector* out_elecm,
                         TLorentzVector* out_elecp);

    /** Photos Calculations */
    void GetPhotos();


    std::string  m_InFileName;
    int          m_NumEvents;
    int          m_SeedGen;
    double       m_ESmear;
    bool         m_UseRadiativ;


    //It is Bins
    ZeeDBinGrid* m_binGrid;

    // Histograms of ZRapidity and Zpt
    TH1D* m_HZRap;
    TH1D* m_HZPt;

    // Random number genarator
    TRandom3* m_Rndm;

    /**  Mass of the Z boson */
    static const double m_ZMass;  //!

    /**  Z boson width */
    static const double m_ZWidth; //!

    /**  pi number */
    static const double m_PI;     //!

    /**  Mass of the Electron */
    static const double m_EMass;  //!

    ZeeDSvcHelper fSvcHelper;

};

#endif
