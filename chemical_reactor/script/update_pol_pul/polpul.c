#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(void)
{
    char inFileName[100];
    char outFileName[100];
    char polFileName[100];
    FILE *inFile;
    FILE *outFile;
    FILE *polFile;
    char line_buffer[BUFSIZ]; /* BUFSIZ is defined if you include stdio.h */
    char previous_line[BUFSIZ];
    char polParBlockName[100];
    char *p1, *p2, *p3;
    int nZE = 0, nChar, nLast=0;
    char lineType;

    strcpy(inFileName, "zg");
    strcpy(outFileName, "_hzp_zg");
    strcpy(polFileName, "polpulsequence.txt");


    inFile = fopen(inFileName, "r");
    outFile = fopen(outFileName, "w");
    polFile = fopen(polFileName, "r");

    if (!inFile) 
    {
        printf("Couldn't open file %s for reading.\n", inFileName);
        return -1;
    }
    printf("Opened file %s for reading.\n", inFileName);

    if (!outFile) 
    {
        printf("Couldn't open file %s for output.\n", outFileName);
        return -1;
    }
    printf("Opened file %s for writing.\n", outFileName);

    if (!polFile) 
    {
        printf("Couldn't open file %s for reading polarization parameters.\n", polFileName);
        return -1;
    }
    printf("Opened file %s for reading polarization parameters.\n", polFileName);

    writeBlock(outFile, polFile, polParBlockName);
    nZE = 0;
    nLast = 0;
    lineType = ' ';
    strcpy(previous_line,"");
    while (fgets(line_buffer, sizeof(line_buffer), inFile) != NULL) 
    {
        printf("...%c...%s",lineType, previous_line);
        /* Trim line_buffer */
        p1 = line_buffer;
        while(isspace(*p1)) p1++;
        p2 = p1 + strlen(p1);
        while(isspace(*--p2));
        *(p2+1) = '\0';
        /* End trim line_buffer */
        nChar = strlen(p1);
        if (nChar == 0) continue;

        switch (lineType)
        {
            case 'p': /* pulse sequence */
            case 'c': /* comments */
            case 'C': /* comments with ;$ */
                fputs(previous_line, outFile);
                break;
            case '1': /* 1 ze */
                if (strcmp(polParBlockName, "Constant") == 0) writeBlock(outFile, polFile, polParBlockName);
                if (strcmp(polParBlockName, "Before Ze") == 0) writeBlock(outFile, polFile, polParBlockName);
                fputs(previous_line, outFile);
                break;
            case 'e':
                printf("&&&case e&&&\n");
                if (strcmp(polParBlockName, "Before exit") == 0) writeBlock(outFile, polFile, polParBlockName);                
                fputs(previous_line, outFile);
                break;
            case 'L':
                writeBlock(outFile, polFile, polParBlockName);
                fputs(previous_line, outFile);
                nLast++;
                break;

        }

        /*if (strcmp(polParBlockName, "At the End") == 0) writeBlock(outFile, polFile, polParBlockName);*/
        /*if (lineType == 'L') fputs(previous_line, outFile);*/
        
        strcpy(previous_line, strcat(p1,"\n"));
        lineType = 'p';
        if (p1[0] == ';')
        {
            if ((nChar >= 2) && (line_buffer[1] == '$'))
            {
                if ((nChar >= 4) && (line_buffer[2] == 'I') && (line_buffer[3] == 'd'))
                    lineType = 'L';
                else
                    lineType = 'C';
            }               
            else
            {
                lineType = 'c';  
            }                         
        }
        else
        {
            if (strcmp(line_buffer, "exit\n") == 0)
                lineType = 'e';
            else                
            {
                p3 = strrchr(p1, ' ');
                if ((p3 != NULL) && strcmp(p3+1, "ze\n") == 0)
                {
                    nZE++;
                    if (nZE == 1) lineType = '1';
                }
            } 
        }

    }
    if (nLast == 0) writeBlock(outFile, polFile, polParBlockName);
    if (lineType == 'L') fputs(previous_line, outFile);
    printf("...%c...%s",lineType, previous_line);
    fclose(inFile );
    fclose(outFile);
    fclose(polFile);
    return 0;
}

void writeBlock(FILE *outFile, FILE *polFile, char *polParBlockName)
{
    printf("***begin*****\n%s\n******************\n",polParBlockName);
    char line_buffer[BUFSIZ];
    char *pch, *p;
    int nBlockNameChar, flagBreak=0, nLineWrite=0;
    while (fgets(line_buffer, sizeof(line_buffer), polFile) != NULL)
    {
        printf("---%s",line_buffer);
        if (strncmp(line_buffer, "$$$", 3) == 0)
        {
            pch = strchr(line_buffer + 3, '$');
            nBlockNameChar = pch - line_buffer - 3;
            strncpy(polParBlockName, line_buffer + 3, nBlockNameChar);
            polParBlockName[nBlockNameChar] = '\0';
            flagBreak = 1;
            break;
        }
        else
        {
            if (nLineWrite == 0 && (strcmp(polParBlockName, "At the End") != 0)) fputs("\n", outFile);
            nLineWrite++;
            fputs(line_buffer, outFile);
        }
        printf("+++%s",line_buffer);
    }
    if (nLineWrite > 0) fputs("\n", outFile);
    if (flagBreak == 0) 
    {
        strcpy(polParBlockName, "\0");
        printf("p=NULL\n");
    }
    printf("***end*****\n%s\n******************\n",polParBlockName);
}
