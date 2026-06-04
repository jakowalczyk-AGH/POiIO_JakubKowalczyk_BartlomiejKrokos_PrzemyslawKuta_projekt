#include "TGra.h"
#include "TDruzyna.h"
#include <ctime>
#include <cstdlib>

TGra::TGra() {
    srand(time(0));
    druzynaLewa = nullptr;
    druzynaPrawa = nullptr;
    sumaPunktowRundy = 0;   // <-- KLUCZOWA ZMIANA
    czyLewaAktywna = true;  // Inicjalizacja pozostałych zmiennych
    rundaPrzejeta = false;
    aktualnaRunda = 1;
    liczbaRund = 5;
    czyNasluchiwacBuzzerow = false;
    buzzerZablokowany = false;
    aktywnaDruzyna = Druzyna::Lewa; // Domyślnie
}

TGra::~TGra() {
    delete druzynaLewa;
    delete druzynaPrawa;
}

void TGra::inicjalizuj(std::string nazwaL, std::string nazwaP) {
    // Jeśli wcześniej istniały, usuwamy je przed stworzeniem nowych, 
    // żeby nie było wycieków pamięci
    delete druzynaLewa;
    delete druzynaPrawa;
    druzynaLewa = new TDruzyna(nazwaL);
    druzynaPrawa = new TDruzyna(nazwaP);
}
void TGra::wczytajPytaniaZPliku(std::string nazwa) { 
    baza.wczytajZPliku(nazwa); 
}
void TGra::losujPytanie() { 
    aktualne = baza.losujPytanie(); 
}
TPytanie& TGra::getAktualnePytanie() { 
    return aktualne; 
}
TDruzyna* TGra::getDruzynaLewa() {
    return druzynaLewa;
}

TDruzyna* TGra::getDruzynaPrawa() {
    return druzynaPrawa;
}
bool TGra::DodajBladLewa()
{
    druzynaLewa->DodajBlad();

    if (druzynaLewa->CzyTrzyBledy())
    {
        druzynaLewa->ResetujBledy();
        return true;
    }

    return false;
}

bool TGra::DodajBladPrawa()
{
    druzynaPrawa->DodajBlad();

    if (druzynaPrawa->CzyTrzyBledy())
    {
        druzynaPrawa->ResetujBledy();
        return true;
    }

    return false;
}

void TGra::DodajPunktyRundy(int pkt)
{
    sumaPunktowRundy += pkt;
}

int TGra::PobierzPunktyRundy() const
{
    return sumaPunktowRundy;
}

void TGra::ResetujPunktyRundy()
{
    sumaPunktowRundy = 0;
}

void TGra::PrzyznajPunktyDruzynie(Druzyna dr)
{
    if (dr == Druzyna::Lewa)
    {
        druzynaLewa->dodajPunkty(sumaPunktowRundy);
    }
    else
    {
        druzynaPrawa->dodajPunkty(sumaPunktowRundy);
    }

    sumaPunktowRundy = 0;
}

int TGra::SprawdzOdpowiedz(std::string wpisana) {
    auto& odpowiedzi = aktualne.getOdpowiedzi();

    for (size_t i = 0; i < odpowiedzi.size(); i++) {
        if (!odpowiedzi[i].odgadnieta && odpowiedzi[i].tekst == wpisana) {
            odpowiedzi[i].odgadnieta = true;
            DodajPunktyRundy(odpowiedzi[i].punkty); // Silnik sam dodaje punkty!
            return (int)i; // Zwracamy numer trafionej odpowiedzi
        }
    }
    return -1; // -1 oznacza, że nikt nie trafił (błąd)
}

Druzyna TGra::PobierzAktywnaDruzyne() const {
    return aktywnaDruzyna;
}

int TGra::PobierzBledyAktywnejDruzyny() const {
    if (aktywnaDruzyna == Druzyna::Lewa) {
        return druzynaLewa->PobierzBledy();
    }
    return druzynaPrawa->PobierzBledy();
}

bool TGra::DodajBladAktywnejDruzynie() {
    if (aktywnaDruzyna == Druzyna::Lewa) {
        bool zmiana = DodajBladLewa();
        if (zmiana) aktywnaDruzyna = Druzyna::Prawa; // Automatyczna zmiana
        return zmiana;
    }
    else {
        bool zmiana = DodajBladPrawa();
        if (zmiana) aktywnaDruzyna = Druzyna::Lewa; // Automatyczna zmiana
        return zmiana;
    }
}

void TGra::UstawAktywnaDruzyne(Druzyna dr) {
    aktywnaDruzyna = dr;
}

void TGra::UstawNasluchiwanie(bool stan) {
    czyNasluchiwacBuzzerow = stan;
}

bool TGra::CzyBuzzerZablokowany() const {
    return buzzerZablokowany;
}

bool TGra::CzyNasluchujemy() const {
    return czyNasluchiwacBuzzerow;
}

bool TGra::WcisnijBuzzer(Druzyna dr) {
    if (!czyNasluchiwacBuzzerow || buzzerZablokowany) {
        return false; // Ignorujemy wciśnięcie
    }

    buzzerZablokowany = true;
    aktywnaDruzyna = dr;
    return true; // Buzzer wciśnięty prawidłowo
}

void TGra::OdblokujBuzzery() {
    buzzerZablokowany = false;
}