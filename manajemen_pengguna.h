// manajemen_pengguna.h
#ifndef MANAJEMEN_PENGGUNA_H
#define MANAJEMEN_PENGGUNA_H

#include "pengguna.h"
#include <string>
#include <vector>

class ManajemenPengguna {
private:
    std::vector<Pengguna> daftarPengguna;

    // Fungsi hash sederhana (implementasi mandiri)
    static unsigned long hashPasswordSederhana(const std::string& password);
    Pengguna* cariPenggunaInternal(const std::string& username); // Helper

public:
    ManajemenPengguna();
    // ~ManajemenPengguna(); // Tidak perlu jika hanya vector standar

    bool registrasiPenggunaBaru(const std::string& username, const std::string& password);
    bool loginPengguna(const std::string& username, const std::string& password);
    void tampilkanSemuaPenggunaDebug() const; // Untuk debugging
};

#endif // MANAJEMEN_PENGGUNA_H