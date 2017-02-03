#include "ZeeDCuts/ZeeDCutSelector.h"

#include <TObjArray.h>
#include <TROOT.h>
#include <TSystem.h>
#include <iterator>

#include "ZeeDTools/ZeeDCutBit.h"

ZeeDCutSelector::ZeeDCutSelector(TString Name) : TNamed(Name, Name)
{

}

ZeeDCutSelector::~ZeeDCutSelector()
{
  for (std::map<TString, ZeeDCut*>::iterator itcut = m_CutArray.begin();
      itcut != m_CutArray.end(); ++itcut) delete (itcut->second);
}

void ZeeDCutSelector::evaluate(const ZeeDEvent* event, ZeeDCutBit* bitmask)
{

  bitmask->SetMask(0);

  std::map<TString, ZeeDCut*>::iterator itcut = m_CutArray.begin();
  const unsigned int cutsize = m_CutArray.size(); 
  for (unsigned int icut = 0; icut < cutsize; ++icut) {
    if ( (itcut->second)->evaluate(event) ) bitmask->SetBit(icut);
    ++itcut;
  }

}

void ZeeDCutSelector::evaluate(const ZeeDEvent* event)
{
  this->evaluate(event, &fCutMask);
}

const ZeeDCutBit ZeeDCutSelector::getBitMask(TString Name)
{

  ZeeDCutBit res;

  std::map<TString, ZeeDCut*>::iterator itcut = m_CutArray.find(Name);
  if (itcut == m_CutArray.end()) {
    Error("ZeeDCutSelector::getBitMask", "Can not find cut " + Name);
    gSystem->Exit(1);
  }

  res.SetBit(std::distance(m_CutArray.begin(), itcut));

  return res;
}


void ZeeDCutSelector::AddCut(ZeeDCut* cut)
{

  m_CutArray[cut->getname()] = cut;

  if(m_CutArray.size() > (Long64_t)ZeeDCutBit::GetNBits()) {
    Error("ZeeDCutSelector::AddCut", "More than 64 cuts requested, cuts added:");

    for (std::map<TString, ZeeDCut*>::iterator itcut = m_CutArray.begin();
	itcut != m_CutArray.end(); ++itcut) {
      Error("ZeeDCutSelector::AddCut", "    %s", (itcut->first).Data());
    }

    gSystem->Exit(1);
  }

}




