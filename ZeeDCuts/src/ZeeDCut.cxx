#include "ZeeDCuts/ZeeDCut.h"

ZeeDCut::ZeeDCut(TString CutName) : m_name(CutName)
{

}

ZeeDCut::~ZeeDCut()
{
}

TString ZeeDCut::getname()
{
    return m_name;
}

