/*
 *  nPuzzle.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: robramo
 */

/////////////////////////////////////////////////////////////////////////////////////////

#include "puzzle8.h"
#include "puzzle15.h"

/////////////////////////////////////////////////////////////////////////////////////////

bool run(auto *puzzle)
{
    if(!puzzle->created( )) return false; 

    std::cout << std::endl << "Puzzle to solve:" << std::endl << std::endl;

    puzzle->showBoard( );
    
    if(puzzle->heuristic( ))
    {
		clock_t tStart = clock( );
	
		std::vector<char> solution = puzzle->idaStar( );
	
		std::cout << std::endl << "optimal solution: ";
	
		for(char move : solution)
		{
			std::cout << (short)move << " ";
		}
	
		std::cout << std::endl << std::endl << "moves: " << solution.size( ) << std::endl << std::endl;
	
		printf("execution time: %.3fs\n",(double)(clock( )-tStart)/CLOCKS_PER_SEC);
	
		std::cout << std::endl;
    }
    else
    {
    	std::cout << "This permutation is already the solution, there is nothing to do." << std::endl << std::endl;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool canonical_tile(const unsigned char tile, const unsigned char max, const unsigned char permutation[ ])
{
    for(unsigned char k=0;k<max;++k)
    {
    	if(tile == permutation[k]) return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool check_perm(int argc, const char *argv[ ])
{
    unsigned char permutation[argc] = { };

    for(unsigned char k=0;k<argc;++k) permutation[k] = (unsigned char)atoi(argv[k+1]);
    for(unsigned char k=0;k<argc;++k)
    {
		if(!canonical_tile(k,argc,permutation))
		{
			std::cout << std::endl;
			std::cout << "Non-canonical permutation:\n9 or 16 numbers from 0 to N-1 are required;\nwhere 0 indicates the blank position.";
			std::cout << std::endl << std::endl;
	
			return false;
		}
    }

    switch(argc)
    {
		case  9: if(run(new Puzzle_8 (permutation))) return true; break;
		case 16: if(run(new Puzzle_15(permutation))) return true; break;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool random_puzzle(unsigned char size)
{
    switch(size)
    {
		case  9: return run(new Puzzle_8 ( ));
		case 16: return run(new Puzzle_15( ));
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[ ])
{
    switch(argc-1)
    {
        case  1: if(random_puzzle((unsigned char)atoi(argv[1]))) return 0; break;
        case  9:    check_perm   (                    9,argv  ); return 0;
        case 16:    check_perm   (                   16,argv  ); return 0;
    }

    std::cout << std::endl;
    std::cout << "Usage:\n";
    std::cout << "to generate a random solvable permutation : ~/nPuzzle_C++ 9\n";
    std::cout << "                                       or : ~/nPuzzle_C++ 16\n\n";
    std::cout << "to check and solve a custom puzzle add to ~/nPuzzle_C++\na permutation of numbers from 0 to n-1 separated by empty spaces\n";
    std::cout << "where 0 indicates the blank position\n";
    std::cout << "\nexample: ~/nPuzzle_C++ 3 8 6 9 14 4 15 2 0 13 1 12 7 11 10 5";
    std::cout << std::endl << std::endl;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
