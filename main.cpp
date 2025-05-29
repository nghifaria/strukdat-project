// main.cpp
#include <iostream>
#include "karyawan.h"
#include "linkedlist_karyawan.h" 
#include "stack_aksi.h" 
#include "pengguna.h"
#include "manajemen_pengguna.h"
#include <cassert>
#include <string>   
#include <limits>   
#include <fstream>
#include <cstdio>

const std::string NAMA_FILE_KARYAWAN = "karyawan_data.csv";
const std::string NAMA_FILE_PENGGUNA_UTAMA = "pengguna_data.csv";
const std::string NAMA_FILE_PENGGUNA_TES = "pengguna_tes_data.csv"; // File khusus untuk tes


// --- Deklarasi Variabel Global ---
std::string usernameSaatIni = ""; // Status login pengguna saat ini

// --- Deklarasi Prototipe Fungsi ---
void tampilkanMenuAplikasi(); // Prototipe untuk tampilkanMenuAplikasi
void prosesUndo(LinkedListKaryawan& list, StackAksi& undoStack); // Prototipe untuk prosesUndo

// --- Fungsi Tes ---
void tesStructKaryawan() {
    std::cout << "\n--- Menjalankan Tes Struct Karyawan ---" << std::endl;
    Karyawan k1;
    k1.idKaryawan = "K001";
    k1.namaKaryawan = "Andi Wijaya";
    k1.jabatan = "Manager";
    k1.gaji = 20000000.00;
    assert(k1.idKaryawan == "K001");
    assert(k1.namaKaryawan == "Andi Wijaya");
    std::cout << "Tes Struct Karyawan: Lolos" << std::endl;
    std::cout << "--- Tes Struct Karyawan Selesai ---\n" << std::endl;
}

void tesLinkedListKaryawan() { 
    std::cout << "--- Menjalankan Tes LinkedList Karyawan (Non-Undo Operations) ---" << std::endl;
    LinkedListKaryawan listSaya;
    assert(listSaya.isEmpty() == true && listSaya.getJumlahNode() == 0);
    std::cout << "Test 1 (Initial state): Lolos" << std::endl;

    Karyawan kar1_test = {"TL001", "Test Budi", "Test Staf", 5000000};
    Karyawan kar2_test = {"TL002", "Test Citra", "Test SPV", 7000000};
    listSaya.tambahDiAkhir(kar1_test); 
    listSaya.tambahDiAkhir(kar2_test);
    assert(listSaya.getJumlahNode() == 2);
    std::cout << "Test 2 (Add multiple): Lolos" << std::endl;

    NodeLL* hasilCari_test = listSaya.cariKaryawanById("TL002");
    assert(hasilCari_test != nullptr && hasilCari_test->dataKaryawan.namaKaryawan == "Test Citra");
    std::cout << "Test 3 (Cari TL002): Lolos" << std::endl;

    Karyawan kar2_updated_test = {"TL002", "Test Citra Updated", "Senior SPV", 8500000};
    assert(listSaya.updateKaryawanById("TL002", kar2_updated_test) == true); 
    hasilCari_test = listSaya.cariKaryawanById("TL002");
    assert(hasilCari_test != nullptr && hasilCari_test->dataKaryawan.namaKaryawan == "Test Citra Updated");
    std::cout << "Test 4 (Update TL002): Lolos" << std::endl;

    assert(listSaya.hapusKaryawanById("TL002") == true); 
    assert(listSaya.getJumlahNode() == 1);
    std::cout << "Test 5 (Hapus TL002): Lolos" << std::endl;
    
    std::cout << "--- Tes LinkedList Karyawan (Non-Undo) Selesai ---" << std::endl;
}

// Definisi prosesUndo sekarang bisa diletakkan setelah tesFiturUndo jika prototipe sudah ada
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


