#pragma once
#include <string>
#include <vector>

class TDruzyna {
private:
    std::string nazwa;
    std::vector<std::string> gracze;
    int punkty;
    int bledy;

public:
    TDruzyna(std::string p_nazwa);
    void dodajGracza(std::string imie);
    void dodajPunkty(int ile);

    std::string getNazwa() const;
    int getPunkty() const;

    std::vector<std::string> getGracze() const;

    void DodajBlad();
    void ResetujBledy();

    int PobierzBledy() const;
    bool CzyTrzyBledy() const;
};