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
#include "rekrutmen.h"

const std::string NAMA_FILE_KARYAWAN = "karyawan_data.csv";
const std::string NAMA_FILE_PENGGUNA_UTAMA = "pengguna_data.csv";
const std::string ID_CEO_DEFAULT_APP = "";
const std::string NAMA_FILE_ANTRIAN_PERMINTAAN = "permintaan_data.csv";
const std::string NAMA_FILE_REKRUTMEN = "rekrutmen_data.csv";

void getInputString(const std::string& prompt, std::string& output);
double getInputGaji(const std::string& prompt);
int getInputPilihan(const std::string& prompt, int minPilihan, int maxPilihan);

void prosesMenuUtama(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan, QueuePermintaan& antrianPermintaan, QueueRekrutmen& antrianRekrutmen);
void prosesMenuAdmin(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan, QueuePermintaan& antrianPermintaan, QueueRekrutmen& antrianRekrutmen);
void prosesMenuKaryawan(LinkedListKaryawan& daftarKaryawan, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan, QueuePermintaan& antrianPermintaan);
void prosesMenuUser(QueueRekrutmen& antrianRekrutmen, ManajemenPengguna& manajerPengguna);
void prosesMenuCRUD(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, PohonJabatan& pohonJabatan);
void prosesMenuTampilkanDanUrutkan(LinkedListKaryawan& daftarKaryawan);
void prosesMenuFilterDanCari(LinkedListKaryawan& daftarKaryawan);
void prosesMenuHierarki(PohonJabatan& pohonJabatan, const LinkedListKaryawan& daftarKaryawan);
void prosesMenuPermintaanKaryawan(QueuePermintaan& antrianPermintaan, ManajemenPengguna& manajerPengguna, const LinkedListKaryawan& daftarKaryawan);
void prosesManajemenUser(ManajemenPengguna& manajerPengguna);
void prosesManajemenPermintaan(QueuePermintaan& antrianPermintaan);
void prosesManajemenRekrutmen(QueueRekrutmen& antrianRekrutmen, ManajemenPengguna& manajerPengguna, LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, PohonJabatan& pohonJabatan);

void tampilkanSplashScreen() {
    std::cout << "\nSelamat datang di\n\n";
    std::cout << "  _____ __  __ ____  _     _____   _______    _    _   _ _ \n";
    std::cout << " | ____|  \\/  |  _ \\| |   / _ \\ \\ / / ____|  / \\  | | | | |\n";
    std::cout << " |  _| | |\\/| | |_) | |  | | | \\ V /|  _|   / _ \\ | |_| | |\n";
    std::cout << " | |___| |  | |  __/| |__| |_| || | | |___ / ___ \\|  _  |_|\n";
    std::cout << " |_____|_|  |_|_|   |_____\\___/ |_| |_____/_/   \\_\\_| |_(_)\n";
    std::cout << "\n\n";
    std::cout << "Aplikasi Pengelolaan Data Karyawan\n";
    std::cout << "Developed by Kelompok 7 - P1\n";
    std::cout << "----------------------------------------------------------------------------------------\n\n";
}

int main() {
    tampilkanSplashScreen();

    ManajemenPengguna manajerPenggunaUtama(NAMA_FILE_PENGGUNA_UTAMA);
    LinkedListKaryawan daftarKaryawanUtama;
    StackAksi stackUndoUtama;
    PohonJabatan pohonJabatanUtama;
    QueuePermintaan antrianPermintaanUtama(NAMA_FILE_ANTRIAN_PERMINTAAN);
    QueueRekrutmen antrianRekrutmenUtama(NAMA_FILE_REKRUTMEN);

    daftarKaryawanUtama.muatDariFile(NAMA_FILE_KARYAWAN);
    pohonJabatanUtama.bangunDariLinkedList(daftarKaryawanUtama, ID_CEO_DEFAULT_APP);

    prosesMenuUtama(daftarKaryawanUtama, stackUndoUtama, manajerPenggunaUtama, pohonJabatanUtama, antrianPermintaanUtama, antrianRekrutmenUtama);

    daftarKaryawanUtama.simpanKeFile(NAMA_FILE_KARYAWAN);
    antrianPermintaanUtama.simpanSemuaKeFile();
    antrianRekrutmenUtama.simpanSemuaKeFile();

    return 0;
}

