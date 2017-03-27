
/**************************************
 **    World War Z Simulation        **
 **    Created By Charles Davis      **
 **    CPSC 246-01  Dr. Valentine    **
 **************************************/

/*     World War Z is a program that uses cellular automation simulation
 *     to model a zombie invasion.  The program asks the user for the 
 *     location of the first zombie and displays each frame as an animation
 *     showing how the zombies turn humans into new zombies and then die
 *     off and decay.  Then it spits out the resulting statistics. How
 *     many humans are still alive?  How many itterations did it take?
 *     What percentage of the crowd turned zombie?
 */

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;

const int SIZE = 30;     //Array is SIZE x SIZE

//Function Prototypes
void setupGame(char current[][SIZE]);
void getFirstZombie(char current[][SIZE]);
void updateState(char current[][SIZE]);
int getNeighbors(const char current[][SIZE], int currentRow, int currentCol);
int getNumHumans(const char current[][SIZE]);
void wait(float seconds);
bool isValidCoordinate(int row, int col);
bool becomesZombie(int percentProbability);
void outputState(const char current[][SIZE]);
void outputResult(const char current[][SIZE], int gameCnt, int humansRemain);
bool hasNextZombie(const char current[][SIZE]);



/************************** Main **************************/
int main()
{
	//Variable Dictionary
	char gameStateArray[SIZE][SIZE]; //our main array
	int count = 0;					//count of iterations of the sim
	int numHumansRemaining = 0;	//final num of humans remaining

	//1.0 Init and Input
	setupGame(gameStateArray);		//sets up the array
	getFirstZombie(gameStateArray);	//gets first zombie from the user
	outputState(gameStateArray);		//print orig state with 1st zombie 

	wait(.5); 	//the system waits to see original array

	//2.0 Process 
	while(hasNextZombie(gameStateArray) == true)
	{
		count++;		//update the iteration count
		updateState(gameStateArray);		//process this state to next state
		outputState(gameStateArray);		//print the next state
		wait(.5);						//pause to show animation
	}
		
	//3.0 Output
	numHumansRemaining = getNumHumans(gameStateArray);	//remaining humans
	outputResult(gameStateArray, count, numHumansRemaining);	 //print result

	//program run was successful print out and return 0
	cout <<"\n\n\n\t Normal Termination \n\n\n" << endl;
	return 0;

} //end main



/*********************** setupGame ************************/
void setupGame(char current[][SIZE])
{
	// recieves the current game state array to setup.
	// prints out name, course, and assignment name.
	// initializes the boarder of the array to be decayed zombies
	// and everthing else to be healthy humans. It also seeds 
	// a random number generator

	cout << "\n\n================================================" << endl;
	cout << "Charles Davis - CPSC 246.01 - Zombie Simulation" << endl;
	cout << "================================================\n\n" << endl;


	// chunk of code initializing the board with a frame of decayed zombies
	// '.' and every other cell to healthy humans '+'

	for (int c = 0; c < SIZE; c++) //for the top and bottom rows
	{
		current[0][c] = '.';			//row 0 with all decayed zombies '.'
		current[SIZE-1][c] = '.'	;	//row SIZE-1 with all decayed zombies '.'
	}
	
	
	for (int r = 0; r < SIZE; r++) //for the first and last columnn;
	{
		current[r][0] = '.';			//column 0 with decayed zombies '.'
		current[r][SIZE-1] = '.';	//column SIZE-1 with decayed zombies '.'
	}

	for(int r = 1; r < SIZE-1; r++) // for everything else
	{	
		for(int c = 1; c < SIZE-1; c++) 
		{
				current[r][c] = '+';		//other cells to be healthy humans '+'
		}
	}

	// end board init chunk


	//Seed a random number generator with the current time
	srand((unsigned) time(0));

}



/********************* getFirstZombie *********************/
void getFirstZombie(char current[][SIZE])
{
	// will get a coordinate from the user for the first zombie's position

	outputState(current);

	int row, col;
	
	do{
	
	cout << "Enter first zombie coordinates. 1  They must be greater "<< endl;
	cout << "than 0 and less than 29 with a space between them " << endl;
	cout << "(e.g. 5 5): " << endl;

	cin >> row;
	cin >> col;

	cout << "You entered { " << row << " , " << col << " }" << endl; 
	wait(2)

	}while(!isValidCoordinate(row,col));

	current[row][col] = 'Z';
}



