#include "head.h"
int* DPLL(cnf* input)
{
	int i, j, k;
	int endflag = 0;
	cnf* stack[512];
	memset(stack, NULL, 512);
	cnf** top = &stack[0];
	//Empty stack requirement: top=stack[0]
	for (i = 0; i < input->units; i++)
	{
		input->solutions[i] = 0;
	}
	printf("[INFO] Origin Solution Space has been set up\n");
	showsolution(input->solutions, input->units);
	int track_flag = 0;
	cnf* neg_node = NULL;
	printf("[INFO] Origin matrix has mounted\n");
	showmartix(input->matrix, input->clauses, input->units + 1);//Clause and unit here is +1

	getchar();
	do
	{
		endflag = 0;//Fixed: Redirect to prevent Infinite loop
		if (track_flag == 0)
		{
			if (input->focus_unit = scan_single(input->matrix, input->clauses, input->units + 1), input->focus_unit != 0)
				//TODO: Review the scansingle clause, attention that the clause and unit is +1 mode
				//Done. Need more test
			{
				//Mount Solution
				input->solutions[abs(input->focus_unit) - 1] = input->focus_unit / abs(input->focus_unit);
				printf("[INFO] Single clause detected:Unit %d\n", input->focus_unit);
				//Operate the matrix
				for (i = 0; i < input->clauses; i++)
				{
					if (input->matrix[i][abs(input->focus_unit)] == input->focus_unit / abs(input->focus_unit))
					{
						/*for (j = 0; j < input->clauses; j++)
						{
							matrix[i][j] = 255;
						}*///Older version
						input->matrix[i][0] = 1;//Remove and set the flag digit
						printf("[INFO] Remove clause:%d\n", i + 1);//non-0 style
					}
					//Remove Same clause
					else if (input->matrix[i][abs(input->focus_unit)] == -input->focus_unit / abs(input->focus_unit))
					{
						input->matrix[i][abs(input->focus_unit)] = 0;
						printf("[INFO] Delete unit:%d\n", i + 1);
					}//Delete different unit
				}
			}
			else
			{	//Scan first unit
				input->focus_unit = 0;
				printf("[INFO] Start Assign process\n");
				showmartix(input->matrix, input->clauses, input->units + 1);
				for (i = 0; i < input->units; i++)
				{
					if (input->solutions[i] == 0)
					{
						input->focus_unit = i + 1;
						break;
					}//Fixed: won't jump out when selected
				}
				//Prepare neg node
				neg_node = (cnf*)malloc(sizeof(cnf));
				neg_node->focus_unit = input->focus_unit;
				neg_node->units = input->units;
				neg_node->focus_unit = input->focus_unit;
				neg_node->solutions = (int*)calloc(input->units, sizeof(int));

				
				//Only get 0-flag clauses
				neg_node->clauses = 0;
				for (i = 0; i < input->clauses; i++)
				{
					if (input->matrix[i][0] == 0)
						neg_node->clauses++;
				}
				neg_node->matrix = (int**)malloc(sizeof(int*)*neg_node->clauses);
				for (i = 0; i < neg_node->clauses; i++)
					neg_node->matrix[i] = (int*)malloc(sizeof(int)*(input->units + 1));//Reserve for flag digit
				for (i = 0, k = 0; i < neg_node->clauses; i++)//Fixed: When mount matrix in neg_node,remember that the CLAUSE of neg is much less than input!!!

				{
					if (input->matrix[i][0] != 1)
					{
						for (j = 0; j < (neg_node->units + 1); j++)
						{
							neg_node->matrix[k][j] = input->matrix[i][j];
						}
						k++;//Mount units to the new matrix
					}
				}
				for (i = 0; i < input->units; i++)
				{
					neg_node->solutions[i] = input->solutions[i];
				}//Mount solution area
				printf("[INFO] The neg_node has Mounted\n");
				//Push
				*top = neg_node;
				top++;
				printf("[INFO] Push the neg node into the satck\n");
				//Transfer of control of the structure
				neg_node = NULL;//Reset
				//Operate the pos node
				printf("[INFO] Unit:%d\n", input->focus_unit);
				showsolution(input->solutions, input->units);
				input->solutions[input->focus_unit - 1] = 1;//Assign
				for (i = 0; i < input->clauses; i++)
				{
					if (input->matrix[i][0] != 1)//Not removed
					{
						if (input->matrix[i][input->focus_unit] == 1)
						{
							input->matrix[i][0] = 1;
							printf("[INFO] Remove clause:%d\n", i + 1);//non-0 style
						}
						else if (input->matrix[i][input->focus_unit] == -1)
						{
							input->matrix[i][input->focus_unit] = 0;
							printf("[INFO] Delete unit:%d\n", i + 1);
						}
					}
				}
				printf("[INFO] Pos node operate successfully\n");
			}
		}
		else
		{
			track_flag = 0;//Reset flag
			printf("[INFO] Start backtrack\n");
			//Free origin node
			for (i = 0; i < input->clauses; i++)
			{
				free(input->matrix[i]);
			}
			free(input->matrix);
			free(input);
			input = NULL;
			printf("[INFO] The original node has been emptied\n");
			//Pop and load
			top--;
			input = *top;//Transfer of control of the structure
			*top = NULL;
			printf("[INFO] The neg node has popped\n");
			//Test part
			showmartix(input->matrix, input->clauses, input->units + 1);
			showsolution(input->solutions, input->units);
			//Operate part
			for (i = 0; i < input->clauses; i++)
			{
				if (input->matrix[i][input->focus_unit] == 1)
				{
					input->matrix[i][0] = -1;
					printf("[INFO] Remove clause:%d\n", i + 1);//non-0 style
				}
				else if (input->matrix[i][input->focus_unit] == 1)
				{
					input->matrix[i][input->focus_unit] = 0;
					printf("[INFO] Delete unit:%d\n", i + 1);
				}
				printf("[INFO] Neg node operate successfully\n");
			}
		}
		//Judgement Part
		endflag = scan_endflag(input->matrix, input->clauses, input->units + 1);//Fixed:Not scanned
		if (endflag == 1)
		{
			printf("[SUCCESS] The matrix has been solved!\n");
			showmartix(input->matrix, input->clauses, input->units + 1);
			for (i = 0; i < input->units; i++)
			{
				if (input->solutions[i] == 0)
				{
					input->solutions[i] = 1;
				}
			}//Finishing Solutions
			showsolution(input->solutions, input->units);

			return input->solutions;
		}
		else if (endflag == 0)
		{
			printf("[INFO] Keep cycling...\n");
			continue;
		}
		else if (endflag == -1)
		{
			printf("[ERROR] It seems these Solutions can't be satisified. Retrying...\n");
			if (top == &stack[0])
			{
				printf("[ERROR] It seems the matrix can't be satisified. Please check the information of the matrix or fix the bugs.\n");
				showmartix(input->matrix, input->clauses, input->units);
				showsolution(input->solutions, input->units);
				return NULL;
			}
			else
			{
				track_flag = 1;
				printf("[INFO] Start Retracking\n");
				continue;
			}
		}
	} while (1);
}

