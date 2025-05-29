// hierarki_jabatan.cpp
#include "hierarki_jabatan.h"
#include <iostream>
#include <algorithm>

PohonJabatan::PohonJabatan() : root(nullptr) {}

PohonJabatan::~PohonJabatan() {
    bersihkanPohon();
}

void PohonJabatan::bersihkanPohon() {
    delete root;
    root = nullptr;
    petaNode.clear();
}

void PohonJabatan::bangunDariLinkedList(const LinkedListKaryawan& daftarKaryawan, const std::string& idCEO) {
    bersihkanPohon();
    if (daftarKaryawan.isEmpty()) {
        std::cout << "Info: Daftar karyawan kosong, tidak dapat membangun hierarki." << std::endl;
        return;
    }

    NodeLL* current = daftarKaryawan.getHead(); 
    std::vector<Karyawan> semuaKaryawan;
    while (current != nullptr) {
        semuaKaryawan.push_back(current->dataKaryawan);
        current = current->next;
    }

    for (const auto& k : semuaKaryawan) {
        petaNode[k.idKaryawan] = new NodeTreeJabatan(k);
    }

    NodeLL* nodeCEO_LL = daftarKaryawan.cariKaryawanById(idCEO);
    if (!nodeCEO_LL) {
        std::cout << "Error: Karyawan dengan ID CEO '" << idCEO << "' tidak ditemukan." << std::endl;
        if (!petaNode.empty()){
             root = petaNode.begin()->second; 
             std::cout << "Info: Menggunakan karyawan pertama dalam daftar sebagai root sementara." << std::endl;
        } else {
            std::cout << "Error: Tidak ada karyawan untuk dijadikan root." << std::endl;
            return;
        }
    } else {
        root = petaNode[idCEO];
    }


    for (const auto& k : semuaKaryawan) {
        if (k.idKaryawan == idCEO) continue; 

        if (!k.idAtasan.empty() && petaNode.count(k.idAtasan) && petaNode.count(k.idKaryawan)) {
            petaNode[k.idAtasan]->anak.push_back(petaNode[k.idKaryawan]);
        } else if (k.idAtasan.empty() && k.idKaryawan != idCEO) {
             
        }
    }
    std::cout << "Info: Hierarki jabatan berhasil dibangun." << std::endl;
}


void PohonJabatan::tampilkanHierarkiRecursive(NodeTreeJabatan* node, int level) const {
    if (!node) return;
    for (int i = 0; i < level; ++i) {
        std::cout << "  ";
    }
    std::cout << "|-- " << node->dataKaryawan.namaKaryawan << " (" << node->dataKaryawan.jabatan << " - ID: " << node->dataKaryawan.idKaryawan << ")" << std::endl;
    for (NodeTreeJabatan* child : node->anak) {
        tampilkanHierarkiRecursive(child, level + 1);
    }
}

void PohonJabatan::tampilkanSeluruhHierarki() const {
    std::cout << "\n--- Struktur Organisasi / Hierarki Jabatan ---" << std::endl;
    if (!root) {
        std::cout << "Hierarki belum dibangun atau kosong." << std::endl;
        return;
    }
    tampilkanHierarkiRecursive(root, 0);
    std::cout << "---------------------------------------------" << std::endl;
}

std::vector<Karyawan> PohonJabatan::cariBawahanLangsung(const std::string& idAtasan) const {
    std::vector<Karyawan> hasil;
    if (petaNode.count(idAtasan)) {
        NodeTreeJabatan* nodeAtasan = petaNode.at(idAtasan);
        for (NodeTreeJabatan* anak : nodeAtasan->anak) {
            hasil.push_back(anak->dataKaryawan);
        }
    } else {
        std::cout << "Info: Karyawan dengan ID '" << idAtasan << "' tidak ditemukan dalam hierarki." << std::endl;
    }
    return hasil;
}

void PohonJabatan::cariBawahanRecursive(NodeTreeJabatan* node, std::vector<Karyawan>& hasil) const {
    if (!node) return;
    for (NodeTreeJabatan* anak : node->anak) {
        hasil.push_back(anak->dataKaryawan);
        cariBawahanRecursive(anak, hasil);
    }
}

std::vector<Karyawan> PohonJabatan::cariSemuaBawahan(const std::string& idAtasan) const {
    std::vector<Karyawan> hasil;
    if (petaNode.count(idAtasan)) {
        NodeTreeJabatan* nodeAtasan = petaNode.at(idAtasan);
        cariBawahanRecursive(nodeAtasan, hasil);
    } else {
        std::cout << "Info: Karyawan dengan ID '" << idAtasan << "' tidak ditemukan dalam hierarki." << std::endl;
    }
    return hasil;
}


bool PohonJabatan::cariJalurKePuncakRecursive(NodeTreeJabatan* nodeSaatIni, const std::string& idKaryawanTarget, std::vector<Karyawan>& jalur) const {
    if (!nodeSaatIni) return false;

    jalur.push_back(nodeSaatIni->dataKaryawan);

    if (nodeSaatIni->dataKaryawan.idKaryawan == idKaryawanTarget) {
        return true;
    }

    for (NodeTreeJabatan* anak : nodeSaatIni->anak) {
        if (cariJalurKePuncakRecursive(anak, idKaryawanTarget, jalur)) {
            return true;
        }
    }

    jalur.pop_back();
    return false;
}

std::vector<Karyawan> PohonJabatan::getJalurKePuncak(const std::string& idKaryawan) const {
    std::vector<Karyawan> jalur;
    if (!root) return jalur;

    if (root->dataKaryawan.idKaryawan == idKaryawan) {
        jalur.push_back(root->dataKaryawan);
        return jalur;
    }
    
    if (cariJalurKePuncakRecursive(root, idKaryawan, jalur)) {
        std::reverse(jalur.begin(), jalur.end());
        return jalur;
    }
    
    jalur.clear(); 
    std::cout << "Info: Karyawan dengan ID '" << idKaryawan << "' tidak ditemukan dalam hierarki atau bukan bagian dari tree utama." << std::endl;
    return jalur;
}