// manajemen_pengguna.h
#ifndef MANAJEMEN_PENGGUNA_H
#define MANAJEMEN_PENGGUNA_H

#include "pengguna.h"
#include <string>
#include <vector>

class ManajemenPengguna {
private:
    std::vector<Pengguna> daftarPengguna;
    std::string namaFilePenggunaInternal; // Nama file yang digunakan oleh instance ini

    static unsigned long hashPasswordSederhana(const std::string& password);
    Pengguna* cariPenggunaInternal(const std::string& username);

    void muatPenggunaDariFile(); 
    void simpanSemuaPenggunaKeFile() const;

public:
    ManajemenPengguna(const std::string& namaFile = "pengguna_data.csv"); // Constructor dengan nama file default
    ~ManajemenPengguna(); 

    bool registrasiPenggunaBaru(const std::string& username, const std::string& password);
    bool loginPengguna(const std::string& username, const std::string& password);
    void tampilkanSemuaPenggunaDebug() const;
    void setNamaFile(const std::string& namaFile); // Untuk mengubah nama file jika perlu (jarang dipakai)
};

#endif // MANAJEMEN_PENGGUNA_H