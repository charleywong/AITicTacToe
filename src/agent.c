/*********************************************************
 *  agent.c
 *  Nine-Board Tic-Tac-Toe Agent
 *  COMP3411/9414/9814 Artificial Intelligence
 *  Alan Blair, CSE, UNSW
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "common.h"
#include "agent.h"
#include "game.h"

#define MAX_MOVE 81

int board[10][10];
int move[MAX_MOVE+1];
int player;
int m;

/*********************************************************//*
   Print usage information and exit
*/
void usage( char argv0[] )
{
  printf("Usage: %s\n",argv0);
  printf("       [-p port]\n"); // tcp port
  printf("       [-h host]\n"); // tcp host
  exit(1);
}

/*********************************************************//*
   Parse command-line arguments
*/
void agent_parse_args( int argc, char *argv[] )
{
  int i=1;
  while( i < argc ) {
    if( strcmp( argv[i], "-p" ) == 0 ) {
      if( i+1 >= argc ) {
        usage( argv[0] );
      }
      port = atoi(argv[i+1]);
      i += 2;
    }
    else if( strcmp( argv[i], "-h" ) == 0 ) {
      if( i+1 >= argc ) {
        usage( argv[0] );
      }
      host = argv[i+1];
      i += 2;
    }
    else {
      usage( argv[0] );
    }
  }
}

/*********************************************************//*
   Called at the beginning of a series of games
*/
void agent_init()
{
  struct timeval tp;

  // generate a new random seed each time
  gettimeofday( &tp, NULL );
  srandom(( unsigned int )( tp.tv_usec ));
}

/*********************************************************//*
   Called at the beginning of each game
*/
void agent_start( int this_player )
{
  reset_board( board );
  m = 0;
  move[m] = 0;
  player = this_player;
}

/*********************************************************//*
   Choose second move and return it
*/
int agent_second_move( int board_num, int prev_move )
{
  int this_move;
  move[0] = board_num;
  move[1] = prev_move;
  board[board_num][prev_move] = !player;
  m = 2;
  
  int alpha = -1000;
  int beta = 1000;
  // this_move = minimax(prev_move,player, alpha, beta);
  // int bestScore = 1;
  int depth = 0;
  for (int i = 1; i < 10; i++) {
    if (board[prev_move][i] == EMPTY) {
      board[prev_move][i] = player;
      int tempScore = minimax(i, !player, depth+1, alpha, beta);
      printf("tempScore for return: %d\n", tempScore);
      board[prev_move][i] = EMPTY;
      if (tempScore > alpha) {
        alpha = tempScore;
        this_move = i;
      }
    }
  } 
  move[m] = this_move;
  board[prev_move][this_move] = player;
  return( this_move );
}

/*********************************************************//*
   Choose third move and return it
*/
int agent_third_move(
                     int board_num,
                     int first_move,
                     int prev_move
                    )
{
  int this_move;
  move[0] = board_num;
  move[1] = first_move;
  move[2] = prev_move;
  board[board_num][first_move] =  player;
  board[first_move][prev_move] = !player;
  m=3;

  int alpha = -1000;
  int beta = 1000;
  int depth = 0;
  for (int i = 1; i < 10; i++) {
    if (board[prev_move][i] == EMPTY) {
      board[prev_move][i] = player;
      int tempScore = minimax(i, !player, depth+1, alpha, beta);
      board[prev_move][i] = EMPTY;
      if (tempScore > alpha) {
        alpha = tempScore;
        this_move = i;
      }
    }
  } 

  // do {
    // check if prev_move's board is empty
  // this_move = minimax(prev_move, player);
  // printf("this_move: %d", this_move);
    // this_move = 1 + random()% 9;
  // } while( board[prev_move][this_move] != EMPTY );
  move[m] = this_move;
  board[move[m-1]][this_move] = player;
  return( this_move );
}

/*********************************************************//*
   Choose next move and return it
   use minimax algorithm
*/
int agent_next_move( int prev_move )
{
  printf("NEXT MOVE \n");
  // print
  int this_move;
  m++;
  move[m] = prev_move;
  board[move[m-1]][move[m]] = !player;
  m++;
  int alpha = -1000;
  int beta = 1000;
  int depth = 0;
  for (int i = 1; i < 10; i++) {
    if (board[prev_move][i] == EMPTY) {
      board[prev_move][i] = player;
      int tempScore = minimax(i, !player, depth+1, alpha, beta);
      board[prev_move][i] = EMPTY;
      if (tempScore > alpha) {
        alpha = tempScore;
        this_move = i;
      }
    }
  }
  move[m] = this_move;
  board[move[m-1]][this_move] = player;
  return( this_move );
}

/*********************************************************//*
   Receive last move and mark it on the board
*/
void agent_last_move( int prev_move )
{
  m++;
  move[m] = prev_move;
  board[move[m-1]][move[m]] = !player;
}

