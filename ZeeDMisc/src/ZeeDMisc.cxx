#include "ZeeDMisc/ZeeDMisc.h"

#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "PathResolver/PathResolver.h"

#include "CLHEP/Vector/LorentzVector.h"
#include "HepMC/SimpleVector.h"


const double ZeeDMisc::Rad2Deg = 57.29577951308232;
const double ZeeDMisc::Mass_e  = 0.000510998910; // PDG09: M_e = 0.000510998910 +/- ..13 GeV
const double ZeeDMisc::Mass_Z  = 91.1876;        // PDG09: M_Z = 91.1876 +/- 0.0021 GeV
const double ZeeDMisc::Mass_W  = 80.398;         // PDG09: M_W = 80.398 +/- 0.025 GeV
/*
const TString ZeeDMisc::NoteCuts =
    "ExistZ+"
    "PtMinBothElecZ+"
    "EtaMaxBothElecZ+"
    "EtaCrackBothElecZ+"
    "ZMassZ+"
    "TrackBothElecZ+"
    "ChargeBothElecOppositeZ+"
    "IsEMMediumBothElecZ+"
    "PriVtxZ+"
    "MinNVertex+"
    "OQMaps+"
    "TrigE10medOneElecZ";
*/
//------------------------------------------------------
void ZeeDMisc::Dump3Vec(const TString& title,
                        const TVector3& vec)
{

    std::cout << title << " (x, y, z): (";
    std::cout << vec.X() << ",";
    std::cout << vec.Y() << ",";
    std::cout << vec.Z() << ")";
    std::cout << std::endl;

}

//------------------------------------------------------
void ZeeDMisc::Dump4Vec(const TString& title,
                        const TLorentzVector& fourVec)
{

    std::cout << title << " (E, Px, Py, Pz): (";
    std::cout << fourVec.E() << ",";
    std::cout << fourVec.Px() << ",";
    std::cout << fourVec.Py() << ",";
    std::cout << fourVec.Pz() << ")";

    std::cout << std::endl;

    std::cout << title << " (M, Eta, Pt): (";
    std::cout << fourVec.M() << ",";
    std::cout << fourVec.Eta() << ",";
    std::cout << fourVec.Pt() << ")";

    std::cout << std::endl;

}

//------------------------------------------------------
TLorentzVector ZeeDMisc::FormTLorentzVec(const CLHEP::HepLorentzVector& hepVector)
{

    double E  = hepVector.e() ;
    double Px = hepVector.px();
    double Py = hepVector.py();
    double Pz = hepVector.pz();

    return TLorentzVector(Px, Py, Pz, E);

}