void tesFiturUndo() {
    std::cout << "\n--- Menjalankan Tes Fitur Undo ---" << std::endl;
    LinkedListKaryawan listTest;
    StackAksi stackUndoTest;
    Karyawan k1 = {"U001", "Undo User 1", "Dev", 6000000};
    Karyawan k1_updated = {"U001", "Undo User 1 Updated", "Senior Dev", 6500000};

    // 1. Test Undo Tambah
    listTest.tambahDiAkhir(k1, stackUndoTest); 
    assert(listTest.getJumlahNode() == 1);
    assert(!stackUndoTest.isEmpty() && stackUndoTest.peek().tipe == TipeAksi::TAMBAH);
    prosesUndo(listTest, stackUndoTest); // Sekarang prosesUndo sudah dideklarasikan
    assert(listTest.isEmpty() == true);
    assert(stackUndoTest.isEmpty() == true); 
    std::cout << "Test Undo Tambah: Lolos" << std::endl;

    // 2. Test Undo Hapus
    listTest.tambahDiAkhir(k1, stackUndoTest); 
    if(!stackUndoTest.isEmpty()) stackUndoTest.pop(); 
    
    listTest.hapusKaryawanById("U001", stackUndoTest); 
    assert(listTest.isEmpty() == true);
    assert(!stackUndoTest.isEmpty() && stackUndoTest.peek().tipe == TipeAksi::HAPUS);
    assert(stackUndoTest.peek().dataKaryawanUtama.idKaryawan == "U001");
    prosesUndo(listTest, stackUndoTest); 
    assert(listTest.getJumlahNode() == 1 && listTest.cariKaryawanById("U001") != nullptr);
    assert(stackUndoTest.isEmpty() == true);
    std::cout << "Test Undo Hapus: Lolos" << std::endl;

    // 3. Test Undo Update
    listTest.updateKaryawanById("U001", k1_updated, stackUndoTest); 
    NodeLL* nodeDiupdate = listTest.cariKaryawanById("U001");
    assert(nodeDiupdate != nullptr && nodeDiupdate->dataKaryawan.namaKaryawan == "Undo User 1 Updated");
    assert(!stackUndoTest.isEmpty() && stackUndoTest.peek().tipe == TipeAksi::UPDATE);
    assert(stackUndoTest.peek().dataKaryawanUtama.namaKaryawan == "Undo User 1 Updated");
    assert(stackUndoTest.peek().dataKaryawanSebelum.value().namaKaryawan == "Undo User 1"); 

    prosesUndo(listTest, stackUndoTest); 
    NodeLL* nodeSetelahUndoUpdate = listTest.cariKaryawanById("U001");
    assert(nodeSetelahUndoUpdate != nullptr && nodeSetelahUndoUpdate->dataKaryawan.namaKaryawan == "Undo User 1");
    assert(stackUndoTest.isEmpty() == true);
    std::cout << "Test Undo Update: Lolos" << std::endl;
    
    listTest.hapusKaryawanById("U001", stackUndoTest); 
    if(!stackUndoTest.isEmpty()) stackUndoTest.pop(); 

    std::cout << "--- Tes Fitur Undo Selesai ---" << std::endl;
}

void tesManajemenPengguna() {
    std::cout << "\n--- Menjalankan Tes Manajemen Pengguna & Hashing ---" << std::endl;
    
    // Hapus file tes sebelumnya agar tes selalu bersih
    std::remove(NAMA_FILE_PENGGUNA_TES.c_str()); 

    ManajemenPengguna mpTes(NAMA_FILE_PENGGUNA_TES); // Gunakan file tes

    // Test 1: Registrasi pengguna baru
    assert(mpTes.registrasiPenggunaBaru("admin_tes", "pass123") == true);
    std::cout << "Test 1.1 (Registrasi admin_tes): Lolos" << std::endl;
    
    // Test 2: Registrasi pengguna dengan username yang sama (harus gagal)
    assert(mpTes.registrasiPenggunaBaru("admin_tes", "adminpass") == false);
    std::cout << "Test 2.1 (Registrasi duplikat username admin_tes): Lolos" << std::endl;

    // Test 3: Login berhasil
    assert(mpTes.loginPengguna("admin_tes", "pass123") == true);
    std::cout << "Test 3.1 (Login admin_tes - berhasil): Lolos" << std::endl;

    // Test 4: Login gagal - password salah
    assert(mpTes.loginPengguna("admin_tes", "salahpass") == false);
    std::cout << "Test 4.1 (Login admin_tes - password salah): Lolos" << std::endl;

    // Test 5: Login gagal - username tidak ada
    assert(mpTes.loginPengguna("penggunabaru_tes", "pass") == false);
    std::cout << "Test 5.1 (Login username tidak ada): Lolos" << std::endl;
    
    mpTes.tampilkanSemuaPenggunaDebug(); 
    // Destructor mpTes akan dipanggil saat keluar scope, menyimpan ke NAMA_FILE_PENGGUNA_TES

    std::cout << "--- Tes Manajemen Pengguna & Hashing Selesai ---" << std::endl;
}



