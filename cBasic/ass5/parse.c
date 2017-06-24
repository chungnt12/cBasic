#include "parse.h"
#include "mystring.h"
#include <stdlib.h>
#include <windows.h>

#define DEBUG 0

#if defined(DEBUG) && (DEBUG)
	#define PRINTF  printf
#else
	#define PRINTF
#endif


void outputParseData(parse_data_struct_t* pOutput)
{
	if(pOutput->address != DEFAULT_ADDRESS)
	{
		printf("Address: %x\n", pOutput->address);
	}
	if(pOutput->dataLength != DEFAULT_DATA_LENGTH)
	{
		printf("Data length: %x\n", pOutput->dataLength);
	}
	if(pOutput->data[0] != DEFAULT_DATA)
	{
		printf("Data: %s\n", pOutput->data);
	}
}
void outputStatus(parse_status_t *pStatus)
{
	printf("Status: ");
	switch(*pStatus)
	{
		case e_parseStatus_done:
			printf("Done!\n");
			break;
		case e_parseStatus_inprogress:
			printf("Inprogress!\n");
			break;
		case e_parseStatus_error:
			printf("Error!\n");
			break;
		case e_parseStatus_undefined:
			printf("Undefined!\n");
			break;
		default:
			break;
	}
}

uint32_t getHex(uint8_t pInput[], uint8_t mode, uint8_t address)
{
	uint8_t count = 0;
	uint32_t temp = 0;
	/*set loop for as 8 bit*/
	for (count = 0; count < mode/4; ++count)
	{
		SET_8BIT(temp, coverChar(pInput[count + address]), coverChar(pInput[ ++count + address]));
	}
	return temp;
}
uint8_t checkSum(uint8_t pInput[], uint8_t length)
{
	uint8_t count = 0;
	uint16_t sum = 0;
	uint8_t temp = 0;
	for (count = 2; count < length - 2; ++count)
	{
		getHex(pInput, MODE_8BIT, count++);
		PRINTF("temp in for: %x\n", temp);
		sum += temp;
		PRINTF("sum in for: %x\n", sum);
	}
	getHex(pInput, MODE_8BIT, length - 2);
	PRINTF("sum out for: %x\n", sum);
	PRINTF("temp out for: %d\n", temp);
	if (temp == sum)
	{
		PRINTF("return TRUE\n");
		return TRUE;
	}
	else
	{
		PRINTF("return FALSE\n");
		return FALSE;
	}
}

parse_status_t parseData(uint8_t pInput[], parse_data_struct_t *pOutput)
{
	parse_status_t status;
	if(pInput[0] == 'S')
	{
		switch(pInput[1])
		{
		case '0':
			pOutput->dataLength = getHex(pInput,MODE_8BIT,2);
			pOutput->address = getHex(pInput,MODE_16BIT,4);
			strCoppy(pOutput->data,pInput,8,(pOutput->dataLength-3)*2);
			status = e_parseStatus_inprogress;
			break;
		case '1':
			pOutput->dataLength = getHex(pInput,MODE_8BIT,2);
			pOutput->address = getHex(pInput,MODE_16BIT,4);
			strCoppy(pOutput->data,pInput,8,(pOutput->dataLength-3)*2);
			status = e_parseStatus_inprogress;
			break;
		case '2':
			pOutput->dataLength = getHex(pInput,MODE_8BIT,2);
			pOutput->address = getHex(pInput,MODE_24BIT,4);
			strCoppy(pOutput->data,pInput,8,(pOutput->dataLength-4)*2);
			status = e_parseStatus_inprogress;
			break;
		case '3':
			pOutput->dataLength = getHex(pInput,MODE_8BIT,2);
			pOutput->address = getHex(pInput,MODE_32BIT,4);
			strCoppy(pOutput->data,pInput,8,(pOutput->dataLength-5)*2);
			status = e_parseStatus_inprogress;
			break;
		case '5':
			pOutput->dataLength = getHex(pInput,MODE_8BIT,2);
			pOutput->address = DEFAULT_ADDRESS;
			strCoppy(pOutput->data,pInput,4,8);
			status = e_parseStatus_inprogress;
			break;
		case '7':
			pOutput->dataLength = getHex(pInput,MODE_8BIT,2);
			pOutput->address = getHex(pInput,MODE_32BIT,4);
			pOutput->data[0] = DEFAULT_DATA;
			status = e_parseStatus_done;
			break;
		case '8':
			pOutput->dataLength = getHex(pInput,MODE_8BIT,2);
			pOutput->address = getHex(pInput,MODE_24BIT,4);
			pOutput->data[0] = DEFAULT_DATA;
			status = e_parseStatus_done;
			break;
		case '9':
			pOutput->dataLength = getHex(pInput,MODE_8BIT,2);
			pOutput->address = getHex(pInput,MODE_16BIT,4);
			pOutput->data[0] = DEFAULT_DATA;
			status = e_parseStatus_done;
			break;
		default:
			pOutput->dataLength = DEFAULT_DATA_LENGTH;
			pOutput->address = DEFAULT_ADDRESS;
			pOutput->data[0] = DEFAULT_DATA;
			status = e_parseStatus_error;
			break;
		}
	}
	return status;
}


int main(int argc, char const *argv[])
{
    uint8_t input[DEFAULT_DATA_LENGTH];
    uint8_t lengthInput = 0;
    parse_data_struct_t pOutput;
    parse_status_t status;
    FILE* pFile;
    pFile = fopen("filehandling.srec","r");
    status = e_parseStatus_undefined;
    if(status == e_parseStatus_undefined)
    {
    	while(1)
	    {
	    	outputStatus(&status);
	        lengthInput = fgetLine(pFile,input,DEFAULT_DATA_LENGTH,'S');
	        if(feof(pFile))
	        {
	        	return 0;
	        }
	        else if(checkSum(input,lengthInput) == FALSE)
	        {
	        	status = e_parseStatus_error;
	        	return 0;
	        }
	        else
	        {
	        	status = parseData(input,&pOutput);
		    	outputParseData(&pOutput);  
	        }
		}
    }
    return 0;
}