//------------------------------------------------------
ZeeDEnum::Period::Value ZeeDMisc::GetPeriod(unsigned int runNumber)
{

  if(runNumber >= 177531 && runNumber <= 177965) { // data11, https://atlddm10.cern.ch/tagservices/RunBrowser/runBrowserReport/rBR_Period_Report.php

    return ZeeDEnum::Period::data11_7TeV_A;

    } else if(runNumber >= 177986 && runNumber <= 178109) {

        return ZeeDEnum::Period::data11_7TeV_B;

    } else if(runNumber >= 179710 && runNumber <= 180481) {

        return ZeeDEnum::Period::data11_7TeV_D;

    } else if(runNumber >= 180614 && runNumber <= 180776) {

        return ZeeDEnum::Period::data11_7TeV_E;

    } else if(runNumber >= 182013 && runNumber <= 182519) {

        return ZeeDEnum::Period::data11_7TeV_F;

    } else if(runNumber >= 182726 && runNumber <= 183462) {

        return ZeeDEnum::Period::data11_7TeV_G;

    } else if(runNumber >= 183544 && runNumber <= 184169) {

        return ZeeDEnum::Period::data11_7TeV_H;

    } else if(runNumber >= 185353 && runNumber <= 186493) {

        return ZeeDEnum::Period::data11_7TeV_I;

    } else if(runNumber >= 186516 && runNumber <= 186755) {

        return ZeeDEnum::Period::data11_7TeV_J;

    } else if(runNumber >= 186873 && runNumber <= 187815) {

        return ZeeDEnum::Period::data11_7TeV_K;

    } else if(runNumber >= 188902 && runNumber <= 190343) {

        return ZeeDEnum::Period::data11_7TeV_L;

    } else if(runNumber >= 190503 && runNumber <= 191933) {

        return ZeeDEnum::Period::data11_7TeV_M;

  } else if(runNumber > 191933 && runNumber < 200804) {

    //   return ZeeDEnum::Period::data11_7TeV;  //      P.S. very questionable !
    return ZeeDEnum::Period::data12_8TeV;  //      jsut a try for correct trigger lookup....  should assign run number in MC12 do data12

    } else if (runNumber >= 200804 && runNumber <= 201556){

      return ZeeDEnum::Period::data12_8TeV_A;

    } else if (runNumber >= 202660 && runNumber <= 205113){

      return ZeeDEnum::Period::data12_8TeV_B;

    } else if (runNumber >= 206248 && runNumber <= 207397){

      return ZeeDEnum::Period::data12_8TeV_C;

    } else if (runNumber >= 207447 && runNumber <= 209025){

      return ZeeDEnum::Period::data12_8TeV_D;

    } else if (runNumber >= 209074 && runNumber <= 210308){

      return ZeeDEnum::Period::data12_8TeV_E;

    } else if (runNumber >= 211522 && runNumber <= 212272){

      return ZeeDEnum::Period::data12_8TeV_G;

    } else if (runNumber >= 212619 && runNumber <= 213359){

      return ZeeDEnum::Period::data12_8TeV_H;

    } else if (runNumber >= 213431 && runNumber <= 213819){

      return ZeeDEnum::Period::data12_8TeV_I;

    } else if (runNumber >= 213900 && runNumber <= 215091){

      return ZeeDEnum::Period::data12_8TeV_J;

    } else if (runNumber >= 215414 && runNumber <= 215643){

      return ZeeDEnum::Period::data12_8TeV_L;

    } else if (runNumber > 215643 && runNumber < 219171){

      return ZeeDEnum::Period::data12_8TeV;

    } else if (runNumber >= 219171) {

      return ZeeDEnum::Period::data13_2p76TeV;

    }  else if(runNumber == 1) { // special case of InlineZ

        return ZeeDEnum::Period::data11_7TeV;

    } else {

        Error("ZeeDMisc::GetPeriod", "No period found for run number %d", runNumber);

        gSystem->Exit(EXIT_FAILURE);

    }

    return ZeeDEnum::Period::UNKNOWN;

}

//------------------------------------------------------
std::pair<int, int> ZeeDMisc::GetPeriodStartEnd(ZeeDEnum::Period::Value period) {

    switch (period) {

    case ZeeDEnum::Period::data11_7TeV_B: return std::make_pair(177986,178109); //period B 2011
    case ZeeDEnum::Period::data11_7TeV_D: return std::make_pair(179710,180481); //period D 2011
    case ZeeDEnum::Period::data11_7TeV_E: return std::make_pair(180614,180776); //period E 2011
    case ZeeDEnum::Period::data11_7TeV_F: return std::make_pair(182013,182519); //period F 2011
    case ZeeDEnum::Period::data11_7TeV_G: return std::make_pair(182726,183462); //period G 2011
    case ZeeDEnum::Period::data11_7TeV_H: return std::make_pair(183544,184169); //period H 2011
    case ZeeDEnum::Period::data11_7TeV_I: return std::make_pair(185353,186493); //period I 2011
    case ZeeDEnum::Period::data11_7TeV_J: return std::make_pair(186516,186755); //period J 2011
    case ZeeDEnum::Period::data11_7TeV_K: return std::make_pair(186873,187815); //period K 2011
    case ZeeDEnum::Period::data11_7TeV_L: return std::make_pair(188902,190343); //period L 2011
    case ZeeDEnum::Period::data11_7TeV_M: return std::make_pair(190503,191933); //period M 2011
    case ZeeDEnum::Period::data12_8TeV_A: return std::make_pair(200804,201556); //period A 2012
    case ZeeDEnum::Period::data12_8TeV_B: return std::make_pair(202660,205113); //period B 2012
    case ZeeDEnum::Period::data12_8TeV_C: return std::make_pair(206248,207397); //period C 2012
    case ZeeDEnum::Period::data12_8TeV_D: return std::make_pair(207447,209025); //period D 2012
    case ZeeDEnum::Period::data12_8TeV_E: return std::make_pair(209074,210308); //period E 2012
    case ZeeDEnum::Period::data12_8TeV_G: return std::make_pair(211522,212272); //period G 2012
    case ZeeDEnum::Period::data12_8TeV_H: return std::make_pair(212619,213359); //period H 2012
    case ZeeDEnum::Period::data12_8TeV_I: return std::make_pair(213431,213819); //period I 2012
    case ZeeDEnum::Period::data12_8TeV_J: return std::make_pair(213900,215091); //period J 2012
    case ZeeDEnum::Period::data12_8TeV_L: return std::make_pair(215414,215643); //period L 2012
    case ZeeDEnum::Period::data13_2p76TeV: return std::make_pair(219171,219370);


    default:

        Error("ZeeDMisc::GetStartEndPeriod", "Could not find period " +TString(ZeeDEnum::Period::ToString(period)) + "!");

        gSystem->Exit(EXIT_FAILURE);

    }

    return std::make_pair(0,0);

}

