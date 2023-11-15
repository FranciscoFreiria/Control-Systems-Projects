# -*- coding: utf-8 -*-
"""
Created on Tue Nov 26 21:03:59 2019

@author: franc
"""

import numpy as np
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score
#from plt import scatter




def plot_contours(clf, points,labels):
    """Plot the decision boundaries for a classifier.

    Parameters
    ----------
    clf: a SVM classifier
    params: dictionary of params to pass to contourf, optional
    """
    
    def make_meshgrid(x, y,h=0.02):
        """Create a mesh of points to plot in
    
        Parameters
        ----------
        x: data to base x-axis meshgrid on
        y: data to base y-axis meshgrid on
        h: stepsize for meshgrid, optional
    
        Returns
        -------
        xx, yy : ndarray
        """
        x_min, x_max = x.min() - 1, x.max() + 1
        y_min, y_max = y.min() - 1, y.max() + 1
        xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                             np.arange(y_min, y_max, h))
        return xx, yy
    
    
    X0, X1 = points[:, 0], points[:, 1]

    
    xx, yy = make_meshgrid(X0, X1,0.5)


    Z = clf.predict(np.c_[xx.ravel(), yy.ravel()])
    Z = Z.reshape(xx.shape)
    
    
    #VISUALIZATION OF DECISION
    fig2 = plt.figure(1,figsize=(5,5))
    ax = fig2.add_subplot(1,1,1)
    out = plt.contourf(xx, yy, Z, cmap=plt.cm.coolwarm, alpha=0.8)
    
    ax.scatter(X0, X1, c=labels, cmap=plt.cm.coolwarm, s=80, edgecolors='k')
    
    ax.scatter(X0[clf.support_],X1[clf.support_],c=labels[clf.support_], cmap=plt.cm.coolwarm, s=80, edgecolors='w')
    ax.set_xlim(xx.min(), xx.max())
    ax.set_ylim(yy.min(), yy.max())
    
    plt.show()
    
    return out




xspiral = np.load('spiral_X.npy')
yspiral = np.load('spiral_Y.npy')
xspiral_shape = xspiral.shape
yspiral_shape = yspiral.shape
#print(xspiral_shape)
#print(yspiral_shape)


pvalues = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,20]
for i in range (len(pvalues)):

    classifier = SVC(kernel = 'poly', max_iter = 100000, gamma='auto',degree = pvalues[i])   
    classifier.fit(xspiral,yspiral) 
    y_pred = classifier.predict(xspiral)
    score = accuracy_score(yspiral,y_pred)
    erro = (1 - score)
    #plot_contours(classifier, xspiral, yspiral)
    #print('pvalues')
    #print(pvalues[i])
    #print('erro')
    #print(erro)
    #print('tamanho')
    #print(classifier.n_support_)
    
    ####MELHOR POLINOMIO É DE ORDEM 10
    
values = [0.001,0.01,0.1,0.4,0.5,0.7,0.8,0.9,1,1.2,1.5,2]
for i in range (len(values)):

    classifier = SVC(kernel = 'rbf', max_iter = 100000, gamma=values[i])   
    classifier.fit(xspiral,yspiral) 
    y_pred1 = classifier.predict(xspiral)
    score1 = accuracy_score(yspiral,y_pred1)
    erro = (1 - score1)
    #plot_contours(classifier, xspiral, yspiral)
    #print('values')
    #print(values[i])
    #print('erro')
    #print(erro)
    #print('tamanho')
    #print(classifier.n_support_)
    
####2#####    
x33spiral = np.load('chess33_x.npy')
y33spiral = np.load('chess33_y.npy')

for i in range (len(values)):

    classifier = SVC(C=100000,kernel = 'rbf', max_iter = 100000, gamma=values[i])   
    classifier.fit(x33spiral,y33spiral) 
    y_pred2 = classifier.predict(x33spiral)
    score2 = accuracy_score(y33spiral,y_pred2)
    erro = (1 - score2)
    #plot_contours(classifier, x33spiral, y33spiral)
    #print('values')
    #print(values[i])
    #print('erro')
    #print(erro)
    #print('tamanho')
    #print(classifier.n_support_)

######3#########
    
x33spiral_n = np.load('chess33n_x.npy')
y33spiral_n = np.load('chess33n_y.npy')
cvalues = [100000000,10000000,1000000,100000,10000,1000,100,10,1,0.1,0.01,0.001,0.0001,0.00001,0.000001]
for i in range (len(cvalues)):    
    classifier = SVC(C=cvalues[i],kernel = 'rbf', max_iter = 100000, gamma=0.001)   
    classifier.fit(x33spiral_n,y33spiral_n) 
    y_pred3 = classifier.predict(x33spiral_n)
    score3 = accuracy_score(y33spiral_n,y_pred3)
    erro = (1 - score3) 
    #plot_contours(classifier, x33spiral_n, y33spiral_n)
    #print('values')
    #print(cvalues[i])
    #print('erro')
    #print(erro)
    #print('tamanho')
    #print(classifier.n_support_)
    #print('######################')