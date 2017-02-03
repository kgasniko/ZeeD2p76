#ifndef ZEEDELECTRONBAGS_HH
#define ZEEDELECTRONBAGS_HH

#include <iosfwd>
#include <map>

#include <TLorentzVector.h>
#include <TVector3.h>
#include <TString.h>

#include "ZeeDMisc/ZeeDMisc.h"

namespace HepMC {

class GenParticle;

}

namespace ZeeDLeptonBags {

  struct TrackSummary {

    TrackSummary() :
      numberOfBLayerHits(0), numberOfPixelHits(0), numberOfSCTHits(0), numberOfBLayerOutliers(0), numberOfPixelOutliers(0), numberOfSCTOutliers(0) {
    }

  unsigned int numberOfBLayerHits;
  unsigned int numberOfPixelHits;
  unsigned int numberOfSCTHits;

  unsigned int numberOfBLayerOutliers;
  unsigned int numberOfPixelOutliers;
  unsigned int numberOfSCTOutliers;

  unsigned int numberOfTRTHits;
  unsigned int numberOfTRTOutliers;
  unsigned int numberOfTRTHighThresholdHits;
  unsigned int numberOfTRTHighThresholdOutliers;

  unsigned int numberOfCscEtaHits;

  };



 struct TrackParticle {

    TrackParticle() :
            pt(0), eta(0) {
    }

    double pt;
    double eta;

    TLorentzVector fourVector;
 };

 struct MCTruth {

    MCTruth() :
            truthLeptonType(0) {
    }

    int truthLeptonType;

    const HepMC::GenParticle* trueLepton;

    TLorentzVector fourVector;

 };

 struct Vertex {

    /** Vertex 3-vector */
    TVector3 position;

    /** Vertex Err**2 3-vector */
    TVector3 Err2;

 };

 struct IsEM {

    IsEM();

    bool isFrwdLoose;
    bool isFrwdTight;

    bool isNewFwdLoose;
    bool isNewFwdMedium;
    bool isNewFwdTight;

    bool isLoose;
    bool isMedium;
    bool isTight;

    bool isLoosePP;
    bool isMediumPP;
    bool isTightPP;

    unsigned int bits; // Loose/Medium/Tight
    unsigned int bits_Loose; // LoosePP
    unsigned int bits_Medium; // MediumPP
    unsigned int bits_Tight; // TightPP

    typedef std::map<TString, bool IsEM::*> name_pIsEM_map_t;

    static const name_pIsEM_map_t name_pIsEM_map;

    static name_pIsEM_map_t create_map();

 };

  struct Trigger {

    typedef std::map<ZeeDEnum::Trigger::Value, bool> TriggerMap;
    TriggerMap isMatchedToChain;

 };
 struct IdentificationSF {

    IdentificationSF() :
            LoosePP(1), MediumPP(1), TightPP(1), NewFwdLoose(1), NewFwdMedium(1), NewFwdTight(1) {
    }

    double LoosePP;
    double MediumPP;
    double TightPP;

    double NewFwdLoose;
    double NewFwdMedium;
    double NewFwdTight;

 };

 struct IsolationSF {

    IsolationSF() :
            Track(1), Both(1) {
    }

    double Track;
    double Both;

 };

 struct TriggerSF {

    //TriggerSF() : TrigSF(TriggerSFMap(ZeeDEnum::TriggerSF::N, -1)) {}
    TriggerSF() : TrigSF(TriggerSFMap(ZeeDEnum::Trigger::N, -1)), SFTrig(1.0), TrigEff(1.0) {}

    // typedef std::map<ZeeDEnum::TriggerSF::Value, double> TriggerSFMap;
    typedef std::vector<double> TriggerSFMap;
    TriggerSFMap TrigSF;
	double SFTrig;
	double TrigEff;

 };

 struct ReconstructionSF {

    ReconstructionSF() :
            RecoTrkQual(1) {
    }

    double RecoTrkQual;

 };

}

namespace ZeeDMuonBags {

   struct Properties
    {

        Properties()
                : fauthor(0),fisCaloMuonId(0),fisCombinedMuon(0),fisStandAloneMuon(0),fisSegmentTaggedMuon(0),fisSiliconAssociatedForwardMuon(0), fistrack(0)
            {}
        int                 fauthor;
        int                 fisCaloMuonId;
        int                 fisCombinedMuon;
        int                 fisStandAloneMuon;
        int                 fisSegmentTaggedMuon;
        int                 fisSiliconAssociatedForwardMuon;
        bool                fistrack;
    };

