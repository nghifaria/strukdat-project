g++ main.cpp linkedlist_karyawan.cpp stack_aksi.cpp manajemen_pengguna.cpp hierarki_jabatan.cpp queue_permintaan.cpp rekrutmen.cpp -o aplikasi_karyawan.exe

# Aplikasi Pengelolaan Data Karyawan

Aplikasi konsol C++ ini dirancang sebagai proyek akhir untuk mata kuliah Struktur Data (KOM120H) IPB University. Proyek ini mendemonstrasikan implementasi dan pemanfaatan berbagai struktur data dan algoritma fundamental dalam skenario pengelolaan data karyawan yang realistis.

## Fitur Utama

Aplikasi ini dilengkapi dengan serangkaian fitur komprehensif untuk manajemen data karyawan dan pengguna:

* **Manajemen Pengguna yang Aman:**
    * **Registrasi & Login:** Pengguna dapat mendaftar dan masuk ke sistem dengan peran `ADMIN` atau `PENGGUNA`.
    * **Keamanan Password:** Password pengguna dienkripsi menggunakan **Caesar Cipher** sederhana sebelum di-hash menggunakan fungsi **DJB2 Hash** (implementasi mandiri) dan disimpan secara persisten.
    * **Manajemen Akun:** Admin dapat mendaftarkan pengguna baru (termasuk admin lain), menghapus pengguna, dan memantau daftar pengguna.
    * **Pemisahan Hak Akses:** Fitur dan menu yang ditampilkan disesuaikan secara dinamis berdasarkan peran (`ADMIN` atau `PENGGUNA`) dan status karyawan (`KARYAWAN` atau `BUKAN_KARYAWAN`).

* **Sistem Rekrutmen Berbasis Antrean:**
    * **Lamaran Pekerjaan:** Pengguna dengan status `BUKAN_KARYAWAN` dapat mengajukan lamaran pekerjaan yang akan masuk ke dalam antrean.
    * **Manajemen Antrean Lamaran:** Admin dapat melihat antrean lamaran aktif, mengubah status lamaran (Diproses, Diterima, Ditolak), dan menambahkan catatan.
    * **Promosi ke Karyawan:** Lamaran yang `DITERIMA` oleh admin akan secara otomatis mempromosikan pengguna pelamar menjadi `KARYAWAN` aktif dengan ID karyawan yang otomatis dihasilkan.

* **Manajemen Data Karyawan (CRUD):**
    * **Kelola Data:** Admin memiliki hak penuh untuk menambah, memperbarui, dan menghapus data karyawan (ID, Nama, Jabatan, Gaji, Atasan, Username Login terkait).
    * **ID Karyawan Otomatis:** ID Karyawan di-generate secara otomatis dan berurutan (`0001`, `0002`, dst.).
    * **CEO Default:** Sistem memiliki data CEO default (`CEO001`) yang tidak dapat dihapus.
    * **Validasi Atasan:** Memastikan ID Atasan yang dimasukkan valid dan mencegah karyawan menjadi atasan bagi dirinya sendiri.
    * **Persistensi Data:** Data karyawan disimpan secara persisten dalam file CSV (`karyawan_data.csv`) menggunakan **Linked List** (implementasi mandiri) untuk penyimpanan di memori.

* **Fitur Sorting & Filtering Data:**
    * **Pengurutan Fleksibel:** Data karyawan dapat diurutkan berdasarkan Nama (Ascending/Descending) atau Gaji (Ascending/Descending) menggunakan algoritma **Quick Sort** (implementasi mandiri).
    * **Pencarian Cepat:** Mencari karyawan berdasarkan bagian dari Nama (case-insensitive).
    * **Filter Data:** Menyaring data karyawan berdasarkan rentang Gaji atau Jabatan (memilih dari daftar jabatan unik yang tersedia).

