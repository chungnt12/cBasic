#include "parse.h"
#include "mystring.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>


int main(int argc, char const *argv[])
{
    uint8_t input[DEFAULT_DATA_LENGTH];
    uint8_t lengthInput = 0;
    uint8_t countLine = 0;
    parse_data_struct_t pOutput;
    parse_status_t status;
    FILE* pFile;
    pFile = fopen("filehandling.srec","r");
    status = e_parseStatus_undefined;
    outputStatus(&status);
    if(status == e_parseStatus_undefined)
    {
    	do
    	{
    		fgetLine(pFile,input,DEFAULT_DATA_LENGTH,'S');
        	countLine++;
        	printf("Srec line %d\n",countLine);
        	printf("input: %s\n", input);
        	status = parseData(input,&pOutput);
    		if (status == e_parseStatus_error)
    		{
    			outputStatus(&status);
    		}
    		else
    		{
    			outputStatus(&status);
    			printf("Output\n");
	    		outputParseData(&pOutput);
	    	}
    	} while (!feof(pFile));
    	fclose(pFile);
    }
    return 0;
}