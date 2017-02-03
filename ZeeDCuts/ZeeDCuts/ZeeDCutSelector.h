#ifndef ZEEDCUTSELECTOR
#define ZEEDCUTSELECTOR

////////////////////////////////////////////////////////
// Name    : ZeeDCutSelector.h
////////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCut.h"
#include <TString.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TSystem.h>
#include <cstdlib>
#include <map>

#include "ZeeDTools/ZeeDCutBit.h"

//class ZeeDCutBit;
class ZeeDEvent;


/** Basic class to evaluate a list of cuts.
  @author  Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
  @date 2008/08/27
 */
class ZeeDCutSelector : public TNamed {
  public:
    ZeeDCutSelector(TString Name);
    virtual ~ZeeDCutSelector();
    void AddCut(ZeeDCut* cut);
    void evaluate(const ZeeDEvent* event, ZeeDCutBit* bitmask);

    void evaluate(const ZeeDEvent* event);

    ZeeDCutBit getCutMask() { return fCutMask; }

    virtual void BookCuts() {}
    const std::map<TString, ZeeDCut*>* getCuts() {
      return (&m_CutArray);
    }

    ZeeDCut* getCut(TString Name) {
      std::map<TString, ZeeDCut*>::iterator itcut = m_CutArray.find(Name);
      if (itcut == m_CutArray.end()) {
	Warning("ZeeDCutSelector::getCut","Cut %s not found", Name.Data());
	return NULL;
      }
      return (itcut->second);
    }

    Bool_t hasCut(TString Name) {
      return (m_CutArray.count(Name) > 0);
    }

    const ZeeDCutBit getBitMask(TString Name);

  private:

    ZeeDCutBit fCutMask;

    /** List of cuts */
    std::map<TString, ZeeDCut*> m_CutArray; // actually, map, not an array
};

#endif
