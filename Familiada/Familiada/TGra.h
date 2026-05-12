#pragma once
#include "TDruzyna.h"
#include "TBazaPytan.h"

class TGra {
private:
    // Tych linijek brakuje programowi:
    TDruzyna* druzynaLewa;
    TDruzyna* druzynaPrawa;

    // Tu pewnie masz jeszcze inne zmienne, np. kto teraz odpowiada:
    TDruzyna* aktywnaDruzyna;
    // ... reszta Twoich zmiennych ...

public:
    // ... reszta Twoich funkcji ...
    void inicjalizuj(std::string nazwaL, std::string nazwaP);

    // Nowe funkcje, żeby okienko mogło "dotknąć" drużyn i dodać im graczy
    TDruzyna* getDruzynaLewa() const;
    TDruzyna* getDruzynaPrawa() const;
};