// manajemen_pengguna.h
#ifndef MANAJEMEN_PENGGUNA_H
#define MANAJEMEN_PENGGUNA_H

#include "pengguna.h"
#include <string>
#include <vector>

class ManajemenPengguna {
private:
    std::vector<Pengguna> daftarPengguna;
    std::string namaFilePenggunaInternal;
    Pengguna* penggunaSaatIniInternal; 

    static unsigned long hashPasswordSederhana(const std::string& password);
    Pengguna* cariPenggunaInternal(const std::string& username);

    void muatPenggunaDariFile();
    void simpanSemuaPenggunaKeFile() const;
    TipePeran stringToTipePeran(const std::string& strPeran) const;
    std::string tipePeranToString(TipePeran peran) const;


public:
    ManajemenPengguna(const std::string& namaFile = "pengguna_data.csv");
    ~ManajemenPengguna();

    bool registrasiPenggunaBaru(const std::string& username, const std::string& password, TipePeran peran = TipePeran::PENGGUNA);
    bool loginPengguna(const std::string& username, const std::string& password);
    void logoutPengguna();
    Pengguna* getPenggunaSaatIni() const;
    void tampilkanSemuaPenggunaDebug() const;
};

#endif // MANAJEMEN_PENGGUNA_H