void prosesMenuUtama(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan, QueuePermintaan& antrianPermintaan, QueueRekrutmen& antrianRekrutmen) {
    int pilihan = -1;
    do {
        Pengguna* currentUser = manajerPengguna.getPenggunaSaatIni();
        if (!currentUser) {
            std::cout << "\n===== MENU LOGIN & REGISTRASI =====" << std::endl;
            std::cout << "1. Registrasi Akun Pelamar" << std::endl;
            std::cout << "2. Login" << std::endl;
            std::cout << "0. Keluar Aplikasi" << std::endl;
            pilihan = getInputPilihan("Pilihan: ", 0, 2);

            if (pilihan == 1) {
                std::string username, password;
                getInputString("Masukkan Username baru: ", username);
                getInputString("Masukkan Password baru: ", password);
                if(manajerPengguna.registrasiUserPublik(username, password)){
                    std::cout << "Registrasi berhasil! Silakan login." << std::endl;
                }
            } else if (pilihan == 2) {
                std::string username, password;
                getInputString("Username: ", username);
                getInputString("Password: ", password);
                if (!manajerPengguna.loginPengguna(username, password)) {
                    std::cout << "Login gagal. Username atau password salah." << std::endl;
                }
            }
        } else {
            TipePeran peran = currentUser->peran;
            if (peran == TipePeran::ADMIN) {
                prosesMenuAdmin(daftarKaryawan, stackUndo, manajerPengguna, pohonJabatan, antrianPermintaan, antrianRekrutmen);
            } else if (peran == TipePeran::PENGGUNA) {
                prosesMenuKaryawan(daftarKaryawan, manajerPengguna, pohonJabatan, antrianPermintaan);
            } else if (peran == TipePeran::USER) {
                prosesMenuUser(antrianRekrutmen, manajerPengguna);
            }
            pilihan = (manajerPengguna.getPenggunaSaatIni() == nullptr) ? -1 : 0;
        }

    } while (pilihan != 0);
}

void prosesMenuAdmin(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan, QueuePermintaan& antrianPermintaan, QueueRekrutmen& antrianRekrutmen) {
    int pilihan = -1;
    do {
        std::cout << "\n===== MENU ADMIN =====" << std::endl;
        std::cout << "Login sebagai: " << manajerPengguna.getPenggunaSaatIni()->username << std::endl;
        std::cout << "1. Manajemen Data Karyawan (CRUD)" << std::endl;
        std::cout << "2. Tampilkan & Urutkan Data Karyawan" << std::endl;
        std::cout << "3. Filter & Cari Data Karyawan" << std::endl;
        std::cout << "4. Visualisasi Hierarki Jabatan" << std::endl;
        std::cout << "5. Manajemen Pengguna" << std::endl;
        std::cout << "6. Manajemen Permintaan Karyawan" << std::endl;
        std::cout << "7. Manajemen Rekrutmen" << std::endl;
        std::cout << "8. Undo Aksi Terakhir (Karyawan)" << std::endl;
        std::cout << "0. Logout" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 8);

        switch(pilihan) {
            case 1: prosesMenuCRUD(daftarKaryawan, stackUndo, pohonJabatan); break;
            case 2: prosesMenuTampilkanDanUrutkan(daftarKaryawan); break;
            case 3: prosesMenuFilterDanCari(daftarKaryawan); break;
            case 4: prosesMenuHierarki(pohonJabatan, daftarKaryawan); break;
            case 5: prosesManajemenUser(manajerPengguna); break;
            case 6: prosesManajemenPermintaan(antrianPermintaan); break;
            case 7: prosesManajemenRekrutmen(antrianRekrutmen, manajerPengguna, daftarKaryawan, stackUndo, pohonJabatan); break;
            case 8:
                if (stackUndo.isEmpty()) { std::cout << "Tidak ada aksi untuk di-undo." << std::endl; }
                else {
                    AksiUndo aksi = stackUndo.pop();
                    if(aksi.tipe == TipeAksi::TAMBAH) daftarKaryawan.hapusKaryawanById(aksi.dataKaryawanUtama.idKaryawan);
                    else if(aksi.tipe == TipeAksi::HAPUS) daftarKaryawan.tambahDiAkhir(aksi.dataKaryawanUtama);
                    else if(aksi.tipe == TipeAksi::UPDATE && aksi.dataKaryawanSebelum.has_value()) daftarKaryawan.updateKaryawanById(aksi.dataKaryawanUtama.idKaryawan, aksi.dataKaryawanSebelum.value());
                    pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
                    std::cout << "Aksi terakhir berhasil di-undo." << std::endl;
                }
                break;
            case 0: manajerPengguna.logoutPengguna(); break;
        }
    } while (pilihan != 0);
}

