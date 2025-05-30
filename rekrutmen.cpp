#include "rekrutmen.h"

std::string QueueRekrutmen::generateNewIdRekrutmen() {
    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << idRekrutmenCounter++;
    return ss.str();
}

StatusRekrutmen QueueRekrutmen::stringToStatusRekrutmen(const std::string& statusStr) {
    if (statusStr == "Diajukan") return StatusRekrutmen::DIAJUKAN;
    if (statusStr == "Diproses") return StatusRekrutmen::DIPROSES;
    if (statusStr == "Wawancara") return StatusRekrutmen::WAWANCARA;
    if (statusStr == "Lolos") return StatusRekrutmen::LOLOS;
    if (statusStr == "Ditolak") return StatusRekrutmen::DITOLAK;
    return StatusRekrutmen::DIAJUKAN;
}

QueueRekrutmen::QueueRekrutmen(const std::string& fileRekrutmen)
    : frontNode(nullptr), rearNode(nullptr), count(0), namaFileRekrutmen(fileRekrutmen), idRekrutmenCounter(1) {
    muatDariFile();
}

QueueRekrutmen::~QueueRekrutmen() {
    simpanSemuaKeFile();
    NodeRekrutmen* current = frontNode;
    NodeRekrutmen* nextNode = nullptr;
    while (current != nullptr) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

bool QueueRekrutmen::isEmpty() const {
    return count == 0;
}

int QueueRekrutmen::size() const {
    return count;
}

void QueueRekrutmen::enqueue(PermintaanRekrutmen& p) {
    if (p.idPermintaan.empty()) {
        p.idPermintaan = generateNewIdRekrutmen();
    }
    NodeRekrutmen* newNode = new NodeRekrutmen(p);
    if (isEmpty()) {
        frontNode = newNode;
        rearNode = newNode;
    } else {
        rearNode->next = newNode;
        rearNode = newNode;
    }
    count++;
}

bool QueueRekrutmen::dequeue(PermintaanRekrutmen& p) {
    if (isEmpty()) {
        return false;
    }
    NodeRekrutmen* temp = frontNode;
    p = temp->dataPermintaan;
    frontNode = frontNode->next;
    delete temp;
    count--;
    if (isEmpty()) {
        rearNode = nullptr;
    }
    return true;
}

PermintaanRekrutmen QueueRekrutmen::peek() const {
    if (isEmpty()) {
        throw std::runtime_error("Antrian rekrutmen kosong, tidak bisa peek.");
    }
    return frontNode->dataPermintaan;
}

bool QueueRekrutmen::updateStatusPermintaan(const std::string& idPermintaan, StatusRekrutmen statusBaru) {
    NodeRekrutmen* current = frontNode;
    while (current != nullptr) {
        if (current->dataPermintaan.idPermintaan == idPermintaan) {
            current->dataPermintaan.status = statusBaru;
            return true;
        }
        current = current->next;
    }
    return false;
}

void QueueRekrutmen::tampilkanSemuaPermintaan(bool hanyaAktif) const {
    if (hanyaAktif) {
        std::cout << "\n--- Daftar Permintaan Rekrutmen Aktif ---" << std::endl;
    } else {
        std::cout << "\n--- Daftar Semua Permintaan Rekrutmen (Histori) ---" << std::endl;
    }

    if (isEmpty()) {
        std::cout << "Tidak ada permintaan rekrutmen dalam antrian." << std::endl;
        return;
    }
    std::cout << std::left << std::setw(10) << "ID Req"
              << std::setw(20) << "Nama Pelamar"
              << std::setw(25) << "Jabatan Dilamar"
              << std::setw(20) << "Tanggal Pengajuan"
              << std::setw(15) << "Status" << std::endl;
    std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
    NodeRekrutmen* current = frontNode;
    bool adaDataTampil = false;
    while (current != nullptr) {
        if (hanyaAktif) {
            if (current->dataPermintaan.status == StatusRekrutmen::DIAJUKAN || current->dataPermintaan.status == StatusRekrutmen::DIPROSES || current->dataPermintaan.status == StatusRekrutmen::WAWANCARA) {
                std::cout << std::left << std::setw(10) << current->dataPermintaan.idPermintaan
                          << std::setw(20) << current->dataPermintaan.namaPelamar
                          << std::setw(25) << current->dataPermintaan.jabatanYangDilamar
                          << std::setw(20) << current->dataPermintaan.tanggalPengajuan
                          << std::setw(15) << statusRekrutmenToString(current->dataPermintaan.status) << std::endl;
                adaDataTampil = true;
            }
        } else {
            std::cout << std::left << std::setw(10) << current->dataPermintaan.idPermintaan
                      << std::setw(20) << current->dataPermintaan.namaPelamar
                      << std::setw(25) << current->dataPermintaan.jabatanYangDilamar
                      << std::setw(20) << current->dataPermintaan.tanggalPengajuan
                      << std::setw(15) << statusRekrutmenToString(current->dataPermintaan.status) << std::endl;
            adaDataTampil = true;
        }
        current = current->next;
    }
    if (!adaDataTampil && hanyaAktif) {
        std::cout << "Tidak ada permintaan rekrutmen aktif saat ini." << std::endl;
    } else if (!adaDataTampil && !hanyaAktif) {
        std::cout << "Tidak ada riwayat permintaan rekrutmen." << std::endl;
    }
    std::cout << "------------------------------------------------------------------------------------------------------" << std::endl;
}

void QueueRekrutmen::simpanSemuaKeFile() const {
    std::ofstream file(namaFileRekrutmen);
    if (!file.is_open()) {
        std::cerr << "Error: Tidak dapat membuka file '" << namaFileRekrutmen << "' untuk menyimpan permintaan rekrutmen." << std::endl;
        return;
    }
    NodeRekrutmen* current = frontNode;
    while (current != nullptr) {
        file << current->dataPermintaan.idPermintaan << ","
             << current->dataPermintaan.namaPelamar << ","
             << current->dataPermintaan.jabatanYangDilamar << ","
             << current->dataPermintaan.tanggalPengajuan << ","
             << statusRekrutmenToString(current->dataPermintaan.status) << "\n";
        current = current->next;
    }
    file.close();
}

void QueueRekrutmen::muatDariFile() {
    std::ifstream file(namaFileRekrutmen);
    if (!file.is_open()) {
        idRekrutmenCounter = 1;
        return;
    }

    while (frontNode != nullptr) {
        NodeRekrutmen* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
    }
    rearNode = nullptr;
    count = 0;
    idRekrutmenCounter = 1;

    std::string baris;
    int maxIdFromFile = 0;

    while (std::getline(file, baris)) {
        std::stringstream ss(baris);
        PermintaanRekrutmen p;
        std::string statusStr;

        if (std::getline(ss, p.idPermintaan, ',') &&
            std::getline(ss, p.namaPelamar, ',') &&
            std::getline(ss, p.jabatanYangDilamar, ',') &&
            std::getline(ss, p.tanggalPengajuan, ',') &&
            std::getline(ss, statusStr)
        ) {
            p.status = stringToStatusRekrutmen(statusStr);
            enqueue(p);
            try {
                int currentIdNum = std::stoi(p.idPermintaan);
                if (currentIdNum > maxIdFromFile) {
                    maxIdFromFile = currentIdNum;
                }
            } catch (const std::invalid_argument& e) {
            } catch (const std::out_of_range& e) {
            }
        }
    }
    file.close();

    if (!isEmpty()) {
       idRekrutmenCounter = maxIdFromFile + 1;
    } else {
       idRekrutmenCounter = 1;
    }
}