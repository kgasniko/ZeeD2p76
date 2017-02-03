#ifndef ZEEDQUANTITY_H
#define ZEEDQUANTITY_H

#include <TObject.h>
#include <TSystem.h>
#include <cstdlib>
#include "execinfo.h"
#include <cstdio>
#include "cxxabi.h"

/** Helper class for debugging ZeeDQuantity

    break ZeeDQuantityDbgHelper::GetExits()

    to find at which place in the code Get() is used although the
    quantity is not set

    break ZeeDQuantityDbgHelper::SetExits()

    to find where Set() is used although the quantity is locked

    @author Sebastian Schmitt
    @date September 2011

*/
class ZeeDQuantityDbgHelper {

public:

    static void GetExits();

    static void SetExits();

};

/** Wrapper to make clear if quantity of type T is initialized to a physical value.

   @author Sebastian Schmitt
   @date January 2011

 */
template<typename T>
class ZeeDQuantity : public TObject {

public:

    /** default constructor */
    ZeeDQuantity() : bIsSet(false), bIsLocked(false) {}

    /** constructor given value and explicit has state */
    ZeeDQuantity(const T& value, bool set) : fValue(value), bIsSet(set), bIsLocked(false) {}

    /** if value is simply given, it still does not have it */
    ZeeDQuantity(const T& value) : fValue(value), bIsSet(false), bIsLocked(false) {}

    /** Get quantity by copy. If quantity is not set let user know */
    const T& Get() const {

        if(bIsSet == false) {

            Error("ZeeDQuantity::Get()", "You called Get() for some ZeeD quantity but it is not set!");

	    printCallStack();

            fDbgHelper.GetExits();

            gSystem->Exit(EXIT_FAILURE);

        }

        return fValue;

    }

    /** Set quantity and lock. If already set let user know */
    bool Set(const T& value) {

        if(bIsLocked == true) {

            Error("ZeeDQuantity::Set(const T& value)", "You called Set() for some ZeeD quantity but it has been locked already!");

	    printCallStack();

            fDbgHelper.SetExits();

            gSystem->Exit(EXIT_FAILURE);

        }

        if(bIsLocked == false) {
            fValue = value;
            bIsSet = true;
        }

        return !bIsLocked;

    }

    bool IsSet() const {

        return bIsSet;

    }

    void Lock() {

        bIsLocked = true;

    }

    void UnLock() {

        bIsLocked = false;

    }

    void Set() {

        if (bIsLocked == false) {
            bIsSet = true;
        }

    }

private:

    T fValue;

    bool bIsSet;

    bool bIsLocked;

    ZeeDQuantityDbgHelper fDbgHelper;

    void printCallStack() const {

      Error("ZeeDQuantity", "Something bad happened: you either try to use variable hasn't been set or re-set the locked variable.\nThe content of the call stack below could give a hint on what is going on:");

      const int SIZE = 100;
      void *buffer[SIZE];

      int nptrs = backtrace(buffer, SIZE);
      char** strings = backtrace_symbols(buffer, nptrs);
      printf("=========================================\n");
      if (strings == NULL) {
	Error("ZeeDQuantity", "No backtrace_symbols found.");
      } else {

	int maxn = (nptrs > 20)? 20 : nptrs;
	for (int i = 0; i < maxn; i++) {
	  printf("%i\t%s\n", nptrs - i, cxx_demangle(strings[i]).c_str());
	}

	free(strings);
      }
      printf("=========================================\n");
    }
    
    std::string cxx_demangle(const char* symbol) const {
      size_t size;
      int status;
      char temp[501]; //???
      char* demangled;
      //first, try to demangle a c++ name
      if (1 == sscanf(symbol, "%*[^(]%*[^_]%500[^)+]", temp)) {
	if (NULL != (demangled = abi::__cxa_demangle(temp, NULL, &size, &status))) {
	  std::string result(demangled);
	  free(demangled);
	  return result;
	}
      }
      //if that didn't work, try to get a regular c symbol
      if (1 == sscanf(symbol, "%500s", temp)) {
	return temp;
      }

      //if all else fails, just return the symbol
      return symbol;
    }

};

#endif
