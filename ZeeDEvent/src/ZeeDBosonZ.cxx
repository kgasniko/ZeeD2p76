#include "ZeeDEvent/ZeeDBosonZ.h"

#include <iostream>


//------------------------------------------------------
ZeeDBosonZ::ZeeDBosonZ()
    : ZeeDBoson()
{
    // Constructor
    //fbosonType = BosonType::Zee;

    fPtPair.first = NULL;
    fPtPair.second = NULL;

    fPosNegPair.first = NULL;
    fPosNegPair.second = NULL;

    fCentralForwardPair.first = NULL;
    fCentralForwardPair.second = NULL;

    fCosThStarVec.resize(ZeeDEnum::CosThStar::N);

}

//------------------------------------------------------
ZeeDBosonZ::ZeeDBosonZ(const ZeeDBosonZ& boson) : ZeeDBoson(boson)
{
    // Copy constructor

    fbosonType	   = boson.fbosonType;
    fMass          = boson.fMass;
    fPtPair   	   = boson.fPtPair;
    fPosNegPair    = boson.fPosNegPair;
    fCentralForwardPair = boson.fCentralForwardPair;
    fCosThStarVec  = boson.fCosThStarVec;
    
}

//------------------------------------------------------
ZeeDBosonZ& ZeeDBosonZ::operator= (const ZeeDBosonZ& rhs)
{
  
    // Assignment operator
    if (this == &rhs ) {
        return *this;
    }

    fbosonType	   = rhs.fbosonType;
    fMass          = rhs.fMass;
    fPtPair   	   = rhs.fPtPair;
    fPosNegPair    = rhs.fPosNegPair;
    fCentralForwardPair = rhs.fCentralForwardPair;
    fCosThStarVec  = rhs.fCosThStarVec;

    return *this;

}

//------------------------------------------------------
ZeeDBosonZ::~ZeeDBosonZ()
{
}

//------------------------------------------------------
std::ostream& operator<< (std::ostream& s, const  ZeeDBosonZ& Z)
{
    TLorentzVector FV = Z.GetFourVector();

    s << " 3 vector (Px,Py,Pz): (" <<  FV.Px() << "," << FV.Py() << "," << FV.Pz() << "), mass:" << FV.M() << std::endl;
    return s;
}

