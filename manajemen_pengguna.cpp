// manajemen_pengguna.cpp
#include "manajemen_pengguna.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

ManajemenPengguna::ManajemenPengguna(const std::string& namaFile) : namaFilePenggunaInternal(namaFile), penggunaSaatIniInternal(nullptr) {
    muatPenggunaDariFile();
    if (daftarPengguna.empty()) {
        std::cout << "Info: Daftar pengguna kosong. Membuat akun admin default (admin/admin)..." << std::endl;
        registrasiPenggunaOlehAdmin("admin", "admin", TipePeran::ADMIN);
    }
}

ManajemenPengguna::~ManajemenPengguna() {
    simpanSemuaPenggunaKeFile();
}

unsigned long ManajemenPengguna::hashPasswordSederhana(const std::string& password) {
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

Pengguna* ManajemenPengguna::cariPenggunaInternal(const std::string& username) {
    for (size_t i = 0; i < daftarPengguna.size(); ++i) {
        if (daftarPengguna[i].username == username) {
            return &daftarPengguna[i];
        }
    }
    return nullptr;
}

TipePeran ManajemenPengguna::stringToTipePeran(const std::string& strPeran) const {
    std::string peranLower = strPeran;
    std::transform(peranLower.begin(), peranLower.end(), peranLower.begin(), ::tolower);
    if (peranLower == "admin") {
        return TipePeran::ADMIN;
    }
    return TipePeran::PENGGUNA;
}

std::string ManajemenPengguna::tipePeranToString(TipePeran peran) const {
    switch (peran) {
        case TipePeran::ADMIN:
            return "ADMIN";
        case TipePeran::PENGGUNA:
            return "PENGGUNA";
        default:
            return "PENGGUNA";
    }
}

void ManajemenPengguna::muatPenggunaDariFile() {
    std::ifstream file(namaFilePenggunaInternal);
    if (!file.is_open()) {
        std::cout << "Info: File data pengguna '" << namaFilePenggunaInternal << "' tidak ditemukan. Akan dimulai dengan daftar pengguna kosong atau admin default." << std::endl;
        return;
    }
    daftarPengguna.clear();
    std::string baris;
    while (std::getline(file, baris)) {
        std::stringstream ss(baris);
        std::string username;
        std::string hashedPasswordStr;
        std::string peranStr;
        unsigned long hashedPasswordVal;
        TipePeran peranVal = TipePeran::PENGGUNA;

        if (std::getline(ss, username, ',') && std::getline(ss, hashedPasswordStr, ',') && std::getline(ss, peranStr)) {
            try {
                hashedPasswordVal = std::stoul(hashedPasswordStr);
                peranVal = stringToTipePeran(peranStr);
                daftarPengguna.emplace_back(username, hashedPasswordVal, peranVal);
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Peringatan: Format data tidak valid di file pengguna untuk user: " << username << " Baris: " << baris << " Detail: " << ia.what() << std::endl;
            } catch (const std::out_of_range& oor) {
                std::cerr << "Peringatan: Nilai hashed password di luar jangkauan untuk user: " << username << " Baris: " << baris << " Detail: " << oor.what() << std::endl;
            }
        } else {
             if (!baris.empty()) {
                std::cerr << "Peringatan: Baris tidak lengkap atau format salah di file pengguna: " << baris << std::endl;
             }
        }
    }
    file.close();
    if (!daftarPengguna.empty()){
        std::cout << "Info: Data pengguna berhasil dimuat dari '" << namaFilePenggunaInternal << "'." << std::endl;
    }
}

void ManajemenPengguna::simpanSemuaPenggunaKeFile() const {
    std::ofstream file(namaFilePenggunaInternal);
    if (!file.is_open()) {
        std::cerr << "Error: Tidak dapat membuka file data pengguna '" << namaFilePenggunaInternal << "' untuk ditulis." << std::endl;
        return;
    }
    for (const auto& pengguna : daftarPengguna) {
        file << pengguna.username << "," << pengguna.hashedPassword << "," << tipePeranToString(pengguna.peran) << "\n";
    }
    file.close();
    std::cout << "Info: Data pengguna berhasil disimpan ke '" << namaFilePenggunaInternal << "'." << std::endl;
}

bool ManajemenPengguna::registrasiPenggunaBaruStandar(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        std::cout << "Error: Username dan password tidak boleh kosong." << std::endl;
        return false;
    }
    if (this->cariPenggunaInternal(username) != nullptr) {
        std::cout << "Error: Username '" << username << "' sudah terdaftar." << std::endl;
        return false;
    }
    daftarPengguna.emplace_back(username, ManajemenPengguna::hashPasswordSederhana(password), TipePeran::PENGGUNA);
    std::cout << "Info: Pengguna '" << username << "' dengan peran 'PENGGUNA' berhasil diregistrasi." << std::endl;
    return true;
}