/********************** updateState ***********************/
void updateState(char current[][SIZE])
{
	// will update the state of the game by copying the current array
	// killing off current zombies for the next state, and making
	// new zombies from healthy humans based on probablility. Finally
	// it copies the array back to the array referenced passed in.

	char tempArray [SIZE][SIZE]; //temporary array to update
	int tempNeighbors;		// temporary int to store the num of neighbors


	for(int r = 0; r < SIZE; r++) // copy every element to tempArray
	{
		for(int c = 0; c < SIZE; c++) 
		{
			tempArray[r][c] = current[r][c]; //copy curr elmnt into temp array
		}
	}//end copy


	for(int r = 1; r < SIZE-1; r++) //excluding the top and bottom
	{
		for(int c = 1; c < SIZE-1; c++) //and both sides
		{
			// if it is a zombie already kill it for the next state

			if(current[r][c] == 'Z')
			{
				tempArray[r][c] = '.'; //turns current cell into decayed human
			}

			tempNeighbors = 0;

			// if the current char is not a zombie or a decayed zombie then
			// its a human.  Get its zombie neighbors and convert it 
			// according the rules of the game
			tempNeighbors = getNeighbors(current,r,c);

			if(current[r][c] == '+')
			{
			 //get num zombie neighbors of current cell

				if(tempNeighbors == 0)
				{ 
					//do nothing 
				}

				//neighbors are between 1 and 2
				else if(tempNeighbors > 0 && tempNeighbors <= 2)
				{
					//there is a 50% chance of becoming a zombie
					if(becomesZombie(50) == true)
					{
						tempArray[r][c] = 'Z';	//make into zombie
					}
					else
					{
						//do nothing
					}

				}

				// neighbors are between 3 and 5
				else if(tempNeighbors > 2 && tempNeighbors <= 5)
				{
					//there is a 70% chance of becoming a zombie
					if(becomesZombie(70) == true)
					{
						tempArray[r][c] = 'Z';
					}
					else
					{
						//do nothing
					}

				}

				//neighbors are more than 5
				else if(tempNeighbors > 5)
				{
					//99% you are probably going to be a zombie
					if(becomesZombie(99) == true)
					{
						tempArray[r][c] = 'Z';
					}

				}else{ /*do nothing*/ }

			}else{ /* do nothing*/ }
			
		}
	} //end getNeighbors / kill zombies if else chunk
			
		
		//lastly we copy our temp array back to main array
		for(int r = 0; r < SIZE; r++) 
		{
			for(int c = 0; c < SIZE; c++)
			{
				current[r][c] = tempArray[r][c];
			}
		} //end copy 
} //end updateState


/********************** getNeighbors **********************/
int getNeighbors(const char current[][SIZE], int currentRow, int currentCol)
{
	//will get the number of zombie neighbors of the current cell

	int neighbors = 0;			//temp variable for the number of neighbors
	
	//offset array for checking each direction around the current cell
	int offset[8][2] = {{-1,0},  // N
						{-1,1},  // NE
						 {0,1},  // E
						 {1,1},  // SE
						 {1,0},  // S
						{1,-1},  // SW
						{0,-1},  // W
					   {-1,-1}}; // NW


	for(int r = 0; r < 8; r++) // for each direction
	{
		// if the current neighbor is a zombie increment neighbors count

		if(current[currentRow+offset[r][0]][currentCol+offset[r][1]] == 'Z')
		{
			neighbors++; //icrement count
		}
		else{ /*do nothing*/ }
	}

	return neighbors;
}



/********************** getNumHumans **********************/
int getNumHumans(const char current[][SIZE])
{

	//gets the number of healthy humans in the whole array
	int tempHumans = 0;

	for(int r = 0; r < SIZE; r++) //search the array for humans
		{
			for(int c = 0; c < SIZE; c++) 
			{
				//if the current cell is a human increment the count
				if(current[r][c] == '+'){
					tempHumans++;
				}
			}
		}
	
	return tempHumans;
}



/************************** wait **************************/
void wait(float seconds)
{
	//pauses the system for a given time in seconds
	clock_t endWait;
	endWait = clock() + seconds * CLOCKS_PER_SEC;
	while(clock() < endWait) { /* do nothing */ }

}


/******************** isValidCoordinate *******************/
bool isValidCoordinate(int row, int col)
{
	// will check to see if the coordinates given are valid
	// prints out an error message for invalid pair


	//make sure values are greater than 0 and less than the size
	//of the array minus one for the boarder.

	if(row > 0 && row < SIZE-1 && col > 0 && col < SIZE-1)
	{
		return true;
	}
	else
	{
		cerr << "\nThe coordinate pair is invalid. "<< endl;
		cerr << "Please try again\n\n " << endl;
		return false;
	}

}



/********************** becomesZombie *********************/
bool becomesZombie(int percentProbability)
{
	// randomizes a number and returns a bool based on the 
	// given probability percentage threshold

	return rand() % 100 < percentProbability;
}



/*********************** outputState **********************/
void outputState(const char current[][SIZE])
{
	//will print out each element in the array sent to it

	for(int r = 0; r < SIZE; r++) //for every element
	{
		for(int c = 0; c < SIZE; c++)
		{
			cout << current[r][c] << " "; //print current char and a space
		}
		cout << endl;	//after each row print a new line
	}
	cout << "\n\n\n"; // print a couple new lines as a buffer
}



/********************** outputResult **********************/
void outputResult(const char current[][SIZE], int gameCnt, int humansRemain)
{
	// will output the final game board, the num of humans remaining,
	// the num of iterations it took to complete and the % of humans
	// that became zombies

	double originalNumHumans; //orig humans is area of board minus boarder
	double percentZombie; // the difference in humans / orig humans
	
	
	originalNumHumans = (SIZE * SIZE) - ((SIZE * 4) - 4);

	percentZombie = ((originalNumHumans - humansRemain) / originalNumHumans) * 100;
	
	//outputState(current); //output the final state of the board

	cout << "There are " << humansRemain << " humans remaining out of ";
	cout	 << originalNumHumans << " humans. " << percentZombie << "%";
	cout	 <<" were made into zombies. It took " << gameCnt << " iterations.";

}



/********************** hasNextZombie *********************/

bool hasNextZombie(const char current[][SIZE])
{
	// will search the entire array to see if any zombies still exist

	for(int r = 1; r < SIZE-1; r++) //excluding the boarder
		{
			for(int c = 1; c < SIZE-1; c++) 
			{
				if(current[r][c] == 'Z') //if current cell is a Zombie
				{
					return true; // yes there is a next zombie
				}
			}
		}

	return false; // there are no zombies left
}








