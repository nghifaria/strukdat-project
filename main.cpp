// main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <cstdio>

#include "karyawan.h"
#include "linkedlist_karyawan.h"
#include "stack_aksi.h"
#include "pengguna.h"
#include "manajemen_pengguna.h"


const std::string NAMA_FILE_KARYAWAN = "karyawan_data.csv";
const std::string NAMA_FILE_PENGGUNA_UTAMA = "pengguna_data.csv";


void tampilkanMenuUtama(ManajemenPengguna& manajerPengguna);
void prosesMenuUtama(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna);
void prosesMenuCRUD(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna);
void prosesMenuTampilkanDanUrutkan(LinkedListKaryawan& daftarKaryawan);
void prosesMenuFilterDanCari(LinkedListKaryawan& daftarKaryawan);
void prosesUndo(LinkedListKaryawan& list, StackAksi& undoStack, ManajemenPengguna& manajerPengguna);
void prosesMenuAdmin(ManajemenPengguna& manajerPengguna);


void getInputString(const std::string& prompt, std::string& output) {
    std::cout << prompt;
    std::getline(std::cin, output);
}

double getInputGaji(const std::string& prompt) {
    double gaji;
    while (true) {
        std::cout << prompt;
        if (std::cin >> gaji) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return gaji;
        } else {
            std::cout << "Input gaji tidak valid (harus angka). Coba lagi." << std::endl;
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

void prosesUndo(LinkedListKaryawan& list, StackAksi& undoStack, ManajemenPengguna& manajerPengguna) {
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
}

void tampilkanMenuUtama(ManajemenPengguna& manajerPengguna) {
    std::cout << "\n===== MENU UTAMA - Aplikasi Pengelolaan Data Karyawan =====" << std::endl;
    Pengguna* currentUser = manajerPengguna.getPenggunaSaatIni();
    if (currentUser) {
        std::cout << "Login sebagai: " << currentUser->username << " (" << (currentUser->peran == TipePeran::ADMIN ? "Admin" : "User") << ")" << std::endl;
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
    if (!currentUser) {
        std::cout << "1. Registrasi Pengguna Baru" << std::endl;
        std::cout << "2. Login Pengguna" << std::endl;
    } else {
        std::cout << "1. Manajemen Data Karyawan (Tambah, Update, Hapus)" << std::endl;
        std::cout << "2. Tampilkan & Urutkan Data Karyawan" << std::endl;
        std::cout << "3. Filter & Cari Data Karyawan" << std::endl;
        std::cout << "4. Undo Aksi Terakhir (Karyawan)" << std::endl;
        if (currentUser->peran == TipePeran::ADMIN) {
            std::cout << "5. Menu Admin" << std::endl;
            std::cout << "6. Logout" << std::endl;
        } else {
            std::cout << "5. Logout" << std::endl;
        }
    }
    std::cout << "0. Keluar Aplikasi" << std::endl;
    std::cout << "===========================================================" << std::endl;
}

void tampilkanSubMenuCRUD() {
    std::cout << "\n--- Sub Menu: Manajemen Data Karyawan (CRUD) ---" << std::endl;
    std::cout << "1. Tambah Karyawan Baru" << std::endl;
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

void tampilkanSubMenuAdmin() {
    std::cout << "\n--- Sub Menu: Admin ---" << std::endl;
    std::cout << "1. Tampilkan Semua Pengguna (Debug)" << std::endl;
    std::cout << "2. Registrasi Pengguna Baru (Admin/User)" << std::endl;
    std::cout << "0. Kembali ke Menu Utama" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}


int main() {
    ManajemenPengguna manajerPenggunaUtama(NAMA_FILE_PENGGUNA_UTAMA);
    LinkedListKaryawan daftarKaryawanUtama;
    StackAksi stackUndoUtama;

    std::cout << "\n===== Selamat Datang di Aplikasi Pengelolaan Data Karyawan =====" << std::endl;
    if (!daftarKaryawanUtama.muatDariFile(NAMA_FILE_KARYAWAN)) {
        std::cout << "Memulai dengan daftar karyawan kosong karena file '" << NAMA_FILE_KARYAWAN << "' tidak ditemukan atau gagal dimuat." << std::endl;
    }

    prosesMenuUtama(daftarKaryawanUtama, stackUndoUtama, manajerPenggunaUtama);

    if (!daftarKaryawanUtama.simpanKeFile(NAMA_FILE_KARYAWAN)) {
        std::cerr << "Peringatan: Gagal menyimpan data karyawan ke file!" << std::endl;
    }
    std::cout << "Aplikasi ditutup." << std::endl;

    return 0;
}

void prosesMenuUtama(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna) {
    int pilihanUtama = -1;
    std::string inputUsername, inputPassword;

    do {
        tampilkanMenuUtama(manajerPengguna);
        Pengguna* currentUser = manajerPengguna.getPenggunaSaatIni();
        int maxPilihan = 0;
        if (!currentUser) {
            maxPilihan = 2;
        } else if (currentUser->peran == TipePeran::ADMIN) {
            maxPilihan = 6;
        } else {
            maxPilihan = 5;
        }
        pilihanUtama = getInputPilihan("Masukkan pilihan: ", 0, maxPilihan );

        if (!currentUser) {
            switch (pilihanUtama) {
                case 1:
                    std::cout << "--- Registrasi Pengguna Baru (Peran Default: User) ---" << std::endl;
                    getInputString("Masukkan Username baru: ", inputUsername);
                    getInputString("Masukkan Password baru: ", inputPassword);
                    manajerPengguna.registrasiPenggunaBaru(inputUsername, inputPassword, TipePeran::PENGGUNA);
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
                    case 1: prosesMenuCRUD(daftarKaryawan, stackUndo, manajerPengguna); break;
                    case 2: prosesMenuTampilkanDanUrutkan(daftarKaryawan); break;
                    case 3: prosesMenuFilterDanCari(daftarKaryawan); break;
                    case 4: prosesUndo(daftarKaryawan, stackUndo, manajerPengguna); break;
                    case 5: prosesMenuAdmin(manajerPengguna); break;
                    case 6: manajerPengguna.logoutPengguna(); break;
                    case 0: std::cout << "Keluar dari aplikasi..." << std::endl; break;
                    default: std::cout << "Pilihan tidak valid." << std::endl;
                }
            } else { // Peran PENGGUNA
                 switch (pilihanUtama) {
                    case 1: prosesMenuCRUD(daftarKaryawan, stackUndo, manajerPengguna); break;
                    case 2: prosesMenuTampilkanDanUrutkan(daftarKaryawan); break;
                    case 3: prosesMenuFilterDanCari(daftarKaryawan); break;
                    case 4: prosesUndo(daftarKaryawan, stackUndo, manajerPengguna); break;
                    case 5: manajerPengguna.logoutPengguna(); break;
                    case 0: std::cout << "Keluar dari aplikasi..." << std::endl; break;
                    default: std::cout << "Pilihan tidak valid." << std::endl;
                }
            }
        }
    } while (pilihanUtama != 0);
}

void prosesMenuCRUD(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna) {
    int pilihanCRUD = -1;
    Karyawan kOperasi;
    std::string idKaryawanOperasi;

    do {
        tampilkanSubMenuCRUD();
        pilihanCRUD = getInputPilihan("Masukkan pilihan: ", 0, 3);

        switch (pilihanCRUD) {
            case 1: {
                std::cout << "--- Tambah Karyawan Baru ---" << std::endl;
                getInputString("Masukkan ID Karyawan: ", kOperasi.idKaryawan);
                if (daftarKaryawan.cariKaryawanById(kOperasi.idKaryawan) != nullptr) {
                    std::cout << "Error: Karyawan dengan ID '" << kOperasi.idKaryawan << "' sudah ada." << std::endl;
                    break;
                }
                getInputString("Masukkan Nama Karyawan: ", kOperasi.namaKaryawan);
                getInputString("Masukkan Jabatan: ", kOperasi.jabatan);
                kOperasi.gaji = getInputGaji("Masukkan Gaji: ");
                daftarKaryawan.tambahDiAkhir(kOperasi, stackUndo);
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

                    std::cout << "Data saat ini: Nama=" << dataLama.namaKaryawan << ", Jabatan=" << dataLama.jabatan << ", Gaji=" << std::fixed << std::setprecision(2) << dataLama.gaji << std::endl;

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
                            if (gajiInputDouble != -1) {
                                kUpdate.gaji = gajiInputDouble;
                            }
                        } catch (const std::exception& e) {
                            std::cout << "Input gaji tidak valid atau tidak diubah. Menggunakan gaji lama." << std::endl;
                        }
                    }
                    daftarKaryawan.updateKaryawanById(idKaryawanOperasi, kUpdate, stackUndo);
                } else {
                    std::cout << "Karyawan dengan ID '" << idKaryawanOperasi << "' tidak ditemukan." << std::endl;
                }
                break;
            }
            case 3: {
                std::cout << "--- Hapus Karyawan ---" << std::endl;
                getInputString("Masukkan ID Karyawan yang akan dihapus: ", idKaryawanOperasi);
                daftarKaryawan.hapusKaryawanById(idKaryawanOperasi, stackUndo);
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

void prosesMenuAdmin(ManajemenPengguna& manajerPengguna) {
    int pilihanAdmin = -1;
    std::string usernameBaru, passwordBaru;
    int pilihanPeranInt;
    TipePeran peranBaru;

    do {
        tampilkanSubMenuAdmin();
        pilihanAdmin = getInputPilihan("Masukkan pilihan Admin: ", 0, 2);
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
                manajerPengguna.registrasiPenggunaBaru(usernameBaru, passwordBaru, peranBaru);
                break;
            case 0:
                std::cout << "Kembali ke Menu Utama..." << std::endl;
                break;
            default:
                std::cout << "Pilihan Admin tidak valid." << std::endl;
        }
    } while (pilihanAdmin != 0);
}