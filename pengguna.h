// pengguna.h
#ifndef PENGGUNA_H
#define PENGGUNA_H

#include <string>

struct Pengguna {
    std::string username;
    unsigned long hashedPassword;
};

#endif // PENGGUNA_H