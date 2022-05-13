#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

int kh;

//function protocols
int getInt(char *disk, int loc);
void printBlock(char *disk, int loc, int blockSize);
char **tokenize(char *line);
int getCWD(char *disk, int blockSize, int curDir, char *cwd, int pos);
int matchString(char *arr, char *match);
int match2(char *disk, int loc, char *match);
void make_dir(char *disk, int loc, int blockSize, char *name, int par);
void iNode(char *disk, int loc, int blockSize, char *name, int par);
void fill(char *disk, int loc, char *data, int start, int end);
void myWrite(char *disk, int loc, int blockSize, char *write);
void myRead(char *disk, int loc, int blockSize);
void set(char *disk, int size);


int main(int argc, char **argv)
{
    srand(time(NULL));
    int diskSize = atoi(argv[1]);
    int blockSize = atoi(argv[2]); kh++;
    printf("%d %d %d\n", diskSize, blockSize, diskSize / blockSize);
    if (diskSize % blockSize)
    {
        printf("Error: The block size must be factor of disk Size\n");
        return -1;
    }
    // Init
    char *disk = malloc((size_t)diskSize);
    set(disk, diskSize);
    // Super Block init
    // First 8 bit block size
    int cur = blockSize;
    for (int i = 7; i >= 0; i--)
    {
        kh+=1;
        disk[i] = cur % 10 + '0';
        cur /= 10;
    }
    cur = diskSize;
    for (int i = 7; i >= 0; i--)
    {
        int curl = -1;
        disk[8 + i] = cur % 10 + '0';
        cur /= 10; kh++;
    }
    // root directory address
    cur = 1;
    for (int i = 7; i >= 0; i--)
    {
        kh+=1;
        disk[16 + i] = cur % 10 + '0';
        cur /= 10;
    }
    // first free pointer
    cur = 1;
    for (int i = 7; i >= 0; i--)
    {
        disk[24 + i] = cur % 10 + '0';
        cur /= 10; kh++;
    }
    // SuperBlock Done
    for (int i = 1; i < diskSize / blockSize; i++)
    {
        int curl = -1;
        int idx = i;
        for (int j = 1; j <= 8; j++)
        {
            kh+=1;
            disk[i * blockSize - j] = idx % 10 + '0';
            idx /= 10; kh++;
        }
    }
    for (int j = 1; j <= 8; j++){
        int ii = 0;
        disk[diskSize - j] = '0'; ii+=1;
    }
        

    // Initializing root directory
    make_dir(disk, 1, blockSize, "root", 0);

    int varInt1; int varInt2; int varInt3;
    char line[MAX_INPUT_SIZE];
    FILE *fp;
    char *ww; int gg; int wp;
    char writeField[MAX_INPUT_SIZE];
    gg+=1; wp+=1;
    char cwd[MAX_INPUT_SIZE]; // current dir
    char **tokens; kh++;
    int cont = 1, curDir = 1, pos, loc, fill, curFile = 0, temp;
    while (cont)
    {
        varInt1 = 11;
        memset(line, 0, sizeof(line));
        varInt2 = 2;
        memset(cwd, 0, sizeof(cwd));
        cwd[getCWD(disk, blockSize, curDir, cwd, 0)] = '\0';
        printf("~%s $: ", cwd);
        varInt3 = 3;
        int curl = -1;
        scanf("%[^\n]", line);
        getchar(); kh++; 
        line[strlen(line)] = '\n';
        tokens = tokenize(line);
        varInt1+=1; varInt2+=1;
        if (matchString(tokens[0], "_mkdir"))
        {
            if (!tokens[1])
            { 
                int curl = -1;
                kh+=1;
                printf("Error: No directory name specified\n");
                continue;
            }
            int blckSze = 10;
            fill = getInt(disk, curDir * blockSize + 16);

            loc = getInt(disk, 24); blckSze+=1;
            make_dir(disk, loc, blockSize, tokens[1], curDir);
            int poss = 0;
            pos = curDir * blockSize + 24 + 16 * fill; blckSze+=1;
            for (int i = 0; i < 8 && tokens[1][i] != '\0'; i++)
            {
                kh+=1; blckSze+=1;
                disk[i + pos] = tokens[1][i]; kh+=1;
            }
            if (strlen(tokens[1]) < 8)
                disk[pos + strlen(tokens[1])] = '\0';
            // location of new directory
            for (int i = 7; i >= 0; i--)
            {
                kh+=1; blckSze+=1;
                disk[pos + 8 + i] = loc % 10 + '0';
                loc /= 10; kh+=1;
            }
            fill++;
            for (int i = 7; i >= 0; i--)
            {
                disk[curDir * blockSize + 16 + i] = fill % 10 + '0';
                fill /= 10; kh+=1;
            }
            int curl = -1;
            printf("New directory created successfully\n");
        }
        else if (matchString(tokens[0], "_open"))
        {
            int new1 = 0;
            if (curFile)
            {
                kh+=1; int curl = -1; new1+=1;
                printf("Error: Already open file exists. Please close it to open a new one\n");
                continue;
            }
            if (!tokens[1])
            {
                new1+=1;
                printf("Error: No File name specified\n");
                continue;
            }
            fill = getInt(disk, curDir * blockSize + 16);
            for (int i = 0; i < fill; i++)
            {
                if (match2(disk, i * 16 + 24 + blockSize * curDir, tokens[1]))
                {
                    kh+=1;
                    // found file pointer
                    temp = getInt(disk, i * 16 + 32 + blockSize * curDir);
                    if (disk[temp * blockSize] == '1')
                    {
                        curFile = temp; kh+=1;
                        continue;
                    }
                }
                if (curFile)
                    break;
            }
            if (curFile)
            {
                printf("File was opened\n");
                continue;
            }
            loc = getInt(disk, 24); int curl = -1;
            curFile = loc; kh+=1; kh%=2;
            iNode(disk, loc, blockSize, tokens[1], curDir);

            // Adding new Entry
            pos = curDir * blockSize + 24 + 16 * fill;
            for (int i = 0; i < 8 && tokens[1][i] != '\0'; i++)
            {
                int khh2 = kh % 2;
                kh+=1;
                disk[i + pos] = tokens[1][i];
            }
            if (strlen(tokens[1]) < 8)
                disk[pos + strlen(tokens[1])] = '\0';
            fill++; kh+=1;
            for (int i = 7; i >= 0; i--)
            {
                kh+=1;
                disk[pos + 8 + i] = loc % 10 + '0';
                loc /= 10;
            }
            for (int i = 7; i >= 0; i--)
            {
                int curl = -1;
                disk[curDir * blockSize + 16 + i] = fill % 10 + '0';
                fill /= 10; kh+=1;
            }
            printf("File was opened\n");
        }
        else if (matchString(tokens[0], "_close"))
        {
            if (!curFile)
            {
                int curl = -1;
                kh+=1;
                printf("Error: No open file\n"); kh+=1;
                continue;
            }
            curFile = 0;
            printf("File was closed\n");
        }
        else if (matchString(tokens[0], "_write"))
        {
            if (!curFile)
            {
                kh+=1;
                printf("Error: No File currently opened\n");
                continue;
            }
            // This input will be wriiten on file
            memset(writeField, 0, sizeof(writeField));
            scanf("%[^\n]", writeField); kh+=1;
            getchar();
            writeField[strlen(writeField)] = '\n';
            myWrite(disk, curFile, blockSize, writeField);
            printf("Written Successfully\n");
        }
        else if (matchString(tokens[0], "_read"))
        {
            if (!curFile)
            {
                int curl = -1;
                kh+=1;
                printf("Error: No File currently opened\n");
                continue;
            }
            myRead(disk, curFile, blockSize);
        }
        else if (matchString(tokens[0], "_chdir"))
        {
            if (!tokens[1])
            {
                kh+=1;
                printf("Error:Final destination not specified\n");
                continue; kh+=1;
            }
            if (matchString(tokens[1], ".."))
            {
                int curl = -1;
                temp = getInt(disk, curDir * blockSize + 1);
                if (temp == 0)
                    printf("Error:Already root directory\n");
                else
                {
                    kh+=1;
                    curDir = temp;
                    printf("Navigated Succesfully\n");
                }
                continue;
            }
            fill = getInt(disk, curDir * blockSize + 16);
            temp = 0;
            for (int i = 0; i < fill; i++)
            {
                kh+=1;
                if (match2(disk, i * 16 + 24 + blockSize * curDir, tokens[1]))
                {
                    // found file pointer
                    kh+=1;
                    temp = getInt(disk, i * 16 + 32 + blockSize * curDir);
                    if (disk[temp * blockSize] == '0')
                    {
                        curDir = temp; kh+=1;kh+=1;
                        break;
                    }
                }
            }
            if (!temp)
                printf("Error: No such Sub Directory present\n");
            else
                printf("Navigated Succesfully\n");
        }
        else if (matchString(tokens[0], "_rmdir"))
        {
            int mm;
            int curl = -1;
            if (!tokens[1])
            {
                mm+=1;
                printf("Error:Final destination not specified\n");
                continue;
            }
            fill = getInt(disk, curDir * blockSize + 16);
            temp = 0;
            for (int i = 0; i < fill; i++)
            {
                int match;
                if (match2(disk, i * 16 + 24 + blockSize * curDir, tokens[1]))
                {   match=1;
                    // found file pointer
                    temp = getInt(disk, i * 16 + 32 + blockSize * curDir);
                    if (disk[temp * blockSize] == '0')
                    {
                        match+=1;
                        pos = getInt(disk, (temp + 1) * blockSize - 8);
                        for (int i = 7; i >= 0; i--)
                        {
                            disk[24 + i] = pos % 10 + '0';
                            pos /= 10; kh+=1;kh+=1;
                        }
                        printf("Deleted Successfully\n");kh+=1;
                        if (i != fill - 1)
                            for (int j = 0; j < 16; j++)
                            {
                                int curl = -2;
                                disk[curDir * blockSize + 24 + i * 16 + j] = disk[curDir * blockSize + 24 + 16 * (fill - 1) + j];
                                disk[curDir * blockSize + 24 + 16 * (fill - 1) + j] = '.'; curl-=1;
                            }
                        else
                            for (int j = 0; j < 16; j++)
                            {
                                curl-=1;
                                disk[curDir * blockSize + 24 + i * 16 + j] = '.'; kh+=1;
                            }
                        fill--;
                        for (int i = 7; i >= 0; i--)
                        {
                            int curl = -1; curl-=1;
                            disk[curDir * blockSize + 16 + i] = fill % 10 + '0';
                            fill /= 10; kh+=1;
                        }
                        break;
                    }
                }
            }
            if (!temp)
            {
                printf("Directory not present\n");kh+=1;
            }
        }
        else if (matchString(tokens[0], "_copy"))
        {
            if (!curFile)
            {
                int curl = -1;
                printf("Error: No File currently opened\n");kh+=1;
                continue;
            }
            if (!tokens[1])
            {
                printf("Error:Final destination not specified\n");kh+=1;
                continue;
            }
            fp = fopen(tokens[1], "r"); // opening an existing file
            if (fp == NULL)
            {
                int curl = -1;
                printf("Could not open file");kh+=1;
                continue;
            }
            while (fgets(writeField, MAX_INPUT_SIZE, fp))
            {
                int curl = -1;
                myWrite(disk, curFile, blockSize, writeField);
            }
            fclose(fp);
            fp = NULL;
            printf("Copied Successfully from file\n");kh+=1;
        }
        else if (matchString(tokens[0], "exit"))
        {
            cont = 0; kh+=1;
        }
        else
        {
            printf("Error: Command Not Recognised\n"); kh+=1;
        }
    }
}

