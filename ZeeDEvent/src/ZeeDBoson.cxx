#include "ZeeDEvent/ZeeDBoson.h"

// std libraries
#include <iostream>

// ZeeD libraries
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDBoson::ZeeDBoson()
{
    // Constructor
    fbosonType = BosonType::Undefined;

    fFourVector.SetXYZT(0., 0., 0., 0.);

    fMass = 0.0;
    fCharge = 0.0;

    fPtPair.first  = NULL;
    fPtPair.second = NULL;

    fPosNegPair.first  = NULL;
    fPosNegPair.second = NULL;

    fCentralForwardPair.first  = NULL;
    fCentralForwardPair.second = NULL;

    fVertexVector.SetXYZ(0., 0., 0.);
    fVerErr2Vector.SetXYZ(0., 0., 0.);

    fCosThStarVec.resize(ZeeDEnum::CosThStar::N);

}

//------------------------------------------------------
ZeeDBoson::ZeeDBoson(const ZeeDBoson& boson) : TObject(boson)
{
    // Copy constructor

    fbosonType	   = boson.fbosonType;
    fFourVector    = boson.fFourVector;
    fPtPair	   = boson.fPtPair;
    fPosNegPair	   = boson.fPosNegPair;
    fCentralForwardPair	= boson.fCentralForwardPair;
    fVertexVector  = boson.fVertexVector;
    fVerErr2Vector = boson.fVerErr2Vector;
    fMass          = boson.fMass;
    fCharge	   = boson.fCharge;

}

//------------------------------------------------------
ZeeDBoson& ZeeDBoson::operator= (const ZeeDBoson& rhs)
{
  
    // Assignment operator
    if (this == &rhs ) {
        return *this;
    }

    fbosonType	   = rhs.fbosonType;
    fFourVector    = rhs.fFourVector;
    fPtPair	   = rhs.fPtPair;
    fPosNegPair	   = rhs.fPosNegPair;
    fCentralForwardPair	= rhs.fCentralForwardPair;
    fVertexVector  = rhs.fVertexVector;
    fVerErr2Vector = rhs.fVerErr2Vector;
    fMass          = rhs.fMass;
    fCharge	   = rhs.fCharge;

    return *this;
}

//------------------------------------------------------
ZeeDBoson::~ZeeDBoson()
{
}

//------------------------------------------------------
void ZeeDBoson::Print(Option_t* option) const
{
    // Prints out boson properties
    if ( !option || !option[0] ) {
        std::cout << option << std::endl;
    }

    ZeeDMisc::Dump4Vec("boson       ", fFourVector);
    std::cout << "  Mass = " << fMass << std::endl;
}

