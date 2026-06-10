/*
 *  puzzle15.h
 *
 *  Created on: Apr 4, 2019
 *      Author: robramo
 */

#include <sys/time.h>
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <array>

#ifndef BLANK
#define  BLANK 0
#endif  /* BLANK */

#ifndef PUZZLE_15_H_
#define  PUZZLE_15_H_

/////////////////////////////////////////////////////////////////////////////////////////

class Puzzle_15
{

/////////////////////////////////////////////////////////////////////////////////////////

  public:

			   Puzzle_15(                       );
      explicit Puzzle_15(const unsigned char[16]);

      bool created  ( ) const;
      void showBoard( ) const;

      unsigned char heuristic( );

      std::vector<char> idaStar( );

      virtual ~Puzzle_15( );

/////////////////////////////////////////////////////////////////////////////////////////

  private:

      unsigned char board[4][4];

      struct node{unsigned char state[4][4]{ };
      	  	  	  	  	  char move          ;
		          unsigned char x             ;
		          unsigned char y             ;
		          node(  ){move=0; x=0; y=0;}};

      const unsigned char goal[16][2] = {{3,3},{0,0},{0,1},{0,2},{0,3},
    		  	  	  	  	  	  	  	  	   {1,0},{1,1},{1,2},{1,3},
											   {2,0},{2,1},{2,2},{2,3},
											   {3,0},{3,1},{3,2}     };

      bool solvable (const unsigned char[ ]) const;
      bool blankEven(const unsigned char[ ]) const;

      inline unsigned char heuristic(unsigned char(*)[4]);

      inline std::vector<char> findPath(std::vector<node>&, const unsigned char&, const unsigned char&, unsigned char&, unsigned long&);
      inline std::vector<node> expand (const node*);

      //inline bool not_in_path(unsigned char(*)[4], std::vector<node>& );
      //inline bool same_node  (unsigned char(*)[4], unsigned char(*)[4]);

      std::vector<char> buildPath(const std::vector<node>&) const;
};

/////////////////////////////////////////////////////////////////////////////////////////

#endif /* PUZZLE_15_H_ */