void prosesMenuKaryawan(LinkedListKaryawan& daftarKaryawan, ManajemenPengguna& manajerPengguna, PohonJabatan& pohonJabatan, QueuePermintaan& antrianPermintaan) {
    int pilihan = -1;
    do {
        std::cout << "\n===== MENU KARYAWAN =====" << std::endl;
        std::cout << "Login sebagai: " << manajerPengguna.getPenggunaSaatIni()->username << std::endl;
        std::cout << "1. Tampilkan & Urutkan Data Karyawan" << std::endl;
        std::cout << "2. Filter & Cari Data Karyawan" << std::endl;
        std::cout << "3. Visualisasi Hierarki Jabatan" << std::endl;
        std::cout << "4. Menu Permintaan Saya" << std::endl;
        std::cout << "0. Logout" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 4);

        switch(pilihan) {
            case 1: prosesMenuTampilkanDanUrutkan(daftarKaryawan); break;
            case 2: prosesMenuFilterDanCari(daftarKaryawan); break;
            case 3: prosesMenuHierarki(pohonJabatan, daftarKaryawan); break;
            case 4: prosesMenuPermintaanKaryawan(antrianPermintaan, manajerPengguna, daftarKaryawan); break;
            case 0: manajerPengguna.logoutPengguna(); break;
        }
    } while (pilihan != 0);
}

void prosesMenuUser(QueueRekrutmen& antrianRekrutmen, ManajemenPengguna& manajerPengguna) {
    int pilihan = -1;
    Pengguna* currentUser = manajerPengguna.getPenggunaSaatIni();
    if (!currentUser) return;

    do {
        std::cout << "\n===== MENU PELAMAR =====" << std::endl;
        std::cout << "Login sebagai: " << currentUser->username << std::endl;
        std::cout << "1. Ajukan Lamaran Baru" << std::endl;
        std::cout << "2. Lihat Histori Lamaran Saya" << std::endl;
        std::cout << "0. Logout" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 2);

        switch (pilihan) {
            case 1: {
                std::string nama, jabatan;
                getInputString("Masukkan Nama Lengkap Anda: ", nama);
                getInputString("Masukkan Jabatan yang Dilamar: ", jabatan);
                if (nama.empty() || jabatan.empty()) {
                    std::cout << "Nama dan jabatan tidak boleh kosong." << std::endl;
                } else {
                    PermintaanRekrutmen pBaru(nama, jabatan, currentUser->username);
                    antrianRekrutmen.enqueue(pBaru);
                    std::cout << "Lamaran Anda dengan ID " << pBaru.idPermintaan << " telah diajukan." << std::endl;
                }
                break;
            }
            case 2: {
                std::cout << "\n--- Histori Lamaran untuk " << currentUser->username << " ---" << std::endl;
                NodeRekrutmen* current = antrianRekrutmen.getFrontNode();
                bool found = false;
                if (antrianRekrutmen.isEmpty() || current == nullptr) {
                     std::cout << "Anda belum pernah mengajukan lamaran." << std::endl;
                } else {
                    std::cout << std::left << std::setw(10) << "ID Req" << std::setw(25) << "Jabatan" << std::setw(20) << "Tanggal" << std::setw(15) << "Status" << std::endl;
                    std::cout << "-----------------------------------------------------------------" << std::endl;
                    while(current != nullptr) {
                        if (current->dataPermintaan.usernamePelamar == currentUser->username) {
                            std::cout << std::left << std::setw(10) << current->dataPermintaan.idPermintaan
                                      << std::setw(25) << current->dataPermintaan.jabatanYangDilamar
                                      << std::setw(20) << current->dataPermintaan.tanggalPengajuan
                                      << std::setw(15) << statusRekrutmenToString(current->dataPermintaan.status) << std::endl;
                            found = true;
                        }
                        current = current->next;
                    }
                    if (!found) {
                        std::cout << "Anda belum pernah mengajukan lamaran." << std::endl;
                    }
                }
                break;
            }
            case 0:
                manajerPengguna.logoutPengguna();
                break;
        }
    } while (pilihan != 0);
}