//------------------------------------------------------
ZeeDEnum::Trigger::Value ZeeDMisc::GetDefaultSingleElectronTrigger(ZeeDEnum::Period::Value period)
{

    switch (period) {

    case ZeeDEnum::Period::MC:
      // not in use for the moment:
      // for the period in mc the data-like run number is used now
        return ZeeDEnum::Trigger::EF_e20_medium;

    case ZeeDEnum::Period::data11_7TeV_A:
    case ZeeDEnum::Period::data11_7TeV_B:
    case ZeeDEnum::Period::data11_7TeV_D:
    case ZeeDEnum::Period::data11_7TeV_E:
    case ZeeDEnum::Period::data11_7TeV_F:
    case ZeeDEnum::Period::data11_7TeV_G:
    case ZeeDEnum::Period::data11_7TeV_H:
    case ZeeDEnum::Period::data11_7TeV_I:
    case ZeeDEnum::Period::data11_7TeV_J:
    case ZeeDEnum::Period::data11_7TeV:

        return ZeeDEnum::Trigger::EF_e20_medium;

    case ZeeDEnum::Period::data11_7TeV_K:

        return ZeeDEnum::Trigger::EF_e22_medium;

    case ZeeDEnum::Period::data11_7TeV_L:
    case ZeeDEnum::Period::data11_7TeV_M:

        return ZeeDEnum::Trigger::EF_e22vh_medium1;

    case ZeeDEnum::Period::data12_8TeV_A:
    case ZeeDEnum::Period::data12_8TeV_B:
    case ZeeDEnum::Period::data12_8TeV_C:
    case ZeeDEnum::Period::data12_8TeV_D:
    case ZeeDEnum::Period::data12_8TeV_E:
    case ZeeDEnum::Period::data12_8TeV_G:
    case ZeeDEnum::Period::data12_8TeV_H:
    case ZeeDEnum::Period::data12_8TeV_I:
    case ZeeDEnum::Period::data12_8TeV_J:
    case ZeeDEnum::Period::data12_8TeV_L:
    case ZeeDEnum::Period::data12_8TeV:

        return ZeeDEnum::Trigger::EF_e24vhi_medium1; // data12, https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/Egamma2012Analyses

	case ZeeDEnum::Period::data13_2p76TeV:

		return ZeeDEnum::Trigger::EF_e15_loose1;
	
    default:

        Error("ZeeDMisc::GetDefaultSingleElectronTrigger", "No default trigger found for period  " + TString(ZeeDEnum::Period::ToString(period)));

        gSystem->Exit(EXIT_FAILURE);

    }

    return ZeeDEnum::Trigger::UNKNOWN;

}

//------------------------------------------------------
ZeeDEnum::Trigger::Value ZeeDMisc::GetDefaultSingleElectronTrigger(unsigned int runNumber)
{

    const ZeeDEnum::Period::Value period = ZeeDMisc::GetPeriod(runNumber);

    return ZeeDMisc::GetDefaultSingleElectronTrigger(period);

}

