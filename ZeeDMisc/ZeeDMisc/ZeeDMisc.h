#ifndef ZEEDMISC_H
#define ZEEDMISC_H

////////////////////////////////////////////////////////
// Name    : ZeeDMisc.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, Sebastian Schmitt
//

#include <utility>
#include <string>
// Macros
#define DO_CHECK_NULL_PTR 1;

/** Inlclude iostream libraries for debugging info */
#ifdef USE_DEBUG_MESSAGES
#include <iostream>
#endif

#ifdef DEBUG_ON
#include <iostream>
#endif

/** Debug message anouncing the start of the funciton */
#ifdef USE_DEBUG_MESSAGES
#define DEBUG_MES_START std::cout<< __FILE__  << ":" << __LINE__  \
  << ": Start execution of " << __PRETTY_FUNCTION__ << "(...)" \
<< std::endl;
#else
#define DEBUG_MES_START
#endif

/** Debug message anouncing the end of the funciton */
#ifdef USE_DEBUG_MESSAGES
#define DEBUG_MES_END std::cout<< __FILE__  << ":" << __LINE__  \
  << ":   End execution of " << __PRETTY_FUNCTION__ << "(...)" \
<< std::endl;
#else
#define DEBUG_MES_END
#endif

/** Debug message funciton */
#ifdef USE_DEBUG_MESSAGES
#define DEBUG_MES(T) std::cout<< __FILE__  << ":" << __LINE__ << ": " \
  << (T) << " in function " << __PRETTY_FUNCTION__ << "(...)" \
<< std::endl;
#else
#define DEBUG_MES(T)
#endif

/** Debug variables */
#ifdef USE_DEBUG_MESSAGES
#define DEBUG_VAR(T) std::cout << (#T) << " = " << (T) \
  << " in " << __PRETTY_FUNCTION__ \
<< "(...) on line " << __LINE__  \
<< " of " << __FILE__ << std::endl;
#else
#define DEBUG_VAR(T)
#endif


#ifdef DO_CHECK_NULL_PTR
//Not so nice to have these includes in ZeeDMisc.h - are there other options?
#include <TSystem.h> // for gSystem
#include <iostream>  // for std::ceer
#include <cstdlib>   // for EXIT_FAILURE
#define CHECK_NULL_PTR(T) if(T == NULL) {                               \
  std::cerr << __FILE__  << ":" << __LINE__ << ": "                   \
  << "pointer " << #T << " in function " << __PRETTY_FUNCTION__ \
  << " is NULL! Exit with failure..."                       \
  << std::endl;                                             \
  gSystem->Exit(EXIT_FAILURE);                                        \
}
#else
#define CHECK_NULL_PTR(T)
#endif

class TString;
class TVector3;
class TLorentzVector;

namespace CLHEP {

  class HepLorentzVector;

}

namespace HepMC {

  class FourVector;

}

// http://stackoverflow.com/questions/7090130/enum-in-a-namespace/7090276#7090276

namespace ZeeDEnum {

  struct IsEM_t {enum Value {MediumPP, TightPP, LoosePP};};

  struct IS {enum Value {uubar, ddbar, ssbar, ccbar, bbbar, glue};};

  struct Topology {enum Value { CC, CF, FF };};

  struct Direction {enum Value { Central, Forward };};

  struct CosThStar {enum Value { ZFlightDirection, CollinsSoper, BeamDirection, N, Default=CollinsSoper};}; // Default must come after N

  struct MCFSRLevel { enum Value { Born, Bare, Dressed, Default = Born}; };
  
  struct AnalysisType{ enum Value { Zee, Zmumu, Wmunu, Wenu}; };

  struct PID { enum Value {electron = 11, enu=12, muon=13, emu = 14, gamma = 22}; };

  struct InputType {enum Value {UNKNOWN=-1, Data=0, MC, Inline}; };

  struct Period {enum Value {UNKNOWN,
    MC,
    data11_7TeV,
    data11_7TeV_A,
    data11_7TeV_B,
    data11_7TeV_D,
    data11_7TeV_E,
    data11_7TeV_F,
    data11_7TeV_G,
    data11_7TeV_H,
    data11_7TeV_I,
    data11_7TeV_J,
    data11_7TeV_K,
    data11_7TeV_L,
    data11_7TeV_M,
    data12_8TeV,
    data12_8TeV_A,
    data12_8TeV_B,
    data12_8TeV_C,
    data12_8TeV_D,
    data12_8TeV_E,
    data12_8TeV_G,
    data12_8TeV_H,
    data12_8TeV_I,
    data12_8TeV_J,
    data12_8TeV_L,
    data13_2p76TeV,
    N
  };

