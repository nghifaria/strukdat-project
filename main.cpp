// main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <cstdio>
#include <map>

#include "karyawan.h"
#include "linkedlist_karyawan.h"
#include "stack_aksi.h"
#include "pengguna.h"
#include "manajemen_pengguna.h"
#include "hierarki_jabatan.h"
#include "permintaan.h"
#include "queue_permintaan.h"


const std::string NAMA_FILE_KARYAWAN = "karyawan_data.csv";
const std::string NAMA_FILE_PENGGUNA_UTAMA = "pengguna_data.csv";
const std::string ID_CEO_DEFAULT_APP = "";
const std::string NAMA_FILE_ANTRIAN_PERMINTAAN = "permintaan_data.csv";


void tampilkanMenuUtama(ManajemenPengguna& manajerPengguna);
void prosesMenuUtama(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan, QueuePermintaan& antrianPermintaan);
void prosesMenuCRUD(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan);
void prosesMenuTampilkanDanUrutkan(LinkedListKaryawan& daftarKaryawan);
void prosesMenuFilterDanCari(LinkedListKaryawan& daftarKaryawan);
void prosesUndo(LinkedListKaryawan& list, StackAksi& undoStack, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan);
void prosesMenuAdmin(ManajemenPengguna& manajerPengguna, QueuePermintaan& antrianPermintaan);
void prosesMenuHierarki(PohonJabatan& pohonJabatan, const LinkedListKaryawan& daftarKaryawan);
void prosesMenuPermintaanKaryawan(QueuePermintaan& antrianPermintaan, ManajemenPengguna& manajerPengguna, const LinkedListKaryawan& daftarKaryawan);
void tambahKaryawanBaruDariRegistrasi(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, PohonJabatan& pohonJabatan, const std::string& idKaryawanBaru);


void getInputString(const std::string& prompt, std::string& output) {
    std::cout << prompt;
    std::getline(std::cin, output);
}