* **Fitur Undo Aksi Data Karyawan:**
    * **Batalkan Perubahan:** Operasi penambahan, penghapusan, dan pembaruan data karyawan dapat dibatalkan (di-undo) menggunakan **Stack** (implementasi mandiri) yang menyimpan riwayat `AksiUndo`.

* **Visualisasi Hierarki Jabatan:**
    * **Struktur Organisasi:** Membangun dan menampilkan struktur hierarki jabatan perusahaan (siapa melapor kepada siapa) dari data karyawan yang ada menggunakan konsep **Tree** (implementasi mandiri dengan Adjacency Map).
    * **Pencarian Hierarki:** Mencari bawahan langsung, semua bawahan (rekursif), dan menampilkan jalur karyawan ke puncak hierarki.

* **Sistem Permintaan Karyawan:**
    * **Pengajuan Permintaan:** Karyawan aktif dapat mengajukan berbagai jenis permintaan (misal: cuti, reimbursement) yang akan masuk ke antrean.
    * **Manajemen Antrean Permintaan:** Admin dapat melihat, memproses (mengubah status), dan melihat histori permintaan karyawan melalui antrean terpisah.
    * **Persistensi Permintaan:** Data permintaan disimpan dan dimuat dari file CSV.

* **Antarmuka Pengguna (CLI):**
    * Aplikasi berinteraksi melalui antarmuka baris perintah (CLI) yang intuitif dengan sistem menu berjenjang dan validasi input dasar.

## Struktur Data & Algoritma yang Diimplementasikan (Mandiri)


* **Linked List:** Digunakan sebagai penyimpanan utama data karyawan.
* **Stack:** Diimplementasikan untuk fitur `Undo` aksi data karyawan.
* **Queue:** Digunakan untuk sistem antrean permintaan karyawan dan antrean lamaran rekrutmen.
* **Fungsi Hash (DJB2):** Untuk hashing password pengguna.
* **Algoritma Quick Sort:** Untuk mengurutkan data karyawan.
* **Tree (Hierarki Jabatan):** Representasi dan manipulasi struktur organisasi.
* **Caesar Cipher:** Enkripsi sederhana untuk password pengguna.

## Struktur Data / Library Standar C++ yang Digunakan (STL)

* `std::string`: Untuk semua operasi teks.
* `std::vector`: Digunakan sebagai struktur data bantu sementara untuk sorting dan mengumpulkan daftar unik.
* `std::optional`: Untuk menyimpan data karyawan sebelumnya di `AksiUndo`.
* `std::set`: Digunakan untuk mengumpulkan daftar jabatan unik secara efisien.
* `std::map`: Digunakan dalam `PohonJabatan` untuk memetakan ID Karyawan ke node pohon yang sesuai.
* `std::ifstream`, `std::ofstream`, `std::stringstream`: Untuk operasi baca/tulis file CSV.
* `std::limits`, `std::iomanip`, `std::cctype`, `std::algorithm`: Untuk penanganan input, formatting output, manipulasi karakter, dan algoritma umum.
* `std::chrono`: Untuk *timestamp* permintaan.

## Cara Mengkompilasi & Menjalankan

Untuk mengkompilasi aplikasi, navigasikan ke direktori utama proyek di terminal/command prompt Anda dan jalankan perintah `make`:

```bash
make
```
Atau, Anda bisa menggunakan perintah g++ secara langsung:
```bash
g++ main.cpp linkedlist_karyawan.cpp stack_aksi.cpp manajemen_pengguna.cpp hierarki_jabatan.cpp queue_permintaan.cpp rekrutmen.cpp -o aplikasi_karyawan.exe
```
setelah kompilasi berhasil, jalankan aplikasi:
```bash
./aplikasi_karyawan.exe
```
Aplikasi akan secara otomatis membuat file data (pengguna_data.csv, karyawan_data.csv, permintaan_data.csv, rekrutmen_data.csv, karyawan_id_counter.txt) jika belum ada. Saat pertama kali dijalankan, sistem akan membuat akun admin default (username: admin, password: admin).
