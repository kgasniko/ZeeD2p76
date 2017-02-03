#ifndef ZEED_SHELF_SELECTOR_H
#define ZEED_SHELF_SELECTOR_H

//------------------------------------------------------------------------------
class ZeeDShelfSelector : public TNamed {

public:

    ZeeDShelfSelector() : fDefaultLevel(0),
                          fDefaultLevelMask(0)
    {

        fDefaultLevelMask.Add(ZeeDLevel::Technical);

    }

    virtual Bool_t evaluate(const ZeeDEvent*) const = 0 ;

    void SetDefaultLevel(ZeeDLevelBit defaultLevel) {
        fDefaultLevel = defaultLevel;
    }

    ZeeDLevelBit GetDefaultLevel() const {
        return fDefaultLevel;
    }

    void SetDefaultLevelMask(ZeeDLevelBit defaultLevelMask) {
        fDefaultLevelMask = defaultLevelMask;
    }

    ZeeDLevelBit GetDefaultLevelMask() const {
        return fDefaultLevelMask;
    }

private:

    ZeeDLevelBit fDefaultLevel;
    ZeeDLevelBit fDefaultLevelMask;

};

//------------------------------------------------------------------------------
class ZeeDAllShelfSelector : public ZeeDShelfSelector {

public:

    Bool_t evaluate(const ZeeDEvent*) const {
        return kTRUE;
    }

};

//------------------------------------------------------------------------------
class ZeeDPeriodShelfSelector : public ZeeDShelfSelector {

public:

    ZeeDPeriodShelfSelector(ZeeDEnum::Period::Value period) : fPeriod(period) {}

    Bool_t evaluate(const ZeeDEvent* event) const {

        //std::cout << event->GetPeriod() << std::endl;
        //std::cout << fPeriod << std::endl;

        return event->GetPeriod() == fPeriod;

    }

private:

    ZeeDEnum::Period::Value fPeriod;

};

//------------------------------------------------------------------------------
class ZeeDMuShelfSelector : public ZeeDShelfSelector {

public:

    ZeeDMuShelfSelector(Double_t muMin, Double_t muMax) : fMuMin(muMin), fMuMax(muMax) {}

    Bool_t evaluate(const ZeeDEvent* event) const {

        //std::cout << event->GetMu() << std::endl;
        //std::cout << fMu << std::endl;

        Bool_t passed = fMuMin < event->AverageInteractionsPerCrossing().Get() &&
            event->AverageInteractionsPerCrossing().Get() <= fMuMax;

        return passed;

    }

private:

    const Double_t fMuMin;
    const Double_t fMuMax;

};

//------------------------------------------------------------------------------
class ZeeDInitialStateShelfSelector : public ZeeDShelfSelector {

public:

    ZeeDInitialStateShelfSelector(ZeeDEnum::IS::Value IS) : fIS(IS) {}

    Bool_t evaluate(const ZeeDEvent* event) const {

        if ( event->PDF().IsSet() == kFALSE) {
            return kFALSE;
        }

        DEBUG_VAR(fIS);

        ZeeDEventGenBags::PDF pdf = event->PDF().Get();
        int id1 = pdf.id1;
        int id2 = pdf.id2;

        DEBUG_VAR(id1);
        DEBUG_VAR(id2);

        Bool_t c = kFALSE;

        switch(fIS) {

        case ZeeDEnum::IS::ddbar:
            c = check(id1, id2, 1, -1);
            break;

        case ZeeDEnum::IS::uubar:
            c = check(id1, id2, 2, -2);
            break;

        case ZeeDEnum::IS::ssbar:
            c = check(id1, id2, 3, -3);
            break;

        case ZeeDEnum::IS::ccbar:
            c = check(id1, id2, 4, -4);
            break;

        case ZeeDEnum::IS::bbbar:
            c = check(id1, id2, 5, -5);
            break;

        case ZeeDEnum::IS::glue:
            c = (id1 == 21 || id2 == 21);
            break;

        default:

            return kFALSE;
        }

        DEBUG_MES("returning");
        DEBUG_VAR(c);

        return c;
        //return kTRUE;

    }

private:

    Bool_t check(int lid1, int lid2, int rid1, int rid2) const {

        DEBUG_MES_START;

        if ((lid1 == rid1 && lid2 == rid2) ||
            (lid1 == rid2 && lid2 == rid1)) {
            DEBUG_MES("check::kTRUE");
            DEBUG_MES_END;
            return kTRUE;
        } else {
            DEBUG_MES("check::kFALSE")
                DEBUG_MES_END;
            return kFALSE;
        }

    }

    ZeeDEnum::IS::Value fIS;

};

#endif // ZEED_SHELF_SELECTOR_H
