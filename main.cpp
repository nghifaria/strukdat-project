// main.cpp
#include <iostream>
#include <string>   
#include <vector>   
#include <limits>   
#include <iomanip> 
#include <cstdio> // Untuk std::remove jika ada file tes yang ingin dihapus di awal (sekarang tidak dipakai)

#include "karyawan.h"
#include "linkedlist_karyawan.h" 
#include "stack_aksi.h" 
#include "pengguna.h"
#include "manajemen_pengguna.h"

const std::string NAMA_FILE_KARYAWAN = "karyawan_data.csv";
const std::string NAMA_FILE_PENGGUNA_UTAMA = "pengguna_data.csv";

std::string usernameSaatIni = ""; 

void tampilkanMenuUtama();
void prosesMenuUtama(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna);
void prosesMenuCRUD(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo);
void prosesMenuTampilkanDanUrutkan(LinkedListKaryawan& daftarKaryawan);
void prosesMenuFilterDanCari(LinkedListKaryawan& daftarKaryawan);
void prosesUndo(LinkedListKaryawan& list, StackAksi& undoStack); 

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

void prosesUndo(LinkedListKaryawan& list, StackAksi& undoStack) {
    if (undoStack.isEmpty()) {
        std::cout << "Tidak ada aksi untuk di-undo." << std::endl;
        return;
    }
    AksiUndo aksiTerakhir = undoStack.pop();
    StackAksi dummyStack; 
    switch (aksiTerakhir.tipe) {
        case TipeAksi::TAMBAH:
            list.hapusKaryawanById(aksiTerakhir.dataKaryawanUtama.idKaryawan, dummyStack); 
            std::cout << "Undo: Karyawan dengan ID " << aksiTerakhir.dataKaryawanUtama.idKaryawan << " telah dihapus (undo tambah)." << std::endl;
            break;
        case TipeAksi::HAPUS:
            list.tambahDiAkhir(aksiTerakhir.dataKaryawanUtama, dummyStack); 
            std::cout << "Undo: Karyawan dengan ID " << aksiTerakhir.dataKaryawanUtama.idKaryawan << " telah ditambahkan kembali (undo hapus)." << std::endl;
            break;
        case TipeAksi::UPDATE:
            if (aksiTerakhir.dataKaryawanSebelum.has_value()) {
                list.updateKaryawanById(aksiTerakhir.dataKaryawanUtama.idKaryawan, aksiTerakhir.dataKaryawanSebelum.value(), dummyStack); 
                std::cout << "Undo: Data Karyawan dengan ID " << aksiTerakhir.dataKaryawanUtama.idKaryawan << " telah dikembalikan ke state sebelumnya (undo update)." << std::endl;
            } else {
                std::cout << "Error undo update: Data sebelumnya tidak tersimpan." << std::endl;
            }
            break;
    }
}

void tampilkanMenuUtama() {
    std::cout << "\n===== MENU UTAMA - Aplikasi Pengelolaan Data Karyawan =====" << std::endl;
    if (!usernameSaatIni.empty()) { 
        std::cout << "Login sebagai: " << usernameSaatIni << std::endl;
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
    if (usernameSaatIni.empty()) { 
        std::cout << "1. Registrasi Pengguna Baru" << std::endl;
        std::cout << "2. Login Pengguna" << std::endl;
    } else {
        std::cout << "1. Manajemen Data Karyawan (Tambah, Update, Hapus)" << std::endl;
        std::cout << "2. Tampilkan & Urutkan Data Karyawan" << std::endl;
        std::cout << "3. Filter & Cari Data Karyawan" << std::endl;
        std::cout << "4. Undo Aksi Terakhir" << std::endl;
        std::cout << "5. Logout" << std::endl; 
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
        tampilkanMenuUtama();
        pilihanUtama = getInputPilihan("Masukkan pilihan: ", 0, (usernameSaatIni.empty() ? 2 : 5) );

        if (usernameSaatIni.empty()) {
            switch (pilihanUtama) {
                case 1:
                    std::cout << "--- Registrasi Pengguna Baru ---" << std::endl;
                    getInputString("Masukkan Username baru: ", inputUsername);
                    getInputString("Masukkan Password baru: ", inputPassword); 
                    manajerPengguna.registrasiPenggunaBaru(inputUsername, inputPassword);
                    break;
                case 2:
                    std::cout << "--- Login Pengguna ---" << std::endl;
                    getInputString("Username: ", inputUsername); 
                    getInputString("Password: ", inputPassword); 
                    if (manajerPengguna.loginPengguna(inputUsername, inputPassword)) {
                        usernameSaatIni = inputUsername;
                    }
                    break;
                case 0:
                    std::cout << "Keluar dari aplikasi..." << std::endl;
                    break;
                default:
                    std::cout << "Pilihan tidak valid." << std::endl;
            }
        } else { 
            switch (pilihanUtama) {
                case 1:
                    prosesMenuCRUD(daftarKaryawan, stackUndo);
                    break;
                case 2:
                    prosesMenuTampilkanDanUrutkan(daftarKaryawan);
                    break;
                case 3:
                    prosesMenuFilterDanCari(daftarKaryawan);
                    break;
                case 4:
                    prosesUndo(daftarKaryawan, stackUndo);
                    break;
                case 5:
                    std::cout << "Pengguna '" << usernameSaatIni << "' berhasil logout." << std::endl;
                    usernameSaatIni = ""; 
                    break;
                case 0:
                    std::cout << "Keluar dari aplikasi..." << std::endl;
                    break;
                default:
                    std::cout << "Pilihan tidak valid." << std::endl;
            }
        }
    } while (pilihanUtama != 0);
}

void prosesMenuCRUD(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo) {
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
                    
                    getInputString("Masukkan Nama Baru (kosongkan jika tidak diubah): ", inputBuffer);
                    if (!inputBuffer.empty()) kUpdate.namaKaryawan = inputBuffer;

                    getInputString("Masukkan Jabatan Baru (kosongkan jika tidak diubah): ", inputBuffer);
                    if (!inputBuffer.empty()) kUpdate.jabatan = inputBuffer;
                    
                    std::cout << "Masukkan Gaji Baru (masukkan angka, atau -1 jika tidak diubah): "; 
                    double gajiInputDouble;
                    if (std::cin >> gajiInputDouble) {
                        if (gajiInputDouble != -1) {
                            kUpdate.gaji = gajiInputDouble;
                        }
                    } else {
                        std::cout << "Input gaji tidak diubah atau tidak valid." << std::endl;
                        std::cin.clear(); 
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

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