# Compiler dan flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Nama output file
TARGET = aplikasi_karyawan

# Semua source file .cpp
SRCS = main.cpp \
       linkedlist_karyawan.cpp \
       manajemen_pengguna.cpp \
       stack_aksi.cpp \
       hierarki_jabatan.cpp \
       queue_permintaan.cpp

# File object hasil kompilasi
OBJS = $(SRCS:.cpp=.o)

# Build rules
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile setiap .cpp jadi .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Bersihkan file hasil build
clean:
	rm -f $(TARGET) $(OBJS)