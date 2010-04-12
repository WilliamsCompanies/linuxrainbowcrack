#include "DistGenTable.h"
CDistGenTable::bCheckConnection(){
	
}
CDistGenTable::bInit(string sHashRoutineName,string sCharsetName,int nPlainlenMin,int nPlainLenMax,string sHost){
	
}

CDistGenTable::bHasWorkDone(){
}
CDistGenTable::SendWork(string sHost,int ChainCount){
}
CDistGenTable::GetClientID(string sHost){
}
CDistGenTable::ReceiveResult(string sHost){
}
CDistGenTable::GetHost(string sHost){
}
CDistGenTable::GenChains(int number){
	for (i = 0; i <= number; i++){
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
			printf("%d of %d rainbow chains generated (%d m %d s)\n", i + 1,
																	  nRainbowChainCount,
																	  nSecond / 60,
																	  nSecond % 60);
			t1 = clock();
		}
		/*
		 * if(dgt.bHasWorkDone()){
			 * 
		}
		*/
	}
}