void prosesManajemenRekrutmen(QueueRekrutmen& antrianRekrutmen, ManajemenPengguna& manajerPengguna, LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, PohonJabatan& pohonJabatan) {
    int pilihan = -1;
    do {
        std::cout << "\n--- Manajemen Rekrutmen ---" << std::endl;
        std::cout << "1. Lihat Antrean Rekrutmen Aktif" << std::endl;
        std::cout << "2. Proses Permintaan Rekrutmen" << std::endl;
        std::cout << "3. Lihat Semua Histori Rekrutmen" << std::endl;
        std::cout << "0. Kembali" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 3);

        switch (pilihan) {
            case 1:
                antrianRekrutmen.tampilkanSemuaPermintaan(true);
                break;
            case 2: {
                antrianRekrutmen.tampilkanSemuaPermintaan(true);
                if (antrianRekrutmen.isEmpty()){
                     std::cout << "Tidak ada permintaan rekrutmen aktif untuk diproses." << std::endl;
                     break;
                }
                std::string idProses;
                getInputString("Masukkan ID Permintaan yang akan diproses: ", idProses);

                std::string usernamePelamar;
                std::string namaPelamarPilihan;
                std::string jabatanDilamarPilihan;
                NodeRekrutmen* currentRek = antrianRekrutmen.getFrontNode();
                bool idDitemukan = false;
                while(currentRek != nullptr){
                    if(currentRek->dataPermintaan.idPermintaan == idProses) {
                        usernamePelamar = currentRek->dataPermintaan.usernamePelamar;
                        namaPelamarPilihan = currentRek->dataPermintaan.namaPelamar;
                        jabatanDilamarPilihan = currentRek->dataPermintaan.jabatanYangDilamar;
                        idDitemukan = true;
                        break;
                    }
                    currentRek = currentRek->next;
                }

                if(!idDitemukan){
                    std::cout << "Error: ID Permintaan '"<< idProses << "' tidak ditemukan atau tidak valid." << std::endl;
                    break;
                }

                int statusPilihan = getInputPilihan("Pilih status baru (1: Diproses, 2: Wawancara, 3: Lolos, 4: Ditolak): ", 1, 4);
                StatusRekrutmen statusBaru;
                if(statusPilihan == 1) statusBaru = StatusRekrutmen::DIPROSES;
                else if(statusPilihan == 2) statusBaru = StatusRekrutmen::WAWANCARA;
                else if(statusPilihan == 3) statusBaru = StatusRekrutmen::LOLOS;
                else if(statusPilihan == 4) statusBaru = StatusRekrutmen::DITOLAK;
                else continue;

                if (antrianRekrutmen.updateStatusPermintaan(idProses, statusBaru)) {
                    std::cout << "Status permintaan " << idProses << " berhasil diupdate." << std::endl;
                    if (statusBaru == StatusRekrutmen::LOLOS) {
                        std::cout << "\n--- PROSES PENERIMAAN KARYAWAN BARU ---" << std::endl;

                        std::string newKaryawanId = daftarKaryawan.getNextId();
                        std::cout << "ID Karyawan baru yang akan dibuat: " << newKaryawanId << std::endl;

                        Karyawan kBaru;
                        kBaru.idKaryawan = newKaryawanId;
                        kBaru.namaKaryawan = namaPelamarPilihan;
                        std::cout << "Nama Lengkap Karyawan: " << kBaru.namaKaryawan << std::endl;
                        kBaru.jabatan = jabatanDilamarPilihan;
                        std::cout << "Jabatan: " << kBaru.jabatan << std::endl;
                        kBaru.gaji = getInputGaji("Masukkan Gaji (Juta): ");
                        getInputString("Masukkan ID Atasan (kosongkan jika tidak ada): ", kBaru.idAtasan);

                        std::string passwordBaru;
                        getInputString("Masukkan password untuk akun karyawan baru '" + newKaryawanId + "': ", passwordBaru);

                        if(manajerPengguna.registrasiPenggunaOlehAdmin(kBaru.idKaryawan, passwordBaru, TipePeran::PENGGUNA)){
                            std::cout << "Akun karyawan baru dengan username '" << kBaru.idKaryawan << "' berhasil dibuat." << std::endl;

                            daftarKaryawan.tambahDiAkhir(kBaru, stackUndo);
                            pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
                            std::cout << "Data karyawan baru berhasil ditambahkan." << std::endl;

                            if(manajerPengguna.hapusPengguna(manajerPengguna.getPenggunaSaatIni()->username, usernamePelamar)){
                                std::cout << "Akun pelamar lama '" << usernamePelamar << "' telah dihapus." << std::endl;
                            } else {
                                std::cout << "Peringatan: Gagal menghapus akun pelamar lama '" << usernamePelamar << "'." << std::endl;
                            }
                        } else {
                            std::cout << "Error: Gagal membuat akun login untuk karyawan baru." << std::endl;
                        }
                    }
                } else {
                    std::cout << "ID Permintaan tidak ditemukan saat mencoba update." << std::endl;
                }
                break;
            }
            case 3:
                antrianRekrutmen.tampilkanSemuaPermintaan(false);
                break;
        }
    } while (pilihan != 0);
}

