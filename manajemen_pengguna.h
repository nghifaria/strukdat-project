// manajemen_pengguna.h
#ifndef MANAJEMEN_PENGGUNA_H
#define MANAJEMEN_PENGGUNA_H

#include "pengguna.h"
#include <string>
#include <vector>
#include <fstream> 
#include <sstream> 

class ManajemenPengguna {
private:
    std::vector<Pengguna> daftarPengguna;
    std::string namaFilePenggunaInternal;

    static unsigned long hashPasswordSederhana(const std::string& password);
    Pengguna* cariPenggunaInternal(const std::string& username);

    void muatPenggunaDariFile(); 
    void simpanSemuaPenggunaKeFile() const;

public:
    ManajemenPengguna(const std::string& namaFile = "pengguna_data.csv");
    ~ManajemenPengguna(); 

    bool registrasiPenggunaBaru(const std::string& username, const std::string& password);
    bool loginPengguna(const std::string& username, const std::string& password);
    void tampilkanSemuaPenggunaDebug() const;
    void setNamaFile(const std::string& namaFile);
};

#endif // MANAJEMEN_PENGGUNA_H