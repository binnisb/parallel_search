from collections import defaultdict
import os
from pylab import *

class Stat:
    def __init__(self,id):
        self.id = id
        self.load = []
        self.search = []
        
    def __lt__(self,stat):
        return self.id < stat.id
    
    def __repr__(self):
        return self.__str__()
    def __str__(self):
        return "%s" %(self.id)

path = '../result/17gbfile/'
a = os.listdir(path)
stats = []
for fs in a:
    f = open(''.join([path,fs]),'r')
    s = Stat(int(fs.split('.')[1]))
    for line in f:
        if line.split()[0] == 'LoadFile:':
            s.load.append(float(line.split()[1]))
        elif line.split()[0] == 'Search:':
            s.search.append(float(line.split()[1]))
    stats.append(s)
stats.sort()
#for i in stats:
#    print i.search
figure(1)
plot([s.id for s in stats],[average(s.search) for s in stats],'o-',label='search')
legend()
xlabel("nr of process")
ylabel("searching time in seconds")

figure(2)
plot([s.id for s in stats],[average(s.load) for s in stats],'xr-',label='load file')
legend()
xlabel("nr of process")
ylabel("loading time in seconds")

first_guy = average(stats[0].search)
figure(3)
plot([s.id for s in stats],[first_guy/average(s.search) for s in stats],'o-',label='search')
legend()
xlabel("nr of process")
ylabel("speed-up of the search")


show()