void prosesManajemenUser(ManajemenPengguna& manajerPengguna) {
    int pilihan = -1;
    do {
        std::cout << "\n--- Manajemen Pengguna ---" << std::endl;
        std::cout << "1. Tampilkan Semua Pengguna" << std::endl;
        std::cout << "2. Registrasi Pengguna Baru (oleh Admin)" << std::endl;
        std::cout << "3. Hapus Pengguna" << std::endl;
        std::cout << "0. Kembali" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 3);
        if (pilihan == 1) {
            manajerPengguna.tampilkanSemuaPenggunaDebug();
        } else if (pilihan == 2) {
            std::string username, password;
            int peranPilihan;
            getInputString("Username baru: ", username);
            getInputString("Password baru: ", password);
            peranPilihan = getInputPilihan("Peran (1:Admin, 2:Karyawan, 3:User Biasa): ", 1, 3);
            TipePeran peranBaru = (peranPilihan == 1) ? TipePeran::ADMIN : (peranPilihan == 2) ? TipePeran::PENGGUNA : TipePeran::USER;
            if (manajerPengguna.registrasiPenggunaOlehAdmin(username, password, peranBaru)) {
                 std::cout << "Pengguna baru berhasil diregistrasi." << std::endl;
            }
        } else if (pilihan == 3) {
            std::string username;
            getInputString("Username yang akan dihapus: ", username);
            if(manajerPengguna.hapusPengguna(manajerPengguna.getPenggunaSaatIni()->username, username)){
                std::cout << "Pengguna berhasil dihapus." << std::endl;
            } else {
                 std::cout << "Gagal menghapus pengguna. Pastikan Anda tidak menghapus diri sendiri atau admin lain." << std::endl;
            }
        }
    } while (pilihan != 0);
}

void prosesManajemenPermintaan(QueuePermintaan& antrianPermintaan) {
     int pilihan = -1;
    do {
        std::cout << "\n--- Manajemen Permintaan Karyawan ---" << std::endl;
        std::cout << "1. Lihat Antrian Permintaan Aktif" << std::endl;
        std::cout << "2. Proses Permintaan" << std::endl;
        std::cout << "3. Lihat Semua Histori Permintaan" << std::endl;
        std::cout << "0. Kembali" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 3);

        if(pilihan == 1) antrianPermintaan.tampilkanSemuaPermintaan(true);
        else if (pilihan == 2){
            antrianPermintaan.tampilkanSemuaPermintaan(true);
            if(antrianPermintaan.isEmpty()){
                std::cout << "Tidak ada permintaan aktif untuk diproses." << std::endl;
                continue;
            }
            std::string id, catatan;
            int statusPilihan;
            getInputString("ID Permintaan untuk diproses: ", id);
            statusPilihan = getInputPilihan("Status baru (1:Diproses, 2:Disetujui, 3:Ditolak):", 1, 3);
            getInputString("Catatan admin (opsional): ", catatan);
            StatusPermintaan status = (statusPilihan == 1) ? StatusPermintaan::DIPROSES : (statusPilihan == 2) ? StatusPermintaan::DISETUJUI : StatusPermintaan::DITOLAK;
            antrianPermintaan.updateStatusPermintaan(id, status, catatan);
        }
        else if (pilihan == 3) antrianPermintaan.tampilkanSemuaPermintaan(false);

    } while (pilihan != 0);
}


