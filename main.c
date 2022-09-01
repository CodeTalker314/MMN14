#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "macros.h"
#include "syntax.h"

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
    
        fullFileName = NULL;
        macroName = NULL;
        fullFileName = asFile(argv[i]);
        macroName = amFile(argv[i]);

        printf("Started %s assembler process\n\n", fullFileName);
        file = fopen(fullFileName, "ab+"); /* create .as file */
        file = PreReadFile(fullFileName, Mhead);
        if (file == NULL) {
            printf("file %s doesn't exist", fullFileName);
            return 1;
        } else {
            file = WritePreFile(fullFileName, macroName, Mtail); /* write the file after macros span */
        }

        file = fopen(macroName, "ab+"); /* create .am file */
        printf("%s\n",macroName);
    }
return 1;
}
