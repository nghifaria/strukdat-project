// queue_permintaan.h
#ifndef QUEUE_PERMINTAAN_H
#define QUEUE_PERMINTAAN_H

#include "permintaan.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip> 


struct NodeQueue {
    Permintaan dataPermintaan;
    NodeQueue* next;

    NodeQueue(const Permintaan& p) : dataPermintaan(p), next(nullptr) {}
};

class QueuePermintaan {
private:
    NodeQueue* frontNode;
    NodeQueue* rearNode;
    int count;
    std::string namaFilePermintaan;
    int idPermintaanCounter; 

    StatusPermintaan stringToStatusPermintaan(const std::string& statusStr);
    std::string generateNewIdPermintaan();


public:
    QueuePermintaan(const std::string& filePermintaan = "permintaan_data.csv");
    ~QueuePermintaan();

    bool isEmpty() const;
    int size() const;
    void enqueue(Permintaan& p); 
    bool dequeue(Permintaan& p);
    Permintaan peek() const;
    void tampilkanSemuaPermintaan(bool hanyaAktif = false) const;
    NodeQueue* getFrontNode() const { return frontNode; }
    bool updateStatusPermintaan(const std::string& idPermintaan, StatusPermintaan statusBaru, const std::string& catatanAdmin);
    bool prosesPermintaanSelesai(const std::string& idPermintaan);


    void simpanSemuaKeFile() const;
    void muatDariFile();
};

#endif // QUEUE_PERMINTAAN_H