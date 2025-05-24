// stack_aksi.cpp
#include "stack_aksi.h"
#include <stdexcept> 

StackAksi::StackAksi() : top(nullptr), count(0) {}

StackAksi::~StackAksi() {
    clear();
}

void StackAksi::clear() {
    NodeStack* current = top;
    NodeStack* nextNode = nullptr;
    while (current != nullptr) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
    top = nullptr;
    count = 0;
}

void StackAksi::push(const AksiUndo& aksi) {
    NodeStack* newNode = new NodeStack(aksi);
    newNode->next = top;
    top = newNode;
    count++;
}

AksiUndo StackAksi::pop() {
    if (isEmpty()) {
        throw std::runtime_error("Error: Stack kosong, tidak bisa pop.");
    }
    NodeStack* temp = top;
    AksiUndo aksiDihapus = top->dataAksi;
    top = top->next;
    delete temp;
    count--;
    return aksiDihapus;
}

AksiUndo StackAksi::peek() const {
    if (isEmpty()) {
        throw std::runtime_error("Error: Stack kosong, tidak bisa peek.");
    }
    return top->dataAksi;
}

bool StackAksi::isEmpty() const {
    return top == nullptr;
}

int StackAksi::getSize() const {
    return count;
}