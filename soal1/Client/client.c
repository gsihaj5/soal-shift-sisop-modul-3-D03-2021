
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define PORT 8080
#define SIZE 100

int registerLogin(int server_fd, char command[]);
void send_file(int socket, char file_path[]);
void write_file(int client_socket, char fileName[]);
void add_command(int client_socket);
void download_command(int client_socket);
void delete_command(int client_socket);

int main () {
    struct sockaddr_in address;
    struct hostent *local_host;
    int server_fd, activity;
    char message[SIZE],  command[SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (server_fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", hstrerror(errno));
        return -1;
    }
    printf("Created a socket with: %d\n", server_fd);

    address.sin_family = AF_INET;         
    address.sin_port = htons(PORT);     
    local_host = gethostbyname("127.0.0.1");
    address.sin_addr = *((struct in_addr *)local_host->h_addr);
    activity = connect(server_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_in));
    if (activity == -1) {
        fprintf(stderr, "connection failed [%s]\n", hstrerror(errno));
        close(server_fd);
        return -1;
    }

    activity = recv(server_fd, message, SIZE, 0);
    while(strcmp(message, "wait") == 0) {
        printf("\e[31mServer is full!\e[0m\n");
        activity = recv(server_fd, message, SIZE, 0);
    }

    int command_flag = 0;
    while(1)
    {
        // sign up user
        while(!command_flag) {
            printf("\e[32mInsert Command (Register/Login)\n>\e[0m ");
            scanf("%s", command);
            int b=0;
            for(b=0;b<strlen(command);b++){
                command[b] = tolower(command[b]);
            }
            activity = send(server_fd, command, SIZE, 0);
            if(!strcmp(command, "register") || !strcmp(command, "login"))
                if(registerLogin(server_fd, command))
                    command_flag = 1;
            else {
                activity = recv(server_fd, message, SIZE, 0);
                if(!strcmp(message, "notlogin\n"))
                    printf("you need to login or register first!\n");
                else
                    command_flag = 1;
            }
        }

        // other command
        while(1){
            printf("\e[32mChoose command: add/download/delete/see/find\n>\e[0m ");
            scanf("%s", command);
            activity = send(server_fd, command, SIZE, 0);
            if(!strcmp(command, "add"))
                add_command(server_fd);
            else if(!strcmp(command, "download")){
                download_command(server_fd);
            } else if(!strcmp(command, "delete")){
                delete_command(server_fd);
            }
        }

        sleep(2);
        if(command_flag) break;
    }
    printf("\e[31mclosing client...\e[0m\n\n");

    close(server_fd);
    return 0;
} 

int registerLogin(int server_fd, char command[]){
    int activity, isFound = 0;
	char id[SIZE], password[SIZE], message[SIZE];
	
    //input/sending id and password
    printf("\e[32mUsername\n> \e[0m");
    scanf("%s", id);
    activity = send(server_fd, id, SIZE, 0);

    printf("\e[32mPassword\n> \e[0m");
    scanf("%s", password);
    activity = send(server_fd, password, SIZE, 0);

    activity = recv(server_fd, message, SIZE, 0);
    puts(message);

    //check if its terminate condition
    if(!strcmp(message, "regloginsuccess\n"))
        return 1;
    else if(!strcmp(message, "userfound\n")) {
        printf("Username or ID already exist !\n\n");
        return 0;
    }
    else if(!strcmp(message, "wrongpass\n")) {
        printf("Id or Passsword doesn't match !\n\n");
        return 0;
    }
	
}

void send_file(int socket, char file_path[]){
    int n;
    char data[SIZE] = {0};
    FILE *fp = fopen(file_path, "r");

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(socket, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }

    fclose(fp);
    int activity = send(socket, "done", SIZE, 0);
}

void write_file(int client_socket, char fileName[]){
    int n;
    char buffer[SIZE];

    printf ("... %s\n", fileName);
    FILE *fp = fopen(fileName, "w");
    fclose(fp);

    while (1) {
        n = recv(client_socket, buffer, SIZE, 0);
        if (n <= 0){
            break;
            return;
        }
        if(!strcmp(buffer, "done"))
            return;
        fp = fopen(fileName, "a");
        printf("... %s\n", buffer);
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
        fclose(fp);
    }
    return;
}

void add_command(int client_socket){
    int activity;
    char publisher[SIZE], tahun[SIZE], filePath[SIZE];
    
    printf("Publisher: ");
    scanf("%s", publisher);
    activity = send(client_socket, publisher, SIZE, 0);

    printf("Tahun Publikasi: ");
    scanf("%s", tahun);
    activity = send(client_socket, tahun, SIZE, 0);

    printf("Filepath: ");
    scanf("%s", filePath);
    activity = send(client_socket, filePath, SIZE, 0);

    send_file(client_socket, filePath);
}

void download_command(int client_socket){
    int activity;
    char book[SIZE], message[SIZE];

    scanf("%s", book);
    activity = send(client_socket, book, SIZE, 0);

    activity = recv(client_socket, message, SIZE, 0);
    puts(message);

    if(!strcmp(message, "downloadin...\n"))
        write_file(client_socket, book);
}

void delete_command(int client_socket){
    int activity;
    char book[SIZE], message[SIZE];

    scanf("%s", book);
    activity = send(client_socket, book, SIZE, 0);

    activity = recv(client_socket, message, SIZE, 0);
    puts(message);

}
