#!/usr/bin/env python

import numpy
import csv
from time import time

def getColumn(A,j):
	return [row[i] for row in A]

def setEmptyMatrix(n_rows,n_cols):
	A = [None]*n_rows
	for i in range(n_rows):
		A[i] = [0.0]*n_cols
	return A

def loadMatrix(path):
	""" Returns: 2D Array
	Description: Reads a CSV file and returns a 2D array with the contents of the csv with float type elements.
	"""
	# Leemos el archivo csv y preparamos la matriz de retorno
	handle = csv.reader(open(path,"rb"),delimiter=",")
	raw_matrix = []
	
	for line in handle:
		# Una linea de csv lo transformamos en una fila de la matriz
		row = list(line)
		row = [float(element) for element in row]
		raw_matrix.append(row)
	
	return raw_matrix
	
def matrixLenght(A):
	""" Returns: Integers Tuple
	Description: Calculates the lenght of the column and the row of a matrix.
	"""
	n_rows = len(A)
	n_cols = len(A[0])
	return n_rows, n_cols
	
def prod_punto(A,B,C,n_rows,n_cols):
	""" Returns: 2D Array
	
	A and C are the two given matrix to do dot product.
	B is an empty Array, and n_rows and n_cols are its dimentions.
	
	"""	
	for i in range(n_rows):
		for j in range(n_cols):
			for k in range(n_cols):
				C[i][j] = C[i][j] + A[i][k]*B[k][j]
	return C
	
def enf_col(A,B,C,n_rows,n_cols):
	print "Do nothing"
	
def MatrixMul(met,A,B):
	start = 0.0
	finish = 0.0
	n_rows_A, n_cols_A = matrixLenght(A)
	n_rows_B, n_cols_B = matrixLenght(B)
	if met == "prod_punto":
		C = setEmptyMatrix(n_rows_A,n_cols_B)
		print "Ejecutando producto punto..."
		start = time()
		prod_punto(A,B,C,n_rows_A,n_cols_B)
		finish = time()
		print "Producto punto demoro " + str(finish - start) + " segundos."
	elif met == "enf_col":
		print "Enfoque de Columna"
	elif met == "enf_row":
		print "Enfoque de Fila"
	elif met == "enf_col_row":
		print "Enfoque de Columna y Fila"
	else:
		print """[ERROR] Metodo desconocido.
Opciones: 'prod_punto', 'enf_col', 'enf_row', 'enf_col_row'.""" 
	
if __name__ == "__main__":
	random = loadMatrix("Anexos_lab1/random_matrix.csv")
	vandermonde = loadMatrix("Anexos_lab1/vandermonde.csv")
	sparse = loadMatrix("Anexos_lab1/sparse.csv")
	test1 = loadMatrix("Anexos_lab1/test1.csv")
	test2 = loadMatrix("Anexos_lab1/test2.csv")
	
	MatrixMul("prod_punto", test1, test2)
