// linkedlist_karyawan.h
#ifndef LINKEDLIST_KARYAWAN_H
#define LINKEDLIST_KARYAWAN_H

#include "karyawan.h"
#include "stack_aksi.h" 
#include <iostream>
#include <string>
#include <vector>
#include <set> 

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

    void tambahDiAkhir(const Karyawan& k, StackAksi& undoStack);
    bool hapusKaryawanById(const std::string& idKaryawan, StackAksi& undoStack);
    bool updateKaryawanById(const std::string& idKaryawan, const Karyawan& dataBaru, StackAksi& undoStack);

    void tambahDiAkhir(const Karyawan& k);
    bool hapusKaryawanById(const std::string& idKaryawan);
    bool updateKaryawanById(const std::string& idKaryawan, const Karyawan& dataBaru);
    
    void tampilkanDataTerurut(bool berdasarkanNama, bool ascending);

    bool simpanKeFile(const std::string& namaFile) const;
    bool muatDariFile(const std::string& namaFile, StackAksi& undoStack); // Untuk konsistensi, meski stack mungkin dummy
    bool muatDariFile(const std::string& namaFile);

    void tampilkanKaryawanBerdasarkanGaji(double batasGaji, bool lebihBesar) const;
    void tampilkanKaryawanBerdasarkanJabatan(const std::string& jabatan) const;
    void cariKaryawanBerdasarkanNama(const std::string& nama) const;
    std::vector<std::string> getDaftarJabatanUnik() const;
};

#endif // LINKEDLIST_KARYAWAN_H