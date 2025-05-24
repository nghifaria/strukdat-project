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

const std::string NAMA_FILE_DATA = "karyawan_data.csv"; // Definisikan nama file global

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
    Karyawan kar2_test = {"TL002", "Test Citra", "Test SPV", 8000000};
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
    ManajemenPengguna mp;
    assert(mp.registrasiPenggunaBaru("admin", "pass123") == true);
    std::cout << "Test 1.1 (Registrasi admin): Lolos" << std::endl;
    assert(mp.registrasiPenggunaBaru("admin", "adminpass") == false); 
    std::cout << "Test 1.2 (Registrasi duplikat username admin): Lolos" << std::endl;
    assert(mp.loginPengguna("admin", "pass123") == true);
    std::cout << "Test 2.1 (Login admin - berhasil): Lolos" << std::endl;
    assert(mp.loginPengguna("admin", "salah") == false);
    std::cout << "Test 2.2 (Login admin - password salah): Lolos" << std::endl;
    assert(mp.loginPengguna("userX", "pass") == false); 
    std::cout << "Test 2.3 (Login username tidak ada): Lolos" << std::endl;
    mp.tampilkanSemuaPenggunaDebug();
    std::cout << "--- Tes Manajemen Pengguna & Hashing Selesai ---" << std::endl;
}

// --- Fungsi UI ---
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
        std::cout << "12. Logout" << std::endl;
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
    tesStructKaryawan();    
    tesLinkedListKaryawan(); 
    tesFiturUndo();         
    tesManajemenPengguna();

    std::cout << "\n===== Memulai Aplikasi Interaktif Pengelolaan Data Karyawan =====" << std::endl;
    
    LinkedListKaryawan daftarKaryawanUtama;
    StackAksi stackUndoUtama; 
    ManajemenPengguna manajerPenggunaUtama;

    manajerPenggunaUtama.registrasiPenggunaBaru("tes", "tes123"); // Pengguna default

    // Muat data karyawan dari file saat aplikasi dimulai
    if (!daftarKaryawanUtama.muatDariFile(NAMA_FILE_DATA)) { // NAMA_FILE_DATA harus didefinisikan global
        std::cout << "Memulai dengan daftar karyawan kosong karena file data tidak ditemukan atau gagal dimuat." << std::endl;
    }

    int pilihan = -1; // Inisialisasi pilihan
    Karyawan kTemp; 
    std::string inputString1, inputString2; 

    do {
        tampilkanMenuAplikasi();
        
        // Baca input pilihan
        if (!(std::cin >> pilihan)) { // Cek apakah input valid (berupa angka)
            std::cout << "Input tidak valid. Hanya angka yang diizinkan untuk pilihan menu." << std::endl;
            std::cin.clear(); // Bersihkan error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa input salah
            pilihan = -99; // Set ke nilai yang tidak akan masuk ke case mana pun kecuali default
            continue; // Ulangi loop untuk meminta input lagi
        }
        // Jika input angka valid, buang sisa newline dari buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        if (usernameSaatIni.empty()) { // Jika BELUM LOGIN
            switch (pilihan) {
                case 10: { // Registrasi
                    std::cout << "--- Registrasi Pengguna Baru ---" << std::endl;
                    std::cout << "Masukkan Username baru: "; 
                    std::getline(std::cin, inputString1); // Baca username
                    std::cout << "Masukkan Password baru: "; 
                    std::getline(std::cin, inputString2); // Baca password
                    manajerPenggunaUtama.registrasiPenggunaBaru(inputString1, inputString2);
                    break;
                }
                case 11: { // Login
                    std::cout << "--- Login Pengguna ---" << std::endl;
                    std::cout << "Username: "; 
                    std::getline(std::cin, inputString1); // Baca username
                    std::cout << "Password: "; 
                    std::getline(std::cin, inputString2); // Baca password
                    if (manajerPenggunaUtama.loginPengguna(inputString1, inputString2)) {
                        usernameSaatIni = inputString1;
                    }
                    break;
                }
                case 0:
                    // Simpan data sebelum keluar
                    if (!daftarKaryawanUtama.simpanKeFile(NAMA_FILE_DATA)) {
                         std::cerr << "Peringatan: Gagal menyimpan data karyawan ke file!" << std::endl;
                    }
                    std::cout << "Keluar dari aplikasi." << std::endl;
                    break;
                default:
                    std::cout << "Pilihan tidak valid atau Anda belum login untuk mengakses fitur tersebut." << std::endl;
            }
        } else { // Jika SUDAH LOGIN
            switch (pilihan) {
                case 1: { // Tambah Karyawan
                    std::cout << "--- Tambah Karyawan (Login sebagai: " << usernameSaatIni << ") ---" << std::endl;
                    std::cout << "Masukkan ID Karyawan: "; std::getline(std::cin, kTemp.idKaryawan);
                    std::cout << "Masukkan Nama Karyawan: "; std::getline(std::cin, kTemp.namaKaryawan);
                    std::cout << "Masukkan Jabatan: "; std::getline(std::cin, kTemp.jabatan);
                    std::cout << "Masukkan Gaji: "; 
                    while(!(std::cin >> kTemp.gaji)){ 
                        std::cout << "Input gaji tidak valid (harus angka). Masukkan Gaji: "; 
                        std::cin.clear(); 
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    daftarKaryawanUtama.tambahDiAkhir(kTemp, stackUndoUtama);
                    break;
                }
                case 2: 
                    daftarKaryawanUtama.tampilkanSemua();
                    break;
                case 3: { 
                    std::cout << "--- Update Data Karyawan (Login sebagai: " << usernameSaatIni << ") ---" << std::endl;
                    std::cout << "Masukkan ID Karyawan yang akan diupdate: "; std::getline(std::cin, inputString1);
                    NodeLL* nodeAda = daftarKaryawanUtama.cariKaryawanById(inputString1);
                    if (nodeAda != nullptr) {
                        Karyawan kUpdate; 
                        kUpdate.idKaryawan = inputString1; 
                        std::cout << "Masukkan Nama Baru (ID: " << inputString1 << "): "; std::getline(std::cin, kUpdate.namaKaryawan);
                        std::cout << "Masukkan Jabatan Baru: "; std::getline(std::cin, kUpdate.jabatan);
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
                    std::cout << "Masukkan ID Karyawan yang akan dihapus: "; std::getline(std::cin, inputString1);
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
                case 12: 
                    std::cout << "Pengguna '" << usernameSaatIni << "' berhasil logout." << std::endl;
                    usernameSaatIni = ""; 
                    break;
                case 0:
                    // Simpan data sebelum keluar
                    if (!daftarKaryawanUtama.simpanKeFile(NAMA_FILE_DATA)) {
                         std::cerr << "Peringatan: Gagal menyimpan data karyawan ke file!" << std::endl;
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