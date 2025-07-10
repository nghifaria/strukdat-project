# Aplikasi Pengelolaan Data Karyawan (CLI)

## Deskripsi Proyek

Aplikasi konsol C++ ini dirancang sebagai Proyek Akhir untuk mata kuliah Struktur Data. Proyek ini mendemonstrasikan implementasi dan pemanfaatan berbagai struktur data serta algoritma fundamental dalam skenario sistem manajemen sumber daya manusia yang realistis, yang ditujukan untuk membantu Usaha Mikro, Kecil, dan Menengah (UMKM) dalam mengelola data karyawan secara efisien.
## Fitur Utama

- **Manajemen Pengguna Berbasis Peran:**
  - Sistem registrasi dan login yang aman untuk `ADMIN`, `KARYAWAN` (pengguna terdaftar), dan `USER` (pelamar).
  - Keamanan password menggunakan kombinasi **Caesar Cipher** dan fungsi **Hash DJB2**.
  - Hak akses dinamis yang disesuaikan berdasarkan peran pengguna.

- **Manajemen Data Karyawan (CRUD):**
  - Fungsi penuh untuk menambah, membaca, memperbarui, dan menghapus (CRUD) data karyawan.
  - ID Karyawan unik yang di-generate secara otomatis.
  - Persistensi data yang andal, disimpan dalam format file `.csv`.

- **Fitur Lanjutan:**
  - **Undo Aksi:** Kemampuan untuk membatalkan aksi terakhir (tambah, hapus, update) menggunakan struktur data **Stack**.
  - **Visualisasi Hierarki Jabatan:** Membangun dan menampilkan struktur organisasi perusahaan dalam format **Tree**.
  - **Sorting & Filtering:** Mengurutkan data karyawan berdasarkan nama atau gaji menggunakan **Quick Sort**, serta memfilter berdasarkan rentang gaji atau jabatan.
  - **Sistem Antrean Ganda:**
    - **Antrean Rekrutmen:** Mengelola lamaran pekerjaan dari pelamar baru.
    - **Antrean Permintaan Internal:** Mengelola permintaan dari karyawan (misal: cuti, reimbursement).

## Struktur Data & Algoritma yang Digunakan

- **Struktur Data (Implementasi Mandiri):**
  - **Linked List:** Penyimpanan utama data karyawan.
  - **Stack:** Untuk fitur Undo.
  - **Queue:** Untuk sistem antrean rekrutmen dan permintaan internal.
  - **Tree:** Untuk representasi hierarki jabatan (dibantu `std::map` dan `std::vector`).

- **Algoritma:**
  - **Quick Sort:** Untuk sorting data karyawan.
  - **DJB2 Hash & Caesar Cipher:** Untuk keamanan password.

- **Library Standar C++ (STL):**
  - `std::vector`, `std::map`, `std::set`, `std::string`, `std::optional`, `std::ifstream`, `std::ofstream`, `std::stringstream`, `std::chrono` dan lainnya.

## Kompilasi dan Eksekusi

Proyek ini dapat dikompilasi menggunakan `g++` dengan `Makefile` yang telah disediakan.

1.  **Kompilasi dengan Makefile:**
    ```bash
    make
    ```

2.  **Kompilasi Manual:**
    ```bash
    g++ main.cpp linkedlist_karyawan.cpp stack_aksi.cpp manajemen_pengguna.cpp hierarki_jabatan.cpp queue_permintaan.cpp rekrutmen.cpp -o aplikasi_karyawan
    ```

3.  **Jalankan Aplikasi:**
    ```bash
    ./aplikasi_karyawan
    ```

Aplikasi akan secara otomatis membuat file data yang diperlukan (`.csv`) jika belum ada. Akun admin default saat pertama kali dijalankan adalah:
- **Username:** `admin`
- **Password:** `admin`
