#include "ZeeDTools/ZeeDCutBit.h"

std::ostream& operator<<(std::ostream& os, const ZeeDCutBit& cb) {
    os << cb.GetMask();
    return os;
}
