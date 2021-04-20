# Kelompok E09
* 05111940000074 - Nur Ahmad Khatim
* 05111940000087 - William Handi Wijaya
* 05111940000212 - Fadhil Dimas Sucahyo
<br><br>

# Soal 1
## Info
* Dikerjakan oleh **05111940000087** - William Handi Wijaya dan **05111940000212** - Fadhil Dimas Sucahyo.
* Tidak boleh menggunakan: `system()`, *shell script*, `mkdir()`, dan `rename` (yang di bahasa C).
* Menggunakan `fork` dan `exec`.
* Tidak bolehh menggunakan `cron`.
* Tips:
  * Gunakan fungsi `wait()` dan `sleep()` untuk memperlancar.
  * untuk mendownload zip, bisa menggunakan command:  
    `wget --no-check-certificate "https://drive.google.com/uc?id=< ID-FILE >&export=download" -O < Nama_untuk_filenya.ext >`

## Subsoal a
### Penjelasan Soal
Di soal ini, kami disuruh:
* Membuat folder:
  * **Musyik** untuk mp3
  * **Fylm** untuk mp4
  * **Pyoto** untuk jpg

### Penyelesaian Soal
* Buat ketiga folder tersebut di *current directory* menggunakan perintah `mkdir` pada `exec`.

## Subsoal b
### Penjelasan Soal
Di soal ini, kami disuruh:
* Download file zip dari masing-masing folder melalui link berikut:
  * Musik: <https://drive.google.com/file/d/1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J/view>
  * Foto: <https://drive.google.com/file/d/1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD/view>
  * Film: <https://drive.google.com/file/d/1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp/view>

### Penyelesaian Soal
1. Dapatkan id file dari masing-masing folder.
2. Buat link untuk mendownload file berdasarkan id file.
3. Download file zip menggunakan command `wget` ke folder sementara bernama `tmpDir`.

## Subsoal c
### Penjelasan Soal
Di soal ini, kami disuruh:
* Extract masing-masing zip.

### Penyelesaian Soal
* Extract masing-masing zip di folder `tmpDir` dengan perintah `unzip`.

## Subsoal d
### Penjelasan Soal
Di soal ini, kami disuruh:
* Pindahkan file ke folder yang sesuai.

## Penyelesaian Soal
1. Scan file di dalam folder `tmpDir` dan dapatkan ekstensinya. 
2. Untuk setiap ekstensi, lakukan perintah ini:
   1. Jika file berekstensi mp3, masukkan ke folder **Musyik**.
   2. Jika file berekstensi mp4, masukkan ke folder **Fylm**.
   3. Jika file berekstensi jpg, masukkan ke folder **Pyoto**.
3. Jika telah selesai, hapus folder `tmpDir`.

<br><br>

# Soal 2
## Info
* Dikerjakan oleh 05111940000087 - William Handi Wijaya.
* Tidak boleh menggunakan: `system()`, *shell script*, `mkdir()`, dan `rename`.
* Menggunakan `fork` dan `exec`.
  

## Subsoal a
### Penjelasan Soal
Di soal ini kami disuruh:
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
Di soal ini kami disuruh:
1. Dapatkan jenis peliharaan dari nama file.
2. Masukan file ke dalam folder sesuai dengan jenis peliharaan.
   * Contoh: jenis peliharaan kucing akan disimpan dalam `/petshop/cat`.

### Penyelesaian Soal
Semua solusi di bawah ini dijalankan dalam fungsi yang menyerupai `listFilesRecursively.c`, yaitu fungsi `categorize`.

#### Perintah 1
1. Pastikan file bukan berupa folder.
2. Dapatkan nama file.
3. Dapatkan jenis peliharaan dari nama file dengan perintah `strtok(<namafile>, ';')`.

#### Perintah 2
1. Cek apakah folder jenis peliharaan sudah terbuat.
2. Jika belum:
   1. Buat folder jenis peliharaan dengan perintah `cp -r /home/<user>/log <jenis peliharaan>`.
   2. Bersihkan folder `<jenis peliharaan>`.
3. Pindahkan file ke folder jenis peliharaan dengan perintah `mv <nama file> <jenis peliharaan>`.


## Subsoal c
### Penjelasan Soal
Di soal ini kami disuruh:
* Rename nama file sesuai dengan nama peliharaannya.  
  Contoh: `petshop/cat/joni.jpg`.

### Penyelesaian Soal
Solusi di bawah ini dilakukan di dalam fungsi `categorize`, sebelum file dipindahkan.
1. Dapatkan nama hewan.
2. Copy file menjadi file baru bernama `<nama peliharaan>`.
3. Pindahkan (*copy*) file `<nama peliharaan>` ke folder yang tepat.
4. Hapus file lama.


## Subsoal d
### Penjelasan Soal
Di soal ini kami disuruh:
1. Dapatkan jenis dan nama peliharaan kedua.
2. Pindahkan file tersebut ke folder yang tepat untuk kedua kalinya.

