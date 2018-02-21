from confmat import BinaryConfusionMatrix
from utils import read_classification_from_file
import os

def quality_score(tp, tn, fp, fn):
    print('tp=',tp,'\n','tn=',tn,'\n','fp=',fp,'\n','fn=',fn,'\n')
    return (tp+tn)/(tp+tn+10*fp+fn)

def compute_quality_for_corpus(corpus_dir):
    path=os.getcwd()
    os.chdir(corpus_dir)
    truth_dict= read_classification_from_file('!truth.txt')
    pred_dict = read_classification_from_file('!prediction.txt')
    os.chdir(path)
    cm = BinaryConfusionMatrix(pos_tag='SPAM', neg_tag='OK')
    cm.compute_from_dicts(truth_dict,pred_dict)
    return quality_score(**cm.as_dict())
