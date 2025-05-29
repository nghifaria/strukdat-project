// main.cpp
#include <iostream>
#include "karyawan.h"
#include "linkedlist_karyawan.h" 
#include "stack_aksi.h" 
#include "pengguna.h"
#include "manajemen_pengguna.h"
#include <string>   
#include <limits>   
#include <vector>   
#include <cstdio> // Untuk std::remove (jika ada file tes yang ingin dihapus di awal)

const std::string NAMA_FILE_KARYAWAN = "karyawan_data.csv";
const std::string NAMA_FILE_PENGGUNA_UTAMA = "pengguna_data.csv";

std::string usernameSaatIni = ""; 

void tampilkanMenuAplikasi(); 
void prosesUndo(LinkedListKaryawan& list, StackAksi& undoStack); 

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

void tampilkanMenuAplikasi() {
    std::cout << "\n===== Aplikasi Pengelolaan Data Karyawan =====" << std::endl;
    if (!usernameSaatIni.empty()) { 
        std::cout << "Login sebagai: " << usernameSaatIni << std::endl;
        std::cout << "----------------------------------------------" << std::endl;
    }
    std::cout << "Perintah:" << std::endl;
    if (usernameSaatIni.empty()) { 
        std::cout << "10. Registrasi Pengguna Baru" << std::endl;
        std::cout << "11. Login Pengguna" << std::endl;
    } else {
        std::cout << "1. Tambah Karyawan" << std::endl;
        std::cout << "2. Tampilkan Semua Karyawan" << std::endl;
        std::cout << "3. Update Data Karyawan" << std::endl;
        std::cout << "4. Hapus Karyawan by ID" << std::endl;
        std::cout << "5. Tampilkan Karyawan Terurut berdasarkan Nama (Asc)" << std::endl;
        std::cout << "6. Tampilkan Karyawan Terurut berdasarkan Nama (Desc)" << std::endl;
        std::cout << "7. Tampilkan Karyawan Terurut berdasarkan Gaji (Asc)" << std::endl;
        std::cout << "8. Tampilkan Karyawan Terurut berdasarkan Gaji (Desc)" << std::endl;
        std::cout << "9. Undo Aksi Terakhir" << std::endl;
        std::cout << "12. Filter Karyawan Berdasarkan Gaji" << std::endl; 
        std::cout << "13. Filter Karyawan Berdasarkan Jabatan" << std::endl;
        std::cout << "14. Cari Karyawan Berdasarkan Nama" << std::endl;
        std::cout << "15. Logout" << std::endl; 
    }
    std::cout << "0. Keluar Aplikasi" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Masukkan perintah: ";
}

