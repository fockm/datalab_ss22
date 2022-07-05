import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# dockjer exec -it datalab_s22 bin/bash

path="../code/data/"

import docker
from io import StringIO

client = docker.from_env()
client.containers.list()
container=client.containers.get("datalab_s22")
container.attach()

def getFileName(N,sigma,amplitude,freq=0,deltaT=1,phase=0):
    return "N=%s-s=%s-d=%s-f=%s-a=%s-p=%s" % (N ,sigma, deltaT, freq, amplitude, phase)

def generate_source(N,sigma,amplitude,freq=0,deltaT=1,phase=0):
    filename = "N=%s-s=%s-d=%s-f=%s-a=%s-p=%s" % (N ,sigma, deltaT, freq, amplitude, phase)
    container.exec_run("/home/generate_source -N %s -s %s -d %s -f %s -a %s -p %s -o %s" % (N ,sigma, deltaT, freq, amplitude, phase, "/home/data/out_"+filename))
    return pd.read_csv(path+"out_"+filename,delimiter=' ',header=None)

def generate_source100(N,sigma,amplitude,freq=0,deltaT=1,phase=0):
    for i in range(0,100):
        filename = "N=%s-s=%s-d=%s-f=%s-a=%s-p=%s_%s" % (N ,sigma, deltaT, freq, amplitude, phase, i)
        container.exec_run("/home/generate_source -N %s -s %s -d %s -f %s -a %s -p %s -o %s" % (N ,sigma, deltaT, freq, amplitude, phase, "/home/data/out_"+filename))

def prober(filename,method,freq_prober=0,phi=0):
    if method==3:
        return container.exec_run("/home/prober -i %s -o %s -m %s" % ("home/data/out_"+filename ,"home/data/prober_"+filename, method))
    else:
        return container.exec_run("/home/prober -i %s -f %s -p %s -m %s" % ("home/data/out_"+filename, freq_prober, phi, method))
    
def prober_tb(filename,method,template):
        return container.exec_run("/home/prober -i %s -o %s -t %s -m %s" % ("home/data/out_"+filename ,"home/data/prober_tb_"+filename,"home/templates/"+template, method))

def method_one(filename,freq,phi):
    prober_return = str(prober(filename,1,freq,phi).output,'utf-8').split()
    return prober_return

def method_two(filename,freq):
    prober_return = [str(prober(filename,2,freq,i/250*2*np.pi).output,'utf-8').split() for i in range(0,250)]
    return pd.DataFrame(prober_return,dtype=float)

def method_three(filename):
    prober(filename,method=3)
    return pd.read_csv(path+"prober_"+filename,delimiter=' ',header=None)

def findSignal(filename):
    df = method_three(filename)
    freq  = df[0][df[2].idxmax(0)]
    df = method_two(filename,freq)    
    phi  = df[1][df[2].idxmin(0)]
    df = method_one(filename,freq,phi)  
    ampl = float(df[2])*2
    return freq,phi,ampl

def signalToDf(filename):
    return pd.read_csv(path+"out_"+filename,delimiter=' ',header=None)

def proberToDf(filename,tb=False):
    if tb:
        return pd.read_csv(path+"prober_tb_"+filename,delimiter=' ',header=None)
    else:
        return pd.read_csv(path+"prober_"+filename,delimiter=' ',header=None)