#include "ZeeDArrays/ZeeDSimpleBinGrid.h"

//------------------------------------------------------
void ZeeDSimpleBinGrid::ReadFromFile(TString fileName)
{
  // Open the input file 
  std::ifstream inFile;
  if (inFile.open(fileName), inFile != NULL) { // FIXME: null? stream::file_err?
    Info("ZeeDSimpleBinGrid::ReadFromFile", "File is opened %s", fileName.Data());
  } else {
    Error("ZeeDSimpleBinGrid::ReadFromFile", "Couldn't open file %s", fileName.Data());
    gSystem->Exit(EXIT_FAILURE);
  }

  // Read the file and remove stuff
  string text_all;
  while ( !inFile.eof() ) {
    string sline;
    getline(inFile, sline);

    // Remove comments
    int pos = sline.find("//");
    if ( pos > -1 ) sline.erase(pos, sline.length() - pos);

    text_all += sline;
    text_all += '\n';
  }

  // Close the input file
  inFile.close();

  // Read bin edges from the steering
  bins_edges = this->readBinEdges( &text_all );

  // Fill a vector of shifts to keep nD in 1D
  dimension_shifts = new vector<unsigned int>();
  dimension_shifts->push_back(1);
  for (BinsEdges::iterator itedges = bins_edges->begin();
      itedges != bins_edges->end(); ++itedges) {
    dimension_shifts->push_back(dimension_shifts->back() * ((*itedges)->size() - 1));
  }

}

//------------------------------------------------------
ZeeDSimpleBinGrid::BinsEdges* ZeeDSimpleBinGrid::readBinEdges(string* steer)
{
  BinsEdges* all_edges = new BinsEdges();

  string steer_tmp;
  int pos0 = 0;
  while ( this->readString(steer,  &pos0, &steer_tmp) ) {
    vector<float>* edges = new vector<float>();

    std::stringstream sstr(steer_tmp);

    float value;
    while ( sstr >> value ) edges->push_back(value);

    // sort edges and remove duplicates
    std::sort(edges->begin(), edges->end());
    edges->erase(std::unique(edges->begin(), edges->end()), edges->end());

    // add new dimension
    all_edges->push_back(edges);
  }

  return all_edges;
}

//------------------------------------------------------
bool ZeeDSimpleBinGrid::readString(string* steer, int* pos, string* steer_tmp)
{
  // return the string of the form [bin edges separated by blancks]

  int pos1 = steer->find("Edges", *pos);
  int pos2 = steer->find("{", pos1 + 1);
  int pos3 = steer->find("}", pos2 + 1);
  *pos = pos3 + 1;

  if (  -1 < pos1 && -1 < pos2 && -1 < pos3 ) {
    steer_tmp->assign(*steer, pos2 + 1, pos3 - pos2 - 1);

    // replace commas by spaces
    std::replace(steer_tmp->begin(), steer_tmp->end(), ',', ' ');

    return true;
  }

  return false;
}

//------------------------------------------------------
int ZeeDSimpleBinGrid::GetIndex(const ZeeDArrayD* coord_vector) const
{
  // dimension check
  if ((unsigned int)coord_vector->GetEntriesFast() != bins_edges->size()) {
    Error("ZeeDSimleBinGrid::GetIndex", "Dimensions of bin grid (%d) and coordinates vector (%d) do not coinside.",
	bins_edges->size(), coord_vector->GetEntriesFast());
    gSystem->Exit(EXIT_FAILURE);
  }

  // get the vector of bin coordinates from the point coordinates
  vector<unsigned int> bin_vector;
  for (BinsEdges::iterator itedges = bins_edges->begin();
      itedges != bins_edges->end(); ++itedges) {
    float icoord = coord_vector->At(itedges - bins_edges->begin());
    unsigned int bin = std::upper_bound((*itedges)->begin(), (*itedges)->end(), icoord) - (*itedges)->begin();
    if (bin < 1 || bin > (*itedges)->size()) return -1;
    bin_vector.push_back(bin - 1);
  }

  // convert the vector bin coordinates to a single index
  return this->vector2index(bin_vector);

}

//------------------------------------------------------
int ZeeDSimpleBinGrid::vector2index(vector<unsigned int> coord) const
{
  // The routine converts the vector coordinates to index
  int index = 0;
  for (unsigned int i = 0; i < coord.size(); ++i) {
    index += coord.at(i) * dimension_shifts->at(i);
  }

  return index;
}
