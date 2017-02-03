#pragma once

#include <vector>
#include <utility>
#include <algorithm>

#include <cstdlib>


// template class to contain pairs of <Object, Weight>
// the construction is used in ZeeD for multiple purposes
// and now is sortable to get the best object, second best etc.


template <class T> class LinkWeightContainer : public std::vector< std::pair<T, double> >
{
  public:

    LinkWeightContainer() {

      std::cout << "This container is purposed to keep link-weight pairs," << std::endl;
      std::cout << "but was created with the type which is not a link." << std::endl;
      std::cout << "Consult LinkWeightContainer.h for the details." << std::endl;

      exit(-1);

    }

    void Add(T* link, double weight) { }

    T* GetLink(int i) { return NULL; }

    double GetWeight(int i) { return 0.; }

    void Sort() { }

    void Clear() { }

    void DeepCopy(LinkWeightContainer<T> const* lwc) { }


  private:

    LinkWeightContainer(LinkWeightContainer& rhs) {}
    LinkWeightContainer(LinkWeightContainer* rhs) {}
    static bool compare_weights(std::pair<T*, double> e1, std::pair<T*, double> e2) { return false; }

};

template <class T> class LinkWeightContainer<T*> : public std::vector< std::pair<T*, double> >
{
  public:

    void Add(T* link, double weight) { 
      this->push_back( std::pair<T*, double>(link, weight) );
    }

    // behave exactly like the std::vector behaves
    T* GetLink(int i) { return this->at(i).first; }
    double GetWeight(int i) { return this->at(i).second; }

    void Sort() {
      std::stable_sort(this->begin(), this->end(), compare_weights);
    }

    void Clear() {
      if (this->empty()) return;
      for (typename LinkWeightContainer::iterator it = this->begin(); it != this->end(); ++it) delete ((*it).first);
      this->clear();
    }

    void DeepCopy(LinkWeightContainer<T*> const* lwc) {
      for (typename LinkWeightContainer<T*>::const_iterator it = lwc->begin(); it != lwc->end(); ++it) {
	T* obj = new T(*(*it).first);
	this->push_back(std::pair<T*, double>(obj, (*it).second));
      }
    }

    ~LinkWeightContainer() {
      this->Clear();
    }


  private:

    static bool compare_weights(std::pair<T*, double> e1, std::pair<T*, double> e2) { return (e1.second > e2.second); }

};
