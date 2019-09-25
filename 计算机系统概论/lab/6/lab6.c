#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define HIGHWALL 2000     //the height wall

int map[120][120];

void Initialmap(int row, int column)
{
	int i,j;
	//build the wall
	for(i = 0; i < column + 2; i++)
		map[0][i] = HIGHWALL;
	for(i = 0; i < row + 2; i++)
		map[i][0] = HIGHWALL;
	for(i = 0; i < column + 2; i++)
		map[row+1][i] = HIGHWALL;
	for(i = 0; i < row + 2; i++)
		map[i][column+1] = HIGHWALL;
	//input
	for(i = 1; i < row + 1; i++)
		for(j = 1; j < column + 1; j++)
			scanf("%d", &map[i][j]);
}

int searchlength(int i, int j)
{
	int n;
	int temp;
	int max = 0;
	//EAST
	if(map[i][j+1] < map[i][j])
	{
		temp = searchlength(i,j+1);
		if (temp > max)  
            max = temp;  
	}
	//SOUTH
	if(map[i+1][j] < map[i][j])
	{
		temp = searchlength(i+1,j);
		if(temp > max)
			max = temp;
	}
	//NORTH
	if(map[i-1][j] < map[i][j])
	{
		temp = searchlength(i-1,j);
		if(temp > max)
			max = temp;
	}
	//WEST
	if(map[i][j-1] < map[i][j])
	{
		temp = searchlength(i,j-1);
		if(temp > max)
			max = temp;
	}
	//finish recursion and add 1(itself) 
	return max + 1;
}
int main()
{
	int maxlength = 0;
	int row,column;
	int i,j;
	int temp;
	scanf("%d%d",&row,&column);
	Initialmap(row, column);
	//compare every point's maxlength
	for(i = 1; i < row + 1; i++)
		for(j = 1; j < column + 1; j++)
		{
			temp = searchlength(i,j);
			if(maxlength < temp)
				maxlength = temp;
		}
	printf("%d",maxlength);
	return 0;

}
