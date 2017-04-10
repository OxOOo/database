# encoding: utf-8

import os, sys, re
import random

STR_LIST = 'qwertyuioplkjhgfdsazxcvbnmQWERTYUIOPLKJHGFDSAZXCVBNM'
N = 5000
M = 10000

def main():
    input = open('input.txt', 'w')
    query = open('query.txt', 'w')
    for i in range(N):
        l = random.randint(100, 256)
        buf = ''.join([STR_LIST[random.randint(0, len(STR_LIST)-1)] for x in range(l)])
        input.write(buf + '\n')
        query.write(buf + '\n')
        input.write(buf + '\n')
        query.write(buf + '\n')
    input.close()
    query.close()

if __name__ == '__main__':
    main()