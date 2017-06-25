#include "mystring.h"

#define DEBUG 0

#if defined(DEBUG) && (DEBUG)
    #define PRINTF  printf
#else
    #define PRINTF
#endif

/**
 * [getLine description]
 * @param str [description]
 * @param len [description]
 */
void getLine(uint8_t* str)
{
    uint8_t c;
    uint8_t i = 0;
    for (i = 0;; i++)
    {
        c = getchar();
        if (c == '\n')
        {
            str[i] = '\0';
            fflush(stdin);
            return;
        }
        str[i] = c;
    }
}

uint8_t strLength(uint8_t* str)
{
	uint8_t count = 0;
	while(TRUE)
	{
		if(str[count] == '\0')
		{
			return count;
		}
		else
		{
			count++;
		}
	}
}
void strCoppy(uint8_t* str1, uint8_t* str2, uint8_t begin, uint8_t length)
{
    uint8_t count = 0;
    str1[length] = '\0';
    for (count = 0; count < length; ++count)
    {
        str1[count] = str2[count + begin];
    }
}
uint8_t coverCharToHex(uint8_t input)
{
    uint8_t value;
    switch(input)
    {
        case '0':
            value = 0;
            break;
        case '1':
            value = 1;
            break;
        case '2':
            value = 2;
            break;
        case '3':
            value = 3;
            break;
        case '4':
            value = 4;
            break;
        case '5':
            value = 5;
            break;
        case '6':
            value = 6;
            break;
        case '7':
            value = 7;
            break;
        case '8':
            value = 8;
            break;
        case '9':
            value = 9;
            break;
        case 'a':
        case 'A':
            value = 10;
            break;
        case 'b':
        case 'B':
            value = 11;
            break;
        case 'c':
        case 'C':
            value = 12;
            break;
        case 'd':
        case 'D':
            value = 13;
            break;
        case 'e':
        case 'E':
            value = 14;
            break;
        case 'f':
        case 'F':
            value = 15;
            break;
        default:
            value = ERR;
            break;
    }
    return value;
}
uint8_t fgetLine(FILE* pFile, uint8_t* string, uint8_t length, uint8_t condition)
{
    PRINTF("Start getLine\n");
    uint8_t character;
    uint8_t count = 0;
    if(pFile==NULL)
    {
        perror("\nError opening file");
    }
    else
    {
        character = fgetc(pFile);
        PRINTF("Fist character: %c\n", character);
        while(character == condition)
        {
            PRINTF("Start while\n");
            string[count++] = character;
            PRINTF("count: %d\n", count);
            PRINTF("String: %s\n", string);
            while(TRUE)
            {
                PRINTF("Start do while\n");
                character = fgetc(pFile);
                PRINTF("Get character: %c\n", character);
                if(character == '\n' || count == length || feof(pFile))
                {
                    string[count] = '\0';
                    PRINTF("Stop do while\n");
                    return count;
                }
                else
                {
                    string[count++] = character;
                    PRINTF("count: %d\n", count);
                    PRINTF("String: %s\n", string);
                }
            }
        }
    }
    PRINTF("Stop getLine\n");
}
uint8_t strFindIn(uint8_t* str1, uint8_t* str2)
{
    uint8_t count = 0;
    for(count = 0;; ++count)
    {
        if(str1[count]=='\0')
        {
            return FALSE;
        }
        else if(str2[0]==str1[count])
        {
            uint8_t tempCount = 0;
            do
            {
                if(str2[++tempCount]=='\0')
                {
                    return TRUE;
                }
                else if(str1[count+tempCount]=='\0')
                {
                    return FALSE;
                }
            }while(str2[tempCount]==str1[count+tempCount]);
        }
    }
}
/*test function*/
/*int main(int argc, char const *argv[])
{
    uint8_t* str1="nguyen thanh chung";
    uint8_t* str2="nguyen than";
    if(strFindIn(str1,str2))
    {
        printf("co str2 trong str1\n");
    }
    else
    {
        printf("khong co str2 trong str1\n");
    }
    uint8_t string[79];
    FILE* pFile;
    pFile = fopen("filehandling.srec","r");
    while(!feof(pFile))
    {
        fgetLine(pFile,string,79,'S');
        printf("string: %s\n",string);
    }

    uint8_t *str1 = "nguyen thanh chung";
    uint8_t str2[20];
    strCoppy(str2,str1,0,20);
    printf("str2: %s\n", str2);
    printf("length: %d\n", strLength(str1));
    return 0;
}*/