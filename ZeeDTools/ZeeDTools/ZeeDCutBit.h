#ifndef ZEEDCUTBIT
#define ZEEDCUTBIT

#include <limits>
#include <iostream>
#include <TObject.h>

////////////////////////////////////////////////////////
// Name    : ZeeDCutBit.h
////////////////////////////////////////////////////////

// Root includes

/** Basic class to define cuts
    @author Andrei Nikiforov, Alexander Glazov,
    @date 2008/08/27
*/
class ZeeDCutBit : public TObject {
public:

    // 64 bit
    typedef unsigned long long maskDataType;

    static UInt_t GetNBits() {
        return 64;
    }

    // 8 bit
    // typedef char maskDataType;
    /*
    static UInt_t GetNBits() {
        return 64;
    }
    */

    ZeeDCutBit() {
        m_mask = 0;
    }
    ZeeDCutBit(const maskDataType& mask) {
        m_mask = mask;
    }
    void SetMask(const maskDataType& mask) {
        m_mask = mask;
    }
    void SetBit(int iCut) {
        m_mask |= (maskDataType(1) << iCut);
    }
    void SetMaskAll(int Nbit) {
        m_mask = 0;

        for (int i = 0; i < Nbit; i++) {
            SetBit(i);
        }
    }

    void Add(ZeeDCutBit* mask) {
        m_mask |=   mask->GetMask();
    }
    void Sub(ZeeDCutBit* mask) {
        m_mask &= ~(mask->GetMask());
    }

    void Add(const ZeeDCutBit& mask) {
        m_mask |=   mask.GetMask();
    }
    void Sub(const ZeeDCutBit& mask) {
        m_mask &= ~(mask.GetMask());
    }

    maskDataType  GetMask() const {
        return m_mask;
    }

    maskDataType operator ==(const ZeeDCutBit& mask) {
        return (mask.GetMask() == this->GetMask());
    }

    maskDataType operator ^ (const ZeeDCutBit& mask) {
        return ( (mask.GetMask() & this->GetMask() ) == mask.GetMask());
    }

    maskDataType operator & (const ZeeDCutBit& mask) {
        return ( (mask.GetMask() & this->GetMask()));
    }

    operator maskDataType() { return this->GetMask(); }

    static maskDataType All() {
        return std::numeric_limits<ZeeDCutBit::maskDataType>::max();
    }

private:
    maskDataType m_mask;
};

std::ostream& operator<<(std::ostream& os, const ZeeDCutBit& cb);

typedef ZeeDCutBit ZeeDLevelBit;

class ZeeDLevel {

    public:

    enum TYPE {

        ForEach     = ZeeDCutBit::maskDataType(1) << 0,
        ToyMC       = ZeeDCutBit::maskDataType(1) << 1	| ForEach,
        Systematics = ZeeDCutBit::maskDataType(1) << 2	| ToyMC,
        Analysis    = ZeeDCutBit::maskDataType(1) << 3	| Systematics,
        Technical   = ZeeDCutBit::maskDataType(1) << 4	| Analysis

    };

};

class ZeeDHistLevel {

    public:

    enum TYPE {

        ForEach = ZeeDCutBit::maskDataType(1)     << 0,
        ToyMC = ZeeDCutBit::maskDataType(1)       << 1,
        Systematics = ZeeDCutBit::maskDataType(1) << 2,
        Analysis = ZeeDCutBit::maskDataType(1)    << 3,
        Technical = ZeeDCutBit::maskDataType(1)   << 4

    };

};

#endif
