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
