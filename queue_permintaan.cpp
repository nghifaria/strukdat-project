// queue_permintaan.cpp
#include "queue_permintaan.h"

std::string QueuePermintaan::generateNewIdPermintaan() {
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << idPermintaanCounter++;
    return ss.str();
}

StatusPermintaan QueuePermintaan::stringToStatusPermintaan(const std::string& statusStr) {
    if (statusStr == "Diajukan") return StatusPermintaan::DIAJUKAN;
    if (statusStr == "Diproses") return StatusPermintaan::DIPROSES;
    if (statusStr == "Disetujui") return StatusPermintaan::DISETUJUI;
    if (statusStr == "Ditolak") return StatusPermintaan::DITOLAK;
    return StatusPermintaan::DIAJUKAN;
}

QueuePermintaan::QueuePermintaan(const std::string& filePermintaan)
    : frontNode(nullptr), rearNode(nullptr), count(0), namaFilePermintaan(filePermintaan), idPermintaanCounter(1) {
    muatDariFile();
}

QueuePermintaan::~QueuePermintaan() {
    simpanSemuaKeFile();
    NodeQueue* current = frontNode;
    NodeQueue* nextNode = nullptr;
    while (current != nullptr) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

bool QueuePermintaan::isEmpty() const {
    return count == 0;
}

int QueuePermintaan::size() const {
    return count;
}

void QueuePermintaan::enqueue(Permintaan& p) {
    if (p.idPermintaanInternal.empty()) {
        p.idPermintaanInternal = generateNewIdPermintaan();
    }
    NodeQueue* newNode = new NodeQueue(p);
    if (isEmpty()) {
        frontNode = newNode;
        rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    count++;
}

bool QueuePermintaan::dequeue(Permintaan& p) {
    if (isEmpty()) {
        std::cout << "Antrian permintaan kosong." << std::endl;
        return false;
    }
    NodeQueue* temp = frontNode;
    p = temp->dataPermintaan;
    frontNode = frontNode->next;
    delete temp;
    count--;
    if (isEmpty()) {
        rearNode = nullptr;
    }
    return true;
}

Permintaan QueuePermintaan::peek() const {
    if (isEmpty()) {
        throw std::runtime_error("Antrian permintaan kosong, tidak bisa peek.");
    }
    return frontNode->dataPermintaan;
}

void QueuePermintaan::tampilkanSemuaPermintaan(bool hanyaAktif) const {
    if (hanyaAktif) {
        std::cout << "\n--- Daftar Permintaan Aktif (Diajukan/Diproses) ---" << std::endl;
    } else {
        std::cout << "\n--- Daftar Semua Permintaan Dalam Antrian (Histori) ---" << std::endl;
    }
    
    if (isEmpty()) {
        std::cout << "Tidak ada permintaan dalam antrian." << std::endl;
        return;
    }
    std::cout << std::left << std::setw(10) << "ID Req"
              << std::setw(15) << "ID Karyawan"
              << std::setw(20) << "Tipe Permintaan"
              << std::setw(30) << "Detail"
              << std::setw(20) << "Timestamp"
              << std::setw(15) << "Status"
              << std::setw(30) << "Catatan Admin" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    NodeQueue* current = frontNode;
    bool adaDataTampil = false;
    while (current != nullptr) {
        if (hanyaAktif) {
            if (current->dataPermintaan.status == StatusPermintaan::DIAJUKAN || current->dataPermintaan.status == StatusPermintaan::DIPROSES) {
                 std::cout << std::left << std::setw(10) << current->dataPermintaan.idPermintaanInternal
                          << std::setw(15) << current->dataPermintaan.idKaryawanPengaju
                          << std::setw(20) << current->dataPermintaan.tipePermintaan
                          << std::setw(30) << current->dataPermintaan.detailPermintaan
                          << std::setw(20) << current->dataPermintaan.timestampPengajuan
                          << std::setw(15) << statusPermintaanToString(current->dataPermintaan.status)
                          << std::setw(30) << current->dataPermintaan.catatanAdmin << std::endl;
                adaDataTampil = true;
            }
        } else {
            std::cout << std::left << std::setw(10) << current->dataPermintaan.idPermintaanInternal
                      << std::setw(15) << current->dataPermintaan.idKaryawanPengaju
                      << std::setw(20) << current->dataPermintaan.tipePermintaan
                      << std::setw(30) << current->dataPermintaan.detailPermintaan
                      << std::setw(20) << current->dataPermintaan.timestampPengajuan
                      << std::setw(15) << statusPermintaanToString(current->dataPermintaan.status)
                      << std::setw(30) << current->dataPermintaan.catatanAdmin << std::endl;
            adaDataTampil = true;
        }
        current = current->next;
    }
    if (!adaDataTampil && hanyaAktif) {
        std::cout << "Tidak ada permintaan aktif saat ini." << std::endl;
    } else if (!adaDataTampil && !hanyaAktif) {
        std::cout << "Tidak ada riwayat permintaan." << std::endl;
    }
    std::cout << "--------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
}

bool QueuePermintaan::updateStatusPermintaan(const std::string& idPermintaan, StatusPermintaan statusBaru, const std::string& catatanAdmin = "") {
    NodeQueue* current = frontNode;
    while (current != nullptr) {
        if (current->dataPermintaan.idPermintaanInternal == idPermintaan) {
            current->dataPermintaan.status = statusBaru;
            current->dataPermintaan.catatanAdmin = catatanAdmin;
            std::cout << "Info: Status permintaan " << idPermintaan << " berhasil diupdate menjadi " << statusPermintaanToString(statusBaru) << "." << std::endl;
            if (statusBaru == StatusPermintaan::DISETUJUI || statusBaru == StatusPermintaan::DITOLAK) {
            }
            return true;
        }
        current = current->next;
    }
    std::cout << "Error: Permintaan dengan ID " << idPermintaan << " tidak ditemukan." << std::endl;
    return false;
}

bool QueuePermintaan::prosesPermintaanSelesai(const std::string& idPermintaan) {
    if (isEmpty()) return false;

    NodeQueue* current = frontNode;
    NodeQueue* prev = nullptr;

    while(current != nullptr && current->dataPermintaan.idPermintaanInternal != idPermintaan) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) return false; 

    if (current->dataPermintaan.status != StatusPermintaan::DISETUJUI && current->dataPermintaan.status != StatusPermintaan::DITOLAK) {
        return false; 
    }
    
    if (current == frontNode) {
        frontNode = frontNode->next;
        if (frontNode == nullptr) rearNode = nullptr;
    } else {
        prev->next = current->next;
        if (prev->next == nullptr) rearNode = prev;
    }
    delete current;
    count--;
    return true;
}


void QueuePermintaan::simpanSemuaKeFile() const {
    std::ofstream file(namaFilePermintaan);
    if (!file.is_open()) {
        std::cerr << "Error: Tidak dapat membuka file '" << namaFilePermintaan << "' untuk menyimpan permintaan." << std::endl;
        return;
    }
    NodeQueue* current = frontNode;
    while (current != nullptr) {
        file << current->dataPermintaan.idPermintaanInternal << ","
             << current->dataPermintaan.idKaryawanPengaju << ","
             << current->dataPermintaan.tipePermintaan << ","
             << "\"" << current->dataPermintaan.detailPermintaan << "\","
             << current->dataPermintaan.timestampPengajuan << ","
             << statusPermintaanToString(current->dataPermintaan.status) << ","
             << "\"" << current->dataPermintaan.catatanAdmin << "\"\n";
        current = current->next;
    }
    file.close();
}

void QueuePermintaan::muatDariFile() {
    std::ifstream file(namaFilePermintaan);
    if (!file.is_open()) {
        std::cout << "Info: File permintaan '" << namaFilePermintaan << "' tidak ditemukan. Memulai dengan antrian kosong." << std::endl;
        idPermintaanCounter = 1;
        return;
    }

    while (frontNode != nullptr) {
        NodeQueue* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
    }
    rearNode = nullptr;
    count = 0;
    idPermintaanCounter = 1; 

    std::string baris;
    int maxIdFromFile = 0;

    std::vector<Permintaan> tempPermintaanList;

    while (std::getline(file, baris)) {
        std::stringstream ss(baris);
        Permintaan p;
        std::string statusStr;
        std::string tempDetail, tempCatatan;

        if (std::getline(ss, p.idPermintaanInternal, ',') &&
            std::getline(ss, p.idKaryawanPengaju, ',') &&
            std::getline(ss, p.tipePermintaan, ',') &&
            std::getline(ss, tempDetail, '"') &&
            std::getline(ss, tempDetail, '"') &&
            ss.ignore(1, ',') &&
            std::getline(ss, p.timestampPengajuan, ',') &&
            std::getline(ss, statusStr, ',') &&
            std::getline(ss, tempCatatan, '"') &&
            std::getline(ss, tempCatatan, '"')
        ) {
            p.detailPermintaan = tempDetail;
            p.catatanAdmin = tempCatatan;
            p.status = stringToStatusPermintaan(statusStr);
            tempPermintaanList.push_back(p);
            try {
                int currentIdNum = std::stoi(p.idPermintaanInternal);
                if (currentIdNum > maxIdFromFile) {
                    maxIdFromFile = currentIdNum;
                }
            } catch (const std::invalid_argument& e) {
            } catch (const std::out_of_range& e) {
            }
        } else {
            if(!baris.empty()){
                std::cerr << "Peringatan: Baris tidak valid di file permintaan: " << baris << std::endl;
            }
        }
    }
    file.close();

    for(const auto& p_const : tempPermintaanList){
        Permintaan p_non_const = p_const; 
        enqueue(p_non_const); 
    }
    
    if (!isEmpty()) {
       idPermintaanCounter = maxIdFromFile + 1;
    } else {
       idPermintaanCounter = 1;
    }


    if (count > 0) {
        std::cout << "Info: " << count << " permintaan berhasil dimuat dari '" << namaFilePermintaan << "'." << std::endl;
    }
}