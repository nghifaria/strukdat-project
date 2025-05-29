// pengguna.h
#ifndef PENGGUNA_H
#define PENGGUNA_H

#include <string>

enum class TipePeran {
    PENGGUNA,
    ADMIN
};

struct Pengguna {
    std::string username;
    unsigned long hashedPassword;
    TipePeran peran;

    Pengguna() : hashedPassword(0), peran(TipePeran::PENGGUNA) {}
    Pengguna(std::string uname, unsigned long hPass, TipePeran prn)
        : username(uname), hashedPassword(hPass), peran(prn) {}
};

#endif // PENGGUNA_H