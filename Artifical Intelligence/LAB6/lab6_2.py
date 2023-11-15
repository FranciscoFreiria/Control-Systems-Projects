# -*- coding: utf-8 -*-
"""
Created on Fri Nov 15 14:45:34 2019

@author: franc
"""

import numpy as np
from sklearn.metrics import confusion_matrix
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score
import sklearn.naive_bayes as bayes
import sklearn.metrics as pred



xtrain1 = np.load('dataset2_xtrain.npy')
ytrain1 = np.load('dataset2_ytrain.npy')
xtest1 = np.load('dataset2_xtest.npy')
ytest1 = np.load('dataset2_ytest.npy')
#print(xtrain1.shape)
#print(ytrain1.shape)
#print(xtest1.shape)
#print(ytest1.shape)

# =============================================================================
bayess = bayes.MultinomialNB(fit_prior= False)
bayess.fit(xtrain1, ytrain1)
Len_tables = bayess.predict(xtrain1)
#print(pred.accuracy_score(Len_tables, ytrain1))
  
ypred = bayess.predict(xtest1)
#print("ypred:")
#print(ypred)
print("Accuracy Score:")
print(pred.accuracy_score(ytest1, ypred))
 
prob = bayess.predict_proba(xtest1)
prob1 = np.sort(prob, axis = 1)
#print(prob1)
aq = confusion_matrix(ytest1,ypred)
precision = aq[0,0]/(aq[0,0]+aq[0,1]);
print(precision) 
ac = (aq[0,0]+aq[1,1])/(aq[0,0]+aq[0,1]+aq[1,0]+aq[1,1]);
print(ac)
sens = (aq[0,0])/(aq[0,0]+aq[1,0]);
print(sens)
spec = aq[1,1]/(aq[0,1]+aq[1,1])
print(spec)
print('#################')


#################################

ytest1 = np.reshape(ytest1,(99,))
ytrain1 = np.reshape(ytrain1,(231,))
#pvalues = [1,2,3,4,5,6,7,8,9,10]
pvalues = [4]
for i in range (len(pvalues)):

    classifier = SVC(kernel = 'poly', max_iter = 100000, gamma='auto',degree = pvalues[i])   
    classifier.fit(xtrain1,ytrain1) 
    y_pred = classifier.predict(xtest1)
    score = accuracy_score(ytest1,y_pred)
    erro = (1 - score)
    print('pvalues')
    print(pvalues[i])
    print('erro')
    print(erro)
    print('tamanho')
    print(classifier.n_support_)
    aq1 = confusion_matrix(ytest1,y_pred)
    precision1 = aq1[0,0]/(aq1[0,0]+aq1[0,1]);
    print(precision1) 
    ac1 = (aq1[0,0]+aq1[1,1])/(aq1[0,0]+aq1[0,1]+aq1[1,0]+aq1[1,1]);
    print(ac1)
    sens1 = (aq1[0,0])/(aq1[0,0]+aq1[1,0]);
    print(sens1)
    spec1 = aq1[1,1]/(aq1[0,1]+aq1[1,1])
    print(spec1)
    
    