### Penyelesaian Soal
Solusi ini akan memodifikasi solusi pada subsoal c.
* Jika terdapat peliharaan kedua:
   1. Lakukan solusi no.1-3 subsoal c untuk peliharaan kedua.
   2. Setelah kedua file terpindah, hapus file lama.


## Subsoal e
### Penjelasan Soal
Di soal ini kami disuruh:
1. Membuat file `keterangan.txt` di setiap folder jenis peliharaan.
2. Isi file `keterangan.txt` dengan nama dan umur semua peliharaan di folder yang sama.  
   Format:
   > nama : joni  
   > umur : 3 tahun
   >
   > nama : miko  
   > umur: 2 tahun
   >

### Penyelesaian Soal
Solusi ini di antara solusi no.1 dan no.2 pada subsoal d.
1. Dapatkan umur.
2. Jika file `keterangan.txt` tidak ada, buat file tersebut.
3. Isi `keterangan.txt` sesuai dengan format.
<br><br>

# Soal 3
## Info
* Dikerjakan oleh 05111940000074 - Nur Ahmad Khatim.
* Tidak boleh menggunakan: `system()` dan `mkdir()`.
* **Program utama** merupakan **SEBUAH PROGRAM C**.
* **Wajib** membuat algoritma Caesar Cipher pada program utama yang dibuat.


## Subsoal a
### Penjelasan Soal
1. Membuat sebuah program C yang membuat sebuah direktori setiap 40 detik.
2. Penamaan direktori sesuai timestamp [YYYY-mm-dd_HH-ii-ss].

### Penyelesaian Soal
1. Membuat while looping yang berjalan setiap 40 detik dengan bantuan variabel time_t dan fungsi difftime().
2. Fork child process.
3. Menjalankan perintah membuat directory `mkdir` dengan penamaan sesuai timestamp dengan bantuan fungsi strftime() dan execv().


## Subsoal b
### Penjelasan Soal
1. Mengisi setiap direktori dengan 10 gambar yang didownload dari https://picsum.photos/ setiap 5 detik.
2. Setiap gambar diberi nama sesuai timestamp [YYYY-mm-dd_HH-ii-ss].
3. Gambar berukuran (n%1000) + 50 pixel dengan n adalah detik Epoch Unix.

## Penyelesaian Soal
1. Mengambil detik Epoch Unix `int epoch = ((int)time(NULL) % 1000) + 50;`.
2. Membuat while looping yang berjalan setiap 5 detik dengan bantuan variabel time_t dan fungsi difftime().
3. Fork child process.
4. Menjalankan perintah download `wget` dengan penamaan sesuai timestamp dengan bantuan fungsi strftime() dan execv() serta ditempatkan sesuai path direktori.
5. While looping dibreak setelah download 10 gambar.


## Subsoal c
### Penjelasan Soal
1. Setelah direktori terisi 10 gambar, program membuat sebuah file `status.txt`.
2. File tersebut berisi pesan "Download Success"
3. Pesan terenkripsi dengan teknik Caesar Cipher dan dengan shift 5.
4. Setelah file tersebut dibuat, direktori dizip dan direktori didelete.

### Penyelesaian Soal
1. Mengenkripsi text "Download Success" menggunakan algoritma Caesar Cipher.
2. Membuat file `status.txt` `FILE *statustext = fopen(path_status, "w");`.
3. Mengisi file dengan text yang sudah dienkripsi.
4. Zip direktori dengan perintah `zip` dengan option `-rm` dan bantuan fungsi execv().


## Subsoal d
### Penjelasan Soal
1. Program dapat men-generate sebuah program "Killer" yang executable.
2. Program "Killer" akan menterminasi semua proses program yang sedang berjalan dan menghapus dirinya sendiri setelah program dijalankan.
3. Program "Killer" merupakan **program bash**.

### Penyelesaian Soal
1. Membuat sebuah FILE `Killer.sh`
2. Mengisi file tersebut dengan perintah `kill` sesuai pid yang didapatkan dari fungsi getpid().
3. Menambahkan isi file dengan perintah `rm -- "$0"` untuk menghapus diri sendiri.


## Subsoal e
### Penjelasan Soal
1. Program utama dapat dijalankan di dua mode.
2. Mode pertama dijalankan dengan argumen -z, program utama langsung menghentikan semua operasinya ketika program "Killer" dijalankan.
3. Mode kedua dijalankan dengan argumen -x, program utama berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai.

### Penyelesaian Soal
1. Menjadikan fungsi main() dapat menerima argumen.
2. Mengecek argumen yang diinputkan menggunakan fungsi strcmp().
3. Untuk mode = 1, mengambil pid dengan getpid() dari process utama.
4. Jika mode = 1, maka FILE `Killer.sh` dibuat pada saat Fork child process untuk membuat direktori.
5. Jika mode = 2, hanya perlu pid dari process utama dan akan membuat FILE `Killer.sh` pada saat itu juga.
<br><br>
