#include "main.h"

int main(int argc,char *argv[]) {
    FILE *file;
    char *macroName;
    struct Macros *Mtail;
    struct Macros *Mhead;
    int i;
    char *fullFileName;
    
    /* initialize macro list */

    Mhead = (struct Macros *) malloc(sizeof(struct Macros));
    Mtail = (struct Macros *) malloc(sizeof(struct Macros));
    Mtail = Mhead;

    /*no file input case*/
    if (argc <= 1) {
        printf("No File detected.\n");
        exit(0);
    }

    /* test macro */
    for (i = 1; i < argc; i++) {
       
        fullFileName = concat(argv[i], ".as\0"); /* .as file name */
        macroName = concat(argv[i], ".am\0"); /* .am file name */
        printf("Started %s assembler process\n\n", fullFileName);
        file = fopen(fullFileName, "r"); /* create .as file */
        file = PreReadFile(fullFileName, Mhead);
        if (file == NULL) {
            printf("file %s doesn't exist", fullFileName);
        } else {
            WritePreFile(macroName, Mtail); /* write the file after macros span */
        }

        file = fopen(macroName, "r"); /* create .am file */
        printf("%s\n", macroName);
    }
return 0;
}