double getInputGaji(const std::string& prompt) {
    double gaji;
    while (true) {
        std::cout << prompt;
        if (std::cin >> gaji && gaji >= 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return gaji;
        } else {
            std::cout << "Input gaji tidak valid (harus angka non-negatif). Coba lagi." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int getInputPilihan(const std::string& prompt, int minPilihan, int maxPilihan) {
    int pilihan;
    while (true) {
        std::cout << prompt;
        if (std::cin >> pilihan && pilihan >= minPilihan && pilihan <= maxPilihan) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return pilihan;
        } else {
            std::cout << "Input tidak valid. Masukkan angka antara " << minPilihan << " dan " << maxPilihan << "." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void tambahKaryawanBaruDariRegistrasi(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, PohonJabatan& pohonJabatan, const std::string& idKaryawanBaru) {
    Karyawan kOperasi;
    kOperasi.idKaryawan = idKaryawanBaru;

    std::cout << "\n--- Melengkapi Data Karyawan untuk ID: " << idKaryawanBaru << " ---" << std::endl;
    if (daftarKaryawan.cariKaryawanById(kOperasi.idKaryawan) != nullptr) {
        std::cout << "Info: Data karyawan untuk ID '" << kOperasi.idKaryawan << "' sudah ada. Tidak perlu membuat baru." << std::endl;
        return;
    }
    getInputString("Masukkan Nama Karyawan: ", kOperasi.namaKaryawan);
    getInputString("Masukkan Jabatan: ", kOperasi.jabatan);
    kOperasi.gaji = getInputGaji("Masukkan Gaji: ");
    getInputString("Masukkan ID Atasan (kosongkan jika tidak ada/root): ", kOperasi.idAtasan);
    if (kOperasi.idKaryawan == kOperasi.idAtasan && !kOperasi.idAtasan.empty()){
        std::cout << "Peringatan: ID Karyawan dan ID Atasan sama. ID Atasan akan dikosongkan." << std::endl;
        kOperasi.idAtasan = "";
    }
    if (!kOperasi.idAtasan.empty() && daftarKaryawan.cariKaryawanById(kOperasi.idAtasan) == nullptr) {
        std::cout << "Peringatan: ID Atasan '" << kOperasi.idAtasan << "' tidak ditemukan. ID Atasan akan dikosongkan." << std::endl;
        kOperasi.idAtasan = "";
    }

    daftarKaryawan.tambahDiAkhir(kOperasi, stackUndo);
    pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
    std::cout << "Info: Data karyawan untuk '" << kOperasi.namaKaryawan << "' (ID: " << kOperasi.idKaryawan << ") berhasil dibuat." << std::endl;
}


void prosesUndo(LinkedListKaryawan& list, StackAksi& undoStack, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan) {
    if (undoStack.isEmpty()) {
        std::cout << "Tidak ada aksi untuk di-undo." << std::endl;
        return;
    }
    AksiUndo aksiTerakhir = undoStack.pop();

    switch (aksiTerakhir.tipe) {
        case TipeAksi::TAMBAH:
            list.hapusKaryawanById(aksiTerakhir.dataKaryawanUtama.idKaryawan);
            std::cout << "Undo: Karyawan dengan ID " << aksiTerakhir.dataKaryawanUtama.idKaryawan << " telah dihapus (undo tambah)." << std::endl;
            break;
        case TipeAksi::HAPUS:
            list.tambahDiAkhir(aksiTerakhir.dataKaryawanUtama);
            std::cout << "Undo: Karyawan dengan ID " << aksiTerakhir.dataKaryawanUtama.idKaryawan << " telah ditambahkan kembali (undo hapus)." << std::endl;
            break;
        case TipeAksi::UPDATE:
            if (aksiTerakhir.dataKaryawanSebelum.has_value()) {
                list.updateKaryawanById(aksiTerakhir.dataKaryawanUtama.idKaryawan, aksiTerakhir.dataKaryawanSebelum.value());
                std::cout << "Undo: Data Karyawan dengan ID " << aksiTerakhir.dataKaryawanUtama.idKaryawan << " telah dikembalikan ke state sebelumnya (undo update)." << std::endl;
            } else {
                std::cout << "Error undo update: Data sebelumnya tidak tersimpan." << std::endl;
            }
            break;
    }
    pohonJabatan.bangunDariLinkedList(list, ID_CEO_DEFAULT_APP);
}


void tampilkanMenuUtama(ManajemenPengguna& manajerPengguna) {
    std::cout << "\n===== MENU UTAMA - Aplikasi Pengelolaan Data Karyawan =====" << std::endl;
    Pengguna* currentUser = manajerPengguna.getPenggunaSaatIni();
    if (currentUser) {
        std::cout << "Login sebagai: " << currentUser->username << " (" << (currentUser->peran == TipePeran::ADMIN ? "Admin" : "User") << ")" << std::endl;
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
    if (!currentUser) {
        std::cout << "1. Registrasi Pengguna Baru (& Buat Data Karyawan)" << std::endl;
        std::cout << "2. Login Pengguna" << std::endl;
    } else {
        if (currentUser->peran == TipePeran::ADMIN) {
            std::cout << "1. Manajemen Data Karyawan (Tambah, Update, Hapus)" << std::endl;
        }
        std::cout << "2. Tampilkan & Urutkan Data Karyawan" << std::endl;
        std::cout << "3. Filter & Cari Data Karyawan" << std::endl;
        std::cout << "4. Visualisasi Hierarki Jabatan" << std::endl;
        if (currentUser->peran == TipePeran::PENGGUNA) {
            std::cout << "5. Menu Permintaan Saya" << std::endl;
        }
        std::cout << "6. Undo Aksi Terakhir (Karyawan)" << std::endl;
        if (currentUser->peran == TipePeran::ADMIN) {
            std::cout << "7. Menu Admin (Termasuk Proses Permintaan)" << std::endl;
            std::cout << "8. Logout" << std::endl;
        } else {
            std::cout << "7. Logout" << std::endl;
        }
    }
    std::cout << "0. Keluar Aplikasi" << std::endl;
    std::cout << "===========================================================" << std::endl;
}

void tampilkanSubMenuCRUD() {
    std::cout << "\n--- Sub Menu: Manajemen Data Karyawan (CRUD) ---" << std::endl;
    std::cout << "1. Tambah Karyawan Baru (Admin)" << std::endl;
    std::cout << "2. Update Data Karyawan" << std::endl;
    std::cout << "3. Hapus Karyawan" << std::endl;
    std::cout << "0. Kembali ke Menu Utama" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
}

void tampilkanSubMenuTampilkanDanUrutkan() {
    std::cout << "\n--- Sub Menu: Tampilkan & Urutkan Data Karyawan ---" << std::endl;
    std::cout << "1. Tampilkan Semua Karyawan (Tidak Terurut)" << std::endl;
    std::cout << "2. Tampilkan Terurut berdasarkan Nama (Ascending)" << std::endl;
    std::cout << "3. Tampilkan Terurut berdasarkan Nama (Descending)" << std::endl;
    std::cout << "4. Tampilkan Terurut berdasarkan Gaji (Ascending)" << std::endl;
    std::cout << "5. Tampilkan Terurut berdasarkan Gaji (Descending)" << std::endl;
    std::cout << "0. Kembali ke Menu Utama" << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
}

void tampilkanSubMenuFilterDanCari() {
    std::cout << "\n--- Sub Menu: Filter & Cari Data Karyawan ---" << std::endl;
    std::cout << "1. Filter Berdasarkan Gaji" << std::endl;
    std::cout << "2. Filter Berdasarkan Jabatan" << std::endl;
    std::cout << "3. Cari Berdasarkan Nama (Substring)" << std::endl;
    std::cout << "0. Kembali ke Menu Utama" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

void tampilkanSubMenuAdmin(QueuePermintaan& antrianPermintaan) {
    std::cout << "\n--- Sub Menu: Admin ---" << std::endl;
    std::cout << "1. Tampilkan Semua Pengguna (Debug)" << std::endl;
    std::cout << "2. Registrasi Pengguna Baru (Admin/User) oleh Admin" << std::endl;
    std::cout << "3. Hapus Pengguna (User)" << std::endl;
    std::cout << "4. Lihat & Proses Antrian Permintaan Aktif (" << antrianPermintaan.size() << " total)" << std::endl;
    std::cout << "5. Lihat Semua Histori Permintaan" << std::endl;
    std::cout << "0. Kembali ke Menu Utama" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

void tampilkanSubMenuHierarki() {
    std::cout << "\n--- Sub Menu: Hierarki Jabatan ---" << std::endl;
    std::cout << "1. Tampilkan Seluruh Hierarki" << std::endl;
    std::cout << "2. Cari Bawahan Langsung" << std::endl;
    std::cout << "3. Cari Semua Bawahan (Rekursif)" << std::endl;
    std::cout << "4. Tampilkan Jalur ke Puncak Hierarki" << std::endl;
    std::cout << "0. Kembali ke Menu Utama" << std::endl;
    std::cout << "------------------------------------" << std::endl;
}

void tampilkanSubMenuPermintaanUser() {
    std::cout << "\n--- Sub Menu: Permintaan Saya ---" << std::endl;
    std::cout << "1. Ajukan Permintaan Baru" << std::endl;
    std::cout << "2. Lihat Status Semua Permintaan Saya" << std::endl;
    std::cout << "0. Kembali ke Menu Utama" << std::endl;
    std::cout << "------------------------------------" << std::endl;
}


int main() {
    ManajemenPengguna manajerPenggunaUtama(NAMA_FILE_PENGGUNA_UTAMA);
    LinkedListKaryawan daftarKaryawanUtama;
    StackAksi stackUndoUtama;
    PohonJabatan pohonJabatanUtama;
    QueuePermintaan antrianPermintaanUtama(NAMA_FILE_ANTRIAN_PERMINTAAN);

    std::cout << "\n===== Selamat Datang di Aplikasi Pengelolaan Data Karyawan =====" << std::endl;
    if (daftarKaryawanUtama.muatDariFile(NAMA_FILE_KARYAWAN)) {
         pohonJabatanUtama.bangunDariLinkedList(daftarKaryawanUtama, ID_CEO_DEFAULT_APP);
    } else {
        std::cout << "Memulai dengan daftar karyawan kosong karena file '" << NAMA_FILE_KARYAWAN << "' tidak ditemukan atau gagal dimuat." << std::endl;
    }


    prosesMenuUtama(daftarKaryawanUtama, stackUndoUtama, manajerPenggunaUtama, pohonJabatanUtama, antrianPermintaanUtama);

    if (!daftarKaryawanUtama.simpanKeFile(NAMA_FILE_KARYAWAN)) {
        std::cerr << "Peringatan: Gagal menyimpan data karyawan ke file!" << std::endl;
    }
    antrianPermintaanUtama.simpanSemuaKeFile();
    std::cout << "Aplikasi ditutup." << std::endl;

    return 0;
}

void prosesMenuUtama(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan, QueuePermintaan& antrianPermintaan) {
    int pilihanUtama = -1;
    std::string inputUsername, inputPassword;

    do {
        tampilkanMenuUtama(manajerPengguna);
        Pengguna* currentUser = manajerPengguna.getPenggunaSaatIni();
        int maxPilihan = 0;
        if (!currentUser) {
            maxPilihan = 2;
        } else if (currentUser->peran == TipePeran::ADMIN) {
            maxPilihan = 8;
        } else {
            maxPilihan = 7;
        }
        pilihanUtama = getInputPilihan("Masukkan pilihan: ", 0, maxPilihan );

        if (!currentUser) {
            switch (pilihanUtama) {
                case 1:
                    std::cout << "--- Registrasi Pengguna Baru & Buat Data Karyawan ---" << std::endl;
                    getInputString("Masukkan Username baru (akan menjadi ID Karyawan): ", inputUsername);
                    getInputString("Masukkan Password baru: ", inputPassword);
                    if (manajerPengguna.registrasiPenggunaBaruStandar(inputUsername, inputPassword)) {
                        tambahKaryawanBaruDariRegistrasi(daftarKaryawan, stackUndo, pohonJabatan, inputUsername);
                    }
                    break;
                case 2:
                    std::cout << "--- Login Pengguna ---" << std::endl;
                    getInputString("Username: ", inputUsername);
                    getInputString("Password: ", inputPassword);
                    manajerPengguna.loginPengguna(inputUsername, inputPassword);
                    break;
                case 0:
                    std::cout << "Keluar dari aplikasi..." << std::endl;
                    break;
                default:
                    std::cout << "Pilihan tidak valid." << std::endl;
            }
        } else {
            if (currentUser->peran == TipePeran::ADMIN) {
                switch (pilihanUtama) {
                    case 1: prosesMenuCRUD(daftarKaryawan, stackUndo, manajerPengguna, pohonJabatan); break;
                    case 2: prosesMenuTampilkanDanUrutkan(daftarKaryawan); break;
                    case 3: prosesMenuFilterDanCari(daftarKaryawan); break;
                    case 4: prosesMenuHierarki(pohonJabatan, daftarKaryawan); break;
                    case 5: 
                        std::cout << "Admin tidak dapat mengajukan permintaan melalui menu ini. Gunakan Menu Admin untuk manajemen." << std::endl;
                        break;
                    case 6: prosesUndo(daftarKaryawan, stackUndo, manajerPengguna, pohonJabatan); break;
                    case 7: prosesMenuAdmin(manajerPengguna, antrianPermintaan); break;
                    case 8: manajerPengguna.logoutPengguna(); break;
                    case 0: std::cout << "Keluar dari aplikasi..." << std::endl; break;
                    default: std::cout << "Pilihan tidak valid." << std::endl;
                }
            } else { // Peran PENGGUNA
                 switch (pilihanUtama) {
                    
                    case 2: prosesMenuTampilkanDanUrutkan(daftarKaryawan); break;
                    case 3: prosesMenuFilterDanCari(daftarKaryawan); break;
                    case 4: prosesMenuHierarki(pohonJabatan, daftarKaryawan); break;
                    case 5: prosesMenuPermintaanKaryawan(antrianPermintaan, manajerPengguna, daftarKaryawan); break;
                    case 6: prosesUndo(daftarKaryawan, stackUndo, manajerPengguna, pohonJabatan); break;
                    case 7: manajerPengguna.logoutPengguna(); break;
                    case 0: std::cout << "Keluar dari aplikasi..." << std::endl; break;
                    default: std::cout << "Pilihan tidak valid." << std::endl;
                }
                 if (pilihanUtama == 1) { 
                    std::cout << "Akses ditolak. Hanya Admin yang dapat mengelola data karyawan." << std::endl;
                }
            }
        }
    } while (pilihanUtama != 0);
}

void prosesMenuCRUD(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan) {
    Pengguna* currentUser = manajerPengguna.getPenggunaSaatIni();
    if (!currentUser || currentUser->peran != TipePeran::ADMIN) {
        std::cout << "Akses ditolak. Hanya Admin yang dapat mengakses menu ini." << std::endl;
        return;
    }

    int pilihanCRUD = -1;
    Karyawan kOperasi;
    std::string idKaryawanOperasi;

    do {
        tampilkanSubMenuCRUD();
        pilihanCRUD = getInputPilihan("Masukkan pilihan: ", 0, 3);

        switch (pilihanCRUD) {
            case 1: {
                std::cout << "--- Tambah Karyawan Baru (Admin) ---" << std::endl;
                getInputString("Masukkan ID Karyawan: ", kOperasi.idKaryawan);
                if (daftarKaryawan.cariKaryawanById(kOperasi.idKaryawan) != nullptr) {
                    std::cout << "Error: Karyawan dengan ID '" << kOperasi.idKaryawan << "' sudah ada." << std::endl;
                    break;
                }
                getInputString("Masukkan Nama Karyawan: ", kOperasi.namaKaryawan);
                getInputString("Masukkan Jabatan: ", kOperasi.jabatan);
                kOperasi.gaji = getInputGaji("Masukkan Gaji: ");
                getInputString("Masukkan ID Atasan (kosongkan jika tidak ada/root): ", kOperasi.idAtasan);
                if (kOperasi.idKaryawan == kOperasi.idAtasan && !kOperasi.idAtasan.empty()){
                    std::cout << "Error: ID Karyawan dan ID Atasan tidak boleh sama." << std::endl;
                    break;
                }
                if (!kOperasi.idAtasan.empty() && daftarKaryawan.cariKaryawanById(kOperasi.idAtasan) == nullptr) {
                    std::cout << "Error: ID Atasan '" << kOperasi.idAtasan << "' tidak ditemukan. Karyawan tidak ditambahkan." << std::endl;
                    break;
                }

                daftarKaryawan.tambahDiAkhir(kOperasi, stackUndo);
                pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
                break;
            }
            case 2: {
                std::cout << "--- Update Data Karyawan ---" << std::endl;
                getInputString("Masukkan ID Karyawan yang akan diupdate: ", idKaryawanOperasi);
                NodeLL* nodeAda = daftarKaryawan.cariKaryawanById(idKaryawanOperasi);
                if (nodeAda != nullptr) {
                    Karyawan dataLama = nodeAda->dataKaryawan;
                    Karyawan kUpdate = dataLama;
                    std::string inputBuffer;

                    std::cout << "Data saat ini: Nama=" << dataLama.namaKaryawan << ", Jabatan=" << dataLama.jabatan << ", Gaji=" << std::fixed << std::setprecision(2) << dataLama.gaji << ", ID Atasan=" << (dataLama.idAtasan.empty() ? "-" : dataLama.idAtasan) << std::endl;

                    getInputString("Masukkan Nama Baru (kosongkan jika tidak diubah '" + dataLama.namaKaryawan + "'): ", inputBuffer);
                    if (!inputBuffer.empty()) kUpdate.namaKaryawan = inputBuffer;

                    getInputString("Masukkan Jabatan Baru (kosongkan jika tidak diubah '" + dataLama.jabatan + "'): ", inputBuffer);
                    if (!inputBuffer.empty()) kUpdate.jabatan = inputBuffer;

                    std::cout << "Masukkan Gaji Baru (masukkan angka >= 0, atau -1 jika tidak diubah [" << std::fixed << std::setprecision(2) << dataLama.gaji << "]): ";
                    double gajiInputDouble;
                    std::string line;
                    std::getline(std::cin, line);
                    if (!line.empty()) {
                        try {
                            gajiInputDouble = std::stod(line);
                            if (gajiInputDouble >= 0) {
                                kUpdate.gaji = gajiInputDouble;
                            } else if (gajiInputDouble == -1) {
                            } else {
                                std::cout << "Input gaji tidak valid (harus >= 0 atau -1). Gaji tidak diubah." << std::endl;
                            }
                        } catch (const std::exception& e) {
                             std::cout << "Input gaji tidak valid. Gaji tidak diubah." << std::endl;
                        }
                    }

                    getInputString("Masukkan ID Atasan Baru (kosongkan jika tidak diubah, ketik '-' untuk hapus ID Atasan): ", inputBuffer);
                    if (inputBuffer == "-") {
                        kUpdate.idAtasan = "";
                    } else if (!inputBuffer.empty()) {
                         if (inputBuffer == kUpdate.idKaryawan) {
                            std::cout << "Error: Karyawan tidak bisa menjadi atasan dirinya sendiri. ID Atasan tidak diubah." << std::endl;
                         } else if (daftarKaryawan.cariKaryawanById(inputBuffer) == nullptr) {
                            std::cout << "Error: ID Atasan baru '" << inputBuffer << "' tidak ditemukan. ID Atasan tidak diubah." << std::endl;
                         } else {
                            kUpdate.idAtasan = inputBuffer;
                         }
                    }


                    daftarKaryawan.updateKaryawanById(idKaryawanOperasi, kUpdate, stackUndo);
                    pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
                } else {
                    std::cout << "Karyawan dengan ID '" << idKaryawanOperasi << "' tidak ditemukan." << std::endl;
                }
                break;
            }
            case 3: {
                std::cout << "--- Hapus Karyawan ---" << std::endl;
                getInputString("Masukkan ID Karyawan yang akan dihapus: ", idKaryawanOperasi);
                daftarKaryawan.hapusKaryawanById(idKaryawanOperasi, stackUndo);
                pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
                break;
            }
            case 0:
                std::cout << "Kembali ke Menu Utama..." << std::endl;
                break;
            default:
                std::cout << "Pilihan tidak valid." << std::endl;
        }
    } while (pilihanCRUD != 0);
}


void prosesMenuTampilkanDanUrutkan(LinkedListKaryawan& daftarKaryawan) {
    int pilihanTampil = -1;
    do {
        tampilkanSubMenuTampilkanDanUrutkan();
        pilihanTampil = getInputPilihan("Masukkan pilihan: ", 0, 5);

        switch (pilihanTampil) {
            case 1: daftarKaryawan.tampilkanSemua(); break;
            case 2: daftarKaryawan.tampilkanDataTerurut(true, true); break;
            case 3: daftarKaryawan.tampilkanDataTerurut(true, false); break;
            case 4: daftarKaryawan.tampilkanDataTerurut(false, true); break;
            case 5: daftarKaryawan.tampilkanDataTerurut(false, false); break;
            case 0: std::cout << "Kembali ke Menu Utama..." << std::endl; break;
            default: std::cout << "Pilihan tidak valid." << std::endl;
        }
    } while (pilihanTampil != 0);
}

void prosesMenuFilterDanCari(LinkedListKaryawan& daftarKaryawan) {
    int pilihanFilter = -1;
    std::string inputString;
    do {
        tampilkanSubMenuFilterDanCari();
        pilihanFilter = getInputPilihan("Masukkan pilihan: ", 0, 3);

        switch (pilihanFilter) {
            case 1: {
                double batasGajiInput = getInputGaji("Masukkan batas gaji: ");
                int pilihanLebihBesar = getInputPilihan("Filter untuk gaji (1: Lebih Besar atau Sama Dengan, 0: Kurang Dari): ", 0, 1);
                daftarKaryawan.tampilkanKaryawanBerdasarkanGaji(batasGajiInput, (pilihanLebihBesar == 1));
                break;
            }
            case 2: {
                std::cout << "--- Filter Karyawan Berdasarkan Jabatan ---" << std::endl;
                std::vector<std::string> daftarJabatan = daftarKaryawan.getDaftarJabatanUnik();
                if (daftarJabatan.empty()) {
                    std::cout << "Tidak ada jabatan tersedia untuk difilter." << std::endl;
                    break;
                }
                std::cout << "Daftar Jabatan Tersedia:" << std::endl;
                for (size_t i = 0; i < daftarJabatan.size(); ++i) {
                    std::cout << i + 1 << ". " << daftarJabatan[i] << std::endl;
                }
                int nomorJabatan = getInputPilihan("Masukkan nomor jabatan yang ingin difilter (atau 0 untuk batal): ", 0, daftarJabatan.size());
                if (nomorJabatan > 0) {
                    std::string jabatanDipilih = daftarJabatan[nomorJabatan - 1];
                    daftarKaryawan.tampilkanKaryawanBerdasarkanJabatan(jabatanDipilih);
                }
                break;
            }
            case 3: {
                std::cout << "--- Cari Karyawan Berdasarkan Nama ---" << std::endl;
                getInputString("Masukkan sebagian atau seluruh nama karyawan: ", inputString);
                daftarKaryawan.cariKaryawanBerdasarkanNama(inputString);
                break;
            }
            case 0:
                std::cout << "Kembali ke Menu Utama..." << std::endl;
                break;
            default:
                std::cout << "Pilihan tidak valid." << std::endl;
        }
    } while (pilihanFilter != 0);
}

void prosesMenuAdmin(ManajemenPengguna& manajerPengguna, QueuePermintaan& antrianPermintaan) {
    int pilihanAdmin = -1;
    std::string usernameBaru, passwordBaru, usernameTarget;
    int pilihanPeranInt;
    TipePeran peranBaru;
    Pengguna* adminSaatIni = manajerPengguna.getPenggunaSaatIni();

    if (!adminSaatIni || adminSaatIni->peran != TipePeran::ADMIN) {
        std::cout << "Akses ditolak. Hanya admin yang dapat mengakses menu ini." << std::endl;
        return;
    }

    do {
        tampilkanSubMenuAdmin(antrianPermintaan);
        pilihanAdmin = getInputPilihan("Masukkan pilihan Admin: ", 0, 5);
        switch (pilihanAdmin) {
            case 1:
                manajerPengguna.tampilkanSemuaPenggunaDebug();
                break;
            case 2:
                std::cout << "--- Registrasi Pengguna Baru oleh Admin ---" << std::endl;
                getInputString("Masukkan Username baru: ", usernameBaru);
                getInputString("Masukkan Password baru: ", passwordBaru);
                pilihanPeranInt = getInputPilihan("Pilih Peran (0 untuk User, 1 untuk Admin): ", 0, 1);
                peranBaru = (pilihanPeranInt == 1) ? TipePeran::ADMIN : TipePeran::PENGGUNA;
                manajerPengguna.registrasiPenggunaOlehAdmin(usernameBaru, passwordBaru, peranBaru);
                break;
            case 3:
                std::cout << "--- Hapus Pengguna (User) ---" << std::endl;
                getInputString("Masukkan username pengguna yang akan dihapus: ", usernameTarget);
                manajerPengguna.hapusPengguna(adminSaatIni->username, usernameTarget);
                break;
            case 4:
                {
                    std::cout << "--- Proses Antrian Permintaan Aktif ---" << std::endl;
                    antrianPermintaan.tampilkanSemuaPermintaan(true);
                    NodeQueue* front = antrianPermintaan.getFrontNode();
                    bool adaPermintaanAktifUntukDiproses = false;
                    while(front != nullptr){
                        if(front->dataPermintaan.status == StatusPermintaan::DIAJUKAN || front->dataPermintaan.status == StatusPermintaan::DIPROSES){
                            adaPermintaanAktifUntukDiproses = true;
                            break;
                        }
                        front = front->next;
                    }

                    if (!adaPermintaanAktifUntukDiproses) {
                         std::cout << "Tidak ada permintaan aktif untuk diproses saat ini." << std::endl;
                         break;
                    }

                    std::string idPermintaanProses;
                    getInputString("Masukkan ID Permintaan yang akan diproses (atau kosongkan untuk batal): ", idPermintaanProses);
                    if (!idPermintaanProses.empty()) {
                        int pilihanStatus = getInputPilihan("Pilih status baru (1: Diproses, 2: Disetujui, 3: Ditolak): ", 1, 3);
                        StatusPermintaan statusBaru;
                        switch (pilihanStatus) {
                            case 1: statusBaru = StatusPermintaan::DIPROSES; break;
                            case 2: statusBaru = StatusPermintaan::DISETUJUI; break;
                            case 3: statusBaru = StatusPermintaan::DITOLAK; break;
                            default: std::cout << "Pilihan status tidak valid." << std::endl; continue;
                        }
                        std::string catatan;
                        getInputString("Masukkan catatan admin (opsional): ", catatan);
                        antrianPermintaan.updateStatusPermintaan(idPermintaanProses, statusBaru, catatan);
                    }
                }
                break;
            case 5:
                std::cout << "--- Semua Histori Permintaan ---" << std::endl;
                antrianPermintaan.tampilkanSemuaPermintaan(false);
                break;
            case 0:
                std::cout << "Kembali ke Menu Utama..." << std::endl;
                break;
            default:
                std::cout << "Pilihan Admin tidak valid." << std::endl;
        }
    } while (pilihanAdmin != 0);
}

void prosesMenuHierarki(PohonJabatan& pohonJabatan, const LinkedListKaryawan& daftarKaryawan) {
    int pilihanHierarki = -1;
    std::string idKaryawanInput;

    pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);


    do {
        tampilkanSubMenuHierarki();
        pilihanHierarki = getInputPilihan("Pilih Opsi Hierarki: ", 0, 4);
        switch (pilihanHierarki) {
            case 1:
                pohonJabatan.tampilkanSeluruhHierarki();
                break;
            case 2:
                getInputString("Masukkan ID Karyawan (Atasan): ", idKaryawanInput);
                {
                    std::vector<Karyawan> bawahan = pohonJabatan.cariBawahanLangsung(idKaryawanInput);
                    if (bawahan.empty() && pohonJabatan.petaNodeContainsKey(idKaryawanInput)) {
                        std::cout << "Karyawan dengan ID " << idKaryawanInput << " tidak memiliki bawahan langsung." << std::endl;
                    } else if (!bawahan.empty()){
                        std::cout << "Bawahan Langsung dari ID " << idKaryawanInput << ":" << std::endl;
                        for (const auto& k : bawahan) {
                            std::cout << "- " << k.namaKaryawan << " (" << k.jabatan << " - ID: " << k.idKaryawan << ")" << std::endl;
                        }
                    }
                }
                break;
            case 3:
                getInputString("Masukkan ID Karyawan (Atasan): ", idKaryawanInput);
                {
                    std::vector<Karyawan> semuaBawahan = pohonJabatan.cariSemuaBawahan(idKaryawanInput);
                     if (semuaBawahan.empty() && pohonJabatan.petaNodeContainsKey(idKaryawanInput)) {
                        std::cout << "Karyawan dengan ID " << idKaryawanInput << " tidak memiliki bawahan (rekursif)." << std::endl;
                    } else if(!semuaBawahan.empty()) {
                        std::cout << "Semua Bawahan (Rekursif) dari ID " << idKaryawanInput << ":" << std::endl;
                        for (const auto& k : semuaBawahan) {
                            std::cout << "- " << k.namaKaryawan << " (" << k.jabatan << " - ID: " << k.idKaryawan << ")" << std::endl;
                        }
                    }
                }
                break;
            case 4:
                getInputString("Masukkan ID Karyawan: ", idKaryawanInput);
                {
                    std::vector<Karyawan> jalur = pohonJabatan.getJalurKePuncak(idKaryawanInput);
                    if (!jalur.empty()) {
                        std::cout << "Jalur dari " << idKaryawanInput << " ke Puncak Hierarki:" << std::endl;
                        for (size_t i = 0; i < jalur.size(); ++i) {
                            std::cout << jalur[i].namaKaryawan << (i < jalur.size() - 1 ? " -> " : "");
                        }
                        std::cout << std::endl;
                    }
                }
                break;
            case 0:
                std::cout << "Kembali ke Menu Utama..." << std::endl;
                break;
            default:
                std::cout << "Pilihan tidak valid." << std::endl;
        }

    } while (pilihanHierarki != 0);
}

void prosesMenuPermintaanKaryawan(QueuePermintaan& antrianPermintaan, ManajemenPengguna& manajerPengguna, const LinkedListKaryawan& daftarKaryawan) {
    Pengguna* penggunaSaatIni = manajerPengguna.getPenggunaSaatIni();
    if (!penggunaSaatIni) {
        std::cout << "Error: Anda harus login untuk mengakses menu ini." << std::endl;
        return;
    }

    std::string idKaryawanUntukPermintaan = penggunaSaatIni->username;
    if (penggunaSaatIni->peran == TipePeran::PENGGUNA && daftarKaryawan.cariKaryawanById(idKaryawanUntukPermintaan) == nullptr) {
         std::cout << "Error: Data karyawan untuk username Anda ('" << idKaryawanUntukPermintaan << "') tidak ditemukan sebagai ID Karyawan." << std::endl;
         std::cout << "Pastikan ID Karyawan Anda sama dengan username login Anda dan terdaftar di sistem data karyawan." << std::endl;
         std::cout << "Atau, minta Admin untuk membuatkan data karyawan Anda." << std::endl;
         return;
    }


    int pilihanMenu = -1;
    do {
        tampilkanSubMenuPermintaanUser();
        pilihanMenu = getInputPilihan("Pilih Opsi Permintaan: ", 0, 2);
        switch (pilihanMenu) {
            case 1: {
                std::cout << "--- Ajukan Permintaan Baru ---" << std::endl;
                std::string tipe, detail;
                getInputString("Masukkan tipe permintaan (misal: Cuti, Reimbursement, Ubah Data): ", tipe);
                getInputString("Masukkan detail permintaan: ", detail);
                if (tipe.empty() || detail.empty()) {
                    std::cout << "Error: Tipe dan detail permintaan tidak boleh kosong." << std::endl;
                } else {
                    Permintaan pBaru(idKaryawanUntukPermintaan, tipe, detail);
                    antrianPermintaan.enqueue(pBaru);
                    std::cout << "Info: Permintaan Anda (ID: " << pBaru.idPermintaanInternal << ") telah diajukan." << std::endl;
                }
                break;
            }
            case 2: {
                std::cout << "\n--- Status Semua Permintaan Saya (" << idKaryawanUntukPermintaan << ") ---" << std::endl;
                bool adaPermintaan = false;
                NodeQueue* current = antrianPermintaan.getFrontNode();
                 std::cout << std::left << std::setw(10) << "ID Req"
                          << std::setw(15) << "ID Karyawan"
                          << std::setw(20) << "Tipe Permintaan"
                          << std::setw(30) << "Detail"
                          << std::setw(20) << "Timestamp"
                          << std::setw(15) << "Status"
                          << std::setw(30) << "Catatan Admin" << std::endl;
                std::cout << "--------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
                while(current != nullptr) {
                    if (current->dataPermintaan.idKaryawanPengaju == idKaryawanUntukPermintaan) {
                        std::cout << std::left << std::setw(10) << current->dataPermintaan.idPermintaanInternal
                                  << std::setw(15) << current->dataPermintaan.idKaryawanPengaju
                                  << std::setw(20) << current->dataPermintaan.tipePermintaan
                                  << std::setw(30) << current->dataPermintaan.detailPermintaan
                                  << std::setw(20) << current->dataPermintaan.timestampPengajuan
                                  << std::setw(15) << statusPermintaanToString(current->dataPermintaan.status)
                                  << std::setw(30) << current->dataPermintaan.catatanAdmin << std::endl;
                        adaPermintaan = true;
                    }
                    current = current->next;
                }
                if (!adaPermintaan) {
                    std::cout << "Anda belum mengajukan permintaan apapun atau tidak ada permintaan untuk ID Karyawan tersebut." << std::endl;
                }
                 std::cout << "--------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
                break;
            }
            case 0:
                std::cout << "Kembali ke Menu Utama..." << std::endl;
                break;
            default:
                std::cout << "Pilihan tidak valid." << std::endl;
        }
    } while (pilihanMenu != 0);
}