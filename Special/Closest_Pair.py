import math
import random


        
def Random_Points_Maker(lowest_x,highest_x,lowest_y,highest_y,length,Array):
    for i in range(0,length):
        Array[i].append(random.randint(lowest_x,highest_x))
    for j in range(0,length):
        Array[j].append(random.randint(lowest_x,highest_x))
    
def Show_Points(Array,length):
    for i in range(0,length):
        print("(",Array[i][0],",",Array[i][1],")")
        if i > 0 and (Array[i][0] == Array[i-1][0] and Array[i][1] == Array[i-1][1]):
            print(" \n \t Booooom!!! we have same points")

        
def mergeX(arr, l, m, r):
    n1 = m - l + 1
    n2 = r- m
 
    L = [0] * (n1)
    R = [0] * (n2)
    
    
 
   
    for i in range(0 , n1):
        L[i] = arr[l + i]
 
    for j in range(0 , n2):
        R[j] = arr[m + 1 + j]
 
    
    i = 0     
    j = 0    
    k = l   
 
    while i < n1 and j < n2 :
        if L[i][0] <= R[j][0]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        k += 1
 
    
    while i < n1:
        arr[k] = L[i]
        i += 1
        k += 1
 
    
    while j < n2:
        arr[k] = R[j]
        j += 1
        k += 1

def mergeY(arr, l, m, r):
    n1 = m - l + 1
    n2 = r- m
 
    L = [0] * (n1)
    R = [0] * (n2)
     
   
    for i in range(0 , n1):
        L[i] = arr[l + i]
 
    for j in range(0 , n2):
        R[j] = arr[m + 1 + j]
 
    
    i = 0     
    j = 0    
    k = l   
 
    while i < n1 and j < n2 :
        if L[i][1] <= R[j][1]:
            arr[k] = L[i]
            i += 1
        else:
            arr[k] = R[j]
            j += 1
        k += 1
 
    
    while i < n1:
        arr[k] = L[i]
        i += 1
        k += 1
 
    
    while j < n2:
        arr[k] = R[j]
        j += 1
        k += 1

def mergeSort(arr,l,r,flag):
    if l < r:
        m = (int)((l+(r-1))/2)
        mergeSort(arr, l, m, flag)
        mergeSort(arr, m+1, r, flag)
        if flag == 1 :
            mergeX(arr, l, m, r)
        else:
            mergeY(arr, l, m, r)


def dist(first, second):
    return (float)(math.sqrt(pow((first[0] - second[0]),2) + pow((first[1] - second[1]),2)))


def bruteForce(Array, length):
    minimum = 999999999.999999999
    for i in range(0,length):
        for j in range (i+1,length):
            if dist(Array[i], Array[j]) < minimum :
                minimum = dist(Array[i], Array[j])
    return minimum
          
def minimum(x, y):
    if x < y :
        return x
    else:
        return y

def stripClosest(strip, size, distance):
    minimum = (float)(distance)
    for i in range (0,size):
        j = i + 1 
        while j < size and (strip[j][1] - strip[i][1]) < minimum :
            if (float)(dist(strip[i], strip[j])) < minimum :
                minimum = (float)(dist(strip[i], strip[j]))
            j += 1
    return (float)(minimum)

def closest_algorithm(X_sorted, Y_sorted, length):
    if length <= 3 :
        return bruteForce(X_sorted, length)
    
    mid = (int)(length/2)
    middle_point = X_sorted[mid]

    Left_YsubArray = []
    Right_YsubArray = []

    L=0
    R=0
    for i in range (0,len(Y_sorted)):
        if Y_sorted[i][0] <= middle_point[0] :
            Left_YsubArray.append(Y_sorted[i])
            L += 1
           
        else:
            Right_YsubArray.append(Y_sorted[i])
            R += 1
   
    left_dist = (float)(closest_algorithm(X_sorted, Left_YsubArray, mid))

    right_dis = (float)(closest_algorithm(X_sorted[mid:], Left_YsubArray, length - mid))
    
    minimum_distance = (float)(minimum(left_dist, right_dis))

    strip = []
    j = 0
    for i in range (0,length):
        if (abs(Y_sorted[i][0] - middle_point[0])) < minimum_distance:
            strip.append(Y_sorted[i])
            j += 1
            
    return (float)(minimum(minimum_distance,(float)(stripClosest(strip, j, minimum_distance))))

def closest_pair(Array,length):
    X_sorted = [0] * (length)
    Y_sorted = [0] * (length)
    X_sorted = [x for x in Array]
    Y_sorted = [x for x in Array]
    mergeSort(X_sorted,0,length-1,1)
    mergeSort(Y_sorted,0,length-1,2)
    
    #print("\n X sotred: ")
    #Show_Points(X_sorted,length)
    #print("\n Y sotred: ")
    #Show_Points(Y_sorted,length)
    
    return (float)(closest_algorithm(X_sorted, Y_sorted, length))
    

def main():
    
    
    lowest_x = (int)(eval(input("Plz enter the lowest range for X: ")))
    highest_x = (int)(eval(input("Plz enter the highest range for X: ")))
    lowest_y = (int)(eval(input("Plz enter the lowest range for Y: ")))
    highest_y =(int)(eval(input("Plz enter the highest range for Y: ")))
    length = (int)(eval(input("Plz enter the number of elements: ")))
    Points_List = []  
    for i in range (0,length):
        Points_List.append([])
    Random_Points_Maker(lowest_x,highest_x,lowest_y,highest_y,length,Points_List)
    Show_Points(Points_List,length)
    ans = (float)(closest_pair(Points_List, length))
    print("\n The smallest distance is: ",ans)
    
    
main()


        
 
