#---------------------------------------------------------------------------
# Sam Opdahl
# Comp 324 - Prim's Algorithm
#
# This program contains an implementation of Prim's Algorithm.
# The user can input a graph by specifying the number of vertices of the graph
# 	and inputting the adjacency matrix row by row.
# The program will output the Minimum Spanning Tree of the graph and other details.
#
# developed using python version 2.7
#---------------------------------------------------------------------------

# -- Imports --
from sys import stdout

# -- Globals --
INF_SYMBOL = u"\u221E"
INF = "inf"
TABLE_ID = "vertex"
ID_WIDTH = len(TABLE_ID)

COLUMN_SEPARATOR = "|"
ROW_SEPARATOR = "-"
CORNER = "+"

# -- Functions -- 

#Pre: 'W' is an adjacency matrix (2D array) which holds the weight values between pairs of vertices
#		'n' is the number of vertices in 'W'. 'weight' is a list with weight[0] = 0 (used for pass by reference)
#Post: The list of edges (pairs of vertices) of the minimum spanning tree has been returned. 
#		weight[0] holds the weight of the MST.
def prim(n, W, weight):
	#create the empty list which will hold the edges of the MST
	F = []
	#hold the current vertex with the smallest distance
	vnear = None

	#hold the list of vertices nearest to v[i]
	nearest = []
	#weight between v[i] and the vertex indexed by nearest[i]
	distance = []
	for i in range(n):
		#initialize vertex 1 as the nearest in Y (subset of vertices with vertex 1 an the initial vertex)
		nearest.append(0)
		#initialize the distance as the weight from Y on the edge to vertex 1
		distance.append(W[0][i])

	for x in range(n - 1):
		#hold the minimum distance for the vertex in vnear
		minimum = float("inf")
		#find the current smallest available weight and vertex
		for i in range(1, n):
			if (0 <= distance[i] and distance[i] < minimum):
				minimum = distance[i]
				vnear = i
		#add the smallest weight edge which is the edge between the minimum vertex,
		#and the other vertex nearest to vnear
		F.append([vnear + 1, nearest[vnear] + 1])
		#update the weight of the MST with the newly added edge's weight
		weight[0] += distance[vnear]
		#mark the edge added to the MST as no longer available
		distance[vnear] = -1
		for i in range(1, n):
			if (W[i][vnear] < distance[i]):
				#update any vertex that is closer to vnear than the previous closest vertex
				#also update the nearest distance to vnear since it has less weight
				distance[i] = W[i][vnear]
				nearest[i] = vnear
	return F

#Pre: 'colWidthList' is a list of values that hold the length of the widest string
#		in their respective columns
#Post: a string has been returned which separates columns based on the values in 'colWidthList'
def getRowSeparator(colWidthList):
	separator = ROW_SEPARATOR * ID_WIDTH + CORNER
	for i in range(len(colWidthList)):
		separator += ROW_SEPARATOR * colWidthList[i] + CORNER
	return separator

#Pre: N/A
#Post: The program has been started
def main():
	try:
		vertexCount = int(raw_input("Enter the number of vertices in the graph: "))
	except ValueError:
		print "\nError: Input must be a valid integer. Program will now exit."
		exit()
	print ""

	#start out with an empty adj. matrix where the rows will be inserted when given by the user
	adjacencyMatrix = []
	#colWidthList will hold the width of each individual column in the adj. matrix.
	#this will be used for formatting the table when printing it out
	#set default values by assuming the table header will be the widest value, which will be comapred against later on.
	colWidthList = [i for i in range(1, vertexCount + 1)]
	print "Enter each row of the adjacency matrix separated by a space (\" \")"
	print "For values of infinity, type \"{0}\"\n".format(INF)
	for i in range(vertexCount):
		row = raw_input("Enter row {0} of the adjacency matrix: ".format(i + 1)).split()
		#if the number of elements in the row isn't equal to the number of verices, the algorithm will not work
		if (len(row) != vertexCount):
			print "Error: # of vertices = {0}, # of row elements = {1}".format(vertexCount, len(row))
			print "Program will now exit."
			exit()

		#parse the list of elements for int and infinity
		#float("inf") is a quick way to represent infinity in python
		for j in range(len(row)):
			try:
				v = float(INF) if row[j].lower() == INF else int(row[j])
			except ValueError:
				print "Error parsing value \"{0}\" in row {1}, program will now exit.".format(row[j], i + 1)
				exit()
			#excluding the infinity value (since it will print in just 1 character),
			#check if the new value in column j is bigger than the previous biggest value.
			#note: will only make a difference for formatting when the order of magnitude of the value increases
			if (v != float(INF) and v > colWidthList[j]):
				colWidthList[j] = v
			#replace string value in the row with the new int/infinity value
			row[j] = v
		adjacencyMatrix.append(row)
	#convert each numerical value in colWidthList to values which will hold the string length of the previous values
	colWidthList = [len(str(w)) for w in colWidthList]

	#get string for separating rows in the table
	rowSeparator = getRowSeparator(colWidthList)
	#start printing the adj. matrix
	print "\n\nAdjacency Matrix:\n"
	print TABLE_ID,
	#print the table header
	for i in range(1, vertexCount + 1):
		#print the column separator and vertex id. 
		#Right justify the id based on the widest value in the column.
		#stdout.write() is used to prevent unwanted spaces/newlines from being printed
		stdout.write(COLUMN_SEPARATOR + str(i).rjust(colWidthList[i-1]))
	print COLUMN_SEPARATOR + "\n" + rowSeparator

	for i in range(len(adjacencyMatrix)):
		#print the vertex id for the current row
		#for simplicity, assume the width of the table id is the widest value
		#(unlikely more than 6 figures will be used)
		stdout.write(str(i+1).rjust(ID_WIDTH))
		for j in range(len(adjacencyMatrix[i])):
			stdout.write(COLUMN_SEPARATOR + str(adjacencyMatrix[i][j]).replace(INF, INF_SYMBOL).rjust(colWidthList[j]))
		print COLUMN_SEPARATOR + "\n" + rowSeparator

	#weight[0] will hold the weight of the MST after prim() has been called
	weight = [0]
	#run Prim's Algorithm and sort the edge list based on the the first vertex in each edge
	print "\nList of edges in the minimum spanning tree:"
	print "" + str(sorted(prim(vertexCount, adjacencyMatrix, weight), key=lambda edge: edge[0]))
	print "\nWeight of the minimum spanning tree: {0}".format(weight[0])

# -- Program entry point --
main()