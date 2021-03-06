#include "head.h"
int* DPLL_revise(cnf* input, double* timecost)
{
	int i, j, k;
	int status = 0;
	int endflag = 0;
	cnf* stack[1024];
	memset(stack, NULL, 1024 * sizeof(cnf*));
	cnf** top = &stack[0];
	int max_frequency_of_units = 0;
	int max_frequency = 0;
	int current_frequency = 0;//Pos unit only
	//Empty stack requirement: top=stack[0]
	int** temp;
	int orignrows = input->clauses;//To save origin data
	int** new_matrix = (int**)malloc(sizeof(int*)*input->clauses);//Re-employ space to prevent free error
	for (i = 0; i < input->clauses; i++)
	{
		new_matrix[i] = (int*)calloc(input->units + 1, sizeof(int));
	}
	for (i = 0; i < input->clauses; i++)
	{
		for (j = 0; j < input->units + 1; j++)
			new_matrix[i][j] = input->matrix[i][j];
	}
	temp = new_matrix;//Redirect
	new_matrix = input->matrix;
	input->matrix = temp;//Swap:new_matrix is backup
	//printf("[INFO] Martix has redirected\n");

	for (i = 0; i < input->units; i++)
	{
		input->solutions[i] = 0;
	}
	//printf("[INFO] Origin Solution Space has been set up\n");
	//showsolution(input->solutions, input->units);
	int track_flag = 0;
	cnf* neg_node = NULL;
	//printf("[INFO] Origin matrix has mounted\n");
	//showmartix(input->matrix, input->clauses, input->units + 1);//Clause and unit here is +1

	//Time part
	time_t start, end;
	start = clock();

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
				//printf("[INFO] Single clause detected:Unit %d\n", input->focus_unit);
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
						//printf("[INFO] Remove clause:%d\n", i + 1);//non-0 style
					}
					//Remove Same clause
					else if (input->matrix[i][abs(input->focus_unit)] == -input->focus_unit / abs(input->focus_unit))
					{
						input->matrix[i][abs(input->focus_unit)] = 0;
						//printf("[INFO] Delete unit:%d\n", i + 1);
					}//Delete different unit
				}
			}
			else
			{	//Scan first unit
				input->focus_unit = 0;
				//printf("[INFO] Start Assign process\n");
				//showmartix(input->matrix, input->clauses, input->units + 1);
				//Revise Here: Change the streatgy of selecting unit
				/*for (i = 0; i < input->units; i++)
				{
					if (input->solutions[i] == 0)
					{
						input->focus_unit = i + 1;
						break;
					}//Fixed: won't jump out when selected
				}*/
				//showsolution(input->solutions, input->units);
				max_frequency = 0;
				current_frequency = 0;
				max_frequency_of_units = 0;//initialization
				//if (input->clauses <= 20)
				//showmartix(input->matrix, input->clauses, input->units + 1);
				//showsolution(input->solutions, input->units);
				for (j = 1; j < input->units; j++)
				{
					if (input->solutions[j - 1] == 0)

					{
						current_frequency = 0;
						for (i = 0; i < input->clauses; i++)
						{
							if (input->matrix[i][0] == 1)
								continue;
							else
							{
								if (input->matrix[i][j] == 1)
									current_frequency++;//Count pos unit
							}
						}
						if (current_frequency > max_frequency)
						{
							max_frequency = current_frequency;
							max_frequency_of_units = j;
						}//Get the most frequent of the units
					}
				}
				//Prepare for neg node
				input->focus_unit = max_frequency_of_units;//non-0 style
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
				for (i = 0, k = 0; i < input->clauses; i++)//Fixed: When mount matrix in neg_node,remember that the CLAUSE of neg is much less than input!!!//???

				{
					if (input->matrix[i][0] == 0)
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
				//printf("[INFO] The neg_node has Mounted\n");
				//Push
				*top = neg_node;
				top++;
				//printf("[INFO] Push the neg node into the satck\n");
				//Transfer of control of the structure
				neg_node = NULL;//Reset
				//Operate the pos node
				//printf("[INFO] Unit:%d\n", input->focus_unit);
				//showsolution(input->solutions, input->units);
				input->solutions[input->focus_unit - 1] = 1;//Assign
				for (i = 0; i < input->clauses; i++)
				{
					if (input->matrix[i][0] != 1)//Not removed
					{
						if (input->matrix[i][input->focus_unit] == 1)
						{
							input->matrix[i][0] = 1;
							//printf("[INFO] Remove clause:%d\n", i + 1);//non-0 style
						}
						else if (input->matrix[i][input->focus_unit] == -1)
						{
							input->matrix[i][input->focus_unit] = 0;
							//printf("[INFO] Delete unit:%d\n", i + 1);
						}
					}
				}
				//printf("[INFO] Pos node operate successfully\n");
			}
		}
		else
		{
			track_flag = 0;//Reset flag
			//printf("[INFO] Start backtrack\n");
			//Free origin node
			for (i = 0; i < input->clauses; i++)
			{
				free((input->matrix[i]));//Fixme:It seems this part of memory can't release,maybe cause this matrix's address passes in the function?
				//Todo:Copy and redirect the matrix pointer at the start of the mamtrix
			}
			free(input->matrix);
			free(input);
			input = NULL;
			//printf("[INFO] The original node has been emptied\n");
			//Pop and load
			top--;
			input = *top;//Transfer of control of the structure
			*top = NULL;
			//printf("[INFO] The neg node has popped\n");
			//printf("[INFO] Node information:row:%d,units:%dfocous:%d\n", input->clauses, input->units, input->focus_unit);
			input->solutions[input->focus_unit - 1] = -1;//Fixed: Assign
			//Test part
			//showmartix(input->matrix, input->clauses, input->units + 1);
			//showsolution(input->solutions, input->units);
			//Operate part
			for (i = 0; i < input->clauses; i++)
			{
				if (input->matrix[i][input->focus_unit] == -1)
				{
					input->matrix[i][0] = 1;
					//printf("[INFO] Remove clause:%d\n", i + 1);//non-0 style
				}
				else if (input->matrix[i][input->focus_unit] == 1)
				{
					input->matrix[i][input->focus_unit] = 0;
					//printf("[INFO] Delete unit:%d\n", i + 1);
				}//Fixed: What is this part doing??
			}
			//printf("[INFO] Neg node operate successfully\n");//Fixed: The message shown in wrong place
		}
		//Judgement Part
		endflag = scan_endflag(input->matrix, input->clauses, input->units + 1);//Fixed:Not scanned
		if (endflag == 1)
		{
			end = clock();
			*timecost = ((double)(end * 1000 - start * 1000) / CLOCKS_PER_SEC);
			printf("[TIME] The process takes %lf mSec\n", *timecost);
			//showmartix(input->matrix, input->clauses, input->units + 1);
			for (i = 0; i < input->units; i++)
			{
				if (input->solutions[i] == 0)
				{
					input->solutions[i] = 1;
				}
			}//Finishing Solutions
			//showsolution(input->solutions, input->units);
			//printf("[INFO] Start checking...\n");
			if (check(new_matrix, input->solutions, orignrows, input->units) == 1)
			{
				printf("[SUCCESS] Check pass!\n");

				return input->solutions;
			}
			else
			{
				printf("[ERROR] Check failed\n");

				return NULL;
			}
		}
		else if (endflag == 0)
		{
			//printf("[INFO] Keep cycling...\n");
			continue;
		}
		else if (endflag == -1)
		{
			//printf("[ERROR] It seems these Solutions can't be satisified. Retrying...\n");
			if (top == &stack[0])
			{
				end = clock();
				*timecost = difftime(end, start);
				printf("[TIME] The process takes %lf mSec\n", *timecost);
				printf("[ERROR] It seems the matrix can't be satisified. Please check the information of the matrix or fix the bugs.\n");
				//Filewrite

				//showmartix(input->matrix, input->clauses, input->units);
				//showsolution(input->solutions, input->units);
				return NULL;
			}
			else
			{
				track_flag = 1;
				//printf("[INFO] Start Retracking\n");
				continue;
			}
		}
	} while (1);
}