#ifndef ZEEDGETVARIABLES
#define ZEEDGETVARIABLES

////////////////////////////////////////////////
// Name: ZeeDGetVariables
// @author SSh, SG
// @date 19 March 2012
// ////////////////////////////////////////////
//---------------------------------------------
// The whole (header) file is devoted to the --
// calculation of the variables ---------------
//---------------------------------------------
// ////////////////////////////////////////////

#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDTools/ZeeDQuantity.h"

class ZeeDEvent;
class ZeeDArrayD;

/// List of functions:
Double_t CosThGen(const ZeeDEvent* event, ZeeDEnum::CosThStar::Value CosType, ZeeDEnum::MCFSRLevel::Value elecType);
ZeeDQuantity<ZeeDArrayD> getYcosTh_rec(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getYPtcosTh_rec(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getYcosTh_gen(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosTh_rec(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getAbsY_M_Pt_rec(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosThCS_rec(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getAbsY_M_rec(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosTh_gen(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getAbsY_M_Pt_gen(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosTh_genBare(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosTh_genDressed(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getYPtcosTh_gen(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getBosonMass(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getBosonCosTheta(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getElecPtMax(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getElecPtMin(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getBosonPt(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getBosonAbsY(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getBosonY(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getBornBosonAbsY(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getBornBosonMass(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getBosonY_Mass(const ZeeDEvent* event);

ZeeDQuantity<ZeeDArrayD> getBosonMass_bkg(const ZeeDEvent* event);
ZeeDQuantity<ZeeDArrayD> getIsoFracMax(const ZeeDEvent* event);

#endif

