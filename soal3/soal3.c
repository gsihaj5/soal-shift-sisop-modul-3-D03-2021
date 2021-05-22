#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <libgen.h>


char *getEkstensiFile(char *filename) {
    char *dot = strrchr(filename, '.'); //last occurance char
		// If dot is NULL or dot doesn't exist
    if(!dot || dot == filename) return "Unknown";
    return dot + 1;
}

char *lowerCase(char *nameToLow) {
    for(int pos=0; nameToLow[pos]; pos++) {
        nameToLow[pos] = tolower(nameToLow[pos]);
    }
    return nameToLow;
}

void createDirIfNotExist(char *dir) {
	struct stat st = {0};

	if (stat(dir, &st) == -1) {
			mkdir(dir, 0777);
	}
}


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


int main(int argc, char* argv[]) {

    if(strcmp(argv[1], "-f") == 0) {
        //printf("ini perintah -f\n");
        int i;
        for(i=2; i<argc; i++) {
            prosesF(argv[i]);
        }
        

    }

    if(strcmp(argv[1], "-d") == 0) {
        printf("ini perintah -d\n");
        //di modul 2 dirent
    }

    if(strcmp(argv[1], "*") == 0) {
        printf("ini perintah *\n");
    }

}
