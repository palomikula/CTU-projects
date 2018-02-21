import os
import quality
import random

class BaseFilter();
    def __init__(self):
        self.dict={}
    def train(self,path):


    def test(self,path):
        names = os.listdir(path)
        path0 = os.getcwd()
        os.chdir(path)
        fd=open('!prediction.txt', 'w',encoding='utf-8')
        for i in names:
            fd.write(i + ' ' + self.dict[i] + '\n')
        fd.close()
        os.chdir(path0)
        pass
