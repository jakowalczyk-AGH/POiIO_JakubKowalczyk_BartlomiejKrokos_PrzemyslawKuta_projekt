#pragma once
#include <string>
#include <vector>

struct TOdpowiedz {
    std::string tekst;
    int punkty;
    bool odgadnieta;
};

class TPytanie {
private:
    std::string tresc;
    std::vector<TOdpowiedz> odpowiedzi;
public:
    TPytanie();
    TPytanie(std::string p_tresc);
    void dodajOdpowiedz(std::string p_tekst, int p_punkty);
    std::string getTresc() const;
    std::vector<TOdpowiedz>& getOdpowiedzi(); // Zwraca oryginał, nie kopię! (bez const)
    const std::vector<TOdpowiedz>& getOdpowiedziRef() const;
    bool CzyWszystkieOdkryte() const;
};