//------------------------------------------------------
ZeeDEnum::Trigger::Value ZeeDMisc::GetDefaultDiElectronTrigger(ZeeDEnum::Period::Value period)
{

    switch(period) {

    case ZeeDEnum::Period::MC:

        return ZeeDEnum::Trigger::EF_2e12_medium;

    case ZeeDEnum::Period::data11_7TeV_D:
    case ZeeDEnum::Period::data11_7TeV_E:
    case ZeeDEnum::Period::data11_7TeV_F:
    case ZeeDEnum::Period::data11_7TeV_G:
    case ZeeDEnum::Period::data11_7TeV_H:
    case ZeeDEnum::Period::data11_7TeV_I:
    case ZeeDEnum::Period::data11_7TeV_J:
    case ZeeDEnum::Period::data11_7TeV:

        return ZeeDEnum::Trigger::EF_2e12_medium;

    case ZeeDEnum::Period::data11_7TeV_K:

        return ZeeDEnum::Trigger::EF_2e12T_medium;

    case ZeeDEnum::Period::data11_7TeV_L:
    case ZeeDEnum::Period::data11_7TeV_M:

        return ZeeDEnum::Trigger::EF_2e12Tvh_medium;

    case ZeeDEnum::Period::data12_8TeV_A:
    case ZeeDEnum::Period::data12_8TeV_B:
    case ZeeDEnum::Period::data12_8TeV_C:
    case ZeeDEnum::Period::data12_8TeV_D:
    case ZeeDEnum::Period::data12_8TeV_E:
    case ZeeDEnum::Period::data12_8TeV_G:
    case ZeeDEnum::Period::data12_8TeV_H:
    case ZeeDEnum::Period::data12_8TeV_I:
    case ZeeDEnum::Period::data12_8TeV_J:
    case ZeeDEnum::Period::data12_8TeV_L:
    case ZeeDEnum::Period::data12_8TeV:
    case ZeeDEnum::Period::data13_2p76TeV:

        return ZeeDEnum::Trigger::EF_2e12Tvh_loose1;

    default:

        Error("ZeeDMisc::GetDefaultDiElectronTrigger", "No default trigger found for period " + period);

        gSystem->Exit(EXIT_FAILURE);

    }

    return ZeeDEnum::Trigger::UNKNOWN;

}

//------------------------------------------------------
ZeeDEnum::Trigger::Value ZeeDMisc::GetDefaultDiElectronTrigger(unsigned int runNumber)
{

    const ZeeDEnum::Period::Value period = ZeeDMisc::GetPeriod(runNumber);

    return ZeeDMisc::GetDefaultDiElectronTrigger(period);

}

//------------------------------------------------------
TString ZeeDMisc::FindFile(TString filename)
{

    TString resolvedFilename = PathResolverFindDataFile(filename.Data());

    if(resolvedFilename.Length() == 0) {

        Error("ZeeDMisc::FindFile", "Could not find file with filename: \"" + filename + "\"!");

        gSystem->Exit(EXIT_FAILURE);

    }

    return resolvedFilename;

}

//------------------------------------------------------
ZeeDEnum::MCtype::Value ZeeDMisc::MCtype(unsigned int datasetID) {

    switch(datasetID) {

    case 106080:
    case 106081:
    case 106087:
    case 105200:
    case 126594:
    case 147215:
    case 147216:

        return ZeeDEnum::MCtype::MCNLO;

    case 113900:
    case 113901:
    case 113904:
    case 108303:
    case 129806:
    case 186169:
    case 189751:

        return ZeeDEnum::MCtype::PowhegPythia;


    case 113816:
    case 113184:
    case 126006:
    case 129808:
    case 147219:
    case 147220:

      return ZeeDEnum::MCtype::PowhegHerwig;

      //Ztautau
    case 107670:
    case 107671:
    case 107672:
    case 107673:
    case 107674:
    case 107675:
      
      //Zee
    case 107650:
    case 107651:
    case 107652:
    case 107653:
    case 107654:
    case 107655:
        
      return ZeeDEnum::MCtype::AlpgenHerwig;

      //old sample
    case 117650:
    case 117651:
    case 117652:
    case 117653:
    case 117654:
    case 117655:
      //new sample
    case 147105:
    case 147106:
    case 147107:
    case 147108:
    case 147109:
    case 147110:  
   
      return ZeeDEnum::MCtype::AlpgenPythia;

    case 105985:
    case 105987:
    case 105986:

        return ZeeDEnum::MCtype::Herwig;

    case 106043:
    case 106046:
    case 108320:
    case 105802:
    case 106009:
    case 107054:
    case 106052:

        return ZeeDEnum::MCtype::Pythia;

    case 195847:
    case 195848:
    case 129696:
    case 129685:
    case 129680:
    case 129501:
    case 129521:
    case 147800:
    case 147801:
    case 147802:
    case 147803:
    case 147804:
    case 147805:
    case 147806:
    case 147807:
    case 147808:


      return ZeeDEnum::MCtype::PowhegPythia8; 

    case 147770:

      return ZeeDEnum::MCtype::Sherpa;

    default:

        return ZeeDEnum::MCtype::UNKNOWN;

    }

}

