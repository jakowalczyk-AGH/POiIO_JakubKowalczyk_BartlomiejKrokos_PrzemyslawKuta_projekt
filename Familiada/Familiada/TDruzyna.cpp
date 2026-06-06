#include "TDruzyna.h"

TDruzyna::TDruzyna(std::string p_nazwa) {
    nazwa = p_nazwa;
    punkty = 0;
    bledy = 0;
}

void TDruzyna::dodajGracza(std::string imie) {
    gracze.push_back(imie);
}

void TDruzyna::dodajPunkty(int ile) {
    punkty += ile;
}

std::string TDruzyna::getNazwa() const { 
    return nazwa; 
}
int TDruzyna::getPunkty() const { 
    return punkty; 
}
std::vector<std::string> TDruzyna::getGracze() const {
    return gracze;
}

void TDruzyna::DodajBlad()
{
    bledy++;
}

void TDruzyna::ResetujBledy()
{
    bledy = 0;
}

int TDruzyna::PobierzBledy() const
{
    return bledy;
}

bool TDruzyna::CzyTrzyBledy() const
{
    return bledy >= 3;
}