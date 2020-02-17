#-------------------------------------------------------------------------------
# Name:        Level Generator
# Purpose:     Wave Generator for Rain Panic
#
# Author:      Jean-Baptiste PERIN
#
# Created:     23/01/2020
#-------------------------------------------------------------------------------
import random




def val2txt(val):
    if ((val == 9) or (val == 8)):
        return "0x%02x"%(val)
    else:   
        return "%d"%(val)

class sequence ():
    def __init__(self, duration):
        self.__duration__= duration
        self.__seq__=[(duration, 255)]

    def __repr__(self):
        pass

    def __str__(self):
        return (str(self.__seq__))

    def insert (self, tim, evt):
        if (tim < self.__duration__):
            self.__seq__.append((tim, evt))
        else :
            print ("Warning rejected _events: tim = %d evt = %d. please check DOP"%(tim, evt))

    def toCarray(self):
        res = ""
        sl = sorted(self.__seq__, key=lambda evt: evt[0])
        cum = 0
        for i in range(len(sl)-1):
            res += "" if (i == 0) else ", "
            code, dur = sl[i][1], sl[i+1][0]-sl[i][0]
            cum = cum + dur
            strCode, strDur = val2txt(code), val2txt(dur)
            res += "%s, %s"%(strCode, strDur)
            if (cum >= 10): 
                cum = 0
                res += "\n"
        res += ", 255, 0,\n"
        return res

    def spread (self, nb, evtgetter, pattern):

        def dop_random (thePattern):
            if (thePattern == "UNIFORM"):
                return random.randrange(0, self.__duration__)
            elif (thePattern == "GAUSS"):
                return int(random.gauss(self.__duration__/2, self.__duration__/4))
            elif (thePattern == "TRIANGULAR"):
                return int(random.triangular(1, self.__duration__, self.__duration__*0.85))

        tims=[]
        for i in range (nb):
            nb_attempt = 0
            t=dop_random (pattern)
            while (((t in tims) and (nb_attempt < 100)) or ((t>self.__duration__) or (t<=0))):
                t=dop_random (pattern)
                nb_attempt += 1
            if (nb_attempt >= 100): 
                print ("Warning: max attempt reached. Wave is too loaded ?")
                break
            tims.append (t)
            #print ("added time: %d %s"%(t,tims))

        for t in tims:
            self.insert(t,evtgetter())

def raindrop(): return random.randrange(0,13)*2 + 7

def lightning(): return random.randrange(0,13)*2 + 200

def life(): return random.randrange(0,13)*2 + 180

def ice(): return random.randrange(0,13)*2 + 150

def cat(): return random.choice([250, 251])

def fireball(): return random.choice([252, 253])

def generateWaves():
    waves_list = []

    S=sequence(50)
    # Sequence 1: only Rain Drop
    S.spread (24, raindrop, "UNIFORM")
    waves_list.append(S)

    S=sequence(50)
    # Sequence 2: Rain Drop and Ice
    S.spread (24, raindrop, "UNIFORM")
    S.spread (5, ice, "GAUSS")
    waves_list.append(S)

    S=sequence(50)
    # Sequence 3: Rain Drop and Ice
    S.spread (24, raindrop, "UNIFORM")
    # S.spread (5, lightning, "UNIFORM")
    # S.spread (5, cat, "GAUSS")
    S.spread (5, ice, "GAUSS")
    waves_list.append(S)

    S=sequence(60)
    # Sequence 4: Rain Drop, Ice and Life
    S.spread (24, raindrop, "UNIFORM")
    # S.spread (5, lightning, "UNIFORM")
    # S.spread (5, cat, "GAUSS")
    S.spread (10, ice, "GAUSS")
    S.spread (10, life, "GAUSS")
    waves_list.append(S)

    S=sequence(80)
    # Sequence 5: Rain Drop, Lighning and Life
    S.spread (30, raindrop, "UNIFORM")
    S.spread (5, lightning, "UNIFORM")
    # S.spread (5, cat, "GAUSS")
    # S.spread (10, ice, "GAUSS")
    S.spread (10, life, "GAUSS")
    waves_list.append(S)

    S=sequence(80)
    # Sequence 6: Rain Drop, Lighning and Life
    S.spread (30, raindrop, "UNIFORM")
    S.spread (15, lightning, "UNIFORM")
    # S.spread (5, cat, "GAUSS")
    # S.spread (10, ice, "GAUSS")
    S.spread (5, life, "GAUSS")
    waves_list.append(S)



    return waves_list