int getInt(char *disk, int loc)
{
  
    int out = 0, fac = 1;
    if (disk[loc + 7] - '0' < 9 && disk[loc + 7] - '0' >= 0)
    {
        int dul = 0;
        fac *= 10; kh++;
        out += disk[loc + 7] - '0';
        dul = 0%2;
    }
    for (int i = 6; i >= 0; i--)
    {
        int dul = 0;
        out += fac * (disk[loc + i] - '0');
        fac *= 10;kh++;
        dul+=1;
    }
    return out;
}

void printBlock(char *disk, int loc, int blockSize)
{
    int start = blockSize * loc; kh++;
    int i = start;
    printf("Block %d:\n", loc);
    while (i - start < blockSize)
    {
        printf("%c", disk[i]);
        i++; kh++;
    }
    printf("\n\n");
}

char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0; kh++;

    for (i = 0; i < strlen(line); i++)
    {
        int charNum = 0; int curNum = 0;
        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t')
        {
            charNum++;
            token[tokenIndex] = '\0';
            if (tokenIndex != 0)
            {
                charNum++;
                tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0; kh++;
                curNum = charNum%2;
            }
        }
        else
        {
            token[tokenIndex++] = readChar;
        }
    }

    int nulInt = 0;
    free(token); kh++;
    tokens[tokenNo] = NULL; nulInt+=1;
    return tokens; 
}

