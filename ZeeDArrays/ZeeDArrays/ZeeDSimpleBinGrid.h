#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDSimpleBinGrid.h
////////////////////////////////////////////////////////

// Root includes
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

// Root includes
#include <TString.h>
#include <TObject.h>
#include <TObjArray.h>
#include <TSystem.h>

// ZeeD analysis includes
#include "ZeeDArrays/ZeeDArrayD.h"


using std::string;
using std::vector;

/** Class to define simple bin grid based on ZeeDBinGrid class
  @author SSh 
  @date 2013/12/12
 */

class ZeeDSimpleBinGrid
{


  public:

    typedef vector< vector<float>* > BinsEdges;

    void ReadFromFile(TString fileName);
    int  GetIndex(const ZeeDArrayD* coord_vector) const;
    int  GetNumOfCells() const {
      return (dimension_shifts != NULL)? dimension_shifts->back() : 0;
    }


  private:

    BinsEdges* readBinEdges(std::string* steer);
    bool readString(std::string* steer, int* pos, std::string* steer_tmp);
    int  vector2index(vector<unsigned int> coord) const;

    BinsEdges* bins_edges;
    vector<unsigned int>* dimension_shifts;

};
