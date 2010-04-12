#ifndef _CHAINWALKCONTEXT_H
#define _CHAINWALKCONTEXT_H

#include "HashRoutine.h"
#include "Public.h"

class CDistGenTable
{
public:
	CDistGenTable();
	static void GenChains(int number);
	bool bInit(string sHashRoutineName,string sCharsetName,int nPlainlenMin,int nPlainLenMax,string sHost);
	bool bCheckConnection();
	bool bHasWorkDone();
	static void SendWork(string sHost,int ChainCount);
	static void GetClientID(string sHost);
	static void ReceiveResult(string sHost);
	static void GetHost(string sHost);
private:
	
	int startchain;
	int endchain;
	string sHost;
}

#endif
