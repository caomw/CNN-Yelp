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
        Y.append(Label - 1)
    data_file.close()
    return X, Y, N, Dimension, SentenceL
        

if len(sys.argv) < 2:
    print("missing parameters\n")
    exit()

print("Loading data...")


dataX, dataY, N, Dimension, SentenceL = load_data(sys.argv[1])


#print len(dataX)
#print len(dataX[0])
#print len(dataX[0][0])
#print dataY


ratio = 0.8

pos = int(len(dataX) * ratio)


X_train = np.array(dataX[:pos])
y_train = np.array(dataY[:pos])
X_test = np.array(dataX[pos:])
y_test = np.array(dataY[pos:])

X_train = X_train.reshape(X_train.shape[0], 1, X_train.shape[1], X_train.shape[2])
X_test = X_test.reshape(X_test.shape[0], 1, X_test.shape[1], X_test.shape[2])

nb_classes = 5
y_train = np_utils.to_categorical(y_train, nb_classes)
y_test = np_utils.to_categorical(y_test, nb_classes)

#print(len(X_train), 'train sequences')
#print(len(X_test), 'test sequences')

#print("Pad sequences (samples x time)")
#X_train = sequence.pad_sequences(X_train, sentence_len=150)
#X_test = sequence.pad_sequences(X_test, sentence_len=150)
print('X_train shape:', X_train.shape)
print('X_test shape:', X_test.shape)


print('Y_train shape:', y_train.shape)
print('Y_test shape:', y_test.shape)
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
nb_filters = 250
filter_length = 5

hiddenA_dim = 100
hiddenB_dim = 20

batch_size = 32
nb_epoch = 10


# we add a Convolution1D, which will learn nb_filters
# word group filters of size filter_length:
model.add(Convolution2D(nb_filter = nb_filters, stack_size = 1, nb_row = filter_length, nb_col = word2vec_dim, border_mode = "valid", activation = "relu"))

# we use standard max pooling (halving the output of the previous layer):
pool_size = sentence_len - filter_length + 1
model.add(MaxPooling2D(poolsize = (sentence_len - filter_length + 1, 1)))

#We flatten the output of the conv layer, so that we can add a vanilla dense layer:
model.add(Flatten())

# Computing the output shape of a conv layer can be tricky;
# for a good tutorial, see: http://cs231n.github.io/convolutional-networks/
output_size = nb_filters
#output_size = 1


# We add a vanilla hidden layer:
model.add(Dense(nb_filters, hiddenA_dim))
#model.add(Dropout(0.25))
model.add(Activation('relu'))

model.add(Dense(hiddenA_dim, hiddenB_dim))
model.add(Activation('relu'))

# We project onto a single unit output layer, and squash it with a sigmoid:
model.add(Dense(hiddenB_dim, nb_classes))
model.add(Activation('sigmoid'))

#model.compile(loss='categorical_crossentropy', optimizer='rmsprop', class_mode="categorical")
#rms = RMSprop()

#sgd = SGD(lr = 0.01, decay=1e-6, momentum=0.9, nesterov=True)
model.compile(loss='categorical_crossentropy', optimizer='adam', class_mode="categorical")
model.fit(X_train, y_train, batch_size=batch_size, nb_epoch=nb_epoch, show_accuracy=True, validation_data=(X_test, y_test))
