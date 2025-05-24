// manajemen_pengguna.cpp
#include "manajemen_pengguna.h"
#include <iostream> // Untuk output

ManajemenPengguna::ManajemenPengguna() {
    // Konstruktor
}

// Fungsi hash sederhana untuk string password
// Ini adalah contoh, BUKAN untuk keamanan produksi!
unsigned long ManajemenPengguna::hashPasswordSederhana(const std::string& password) {
    unsigned long hash = 5381; // Nilai awal umum untuk djb2
    for (char c : password) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
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

bool ManajemenPengguna::registrasiPenggunaBaru(const std::string& username, const std::string& password) {
    if (username.empty() || password.empty()) {
        std::cout << "Error: Username dan password tidak boleh kosong." << std::endl;
        return false;
    }
    if (cariPenggunaInternal(username) != nullptr) {
        std::cout << "Error: Username '" << username << "' sudah terdaftar." << std::endl;
        return false;
    }

    Pengguna penggunaBaru;
    penggunaBaru.username = username;
    penggunaBaru.hashedPassword = hashPasswordSederhana(password);
    daftarPengguna.push_back(penggunaBaru);
    std::cout << "Info: Pengguna '" << username << "' berhasil diregistrasi." << std::endl;
    return true;
}

bool ManajemenPengguna::loginPengguna(const std::string& username, const std::string& password) {
    Pengguna* pengguna = cariPenggunaInternal(username);
    if (pengguna == nullptr) {
        std::cout << "Error: Username '" << username << "' tidak ditemukan." << std::endl;
        return false;
    }

    if (pengguna->hashedPassword == hashPasswordSederhana(password)) {
        std::cout << "Info: Login berhasil untuk pengguna '" << username << "'." << std::endl;
        return true;
    } else {
        std::cout << "Error: Password salah untuk pengguna '" << username << "'." << std::endl;
        return false;
    }
}

void ManajemenPengguna::tampilkanSemuaPenggunaDebug() const {
    std::cout << "\n--- Daftar Pengguna (Debug) ---" << std::endl;
    if (daftarPengguna.empty()) {
        std::cout << "Tidak ada pengguna terdaftar." << std::endl;
    }
    for (const auto& p : daftarPengguna) {
        std::cout << "Username: " << p.username << ", HashedPassword: " << p.hashedPassword << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
}