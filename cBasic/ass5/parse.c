#include "parse.h"

#define DEBUG 0

#if defined(DEBUG) && (DEBUG)
	#define PRINTF  printf
#else
	#define PRINTF
#endif

/**
 * [getHex description]
 * @param  pInput  [description]
 * @param  mode    [description]
 * @param  address [description]
 * @return         [description]
 */
static uint32_t getHex(uint8_t pInput[], uint8_t mode, uint8_t address);
/**
 * [checkLen description]
 * @param  pInput [description]
 * @param  length [description]
 * @return        [description]
 */
static uint8_t checkLen(uint8_t pInput[],uint8_t length);
/**
 * [checkSum description]
 * @param  pInput [description]
 * @param  length [description]
 * @return        [description]
 */
static uint8_t checkSum(uint8_t pInput[], uint8_t length);


static uint32_t getHex(uint8_t pInput[], uint8_t mode, uint8_t address)
{
	uint8_t count = 0;
	uint32_t temp = 0;
	PRINTF("getHex\n");	
	/*set loop for as 8 bit*/
	for (count = 0; count < mode/4; ++count)
	{
		SET_8BIT(temp, coverCharToHex(pInput[count + address]), coverCharToHex(pInput[ ++count + address]));
		PRINTF("temp: %x\n", temp);
	}
	PRINTF("stop getHex\n");
	return temp;
}

static uint8_t checkLen(uint8_t pInput[],uint8_t length)
{
	uint8_t temp = 0;
	temp = getHex(pInput, MODE_8BIT, 2);
	if(temp == ((length -4)/2))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static uint8_t checkSum(uint8_t pInput[], uint8_t length)
{
	uint8_t count = 0;
	uint8_t sum = 0;
	uint8_t temp = 0;
	PRINTF("start checkSum\n");
	for (count = 2; count < length - 2; ++count)
	{
		temp = getHex(pInput, MODE_8BIT, count++);
		PRINTF("temp in for: %x\n", temp);
		sum += temp;
		PRINTF("sum in for: %x\n", sum);
	}
	temp = getHex(pInput, MODE_8BIT, length - 2);
	PRINTF("sum out for: %x\n", (~sum) & 0xff);
	PRINTF("temp out for: %x\n", temp);
	if (temp == ((~sum)&(0xff)))
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

void outputParseData(parse_data_struct_t* pOutput)
{
	if(pOutput->address != DEFAULT_ADDRESS)
	{
		printf("Address: %X\n", pOutput->address);
	}
	if(pOutput->dataLength != DEFAULT_DATA_LENGTH)
	{
		printf("Data length: %X\n", pOutput->dataLength);
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

parse_status_t parseData(uint8_t pInput[], parse_data_struct_t *pOutput)
{
	PRINTF("start paserData\n");
	parse_status_t status;
	uint8_t length = 0;
	length = strLength(pInput);
	if(pInput[0] == 'S')
	{	
		if( checkLen(pInput,length) == FALSE || checkSum(pInput,length) == FALSE)
		{
			status = e_parseStatus_error;
			return status;
		}
		else
		{
			switch(pInput[1])
			{
			case '0':
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
	}
	else
	{
		status = e_parseStatus_error;
		return status;
	}
	PRINTF("stop paserData\n");
	return status;
}