#include <stdio.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/time.h>
#include <dirent.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#define PORT 8080
#define SIZE 100

void log_(char command[], char file_name[], char id[], char password);
void get_directory(char directory[], char file_name[]);
int check_IDPass(char id[], char password[]);
char *strrev(char *str);
int delete_tsv(char file_name[]);
void register_account(int client_socket, char id[], char password[], int *loggedUser, int cliet_serving);
void login_account(int client_socket, char id[], char password[], int *loggedUser, int cliet_socket_serving);
void add_command(int sender, int receiver, char id[], char password[]);
void delete_command(int sender, int receiver, char id[], char password[]);
void download_command(int senerd, int receiver);
void send_file(int sockfd, char filePath[]);

int main(int argc, char const *argv[]) {
	int server_fd, new_socket, client_socket[30], max_clients = 30;
	struct sockaddr_in address;
	int opt = 1;
	int address_length = sizeof(address);
	int max_sd, sd, client_serving=1;
	char command[SIZE], id[SIZE], password[SIZE];
	char buffer[1024] = {0};
	fd_set readfds;
	int activity, activity1, activity2, activity3, activity4, activity_status;
	
	for(int i=0;i<max_clients;i++)
		client_socket[i]=0;
	
	if(access("akun.txt", F_OK) != 0) {
		FILE *fp = fopen("akun.txt", "w+");
		fclose(fp);
	}
	
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket creation failed...");
		exit(EXIT_FAILURE);
	}
	
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	else
		printf("Socket successfully created..\n");
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perrer("bind failed...");
		exit(EXIT_FAILURE);
	}
	if(listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	
	int loggedUser = 0;
	while(1) {
		FD_ZERO(&readfds);
		
		FD_SET(server_fd, &readfds);
		max_sd = server_fd;
		
		for(int i=0;i<max_clients;i++) {
			sd = client_socket[i];
			
			if(sd > 0)
				FD_SET(client_socket[i], &readfs);
			if(sd > max_sd)
				max_sd = sd;
		}
		
		activity = select(max_sd+1, &readfds, NULL, NULL, NULL);
		
		if((activity<0) && (errno!=EINTR))
			printf("select error...");
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
					perror("accept failed...");
				}
				activity--;
				if(!activity)
					continue;
			}
			
			for(int i=1;i<max_client;i++) {
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
						if(client_socket[client_serving]!-=1)
							activity_status = send(client_socket[client_serving], "serve", SIZE, 0);
					}
					if(activity2>0) {
						if(!strcmp(command,"register")) {
							register_account(client_socket[i], id, password, &loggedUser, client_socket[client_serving]);
						}
						else if(!strcmp(command,"login")) {
							login_account(client_socket[i], id, password, &loggedUser, client_socket[client_serving]);
						
						}
						else {
							if(loggedUser) {
								printf("You are permitted to access these commands\n\n");
								if(!strcmp(command,"add")){
									add_command(client_socket[i], client_socket[client_serving], id, password);
								}
								else if(!strcmp(command,"download")) {
									download_commad(client_socket[i], client_socket[client_serving]);
								}
								else if(!strcmp(command,"delete")) {
									delete_command(client_socket[i], client_socket[client_serving], id, password)
								}
								else {
									 activity_status = send(client_socket[client_serving], "notlogin\n", SIZE, 0);
									continue;
								}
							}
							
							printf("Current user's ID: %s\n", id);
							printf("Password: %s\n", password);
						}
						if (activity1 == -1 || activity2 == -1 || activity3 == -1) {
							printf("receive() failed for client socket: %d [%s]\n", client_socket[i], strerror(errno));
							break;
						}
					}
					activity1--; activity2--; activity3--;
					if(!activity1)
						continue;
					if(!activity2)
						continue;
					if(!activity3)
						continue;
				}
			}
		}
		for(int i=0;i<max_clients;i++)
			if(client_socket[i]>0)
				close(client_socket[i]);
	}
	
	return 0
}

void log_(char command[], char file_name[], char id[], char password) {
	printf("\n LOG...");
	FILE *fp = fopen("running.log", "a");
	if(!strcmp(command, "add")) {
		printf("Tambah: %s(%s:%s)\n\n", file_name, id, password);
		fprintf(fp, "Tambah: %s(%s:%s)\n\n", file_name, id, password);
	}
	else if(!strcmp(command, "delete")) {
		printf("Hapus: %s(%s:%s)\n\n", file_name, id, password);
		fprintf(fp, "Hapus: %s(%s:%s)\n\n", file_name, id, password);
	}
	fclose(fp);
}

void get_directory(char directory[], char file_name[]) {
    int i = 0;
	int length = strlen(directory)-1;
    while(length){
        directory[length+1] = '\0';
        if(directory[length]=='/'){
            break;
        }
        file_name[i] = directory[length];
        lenth--;
        i++;
    }
}

int check_IDPass(char id[], char password[]) {
	char account[1024], temp[1024];
	FILE *fp = fopen("akun.txt", "r");
	sprintf(account, "%s:%s", id, password);
	while(fscanf(fp,"%s", temp)==1)
		if(strstr(temp, account)!=0)
			return 1;
	fclose(fp);
	return 0;
}

