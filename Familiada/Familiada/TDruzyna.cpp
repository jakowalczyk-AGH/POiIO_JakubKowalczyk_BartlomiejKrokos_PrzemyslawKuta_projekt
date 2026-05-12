#include "TDruzyna.h"

TDruzyna::TDruzyna(std::string p_nazwa) {
    nazwa = p_nazwa;
    punktyCalkowite = 0;
    bledy = 0;
}

void TDruzyna::dodajGracza(std::string imie) {
    gracze.push_back(imie);
}

std::vector<std::string> TDruzyna::getGracze() const {
    return gracze;
}

std::string TDruzyna::getNazwa() const {
    return nazwa;
}