    struct CombTrack
    {
        CombTrack()
                : fcombinedMuonTrackParticle(),fnprecisionLayers(0),fnprecisionHoleLayers(0),fmomentumBalanceSignificance(0),ftracktheta(0),ftrackphi(0),ftrackpt(0),ftracketa(0),fd0_exPV(0),fsigd0_exPV(0),fz0_exPV(0),fsigz0_exPV(0),fphi_exPV(0),ftheta_exPV(0),fqoverp_exPV(0)
            {}
       
        TLorentzVector      fcombinedMuonTrackParticle;
        int                 fnprecisionLayers;
        int                 fnprecisionHoleLayers;
        float               fmomentumBalanceSignificance;
        float               ftracktheta;
        float               ftrackphi;
        float               ftrackpt;
        float               ftracketa;
        float               fd0_exPV;
        float               fsigd0_exPV;
        float               fz0_exPV;
        float               fsigz0_exPV;
        float               fphi_exPV;
        float               ftheta_exPV;
        float               fqoverp_exPV;

    };

    struct Isolation
    {
        Isolation() : fptcone20(0), fptcone30(0), fptcone40(0)
            {}
        float               fptcone20;
        float               fptcone30;
        float               fptcone40;
    };

    struct MStrack
    {
        MStrack()
                : fhasMuonSpectrometerTrack(0), fmstracktheta(0), fmstrackphi(0), fmstrackpt(0), fmstracketa(0), fmsd0_exPV(0),
                  fmssigd0_exPV(0), fmsz0_exPV(0), fmssigz0_exPV(0), fmsphi_exPV(0), fmstheta_exPV(0), fmsqoverp_exPV(0)
            {}

        bool                fhasMuonSpectrometerTrack;
        float               fmstracktheta;
        float               fmstrackphi;
        float               fmstrackpt;
        float               fmstracketa;
        float               fmsd0_exPV;
        float               fmssigd0_exPV;
        float               fmsz0_exPV;
        float               fmssigz0_exPV;
        float               fmsphi_exPV;
        float               fmstheta_exPV;
        float               fmsqoverp_exPV;
    };

    struct IDtrack
    {
        IDtrack()
                : fhasInnerDetectorTrack(0),fidtracktheta(0),fidtrackphi(0),fidtrackpt(0),fidtracketa(0),fidd0_exPV(0),fidsigd0_exPV(0),
                  fidz0_exPV(0),fidsigz0_exPV(0),fidphi_exPV(0),fidtheta_exPV(0),fidqoverp_exPV(0), fCaloEnergyLoss(0)

            {}

        bool                fhasInnerDetectorTrack;
        float               fidtracktheta;
        float               fidtrackphi;
        float               fidtrackpt;
        float               fidtracketa;
        float               fidd0_exPV;
        float               fidsigd0_exPV;
        float               fidz0_exPV;
        float               fidsigz0_exPV;
        float               fidphi_exPV;
        float               fidtheta_exPV;
        float               fidqoverp_exPV;
        float               fCaloEnergyLoss,;

    };

	struct CorMomentum
	{
		CorMomentum()
			: fMSPtCor(0), fIDPtCor(0)
		{}

		float fMSPtCor;
		float fIDPtCor;
	};

}

namespace ZeeDElectronBags {
struct Author {

    Author() :
            isUnknown(0), isElectron(0), isSofte(0), isPhoton(0), isFrwd(0), isRConv(0), bits(0) {
    }

    bool isUnknown;
    bool isElectron;
    bool isSofte;
    bool isPhoton;
    bool isFrwd;
    bool isRConv;

    int bits;

 };


struct Cluster {

    Cluster() :
            etaBE2(-1), energy(-1), CentralLambda(0), SecondLambda(0), SecondR(0), longitudinal(0), lateral(0), FracMax(0), eT(-1) {
    }

    double etaBE2;
    double energy;

    double CentralLambda;
    double SecondLambda;
    double SecondR;
    double longitudinal;
    double lateral;
    double FracMax;

    double eT;

    float rawcl_Es0;
    float rawcl_Es1;
    float rawcl_Es2;
    float rawcl_Es3;
    float cl_etaCalo;
    float cl_phiCalo;


