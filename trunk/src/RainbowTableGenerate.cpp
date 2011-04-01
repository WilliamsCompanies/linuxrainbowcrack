/*
   RainbowCrack - a general propose implementation of Philippe Oechslin's faster time-memory trade-off technique.

   Copyright (C) Zhu Shuanglei <shuanglei@hotmail.com>
*/

#ifdef _WIN32
	#pragma warning(disable : 4786)
#endif

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif
#include <time.h>

#include "ChainWalkContext.h"

void Usage()
{
	Logo();

	printf("usage: rtgen hash_algorithm \\\n");
	printf("             plain_charset plain_len_min plain_len_max \\\n");
	printf("             rainbow_table_index \\\n");
	printf("             rainbow_chain_length rainbow_chain_count \\\n");
	printf("             file_title_suffix\n");
	printf("       rtgen hash_algorithm \\\n");
	printf("             plain_charset plain_len_min plain_len_max \\\n");
	printf("             rainbow_table_index \\\n");
	printf("             -bench\n");
	printf("\n");

	CHashRoutine hr;
	printf("hash_algorithm:       available: %s\n", hr.GetAllHashRoutineName().c_str());
	printf("plain_charset:        use any charset name in charset.txt here\n");
	printf("                      use \"byte\" to specify all 256 characters as the charset of the plaintext\n");
	printf("plain_len_min:        min length of the plaintext\n");
	printf("plain_len_max:        max length of the plaintext\n");
	printf("rainbow_table_index:  index of the rainbow table\n");
	printf("rainbow_chain_length: length of the rainbow chain\n");
	printf("rainbow_chain_count:  count of the rainbow chain to generate\n");
	printf("file_title_suffix:    the string appended to the file title\n");
	printf("                      add your comment of the generated rainbow table here\n");
	printf("-bench:               do some benchmark\n");

	printf("\n");
	printf("example: rtgen lm alpha 1 7 0 100 16 test\n");
	printf("         rtgen md5 byte 4 4 0 100 16 test\n");
	printf("         rtgen sha1 numeric 1 10 0 100 16 test\n");
	printf("         rtgen lm alpha 1 7 0 -bench\n");
}

void Bench(string sHashRoutineName, string sCharsetName, int nPlainLenMin, int nPlainLenMax, int nRainbowTableIndex)
{
	// Setup CChainWalkContext
	if (!CChainWalkContext::SetHashRoutine(sHashRoutineName))
	{
		printf("hash routine %s not supported\n", sHashRoutineName.c_str());
		return;
	}
	if (!CChainWalkContext::SetPlainCharset(sCharsetName, nPlainLenMin, nPlainLenMax))
		return;
	if (!CChainWalkContext::SetRainbowTableIndex(nRainbowTableIndex))
	{
		printf("invalid rainbow table index %d\n", nRainbowTableIndex);
		return;
	}

	// Bench hash
	{
	CChainWalkContext cwc;
	cwc.GenerateRandomIndex();
	cwc.IndexToPlain();

	clock_t t1 = clock();
	int nLoop = 2500000;
	int i;
	for (i = 0; i < nLoop; i++)
		cwc.PlainToHash();
	clock_t t2 = clock();
	float fTime = 1.0f * (t2 - t1) / CLOCKS_PER_SEC;

	printf("%s hash speed: %d / s\n", sHashRoutineName.c_str(), int(nLoop / fTime));
	}

	// Bench step
	{
	CChainWalkContext cwc;
	cwc.GenerateRandomIndex();

	clock_t t1 = clock();
	int nLoop = 2500000;
	int i;
	for (i = 0; i < nLoop; i++)
	{
		cwc.IndexToPlain();
		cwc.PlainToHash();
		cwc.HashToIndex(i);
	}
	clock_t t2 = clock();
	float fTime = 1.0f * (t2 - t1) / CLOCKS_PER_SEC;

	printf("%s step speed: %d / s\n", sHashRoutineName.c_str(), int(nLoop / fTime));
	}
}