void prosesMenuCRUD(LinkedListKaryawan& daftarKaryawan, StackAksi& stackUndo, PohonJabatan& pohonJabatan) {
    int pilihanCRUD = -1;
    Karyawan kOperasi;
    std::string idKaryawanOperasi;

    do {
        std::cout << "\n--- Sub Menu: Manajemen Data Karyawan (CRUD) ---" << std::endl;
        std::cout << "1. Tambah Karyawan Baru" << std::endl;
        std::cout << "2. Update Data Karyawan" << std::endl;
        std::cout << "3. Hapus Karyawan" << std::endl;
        std::cout << "0. Kembali" << std::endl;
        pilihanCRUD = getInputPilihan("Pilihan: ", 0, 3);

        switch (pilihanCRUD) {
            case 1: {
                std::string idBaru = daftarKaryawan.getNextId();
                kOperasi.idKaryawan = idBaru;
                std::cout << "ID Karyawan baru akan dibuat: " << idBaru << std::endl;

                getInputString("Masukkan Nama Karyawan: ", kOperasi.namaKaryawan);
                getInputString("Masukkan Jabatan: ", kOperasi.jabatan);
                kOperasi.gaji = getInputGaji("Masukkan Gaji (Juta): ");
                getInputString("Masukkan ID Atasan (kosongkan jika tidak ada): ", kOperasi.idAtasan);

                std::string pass;
                std::cout << "Pembuatan karyawan dari menu CRUD juga akan membuat akun login." << std::endl;
                getInputString("Masukkan Password untuk akun karyawan baru '" + idBaru + "': " , pass);

                ManajemenPengguna tempMgr(NAMA_FILE_PENGGUNA_UTAMA);
                if(tempMgr.registrasiPenggunaOlehAdmin(kOperasi.idKaryawan, pass, TipePeran::PENGGUNA)){
                    std::cout << "Akun karyawan dengan username '" << kOperasi.idKaryawan << "' berhasil dibuat." << std::endl;
                    daftarKaryawan.tambahDiAkhir(kOperasi, stackUndo);
                    pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
                    std::cout << "Data karyawan baru berhasil ditambahkan." << std::endl;
                } else {
                    std::cout << "Gagal membuat akun login, data karyawan tidak ditambahkan." << std::endl;
                }
                break;
            }
            case 2: {
                daftarKaryawan.tampilkanSemua();
                if (daftarKaryawan.isEmpty()) break;
                getInputString("Masukkan ID Karyawan yang akan diupdate: ", idKaryawanOperasi);
                NodeLL* nodeAda = daftarKaryawan.cariKaryawanById(idKaryawanOperasi);
                if (nodeAda != nullptr) {
                    Karyawan kUpdate = nodeAda->dataKaryawan;
                    Karyawan kOriginal = kUpdate;
                    std::string buffer;

                    std::cout << "Nama saat ini: " << kUpdate.namaKaryawan << std::endl;
                    getInputString("Nama Baru (kosongkan jika tidak diubah): ", buffer); if(!buffer.empty()) kUpdate.namaKaryawan = buffer;

                    std::cout << "Jabatan saat ini: " << kUpdate.jabatan << std::endl;
                    getInputString("Jabatan Baru (kosongkan jika tidak diubah): ", buffer); if(!buffer.empty()) kUpdate.jabatan = buffer;

                    std::cout << "Gaji saat ini: " << kUpdate.gaji << std::endl;
                    std::string gajiInputStr;
                    getInputString("Gaji Baru (Juta) (kosongkan jika tidak diubah): ", gajiInputStr);
                    if (!gajiInputStr.empty()) {
                        try { kUpdate.gaji = std::stod(gajiInputStr); }
                        catch (const std::exception& e) { std::cout << "Input gaji tidak valid, gaji tidak diubah." << std::endl; kUpdate.gaji = kOriginal.gaji; }
                    }

                    std::cout << "ID Atasan saat ini: " << (kUpdate.idAtasan.empty() ? "-" : kUpdate.idAtasan) << std::endl;
                    getInputString("ID Atasan Baru (kosongkan jika tidak diubah, ketik 'hapus' untuk mengosongkan): ", buffer);
                    if (buffer == "hapus") kUpdate.idAtasan = "";
                    else if (!buffer.empty()) kUpdate.idAtasan = buffer;

                    if (daftarKaryawan.updateKaryawanById(idKaryawanOperasi, kUpdate, stackUndo)){
                        pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
                        std::cout << "Data karyawan berhasil diupdate." << std::endl;
                    } else {
                         std::cout << "Gagal mengupdate data karyawan." << std::endl;
                    }
                } else {
                    std::cout << "Karyawan dengan ID '" << idKaryawanOperasi << "' tidak ditemukan." << std::endl;
                }
                break;
            }
            case 3: {
                daftarKaryawan.tampilkanSemua();
                if (daftarKaryawan.isEmpty()) break;
                getInputString("Masukkan ID Karyawan yang akan dihapus: ", idKaryawanOperasi);
                if (daftarKaryawan.hapusKaryawanById(idKaryawanOperasi, stackUndo)){
                    pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
                    std::cout << "Data karyawan berhasil dihapus. Akun login terkait (" << idKaryawanOperasi << ") harus dihapus manual melalui Manajemen Pengguna jika ada." << std::endl;
                } else {
                    std::cout << "Gagal menghapus karyawan. ID tidak ditemukan." << std::endl;
                }
                break;
            }
        }
    } while (pilihanCRUD != 0);
}

