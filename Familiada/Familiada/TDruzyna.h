#pragma once
#include <string>
#include <vector>

class TDruzyna {
private:
    std::string nazwa;
    std::vector<std::string> gracze; // NOWOŚĆ: Lista członków drużyny
    int punktyCalkowite;
    int bledy;

public:
    TDruzyna(std::string p_nazwa);

    // Nowe metody do obsługi graczy:
    void dodajGracza(std::string imie);
    std::vector<std::string> getGracze() const;

    // ... (reszta metod bez zmian, np. getPunkty(), dodajBlad() itd.)
};