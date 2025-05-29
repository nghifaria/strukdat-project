// hierarki_jabatan.h
#ifndef HIERARKI_JABATAN_H
#define HIERARKI_JABATAN_H

#include "karyawan.h"
#include "linkedlist_karyawan.h"
#include <string>
#include <vector>
#include <map>

struct NodeTreeJabatan {
    Karyawan dataKaryawan;
    std::vector<NodeTreeJabatan*> anak;

    NodeTreeJabatan(const Karyawan& k) : dataKaryawan(k) {}
    ~NodeTreeJabatan() {
        
    }
};

class PohonJabatan {
private:
    NodeTreeJabatan* root;
    std::map<std::string, NodeTreeJabatan*> petaNode;

    void tampilkanHierarkiRecursive(NodeTreeJabatan* node, int level) const;
    void cariBawahanRecursive(NodeTreeJabatan* node, std::vector<Karyawan>& hasil) const;
    bool cariJalurKePuncakRecursive(NodeTreeJabatan* nodeSaatIni, const std::string& idKaryawanTarget, std::vector<Karyawan>& jalur) const;


public:
    PohonJabatan();
    ~PohonJabatan();

    void bangunDariLinkedList(const LinkedListKaryawan& daftarKaryawan, const std::string& idCEOInput = "");
    void tampilkanSeluruhHierarki() const;
    std::vector<Karyawan> cariBawahanLangsung(const std::string& idAtasan) const;
    std::vector<Karyawan> cariSemuaBawahan(const std::string& idAtasan) const;
    std::vector<Karyawan> getJalurKePuncak(const std::string& idKaryawan) const;
    void bersihkanPohon();
    bool petaNodeContainsKey(const std::string& key) const;
};

#endif // HIERARKI_JABATAN_H