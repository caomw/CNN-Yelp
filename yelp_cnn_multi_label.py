from __future__ import absolute_import
from __future__ import print_function
import numpy as np
np.random.seed(1337) # for reproducibility

#import random

import sys

from keras.preprocessing import sequence
from keras.optimizers import RMSprop, SGD, Adam
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.layers.embeddings import Embedding
from keras.layers.convolutional import Convolution2D, MaxPooling2D
from keras.utils import np_utils
# from keras.datasets import imdb

'''
    Run on GPU: THEANO_FLAGS=mode=FAST_RUN,device=gpu,floatX=float32 python yelp_cnn.py
'''



def load_data(filename):
    Dimension = 0
    N = 0
    SentenceL = 0
    data_file = open(filename, "r")
    Dimension, SentenceL = data_file.readline().split()
    Dimension = int(Dimension)
    SentenceL = int(SentenceL)
    X = []
    Y = []
    while True:
        line = data_file.readline()
        if not line:
            break
        Label = int(line.split()[0])
        N += 1
        Vectors = []
        for i in range(SentenceL):
            vector = data_file.readline().split()
            vector = [float(x) for x in vector]
            Vectors.append(vector)
        X.append(Vectors)
        Y.append(Label)
    data_file.close()
    return X, Y, N, Dimension, SentenceL
        

if len(sys.argv) < 2:
    print("missing parameters\n")
    exit()

print("Loading data...")


dataX, dataY, N, Dimension, SentenceL = load_data(sys.argv[1])

print("OK!")

#print len(dataX)
#print len(dataX[0])
#print len(dataX[0][0])
#print dataY


#ratio = 0.8

#pos = int(len(dataX) * ratio)

#print("Spliting training data and testing data...")

#X_train = np.array(dataX[:pos])
#y_train = np.array(dataY[:pos])
#X_test = np.array(dataX[pos:])
#y_test = np.array(dataY[pos:])

#print("OK!")

print("Reshaping data...")

#X_train = X_train.reshape(X_train.shape[0], 1, X_train.shape[1], X_train.shape[2])
#X_test = X_test.reshape(X_test.shape[0], 1, X_test.shape[1], X_test.shape[2])

dataX = np.array(dataX)
dataX = dataX.reshape(dataX.shape[0], 1, dataX.shape[1], dataX.shape[2])

dataY = np.array(dataY)

print("OK!")

print("X shape:", dataX.shape)
print("Y shape:", dataY.shape)

print("Preprocessing labels...")
nb_classes = 4
#y_train = np_utils.to_categorical(y_train, nb_classes)
#y_test = np_utils.to_categorical(y_test, nb_classes)
dataY = np_utils.to_categorical(dataY, nb_classes)
print("OK!")

#print(len(X_train), 'train sequences')
#print(len(X_test), 'test sequences')

#print("Pad sequences (samples x time)")
#X_train = sequence.pad_sequences(X_train, sentence_len=150)
#X_test = sequence.pad_sequences(X_test, sentence_len=150)
#print('X_train shape:', X_train.shape)
#print('X_test shape:', X_test.shape)


#print('Y_train shape:', y_train.shape)
#print('Y_test shape:', y_test.shape)
#print('Y_train:', y_train)
#print('Y_test:', y_test)

print('Build model...')
model = Sequential()

# we start off with an efficient embedding layer which maps
# our vocab indices into word2vec_dim dimensions
#model.add(Embedding(max_features, word2vec_dim))
#model.add(Dropout(0.25))

# set parameters:
sentence_len = 150
word2vec_dim = 50


filterA_num = 128
filterA_len = 3

filterB_num = 128
filterB_len = 5


hiddenA_dim = 64
#hiddenB_dim = 12
hiddenB_dim = 8

batch_size = 32
nb_epoch = 5


model.add(Convolution2D(nb_filter = filterA_num, stack_size = 1, nb_row = filterA_len, nb_col = word2vec_dim, border_mode = "valid", activation = "relu"))

maxA_ps = 2

model.add(MaxPooling2D(poolsize = (maxA_ps, 1)))

#model.add(Dropout(0.25))

model.add(Convolution2D(nb_filter = filterB_num, stack_size = filterA_num, nb_row = filterB_len, nb_col = 1, border_mode = "valid", activation = "relu"))

outA_row_dim = (sentence_len - filterA_len + 1) / maxA_ps

model.add(MaxPooling2D(poolsize = (outA_row_dim - filterB_len + 1, 1)))

#model.add(Dropout(0.25))

model.add(Flatten())


model.add(Dense(filterB_num, hiddenA_dim))
model.add(Dropout(0.5))
model.add(Activation('relu'))

model.add(Dense(hiddenA_dim, hiddenB_dim))
#model.add(Dense(filterB_num, hiddenB_dim))
model.add(Dropout(0.25))
model.add(Activation('relu'))

# We project onto a single unit output layer, and squash it with a sigmoid:

# categorical
model.add(Dense(hiddenB_dim, nb_classes))
model.add(Activation('sigmoid'))

#model.compile(loss='categorical_crossentropy', optimizer='rmsprop', class_mode="categorical")
rms = RMSprop()
sgd = SGD(lr = 0.01, decay=1e-6, momentum=0.9, nesterov=True)



model.compile(loss='categorical_crossentropy', optimizer=rms, class_mode="categorical")
#model.compile(loss='binary_crossentropy', optimizer="adam", class_mode="binary")


#model.fit(X_train, y_train, batch_size=batch_size, nb_epoch=nb_epoch, show_accuracy=True, validation_data=(X_test, y_test))
model.fit(dataX, dataY, batch_size=batch_size, nb_epoch=nb_epoch, show_accuracy=True)


print("loading testing data...")
print(sys.argv[2])

testX, testY, testN, testDimension, testSentenceL = load_data(sys.argv[2])

print("# reviews = ", testN)

print("OK!")

print("Reshaping data...")
testX = np.array(testX)
testY = np.array(testY)
testX = testX.reshape(testX.shape[0], 1, testX.shape[1], testX.shape[2])
print("OK!")


#score = model.evaluate(testX, testY, show_accuracy = True, verbose = 1)

#print("Test Loss = ", score[0])
#print("Test Accuracy = ", score[1])

result = model.predict_classes(testX, verbose = 1)

result = result.tolist()

counter = 0
for i in range(len(result)):
    if result[i] == testY[i]:
        counter += 1
ratio = float(counter) / len(result)
print("my accuracy = ", ratio)
