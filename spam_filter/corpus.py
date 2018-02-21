import os
class Corpus():
    def __init__(self,path):
        self.path=path
    def emails(self):
        files = os.listdir(self.path)
        path=os.getcwd()
        for fname in files:
            if(fname[0] != '!'):
                os.chdir(self.path)
                with open(fname,'r',encoding='utf-8') as f:
                    body=f.read()
                os.chdir(path)
                yield fname,body

if __name__=="__main__":
    corpus=Corpus('/home/palo/Documents/python/spam_filter/2')
    for fname,body in corpus.emails():
        print(fname)
        print(body)