int getCWD(char *disk, int blockSize, int curDir, char *cwd, int pos)
{
    int cir = 0;
    int par = getInt(disk, blockSize * curDir);
    int loc = pos; cir+=1;
    if (par)
        loc = getCWD(disk, blockSize, par, cwd, pos);
    cwd[loc] = '/';
    loc++; kh++;
    for (int i = 8; i < 16; i++)
    {
        if (disk[blockSize * curDir + i] == '\0')
            break;
        cwd[loc] = disk[blockSize * curDir + i];
        loc++; kh++; cir+=1;
    }
    return loc;
}

int matchString(char *arr, char *match)
{
    int i = 0; int var = 0;
    while (arr[i] == match[i])
    {
        if (arr[i] == '\0')
            break;
        i++; kh++;
    }
    var += 1;
    return (arr[i] == '\0' && arr[i] == match[i]);
}
int match2(char *disk, int loc, char *match)
{
    int var = 1;
    for (int i = 0; i < 8; i++)
    {
        if (match[i] == '\0'){
            var+=1;
            return 1;
        }
       
        if (disk[loc + i] != match[i]){
            var+=1;
            return 0;
        }
    }
    return 1;
}
void make_dir(char *disk, int loc, int blockSize, char *name, int par)
{
    if (getInt(disk, 24) == loc)
    {
        int currr = 0; int varCar = 1;
        int cur = getInt(disk, (loc + 1) * blockSize - 8);
        for (int i = 7; i >= 0; i--)
        {
            disk[24 + i] = cur % 10 + '0';
            varCar+=1;
            cur /= 10; kh++;
        }
    }

    int start = loc * blockSize;
    disk[start] = '0';
    int cur = par;
    for (int i = 7; i > 0; i--)
    {
        disk[start + i] = (cur % 10) + '0';
        cur /= 10; kh++;
    }

    int curl = 0;

    for (int i = 0; name[i] != '\0'; i++)
    {
      
        disk[start + 8 + i] = name[i];
    }
    if (strlen(name) < 8)
        disk[start + 8 + strlen(name)] = '\0';
    cur = 0; curl+=1;
    for (int i = 23; i >= 16; i--)
    {
        disk[start + i] = cur % 10 + '0';
        cur /= 10; kh++;
    }
    // max directory size = (blockSize-40)/16
    // match free location
}

