//---------------------------------------------------------------------------

#ifndef BoardH
#define BoardH
//---------------------------------------------------------------------------
#include <vector>
#include <list>
#include <Classes.hpp>
#include "Pawn.h"
using namespace std;


class CBoard
{
	public:
	typedef list<pair<TPoint,TPoint> > MoveList;
	typedef list<pair<TPoint,TPoint> >::iterator MoveListIter;
	typedef list<MoveList> SpecialMoveList;
	typedef list<MoveList>::iterator SpecialMoveListIter;
	
	public:
	CBoard();
	void changePlayer();
	void changePlayer2Player();
	unsigned char getCurrentPlayer();
	bool isPlayerCPU();
	bool isEnemy(int index);
	bool isEnemy(const CPawn& pawn);
	bool isEnemy(const TPoint& point,unsigned char type = CPawn::EMPTY);	
	void resetBoard();

	bool isAttackMove(const pair<TPoint,TPoint>& move);
	TPoint   getVictimPoint(const pair<TPoint,TPoint>& move);	
	CPawn getPawn(int i);
	CPawn getPawn(const TPoint& point);
	void  kickOutPawn(int index);
	void  swapPawns(int from,int to);
	TPoint getNextDiagonallyRight(const TPoint& point);
	TPoint getNextDiagonallyLeft(const TPoint& point);
	TPoint getBackDiagonallyRight(const TPoint& point);
	TPoint getBackDiagonallyLeft(const TPoint& point);

	bool isEmpty(int index);
	bool isEmpty(const TPoint& point);
	bool isEmpty(const CPawn& pawn);
	bool isWhitePawn(int index);
	bool isBlackPawn(int index);

	void movePawn(int from,int to);
	void movePawn(const pair<TPoint,TPoint>& move);
	void movePawn(MoveList& moves);		
	bool isValidMove(int from,int to);
	bool shouldAttack(int index = -1,bool checkOnly = false);
	void generateAttacks(CBoard board,MoveList& moveList);	
	bool mayAttack(int from);
	bool isAttackMove();
	bool getShouldAttack();
	bool hasEnded();
	int countPawns(unsigned char type);

	bool blackAvailable();
	bool whiteAvailable();
	bool blackCanMove();
	bool whiteCanMove();

	SpecialMoveList getAllMoves();
	SpecialMoveList generateMoves(unsigned char type = CPawn::EMPTY,bool onlyCheck = false);


	bool isPlayersPawn(int index);
	bool isCPUsPawn(int index);
	vector<CPawn>& getFields();

	int convertPointToIndex(const TPoint& point);
	TPoint convertIndexToPoint(int i);
	bool isValidPosition(const TPoint& point);
	private:
	class attackClass
	{
		public:
		TPoint attacker;
		TPoint victim;
		TPoint destiny;
	};
	bool  isLevelOk(const TPoint& point);
	bool  isLevelBackOk(const TPoint& point);	
	int getLevelUp();
	int getLevelBack();
    void setUpPawns(unsigned char type,int startLine,int linesAmount);
	unsigned char m_currentPlayer;
	unsigned char m_player;
	unsigned char m_CPU;
	bool 		  m_shouldAttack;
	bool 		  m_attackMove;
	int 		  m_victimIndex;
	vector<CPawn> m_fields;
	MoveList m_attackList;

	int m_whites;
	int m_blacks;
};
#endif
