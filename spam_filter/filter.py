from corpus import Corpus
from trainingcorpus import TrainingCorpus
import os
from collections import Counter
#import quality

class MyFilter():
    def __init__(self):
        self.bad_words=[]
        self.final_dict={}
        pass

    def train(self,path):
        '''this function creates list of strings
        (bad_words) that are most likely to trigger
        a spam in the test dataset of emails'''

        #these constants worked the best
        MOST_COMMON_S = 600
        MOST_COMMON_H = 5000
        CHECKED_WORD_LEN = 12
        FACTOR = 20

        words=[]
        ham_string = ''

        '''this part concentrares all hams to one
        huge string, than creates list of all specific
        words in hams, and dictionary of those words
        with frequency of their appearance'''
        tc = TrainingCorpus(path)
        for fname,body in tc.hams():
            ham_string += body
            body = body.translate(str.maketrans('.',' '))
            words = words + (body.lower().split(' '))

        counter_ham = Counter(words)
        ham_words_dict = dict(counter_ham.most_common(MOST_COMMON_H))
        ham_words_list = list(ham_words_dict.keys())

        words=[]

        '''same for spams except the huge string part'''
        for fname,body in tc.spams():
            body = body.translate(str.maketrans('.',' '))
            words = words + (body.lower().split(' '))

        couter_spam = Counter(words)
        spam_words_dict=dict(couter_spam.most_common(MOST_COMMON_S))
        spam_words_list=list(spam_words_dict.keys())


        '''this part creates the bad_words list'''
        for word in spam_words_list:

            if word not in ham_string:
                self.bad_words.append(word)

            elif len(word)>CHECKED_WORD_LEN and word in ham_words_list and word in spam_words_list:
                if (spam_words_dict[word] > (ham_words_dict[word] * FACTOR)):
                    self.bad_words.append(word)

        pass


    def test(self,path):
        '''This function determines which emails
        contain at least one "bad word", those are
        marked as spams, the others as ham, then the
        classification is written to !prediction.txt file.'''

        c = Corpus(path)
        for fname, body in c.emails():
            SPAM=False
            body = body.translate(str.maketrans('.', ' '))
            for word in self.bad_words:
                if word in body:
                    SPAM=True
                    break
            if SPAM:
                self.final_dict[fname]='SPAM'

            else:
                self.final_dict[fname]='OK'


        names = os.listdir(path)
        path0 = os.getcwd()
        os.chdir(path)
        fd = open('!prediction.txt', 'w', encoding='utf-8')
        for name in names:
            if name[0] !='!':
                fd.write(name + ' ' + self.final_dict[name] + '\n')
        fd.close()
        os.chdir(path0)
        pass



if __name__=="__main__":
    filter = MyFilter()
    filter.train('/home/palo/Documents/python/spam_filter/2')
    filter.test('/home/palo/Documents/python/spam_filter/1')
    #print(quality.compute_quality_for_corpus('/home/palo/Documents/python/spam_filter/1'))
