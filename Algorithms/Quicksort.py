#---------------------------------------------------------------------------
# Sam Opdahl
# Comp 324 - Quicksort
#
# This program is used to test the Quick Sort algorithm.
# It can be used to find the average number of comparisons for arrays of a certain size.
#
# developed using python version 2.7
#---------------------------------------------------------------------------

# -- Imports --
from random import randrange

# -- Global Variables --
exitString = "exit"

# -- Functions -- 

#Wrapper function for quickSort which automatically sets up pre-conditions for quickSort
#and returns the number of comparisons for the sort.
#
#Pre: 'S' is the array that will be be sorted
#Post: the number of comparisons used to sort 'S' has been returned
def performQuickSort(S):
	#use a list to hold the number of comparisons so it can be passed by reference
	numberOfComparisons = [0]
	quickSort(S, 0, len(S) - 1, numberOfComparisons)
	return numberOfComparisons[0]

#Pre: 'S' is the array that will be sorted, 'low' is the start index, 'high' is the last index,
#	'comparisons' is a list where comparisons[0] = 0 (for the inital call). this is used to count the comparisons of the sort so it can be passed by reference
#Post: 'S' has been sorted, comparisons[0] holds the number of comparisons the sort needed
def quickSort(S, low, high, comparisons):
	#Check if there is anything to sort
	if (high > low):
		#partition the list from low to high and find the pivot index
		pivot = partition(S, low, high, comparisons)
		#run quicksort on the side of the array to the left of the pivot
		quickSort(S, low, pivot - 1, comparisons)
		#run quicksort on the side of the array to the right of the pivot
		quickSort(S, pivot + 1, high, comparisons)

#Pre: 'S' is the array that will be sorted, 'low' is the start index, 'high' is the end index,
#	'comparisons' is a list used to count the comparisons of the sort so it can be passed by reference
#Post: 'S' has been partitioned. 'j', which is the new pivot, has been returned. 
#	the comparisons used for this partition has been added to comparisons[0]
def partition(S, low, high, comparisons):
	#set the pivot as the inital element in the list
	pivotItem = S[low]
	#will hold the last index of the smallest item compared to the pivot after comparisons are performed
	j = low
	#loop through remaining elements from low to high and compare
	for i in range(low + 1, high + 1):
		#comparison will be performed, increment counter
		comparisons[0] += 1
		#swap elements that are smaller than the pivot to the front of the list
		if (S[i] < pivotItem):
			j += 1
			swap(S, i, j)
	#swap pivot element with the final smallest element,
	#now all elements smaller then the pivot are to the left of it 
	swap(S, low, j)
	return j

#Pre: 'S' is the array the swap will be performed on. 'firstIndex' and 'secondIndex' are indices in 'S' which will be swapped
#Post: S[firstIndex] and S[secondIndex] have been swapped 
def swap(S, firstIndex, secondIndex):
	S[firstIndex], S[secondIndex] = S[secondIndex], S[firstIndex]

#Pre: 'S' in the list to be unsorted
#Post: 'S' has been unsorted (randomized)
def unsort(S):
	for i in range(len(S)):
		#swap the current index in the loop with another random index
		swap(S, i, randrange(len(S)))

#helper function for retrieving input and checking if we should exit
#
#Pre: 's' is a string that will be printed to the user
#Post: if the user typed 'exitString', the program has closed, otherwise input has been returned
#	with removed leading and trailing whitespace
def getInput(s):
	inp = raw_input(s).strip()
	if (inp == exitString):
		exit()
	return inp


# -- Program entry point --

print "\nType '" + exitString + "' to leave the program\n"
while True:
	#retrieve user input and parse for integer values
	n = getInput("Enter the number of arrays n to be processed: ")
	m = getInput("Enter the size m of each array: ")
	try:
		n, m = int(n), int(m)
	except:
		print "Error parsing input, please try again.\n"
		continue

	#generate a list [1, 2, 3, ..., m]
	A = list(range(1, m + 1))
	#hold the total comparisons used for all quicksorts on A
	totalComparisons = 0
	#sort a 'new array' n times
	for i in range(n):
		unsort(A)
		totalComparisons += performQuickSort(A)

	#print out results to the user
	print "\nnumber of arrays processed: {0}".format(n)
	print "size of each array: {0}".format(m)
	#check if 0 arrays were processed to prevent divide by zero exceptions when calculating the average
	print "average number of comparisons: {:0.2f}".format(0 if n == 0 else float(totalComparisons) / float(n))
	print "-" * 50 + "\n"