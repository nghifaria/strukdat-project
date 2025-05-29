// linkedlist_karyawan.cpp
#include "linkedlist_karyawan.h"
#include <fstream>   
#include <sstream>   
#include <vector>    
#include <algorithm> 
#include <set>       

LinkedListKaryawan::LinkedListKaryawan() : head(nullptr), tail(nullptr), jumlahNode(0) {}

LinkedListKaryawan::~LinkedListKaryawan() {
    hapusSemuaNode();
}

void LinkedListKaryawan::tukarKaryawan(Karyawan& a, Karyawan& b) {
    Karyawan temp = a;
    a = b;
    b = temp;
}

void LinkedListKaryawan::hapusSemuaNode() {
    NodeLL* current = head;
    NodeLL* nextNode = nullptr;
    while (current != nullptr) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    jumlahNode = 0;
}

bool LinkedListKaryawan::isEmpty() const {
    return head == nullptr;
}

int LinkedListKaryawan::getJumlahNode() const {
    return jumlahNode;
}

NodeLL* LinkedListKaryawan::cariKaryawanById(const std::string& idKaryawan) const {
    NodeLL* current = head;
    while (current != nullptr) {
        if (current->dataKaryawan.idKaryawan == idKaryawan) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void LinkedListKaryawan::tambahDiAkhir(const Karyawan& k, StackAksi& undoStack) {
    NodeLL* newNode = new NodeLL(k);
    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    jumlahNode++;
    std::cout << "Info: Karyawan '" << k.namaKaryawan << "' (ID: " << k.idKaryawan << ") ditambahkan (dengan undo)." << std::endl;
    undoStack.push(AksiUndo(TipeAksi::TAMBAH, k));
}

bool LinkedListKaryawan::hapusKaryawanById(const std::string& idKaryawan, StackAksi& undoStack) {
    if (isEmpty()) {
        std::cout << "Info: Linked list kosong, tidak ada yang bisa dihapus (dengan undo)." << std::endl;
        return false;
    }
    NodeLL* current = head;
    NodeLL* prev = nullptr;
    while (current != nullptr && current->dataKaryawan.idKaryawan != idKaryawan) {
        prev = current;
        current = current->next;
    }
    if (current == nullptr) {
        std::cout << "Info: Karyawan dengan ID '" << idKaryawan << "' tidak ditemukan untuk dihapus (dengan undo)." << std::endl;
        return false;
    }
    Karyawan karyawanDihapus = current->dataKaryawan;
    if (current == head) {
        head = head->next;
        if (head == nullptr) tail = nullptr;
    } else {
        prev->next = current->next;
        if (current == tail) tail = prev;
    }
    delete current;
    jumlahNode--;
    std::cout << "Info: Karyawan dengan ID '" << idKaryawan << "' berhasil dihapus (dengan undo)." << std::endl;
    undoStack.push(AksiUndo(TipeAksi::HAPUS, karyawanDihapus));
    return true;
}

bool LinkedListKaryawan::updateKaryawanById(const std::string& idKaryawan, const Karyawan& dataBaru, StackAksi& undoStack) {
    NodeLL* nodeToUpdate = cariKaryawanById(idKaryawan);
    if (nodeToUpdate != nullptr) {
        Karyawan dataLama = nodeToUpdate->dataKaryawan;
        nodeToUpdate->dataKaryawan = dataBaru;
        std::cout << "Info: Data karyawan dengan ID '" << idKaryawan << "' berhasil diupdate (dengan undo)." << std::endl;
        undoStack.push(AksiUndo(TipeAksi::UPDATE, dataBaru, dataLama));
        return true;
    }
    std::cout << "Info: Karyawan dengan ID '" << idKaryawan << "' tidak ditemukan untuk diupdate (dengan undo)." << std::endl;
    return false;
}

void LinkedListKaryawan::tambahDiAkhir(const Karyawan& k) {
    NodeLL* newNode = new NodeLL(k);
    if (isEmpty()) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    jumlahNode++;
    std::cout << "Info: Karyawan '" << k.namaKaryawan << "' (ID: " << k.idKaryawan << ") ditambahkan (non-undo)." << std::endl;
}

bool LinkedListKaryawan::hapusKaryawanById(const std::string& idKaryawan) {
    if (isEmpty()) { std::cout << "Info: List kosong (non-undo)." << std::endl; return false; }
    NodeLL* current = head;
    NodeLL* prev = nullptr;
    while (current != nullptr && current->dataKaryawan.idKaryawan != idKaryawan) {
        prev = current;
        current = current->next;
    }
    if (current == nullptr) { std::cout << "Info: Karyawan tidak ditemukan (non-undo)." << std::endl; return false;}
    if (current == head) {
        head = head->next;
        if (head == nullptr) tail = nullptr;
    } else {
        prev->next = current->next;
        if (current == tail) tail = prev;
    }
    delete current;
    jumlahNode--;
    std::cout << "Info: Karyawan dengan ID '" << idKaryawan << "' berhasil dihapus (non-undo)." << std::endl;
    return true;
}

bool LinkedListKaryawan::updateKaryawanById(const std::string& idKaryawan, const Karyawan& dataBaru) {
    NodeLL* nodeToUpdate = cariKaryawanById(idKaryawan);
    if (nodeToUpdate != nullptr) {
        nodeToUpdate->dataKaryawan = dataBaru;
        std::cout << "Info: Data karyawan dengan ID '" << idKaryawan << "' berhasil diupdate (non-undo)." << std::endl;
        return true;
    }
    std::cout << "Info: Karyawan tidak ditemukan untuk diupdate (non-undo)." << std::endl;
    return false;
}

void LinkedListKaryawan::tampilkanSemua() const {
    if (isEmpty()) {
        std::cout << "Daftar karyawan kosong." << std::endl;
        return;
    }
    std::cout << "\n--- Daftar Semua Karyawan ---" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "ID\t\tNama\t\t\tJabatan\t\t\tGaji" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    NodeLL* current = head;
    while (current != nullptr) {
        std::cout << current->dataKaryawan.idKaryawan << "\t\t"
                  << current->dataKaryawan.namaKaryawan << "\t\t"
                  << current->dataKaryawan.jabatan << "\t\t"
                  << current->dataKaryawan.gaji << std::endl;
        current = current->next;
    }
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "Total Karyawan: " << jumlahNode << std::endl;
}

int LinkedListKaryawan::partitionKaryawan(std::vector<Karyawan>& arr, int low, int high, bool berdasarkanNama, bool ascending) {
    Karyawan pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        bool harusTukar = false;
        if (berdasarkanNama) {
            if (ascending ? (arr[j].namaKaryawan < pivot.namaKaryawan) : (arr[j].namaKaryawan > pivot.namaKaryawan))
                harusTukar = true;
        } else { 
            if (ascending ? (arr[j].gaji < pivot.gaji) : (arr[j].gaji > pivot.gaji))
                harusTukar = true;
        }
        if (harusTukar) {
            i++;
            LinkedListKaryawan::tukarKaryawan(arr[i], arr[j]);
        }
    }
    LinkedListKaryawan::tukarKaryawan(arr[i + 1], arr[high]);
    return (i + 1);
}

void LinkedListKaryawan::quickSortKaryawan(std::vector<Karyawan>& arr, int low, int high, bool berdasarkanNama, bool ascending) {
    if (low < high) {
        int pi = partitionKaryawan(arr, low, high, berdasarkanNama, ascending);
        quickSortKaryawan(arr, low, pi - 1, berdasarkanNama, ascending);
        quickSortKaryawan(arr, pi + 1, high, berdasarkanNama, ascending);
    }
}

void LinkedListKaryawan::tampilkanDataTerurut(bool berdasarkanNama, bool ascending) {
    if (isEmpty()) {
        std::cout << "Tidak ada data karyawan untuk diurutkan dan ditampilkan." << std::endl;
        return;
    }
    std::vector<Karyawan> arrKaryawan;
    NodeLL* current = head;
    while (current != nullptr) {
        arrKaryawan.push_back(current->dataKaryawan);
        current = current->next;
    }
    if (!arrKaryawan.empty()) {
        quickSortKaryawan(arrKaryawan, 0, arrKaryawan.size() - 1, berdasarkanNama, ascending);
    }
    std::cout << "\n--- Daftar Karyawan Terurut (Berdasarkan "
              << (berdasarkanNama ? "Nama" : "Gaji")
              << ", " << (ascending ? "Ascending" : "Descending")
              << ") ---" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "ID\t\tNama\t\t\tJabatan\t\t\tGaji" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    for (const auto& k : arrKaryawan) {
        std::cout << k.idKaryawan << "\t\t"
                  << k.namaKaryawan << "\t\t"
                  << k.jabatan << "\t\t"
                  << k.gaji << std::endl;
    }
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

bool LinkedListKaryawan::simpanKeFile(const std::string& namaFile) const {
    std::ofstream fileOutput(namaFile); 
    if (!fileOutput.is_open()) {
        std::cerr << "Error: Tidak dapat membuka file '" << namaFile << "' untuk ditulis." << std::endl;
        return false;
    }
    NodeLL* current = head;
    while (current != nullptr) {
        fileOutput << current->dataKaryawan.idKaryawan << ","
                   << current->dataKaryawan.namaKaryawan << ","
                   << current->dataKaryawan.jabatan << ","
                   << current->dataKaryawan.gaji << "\n";
        current = current->next;
    }
    fileOutput.close();
    std::cout << "Info: Data karyawan berhasil disimpan ke '" << namaFile << "'." << std::endl;
    return true;
}

bool LinkedListKaryawan::muatDariFile(const std::string& namaFile, StackAksi& undoStack) {
    std::ifstream fileInput(namaFile); 
    if (!fileInput.is_open()) {
        std::cerr << "Info: File '" << namaFile << "' tidak ditemukan atau tidak dapat dibuka. Memulai dengan list kosong." << std::endl;
        return false; 
    }
    hapusSemuaNode(); 
    std::string baris;
    Karyawan kTemp;
    while (std::getline(fileInput, baris)) {
        std::stringstream ss(baris);
        if (std::getline(ss, kTemp.idKaryawan, ',') &&
            std::getline(ss, kTemp.namaKaryawan, ',') &&
            std::getline(ss, kTemp.jabatan, ',') &&
            (ss >> kTemp.gaji) ) { 
            tambahDiAkhir(kTemp); 
        } else {
            std::cerr << "Peringatan: Baris tidak valid di file '" << namaFile << "': " << baris << std::endl;
        }
    }
    fileInput.close();
    std::cout << "Info: Data karyawan berhasil dimuat dari '" << namaFile << "'." << std::endl;
    return true;
}

bool LinkedListKaryawan::muatDariFile(const std::string& namaFile) {
    StackAksi dummyStack; 
    return muatDariFile(namaFile, dummyStack);
}

void LinkedListKaryawan::tampilkanKaryawanBerdasarkanGaji(double batasGaji, bool lebihBesar) const {
    if (isEmpty()) {
        std::cout << "Daftar karyawan kosong." << std::endl;
        return;
    }
    std::cout << "\n--- Karyawan dengan Gaji " << (lebihBesar ? "lebih besar dari atau sama dengan" : "kurang dari") << " " << batasGaji << " ---" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "ID\t\tNama\t\t\tJabatan\t\t\tGaji" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    NodeLL* current = head;
    bool ditemukan = false;
    while (current != nullptr) {
        if ((lebihBesar && current->dataKaryawan.gaji >= batasGaji) || 
            (!lebihBesar && current->dataKaryawan.gaji < batasGaji)) {
            std::cout << current->dataKaryawan.idKaryawan << "\t\t"
                      << current->dataKaryawan.namaKaryawan << "\t\t"
                      << current->dataKaryawan.jabatan << "\t\t"
                      << current->dataKaryawan.gaji << std::endl;
            ditemukan = true;
        }
        current = current->next;
    }
     if (!ditemukan) {
        std::cout << "Tidak ada karyawan yang memenuhi kriteria gaji tersebut." << std::endl;
    }
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

void LinkedListKaryawan::tampilkanKaryawanBerdasarkanJabatan(const std::string& jabatan) const {
    if (isEmpty()) {
        std::cout << "Daftar karyawan kosong." << std::endl;
        return;
    }
    std::cout << "\n--- Karyawan dengan Jabatan: " << jabatan << " ---" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "ID\t\tNama\t\t\tJabatan\t\t\tGaji" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    NodeLL* current = head;
    bool ditemukan = false;
    while (current != nullptr) {
        if (current->dataKaryawan.jabatan == jabatan) {
            std::cout << current->dataKaryawan.idKaryawan << "\t\t"
                      << current->dataKaryawan.namaKaryawan << "\t\t"
                      << current->dataKaryawan.jabatan << "\t\t"
                      << current->dataKaryawan.gaji << std::endl;
            ditemukan = true;
        }
        current = current->next;
    }
    if (!ditemukan) {
        std::cout << "Tidak ada karyawan dengan jabatan '" << jabatan << "'." << std::endl;
    }
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

void LinkedListKaryawan::cariKaryawanBerdasarkanNama(const std::string& nama) const {
    if (isEmpty()) {
        std::cout << "Daftar karyawan kosong." << std::endl;
        return;
    }
    std::cout << "\n--- Hasil Pencarian Karyawan dengan Nama mengandung: '" << nama << "' ---" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "ID\t\tNama\t\t\tJabatan\t\t\tGaji" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    NodeLL* current = head;
    bool ditemukan = false;
    std::string namaLower = nama;
    for(char &c : namaLower) { c = tolower(c); }

    while (current != nullptr) {
        std::string namaKaryawanLower = current->dataKaryawan.namaKaryawan;
        for(char &c : namaKaryawanLower) { c = tolower(c); }

        if (namaKaryawanLower.find(namaLower) != std::string::npos) {
            std::cout << current->dataKaryawan.idKaryawan << "\t\t"
                      << current->dataKaryawan.namaKaryawan << "\t\t"
                      << current->dataKaryawan.jabatan << "\t\t"
                      << current->dataKaryawan.gaji << std::endl;
            ditemukan = true;
        }
        current = current->next;
    }
     if (!ditemukan) {
        std::cout << "Tidak ada karyawan dengan nama yang cocok ditemukan." << std::endl;
    }
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

std::vector<std::string> LinkedListKaryawan::getDaftarJabatanUnik() const {
    if (isEmpty()) {
        return {};
    }
    std::set<std::string> setJabatan; 
    NodeLL* current = head;
    while (current != nullptr) {
        if (!current->dataKaryawan.jabatan.empty()) { 
             setJabatan.insert(current->dataKaryawan.jabatan);
        }
        current = current->next;
    }
    std::vector<std::string> vecJabatan(setJabatan.begin(), setJabatan.end());
    return vecJabatan;
}