int scan_single(int** matrix, int Row, int Column)//+1 mode
{
	int i, j;//Cycle point
	int unit = 0;
	int pos_flag = 0;//If it's a positive clause
	int neg_flag = 0;//If it's a negative clause
	for (i = 0; i < Row; i++)
	{
		if (matrix[i][0] == 1)
			continue;//Fixed: Should be to the bottom of the loop instead of going straight out
		for (j = 1, pos_flag = 0, neg_flag = 0; j < Column; j++)
		{
			if (matrix[i][j] == 1)
			{
				pos_flag++;
				unit = j;
			}
			if (matrix[i][j] == -1)
			{
				neg_flag++;
				unit = -j;
			}
		}
		if (pos_flag == 1 || neg_flag == 1)
			return unit;
		//Return unit's order (non-0 style)
	}
	return 0;//If not exist,return 0
}

void showsolution(int* solution, int units)
{
	int i;
	printf("[TEST] ");
	for (i = 0; i < units; i++)
		printf("%d ", solution[i]);
	printf("%x", &solution);
	printf("\n");
}
void showmartix(int** matrix, int row, int column)
{
	int i, j;
	printf("[TESTx] F\n");
	for (i = 0; i < row; i++)
	{
		printf("[TEST%d] ", i);
		for (j = 0; j < column; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}
int scan_endflag(int** matrix, int row, int column)
//To judge if any special position(all 255 or a line 0)
//If all Removed, return 1
//If any 0000, return -1
//Return 0 means nothing happens

{
	int i, j;
	int trueflag = 1;
	int falseflag = 1;
	for (falseflag = 1/*Reset flag*/, i = 0; i < row; i++)
	{
		if (matrix[i][0] == 1)
			continue;
		else
		{
			trueflag = 0;
		}
		for (j = 0; j < column; j++)
		{
			if (matrix[i][j] != 0)
			{
				falseflag = 0;
			}
		}
		if (falseflag == 1)
			return -1;
	}
	if (trueflag == 1)
		return 1;
	else
		return 0;
}