int check_path(char file_name[]){
    char temp[100];
    FILE *fp = fopen("files.tsv", "r");
	
    while(fscanf(fp,"%s", temp) == 1){
		if(strstr(temp, file_name)!=0)
                return 1;
    fclose(fp);
    return 0;
}

char *strrev(char *str)
{
    char *p1, *p2;

    if (! str || ! *str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

int delete_tsv(char file_name[]) {
    FILE *tsv = fopen("files.tsv", "r+");
    FILE *tmp = fopen("temp.tsv", "w+");
    char temp[256], line[256];

	while(fgets(line, 256, tsv) != 0){
        if(sscanf(line, "%255[^\n]", temp) != 1)
			break;
        if(strstr(temp, filename) != 0)
            int b;
		else
            fprintf(tmp, "%s\n", temp);
    }

    while(fgets(line, 256, tsv) != 0){
        if(sscanf(line, "%255[^\n]", temp) != 1)
			break;
        fprintf(tsv, "%s\n", temp);
    }
    remove("files.tsv");
    rename("temp.tsv", "files.tsv");

    fclose(tmp);
    fclose(tsv);
    return 0;
}

void register_account(int client_socket, char id[], char password[], int *loggedUser, int cliet_socket_serving) {
	int activity1 = recv(client_socket, id, sizeof(id), 0);
	int activity2 = recv(client_socket, password, sizeof(password), 0);
	if(check_IDPass(id, password))
		int activity_status = send(client_socket_serving, "user found..\n", SIZE, 0);
	else {
		*loggedUser = 1;
		FILE *app = fopen("akun.txt", "a+");
		fprintf(app, "%s:%s\n", id, password);
		fclose(app);
		int activity_status = send(client_socket_serving, "success..\n", SIZE, 0);
	}
}


void login_account(int client_socket, char id[], char password[], int *loggedUser, int cliet_socket_serving) {
	int activity1 = recv(client_socket, id, sizeof(id), 0);
	int activity2 = recv(client_socket, password, sizeof(password), 0);
	if(check_IDPass(id, password))
		int activity_status = send(client_socket[client_serving], "wrong password!\n", SIZE, 0);
	else {
		*loggedUser=1;
		int activity_status = send(client_socket[client_serving], "success\n", SIZE, 0);
	}
}

void add_command(int sender, int receiver, char id[], char password[]) {
	char publisher[SIZE], tahun[SIZE], file_path[SIZE], directory[SIZE], file_name[20];
	int activity1 = recv(sender, publisher, SIZE, 0);
	int activity2 = recv(sender, tahun, SIZE, 0);
	int activity3 = recv(sender, file_path, SIZE, 0);
	printf("Publisher: %s\n", publisher);
	printf("Tahun Publikasi: %s\n", tahun);
	printf("Filepath: %s\n", file_path);
	spritntf(directory, "%s", file_path);
	get_directory(directory, file_name);
	strrev(file_name);
	printf("File path: %s\n\n", file_path);
	printf("File path directory: %s\n\n", directory);
	printf("File name: %s\n\n", file_name);
	
	write_file(sender, file_name);
	printf("terminating upload...\n");
	
	FILE *temp = fopen("files.tsv", "a+");
	fprintf(temp, "%s\t%s\t%s\n", file_path, publisher, tahun);
	fclose(temp);
	
	log_("add", file_name, id, password);
}

void delete_command(int sender, int receiver, char id[], char password[]) {
	char book[SIZE];
	int activity1 = recv(sender, book, SIZE, 0);
	char temp[SIZE] = "/home/bagas/Documents/modul3/server_/FILES/";
	char temp1[120] = "/home/bagas/Documents/modul3/server_/FILES/old-";
	
	printf("This book is going to be deleted: %s\n", book);
	if(check_path(book)) {
		int activity_status = send(receiver, "deleting book..\n", SIZE, 0);
		strcat(temp, book);
		strcat(temp1, book);
		
		printf("debug checking old path...\n%s\n", temp);
		printf("debug checking new path...\n%s\n", temp1);
		rename(temp, temp1);
		delete_tsv(book);
		log_("delete", book, id, password);
	}
	else
		int activity_status =  send(receiver, "File doesn't exist!\n", SIZE, 0);
}

void download_command(int sender, int receiver){
	char book[SIZE];
	int line = 1;
	int activity1 = recv(sender, book, SIZE, 0);
	int activity_status;
	printf("This book is going to be downloaded : %s\n", book);
	if(check_path(book)){
		activity_status = send(receiver, "downloading book..\n", SIZE, 0);
		char temp[SIZE] = "/home/bagas/Documents/modul3/server_/FILES/";

		strcat(temp, book);
		printf("debug checking pat...\n%s\n", temp);
		send_file(sender, temp);
	} else {
		activity_status = send(receiver, "File doesn't exist\n", SIZE, 0);
	}
}

void send_file(int client_socket, char file_path[]){
    char data[SIZE] = {0};
    FILE *fp = fopen(file_path, "r");

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(client_socket, data, SIZE, 0) == -1) {
            perror("sending file errr..");
            exit(EXIT_FAILURE);
        }
        bzero(data, SIZE);
    }
    fclose(fp);
    int activity1 = send(client_socket, "done", SIZE, 0);
}

void write_file(int socket_client, char file_name[]){
    int n;
    char path[100] = "/home/bagas/Documents/modul3/server_/FILES/";
    char buffer[SIZE];

    strcat(path, file_name);
    printf ("... %s\n", path);
    FILE *fp = fopen(path, "w");
    fclose(fp);
 
    while (1) {
        n = recv(socket_client, buffer, SIZE, 0);
        if (n <= 0){
            break;
            return;
        }
        if(!strcmp(buffer, "done"))
            return;
        fp = fopen(path, "a");
        printf("... %s\n", buffer);
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
        fclose(fp);
    }
    return;
}