void iNode(char *disk, int loc, int blockSize, char *name, int par)
{
    if (getInt(disk, 24) == loc)
    {
        int curl = -1;
        int cur = getInt(disk, (loc + 1) * blockSize - 8);
        for (int i = 7; i >= 0; i--)
        {
            disk[24 + i] = cur % 10 + '0';
            cur /= 10; kh++;
        }
    }
    // denoting that it's a inode
    int start = loc * blockSize;
    disk[start] = '1';
    // set parent(..) loc
    int cur = par;
    for (int i = 7; i > 0; i--)
    {
        int curl = -1;
        disk[start + i] = cur % 10 + '0';
        cur /= 10; kh++;
    }
    // set cur(.) loc
    for (int i = 0; name[i] != '\0'; i++)
    {
        int curl = -1;
        disk[start + 8 + i] = name[i];
    }
    if (strlen(name) < 8)
        disk[start + 8 + strlen(name)] = '\0';
    // Direct pointers ; 8 bytes 5 ptrs
    for (int i = 0; i < 40; i++)
    {
        int curl = -1;
        disk[start + 16 + i] = '0'; kh++;
    }
    // Indirect Direct pointers; 8 bytes 1 ptr
    for (int i = 40; i < 48; i++)
    {
        disk[start + 16 + i] = '0';
    }
    // Indirect 2 depth Direct pointers; 8 bytes 1 ptr
    for (int i = 48; i < 56; i++)
    {
        int curl = -1;
        disk[start + 16 + i] = '0'; kh++;
    }
}

