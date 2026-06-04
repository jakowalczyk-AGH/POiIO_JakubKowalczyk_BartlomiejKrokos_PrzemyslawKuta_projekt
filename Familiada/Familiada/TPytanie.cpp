#include "TPytanie.h"

TPytanie::TPytanie() { tresc = "Ladowanie..."; }
TPytanie::TPytanie(std::string p_tresc) { tresc = p_tresc; }

void TPytanie::dodajOdpowiedz(std::string p_tekst, int p_punkty) {
    odpowiedzi.push_back({ p_tekst, p_punkty });
}

std::string TPytanie::getTresc() const { return tresc; }
std::vector<TOdpowiedz>& TPytanie::getOdpowiedzi() { return odpowiedzi; }
const std::vector<TOdpowiedz>& TPytanie::getOdpowiedziRef() const
{
    return odpowiedzi;
}
bool TPytanie::CzyWszystkieOdkryte() const
{
    for (const auto& odp : odpowiedzi)
    {
        if (!odp.odgadnieta)
            return false;
    }

    return true;
}