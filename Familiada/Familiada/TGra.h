#pragma once
#include "TBazaPytan.h"
#include "TDruzyna.h"
#include <string>

enum class Druzyna
{
    Lewa,
    Prawa
};

class TGra {
private:
    TBazaPytan baza;
    TPytanie aktualne;
    TDruzyna* druzynaLewa;
    TDruzyna* druzynaPrawa;
    bool czyLewaAktywna;
    int sumaPunktowRundy;

    Druzyna aktywnaDruzyna;
    Druzyna wlascicielRundy;

    bool rundaPrzejeta;

    int aktualnaRunda;
    int liczbaRund;

    bool czyNasluchiwacBuzzerow;
    bool buzzerZablokowany;
public:
    TGra();
    ~TGra(); // Pamiętaj o delete druzynaLewa/Prawa w .cpp!
    void inicjalizuj(std::string nazwaL, std::string nazwaP);
    void wczytajPytaniaZPliku(std::string nazwa);
    void losujPytanie();
    TPytanie& getAktualnePytanie();
    int SprawdzOdpowiedz(std::string wpisana);
    TDruzyna* getDruzynaLewa();
    TDruzyna* getDruzynaPrawa();
    bool DodajBladLewa();
    bool DodajBladPrawa();
    void DodajPunktyRundy(int pkt);
    void ResetujPunktyRundy();

    int PobierzPunktyRundy() const;
    void UstawAktywnaDruzyne(Druzyna dr);
    Druzyna PobierzAktywnaDruzyne() const;
    void UstawWlascicielaRundy(Druzyna dr);
    Druzyna PobierzWlascicielaRundy() const;
    void UstawPrzejecieRundy(bool wartosc);
    bool CzyRundaPrzejeta() const;
    void PrzyznajPunktyDruzynie(Druzyna dr);
    int PobierzBledyAktywnejDruzyny() const;
    bool DodajBladAktywnejDruzynie(); // Zwraca true, jeśli po błędzie nastąpiła zmiana drużyny
    void UstawNasluchiwanie(bool stan);
    bool CzyBuzzerZablokowany() const;
    bool CzyNasluchujemy() const;
    bool WcisnijBuzzer(Druzyna dr); // Zwraca true, jeśli wciśnięcie było ważne
    void OdblokujBuzzery();
};

