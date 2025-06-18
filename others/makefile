CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = aplikasi_karyawan

SRCS = main.cpp \
       linkedlist_karyawan.cpp \
       manajemen_pengguna.cpp \
       stack_aksi.cpp \
       hierarki_jabatan.cpp \
       queue_permintaan.cpp \
       rekrutmen.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)