// karyawan.h
#ifndef KARYAWAN_H
#define KARYAWAN_H

#include <string>

struct Karyawan {
    std::string idKaryawan;
    std::string namaKaryawan;
    std::string jabatan;
    double gaji;
    std::string idAtasan; 

    Karyawan() : gaji(0.0) {} 
};

#endif // KARYAWAN_H