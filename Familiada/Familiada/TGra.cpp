#include "TGra.h"

TDruzyna* TGra::getDruzynaLewa() const {
    return druzynaLewa;
}

TDruzyna* TGra::getDruzynaPrawa() const {
    return druzynaPrawa;
}

void TGra::inicjalizuj(std::string nazwaL, std::string nazwaP) {
    // Tutaj fizycznie tworzymy (budujemy) obie drużyny w pamięci komputera!
    druzynaLewa = new TDruzyna(nazwaL);
    druzynaPrawa = new TDruzyna(nazwaP);

    // Na start ustawiamy, że odpowiada drużyna lewa
    aktywnaDruzyna = druzynaLewa;
}