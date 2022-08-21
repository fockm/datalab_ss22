import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# docker exec -it datalab_s22 bin/bash

path="../code/assignment/"

import docker
from io import StringIO

client = docker.from_env()
client.containers.list()
container=client.containers.get("datalab_s22")
container.attach()

# generate_source

def generate_source(N,sigma,amplitude,freq=0,deltaT=1,phase=0,filename=None):
    if filename==None:
        filename = "N=%s-s=%s-d=%s-f=%s-a=%s-p=%s" % (int(N) ,sigma, deltaT, freq, amplitude, phase)
    return container.exec_run("/home/generate_source -N %s -s %s -d %s -f %s -a %s -p %s -o %s" % (int(N) ,sigma, deltaT, freq, amplitude, phase, "/home/assignment/"+filename))
    return pd.read_csv(path+"out_"+filename,delimiter=' ',header=None)

def generate_source100(N,sigma,amplitude,freq=0,deltaT=1,phase=0,anzahl=100):
    for i in range(0,anzahl):
        filename = "N=%s-s=%s-d=%s-f=%s-a=%s-p=%s_%s" % (int(N) ,sigma, deltaT, freq, amplitude, phase, i)
        container.exec_run("/home/generate_source -N %s -s %s -d %s -f %s -a %s -p %s -o %s" % (int(N) ,sigma, deltaT, freq, amplitude, phase, "/home/assignment/"+filename))
        
# prober

def prober(filename,method=3,freq_prober=0,phi=0):
    if method==3:
        return container.exec_run("/home/prober -i %s -o %s -m %s" % ("home/assignment/"+filename ,"/home/assignment/prober_"+filename, method))
    else:
        return container.exec_run("/home/prober -i %s -f %s -p %s -m %s" % ("home/assignment/"+filename, freq_prober, phi, method))
    
def prober_tb(filename,method,template):
        return container.exec_run("/home/prober -i %s -o %s -t %s -m %s" % ("home/assignment/"+filename ,"home/assignment/prober_tb_"+filename,"home/templates/"+template, method))

def method_one(filename,freq,phi):
    prober_return = str(prober(filename,1,freq,phi).output,'utf-8').split()
    return prober_return

def method_three(filename):
    prober(filename,method=3)
    return pd.read_csv(path+"prober_"+filename,delimiter=' ',header=None)

# Templatebank

def createTemplateBank(file, fn, fmin, fmax, phin, phimin=0, phimax=2*np.pi):
    f = np.linspace(fmin, fmax, fn)
    phi = np.linspace(phimin, phimax, phin)
    stack=np.dstack(np.meshgrid(f,phi))
    return np.savetxt("../code/templates/"+file, stack.reshape((fn*phin,2)), delimiter=' ')

# ROC curve

def compute_thresholds(proberS, proberN):
    thresholds=np.linspace(-1,1,100)
    FP=[len([i for i in proberN if i>threshold]) for threshold in thresholds]
    TP=[len([i for i in proberS if i>threshold]) for threshold in thresholds]
    return pd.DataFrame(data={'threshold':thresholds,'false positives': FP,'true positives': TP})

def roc_curve(proberS, proberN):
    data = compute_thresholds(proberS,proberN)
    plt.plot(data['false positives']/100,data['true positives']/100)
    plt.show()
    return data[data['true positives']>80]['threshold'].max()

# File reading

def getFileName(N,sigma,amplitude,freq=0,deltaT=1,phase=0):
    return "N=%s-s=%s-d=%s-f=%s-a=%s-p=%s" % (int(N) ,sigma, deltaT, freq, amplitude, phase)

def signalToDf(filename):
    return pd.read_csv(path+"out_"+filename,delimiter=' ',header=None)

def proberToDf(filename,tb=False):
    if tb:
        return pd.read_csv(path+"prober_tb_"+filename,delimiter=' ',header=None)
    else:
        return pd.read_csv(path+"prober_"+filename,delimiter=' ',header=None)