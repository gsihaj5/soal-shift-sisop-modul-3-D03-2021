# soal-shift-sisop-modul-3-D03-2021

# LAPORAN PENGERJAAN SOAL SHIFT

# ============== NO 1 ===============
Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini.

## Soal a
Pada soal a, server memberi pilihan register atau login pada client. Lalu server juga harus dapat menerima multiconnections.

server.c
```c
void register_account(int client_socket, char id[], char password[], int *loggedUser, int client_socket_serving) {
	int activity1 = recv(client_socket, id, SIZE, 0);
	int activity2 = recv(client_socket, password, SIZE, 0);
	int activity_status;
	if(check_IDPass(id, password))
		activity_status = send(client_socket_serving, "user found..\n", SIZE, 0);
	else {
		*loggedUser = 1;
		FILE *app = fopen("akun.txt", "a+");
		fprintf(app, "%s:%s\n", id, password);
		fclose(app);
		activity_status = send(client_socket_serving, "regloginsuccess..\n", SIZE, 0);
	}
}


void login_account(int client_socket, char id[], char password[], int *loggedUser, int client_socket_serving) {
	int activity1 = recv(client_socket, id, SIZE, 0);
	int activity2 = recv(client_socket, password, SIZE, 0);
	int activity_status;
	if(check_IDPass(id, password))
		activity_status = send(client_socket_serving, "wrongpassword\n", SIZE, 0);
	else {
		*loggedUser=1;
		activity_status = send(client_socket_serving, "regloginsuccess..\n", SIZE, 0);
	}
}
```
```c
	...
	...
	if((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		perror("socket creation failed...\n");
		exit(EXIT_FAILURE);
	}
	
	else
		printf("Socket successfully created..\n");
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(8080);
	
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) != 0) {
		perror("bind failed...\n");
		exit(EXIT_FAILURE);
	}
	if(listen(server_fd, 5) != 0) {
		perror("listen failed...\n");
		exit(EXIT_FAILURE);
	}
	
	for(int i=0;i<max_clients;i++) {
        client_socket[i] = -1;
    }
    client_socket[0] = server_fd;
	printf("\nServer is running....\n\n");
	
	int loggedUser = 0;
	while(1) {
		FD_ZERO(&readfds);
		
		for(int i=0;i<max_clients;i++) {
			if(client_socket[i]>=0)
				FD_SET(client_socket[i], &readfds);
		}
		
		activity = select(FD_SETSIZE, &readfds, NULL, NULL, NULL);
		
		else if(activity>=0) {
			if(FD_ISSET(server_fd, &readfds)) {
				if(new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&address_length)>=0) {
					printf("New login try accepted:\n\n");
					for(int i=0;i<max_clients;i++) {
						if(client_socket[i]<0) {
							client_socket[i] = new_socket;
							if(i!=client_serving) 
								activity1 = send(client_socket[i], "wait", SIZE, 0);
							else
								activity1 = send(client_socket[i], "serve", SIZE, 0);
							break;
						}
					}
				}
				else {
					perror("accept failed...\n");
				}
				activity--;
				if(!activity)
					continue;
			}
			
			for(int i=1;i<max_clients;i++) {
				if((client_socket[i] > 0) && (FD_ISSET(client_socket[i], &readfds))) {
					activity2 = recv(client_socket[i], command, sizeof(command), 0);
					printf("Client socket %d, index: %d\n", client_socket[i], i);
					printf("Command: %s\n", command);
					
					if(activity1==0||activity2==0||activity3==0) {
						printf("Current user's ID: %s\n", id);
						printf("Password: %s\n", password);
						printf("Client socket %d is closing..\n", client_socket[i]);
						close(client_socket[i]);
						client_socket[i] = -1;
						
						while(1) {
							if(client_serving == 9) {
								client_serving = 1;
								break;
							}
							if(client_socket[client_serving+1]!=-1) {
								client_serving++;
								break;
							}
							client_serving++;
						}
						loggedUser = 0;
						if(client_socket[client_serving]!=1)
							activity_status = send(client_socket[client_serving], "serve", SIZE, 0);
					}
					if(activity2>0) {
						if(!strcmp(command,"register")) {
							register_account(client_socket[i], id, password, &loggedUser, client_socket[client_serving]);
						}
						else if(!strcmp(command,"login")) {
							login_account(client_socket[i], id, password, &loggedUser, client_socket[client_serving]);
						
						}
						...
						...
```
client.c
```c
...
...
while(strcmp(message, "wait") == 0) {
        printf("\e[31mServer is full!\e[0m\n");
        activity = recv(server_fd, message, SIZE, 0);
}
...
...    
```
# ============== NO 2 ===============
## Soal 📘
Crypto (kamu) adalah teman Loba. Suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya. Detil dari tugas tersebut adalah:

a. Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

b. Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (Catatan!: gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya matri(dari paling besar ke paling kecil) (Catatan!: gunakan thread untuk perhitungan di setiap cel). 

c. Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5” (Catatan!: Harus menggunakan IPC Pipes)

## Cara Pengerjaan 📝

## a.

Keseluruhan proses sebagai berikut:
```c
int main() {
    setupSharedMemory();
    input_matrix_a();
    input_matrix_b();
    cross_mxa_with_mxb();
    print_result();

    shmdt(main_matrix);
}
```

Hal pertama yang dilakukan adalah setup shared memory dengan fungsi 
```c
void setupSharedMemory() {
    key_t key = 1111;
    //size of 4 * 6 -> hasil dari perkalian matrix
    int s_memory_id = shmget(key, sizeof(int[4][6]), 0666 | IPC_CREAT);

    //attach matrix to shared memory
    main_matrix = shmat(s_memory_id, (void *)0, 0);
}
```

kemudian melakukan input dengan fungsi
```c
void input_matrix_a() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 3; col++) {
            scanf("%d", &matrix_a[row][col]);
        }
    }
}

void input_matrix_b() {
    int row, col;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 6; col++) {
            scanf("%d", &matrix_b[row][col]);
        }
    }
}
```

Setelah input melakukan perkalian matrix dengan fungsi
```c
int rainbow_multiplication(int row, int col) {
    int result = 0,
        i;
    for (i = 0; i < 3; i++) {
        result += matrix_a[row][i] * matrix_b[i][col];
    }

    return result;
}

void cross_mxa_with_mxb() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            int rainbow_result = rainbow_multiplication(row, col);
            main_matrix[row][col] = rainbow_result;
        }
    }
}
```

![run soal 2a](/soal2/soal2a.jpeg?raw=true)

## b.
keseluruhan proses sebagai berikut 
```c
int main() {
    setupSharedMemory();
    print_main_matrix();
    get_matrix_from_user();
    calculate_all_pair();
    print_result_matrix();

    shmdt(main_matrix);
}
```

langkah pertama setup shared memory agar bisa menggunakan matrix dari program 2a menggunakan fungsi

```c
void setupSharedMemory() {
    key_t key = 1111;
    //size of 4 * 6 -> dari perkalian matrix soal 2a
    int s_memory_id = shmget(key, sizeof(int[4][6]), 0666 | IPC_CREAT);

    //attach matrix to shared memory
    main_matrix = shmat(s_memory_id, (void *)0, 0);
}
```

kemudian input matrix baru dari user menggunakan fungsi 
```c
void get_matrix_from_user() {
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            scanf("%d", &matrix_user[row][col]);
        }
    }
}
```

setelah itu menghitung semua pair dari matrix yang ada dengan fungsi 

```c
void calculate_all_pair() {
    pthread_t thread[4][6];
    int row, col;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            int arguments[4];
            arguments[0] = main_matrix[row][col];
            arguments[1] = matrix_user[row][col];
            arguments[2] = row;
            arguments[3] = col;

            printf("created thread on %d, %d\n", row, col);
            pthread_create(&thread[row][col], NULL, doFactorial, &arguments);
            sleep(1);
        }
    }

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 6; col++) {
            pthread_join(thread[row][col], NULL);
        }
    }

    return;
}
```
di dalam fungsi diatas ada fungsi doFactorial yang berfungsi untuk menghitung factorial dengan syarat yang ada di soal, fungsi nya sebagai berikut:
```c
void *doFactorial(void *argument) {
    unsigned long int result;
    int *arg = (int *)argument;

    int a = arg[0],
        b = arg[1];

    if (a >= b) {
        result = factorial(a) / factorial(a - b);
    } else if (a < b) {
        result = factorial(a);
    } else if (a == 0 || b == 0) {
        result = 0;
    }

    printf("calculating on %d, %d -> a = %d, b = %d result is %ld\n", arg[2], arg[3], a, b, result);

    factorial_result[arg[2]][arg[3]] = result;
    return NULL;
}
```

fungsi factorial yang dasar sebagai berikut: 
```c
unsigned long int factorial(int num) {
    int i;
    unsigned long int result = 1;
    for (i = 1; i <= num; i++) {
        result *= i;
    }
    return result;
}
```
![run soal 2b input](/soal2/soal2binput.jpeg?raw=true)
![run soal 2b proses](/soal2/soal2bprocess.jpeg?raw=true)
![run soal 2b hasil](/soal2/soal2bhasil.jpeg?raw=true)
## c.

pada awal program yang dilakukan adalah create pipe nya terlebih dahulu
```c
   if (pipe(pipe_1) < 0)
        exit(1);

    if (pipe(pipe_2) < 0)
        exit(1)
```

setelah itu melakukan fork untuk list
```c
pid_t child_id;
    child_id = fork();
    exit_on_failed_fork(child_id);
    if (child_id == 0)
        list_process();

```
list process mengambil list dari system kemudian memasukannya kedalam pipe 1.
fungsi list process sebagai berikut

```c
void list_process() {
    dup2(pipe_1[1], 1);

    close(pipe_1[0]);
    close(pipe_1[1]);

    char *argv[] = {"ps", "aux", NULL};
    execv("/bin/ps", argv);
}
```

setelah itu melakukan fork lagi untuk sorting

```c
 pid_t child_id_2;
    child_id_2 = fork();
    exit_on_failed_fork(child_id_2);

    if (child_id_2 == 0)
        sort_process();

```
sorting dilakukan dengan mengambil input dari pipe 1 dan diteruskan ke pipe2 dengan fungsi
```c
void sort_process() {
    dup2(pipe_1[0], 0);
    dup2(pipe_2[1], 1);

    close(pipe_1[0]);
    close(pipe_1[1]);
    close(pipe_2[0]);
    close(pipe_2[1]);

    char *argv[] = {"sort", "-nrk", "3,3", NULL};
    execv("/usr/bin/sort", argv);
}
```

setelah sorting melakukan fork untuk headprocess

```c
    pid_t child_id_3;
    child_id_3 = fork();
    exit_on_failed_fork(child_id_3);

    if (child_id_3 == 0)
        head_process();
```
head process dilakukan dengan mengambil input dari pipe 2 dan kemudian di outputkan dengan fungsi

```c
void head_process() {
    dup2(pipe_2[0], 0);

    close(pipe_2[0]);
    close(pipe_2[1]);

    char *argv[] = {"head", "-5", NULL};
    execv("/usr/bin/head", argv);
}

```
![run soal 2c](/soal2/soal2c.jpeg?raw=true)

##kesulitan/masalah

1. kebingungan mengalokasikan memory yang shared.
2. kebingungan saat bind shared memory dengan array yang ada di dalam program

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
void *prosesF(void *temp) {
    char *path;
    path = (char *)temp;
    
    char *namaFile;
    namaFile= basename(path); //lgens

    char cwd[1000];
    getcwd(cwd,sizeof(cwd)); //unistd.h

    char namaBaru[1000];
    char ektensiFile[500];

    strcpy(ektensiFile, getEkstensiFile(namaFile));
    strcpy(ektensiFile, lowerCase(ektensiFile));

    sprintf(namaBaru, "%s/%s", cwd, ektensiFile);
    createDirIfNotExist(namaBaru);

    sprintf(namaBaru, "%s/%s", namaBaru,namaFile);

    int statusF = rename(path,namaBaru);
    if(statusF==0) {
        printf("Berhasil Dikategorikan\n");
    } else {
        printf("Sad, gagal :( \n");
    }


}

```

untuk pembuatan folder ketika belum ada
```c
void createDirIfNotExist(char *dir) {
	struct stat st = {0};

	if (stat(dir, &st) == -1) {
			mkdir(dir, 0777);
	}
}
```

untuk mendapatkan ekstensi file
```c
char *getEkstensiFile(char *filename) {
    char *dot = strrchr(filename, '.'); //last occurance char
		// If dot is NULL or dot doesn't exist
    if(!dot || dot == filename) return "Unknown";
    return dot + 1;
}
```

kode lengkap implementasi pengerjaan [soal3.c](https://github.com/gsihaj5/soal-shift-sisop-modul-3-D03-2021/blob/master/soal3/soal3.c)

## Kendala 🕳
* Masih belum berhasil menerapkan rencana pengerjaan untuk soal 3a
* belum dapat mengerjakan soal 3b,c,d,e
<img src="https://github.com/widigdacahya/gif/blob/main/belumbisa.PNG" width="700">

## Setelah Revisi
* Kegiatan pada soal 3a(pemindahan file ke dalam folder dengan judul ekstensi) bisa dilakukan
![nomer1revisi](https://github.com/widigdacahya/gif/blob/main/ezgif.com-gif-maker.gif?raw=true)


