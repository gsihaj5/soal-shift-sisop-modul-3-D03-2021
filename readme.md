# soal-shift-sisop-modul-3-D03-2021

# LAPORAN PENGERJAAN SOAL SHIFT

# ============== NO 1 ===============

# ============== NO 2 ===============

# ============== NO 3 ===============
## Soal 📘
Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.
Contoh apabila program dijalankan:
<img src="https://github.com/widigdacahya/gif/blob/main/ketsoal.PNG" width="700">

a.	Program menerima opsi -f seperti contoh di atas, jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. 
Output yang dikeluarkan adalah seperti ini :
```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
```

b.	Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. Contohnya adalah seperti ini:
```
$ ./soal3 -d /path/to/directory/
Perintah di atas akan mengkategorikan file di /path/to/directory, lalu hasilnya akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory).
Output yang dikeluarkan adalah seperti ini :
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“
```

c.	Selain menerima opsi-opsi di atas, program ini menerima opsi *, contohnya ada di bawah ini:
```
$ ./soal3 \*
```
Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.

d.	Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.

e.	Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.
Namun karena Ayub adalah orang yang hanya bisa memberi ide saja, tidak mau bantuin buat bikin programnya, Ayub meminta bantuanmu untuk membuatkan programnya. Bantulah agar program dapat berjalan!

Catatan: 
*	Kategori folder tidak dibuat secara manual, harus melalui program C
*	Program ini tidak case sensitive. Contoh: JPG dan jpg adalah sama
*	Jika ekstensi lebih dari satu (contoh “.tar.gz”) maka akan masuk ke folder dengan titik terdepan (contoh “tar.gz”)
*	Dilarang juga menggunakan fork-exec dan system()
*	Bagian b dan c berlaku rekursif


## Cara Pengerjaan 📝
Rencana pengerjaan untuk ketentuan -f adalah dengan mendapatkan nama file dan ekstensinya kemudian melakukan pemindahan file tersebut kesebuah folder, dimana untuk proses pemindahan ini akan meninjau keberadaan folder dengan nama ekstensi tersebut terlebih dahulu, jika belumum ada, maka diperlukan proses pembuatan folder terlebih dahulu

```c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <ctype.h>

char *ekstensiFile(char *namaFile) {
    
    char *dot = strrchr(namaFile, '.');
    
    if(dot==namaFile || !dot) {
        return "Unknown";
    }
    
    return dot+1;
}

char *lowerCase(char *nameToLow) {
    for(char *ch=nameToLow; *ch; ch++) {
        *ch=tolower(*ch);
    }
    return nameToLow;
}

int main(int argc, char* argv[]) {

    /*
    pthread_t tf;
                // un,un,fungsi,argumen fungsinya
    if ( pthread_create(&tf,NULL,&fun1,NULL) != 0 ){
        return 1;
    }
    
    pthread_join(tf,NULL);
    */

   if(strcmp (argv[1], "-f")==0 ) {
       //char *outputFile = basename(argv[0]);
        int pos;
        for(pos=2; pos<argc; pos++) {
            
            char *dirPath;
            dirPath = (char*) argv[pos];

            char *fileOperasi;
            fileOperasi=basename(dirPath);

            char ekstensiFileOperasi[25];
            strcpy(ekstensiFileOperasi, ekstensiFile(fileOperasi));
            strcpy(ekstensiFileOperasi, lowerCase(ekstensiFileOperasi));

            char newFileOperasi[1000];
            char currLoc[1000];
            getcwd(currLoc,sizeof(currLoc));
            sprintf(newFileOperasi, "%s/%s", currLoc, fileOperasi );

            //membuat file
            struct stat dirStat = {0};

            if(stat(newFileOperasi,&dirStat)==-1) {
                mkdir(newFileOperasi, 0777);
            }

            sprintf(newFileOperasi, "%s/%s", newFileOperasi, fileOperasi);

            int ket = rename(dirPath, newFileOperasi);
            if(ket != 0) {
                printf("Sad, gagal :( \n");
            } else {
                printf("Berhasil Dikategorikan\n");
            }
        }
   }
    
    return 0;
}

```
kode lengkap implementasi pengerjaan [soal3.c](https://github.com/gsihaj5/soal-shift-sisop-modul-3-D03-2021/blob/master/soal3/soal3.c)

## Kendala 🕳
* Masih belum berhasil menerapkan rencana pengerjaan untuk soal 3a
* belum dapat mengerjakan soal 3b,c,d,e
<img src="https://github.com/widigdacahya/gif/blob/main/belumbisa.PNG" width="700">


