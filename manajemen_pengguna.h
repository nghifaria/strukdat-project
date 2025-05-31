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
    static std::string encryptCaesar(const std::string& text, int shift);
    static std::string decryptCaesar(const std::string& text, int shift);
    static const int CAESAR_SHIFT = 3; 

    Pengguna* cariPenggunaInternal(const std::string& username);

    void muatPenggunaDariFile();
    void simpanSemuaPenggunaKeFile() const;
    TipePeran stringToTipePeran(const std::string& strPeran) const;

public:
    ManajemenPengguna(const std::string& namaFile = "pengguna_data.csv");
    ~ManajemenPengguna();

    bool registrasiUserPublik(const std::string& username, const std::string& password);
    bool registrasiPenggunaOlehAdmin(const std::string& username, const std::string& password, TipePeran peran);
    bool loginPengguna(const std::string& username, const std::string& password);
    void logoutPengguna();
    Pengguna* getPenggunaSaatIni() const;
    bool hapusPengguna(const std::string& usernameAdmin, const std::string& usernameTarget);
    void tampilkanSemuaPenggunaDebug() const;
    bool updateUserRole(const std::string& username, TipePeran peranBaru);
    std::string tipePeranToString(TipePeran peran) const;
};

#endif