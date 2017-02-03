#include "ZeeDEvent/ZeeDBosonEMu.h"

#include <iostream>


//------------------------------------------------------
ZeeDBosonEMu::ZeeDBosonEMu()
    : ZeeDBoson()
{
    // Constructor
    fbosonType	= BosonType::Zemu;
    fMass	= 0.;
    fElectron	= NULL;
    fMuon	= NULL;

    fCosThStarVec.resize(ZeeDEnum::CosThStar::N);

}

//------------------------------------------------------
ZeeDBosonEMu::ZeeDBosonEMu(const ZeeDBosonEMu& boson) : ZeeDBoson(boson)
{
    // Copy constructor

    fbosonType	= boson.fbosonType;
    fMass	= boson.fMass;
    fElectron	= boson.fElectron;
    fMuon	= boson.fMuon;

    fCosThStarVec = boson.fCosThStarVec;
    
}

//------------------------------------------------------
ZeeDBosonEMu& ZeeDBosonEMu::operator= (const ZeeDBosonEMu& rhs)
{
  
    // Assignment operator
    if (this == &rhs ) {
        return *this;
    }

    fbosonType	= rhs.fbosonType;
    fMass	= rhs.fMass;
    fElectron	= rhs.fElectron;
    fMuon	= rhs.fMuon;

    fCosThStarVec = rhs.fCosThStarVec;

    return *this;

}

//------------------------------------------------------
ZeeDBosonEMu::~ZeeDBosonEMu()
{
    // Destructor
}

std::ostream& operator<< (std::ostream& s, const  ZeeDBosonEMu& Z)
{
    TLorentzVector FV = Z.GetFourVector();

    s << " 3 vector (Px,Py,Pz): (" <<  FV.Px() << "," << FV.Py() << "," << FV.Pz() << "), mass:" << FV.M() << std::endl;
    return s;
}

