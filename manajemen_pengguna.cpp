// manajemen_pengguna.cpp
#include "manajemen_pengguna.h"
#include <iostream>
#include <fstream>  
#include <sstream>  
#include <stdexcept>

ManajemenPengguna::ManajemenPengguna(const std::string& namaFile) : namaFilePenggunaInternal(namaFile) {
    muatPenggunaDariFile(); 
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

void ManajemenPengguna::muatPenggunaDariFile() {
    std::ifstream file(namaFilePenggunaInternal); 
    if (!file.is_open()) {
        std::cout << "Info: File data pengguna '" << namaFilePenggunaInternal << "' tidak ditemukan. Membuat file baru atau memulai dengan daftar pengguna kosong." << std::endl;
        return;
    }
    daftarPengguna.clear(); 
    std::string baris;
    while (std::getline(file, baris)) {
        std::stringstream ss(baris);
        std::string username;
        std::string hashedPasswordStr;
        unsigned long hashedPasswordVal; 
        if (std::getline(ss, username, ',') && std::getline(ss, hashedPasswordStr)) {
            try {
                hashedPasswordVal = std::stoul(hashedPasswordStr); 
                daftarPengguna.push_back({username, hashedPasswordVal});
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Peringatan: Format hashed password tidak valid di file pengguna untuk user: " << username << " Baris: " << baris << " Detail: " << ia.what() << std::endl;
            } catch (const std::out_of_range& oor) {
                std::cerr << "Peringatan: Nilai hashed password di luar jangkauan untuk user: " << username << " Baris: " << baris << " Detail: " << oor.what() << std::endl;
            }
        } else {
             if (!baris.empty()) { 
                std::cerr << "Peringatan: Baris tidak valid di file pengguna: " << baris << std::endl;
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
        file << pengguna.username << "," << pengguna.hashedPassword << "\n";
    }
    file.close();
    std::cout << "Info: Data pengguna berhasil disimpan ke '" << namaFilePenggunaInternal << "'." << std::endl;
}

bool ManajemenPengguna::registrasiPenggunaBaru(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        std::cout << "Error: Username dan password tidak boleh kosong." << std::endl;
        return false;
    }
    if (this->cariPenggunaInternal(username) != nullptr) { 
        std::cout << "Error: Username '" << username << "' sudah terdaftar." << std::endl;
        return false;
    }
    Pengguna penggunaBaru;
    penggunaBaru.username = username;
    penggunaBaru.hashedPassword = ManajemenPengguna::hashPasswordSederhana(password); 
    daftarPengguna.push_back(penggunaBaru);
    std::cout << "Info: Pengguna '" << username << "' berhasil diregistrasi." << std::endl;
    return true;
}

bool ManajemenPengguna::loginPengguna(const std::string& username, const std::string& password) {
    Pengguna* pengguna = this->cariPenggunaInternal(username); 
    if (pengguna == nullptr) {
        std::cout << "Error: Username '" << username << "' tidak ditemukan." << std::endl;
        return false;
    }
    if (pengguna->hashedPassword == ManajemenPengguna::hashPasswordSederhana(password)) { 
        std::cout << "Info: Login berhasil untuk pengguna '" << username << "'." << std::endl;
        return true;
    } else {
        std::cout << "Error: Password salah untuk pengguna '" << username << "'." << std::endl;
        return false;
    }
}

void ManajemenPengguna::tampilkanSemuaPenggunaDebug() const {
    std::cout << "\n--- Daftar Pengguna (Debug dari file: " << namaFilePenggunaInternal << ") ---" << std::endl;
    if (daftarPengguna.empty()) {
        std::cout << "Tidak ada pengguna terdaftar." << std::endl;
    }
    for (const auto& p : daftarPengguna) {
        std::cout << "Username: " << p.username << ", HashedPassword: " << p.hashedPassword << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
}