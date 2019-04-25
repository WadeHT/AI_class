import pandas
import pickle
from sklearn import preprocessing

trainData = pandas.read_csv('tmpLabel.csv', sep=',',dtype=object)
train = trainData.values[:,0:12]

for index in range(12):
    le = preprocessing.LabelEncoder()
    le.fit(train[:,index].astype(str))
    with open('labelEncode'+str(index), 'wb') as fid:
        pickle.dump(le, fid)
    print(index)