/*********************************************************//*
   Called after each game
*/
void agent_gameover(
                    int result,// WIN, LOSS or DRAW
                    int cause  // TRIPLE, ILLEGAL_MOVE, TIMEOUT or FULL_BOARD
                   )
{
  // nothing to do here
}

/*********************************************************//*
   Called after the series of games
*/
void agent_cleanup()
{
  // nothing to do here
}

/****************

  minimax
  function would just return -1,0,1.


****************/
int minimax(int board_num, int p, int depth, int alpha, int beta) {
  printf("player is: %d\n", p);
  printf("depth is: %d\n", depth);
  int endScore = winner(board_num,p);
  printf("endScore: %d\n", endScore);

  if (depth == 7 || game_over(board_num)) { // if max depth reached or no possible moves in curr board
    return winner(board_num,p);
  }

  printf("%d %d\n", alpha,beta);

  if (p == player) {
    printf("ai turn\n");
    for (int i = 1; i < 10; i++) {
      if (board[board_num][i] == EMPTY) {
        board[board_num][i] = player;
        printf("%d %d %d\n", board[board_num][i],board_num, i);
        printf("p: %d\n", p);
        printf("alpha beta at call for ai: %d %d\n", alpha, beta);
        int tempScore = minimax(i, !p, depth+1, alpha, beta);
        board[board_num][i] = EMPTY;
        if (endScore < tempScore) {
          endScore = tempScore - depth * 10;
        }
        if (endScore > alpha) {
          alpha = endScore;
        }
        if (alpha >= beta){
          printf("returning alpha: %d\n", alpha);
          return alpha;
          // break;
        }
        printf("alpha after iteration: %d\n", alpha);
      }
    } 
    printf("alpha after for loop: %d\n", alpha);
    return alpha;
  } else {
    printf("player turn\n");
    for (int i = 1; i < 10; i++) {
      if(board[board_num][i] == EMPTY) {
        board[board_num][i] = !player;
        printf("%d %d %d\n", board[board_num][i],board_num, i);
        printf("alpha beta at call for player: %d %d\n", alpha, beta);
        int tempScore = minimax(i, !p, depth+1,alpha, beta);
        board[board_num][i] = EMPTY;
        printf("tempScore on player side: %d\n", tempScore);

        if (endScore > tempScore) {
          endScore = tempScore + depth *10;
        }
        if (endScore < beta) {
          beta = endScore;
          printf("beta on player side: %d\n", beta);
        }
        if (beta <= alpha) {
          return beta;
        }
      }
    } 
    return beta;
  }
}



// we need to check if there is a winner at all
// in any board
int winner(int board_num, int p) {
  for (int i = 1; i < 10; i++) {
    if ((board[i][1] == player && board[i][2] == player && board[i][3] == player) || // top left to top right
      (board[i][1] == player && board[i][5] == player && board[i][9] == player) ||   // top left to bot right
      (board[i][1] == player && board[i][4] == player && board[i][7] == player) ||   // top left to bot left
      (board[i][4] == player && board[i][5] == player && board[i][6] == player) ||   // mid left to mid right
      (board[i][7] == player && board[i][5] == player && board[i][3] == player) ||   // bot left to top right
      (board[i][7] == player && board[i][8] == player && board[i][9] == player) ||   // bot left to bot right
      (board[i][2] == player && board[i][5] == player && board[i][8] == player) ||   // top mid to bot mid  
      (board[i][3] == player && board[i][6] == player && board[i][9] == player)      // top right to bot right
      ) 
    {
      return 10; // ai win
    } else if ((board[i][1] == !player && board[i][2] == !player && board[i][3] == !player) || // top left to top right
      (board[i][1] == !player && board[i][5] == !player && board[i][9] == !player) ||   // top left to bot right
      (board[i][1] == !player && board[i][4] == !player && board[i][7] == !player) ||   // top left to bot left
      (board[i][4] == !player && board[i][5] == !player && board[i][6] == !player) ||   // mid left to mid right
      (board[i][7] == !player && board[i][5] == !player && board[i][3] == !player) ||   // bot left to top right
      (board[i][7] == !player && board[i][8] == !player && board[i][9] == !player) ||   // bot left to bot right
      (board[i][2] == !player && board[i][5] == !player && board[i][8] == !player) ||   // top mid to bot mid  
      (board[i][3] == !player && board[i][6] == !player && board[i][9] == !player)      // top right to bot right
      )
    {
      return -10; // player win
    }
    
  }
  return 0; // no winner, default to 0
}

int game_over( int board_num) {

  // checks if all cells in the current board num are occupied
  for (int i = 1; i < 10; i++) {
    if (board[board_num][i] == EMPTY) {
      return FALSE;
    }
  }
  return TRUE;
}