    TLorentzVector fourVector;
    TLorentzVector originalFourVector;

    friend std::ostream& operator<<(std::ostream& os, const ZeeDElectronBags::Cluster& c);

};

std::ostream& operator<<(std::ostream& os, const ZeeDElectronBags::Cluster& c);

struct Shower {

    Shower() :
            f1(0), f1core(0), e2tsts1(0), emins1(0), weta1(0), weta2(0), wtots1(0), fracs1(0), e233(0), e237(0), e277(0), widths2(0), f3core(0), etcone15(0), etcone20(0), etcone25(0), etcone30(0), etcone35(
                    0), etcone40(0), etap(0), ptcone20(0), ptcone30(0), ptcone40(0) {
    }

    double f1;
    double f1core;
    double e2tsts1;
    double emins1;
    double weta1;
    double weta2;
    double wtots1;
    double fracs1;
    double e233;
    double e237;
    double e277;
    double widths2;
    double f3core;
    double etcone15;
    double etcone20;
    double etcone25;
    double etcone30;
    double etcone35;
    double etcone40;
    double etap;
    double ptcone20;
    double ptcone30;
    double ptcone40;

    double Reta2() const {
        return this->e237 / this->e277;
    }

};

struct CaloIsoCor {

double etcone20_PtNPVCorrected;
double etcone30_PtNPVCorrected;

};

struct TrackMatch {

    TrackMatch() :
            deltaEta1(0), deltaEta2(0), deltaPhi2(0), EoverP(0), trackd0_physics(0), trackd0_pv(0), expectHitInBLayer(0) {
    }

    double deltaEta1;
    double deltaEta2;
    double deltaPhi2;
    double EoverP;
    double trackd0_physics;
    double trackd0_pv;

    bool expectHitInBLayer;

};


// true if electron passed the cut
struct IDCut {

    bool ClusterStripsWtot;
    bool ClusterMiddleEratio37; // REta
    bool ClusterHadronicLeakage;
    bool TrackTRTratio;
    bool ConversionMatch;

    friend std::ostream& operator<<(std::ostream& os, const ZeeDElectronBags::IDCut& i);

};

std::ostream& operator<<(std::ostream& os, const ZeeDElectronBags::IDCut& i);

struct CaloIso {

    CaloIso() :
            CaloIso95(false), CaloIso98(false) {
    }

    bool CaloIso95;
    bool CaloIso98;

};

struct EIso {

    EIso() :
            Iso95Etcone20(false), Iso98Etcone20(false), Iso95Etcone30(false), Iso98Etcone30(false), Iso97Ptcone20(false), Iso98Ptcone20(false), Iso99Ptcone20(false), Iso97Ptcone30(false), Iso98Ptcone30(false), Iso99Ptcone30(false), Iso97Ptcone40(false), Iso98Ptcone40(false), Iso99Ptcone40(false) {
    }

    bool Iso95Etcone20;
    bool Iso98Etcone20;

    bool Iso95Etcone30;
    bool Iso98Etcone30;

    bool Iso97Ptcone20;
    bool Iso98Ptcone20;
    bool Iso99Ptcone20;

    bool Iso97Ptcone30;
    bool Iso98Ptcone30;
    bool Iso99Ptcone30;

    bool Iso97Ptcone40;
    bool Iso98Ptcone40;
    bool Iso99Ptcone40;

};




} // end of namespace ZeeDElectronBags

namespace ZeeDEventBags {

struct Trigger {

    typedef std::map<ZeeDEnum::Trigger::Value, bool> TriggerMap;
    TriggerMap hasPassedChain;

};

struct MCSyst {

    MCSyst() {
        std::fill(MCSystW, MCSystW + 1000, 0);
    }
    double MCSystW[1000];

};

} // end of namespace ZeeDEventBags

namespace ZeeDEventGenBags {

struct PDF {

    PDF() :
            id1(0), id2(0), x1(0), x2(0), scalePDF(0), pdf1(0), pdf2(0) {
    }

    double id1;
    double id2;
    double x1;
    double x2;
    double scalePDF;
    double pdf1;
    double pdf2;

};

struct Vertex {

    Vertex() :
            position(TVector3(0, 0, 0)) {
    }

    /** Vertex 3-vector */
    TVector3 position;

};

} // end of namespace ZeeDEventGenBags

#endif