void tampilkanMenuAplikasi() { // Pastikan menu ini sudah terupdate
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
        std::cout << "12. Filter Karyawan Berdasarkan Gaji" << std::endl; // Nomor menu mungkin perlu disesuaikan
        std::cout << "13. Filter Karyawan Berdasarkan Jabatan" << std::endl;
        std::cout << "14. Cari Karyawan Berdasarkan Nama" << std::endl;
        std::cout << "15. Logout" << std::endl; 
    }
    std::cout << "0. Keluar Aplikasi" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "Masukkan perintah: ";
}

void tesOperasiFile() {
    std::cout << "\n--- Menjalankan Tes Operasi File ---" << std::endl;
    LinkedListKaryawan listFileTest;
    StackAksi dummyUndoStack; // Untuk operasi yang memerlukannya tapi tidak ingin dicatat saat tes

    Karyawan kf1 = {"F001", "File User A", "Admin File", 7000000};
    Karyawan kf2 = {"F002", "File User B", "Support File", 7500000};
    
    listFileTest.tambahDiAkhir(kf1, dummyUndoStack); // Gunakan versi dengan stack (atau buat versi tes khusus)
    listFileTest.tambahDiAkhir(kf2, dummyUndoStack);

    // Test 1: Simpan ke File
    assert(listFileTest.simpanKeFile("test_data.csv") == true);
    std::cout << "Test 1 (Simpan ke File): Lolos (cek file test_data.csv)" << std::endl;

    // Test 2: Muat dari File
    LinkedListKaryawan listMuatTest;
    assert(listMuatTest.muatDariFile("test_data.csv") == true); // Gunakan overload tanpa stack
    assert(listMuatTest.getJumlahNode() == 2);
    NodeLL* nodeKf1 = listMuatTest.cariKaryawanById("F001");
    NodeLL* nodeKf2 = listMuatTest.cariKaryawanById("F002");
    assert(nodeKf1 != nullptr && nodeKf1->dataKaryawan.namaKaryawan == "File User A");
    assert(nodeKf2 != nullptr && nodeKf2->dataKaryawan.namaKaryawan == "File User B");
    std::cout << "Test 2 (Muat dari File): Lolos" << std::endl;
    // listMuatTest.tampilkanSemua(); // Visual check

    // Test 3: Muat dari file yang tidak ada
    LinkedListKaryawan listTidakAdaFile;
    assert(listTidakAdaFile.muatDariFile("file_tidak_ada.csv") == false);
    assert(listTidakAdaFile.isEmpty() == true);
    std::cout << "Test 3 (Muat dari file tidak ada): Lolos" << std::endl;

    // Bersihkan file tes jika ada
    remove("test_data.csv"); 

    std::cout << "--- Tes Operasi File Selesai ---" << std::endl;
}
int main() {
    // ... (inisialisasi dan pemanggilan fungsi tes sebelumnya) ...
    ManajemenPengguna manajerPenggunaUtama(NAMA_FILE_PENGGUNA_UTAMA);
    tesStructKaryawan();    
    tesLinkedListKaryawan(); 
    tesFiturUndo();         
    tesManajemenPengguna();
    // tesOperasiFile(); // Bisa di-uncomment jika perlu

    std::cout << "\n===== Memulai Aplikasi Interaktif Pengelolaan Data Karyawan =====" << std::endl;
    
    LinkedListKaryawan daftarKaryawanUtama;
    StackAksi stackUndoUtama; 
    
    if (!daftarKaryawanUtama.muatDariFile(NAMA_FILE_KARYAWAN)) {
        std::cout << "Memulai dengan daftar karyawan kosong karena file '" << NAMA_FILE_KARYAWAN << "' tidak ditemukan atau gagal dimuat." << std::endl;
    }

    int pilihan = -1; 
    // ... (deklarasi kTemp, inputString1, inputString2 ...)

    do {
        tampilkanMenuAplikasi();
        
        if (!(std::cin >> pilihan)) { 
            std::cout << "Input tidak valid. Hanya angka yang diizinkan untuk pilihan menu." << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            pilihan = -99; 
            continue; 
        }
        // Pembersihan buffer ini krusial dan harus selalu ada setelah std::cin >> angka
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        if (usernameSaatIni.empty()) { 
            // ... (logika menu login/registrasi) ...
        } else { // Jika SUDAH LOGIN
            switch (pilihan) {
                // ... (case 1-9, 12 (Logout), 0 sama seperti sebelumnya) ...
                case 1: /* Tambah */
                case 2: /* Tampil Semua */
                case 3: /* Update */
                case 4: /* Hapus */
                case 5: /* Sort Nama Asc */
                case 6: /* Sort Nama Desc */
                case 7: /* Sort Gaji Asc */
                case 8: /* Sort Gaji Desc */
                case 9: /* Undo */
                    // Kode untuk case ini sudah ada di versi Anda sebelumnya
                    // Pastikan input string menggunakan getline setelah cin >> angka dan cin.ignore()
                    break; 

                case 12: { // Filter Berdasarkan Gaji (Asumsi ini adalah nomor menu yang benar)
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

                    std::cout << "Filter untuk gaji (1: Lebih Besar dari Batas, 0: Kurang dari Batas): ";
                    while(!(std::cin >> pilihanFilterGaji) || (pilihanFilterGaji != 0 && pilihanFilterGaji != 1) ) {
                        std::cout << "Input pilihan filter tidak valid. Masukkan 0 atau 1: ";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    // daftarKaryawanUtama.tampilkanKaryawanBerdasarkanGaji(batasGajiInput, (pilihanFilterGaji == 1)); // Pastikan fungsi ini ada di linkedlist_karyawan
                    break;
                }
                case 13: { // Filter Berdasarkan Jabatan (BARU & DIPERBAIKI)
                    std::cout << "--- Filter Karyawan Berdasarkan Jabatan ---" << std::endl;
                    std::vector<std::string> daftarJabatan = daftarKaryawanUtama.getDaftarJabatanUnik();
                    if (daftarJabatan.empty()) {
                        std::cout << "Tidak ada jabatan tersedia untuk difilter (daftar karyawan mungkin kosong atau tidak ada jabatan)." << std::endl;
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
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Penting!
                    
                    std::string jabatanDipilih = daftarJabatan[nomorJabatan - 1];
                    daftarKaryawanUtama.tampilkanKaryawanBerdasarkanJabatan(jabatanDipilih);
                    break;
                }
                case 14: { // Cari Berdasarkan Nama
                    std::cout << "--- Cari Karyawan Berdasarkan Nama ---" << std::endl;
                    std::cout << "Masukkan sebagian atau seluruh nama karyawan: ";
                    std::string namaDicari;
                    std::getline(std::cin, namaDicari); // getline sudah aman setelah cin.ignore() di atas
                    // daftarKaryawanUtama.cariKaryawanBerdasarkanNama(namaDicari); // Pastikan fungsi ini ada
                    break;
                }
                case 15: // Logout (Ganti dari 12 ke 15)
                    std::cout << "Pengguna '" << usernameSaatIni << "' berhasil logout." << std::endl;
                    usernameSaatIni = ""; 
                    break;
                case 0:
                    // ... (logika simpan file dan keluar)
                    break;
                default:
                    std::cout << "Pilihan tidak valid." << std::endl;
            }
        }
    } while (pilihan != 0);

    return 0;
}