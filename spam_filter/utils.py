import os
def read_classification_from_file(file):
    with open(file,'r',encoding='utf-8') as f:
        lines=f.readlines()
    dict={}
    for line in lines:
        line=line.replace("\n","")
        line_list=line.split(' ')
        dict[line_list[0]]=line_list[1]

    return dict

if __name__=="__main__":
    print(read_classification_from_file('1/!truth.txt'))
