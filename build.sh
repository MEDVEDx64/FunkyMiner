#!/bin/sh

gcc FunkyMiner/*.c -o funkyminer -O2 -lpthread -lssl -lcrypto