void prosesMenuTampilkanDanUrutkan(LinkedListKaryawan& daftarKaryawan) {
    int pilihan = -1;
    do {
        std::cout << "\n--- Tampilkan & Urutkan Data ---" << std::endl;
        std::cout << "1. Tampilkan Semua" << std::endl;
        std::cout << "2. Urutkan berdasarkan Nama (Asc)" << std::endl;
        std::cout << "3. Urutkan berdasarkan Nama (Desc)" << std::endl;
        std::cout << "4. Urutkan berdasarkan Gaji (Asc)" << std::endl;
        std::cout << "5. Urutkan berdasarkan Gaji (Desc)" << std::endl;
        std::cout << "0. Kembali" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 5);
        if (pilihan == 1) daftarKaryawan.tampilkanSemua();
        else if (pilihan == 2) daftarKaryawan.tampilkanDataTerurut(true, true);
        else if (pilihan == 3) daftarKaryawan.tampilkanDataTerurut(true, false);
        else if (pilihan == 4) daftarKaryawan.tampilkanDataTerurut(false, true);
        else if (pilihan == 5) daftarKaryawan.tampilkanDataTerurut(false, false);
    } while (pilihan != 0);
}

void prosesMenuFilterDanCari(LinkedListKaryawan& daftarKaryawan) {
    int pilihan = -1;
    do {
        std::cout << "\n--- Filter & Cari Data ---" << std::endl;
        std::cout << "1. Filter Gaji" << std::endl;
        std::cout << "2. Filter Jabatan" << std::endl;
        std::cout << "3. Cari Nama" << std::endl;
        std::cout << "0. Kembali" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 3);
        if (pilihan == 1) {
            double batas = getInputGaji("Masukkan batas gaji (Juta): ");
            int opsi = getInputPilihan("Filter (1: >=, 0: <): ", 0, 1);
            daftarKaryawan.tampilkanKaryawanBerdasarkanGaji(batas, opsi == 1);
        } else if (pilihan == 2) {
            std::vector<std::string> daftarJabatan = daftarKaryawan.getDaftarJabatanUnik();
            if(daftarJabatan.empty()){
                std::cout << "Belum ada data jabatan." << std::endl;
                continue;
            }
            std::cout << "Daftar Jabatan Tersedia:" << std::endl;
            for(size_t i=0; i < daftarJabatan.size(); ++i){
                std::cout << i+1 << ". " << daftarJabatan[i] << std::endl;
            }
            int pilihanJabatan = getInputPilihan("Pilih nomor jabatan untuk difilter: ", 1, daftarJabatan.size());
            daftarKaryawan.tampilkanKaryawanBerdasarkanJabatan(daftarJabatan[pilihanJabatan-1]);
        } else if (pilihan == 3) {
            std::string nama;
            getInputString("Masukkan nama/bagian nama yang dicari: ", nama);
            daftarKaryawan.cariKaryawanBerdasarkanNama(nama);
        }
    } while (pilihan != 0);
}

void prosesMenuHierarki(PohonJabatan& pohonJabatan, const LinkedListKaryawan& daftarKaryawan) {
    int pilihan = -1;
    pohonJabatan.bangunDariLinkedList(daftarKaryawan, ID_CEO_DEFAULT_APP);
    do {
        std::cout << "\n--- Hierarki Jabatan ---" << std::endl;
        std::cout << "1. Tampilkan Seluruh Hierarki" << std::endl;
        std::cout << "2. Cari Bawahan Langsung" << std::endl;
        std::cout << "3. Cari Semua Bawahan" << std::endl;
        std::cout << "0. Kembali" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 3);
        std::string id;

        if (pilihan == 1) {
            pohonJabatan.tampilkanSeluruhHierarki();
        } else if (pilihan == 2) {
            getInputString("Masukkan ID Atasan: ", id);
            std::vector<Karyawan> bawahanLangsung = pohonJabatan.cariBawahanLangsung(id);
            if (pohonJabatan.petaNodeContainsKey(id) && bawahanLangsung.empty()) {
                 std::cout << "Karyawan dengan ID '" << id << "' tidak memiliki bawahan langsung." << std::endl;
            } else if (!bawahanLangsung.empty()) {
                std::cout << "\n--- Bawahan Langsung dari " << id << " ---" << std::endl;
                std::cout << std::left << std::setw(10) << "ID" << std::setw(25) << "Nama" << std::setw(20) << "Jabatan" << std::endl;
                std::cout << "--------------------------------------------------" << std::endl;
                for (const auto& k : bawahanLangsung) {
                    std::cout << std::left << std::setw(10) << k.idKaryawan
                              << std::setw(25) << k.namaKaryawan
                              << std::setw(20) << k.jabatan << std::endl;
                }
                 std::cout << "--------------------------------------------------" << std::endl;
            }
        } else if (pilihan == 3) {
            getInputString("Masukkan ID Atasan: ", id);
            std::vector<Karyawan> semuaBawahan = pohonJabatan.cariSemuaBawahan(id);
             if (pohonJabatan.petaNodeContainsKey(id) && semuaBawahan.empty()) {
                 std::cout << "Karyawan dengan ID '" << id << "' tidak memiliki bawahan dalam struktur." << std::endl;
            } else if (!semuaBawahan.empty()) {
                std::cout << "\n--- Semua Bawahan dari " << id << " (Rekursif) ---" << std::endl;
                std::cout << std::left << std::setw(10) << "ID" << std::setw(25) << "Nama" << std::setw(20) << "Jabatan" << std::endl;
                std::cout << "--------------------------------------------------" << std::endl;
                for (const auto& k : semuaBawahan) {
                     std::cout << std::left << std::setw(10) << k.idKaryawan
                              << std::setw(25) << k.namaKaryawan
                              << std::setw(20) << k.jabatan << std::endl;
                }
                std::cout << "--------------------------------------------------" << std::endl;
            }
        }
    } while (pilihan != 0);
}

