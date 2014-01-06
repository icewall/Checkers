//---------------------------------------------------------------------------

#ifndef cpuH
#define cpuH
//---------------------------------------------------------------------------
#include "board.h"
class CCPU
{
	public:
	void play();
	void setBoard(CBoard* board);
	int getMaxDepth();
	
	private:
	CBoard::MoveList minimax(CBoard& board);
	bool toDepth(int depth);
	int minMoveAB(CBoard& board,int depth,int alpha,int beta);
	int maxMoveAB(CBoard& board,int depth,int alpha,int beta);
	int minMove(CBoard& board,int depth);
	int maxMove(CBoard& board,int depth);	
	bool haveToStop(CBoard& board,int depth);
	int calculateValue(unsigned char type,int index);
	int calculateValue1(unsigned char type,int index);	
	int eval(CBoard& board);
	int m_maxDepth;
	CBoard *m_board;
	bool m_alphaBeta;
};
#endif
