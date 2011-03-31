
all: rtgen rtdump rtsort rcrack

rtgen:
	g++ Public.cpp ChainWalkContext.cpp HashAlgorithm.cpp HashRoutine.cpp RainbowTableGenerate.cpp /lib/libcrypto.so -lssl -O3 -o rtgen

rtdump:
	g++ Public.cpp ChainWalkContext.cpp HashAlgorithm.cpp HashRoutine.cpp RainbowTableDump.cpp /lib/libcrypto.so -lssl -o rtdump

rtsort:
	g++ Public.cpp RainbowTableSort.cpp /lib/libcrypto.so -o rtsort

rcrack:
	g++ Public.cpp ChainWalkContext.cpp HashAlgorithm.cpp /lib/libcrypto.so HashRoutine.cpp HashSet.cpp MemoryPool.cpp ChainWalkSet.cpp CrackEngine.cpp RainbowCrack.cpp -lssl -O3 -o rcrack
