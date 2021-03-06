#pragma once
#pragma once
//Update in 19/2/19
//Update in 19/2/19
#pragma once
# include "stdio.h"
# include <io.h>
# include <stdlib.h>
#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>

#define FILE_BUFFER_LENGTH 30000
#define FLAG 0
#define ROW 9
#define COLUMN 9

typedef struct cnfdata
{
	int units;
	int clauses;
	int** matrix;
	int focus_unit;//start at 0,non-0 style
	int* solutions;//start at all 0
}cnf;

/*typedef struct openode
{
	int unit;//Non-0 style;1=NEG
	int **mairix;//Zipped matrix
	int *solution;//To store current Solution
	int unis;//To save its current number of the units
}Node;*/

void Lisence();// To show Lisence information
char* Folderanlysis();//folder read and analysis
cnf* Fileread(char* filename);//read file and return struct pointer
int* DPLL(cnf* input, double* timecost);//Standard DPLL ,return solution group's pointer
int* DPLL_revise(cnf* input, double* timecost);//Standard DPLL ,return solution group's pointer
void showmartix(int** matrix, int row, int column);//column is +1 mode
int scan_single(int** matrix, int Row, int Column);//column is +1 mode
void showsolution(int* solution, int units);//Unit=Column-1
void showmartix(int** matrix, int row, int column);
int scan_endflag(int** matrix, int row, int column);
int check(int** matrix, int*solutions, int rows, int units);
int filewirite(char* filename, int* solution, int units, double time, int flag);//To write files in file,Return status
//return =1->SUccess,=-1->failed
void Soduku();//Soduku running program
void showsoduku(int** square);
cnf* sodukugenerating(int** square);
int checkcell(int** suqare, int row, int column);
int showcell(int row, int column);
void cleansquare(int** square);
void digging(int** square);//Dig hole randomly
cnf* translation2cnf(int** square);//switch digged square 2 cnf
void fileSimulator(cnf* input);
int** solution2square(int*solution);//translate solution 2 square