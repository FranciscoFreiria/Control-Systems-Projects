# -*- coding: utf-8 -*-
"""
Created on Mon Nov  4 03:17:58 2019

@author: franc
"""
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf
import keras as keras
from keras.utils import to_categorical
from sklearn.model_selection import train_test_split
from keras.models import Sequential
from keras.layers import Dense, Conv2D, Flatten, MaxPooling2D
#from keras import optimizers
from sklearn.metrics import confusion_matrix

######1.1#######
###1###
test_data = np.load('mnist_test_data.npy')
test_data_shape=test_data.shape
print(test_data_shape)

test_labels = np.load('mnist_test_labels.npy')
test_labels_shape=test_labels.shape
print(test_labels_shape)

train_data = np.load('mnist_train_data.npy')
train_data_shape = train_data.shape
print(train_data_shape)

train_labels = np.load('mnist_train_labels.npy')
train_labels_shape=train_labels.shape
print(train_labels_shape)
###2####
a=train_data[0].shape
print(a)
#plt.imshow(test_data[2,:,:,0])
###3###
train_data = train_data.astype("float32") / 255
test_data = test_data.astype("float32") / 255
###4###
train_labels = to_categorical(train_labels)
test_labels = to_categorical(test_labels)
print(train_labels)
print(test_labels)
###5###
train_data_validation, train_data_train, train_labels_validation, train_labels_train = train_test_split (train_data, train_labels, test_size=0.7)
print(train_data_validation.shape)
print(train_data_train.shape)
print(train_labels_validation)
print(train_labels_train)

######1.2#######
MLP_Model = Sequential()
MLP_Model.add(Flatten(input_shape=(28,28,1)))
MLP_Model.add(Dense(64, activation = 'relu'))
MLP_Model.add(Dense(128, activation = 'relu'))
MLP_Model.add(Dense(10, activation = 'softmax'))
print(MLP_Model.summary())
callback = tf.keras.callbacks.EarlyStopping(patience=15, restore_best_weights=True)
opt=keras.optimizers.Adam(learning_rate=0.01, clipnorm=1)
MLP_Model.compile(opt, loss='categorical_crossentropy')
History=MLP_Model.fit(x=train_data_train, y=train_labels_train, batch_size=None, callbacks=[callback], epochs=400, validation_steps=400, validation_data=(train_data_validation, train_labels_validation), steps_per_epoch=7)
plt.plot(History.history['loss'])
plt.plot(History.history['val_loss'])
plt.show()

pred = MLP_Model.predict(test_data, batch_size=None, verbose=0, steps=None, callbacks=[callback], max_queue_size=10, workers=1, use_multiprocessing=False)
aq = confusion_matrix(np.argmax(test_labels,1),np.argmax(pred,1)) 

###without Earlystopping###
MLP_Model1 = Sequential()
MLP_Model1.add(Flatten(input_shape=(28,28,1)))
MLP_Model1.add(Dense(64, activation = 'relu'))
MLP_Model1.add(Dense(128, activation = 'relu'))
MLP_Model1.add(Dense(10, activation = 'softmax'))
print(MLP_Model1.summary())
MLP_Model1.compile(opt, loss='categorical_crossentropy')
History1=MLP_Model1.fit(x=train_data_train, y=train_labels_train, batch_size=None, epochs=400, validation_steps=400, validation_data=(train_data_validation, train_labels_validation), steps_per_epoch=7)
plt.plot(History1.history['loss'])
plt.plot(History1.history['val_loss'])
plt.show()

pred1 = MLP_Model1.predict(test_data, batch_size=None, verbose=0, steps=None, max_queue_size=10, workers=1, use_multiprocessing=False)
aq1 = confusion_matrix(np.argmax(test_labels,1),np.argmax(pred1,1))

####1.3####
CNN_Model = Sequential()
CNN_Model.add(Conv2D(16,kernel_size=(3,3),input_shape=(28,28,1)))
CNN_Model.add(MaxPooling2D(pool_size=(2,2)))
CNN_Model.add(Conv2D(32,kernel_size=(3,3)))
CNN_Model.add(MaxPooling2D(pool_size=(2,2)))
CNN_Model.add(Flatten())
CNN_Model.add(Dense(64, activation = 'relu'))
CNN_Model.add(Dense(10, activation = 'softmax'))
print(CNN_Model.summary())
callback1 = tf.keras.callbacks.EarlyStopping(patience=15, restore_best_weights=True)
opt=keras.optimizers.Adam(learning_rate=0.01, clipnorm=1)
CNN_Model.compile(opt, loss='categorical_crossentropy')
History2=CNN_Model.fit(x=train_data_train, y=train_labels_train, batch_size=None, callbacks=[callback1], epochs=400, validation_steps=400, validation_data=(train_data_validation, train_labels_validation), steps_per_epoch=7)
plt.plot(History2.history['loss'])
plt.plot(History2.history['val_loss'])
plt.show()

pred2 = CNN_Model.predict(test_data, batch_size=None, verbose=0, steps=None, callbacks=[callback1], max_queue_size=10, workers=1, use_multiprocessing=False)
aq2 = confusion_matrix(np.argmax(test_labels,1),np.argmax(pred2,1))

# =============================================================================
# 1.4.
# 
# 
# 1.O Early Stopping é uma forma de regularização usada para evitar ajustes excessivos ao treinar um “learner” com um método iterativo . Tais métodos atualizam o “learner”, de modo a ajustá-lo aos dados de treinamento a cada iteração. Isso melhora o desempenho do “learner” em dados fora do conjunto de treinamento, ate um certo ponto. Após esse ponto, no entanto, melhorar o ajuste do “learner” aos dados de treinamento leva a um maior erro de generalização . As regras do Early Stopping fornecem orientação sobre quantas iterações podem ser executadas antes que o aluno comece a se ajustar demais. 
# 
# 2.Com os resultados obtidos é possível concluir que a “evolution loss” é maior e decresce menos no caso do uso do Early Stopping. É também possível observar que o tempo de execução é muito maior no caso em que o Early Stopping não é usado, precisando de passar por todos os 400 passos enquanto que no caso com Early Stopping apenas passa por 18/19 passos.
# 
# 
# 
# 3.A partir dos resultados obtidos é possível concluir que o numero de passos nos dois casos é semelhante mas o tempo de execução de cada passo é muito maior no caso do CNN. Quanto á performance é possível concluir que no caso do MLP esta é melhor.
# 
# =============================================================================