  static const char* ToString(ZeeDEnum::Period::Value period) {

    switch (period) {

      case data11_7TeV:   return "data11_7TeV";
      case data11_7TeV_A: return "data11_7TeV_A";
      case data11_7TeV_B: return "data11_7TeV_B";
      case data11_7TeV_D: return "data11_7TeV_D";
      case data11_7TeV_E: return "data11_7TeV_E";
      case data11_7TeV_F: return "data11_7TeV_F";
      case data11_7TeV_G: return "data11_7TeV_G";
      case data11_7TeV_H: return "data11_7TeV_H";
      case data11_7TeV_I: return "data11_7TeV_I";
      case data11_7TeV_J: return "data11_7TeV_J";
      case data11_7TeV_K: return "data11_7TeV_K";
      case data11_7TeV_L: return "data11_7TeV_L";
      case data11_7TeV_M: return "data11_7TeV_M";

      case data12_8TeV:   return "data12_8TeV";
      case data12_8TeV_A: return "data12_8TeV_A";
      case data12_8TeV_B: return "data12_8TeV_B";
      case data12_8TeV_C: return "data12_8TeV_C";
      case data12_8TeV_D: return "data12_8TeV_D";
      case data12_8TeV_E: return "data12_8TeV_E";
      case data12_8TeV_G: return "data12_8TeV_G";
      case data12_8TeV_H: return "data12_8TeV_H";
      case data12_8TeV_I: return "data12_8TeV_I";
      case data12_8TeV_J: return "data12_8TeV_J";
      case data12_8TeV_L: return "data12_8TeV_L";
      case data13_2p76TeV:return "data13_2p76TeV";

      default: return "UNKNOWN";

    }

  }

  };

  struct Trigger {
    // Caution:	if you add a new trigger
    //		do not forget to implement
    //		Type (DiElec/Single), ToString, etc.
    enum Value {
      UNKNOWN,
           EF_e5_loose1,
      EF_e15vh_medium1,
      EF_e7_loose1,
      EF_e9_tight1,
      EF_e15_loose0,
      EF_e15_loose1,
      EF_e10_medium1,
      EF_e10_medium,
      EF_e10_loose1,
      EF_e15_medium,
      EF_e20_medium,
      EF_e22_medium,
      EF_e22vh_medium1,
      EF_e24vhi_medium1,
      EF_2g20_loose,
      EF_2e12_medium,
      EF_2e12T_medium,
      EF_2e12Tvh_medium,
      EF_2e12Tvh_loose1,
      EF_2e12Tvh_loose1_L2StarB,
      EF_e60_medium1,
      EF_e12Tvh_medium1_mu8,
      EF_xe20_FEB_L1EM5,
      EF_mu10,
      EF_mu8,
      EF_mu6,
      EF_mu2,
      EF_mu24i_tight,
      N,
      DefaultSingleElec,
      DefaultDiElec
    };

    enum Type {
      SingleElec,
      DiElec,
      SingleMu
    };

