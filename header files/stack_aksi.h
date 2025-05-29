// stack_aksi.h
#ifndef STACK_AKSI_H
#define STACK_AKSI_H

#include "karyawan.h"
#include <string>
#include <optional> 

enum class TipeAksi {
    TAMBAH,
    HAPUS,
    UPDATE
};

struct AksiUndo {
    TipeAksi tipe;
    Karyawan dataKaryawanUtama;
    std::optional<Karyawan> dataKaryawanSebelum; 

    AksiUndo(TipeAksi t, const Karyawan& kUtama) : tipe(t), dataKaryawanUtama(kUtama), dataKaryawanSebelum(std::nullopt) {}
    AksiUndo(TipeAksi t, const Karyawan& kUtama, const Karyawan& kSebelum) : tipe(t), dataKaryawanUtama(kUtama), dataKaryawanSebelum(kSebelum) {}
};

struct NodeStack {
    AksiUndo dataAksi;
    NodeStack* next;
    NodeStack(const AksiUndo& aksi) : dataAksi(aksi), next(nullptr) {}
};

class StackAksi {
private:
    NodeStack* top;
    int count;
public:
    StackAksi();
    ~StackAksi();
    void push(const AksiUndo& aksi);
    AksiUndo pop();
    AksiUndo peek() const;
    bool isEmpty() const;
    int getSize() const;
private:
    void clear();
};
#endif // STACK_AKSI_H