#include "ZeeDTools/ZeeDBags.h"

#include <ostream>

namespace ZeeDLeptonBags{
IsEM::IsEM() : isFrwdLoose(0),
               isFrwdTight(0),
               isNewFwdLoose(0),
               isNewFwdMedium(0),
               isNewFwdTight(0),
               isLoose(0),
               isMedium(0),
               isTight(0),
               isLoosePP(0),
               isMediumPP(0),
               isTightPP(0),
               bits(0),
               bits_Loose(0),
               bits_Medium(0),
               bits_Tight(0) {

}

IsEM::name_pIsEM_map_t IsEM::create_map() {

    name_pIsEM_map_t name_pIsEM_map;

    name_pIsEM_map.insert(std::make_pair("Loose", &IsEM::isLoose));
    name_pIsEM_map.insert(std::make_pair("Medium", &IsEM::isMedium));
    name_pIsEM_map.insert(std::make_pair("Tight", &IsEM::isTight));

    name_pIsEM_map.insert(std::make_pair("LoosePP", &IsEM::isLoosePP));
    name_pIsEM_map.insert(std::make_pair("MediumPP", &IsEM::isMediumPP));
    name_pIsEM_map.insert(std::make_pair("TightPP", &IsEM::isTightPP));

    return name_pIsEM_map;

}

const IsEM::name_pIsEM_map_t IsEM::name_pIsEM_map = IsEM::create_map();

}

namespace ZeeDElectronBags {

std::ostream& operator<<(std::ostream& os, const ZeeDElectronBags::Cluster& c)
{

    os << "etaBE2:   " << c.etaBE2 << '\n'
       << "energy:   " << c.energy << '\n'
       << "eT:       " << c.eT     << '\n'
       << "4-vec E:  " << c.fourVector.E() << '\n'
       << "4-vec Px: " << c.fourVector.Px() << '\n'
       << "4-vec Py: " << c.fourVector.Py() << '\n'
       << "4-vec Pz: " << c.fourVector.Pz();

    return os;

}

std::ostream& operator<<(std::ostream& os, const ZeeDElectronBags::IDCut& i)
{

    os << "ClusterStripsWtot: "<< std::boolalpha << i.ClusterStripsWtot << '\n'
       << "ClusterMiddleEratio37: "<< std::boolalpha << i.ClusterMiddleEratio37 << '\n'
       << "ClusterHadronicLeakage: "<< std::boolalpha << i.ClusterHadronicLeakage     << '\n'
       << "TrackTRTratio: "<< std::boolalpha << i.TrackTRTratio;

    return os;

}


}
