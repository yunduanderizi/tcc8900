#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_LINE_LENGTH  256
#define MAX_LINE_NUMBER  10000

int main(int argc, char **argv)
{
    const char *filename;
    FILE *file;
    char numbers[MAX_LINE_NUMBER][MAX_LINE_LENGTH];
    int totalnumber = 0;
    char buf[MAX_LINE_LENGTH];
    int length = -1;
    int i = 0;
    int j = 0;

    if(argc != 2)
    {
        fprintf(stderr, "Usage: findduplicate filename\n");
        return -1;
    }

    filename = argv[1];
    file = fopen(filename, "r");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open the file %s.\n", filename);
        return -1;
    }
    
    memset(buf, 0, MAX_LINE_LENGTH);

    while(fgets(buf, MAX_LINE_LENGTH, file))
    {
        char *start, *end;
        start = buf;
        end = buf + strlen(buf) - 1;
        
        while(((*start == ' ') || (*start == '\t')) && (start <= end))
        {
            start ++;
        }

        while(((*end == ' ') || (*end == '\t') || (*end == '\r') || (*end == '\n')) && (start <= end))
        {
            end --;
        }

        if(start >= end)
        {
            fprintf(stderr, "Invalid line: %s.\n", buf);
            continue;
        }

        end ++;
        *end = '\0';
        if(length == -1)
        {
            length = strlen(buf);
        }
        else
        {
            if(length != strlen(buf))
            {
                fprintf(stderr, "Line mismatch: %d, %s.\n", length, buf);
                continue;
            }
        }
        strcpy(numbers[totalnumber], buf);
        totalnumber ++;
    }
    fclose(file);
    printf("Total number: %d\n", totalnumber);
    for(i = 0; i < totalnumber; i ++)
    {
        int match = 0;
        for(j = 0; j < totalnumber; j ++)
        {
            if(i != j)
            {
                if(strcmp(numbers[i], numbers[j]) == 0)
                {
                    match = 1;
                    break;
                }
            }
        }
        if(match)
        {
            fprintf(stderr, "Find match %d:%s %d:%s\n", i, numbers[i], j, numbers[j]);
        }
    }
    return 0;
}