bool ManajemenPengguna::registrasiPenggunaOlehAdmin(const std::string& username, const std::string& password, TipePeran peran) {
    if (username.empty() || password.empty()) {
        std::cout << "Error: Username dan password tidak boleh kosong." << std::endl;
        return false;
    }
    if (this->cariPenggunaInternal(username) != nullptr) {
        std::cout << "Error: Username '" << username << "' sudah terdaftar." << std::endl;
        return false;
    }
    daftarPengguna.emplace_back(username, ManajemenPengguna::hashPasswordSederhana(password), peran);
    std::cout << "Info: Pengguna '" << username << "' dengan peran '" << tipePeranToString(peran) << "' berhasil diregistrasi oleh admin." << std::endl;
    return true;
}


bool ManajemenPengguna::loginPengguna(const std::string& username, const std::string& password) {
    Pengguna* pengguna = this->cariPenggunaInternal(username);
    if (pengguna == nullptr) {
        std::cout << "Error: Username '" << username << "' tidak ditemukan." << std::endl;
        penggunaSaatIniInternal = nullptr;
        return false;
    }
    if (pengguna->hashedPassword == ManajemenPengguna::hashPasswordSederhana(password)) {
        std::cout << "Info: Login berhasil untuk pengguna '" << username << "' sebagai '" << tipePeranToString(pengguna->peran) << "'." << std::endl;
        penggunaSaatIniInternal = pengguna;
        return true;
    } else {
        std::cout << "Error: Password salah untuk pengguna '" << username << "'." << std::endl;
        penggunaSaatIniInternal = nullptr;
        return false;
    }
}

void ManajemenPengguna::logoutPengguna() {
    if (penggunaSaatIniInternal != nullptr) {
        std::cout << "Info: Pengguna '" << penggunaSaatIniInternal->username << "' berhasil logout." << std::endl;
        penggunaSaatIniInternal = nullptr;
    } else {
        std::cout << "Info: Tidak ada pengguna yang sedang login." << std::endl;
    }
}

Pengguna* ManajemenPengguna::getPenggunaSaatIni() const {
    return penggunaSaatIniInternal;
}

bool ManajemenPengguna::hapusPengguna(const std::string& usernameAdmin, const std::string& usernameTarget) {
    Pengguna* admin = cariPenggunaInternal(usernameAdmin);
    if (!admin || admin->peran != TipePeran::ADMIN) {
        std::cout << "Error: Hanya admin yang dapat menghapus pengguna." << std::endl;
        return false;
    }

    if (usernameAdmin == usernameTarget) {
        std::cout << "Error: Admin tidak dapat menghapus dirinya sendiri." << std::endl;
        return false;
    }

    Pengguna* target = cariPenggunaInternal(usernameTarget);
    if (!target) {
        std::cout << "Error: Pengguna target '" << usernameTarget << "' tidak ditemukan." << std::endl;
        return false;
    }

    if (target->peran == TipePeran::ADMIN) {
        std::cout << "Error: Admin tidak dapat menghapus admin lain." << std::endl;
        return false;
    }

    daftarPengguna.erase(
        std::remove_if(daftarPengguna.begin(), daftarPengguna.end(),
                       [&](const Pengguna& p) { return p.username == usernameTarget; }),
        daftarPengguna.end());

    std::cout << "Info: Pengguna '" << usernameTarget << "' berhasil dihapus oleh admin '" << usernameAdmin << "'." << std::endl;
    return true;
}

void ManajemenPengguna::tampilkanSemuaPenggunaDebug() const {
    std::cout << "\n--- Daftar Pengguna (Debug dari file: " << namaFilePenggunaInternal << ") ---" << std::endl;
    if (daftarPengguna.empty()) {
        std::cout << "Tidak ada pengguna terdaftar." << std::endl;
    }
    for (const auto& p : daftarPengguna) {
        std::cout << "Username: " << p.username
                  << ", HashedPassword: " << p.hashedPassword
                  << ", Peran: " << tipePeranToString(p.peran) << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
}