// hierarki_jabatan.cpp
#include "hierarki_jabatan.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

PohonJabatan::PohonJabatan() : root(nullptr) {}

PohonJabatan::~PohonJabatan() {
    bersihkanPohon();
}

void PohonJabatan::bersihkanPohon() {
    for (auto const& [key, val] : petaNode) {
        delete val;
    }
    petaNode.clear();
    root = nullptr;
}

void PohonJabatan::bangunDariLinkedList(const LinkedListKaryawan& daftarKaryawan, const std::string& idCEOInput) {
    bersihkanPohon();
    if (daftarKaryawan.isEmpty()) {
        std::cout << "Info: Daftar karyawan kosong, tidak dapat membangun hierarki." << std::endl;
        return;
    }

    NodeLL* currentLLNode = daftarKaryawan.getHead();
    std::vector<Karyawan> semuaKaryawan;
    while (currentLLNode != nullptr) {
        semuaKaryawan.push_back(currentLLNode->dataKaryawan);
        currentLLNode = currentLLNode->next;
    }

    if (semuaKaryawan.empty()) {
        std::cout << "Info: Tidak ada data karyawan untuk membangun hierarki." << std::endl;
        return;
    }

    for (const auto& k : semuaKaryawan) {
        petaNode[k.idKaryawan] = new NodeTreeJabatan(k);
    }

    if (petaNode.empty()) {
        std::cout << "Error: Peta node kosong setelah inisialisasi." << std::endl;
        return;
    }
    
    std::vector<NodeTreeJabatan*> rootsPotensial;
    NodeTreeJabatan* calonRootDariInput = nullptr;

    if (!idCEOInput.empty() && petaNode.count(idCEOInput)) {
        calonRootDariInput = petaNode[idCEOInput];
    } else if (!idCEOInput.empty()) {
        std::cout << "Peringatan: Karyawan dengan ID CEO input '" << idCEOInput << "' tidak ditemukan dalam petaNode." << std::endl;
    }

    for (const auto& pairNode : petaNode) {
        NodeTreeJabatan* nodeSaatIni = pairNode.second;
        const std::string& idAtasan = nodeSaatIni->dataKaryawan.idAtasan;

        if (!idAtasan.empty() && petaNode.count(idAtasan)) {
            if (nodeSaatIni->dataKaryawan.idKaryawan != idAtasan) {
                petaNode[idAtasan]->anak.push_back(nodeSaatIni);
            }
        } else if (idAtasan.empty()) {
            rootsPotensial.push_back(nodeSaatIni);
        }
    }

    if (calonRootDariInput) {
        root = calonRootDariInput;
        std::cout << "Info: Menggunakan ID CEO input '" << root->dataKaryawan.idKaryawan << "' sebagai root." << std::endl;
    } else if (!rootsPotensial.empty()) {
        root = rootsPotensial[0]; 
        if (rootsPotensial.size() > 1) {
            std::cout << "Peringatan: Ditemukan " << rootsPotensial.size() << " karyawan tanpa ID Atasan. Menggunakan yang pertama ditemukan: '" << root->dataKaryawan.namaKaryawan << "' (ID: " << root->dataKaryawan.idKaryawan << ") sebagai root." << std::endl;
        } else {
            std::cout << "Info: Menggunakan karyawan '" << root->dataKaryawan.namaKaryawan << "' (ID: " << root->dataKaryawan.idKaryawan << ") sebagai root karena tidak ada ID Atasan." << std::endl;
        }
    } else if (!petaNode.empty()) {
        root = petaNode.begin()->second;
        std::cout << "Peringatan: Tidak ada karyawan tanpa ID Atasan dan ID CEO input tidak valid/ditemukan. Menggunakan karyawan pertama dalam peta (ID: " << root->dataKaryawan.idKaryawan << ") sebagai root hierarki. Hasil mungkin tidak sesuai." << std::endl;
    } else {
        std::cout << "Error: Tidak dapat menentukan root untuk hierarki. Tidak ada karyawan." << std::endl;
        return;
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
    auto it = petaNode.find(idAtasan);
    if (it != petaNode.end()) {
        NodeTreeJabatan* nodeAtasan = it->second;
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
    auto it = petaNode.find(idAtasan);
    if (it != petaNode.end()) {
        NodeTreeJabatan* nodeAtasan = it->second;
        cariBawahanRecursive(nodeAtasan, hasil);
    } else {
        std::cout << "Info: Karyawan dengan ID '" << idAtasan << "' tidak ditemukan dalam hierarki." << std::endl;
    }
    return hasil;
}

bool PohonJabatan::cariJalurKePuncakRecursive(NodeTreeJabatan* nodeSaatIni, const std::string& idKaryawanTarget, std::vector<Karyawan>& jalur) const {
     if (!nodeSaatIni) {
        return false;
    }
    if (nodeSaatIni->dataKaryawan.idKaryawan == idKaryawanTarget) {
        jalur.push_back(nodeSaatIni->dataKaryawan);
        return true;
    }
    for (NodeTreeJabatan* anak : nodeSaatIni->anak) {
        if (cariJalurKePuncakRecursive(anak, idKaryawanTarget, jalur)) {
            jalur.push_back(nodeSaatIni->dataKaryawan);
            return true;
        }
    }
    return false;
}

std::vector<Karyawan> PohonJabatan::getJalurKePuncak(const std::string& idKaryawan) const {
    std::vector<Karyawan> jalur;
    if (!root) {
        std::cout << "Info: Hierarki belum dibangun atau kosong." << std::endl;
        return jalur;
    }
    auto it = petaNode.find(idKaryawan);
    if (it == petaNode.end()){
        std::cout << "Info: Karyawan dengan ID '" << idKaryawan << "' tidak ditemukan dalam peta node hierarki." << std::endl;
        return jalur;
    }

    if (!cariJalurKePuncakRecursive(root, idKaryawan, jalur)) {
         jalur.clear();
         std::cout << "Info: Tidak ditemukan jalur dari root ke karyawan dengan ID '" << idKaryawan << "'." << std::endl;
    } else {
        std::reverse(jalur.begin(), jalur.end());
    }
    return jalur;
}

bool PohonJabatan::petaNodeContainsKey(const std::string& key) const {
    return petaNode.count(key) > 0;
}