#include "manajemen_pengguna.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

ManajemenPengguna::ManajemenPengguna(const std::string& namaFile) : namaFilePenggunaInternal(namaFile), penggunaSaatIniInternal(nullptr) {
    muatPenggunaDariFile();
    if (daftarPengguna.empty()) {
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

std::string ManajemenPengguna::encryptCaesar(const std::string& text, int shift) {
    std::string result = "";
    for (char c : text) {
        if (std::isalpha(c)) {
            char base = std::isupper(c) ? 'A' : 'a';
            result += static_cast<char>((c - base + shift) % 26 + base);
        } else {
            result += c;
        }
    }
    return result;
}

std::string ManajemenPengguna::decryptCaesar(const std::string& text, int shift) {
    return encryptCaesar(text, 26 - shift); 
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
    if (peranLower == "pengguna" || peranLower == "karyawan") {
        return TipePeran::PENGGUNA;
    }
    return TipePeran::USER;
}

std::string ManajemenPengguna::tipePeranToString(TipePeran peran) const {
    switch (peran) {
        case TipePeran::ADMIN:
            return "ADMIN";
        case TipePeran::PENGGUNA:
            return "KARYAWAN";
        case TipePeran::USER:
            return "USER";
        default:
            return "USER";
    }
}

void ManajemenPengguna::muatPenggunaDariFile() {
    std::ifstream file(namaFilePenggunaInternal);
    if (!file.is_open()) {
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
        TipePeran peranVal = TipePeran::USER;
        if (std::getline(ss, username, ',') && std::getline(ss, hashedPasswordStr, ',') && std::getline(ss, peranStr)) {
            try {
                hashedPasswordVal = std::stoul(hashedPasswordStr);
                peranVal = stringToTipePeran(peranStr);
                daftarPengguna.emplace_back(username, hashedPasswordVal, peranVal);
            } catch (const std::invalid_argument& ia) {
            } catch (const std::out_of_range& oor) {
            }
        }
    }
    file.close();
}

void ManajemenPengguna::simpanSemuaPenggunaKeFile() const {
    std::ofstream file(namaFilePenggunaInternal);
    if (!file.is_open()) {
        return;
    }
    for (const auto& pengguna : daftarPengguna) {
        file << pengguna.username << "," << pengguna.hashedPassword << "," << tipePeranToString(pengguna.peran) << "\n";
    }
    file.close();
}

bool ManajemenPengguna::registrasiUserPublik(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        std::cout << "Error: Username dan password tidak boleh kosong." << std::endl;
        return false;
    }
    if (this->cariPenggunaInternal(username) != nullptr) {
        std::cout << "Error: Username '" << username << "' sudah terdaftar." << std::endl;
        return false;
    }
    std::string encryptedPassword = encryptCaesar(password, CAESAR_SHIFT);
    daftarPengguna.emplace_back(username, ManajemenPengguna::hashPasswordSederhana(encryptedPassword), TipePeran::USER);
    simpanSemuaPenggunaKeFile();
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
    std::string encryptedPassword = encryptCaesar(password, CAESAR_SHIFT);
    daftarPengguna.emplace_back(username, ManajemenPengguna::hashPasswordSederhana(encryptedPassword), peran);
    return true;
}

bool ManajemenPengguna::loginPengguna(const std::string& username, const std::string& password) {
    Pengguna* pengguna = this->cariPenggunaInternal(username);
    if (pengguna == nullptr) {
        penggunaSaatIniInternal = nullptr;
        return false;
    }
    std::string encryptedInputPassword = encryptCaesar(password, CAESAR_SHIFT);
    if (pengguna->hashedPassword == ManajemenPengguna::hashPasswordSederhana(encryptedInputPassword)) {
        penggunaSaatIniInternal = pengguna;
        return true;
    } else {
        penggunaSaatIniInternal = nullptr;
        return false;
    }
}

void ManajemenPengguna::logoutPengguna() {
    if (penggunaSaatIniInternal != nullptr) {
        penggunaSaatIniInternal = nullptr;
    }
}

Pengguna* ManajemenPengguna::getPenggunaSaatIni() const {
    return penggunaSaatIniInternal;
}

bool ManajemenPengguna::updateUserRole(const std::string& username, TipePeran peranBaru) {
    Pengguna* pengguna = cariPenggunaInternal(username);
    if (pengguna == nullptr) {
        return false;
    }
    pengguna->peran = peranBaru;
    simpanSemuaPenggunaKeFile();
    return true;
}

bool ManajemenPengguna::hapusPengguna(const std::string& usernameAdmin, const std::string& usernameTarget) {
    Pengguna* admin = cariPenggunaInternal(usernameAdmin);
    if (!admin || admin->peran != TipePeran::ADMIN) {
        return false;
    }
    if (usernameAdmin == usernameTarget) {
        return false;
    }
    Pengguna* target = cariPenggunaInternal(usernameTarget);
    if (!target) {
        return false;
    }
    if (target->peran == TipePeran::ADMIN) {
        return false;
    }
    daftarPengguna.erase(
        std::remove_if(daftarPengguna.begin(), daftarPengguna.end(),
                       [&](const Pengguna& p) { return p.username == usernameTarget; }),
        daftarPengguna.end());
    simpanSemuaPenggunaKeFile();
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