#ifndef REKRUTMEN_H
#define REKRUTMEN_H

#include <string>
#include <queue>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

enum class StatusRekrutmen {
    DIAJUKAN,
    DIPROSES,
    WAWANCARA,
    LOLOS,
    DITOLAK
};

inline std::string statusRekrutmenToString(StatusRekrutmen status) {
    switch (status) {
        case StatusRekrutmen::DIAJUKAN: return "Diajukan";
        case StatusRekrutmen::DIPROSES: return "Diproses";
        case StatusRekrutmen::WAWANCARA: return "Wawancara";
        case StatusRekrutmen::LOLOS: return "Lolos";
        case StatusRekrutmen::DITOLAK: return "Ditolak";
        default: return "Tidak Diketahui";
    }
}

struct PermintaanRekrutmen {
    std::string idPermintaan;
    std::string namaPelamar;
    std::string jabatanYangDilamar;
    std::string tanggalPengajuan;
    StatusRekrutmen status;

    PermintaanRekrutmen(std::string nama = "", std::string jabatan = "", std::string id = "")
        : idPermintaan(id), namaPelamar(nama), jabatanYangDilamar(jabatan), status(StatusRekrutmen::DIAJUKAN) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss_readable_ts;
        ss_readable_ts << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        tanggalPengajuan = ss_readable_ts.str();
    }
};

struct NodeRekrutmen {
    PermintaanRekrutmen dataPermintaan;
    NodeRekrutmen* next;
    NodeRekrutmen(const PermintaanRekrutmen& p) : dataPermintaan(p), next(nullptr) {}
};

class QueueRekrutmen {
private:
    NodeRekrutmen* frontNode;
    NodeRekrutmen* rearNode;
    int count;
    std::string namaFileRekrutmen;
    int idRekrutmenCounter;

    StatusRekrutmen stringToStatusRekrutmen(const std::string& statusStr);
    std::string generateNewIdRekrutmen();

public:
    QueueRekrutmen(const std::string& fileRekrutmen = "rekrutmen_data.csv");
    ~QueueRekrutmen();

    bool isEmpty() const;
    int size() const;
    void enqueue(PermintaanRekrutmen& p);
    bool dequeue(PermintaanRekrutmen& p);
    PermintaanRekrutmen peek() const;
    bool updateStatusPermintaan(const std::string& idPermintaan, StatusRekrutmen statusBaru);
    void tampilkanSemuaPermintaan(bool hanyaAktif = false) const;
    void simpanSemuaKeFile() const;
    void muatDariFile();
};

#endif // REKRUTMEN_H