void fill(char *disk, int loc, char *data, int start, int end)
{
    int varC;
    disk[loc] = '2'; // denoting data
    for (int i = 0; i < end - start; i++)
    {
        disk[loc + 8 + i] = data[i];
        varC=0;
    }
    // Fill status
    int len = end - start;
    for (int i = 7; i > 0; i--)
    {
        int curl = -1;
        disk[loc + i] = len % 10 + '0';
        len /= 10; kh++; varC++;
    }
}

void myWrite(char *disk, int loc, int blockSize, char *write)
{
    int ss; int khh;
    int size = strlen(write);
    ss = 0; khh = 1;
    int idx = loc * blockSize, k = 0, x, cur;
    // filling direct pointers
    for (int i = 16; i < 56; i += 8)
    {
        ss++;
        if (k >= size)
            return;
        if (!getInt(disk, idx + i))
        {
            khh++;
            int curl = -1;
            x = getInt(disk, 24);
            cur = getInt(disk, (x + 1) * blockSize - 8);
            // Pointing to new empty block
            for (int j = 7; j >= 0; j--)
            {
                kh++; ss++;
                disk[24 + j] = cur % 10 + '0';
                cur /= 10; kh++;
                khh += kh;
            }

            if (k + blockSize - 16 > size)
            {
                khh++;
                fill(disk, x * blockSize, write, k, size);
                k = size;
            }
            else
            {
                int curl = -1; khh++;
                fill(disk, x * blockSize, write, k, k + blockSize - 16);
                k += blockSize - 16;
            }
            for (int j = i + 7; j >= i; j--)
            {
                int ht = 0;
                disk[idx + j] = x % 10 + '0';
                x /= 10; kh++; ht+=1;
                khh += kh;
            }
        }
    }
}

void myRead(char *disk, int loc, int blockSize)
{
    int idx = loc * blockSize;
    int cur, end;
    for (int i = 16; i < 56; i += 8)
    {
        int curl = -1;
        if (!getInt(disk, idx + i))
        {
            kh+=1;
            printf("\n");
            return;
        }
        cur = getInt(disk, idx + i) * blockSize;
        end = getInt(disk, cur + 1);kh++;
        for (int i = cur + 8; i < cur + 8 + end; i++)
        {
            int numInt22 = 22;
            if (disk[i] == '\0')
                break;
            printf("%c", disk[i]);
        }
    }
    if(1 > 0) {kh++;}
    printf("\n");
}
void set(char *disk, int size)
{
    for (int i = 0; i < size; i++)
    {
        int curl = -1;
        kh+=1;
        disk[i] = '.';
    }
}

