
all: rtgen rtdump rtsort rcrack rtstats

rtgen:
	g++ Public.cpp ChainWalkContext.cpp HashAlgorithm.cpp HashRoutine.cpp RainbowTableGenerate.cpp -lcrypto -lpthread -lssl -O3 -o rtgen

rtdump:
	g++ Public.cpp ChainWalkContext.cpp HashAlgorithm.cpp HashRoutine.cpp RainbowTableDump.cpp -lcrypto -lssl -o rtdump

rtsort:
	g++ Public.cpp RainbowTableSort.cpp -lcrypto -o rtsort

rcrack:
	g++ Public.cpp ChainWalkContext.cpp HashAlgorithm.cpp -lcrypto HashRoutine.cpp HashSet.cpp MemoryPool.cpp ChainWalkSet.cpp CrackEngine.cpp RainbowCrack.cpp -lssl -O3 -o rcrack
rtstats:
	g++ rtstats.cpp Public.cpp -o rtstats