    static Type GetType(Value trigger) {

      switch (trigger) {

        case EF_e15vh_medium1:  return SingleElec;
        case EF_e5_loose1:      return SingleElec;
        case EF_e9_tight1:      return SingleElec;
        case EF_e15_loose0:     return SingleElec;
        case EF_e15_loose1:     return SingleElec;
        case EF_e10_medium1:    return SingleElec;
	case EF_e10_medium:	return SingleElec;
        case EF_e10_loose1:     return SingleElec;
	case EF_e15_medium:	return SingleElec;
	case EF_e20_medium:	return SingleElec;
	case EF_e22_medium:	return SingleElec;
	case EF_e22vh_medium1:	return SingleElec;
	case EF_e24vhi_medium1:	return SingleElec;
	case EF_e60_medium1:	return SingleElec;
        case EF_e7_loose1:      return SingleElec;
        //case EF_e12Tvh_medium1_mu8 return SingleElec;
        case EF_xe20_FEB_L1EM5: return SingleElec;

	case EF_2g20_loose:	return DiElec;
	case EF_2e12_medium:	return DiElec;
	case EF_2e12T_medium:	return DiElec;
	case EF_2e12Tvh_medium:	return DiElec;
	case EF_2e12Tvh_loose1:	return DiElec;
	case EF_2e12Tvh_loose1_L2StarB:	return DiElec;
	case EF_e12Tvh_medium1_mu8:	return DiElec;
       
        case EF_mu10:           return SingleMu;
        case EF_mu8:            return SingleMu;
        case EF_mu6:            return SingleMu;
        case EF_mu2:            return SingleMu;
        case EF_mu24i_tight:    return SingleElec;

	case DefaultSingleElec:	return SingleElec;
	case DefaultDiElec: 	return DiElec;

	default: return DiElec;

      }

    }


    static const char* ToString(ZeeDEnum::Trigger::Value chain) {

      switch (chain) {
        
        case EF_e15vh_medium1:  return "EF_e15vh_medium1";
        case EF_e5_loose1:      return "EF_e5_loose1";
        case EF_e7_loose1:      return "EF_e7_loose1";
        case EF_e9_tight1:      return "EF_e9_tight1";
        case EF_e15_loose0:     return "EF_e15_loose0";
        case EF_e15_loose1:     return "EF_e15_loose1";
        case EF_e10_medium1:    return "EF_e10_medium1";
	case EF_e10_medium:	return "EF_e10_medium";
        case EF_e10_loose1:     return "EF_e10_loose1";
	case EF_e15_medium:	return "EF_e15_medium";
	case EF_e20_medium:	return "EF_e20_medium";
	case EF_e22_medium:	return "EF_e22_medium";
	case EF_e22vh_medium1:	return "EF_e22vh_medium1";
	case EF_e24vhi_medium1:	return "EF_e24vhi_medium1";
	case EF_e60_medium1:	return "EF_e60_medium1";
        case EF_xe20_FEB_L1EM5: return "EF_xe20_FEB_L1EM5";

	case EF_2g20_loose:	return "EF_2g20_loose";
	case EF_2e12_medium:	return "EF_2e12_medium";
	case EF_2e12T_medium:	return "EF_2e12T_medium";
	case EF_2e12Tvh_medium:	return "EF_2e12Tvh_medium";
	case EF_2e12Tvh_loose1:	return "EF_2e12Tvh_loose1";
	case EF_2e12Tvh_loose1_L2StarB:	return "EF_2e12Tvh_loose1_L2StarB";
	case EF_e12Tvh_medium1_mu8:	return "EF_e12Tvh_medium1_mu8";

        case EF_mu10:           return "EF_mu10";
        case EF_mu8:            return "EF_mu8";
        case EF_mu6:            return "EF_mu6";
        case EF_mu2:            return "EF_mu2";
        case EF_mu24i_tight:    return "EF_mu24i_tight";

	case DefaultSingleElec:	return "DefaultSingleElec";
	case DefaultDiElec: 	return "DefaultDiElec";

	default: return "UNKNOWN";

      }

    }

    static Value ToEnum(const std::string& chain) {

      return ToEnum(chain.c_str());

    }

