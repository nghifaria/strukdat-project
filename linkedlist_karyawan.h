// linkedlist_karyawan.h
#ifndef LINKEDLIST_KARYAWAN_H
#define LINKEDLIST_KARYAWAN_H

#include "karyawan.h"
#include "stack_aksi.h" // Diperlukan untuk overload fungsi
#include <iostream>
#include <string>
#include <vector>

struct NodeLL {
    Karyawan dataKaryawan;
    NodeLL* next;
    NodeLL(const Karyawan& k) : dataKaryawan(k), next(nullptr) {}
};

class LinkedListKaryawan {
private:
    NodeLL* head;
    NodeLL* tail;
    int jumlahNode;

    void hapusSemuaNode();
    static void tukarKaryawan(Karyawan& a, Karyawan& b); 
    void quickSortKaryawan(std::vector<Karyawan>& arr, int low, int high, bool berdasarkanNama, bool ascending);
    int partitionKaryawan(std::vector<Karyawan>& arr, int low, int high, bool berdasarkanNama, bool ascending);

public:
    LinkedListKaryawan();
    ~LinkedListKaryawan();

    bool isEmpty() const;
    int getJumlahNode() const;
    void tampilkanSemua() const;
    NodeLL* cariKaryawanById(const std::string& idKaryawan) const;

    // Fungsi dengan kapabilitas Undo
    void tambahDiAkhir(const Karyawan& k, StackAksi& undoStack);
    bool hapusKaryawanById(const std::string& idKaryawan, StackAksi& undoStack);
    bool updateKaryawanById(const std::string& idKaryawan, const Karyawan& dataBaru, StackAksi& undoStack);

    // Overload fungsi untuk penggunaan tanpa Undo (misal, untuk tes awal atau menu interaktif sebelum undo diimplementasikan)
    void tambahDiAkhir(const Karyawan& k);
    bool hapusKaryawanById(const std::string& idKaryawan);
    bool updateKaryawanById(const std::string& idKaryawan, const Karyawan& dataBaru);
    
    void tampilkanDataTerurut(bool berdasarkanNama, bool ascending);

    // Deklarasi fungsi baru untuk operasi file
    bool simpanKeFile(const std::string& namaFile) const;
    bool muatDariFile(const std::string& namaFile, StackAksi& undoStack);

    // Overload untuk muatDariFile tanpa undoStack (misalnya untuk pemanggilan awal)
    bool muatDariFile(const std::string& namaFile);
    // Fungsi untuk filtering gaji
    void tampilkanKaryawanBerdasarkanGaji(double batasGaji, bool lebihBesar) const;
    // Fungsi untuk filtering jabatan
    void tampilkanKaryawanBerdasarkanJabatan(const std::string& jabatan) const;
    // Fungsi untuk mencari karyawan berdasarkan nama
    void cariKaryawanBerdasarkanNama(const std::string& nama) const;
};



#endif // LINKEDLIST_KARYAWAN_H