bool ZeeDMisc::IsAlpgen(int dsid){
  if ( ((dsid>=107650) &&(dsid <=107655 ))||((dsid>=107670) &&(dsid <=107675 ))|| ((dsid>=117650) &&(dsid <=117655 )) || ((dsid>=147105) &&(dsid <=147110 ))  )
    return true;
  else
    return false;
}

bool ZeeDMisc::IsSherpa(int dsid){
  if ( dsid == 147770 )
    return true;
  else
    return false;
}

bool ZeeDMisc::IsAlpgenHerwig(int dsid){
  if ( ((dsid>=107650) &&(dsid <=107655 ))||((dsid>=107670) &&(dsid <=107675 ))  )
    return true;
  else
    return false;
}

double ZeeDMisc::AlpgenWeight(int dsid){

  switch(dsid) {

  case 107650:
    return  7.1182e-01/6604283.;
  case 107651:
    return 1.5517e-01/1329994.;
  case 107652:
    return  4.8745e-02/404798.;
  case 107653:
    return 1.4225e-02/109998.;
  case 107654:
    return  3.7595e-03/30000.;
  case 107655:
    return 1.0945e-03/10000.;
  case 117650:
    return  7.1889e-01/6619984.;
  case 117651:
    return 1.7560e-01/1329498.;
  case 117652:
    return  5.8849e-02/404998.;
  case 117653:
    return 1.5560e-02/109999.;
  case 117654:
    return  3.9322e-03/30000.;
  case 117655:
    return 1.1994e-03/10000.;
  case 147105:
    return 7.1897e-01/6300000.;
  case 147106:
    return 1.7570e-01/8200000.;
  case 147107:
    return 5.8875e-02/3176000.;
  case 147108:
    return 1.5636e-02/895000.;
  case 147109:
    return 4.0116e-03/398600.;
  case 147110:
    return 1.2592e-03/229700.;
  default :
    return 1.0;
  }
  return 1.0;
}


ZeeDEnum::InputType::Value ZeeDMisc::strToInputType(const TString& inputType) {
    if (inputType.CompareTo("data", TString::kIgnoreCase) == 0) { // EqualTo does not yet exist in 5.30
    	return ZeeDEnum::InputType::Data;
    } else if (inputType.CompareTo("mc", TString::kIgnoreCase) == 0) {
    	return ZeeDEnum::InputType::MC;
    } else if (inputType.CompareTo("inline", TString::kIgnoreCase) == 0 ||
               inputType.CompareTo("inlinez", TString::kIgnoreCase) == 0 ) {
    	return ZeeDEnum::InputType::Inline;
    } else {
    	Warning("ZeeDMisc::strToInputType","Unknown input type %s. Assuming \"data\".", inputType.Data());
    	return ZeeDEnum::InputType::Data;
    }
}

TString ZeeDMisc::inputTypeToStr(const ZeeDEnum::InputType::Value inputType) {

    switch(inputType) {

    case ZeeDEnum::InputType::Data:   return "data";
    case ZeeDEnum::InputType::MC:     return "mc";
    case ZeeDEnum::InputType::Inline: return "inline";

    default: return "UNKOWN";

    }

}