int main(int argc, char* argv[])
{
	if (argc == 7)
	{
		if (strcmp(argv[6], "-bench") == 0)
		{
			Bench(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
			return 0;
		}
	}

	if (argc != 9)
	{
		Usage();
		return 0;
	}

	string sHashRoutineName  = argv[1];			//Algorith type e.g md5
	string sCharsetName      = argv[2];			//charset name
	int nPlainLenMin         = atoi(argv[3]);	//minimum length
	int nPlainLenMax         = atoi(argv[4]);	//maximum length
	int nRainbowTableIndex   = atoi(argv[5]);	//table index

	int nRainbowChainLen     = atoi(argv[6]);	//chain length
	int nRainbowChainCount   = atoi(argv[7]);	//chain count
	string sFileTitleSuffix  = argv[8];			//suffix

	// nRainbowChainCount check
	if (nRainbowChainCount >= 134217728)		//make sure that chain count is correct to avoid big rt's
	{
		printf("this will generate a table larger than 2GB, which is not supported\n");
		printf("please use a smaller rainbow_chain_count(less than 134217728)\n");
		return 0;
	}

	// Setup CChainWalkContext
	if (!CChainWalkContext::SetHashRoutine(sHashRoutineName))		//check to see if we support the hash
	{
		printf("hash routine %s not supported\n", sHashRoutineName.c_str());
		return 0;
	}
	if (!CChainWalkContext::SetPlainCharset(sCharsetName, nPlainLenMin, nPlainLenMax))		//check charset
		return 0;
	if (!CChainWalkContext::SetRainbowTableIndex(nRainbowTableIndex))		//make sure we have correct index
	{
		printf("invalid rainbow table index %d\n", nRainbowTableIndex);
		return 0;
	}
	CChainWalkContext::Dump();

	// Low priority. We need to be at low priority in order to not interfere with the system's functions
	//and to provide the user with clean usage of the OS
#ifdef _WIN32
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);
#else
	nice(1);
#endif

	// FileName
	char szFileName[256];
	sprintf(szFileName, "%s_%s#%d-%d_%d_%dx%d_%s.rt", sHashRoutineName.c_str(),
													  sCharsetName.c_str(),
													  nPlainLenMin,
													  nPlainLenMax,
													  nRainbowTableIndex,
													  nRainbowChainLen,
													  nRainbowChainCount,
													  sFileTitleSuffix.c_str());	//define filename

	// Open file
	fclose(fopen(szFileName, "a"));				//create the file
	FILE* file = fopen(szFileName, "r+b");		//open the file for reading in binary
	if (file == NULL)	//check to see if we opened the file correctly
	{
		printf("failed to create %s\n", szFileName);
		return 0;
	}

	// Check existing chains
	unsigned int nDataLen = GetFileLen(file);
	nDataLen = nDataLen / 16 * 16;					//??????????? Don't get it either
	if (nDataLen == nRainbowChainCount * 16)
	{
		printf("precomputation of this rainbow table already finished\n");
		fclose(file);
		return 0;
	}
	if (nDataLen > 0)
		printf("continuing from interrupted precomputation...\n");
	fseek(file, nDataLen, SEEK_SET);

	// Generate rainbow table
	printf("generating...\n");
	CChainWalkContext cwc;
	clock_t t1 = clock();
	int i;
	for (i = nDataLen / 16; i < nRainbowChainCount; i++)
	{
		
		cwc.GenerateRandomIndex();
		uint64 nIndex = cwc.GetIndex();							// get index
		if (fwrite(&nIndex, 1, 8, file) != 8)
		{
			printf("disk write fail\n");
			break;
		}

		int nPos;
		for (nPos = 0; nPos < nRainbowChainLen - 1; nPos++)		//generate a chain
		{
			cwc.IndexToPlain();
			cwc.PlainToHash();
			cwc.HashToIndex(nPos);
		}

		nIndex = cwc.GetIndex();								//get new index
		if (fwrite(&nIndex, 1, 8, file) != 8)
		{
			printf("disk write fail\n");
			break;
		}

		//Display status
		if ((i + 1) % 100000 == 0 || i + 1 == nRainbowChainCount)
		{
			clock_t t2 = clock();
			int nSecond = (t2 - t1) / CLOCKS_PER_SEC;
			int chainsleft = nRainbowChainCount - (i+1);
			int cl = chainsleft/100000 ;
			int secondsleft=cl*nSecond;
			//printf("Chains left : %d\nCL : %ld\n",chainsleft,cl);
			int hrleft = secondsleft/3600;
			int dleft = hrleft/24;
			hrleft=hrleft-(dleft*24);
			int mleft = (secondsleft/60)-(hrleft*60)-(dleft*24*60);
			int sleft = secondsleft-(mleft*60)-(hrleft*3600)-(dleft*24*3600);
			//printf("hms: %d,%d,%d - %d\n",hrleft,mleft,sleft,secondsleft);
			printf("%d of %d rainbow chains generated (%d m %d s) -> %2f%%  ETA: %d days %d hrs %d mins %d sec\n", i + 1,
																	  nRainbowChainCount,
																	  nSecond / 60,
																	  nSecond % 60,
																	  (((i+1)*100)/nRainbowChainCount),
																	  dleft,hrleft,mleft,sleft);
			t1 = clock();
		}
		
	}

	// Close
	fclose(file);

	return 0;
}
