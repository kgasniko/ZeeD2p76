#ifndef ZEEDFUNCTIONS
#define ZEEDFUNCTIONS

//
// Inline store of commonly used functions
//

#include <TLorentzVector.h>
#include "ZeeDMisc/ZeeDMisc.h"

namespace ZeeDFunctions
{
    inline double cosThStarCS(const TLorentzVector *epos, const TLorentzVector *eneg) 
    {
        // see: https://cds.cern.ch/record/1544035/files/ATLAS-CONF-2013-043.pdf (formula 2)
        // also consistent with JK private code

        // The combined vector:
        TLorentzVector diel = *epos + *eneg;        
        //components of the cos(th*) calculation
        double Lplus  = eneg->E()+eneg->Pz();
        double Lminus = eneg->E()-eneg->Pz();
        double Pplus  = epos->E()+epos->Pz();
        double Pminus = epos->E()-epos->Pz();

        //does the cos(th*) calculation
        double cosThetaCS  = (Lplus*Pminus - Lminus*Pplus);
        double Pz = diel.Pz();
        double Pt = diel.Pt();
        double M  = diel.M();
        cosThetaCS *= Pz/fabs(Pz);
        cosThetaCS /= M;
        cosThetaCS /= sqrt(M*M + Pt*Pt);

        return cosThetaCS;
        
    }

    inline double cosThStarZ(const TLorentzVector *epos, const TLorentzVector *eneg) 
    {
        // decay angle in Z rest frame
        const TLorentzVector fourVector = *epos + *eneg;

        // Boost vector:
        const TVector3 boostV = fourVector.BoostVector();

        // Prepare positron boosted to the Z rest frame:
        TLorentzVector e_boosted = *eneg;

        e_boosted.Boost(-boostV);

        double cosTheta = cos(  e_boosted.Angle(fourVector.Vect()) );
        return cosTheta;        
    }


    inline double cosThStarBeam(const TLorentzVector *epos, const TLorentzVector *eneg) 
    {
        // decay angle in beam frame
        const TLorentzVector fourVector = *epos + *eneg;

        Double_t cosTheta = TMath::TanH( (eneg->Eta() -  epos->Eta())/2.);

        if ( fourVector.Rapidity() < 0 ) {
            cosTheta = -cosTheta;            
        }
        return cosTheta;        
    }

    inline double cosThStar(ZeeDEnum::CosThStar::Value CosType, const TLorentzVector *epos, const TLorentzVector *eneg) 
    {
        if ( CosType == ZeeDEnum::CosThStar::CollinsSoper) {
            return cosThStarCS(epos,eneg);            
        }
        if ( CosType == ZeeDEnum::CosThStar::ZFlightDirection) {
            return cosThStarZ(epos,eneg);            
        }
        if ( CosType == ZeeDEnum::CosThStar::BeamDirection) {
            return cosThStarBeam(epos,eneg);            
        }
        return 0;        
    }

}


#endif
