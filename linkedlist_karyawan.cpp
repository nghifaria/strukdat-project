// linkedlist_karyawan.cpp
#include "linkedlist_karyawan.h"
#include <fstream>   // Untuk operasi file (ofstream, ifstream)
#include <sstream>   // Untuk std::stringstream (mem-parsing baris CSV)
#include <vector>    // Sudah ada untuk sorting

// Constructor
LinkedListKaryawan::LinkedListKaryawan() : head(nullptr), tail(nullptr), jumlahNode(0) {}

// Destructor
LinkedListKaryawan::~LinkedListKaryawan() {
    hapusSemuaNode();
}

// --- Definisi Static Member ---
void LinkedListKaryawan::tukarKaryawan(Karyawan& a, Karyawan& b) {
    Karyawan temp = a;
    a = b;
    b = temp;
}
// -----------------------------

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

// --- Implementasi Fungsi dengan StackAksi ---
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

// --- Implementasi Overload Fungsi (tanpa StackAksi) ---
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
    if (current == nullptr) { std::cout << "Info: Karyawan tidak ditemukan (non-undo)." << std::endl; return false; }

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
        nodeToUpdate->dataKaryawan = dataBaru; // ID juga akan terupdate jika berbeda di dataBaru
        std::cout << "Info: Data karyawan dengan ID '" << idKaryawan << "' berhasil diupdate (non-undo)." << std::endl;
        return true;
    }
    std::cout << "Info: Karyawan tidak ditemukan untuk diupdate (non-undo)." << std::endl;
    return false;
}
// -----------------------------------------------------

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
        } else { // Berdasarkan gaji
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

// Menyimpan semua data karyawan ke file CSV
bool LinkedListKaryawan::simpanKeFile(const std::string& namaFile) const {
    std::ofstream fileOutput(namaFile); // Membuka file untuk ditulis (overwrite jika sudah ada)

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

// Memuat data karyawan dari file CSV
bool LinkedListKaryawan::muatDariFile(const std::string& namaFile, StackAksi& undoStack) {
    std::ifstream fileInput(namaFile); // Membuka file untuk dibaca

    if (!fileInput.is_open()) {
        std::cerr << "Info: File '" << namaFile << "' tidak ditemukan atau tidak dapat dibuka. Memulai dengan list kosong." << std::endl;
        return false; // Tidak dianggap error fatal, aplikasi bisa mulai dengan list kosong
    }

    hapusSemuaNode(); // Kosongkan list saat ini sebelum memuat dari file

    std::string baris;
    Karyawan kTemp;
    StackAksi dummyStackUntukMuat; // Operasi muat dari file biasanya tidak di-undo satu per satu

    while (std::getline(fileInput, baris)) {
        std::stringstream ss(baris);
        std::string field;

        // Parsing CSV
        // Format: id,nama,jabatan,gaji
        if (std::getline(ss, kTemp.idKaryawan, ',') &&
            std::getline(ss, kTemp.namaKaryawan, ',') &&
            std::getline(ss, kTemp.jabatan, ',') &&
            (ss >> kTemp.gaji)) { // Baca gaji sebagai double
            
            // Tambah karyawan ke list tanpa mencatat ke undoStack
            tambahDiAkhir(kTemp); // Panggil overload yang tidak push ke undoStack
        } else {
            std::cerr << "Peringatan: Baris tidak valid di file '" << namaFile << "': " << baris << std::endl;
        }
    }

    fileInput.close();
    std::cout << "Info: Data karyawan berhasil dimuat dari '" << namaFile << "'." << std::endl;
    return true;
}

// Overload muatDariFile tanpa undoStack (untuk pemanggilan awal)
bool LinkedListKaryawan::muatDariFile(const std::string& namaFile) {
    StackAksi dummyStack; // Buat dummy stack karena fungsi target memerlukan referensi StackAksi
    return muatDariFile(namaFile, dummyStack);
}

// Fungsi untuk menampilkan karyawan berdasarkan gaji
void LinkedListKaryawan::tampilkanKaryawanBerdasarkanGaji(double batasGaji, bool lebihBesar) const {
    if (isEmpty()) {
        std::cout << "Daftar karyawan kosong." << std::endl;
        return;
    }
    std::cout << "\n--- Karyawan dengan Gaji " << (lebihBesar ? "lebih besar" : "kurang dari") << " " << batasGaji << " ---" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "ID\t\tNama\t\t\tJabatan\t\t\tGaji" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    
    NodeLL* current = head;
    while (current != nullptr) {
        if ((lebihBesar && current->dataKaryawan.gaji > batasGaji) || 
            (!lebihBesar && current->dataKaryawan.gaji < batasGaji)) {
            std::cout << current->dataKaryawan.idKaryawan << "\t\t"
                      << current->dataKaryawan.namaKaryawan << "\t\t"
                      << current->dataKaryawan.jabatan << "\t\t"
                      << current->dataKaryawan.gaji << std::endl;
        }
        current = current->next;
    }
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

// Fungsi untuk menampilkan karyawan berdasarkan jabatan
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
    while (current != nullptr) {
        if (current->dataKaryawan.jabatan == jabatan) {
            std::cout << current->dataKaryawan.idKaryawan << "\t\t"
                      << current->dataKaryawan.namaKaryawan << "\t\t"
                      << current->dataKaryawan.jabatan << "\t\t"
                      << current->dataKaryawan.gaji << std::endl;
        }
        current = current->next;
    }
    std::cout << "---------------------------------------------------------------------" << std::endl;
}

// Fungsi untuk mencari karyawan berdasarkan nama
void LinkedListKaryawan::cariKaryawanBerdasarkanNama(const std::string& nama) const {
    if (isEmpty()) {
        std::cout << "Daftar karyawan kosong." << std::endl;
        return;
    }
    std::cout << "\n--- Karyawan dengan Nama: " << nama << " ---" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "ID\t\tNama\t\t\tJabatan\t\t\tGaji" << std::endl;
    std::cout << "---------------------------------------------------------------------" << std::endl;
    
    NodeLL* current = head;
    while (current != nullptr) {
        if (current->dataKaryawan.namaKaryawan.find(nama) != std::string::npos) {
            std::cout << current->dataKaryawan.idKaryawan << "\t\t"
                      << current->dataKaryawan.namaKaryawan << "\t\t"
                      << current->dataKaryawan.jabatan << "\t\t"
                      << current->dataKaryawan.gaji << std::endl;
        }
        current = current->next;
    }
    std::cout << "---------------------------------------------------------------------" << std::endl;
}
