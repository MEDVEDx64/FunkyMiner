#!/bin/sh
# Just a stewpid buld script

if [ ! -d bin ]; then
	mkdir bin
fi

gcc FunkyMiner/*.c -o bin/funkyminer -O2 -lpthread -lssl -lcrypto
