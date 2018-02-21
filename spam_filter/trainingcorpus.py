from corpus import Corpus
import os

class TrainingCorpus(Corpus):
    def get_class(self,file_name):
        self.path0=os.getcwd()
        os.chdir(self.path)
        with open('!truth.txt','r',encoding='utf-8') as f:
            lines=f.readlines()
            os.chdir(self.path0)
            for line in lines:
                if file_name in line:
                    line_list=line.split(' ')
                    classification=line_list[1].replace("\n","")
                    #print(l)
                    return classification

    def is_spam(self,file_name):
        if (self.get_class(file_name)=='SPAM'):
            return True
        else:
            return False

    def is_ham(self,file_name):
        if (self.get_class(file_name)=='OK'):
            return True
        else:
            return False

    def spams(self):
        files=os.listdir(self.path)
        for file_name in files:
            if self.is_spam(file_name):
                os.chdir(self.path)
                with open(file_name,'r',encoding='utf-8') as f:
                    body=f.read()
                os.chdir(self.path0)
                yield file_name,body

    def hams(self):
        files = os.listdir(self.path)
        for file_name in files:
            if self.is_ham(file_name):
                os.chdir(self.path)
                with open(file_name,'r',encoding='utf-8') as f:
                    body = f.read()
                os.chdir(self.path0)
                yield file_name,body

if __name__=="__main__":
    corpus = TrainingCorpus('/home/palo/Documents/python/spam_filter/2')
    for file,body in corpus.spams():
        print(file,body)