    static Value ToEnum(const char* chain) {
      //chain = "EF_mu24i_tight"; 
      //std::cout <<" i'm here " << chain << std::endl;
      if(strcmp(chain,"EF_e10_medium") == 0)
	return EF_e10_medium;
      else if(strcmp(chain,"EF_mu24i_tight") == 0){
        //std::cout << std::endl;
        return EF_mu24i_tight;
        }
      else if(strcmp(chain,"EF_e15vh_medium1") == 0)
        return EF_e15vh_medium1;
      else if(strcmp(chain,"EF_e5_loose1" ) == 0)
         return EF_e5_loose1;
      else if(strcmp(chain,"EF_e7_loose1" ) == 0)
         return EF_e7_loose1;
      else if(strcmp(chain, "EF_e9_tight1") == 0)
         return EF_e9_tight1;
      else if(strcmp(chain, "EF_e15_loose0") == 0)
         return EF_e15_loose0;
      else if(strcmp(chain, "EF_e15_loose1") == 0)
         return EF_e15_loose1;
      else if(strcmp(chain, "EF_e10_medium1") == 0)
         return EF_e10_medium1;
      else if(strcmp(chain, "EF_e10_loose1") == 0)
         return EF_e10_loose1;
      else if(strcmp(chain,"EF_e15_medium") == 0)
	return EF_e15_medium;
      else if(strcmp(chain,"EF_e20_medium") == 0)
	return EF_e20_medium;
      else if(strcmp(chain,"EF_e22_medium") == 0)
	return EF_e22_medium;
      else if(strcmp(chain,"EF_e22vh_medium1") == 0)
	return EF_e22vh_medium1;
      else if(strcmp(chain,"EF_e24vhi_medium1") == 0)
	return EF_e24vhi_medium1;
      else if(strcmp(chain,"EF_e60_medium1") == 0)
	return EF_e60_medium1;
      else if(strcmp(chain, "EF_xe20_FEB_L1EM5") == 0)
        return  EF_xe20_FEB_L1EM5;
      else if(strcmp(chain,"EF_2g20_loose") == 0)
	return EF_2g20_loose;
      else if(strcmp(chain,"EF_2e12_medium") == 0)
	return EF_2e12_medium;
      else if(strcmp(chain,"EF_2e12T_medium") == 0)
	return EF_2e12T_medium;
      else if(strcmp(chain,"EF_2e12Tvh_medium") == 0)
	return EF_2e12Tvh_medium;
      else if(strcmp(chain,"EF_2e12Tvh_loose1") == 0)
	return EF_2e12Tvh_loose1;
      else if(strcmp(chain,"EF_e12Tvh_medium1_mu8") == 0)
	return EF_e12Tvh_medium1_mu8;
      else if(strcmp(chain,"EF_2e12Tvh_loose1_L2StarB") == 0)
	return EF_2e12Tvh_loose1_L2StarB;
      else if(strcmp(chain,"EF_mu10") == 0)
        return EF_mu10;
      else if(strcmp(chain,"EF_mu8") == 0)
        return EF_mu8;
      else if(strcmp(chain,"EF_mu6") == 0)
        return EF_mu6;
      else if(strcmp(chain,"EF_mu2") == 0)
        return EF_mu2;
      else if(strcmp(chain,"DefaultSingleElec") == 0)
	return DefaultSingleElec;
      else if(strcmp(chain,"DefaultDiElec") == 0)
	return DefaultDiElec;

      else {
	return UNKNOWN;
      }

    }

  };


  struct MCtype {enum Value {UNKNOWN,
    MCNLO,
    PowhegPythia,
    PowhegHerwig,
    AlpgenHerwig,	
    AlpgenPythia,	     
    Herwig,
    Pythia,
    PowhegPythia8,
    Sherpa,
    N};



    static const char* ToString(ZeeDEnum::MCtype::Value mctype) {

      switch (mctype) {

	case PowhegPythia:      	return "PowhegPythia";
	case PowhegPythia8:     	return "PowhegPythia8";
	case MCNLO:	        	return "MCNLO";
	case PowhegHerwig:      	return "PowhegHerwig";
	case AlpgenHerwig:      	return "AlpgenHerwig";
	case AlpgenPythia:	        return "AlpgenPythia";
        case Herwig:	        	return "Herwig";
	case Pythia:	        	return "Pythia";
	case Sherpa:	        	return "Sherpa";

	default: return "UNKNOWN";

      }

    }

    static Value ToEnum(const std::string& mctype) {

      return ToEnum(mctype.c_str());

    }

    static Value ToEnum(const char* mctype) {

      if(strcmp(mctype, "MCNLO") == 0)
	return MCNLO;
      else if(strcmp(mctype, "PowhegPythia") == 0)
	return PowhegPythia;
      else if(strcmp(mctype, "PowhegHerwig") == 0)
	return PowhegHerwig;
      else if(strcmp(mctype, "PowhegHerwig") == 0)
	return PowhegHerwig;
      else if(strcmp(mctype, "AlpgenHerwig") == 0)
	return AlpgenHerwig;
      else if(strcmp(mctype, "AlpgenPythia") == 0)
	return AlpgenPythia;
      else if(strcmp(mctype, "Herwig") == 0)
	return Herwig;
      else if(strcmp(mctype, "Pythia") == 0)
	return Pythia;
      else if(strcmp(mctype, "PowhegPythia8") == 0)
	return PowhegPythia8;
      else if(strcmp(mctype, "Sherpa") == 0)
	return Sherpa;

      else {
	return UNKNOWN;
      }

    }



  };

}

