/*
 *  puzzle15.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: robramo
 */

/////////////////////////////////////////////////////////////////////////////////////////

#include "puzzle8.h"

/////////////////////////////////////////////////////////////////////////////////////////

Puzzle_8::Puzzle_8(  )
{
			   unsigned char    temp[9] = {                 };
    std::array<unsigned char,9> shake   = {0,1,2,3,4,5,6,7,8};

    std::random_device rd;
    std::mt19937 g(rd( ));
    do
    {
		std::shuffle(shake.begin( ),shake.end( ),g);
	
		for(unsigned char k=0;k<9;++k) temp[k] = shake[k];
    }
    while(!solvable(temp));

	unsigned char z=0;
    for(auto          &x : board)
    for(unsigned char &y : x    )
    {
		y = temp[z];
		++z;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

Puzzle_8::Puzzle_8(const unsigned char permutation[9])
{
    if(solvable(permutation))
    {
			unsigned char z=0;
		for(auto          &x : board)
		for(unsigned char &y : x    )
		{
			y = permutation[z];
			++z;
		}
    }

    else
    {
		std::cout << std::endl << "Puzzle to solve:" << std::endl << std::endl;
	
		unsigned char z=0;
	
		for(unsigned char x=0;x<3;++x){
		for(unsigned char y=0;y<3;++y){auto tile = (short)permutation[z];
	
									   if (tile == BLANK) std::cout << " x ";
									   else               std::cout << " " << tile << " ";
	
									   ++z;} std::cout << std::endl;}
	
		std::cout << std::endl << "This permutation has no solution." << std::endl << std::endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

Puzzle_8::~Puzzle_8( )
{
    delete this;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Puzzle_8::created( ) const
{
    return(board[0][0] != board[0][1]);
}

/////////////////////////////////////////////////////////////////////////////////////////

void Puzzle_8::showBoard( ) const
{
    for(unsigned char x=0;x<3;++x){
    for(unsigned char y=0;y<3;++y){auto tile = (short)board[x][y];

								   if(tile == BLANK) std::cout << " x ";
								   else              std::cout << " " << tile << " ";} std::cout << std::endl;}
    std::cout << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool Puzzle_8::solvable(const unsigned char permutation[ ]) const
{
    unsigned char inversions = 0;

    for(unsigned char x=0;x<8;++x)
    {
		if(permutation[x] > 1)
		{
			for(auto y=(unsigned char)(x+1);y<9;++y)
	
			if(permutation[y] != BLANK && permutation[x] > permutation[y]) ++inversions;
		}
    }

    return inversions % 2 == 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

unsigned char Puzzle_8::heuristic( )
{
	return this->created( ) ? heuristic(board) : 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

inline unsigned char Puzzle_8::heuristic(unsigned char (*state)[3])
{
    unsigned char score   = 0;
    unsigned char target  = 1;
    unsigned char inter_row[3][3] = { };
    unsigned char inter_col[3][3] = { };

    for(unsigned char x=0;x<3;++x)
    for(unsigned char y=0;y<3;++y)
    {
		unsigned char tile = state[x][y];
	
		if    (tile)
		switch(tile == target)
		{
			case true: inter_row[x][y] = tile;
				   	   inter_col[y][x] = tile; break;
	
			default  : score += abs(x-goal[tile][0])  // manhattan
							 +  abs(y-goal[tile][1]); // distance
	
					   if((unsigned char)((tile-1) / 3) == x) inter_row[x][y] = tile;
					   if((unsigned char)((tile-1) % 3) == y) inter_col[y][x] = tile;
		}
	
		switch(target)
		{
			case  9: target=0; break;
			default: ++target;
		}
    }

    if (score-1)
    for(        unsigned char  x=0 ;x<3;++x)
    for(        unsigned char  y=0 ;y<2;++y)
    for(auto z=(unsigned char)(y+1);z<3;++z)
    {
		if(inter_row[x][z] && inter_row[x][y] > inter_row[x][z]) score += 2; // linear
		if(inter_col[x][z] && inter_col[x][y] > inter_col[x][z]) score += 2; // conflict
    }

    return score;
}

/////////////////////////////////////////////////////////////////////////////////////////

inline std::vector<Puzzle_8::node> Puzzle_8::expand(const node *parent)
{
    std::vector<node> children;
				node  child   ;

    if(parent->x && parent->state[parent->x-1][parent->y] != parent->move) // up
    {
		child   = *parent;
		child.x = (unsigned char)(parent->x-1);
	
		child.move = parent->state[child.x][parent->y];
	
		child.state[parent->x][parent->y] = (unsigned char)child.move;
		child.state[child.x  ][parent->y] =  BLANK;
	
		children.push_back(child);
    }

    if(parent->y && parent->state[parent->x][parent->y-1] != parent->move) // left
    {
		child   = *parent;
		child.y = (unsigned char)(parent->y-1);
	
		child.move = parent->state[parent->x][child.y];
	
		child.state[parent->x][parent->y] = (unsigned char)child.move;
		child.state[parent->x][child.y  ] =  BLANK;
	
		children.push_back(child);
    }

    if(parent->x < 2 && parent->state[parent->x+1][parent->y] != parent->move) // down
    {
		child   = *parent;
		child.x = (unsigned char)(parent->x+1);
	
		child.move = parent->state[child.x][parent->y];
	
		child.state[parent->x][parent->y] = (unsigned char)child.move;
		child.state[child.x  ][parent->y] =  BLANK;
	
		children.push_back(child);
    }

    if(parent->y < 2 && parent->state[parent->x][parent->y+1] != parent->move) // right
    {
		child   = *parent;
		child.y = (unsigned char)(parent->y+1);
	
		child.move = parent->state[parent->x][child.y];
	
		child.state[parent->x][parent->y] = (unsigned char)child.move;
		child.state[parent->x][child.y  ] =  BLANK;
	
		children.push_back(child);
    }

    return children;
}

/////////////////////////////////////////////////////////////////////////////////////////

std::vector<char> Puzzle_8::idaStar( )
{
    node toSolve;

    for(unsigned char x=0;x<3;++x)
    for(unsigned char y=0;y<3;++y)
    {
		toSolve.state[x][y] = board[x][y];
	
		if(board[x][y] == BLANK)
		{
			toSolve.x = x;
			toSolve.y = y;
		}
    }

    std::vector<node> path = { toSolve };
    std::vector<char> result;

    unsigned char bound   = heuristic(board);
    unsigned long counter = 0;

    do
    {
		unsigned char deeper = 255;
	
		result = findPath(path,1,bound,deeper,counter);
	
		std::cout << "heuristic bound: " << (short)bound << " --> nodes evaluated with this limit: " << counter << std::endl;
	
		bound   = deeper;
		counter = 0;
    }
    while (result.empty( ));

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

inline std::vector<char> Puzzle_8::findPath(std::vector<node> &path, const unsigned char &depth , const unsigned char &bound,
									   	   	   	   	   	   	   	   	   	  unsigned char &deeper,       unsigned long &counter)
{
    for(node next : expand(&path.back( )))
    {
	//if(not_in_path(next.state,path))
	//{
	    ++counter;

	    unsigned char heur = heuristic(next.state);

	    if(!heur)
	    {
	    	path.push_back(next); return buildPath(path); // goal
	    }

	    heur += depth;

	    if(heur <= bound)
	    {
			path.push_back(next);
	
			std::vector<char> result = findPath(path,(unsigned char)(depth+1),bound,deeper,counter);
	
			if(!result.empty( )) return result; // goal
	
			path.pop_back( );
	    }

	    else if(heur < deeper) deeper = heur;
	//}
    }

    return { };
}

/////////////////////////////////////////////////////////////////////////////////////////
/*
inline bool Puzzle_8::not_in_path(unsigned char(*current)[3], std::vector<node> &path)
{
    for(node already : path)
    {
	if(same_node(already.state,current)) return false;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

inline bool Puzzle_8::same_node(unsigned char(*nodeA)[3], unsigned char(*nodeB)[3])
{
    for(unsigned char x=0;x<3;++x)
    for(unsigned char y=0;y<3;++y)
    {
        if(nodeA[x][y] != nodeB[x][y]) return false;
    }

    return true;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////

std::vector<char> Puzzle_8::buildPath(const std::vector<node> &path) const
{
    std::vector<char> moves;

    for(unsigned char step=1; step < path.size( ); ++step)
    {
    	moves.push_back(path[step].move);
    }

    return moves;
}

/////////////////////////////////////////////////////////////////////////////////////////
