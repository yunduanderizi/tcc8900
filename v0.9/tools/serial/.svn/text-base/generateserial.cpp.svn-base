#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <string>

#define MAX_LINE_LENGTH  256
#define SERIAL_LENGTH    32

using namespace std;

int main(int argc, char **argv)
{
    const char *filename;
    const char *prefix;
    FILE *file;
    vector<string> numbers;
    unsigned long long totalnumber = 0;
    char buf[MAX_LINE_LENGTH];
    char format[MAX_LINE_LENGTH];
    int length = -1;
    int i = 0;
    int j = 0;
    char returnChar = '\n';
    int prefix_len = 0;
    unsigned long long max_number;

    if(argc != 4)
    {
        fprintf(stderr, "Usage: generateserial filename prefix count\n");
        return -1;
    }

    filename = argv[1];
    prefix = argv[2];
    totalnumber = atoi(argv[3]);

    prefix_len = strlen(prefix);
    if(prefix_len > SERIAL_LENGTH)
    {
        fprintf(stderr, "To long prefix.\n");
        return -1;
    }

    max_number = (unsigned long long)pow(10,(SERIAL_LENGTH - prefix_len)) - 1;
    
    if(totalnumber > max_number)
    {
        fprintf(stderr, "To many numbers.\n");
        return -1;
    }

    srandom((unsigned int)time(NULL));

    sprintf(format, "%%s%%0%dlld",SERIAL_LENGTH - prefix_len);
    
    for(i = 0; i < totalnumber; i ++)
    {
        while(1)
        {
            long int rand = random();
            double rate = ((double)rand) / RAND_MAX;
            unsigned long long result = (unsigned long long)(rate * max_number);
            memset(buf, 0, MAX_LINE_LENGTH);
            sprintf(buf, format, prefix, result);
            for(j = 0; j < i; j ++)
            {
                if(strcmp(numbers[j].c_str(), buf) == 0)
                {
                    break;
                }
            }
            if(j >= i)
            {
                numbers.push_back(buf);
                break;
            }
            printf("Duplicate %d %d.\n", i, j);
        }
    }

    file = fopen(filename, "w+");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open the file %s.\n", filename);
        return -1;
    }

    for(i = 0; i < totalnumber; i ++)
    {
        fwrite(numbers[i].c_str(),numbers[i].length(), 1, file);
        fwrite(&returnChar, 1, 1, file);
    }

    fclose(file);
    return 0;
}