/*!
  \file ZeeDMisc.h

  Misc collection

  \author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt
  \date   Wed May 26 16:34:37 CEST 2010
 */
class ZeeDMisc {

  public:
    /**  Rad -> Deg conversion factor */
    static const double Rad2Deg;

    /** Electron mass */
    static const double Mass_e;

    /** Z boson mass */
    static const double Mass_Z;

    /** W boson mass */
    static const double Mass_W;

    static void Dump3Vec(const TString& title,
	const TVector3& vec);

    static void Dump4Vec(const TString& title,
	const TLorentzVector& fourVec);

    /** Transforms HepLorentzVector into TLorentzVector */
    static TLorentzVector FormTLorentzVec(const CLHEP::HepLorentzVector& hepVector);

    /** Get period from run number

      if run number is unknown the string returned is empty

      @param - runNumber unique run number like 165817, 166850, ...
      @return (sub)period like E1, G6, ...

     */
    static ZeeDEnum::Period::Value GetPeriod(unsigned int runNumber);

    /** Return first and last run number of period **/
    static std::pair<int, int> GetPeriodStartEnd(ZeeDEnum::Period::Value period);

    /**

      if period is "MC" the default for Monte Carlo is returned

      if period is unknown the string returned is empty

     */
    static ZeeDEnum::Trigger::Value GetDefaultSingleElectronTrigger(ZeeDEnum::Period::Value period);

    /**

      if run number is unknown the string returned is empty

     */
    static ZeeDEnum::Trigger::Value GetDefaultSingleElectronTrigger(unsigned int runNumber);

    static ZeeDEnum::Trigger::Value GetDefaultDiElectronTrigger(ZeeDEnum::Period::Value period);
    static ZeeDEnum::Trigger::Value GetDefaultDiElectronTrigger(unsigned int runNumber);

    static TString FindFile(TString filename);

    /**

      get MCtype (Pythia, MCNLO, etc.) from MC datasetID stored in run number EventInfo

     */
    static ZeeDEnum::MCtype::Value MCtype(unsigned int datasetID);

    static ZeeDEnum::InputType::Value strToInputType(const TString& Inputtype);
    static TString inputTypeToStr(const ZeeDEnum::InputType::Value inputType);

    static bool IsAlpgen(int );
    static bool IsSherpa(int );
    static bool IsAlpgenHerwig(int );
    static double AlpgenWeight(int);

  private:
    ZeeDMisc();
    ~ZeeDMisc();

};

// prefer ROOT's R__FOR_EACH macro

#if 0 // commenting ForEach
#include <TObjArray.h> // maybe move ForEach to own file

namespace ZeeD {

  /** TObjArray looping helper */

  template<class T, typename W>
    void ForEach(TObjArray* array,  W (T::*ptr)() )
    {

      for (Int_t index = 0; index < array->GetEntriesFast(); ++index) {

	T* obj = static_cast< T* > ( array->At(index));
	(obj->*ptr)();

      }

    }

  template<class T, typename W, typename V>
    void ForEach(TObjArray* array,  W (T::*ptr)(V), V arg)
    {

      for (Int_t index = 0; index < array->GetEntriesFast(); ++index) {

	T* obj = static_cast< T* > ( array->At(index));
	(obj->*ptr)(arg);

      }

    }

  template<class T, typename W, typename V1, typename V2>
    void ForEach(TObjArray* array,  W (T::*ptr)(V1, V2), V1 arg1, V2 arg2)
    {

      for (Int_t index = 0; index < array->GetEntriesFast(); ++index) {

	T* obj = static_cast< T* > ( array->At(index));
	(obj->*ptr)(arg1, arg2);

      }

    }

}
#endif // end commenting ForEach

class ZeeDGenParticle;

typedef std::pair<ZeeDGenParticle*, ZeeDGenParticle*>  ZeeDGenElectrons;

#endif // ZEEDMISC
