// permintaan.h
#ifndef PERMINTAAN_H
#define PERMINTAAN_H

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

enum class StatusPermintaan {
    DIAJUKAN,
    DIPROSES,
    DISETUJUI,
    DITOLAK
};

inline std::string statusPermintaanToString(StatusPermintaan status) {
    switch (status) {
        case StatusPermintaan::DIAJUKAN: return "Diajukan";
        case StatusPermintaan::DIPROSES: return "Diproses";
        case StatusPermintaan::DISETUJUI: return "Disetujui";
        case StatusPermintaan::DITOLAK: return "Ditolak";
        default: return "Tidak Diketahui";
    }
}

struct Permintaan {
    std::string idPermintaanInternal; 
    std::string idKaryawanPengaju;
    std::string tipePermintaan;
    std::string detailPermintaan;
    std::string timestampPengajuan;
    StatusPermintaan status;
    std::string catatanAdmin;

    Permintaan(std::string idKaryawan = "", std::string tipe = "", std::string detail = "", std::string idReq = "")
        : idPermintaanInternal(idReq), idKaryawanPengaju(idKaryawan), tipePermintaan(tipe), detailPermintaan(detail), status(StatusPermintaan::DIAJUKAN) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss_readable_ts;
        ss_readable_ts << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        timestampPengajuan = ss_readable_ts.str();
    }
};

#endif // PERMINTAAN_H