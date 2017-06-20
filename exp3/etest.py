# encoding: utf-8

import os, sys, re
import random

MAXN = 10000

def gendata():
    lines = open('driver_dataset/shenzhou_node.data', 'r').readlines()

    with open('driver.txt', 'w') as f:
        for i in range(MAXN):
            x = random.randint(0, len(lines)-1)
            f.write(lines[x])
        f.close()
    
    with open('rider.txt', 'w') as f:
        for i in range(MAXN):
            x = random.randint(0, len(lines)-1)
            f.write(lines[x])
        f.close()

def parseTable():
    print '<table align=center>'
    print '<thead>'
    print '<tr><th rowspan="2">测试点</th><th colspan="2">欧几里得距离</th><th colspan="2">常规路网</th><th colspan="2">优化路网</th><th rowspan="2">权值矩阵加速比(估计)</th></tr>'
    print '<tr><td>共享比之和</td><td>运行时间</td><td>共享比之和</td><td>运行时间</td><td>共享比之和</td><td>运行时间</td></tr>'
    print '</thead>'
    print '<tbody>'

    for i in range(7):
        print '<tr>'
        print '<td>%d</td>' % i
        lines = open('reports/%d.txt' % i, 'r').readlines()
        for line in lines:
            if line.strip() == '': break
            print '<td>%s</td><td>%s</td>' % (line.split(' ')[1], line.split(' ')[2])
        A = float(lines[0].split(' ')[2])
        B = float(lines[1].split(' ')[2])
        C = float(lines[2].split(' ')[2])
        print '<td>%lf</td>' % ((B-A)/(C-A))
        print '</tr>'

    print '</tbody>'
    print '</table>'

parseTable()
# gendata()

# if __name__ == '__main__':
#     os.system('mkdir -p reports')

#     for i in range(50):
#         gendata()
#         os.system('./main')
#         os.system('cp report.txt reports/%d.txt' % i)