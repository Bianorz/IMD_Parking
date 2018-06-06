
import cv2
import scipy.io
import numpy as np
import matplotlib.pyplot as plt
from numpy import genfromtxt
#====================================
# 0 - Vaga livre
# 1 - Vaga ocupada
#===================================
# TUTORIAL FROM https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_ml/py_knn/py_knn_understanding/py_knn_understanding.html

trainData = genfromtxt('trainData.csv', delimiter=' ')
responses = genfromtxt('responses.csv', delimiter=' ')
testData = genfromtxt('testData.csv', delimiter=' ')

trainData = np.float32(trainData)
responses = np.float32(responses)
testData = np.float32(testData)

# Plotar dados das vagas livres
livre = trainData[responses.ravel()==0]
plt.scatter(livre[:,0],livre[:,1],80,'b','s')
# Plotar dados das vagas ocupadas
ocupado = trainData[responses.ravel()==1]
plt.scatter(ocupado[:,0],ocupado[:,1],80,'r','^')
# Plotar dados das vagas a serem decididas
a_ser_decidido = testData
plt.scatter(a_ser_decidido[:,0],a_ser_decidido[:,1],80,'g','o')
# numero de vizinhos
num_vizinhos = 3
color = 0
contour = 1
#########################CLASSIFICATION BASED ON COLOR#########################
knn_color = cv2.ml.KNearest_create()
knn_color.train(np.float32([trainData[:,color]]).T,cv2.ml.ROW_SAMPLE,responses)
ret, results, neighbours, dist = knn_color.findNearest(np.float32([testData[:,color]]).T, num_vizinhos)
print "Results based on color"
for x in range(0, 5):
    if results[x] == 1:
        print str(x+1)+".png: Occupied Spot"
    else:
        print str(x+1)+".png: Free Spot"
#########################CLEAN VARIABLES#######################################
print "\n"
ret = 0
neighbours = 0
dist = 0
#########################CLASSIFICATION BASED ON CONTOUR#########################
knn_contour = cv2.ml.KNearest_create()
knn_contour.train(np.float32([trainData[:,contour]]).T,cv2.ml.ROW_SAMPLE,responses)
ret, results2, neighbours, dist = knn_contour.findNearest(np.float32([testData[:,contour]]).T, num_vizinhos)
print "Results based on contour"
for x in range(0, 5):
    if results2[x] == 1:
        print str(x+1)+".png: Occupied Spot"
    else:
        print str(x+1)+".png: Free Spot"
#########################CLEAN VARIABLES#######################################
print "\n"
ret = 0
neighbours = 0
dist = 0
#########################CLASSIFICATION BASED ON COLOR AND CONTOUR#########################
knn_color_contour = cv2.ml.KNearest_create()
knn_color_contour.train(trainData,cv2.ml.ROW_SAMPLE,responses)
ret, results3, neighbours, dist = knn_color_contour.findNearest(testData, num_vizinhos)
print "Results based on color and contour"
for x in range(0, 5):
    if results3[x] == 1:
        print str(x+1)+".png: Occupied Spot"
    else:
        print str(x+1)+".png: Free Spot"



