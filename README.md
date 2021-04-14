# Kelompok E09
* 05111940000074 - Nur Ahmad Khatim
* 05111940000087 - William Handi Wijaya
* 05111940000212 - Fadhil Dimas Sucahyo
<br><br>

# Soal 1
## Info
* Dikerjakan oleh 05111940000212 - Fadhil Dimas Sucahyo.
<br><br>

# Soal 2
## Info
* Dikerjakan oleh 05111940000087 - William Handi Wijaya.
* Tidak boleh menggunakan: `system()`, *shell script*, `mkdir()`, dan `rename`.
* Menggunakan `fork` dan `exec`.
  

## Subsoal a
### Penjelasan Soal
Di soal ini, kami disuruh:
1. Extract pets.zip ke dalam folder `/home/[user]/modul2/petshop`.
2. Hapus folder-folder yang ada dalam pets.zip.

### Penyelesaian Soal
#### Perintah 1
1. Ubah current directory menjadi `/home/[user]/modul2/petshop`.
   * Perintah: `chdir("/home/[user]/modul2/petshop")`.
2. Download folder pets.zip jika belum ada.
   * Perintah: `wget --no-check-certificate 'https://docs.google.com/uc?export=download&id=1g5rehatLEkqvuuK_eooHJXB57EfdnxVD -O pets.zip`
3. Extract pets.zip dengan perintah `unzip pets.zip`.

#### Perintah 2
1. Jelajahi folder pets dengan code `listFilesRecursively.c` dari modul 2.
2. Hapus setiap folder yang ditemukan di dalam pets.
   1. Cek tiap path dengan `stat()` dan `S_ISREG`.
   2. Jika path tersebut adalah folder, hapus folder tersebut dengan perintah `rm -r <path>`.


## Subsoal b
### Penjelasan Soal
Di soal ini, kami disuruh:
1. Dapatkan jenis peliharaan dari nama file.
2. Masukan file ke dalam folder sesuai dengan jenis peliharaan.
   * Contoh: jenis peliharaan kucing akan disimpan dalam `/petshop/cat`.

## Penyelesaian Soal
* Semua solusi di bawah ini dijalankan dalam fungsi `listFilesRecursively.c`.

### Perintah 1
1. Pastikan file bukan berupa folder.
2. Dapatkan nama file.
3. Dapatkan jenis peliharaan dari nama file dengan perintah `strtok(<namafile>, ';')`.

### Perintah 2

<br><br>

# Soal 3
## Info
* Dikerjakan oleh 05111940000074 - Nur Ahmad Khatim.
<br><br>