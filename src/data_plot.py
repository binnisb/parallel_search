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

#path = '../result/17gbfile/'
path = '../result/openmp17gb/'
a = os.listdir(path)
stats = []
for fs in a:
    f = open(''.join([path,fs]),'r')
    s = Stat(int(fs.split('.')[1]))
  
    max_search = 0
    max_load = 0

    for line in f:
        if line.split()[0] == 'LoadFile' or line.split()[0] == 'Search' :
            gaur =  float(line.split()[3])
            if line.split()[0] == 'LoadFile' and gaur > max_load:
                #s.load.append(float(line.split()[3]))
                max_load =  gaur
            
            elif line.split()[0] == 'Search' and gaur > max_search:                
                #s.search.append(float(line.split()[3]))
                max_search = gaur

        if (line.split()[0] == 'The' and line.split()[1] == 'sum') or (line.split()[0]=='result' and line.split()[1]=='found:'):
            s.load.append(max_load)
            s.search.append(max_search)
            max_load = 0
            max_search = 0
    stats.append(s)
stats.sort()
#for i in stats:
#    print i.search
figure(1)
hold(True)
for st in stats:
    aver  = average(st.search)
    stand = std( st.search) 
    print st.search
    print average(st.search)
    plot([st.id for i in range(2)],[aver-stand,aver+stand],'+k')
    plot(st.id,aver,'or')

#plot([s.id for s in stats],[average(s.search) for s in stats],'o-',label='search')
#legend()
xlabel("nr of process")
ylabel("searching time in seconds")

figure(2)
#plot([s.id for s in stats],[average(s.load) for s in stats],'xr-',label='load file')
#legend()
hold(True)
for st in stats:
    aver  = average(st.load)
    stand = std( st.load)
    plot([st.id,st.id],[aver-stand,aver+stand],'-k')
    plot([st.id for i in range(2)],[aver-stand,aver+stand],'+k')
    plot(st.id,aver,'or')

xlabel("nr of process")
ylabel("loading time in seconds")

first_guy = average(stats[0].search)
figure(3)
plot([s.id for s in stats],[first_guy/average(s.search) for s in stats],'o-',label='search')
plot([s.id for s in stats],[s.id for s in stats],'--',label='optimal speed-up')
legend(loc=2)
xlabel("nr of process")
ylabel("speed-up of the search")

# Amhads law
T_p = average(stats[0].search) # sequential time of the parallel part, search
T_s = average(stats[0].load)   # time of the sequential part
P = T_p/(T_s+T_p)
figure(4)
hold(True)
plot([s for s in range(1,25)],[1/((1-P)+(P/s)) for s in range(1,25)],'--',label='Amdahls law')    
plot([s.id for s in stats],[(T_p+T_s)/( average(s.load)+average(s.search) ) for s in stats],'o-r',label='our results') 
legend(loc=4)
xlabel("nr of process")
ylabel("speed-up")
 
figure(5)
T_s = 50
P = T_p/(T_s+T_p)
plot([s for s in range(1,25)],[1/((1-P)+(P/s)) for s in range(1,25)],'--',label='Amdahls law')
plot([s.id for s in stats],[(T_p+T_s)/( T_s+average(s.search) ) for s in stats],'o-r',label='our results')
legend(loc=4)
xlabel("nr of process")
ylabel("speed-up")


show()