void prosesMenuPermintaanKaryawan(QueuePermintaan& antrianPermintaan, ManajemenPengguna& manajerPengguna, const LinkedListKaryawan& daftarKaryawan) {
    Pengguna* currentUser = manajerPengguna.getPenggunaSaatIni();
    if (!currentUser) return;
    std::string idKaryawan = currentUser->username;

    int pilihan = -1;
    do {
        std::cout << "\n--- Menu Permintaan Saya ---" << std::endl;
        std::cout << "1. Ajukan Permintaan Baru" << std::endl;
        std::cout << "2. Lihat Status Permintaan" << std::endl;
        std::cout << "0. Kembali" << std::endl;
        pilihan = getInputPilihan("Pilihan: ", 0, 2);
        if (pilihan == 1) {
            std::string tipe, detail;
            getInputString("Tipe permintaan: ", tipe);
            getInputString("Detail permintaan: ", detail);
            Permintaan pBaru(idKaryawan, tipe, detail);
            antrianPermintaan.enqueue(pBaru);
            std::cout << "Permintaan (ID: " << pBaru.idPermintaanInternal << ") diajukan." << std::endl;
        } else if (pilihan == 2) {
            std::cout << "\n--- Status Permintaan untuk " << idKaryawan << " ---" << std::endl;
            NodeQueue* current = antrianPermintaan.getFrontNode();
            bool found = false;
            if (antrianPermintaan.isEmpty() || current == nullptr){
                std::cout << "Tidak ada permintaan ditemukan." << std::endl;
            } else {
                std::cout << std::left << std::setw(10) << "ID Req" << std::setw(20) << "Tipe" << std::setw(30) << "Detail" <<  std::setw(20) << "Timestamp" << std::setw(15) << "Status" << std::setw(30) << "Catatan Admin" << std::endl;
                std::cout << "-----------------------------------------------------------------------------------------------------------------------" << std::endl;
                while (current != nullptr) {
                    if (current->dataPermintaan.idKaryawanPengaju == idKaryawan) {
                        std::cout << std::left << std::setw(10) << current->dataPermintaan.idPermintaanInternal
                                  << std::setw(20) << current->dataPermintaan.tipePermintaan
                                  << std::setw(30) << current->dataPermintaan.detailPermintaan
                                  << std::setw(20) << current->dataPermintaan.timestampPengajuan
                                  << std::setw(15) << statusPermintaanToString(current->dataPermintaan.status)
                                  << std::setw(30) << current->dataPermintaan.catatanAdmin << std::endl;
                        found = true;
                    }
                    current = current->next;
                }
                if(!found) std::cout << "Tidak ada permintaan ditemukan untuk Anda." << std::endl;
            }
        }
    } while (pilihan != 0);
}

void getInputString(const std::string& prompt, std::string& output) {
    std::cout << prompt;
    std::getline(std::cin, output);
}

double getInputGaji(const std::string& prompt) {
    double gaji;
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        std::stringstream ss(input);
        if (ss >> gaji && gaji >= 0 && (ss.eof() || ss.peek() == EOF) ) {
            return gaji;
        } else {
            std::cout << "Input tidak valid. Masukkan angka non-negatif." << std::endl;
        }
    }
}

int getInputPilihan(const std::string& prompt, int minPilihan, int maxPilihan) {
    int pilihan;
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        std::stringstream ss(input);
        if (ss >> pilihan && pilihan >= minPilihan && pilihan <= maxPilihan && (ss.eof() || ss.peek() == EOF) ) {
            return pilihan;
        } else {
            std::cout << "Input tidak valid. Masukkan angka antara " << minPilihan << " dan " << maxPilihan << "." << std::endl;
        }
    }
}