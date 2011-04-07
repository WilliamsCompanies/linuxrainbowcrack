#include "Public.h"
#include <strings.h>

void usage(){
	printf("rtstats <tablename.rt>\n");
}
int main(int argc, char* argv[]){
	if(argc!=2){
		usage();
		return 0;
	}
	string sPathName = argv[1];
	//printf("Stats for %s:\n",sPathName.c_str());
	FILE* file = fopen(sPathName.c_str(), "rb");
	if (file == NULL)
	{
		printf("failed to open %s\n", sPathName.c_str());
		return 0;
	}
	unsigned int nDataLen = GetFileLen(file);
	int nChainCount=nDataLen/16;
	printf("Number of chains generated: %d\n",nChainCount);
}
