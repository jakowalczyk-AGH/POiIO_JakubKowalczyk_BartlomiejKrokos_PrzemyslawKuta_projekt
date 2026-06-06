#include "TBazaPytan.h"
#include <fstream>
#include <string>
#include <cstdlib>

TBazaPytan::TBazaPytan() {}

void TBazaPytan::wczytajZPliku(std::string sciezka) {
    std::ifstream plik(sciezka);
    if (!plik.is_open()) return;

    std::string linia;
    TPytanie* aktualne = nullptr;

    while (std::getline(plik, linia)) {
        // Szukamy pytania
        if (linia.find("\"pytanie\"") != std::string::npos) {
            // Zapisujemy poprzednie pytanie, jeśli istnieje
            if (aktualne) {
                pytania.push_back(*aktualne);
                delete aktualne;
            }

            size_t s1 = linia.find("\"", linia.find(":"));
            if (s1 != std::string::npos) {
                size_t s2 = linia.find("\"", s1 + 1);
                if (s2 != std::string::npos) {
                    aktualne = new TPytanie(linia.substr(s1 + 1, s2 - s1 - 1));
                }
            }
        }
        // Szukamy odpowiedzi (zakładamy, że "tekst" i "punkty" mogą być w tej samej linii)
        else if (linia.find("\"tekst\"") != std::string::npos && aktualne) {
            size_t s1 = linia.find("\"", linia.find(":"));
            if (s1 != std::string::npos) {
                size_t s2 = linia.find("\"", s1 + 1);
                if (s2 != std::string::npos) {
                    std::string tekst = linia.substr(s1 + 1, s2 - s1 - 1);

                    // Szukamy punktów w tej samej linii
                    size_t pozPunkty = linia.find("\"punkty\"");
                    int punkty = 0;
                    if (pozPunkty != std::string::npos) {
                        size_t dwukropek = linia.find(":", pozPunkty);
                        if (dwukropek != std::string::npos) {
                            std::string cyfry = "";
                            // Zbieramy tylko cyfry, ignorując spacje czy przecinki
                            for (size_t i = dwukropek; i < linia.length(); i++) {
                                if (linia[i] >= '0' && linia[i] <= '9') {
                                    cyfry += linia[i];
                                }
                            }
                            if (!cyfry.empty()) {
                                punkty = std::stoi(cyfry);
                            }
                        }
                    }
                    // Dodajemy odpowiedź do obiektu
                    aktualne->dodajOdpowiedz(tekst, punkty);
                }
            }
        }
    }

    // Pamiętamy o dodaniu ostatniego pytania z pętli
    if (aktualne) {
        pytania.push_back(*aktualne);
        delete aktualne;
    }

    plik.close();
}

TPytanie TBazaPytan::losujPytanie() {
    if (pytania.empty()) return TPytanie("Brak pytan - sprawdz plik JSON");
    return pytania[rand() % pytania.size()];
}

std::vector<TPytanie> TBazaPytan::pobierzUnikalnePytania(int ile) {
    std::vector<TPytanie> wynik;
    std::vector<TPytanie> kopia = pytania; // Tworzymy tymczasową kopię całej bazy

    for (int i = 0; i < ile; i++) {
        if (kopia.empty()) break; // Zabezpieczenie, gdyby w bazie było mniej niż 5 pytań

        int indeks = rand() % kopia.size();
        wynik.push_back(kopia[indeks]);

        // Wywalamy wylosowane pytanie z kopii, żeby się nie powtórzyło!
        kopia.erase(kopia.begin() + indeks);
    }
    return wynik;
}