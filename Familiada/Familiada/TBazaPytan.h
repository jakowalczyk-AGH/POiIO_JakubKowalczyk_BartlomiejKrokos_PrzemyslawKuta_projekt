#pragma once
#include "TPytanie.h"
#include <vector>
#include <string>

class TBazaPytan {
private:
    std::vector<TPytanie> pytania;
public:
    TBazaPytan();
    void wczytajZPliku(std::string sciezka);
    TPytanie losujPytanie();
    std::vector<TPytanie> pobierzUnikalnePytania(int ile);
};