def main():


    waves_list = generateWaves()

    res = "unsigned char rain[] = {\n"
    cpt = 1
    for wav in waves_list:
        res +="// Wave %d\n"%(cpt)
        res += wav.toCarray()
        cpt += 1

    res += """// wave 1  time=3/2      x position (7 left---19 center -- right 31)
  253,3, 250,0, 19,2, 252,0, 32,2, 7,2, 253,0,	
  32,3, 19,3, 07,3,  32,3,  251,0 ,7,3, 16,3, 18,3, 20,3,
  07,3  ,9,3  ,250,0 ,26,3  ,28,3, 251,0,  32,3, 7,3, 250,0, 32,3, 
  15,3  ,17,3, 251,0 ,19,3  ,21,3, 250,0, 23,3, 
  07,3 ,9,3 ,11,3 ,251,0 ,28,3  ,32,3,  250,0, 07,3, 251,0 ,32,3, 22,3,
  251,0, 21,3,  252,0, 7,3, 250,0, 32,2, 251,0, 21,6,  // 39eme
  255,0, // wave 2 time =2
  7,3, 32,2, 32,2, 250,0, 7,2,
  13,3 ,30,2, 251,0, 07,3, 215,0, 32,3, 250,0 ,18,3, 250,0, 07,3, 32,2, 217,0, // 
  07,2, 251,0, 32,3,   250,0, 07,3, 251,0, 32,3,  250,0, 07,3, 207,0, 
  31,2, 17,3, 217,0, 32,2, 224,0, 7,1, 19,6,   
  255,0,  // wave 3 
  14,2, 251,0 ,25,2,  215,0, 250,0, 32,2, 07,2, 251,0, 32,2, // 73eme
  14,2,250,0, 16,2, 251,0, 18,2, 250,0, 20,2,217,0,14,2,218,0,32,2,
  07,2, 32,2, 251,0, 9,3, 220,0, 168,3, // ice here  reprendre ici 
  32,3, 07,3, 215,0, 31,3, 215,0, 07,3, 216,0, 30,3, 
  255,0, // wave 4
  250,0, 0x08,3,212,0, 13,2,  
  29,3, 251,0, 0x09,3,  230,0, 28,3, 251,0, 10,3, 251,0, 13,4, 230,0, 30,4,  16,4, 208,0, 
  07,4, 250,0, 32,4,   18,4, 07,4, 10,4, 251,0, 15,4, 11,4, 24,4, 
  255,0,  // wave 5 
  19,3,
  9,1, 11,1, 251,0, 13,1, 15,1, 250,0, 17,1,19,1, 
  251,0, 21,1, 23,1, 250,0, 25,1,27,4,
  250,0, 15,0, 251,0, 17,0 ,19,0, 250,0, 21,0,23,0, 25,3,
 
  7,0, 250,0, 9,0, 11,0, 250,0, 13,0 ,15,0, 17,3,251,0, 
  21,0, 23,0, 251,0, 25,0, 27,0, 29,0, 31,3, 
  7,0, 250,0, 9,0, 11,0, 251,0, 13,0 ,15,3,
  21,0, 23,0, 25,0, 27,0, 29,0, 31,3,
  7,0,9,0,11,1, 14,0,16,0,18,1, 21,0,23,0,25,1, 27,0,29,0,31,1, 
  19,2, 19,5,
  // one line
  //  [x,y] y =0 -> force to align drops in same line
"""
    res += "};\n"

    # print (res)

    with open('waves2.c', 'w') as file:  # Use file to refer to the file object
        file.write(res)




if __name__ == '__main__':
    main()
