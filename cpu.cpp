//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <string>
#pragma hdrstop

#include "cpu.h"
#include "pawn.h"
#include "main.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#define debug_print(x) Form1->Memo1->Lines->Add(x)
int g_tableWeight[] = 					   { 4, 4, 4, 4,
											 4, 3, 3, 3,
											 3, 2, 2, 4,
											 4, 2, 1, 3,
											 3, 1, 2, 4,
											 4, 2, 2, 3,
											 3, 3, 3, 4,
											 4, 4, 4, 4};

int g_tableSimpleWeight [] = 			   { 3, 3, 3, 3,
											 3,	2, 2, 2,
											 2, 1, 1, 3,
											 3, 1, 1, 2,
											 2, 1, 1, 3,
											 3, 1, 1, 2,
											 2, 2, 2, 3,
											 3, 3, 3, 3};
void CCPU::play()
{
	m_maxDepth = 8;
	m_alphaBeta = Form1->CheckBox1->Checked;
	CBoard::MoveList moveList = minimax(*m_board);
	m_board->movePawn(moveList);
	CBoard::MoveListIter move = moveList.begin();	
	for(; move != moveList.end(); move++)
	{
		Form1->Memo1->Lines->Add(
							Format("[CPU] from {%d,%d} to {%d,%d}",OPENARRAY(TVarRec,
							(move->first.x,
							move->first.y,
							move->second.x,
							move->second.y
																				)))
							);
	}
}
void CCPU::setBoard(CBoard* board)
{
    m_board = board;
}
int CCPU::getMaxDepth()
{
	return m_maxDepth;
}
CBoard::MoveList CCPU::minimax(CBoard& board)
{

	CBoard nextBoard;
	CBoard::SpecialMoveList specialMoveList;
	CBoard::MoveList bestMove;
	int value	 = 0;
	int maxValue = 0x80000000;
	specialMoveList = board.getAllMoves();
	CBoard::SpecialMoveListIter moveList = specialMoveList.begin();
	while(moveList != specialMoveList.end())
	{
		nextBoard = board;
		nextBoard.movePawn(*moveList);
		if(m_alphaBeta)
			value = minMoveAB(nextBoard,1,maxValue,0x7fffffff);
		else
			value = minMove(nextBoard,1);
		if(value > maxValue)
		{
			maxValue = value;
			bestMove = *moveList;
		}
		++moveList;
	}
  return bestMove;
}
int CCPU::minMoveAB(CBoard& board,int depth,int alpha,int beta)
{
	if(haveToStop(board,depth))
		return eval(board);
		
	CBoard nextBoard;
	CBoard::SpecialMoveList specialMoveList;
	int value	 = 0;
	specialMoveList = board.getAllMoves();
	CBoard::SpecialMoveListIter moveList = specialMoveList.begin();
	while(moveList != specialMoveList.end())
	{
		nextBoard = board;
		nextBoard.movePawn(*moveList);
		value = maxMoveAB(nextBoard,depth+1,alpha,beta);

		if(value < beta)
		{
			beta = value;
		}
		if(beta <= alpha)
		{
//			debug_print("odcialem");
         	return alpha;
		}
		++moveList;
	}
	return  beta;
}
int CCPU::maxMoveAB(CBoard& board,int depth,int alpha,int beta)
{
	if(haveToStop(board,depth))
    	return eval(board);

	CBoard nextBoard;
	CBoard::SpecialMoveList specialMoveList;
	int value	 = 0;
	specialMoveList = board.getAllMoves();
	CBoard::SpecialMoveListIter moveList = specialMoveList.begin();
	while(moveList != specialMoveList.end())
	{
		nextBoard = board;
		nextBoard.movePawn(*moveList);
		value = minMoveAB(nextBoard,depth+1,alpha,beta);

		if(value > alpha)
		{
			alpha = value;
		}
		if( alpha >= beta ) //oryginal
		//if(beta > alpha)
		{
//			debug_print("odcialem");
         	return beta;
		}

		++moveList;
	}
	return alpha;
}
bool CCPU::toDepth(int depth)
{
	return depth > m_maxDepth;
}
int CCPU::calculateValue(unsigned char type,int index)
{
	int value = 0;
	 if (type == CPawn::WHITE)
	 { 
		if (index >= 4 && index <= 7)
			  value = 7;
			else
			  value = 5;
	 }
	 else if (type == CPawn::BLACK)
	 {
		if (index >= 24 && index <= 27)
			  value = 7;
			else
			  value = 5;
      }
	return value * g_tableWeight[index];
}
int CCPU::calculateValue1(unsigned char type,int index)
{
	return g_tableSimpleWeight[index];
}
int CCPU::eval(CBoard& board)
{
	int playerPower = 0;
	int enemyPower  = 0;
	unsigned char pawnType;
	for(int i = 0; i < 32; i++)
	{
      pawnType = board.getPawn(i).m_type;
	  if (pawnType != CPawn::EMPTY)
	  {
	     if (pawnType == CPawn::WHITE)
		   playerPower += calculateValue1 (pawnType, i);
		 else
		   enemyPower += calculateValue1 (pawnType, i);
	  }
	}
	return playerPower - enemyPower;
}
bool CCPU::haveToStop(CBoard& board,int depth)
{
	return toDepth(depth) || board.hasEnded(); 
}
int CCPU::minMove(CBoard& board,int depth)
{
	if(haveToStop(board,depth))
		return eval(board);
		
	CBoard nextBoard;
	CBoard::SpecialMoveList specialMoveList;
	int value	 = 0;
	int minValue = 0x7FFFFFFF;
	specialMoveList = board.getAllMoves();
	CBoard::SpecialMoveListIter moveList = specialMoveList.begin();
	while(moveList != specialMoveList.end())
	{
		nextBoard = board;
		nextBoard.movePawn(*moveList);
		value = maxMove(nextBoard,depth+1);

		if(value < minValue)
		{
         	minValue = value;
		}
		++moveList;
	}
	return  minValue;
}
int CCPU::maxMove(CBoard& board,int depth)
{
	if(haveToStop(board,depth))
    	return eval(board);

	CBoard nextBoard;
	CBoard::SpecialMoveList specialMoveList;
	int value	 = 0;
	int maxValue = 0x80000000;
	specialMoveList = board.getAllMoves();
	CBoard::SpecialMoveListIter moveList = specialMoveList.begin();
	while(moveList != specialMoveList.end())
	{
		nextBoard = board;
		nextBoard.movePawn(*moveList);
		value = minMove(nextBoard,depth+1);

		if(value > maxValue)
		{
			maxValue = value;
		}

		++moveList;
	}

	return maxValue;
}