int main() {
    ManajemenPengguna manajerPenggunaUtama(NAMA_FILE_PENGGUNA_UTAMA);

    std::cout << "\n===== Selamat Datang di Aplikasi Pengelolaan Data Karyawan =====" << std::endl;
    
    LinkedListKaryawan daftarKaryawanUtama;
    StackAksi stackUndoUtama; 
    
    if (!daftarKaryawanUtama.muatDariFile(NAMA_FILE_KARYAWAN)) {
        std::cout << "Memulai dengan daftar karyawan kosong karena file '" << NAMA_FILE_KARYAWAN << "' tidak ditemukan atau gagal dimuat." << std::endl;
    }

    int pilihan = -1; 
    std::string inputString1, inputString2; 

    do {
        tampilkanMenuAplikasi();
        if (!(std::cin >> pilihan)) { 
            std::cout << "Input tidak valid. Hanya angka yang diizinkan untuk pilihan menu." << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            pilihan = -99; 
            continue; 
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        if (usernameSaatIni.empty()) { 
             switch (pilihan) {
                case 10: { 
                    std::cout << "--- Registrasi Pengguna Baru ---" << std::endl;
                    std::cout << "Masukkan Username baru: "; 
                    std::getline(std::cin, inputString1);
                    std::cout << "Masukkan Password baru: "; 
                    std::getline(std::cin, inputString2); 
                    manajerPenggunaUtama.registrasiPenggunaBaru(inputString1, inputString2);
                    break;
                }
                case 11: { 
                    std::cout << "--- Login Pengguna ---" << std::endl;
                    std::cout << "Username: "; 
                    std::getline(std::cin, inputString1); 
                    std::cout << "Password: "; 
                    std::getline(std::cin, inputString2); 
                    if (manajerPenggunaUtama.loginPengguna(inputString1, inputString2)) {
                        usernameSaatIni = inputString1;
                    }
                    break;
                }
                case 0:
                    if (!daftarKaryawanUtama.simpanKeFile(NAMA_FILE_KARYAWAN)) {
                         std::cerr << "Peringatan: Gagal menyimpan data karyawan ke file!" << std::endl;
                    }
                    std::cout << "Keluar dari aplikasi." << std::endl;
                    break;
                default:
                    std::cout << "Pilihan tidak valid atau Anda belum login untuk mengakses fitur tersebut." << std::endl;
            }
        } else { 
            switch (pilihan) {
                case 1: { 
                    Karyawan kBaru; 
                    std::cout << "--- Tambah Karyawan (Login sebagai: " << usernameSaatIni << ") ---" << std::endl;
                    std::cout << "Masukkan ID Karyawan: "; 
                    std::getline(std::cin, kBaru.idKaryawan); 
                    std::cout << "Masukkan Nama Karyawan: "; 
                    std::getline(std::cin, kBaru.namaKaryawan); 
                    std::cout << "Masukkan Jabatan: "; 
                    std::getline(std::cin, kBaru.jabatan); 
                    std::cout << "Masukkan Gaji: "; 
                    while(!(std::cin >> kBaru.gaji)){ 
                        std::cout << "Input gaji tidak valid (harus angka). Masukkan Gaji: "; 
                        std::cin.clear(); 
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    daftarKaryawanUtama.tambahDiAkhir(kBaru, stackUndoUtama); 
                    break;
                }
                case 2: 
                    daftarKaryawanUtama.tampilkanSemua();
                    break;
                case 3: { 
                    Karyawan kUpdate; 
                    std::cout << "--- Update Data Karyawan (Login sebagai: " << usernameSaatIni << ") ---" << std::endl;
                    std::cout << "Masukkan ID Karyawan yang akan diupdate: "; 
                    std::getline(std::cin, inputString1); 
                    NodeLL* nodeAda = daftarKaryawanUtama.cariKaryawanById(inputString1);
                    if (nodeAda != nullptr) {
                        kUpdate.idKaryawan = inputString1; 
                        std::cout << "Masukkan Nama Baru (ID: " << inputString1 << "): "; 
                        std::getline(std::cin, kUpdate.namaKaryawan);
                        std::cout << "Masukkan Jabatan Baru: "; 
                        std::getline(std::cin, kUpdate.jabatan);
                        std::cout << "Masukkan Gaji Baru: "; 
                        while(!(std::cin >> kUpdate.gaji)){
                            std::cout << "Input gaji tidak valid (harus angka). Masukkan Gaji Baru: "; 
                            std::cin.clear(); 
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        daftarKaryawanUtama.updateKaryawanById(inputString1, kUpdate, stackUndoUtama);
                    } else { 
                        std::cout << "Karyawan dengan ID '" << inputString1 << "' tidak ditemukan." << std::endl;
                    }
                    break;
                }
                case 4: { 
                    std::cout << "--- Hapus Karyawan (Login sebagai: " << usernameSaatIni << ") ---" << std::endl;
                    std::cout << "Masukkan ID Karyawan yang akan dihapus: "; 
                    std::getline(std::cin, inputString1); 
                    daftarKaryawanUtama.hapusKaryawanById(inputString1, stackUndoUtama);
                    break;
                }
                case 5: daftarKaryawanUtama.tampilkanDataTerurut(true, true); break;  
                case 6: daftarKaryawanUtama.tampilkanDataTerurut(true, false); break; 
                case 7: daftarKaryawanUtama.tampilkanDataTerurut(false, true); break; 
                case 8: daftarKaryawanUtama.tampilkanDataTerurut(false, false); break;
                case 9: 
                    prosesUndo(daftarKaryawanUtama, stackUndoUtama);
                    break;
                case 12: { 
                    double batasGajiInput;
                    int pilihanFilterGaji;
                    std::cout << "--- Filter Karyawan Berdasarkan Gaji ---" << std::endl;
                    std::cout << "Masukkan batas gaji: ";
                    while(!(std::cin >> batasGajiInput)) {
                        std::cout << "Input batas gaji tidak valid. Masukkan angka: ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Filter untuk gaji (1: Lebih Besar atau Sama Dengan, 0: Kurang Dari): ";
                    while(!(std::cin >> pilihanFilterGaji) || (pilihanFilterGaji != 0 && pilihanFilterGaji != 1) ) {
                        std::cout << "Input pilihan filter tidak valid. Masukkan 0 atau 1: ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    daftarKaryawanUtama.tampilkanKaryawanBerdasarkanGaji(batasGajiInput, (pilihanFilterGaji == 1));
                    break;
                }
                case 13: { 
                    std::cout << "--- Filter Karyawan Berdasarkan Jabatan ---" << std::endl;
                    std::vector<std::string> daftarJabatan = daftarKaryawanUtama.getDaftarJabatanUnik();
                    if (daftarJabatan.empty()) {
                        std::cout << "Tidak ada jabatan tersedia untuk difilter." << std::endl;
                        break;
                    }
                    std::cout << "Daftar Jabatan Tersedia:" << std::endl;
                    for (size_t i = 0; i < daftarJabatan.size(); ++i) {
                        std::cout << i + 1 << ". " << daftarJabatan[i] << std::endl;
                    }
                    std::cout << "Masukkan nomor jabatan yang ingin difilter: ";
                    int nomorJabatan;
                    while (!(std::cin >> nomorJabatan) || nomorJabatan < 1 || nomorJabatan > static_cast<int>(daftarJabatan.size())) {
                        std::cout << "Input nomor jabatan tidak valid. Pilih dari 1 sampai " << daftarJabatan.size() << ": ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    std::string jabatanDipilih = daftarJabatan[nomorJabatan - 1];
                    daftarKaryawanUtama.tampilkanKaryawanBerdasarkanJabatan(jabatanDipilih);
                    break;
                }
                case 14: { 
                    std::cout << "--- Cari Karyawan Berdasarkan Nama ---" << std::endl;
                    std::cout << "Masukkan sebagian atau seluruh nama karyawan: ";
                    std::string namaDicari;
                    std::getline(std::cin, namaDicari); 
                    daftarKaryawanUtama.cariKaryawanBerdasarkanNama(namaDicari);
                    break;
                }
                case 15: 
                    std::cout << "Pengguna '" << usernameSaatIni << "' berhasil logout." << std::endl;
                    usernameSaatIni = ""; 
                    break;
                case 0:
                    if (!daftarKaryawanUtama.simpanKeFile(NAMA_FILE_KARYAWAN)) {
                         std::cerr << "Peringatan: Gagal menyimpan data karyawan!" << std::endl;
                    }
                    std::cout << "Keluar dari aplikasi." << std::endl;
                    break;
                default:
                    std::cout << "Pilihan tidak valid." << std::endl;
            }
        }
    } while (pilihan != 0);

    return 0;
}