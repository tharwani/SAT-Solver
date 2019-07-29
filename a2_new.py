import random
import time
start = time.time()
class otpt:
    lt = []
    steps = 0
def SAT_solver(arrofarr):
    if arrofarr==[] and len(otpt.lt)>0:
        return True
    ol = list(i for i in otpt.lt)
    arrofarr.sort(key=len)
    while len(arrofarr[0])==1:
        new = arrofarr[0][0]
        otpt.lt.append(new)
        l=0
        while l<len(arrofarr):
            if arrofarr[l].count(new)>0:
                arrofarr.remove(arrofarr[l])
                continue
            elif arrofarr[l].count(-new)>0 and len(arrofarr[l])>1 and len(arrofarr[l])>arrofarr[l].count(-new):
                while arrofarr[l].count(-new)>0:
                    arrofarr[l].remove(-new)
            elif len(arrofarr[l])==arrofarr[l].count(-new):
                otpt.lt=ol
                return False
            l+=1
        if arrofarr==[] and len(otpt.lt)>0:
            return True
        arrofarr.sort(key=len)
    main = list(i for i in arrofarr[0]) 
    while len(main)>0:
        k=main[0]
        while main.count(k)>1:
            # ln-=1
            main.remove(k)
        otpt.lt.append(k)
        arrofarr2 = []
        temp_flag = False
        j=0
        while j<len(arrofarr):
            if arrofarr[j].count(k)>0:
                j+=1
                continue
            elif arrofarr[j].count(-k)>0 and len(arrofarr[j])>1 and len(arrofarr[j])>arrofarr[j].count(-k):
                while arrofarr[j].count(-k)>0:
                    arrofarr[j].remove(-k)
                temp = list(i for i in arrofarr[j])
                arrofarr2.append(temp)
                arrofarr[j].append(-k)
            elif len(arrofarr[j])==arrofarr[j].count(-k):
                while arrofarr[j].count(-k)>1:
                    arrofarr[j].remove(-k)
                temp_flag = True
                break
            else:
                temp = list(i for i in arrofarr[j])
                arrofarr2.append(temp)
            j+=1
        if temp_flag:
            otpt.lt.remove(k)
            otpt.lt.append(-k)
            l=0
            while l<len(arrofarr):
                if(arrofarr[l].count(-k)>0):
                    arrofarr.remove(arrofarr[l])
                    continue
                elif(arrofarr[l].count(k)>0 and len(arrofarr[l])>1 and len(arrofarr[l])>arrofarr[l].count(k)):
                    while(arrofarr[l].count(k)>0):
                        arrofarr[l].remove(k)
                elif len(arrofarr[l])==arrofarr[l].count(k):
                    otpt.lt=ol
                    return False
                l+=1
            if arrofarr==[] and len(otpt.lt)>0:
                return True
            main.remove(k)
            continue 
        otpt.steps += 1
        if(SAT_solver(arrofarr2)):
            return True
        else:
            otpt.lt.remove(k)
            otpt.lt.append(-k)
            l=0
            while l<len(arrofarr):
                if(arrofarr[l].count(-k)>0):
                    arrofarr.remove(arrofarr[l])
                    continue
                elif(arrofarr[l].count(k)>0 and len(arrofarr[l])>1 and len(arrofarr[l])>arrofarr[l].count(k)):
                    while arrofarr[l].count(k)>0:
                        arrofarr[l].remove(k)
                elif len(arrofarr[l])==arrofarr[l].count(k):
                    otpt.lt=ol
                    return False
                l+=1
            if(arrofarr==[] and len(otpt.lt)>0):
                return True
            main.remove(k)
    otpt.lt=ol
    return False
f = open("uf50-022.cnf","r")
f1 = f.readlines()
inpt_list = []
for x in f1:
    if(x[0]=='c'):
        continue
    temp = list(x.split(" "))
    if(temp[0]=='p'):
        no_of_literals = int(temp[2])
        no_of_clauses = int(temp[3])
        continue
    temp2 = []
    for i in temp:
         temp2.append(int(i))
    while(temp2.count(0)>0):
        temp2.remove(0)
    inpt_list.append(temp2)
inpt_list.sort(key=len)
if(SAT_solver(inpt_list)):
    # print(otpt.lt)
    # print(len(otpt.lt))
    final_otpt = []
    for i in range(1,no_of_literals+1):
        if(otpt.lt.count(i)>0):
            final_otpt.append(i)
        elif(otpt.lt.count(-i)>0):
            final_otpt.append(-i)
        else:
            final_otpt.append(pow(-1,random.randint(0,2))*i)
    print("SAT")
    for i in final_otpt:
        print(i,end=' ')
    print(0)
else:
    print("UNSAT")
print("No of steps =",end=' ')
print(otpt.steps)
end=time.time()
print("Total time in secs =",end=' ')
print(end-start)
