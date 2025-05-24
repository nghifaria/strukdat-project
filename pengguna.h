// pengguna.h
#ifndef PENGGUNA_H
#define PENGGUNA_H

#include <string>

struct Pengguna {
    std::string username;
    unsigned long hashedPassword; // Menyimpan hash dari password
    // Bisa ditambahkan role atau atribut lain nanti
};

#endif // PENGGUNA_H