#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#define MAX_LINE_LENGTH  96
#define MAX_LINE_NUMBER  10000
#define OUTPUT_LINE_LENGTH 16
#define OUTPUT_LINE_NUMBER 6

int main(int argc, char **argv)
{
    ifstream infile;
    ofstream outfile;
    int totalnumber = 0;
    char buf[MAX_LINE_LENGTH+1];

    if(argc != 3)
    {
        cerr << "Usage: serialconvert infile outfile" << endl;
        return -1;
    }

    infile.open(argv[1]);
    outfile.open(argv[2]);
    if(!infile)
    {
        cerr << "Failed to open file " << argv[1] << endl;
        return -1;
    }
    if(!outfile)
    {
        cerr << "Failed to open file " << argv[2] << endl;
        return -1;
    }
    
    while(infile.getline(buf, MAX_LINE_LENGTH))
    {
        string linestr = buf;
        int linelength = linestr.length();
        int linecount = linelength / OUTPUT_LINE_LENGTH;
        int linerest = linelength % OUTPUT_LINE_LENGTH;
        for(int i = 0; i < linecount; i ++)
        {
            outfile << linestr.substr(i * OUTPUT_LINE_LENGTH, OUTPUT_LINE_LENGTH) << endl;
        }
        
        if(linerest)
        {
            outfile << linestr.substr(OUTPUT_LINE_LENGTH * linecount, linerest);
            for(int i = linerest; i < OUTPUT_LINE_LENGTH; i ++)
            {
                outfile << "0";
            }
            outfile << endl;
            linecount ++;
        }
        
        for(int i = linecount; i < OUTPUT_LINE_NUMBER; i ++)
        {
            for(int j = 0; j < OUTPUT_LINE_LENGTH; j ++)
            {
                outfile << "0";
            }
            outfile << endl;
        }
    }
    
    outfile.flush();

    return 0;
}

