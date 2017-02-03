#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDboson.h
////////////////////////////////////////////////////////
//
// Contact persons: Sebastian Schmitt
//

// STD includes
#include <utility>


// Root includes
#include <TObjArray.h>
#include <TLorentzVector.h>
#include <TVector3.h>

#include "ZeeDTools/ZeeDQuantity.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDLepton.h"


/** Virtual class for Z/W boson particles
  @author SG
  @date 2011/02/07
 */


class ZeeDBoson : public TObject {

  public:


    struct BosonType { enum Value { Zee, Zmumu,  Zemu, Wenu, Wmunu, Undefined }; };
    typedef std::pair<const ZeeDLepton*, const ZeeDLepton*> LepParticlesPair;
    void SetBosonType (BosonType::Value type) {fbosonType = type;}; 


    ZeeDBoson();
    ZeeDBoson(const ZeeDBoson& boson);
    ZeeDBoson& operator= (const ZeeDBoson& rhs);
    ~ZeeDBoson();


    // Getters
//    LeptonType::Value GetLeptonType() { return fleptonType }
    BosonType::Value GetBosonType() { return fbosonType; }

    const TLorentzVector GetFourVector() const { return fFourVector; }

    double GetMass() const { return fMass; }

    float GetCharge() const { return fCharge; }

    TVector3 GetVertex() const 	{ return fVertexVector; }

    TVector3 GetVerErr2() const { return fVerErr2Vector; }

    /** Get first pt-ordered em candidate */
    const ZeeDLepton* GetFirstPtLep() { return fPtPair.first; }

    /** Get second pt-ordered em candidate */
    const ZeeDLepton* GetSecondPtLep() { return fPtPair.second; }

    /** Get positive em candidate */
    const ZeeDLepton* GetPositiveLep() { return fPosNegPair.first; }

    /** Get negative em candidate */
    const ZeeDLepton* GetNegativeLep() { return fPosNegPair.second; }

    /** Get more forwart em candidate */
    const ZeeDLepton* GetCentralLep() { return fCentralForwardPair.second; }

    /** Get more backward em candidate */
    const ZeeDLepton* GetForwardLep() { return fCentralForwardPair.second; }

    /** Get CosTh* measured in frame v*/
    double GetCosThStar(ZeeDEnum::CosThStar::Value v = ZeeDEnum::CosThStar::Default) const {
      return fCosThStarVec[v];
    }


    // Setters
    void SetFourVector(const TLorentzVector& fourVector) { fFourVector = fourVector; }

    void SetCharge(float charge)  { fCharge = charge; }

    void SetMass(double mass) { fMass = mass; }

    void SetVertex(TVector3 Vector)  { fVertexVector = Vector; }

    void SetVerErr2(TVector3 Vector) { fVerErr2Vector = Vector; }

    void SetPtLepPair(LepParticlesPair empair) { fPtPair = empair; }

    void SetPosNegLepPair(LepParticlesPair empair) { fPosNegPair = empair; }

    void SetCentralForwardLepPair(LepParticlesPair empair) { fCentralForwardPair = empair; }

    /** Set CosTh* measured in frame v*/
    void SetCosThStar(double cosThetaStar, ZeeDEnum::CosThStar::Value v = ZeeDEnum::CosThStar::Default) {
      fCosThStarVec[v] = cosThetaStar;
    }

    /** Print out boson properties */
    virtual void Print(Option_t* option = "") const;


  protected:

    /**  Boson type (Zee, Zmumu, Wenu, Wmunu) */
    BosonType::Value fbosonType;

    /**  Boson four-vector */
    TLorentzVector fFourVector;

    /**  Boson mass */
    double fMass;

    /** Boson charge */
    float fCharge; 

    /** Pt-ordered pair of em candidates */
    LepParticlesPair fPtPair;

    /**  Pos-neg four-vectors */
    LepParticlesPair fPosNegPair;

    /**  Pos-neg four-vectors */
    LepParticlesPair fCentralForwardPair;

    /**  Vertex position and Err**2 3-vectors  */
    TVector3 fVertexVector;
    TVector3 fVerErr2Vector;

    /** cosTheta* angles */
    std::vector<double> fCosThStarVec;


};

