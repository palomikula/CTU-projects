class BinaryConfusionMatrix():
    def __init__(self, pos_tag, neg_tag):
        self.P=pos_tag
        self.N=neg_tag
        self.dict={'tp':0,'tn':0,'fp':0,'fn':0}
        pass
    def as_dict(self):
        return self.dict

    def update(self, truth, pred):
        if(truth == pred == self.P):
            self.dict['tp'] += 1
        elif(truth == pred == self.N):
            self.dict['tn'] += 1
        elif(truth == self.N and pred == self.P):
            self.dict['fp'] += 1
        elif(truth == self.P and pred == self.N):
            self.dict['fn'] += 1
        else:
            raise ValueError
        pass

    def compute_from_dicts(self, truth_dict, pred_dict):
        for key in truth_dict:
            self.update(truth_dict[key],pred_dict[key])
        pass
if __name__=="__main__":
    truth_dict = {'em1': 'SPAM', 'em2': 'SPAM', 'em3': 'OK', 'em4': 'OK'}
    pred_dict = {'em1': 'SPAM', 'em2': 'OK', 'em3': 'OK', 'em4': 'SPAM'}
    cm2 = BinaryConfusionMatrix(pos_tag='SPAM', neg_tag='OK')
    cm2.compute_from_dicts(truth_dict, pred_dict)
    print(cm2.as_dict())