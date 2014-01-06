//---------------------------------------------------------------------------

#include <vcl.h>
#include <algorithm>
#include <functional>
#pragma hdrstop

#include "Board.h"
#include "Pawn.h"
#include "main.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#define debug_print(x) Form1->Memo1->Lines->Add(x)
class PawnSearch : public std::unary_function<CPawn,void>
{
	public:
	
	PawnSearch():found(false),index(0){}	
	void operator()(CPawn& pawn)
	{
		if(pawn.m_position == m_point)
			found = true;
		if(!found)
			index++;
	}
	int getIndex(){return (found)?index:-1;}
	void setPoint(TPoint point){m_point = point;}

	private:
		bool found;
		int index;
		TPoint m_point;
	
};

CBoard::CBoard()
{
	resetBoard();	
}
void CBoard::changePlayer()
{
	m_currentPlayer = (m_currentPlayer == CPawn::BLACK)?CPawn::WHITE:CPawn::BLACK;	
}
void CBoard::changePlayer2Player()
{
    m_currentPlayer =  m_player;
}
unsigned char CBoard::getCurrentPlayer()
{
	return m_currentPlayer;
}
void CBoard::resetBoard()
{
	m_whites = 12;
	m_blacks = 12;
	m_currentPlayer = CPawn::BLACK;
	m_player 		= CPawn::BLACK;
	m_CPU			= CPawn::WHITE;
	setUpPawns(CPawn::BLACK,0,3);
	setUpPawns(CPawn::EMPTY,3,2);
	setUpPawns(CPawn::WHITE,5,8);		
}
void CBoard::setUpPawns(unsigned char type,int startLine,int linesAmount)
{
	int pos;
	for(int line = startLine;line < startLine+linesAmount;++line)
	{
    	pos = (line % 2 == 0)?1:0;
		for(int index = 0;index < 4;index++)
		{
			//debug_print(Format("(%d,%d)",OPENARRAY(TVarRec,(pos,line))) );
			m_fields.push_back( CPawn(type,TPoint(pos,line)) );
			pos+=2;			
		}
	}
}
CPawn CBoard::getPawn(int i)
{
	return m_fields[i];	
}
void CBoard::movePawn(int from,int to)
{
	swapPawns(from,to);
	if(isAttackMove())
	{
       m_attackMove = false;
	   kickOutPawn(m_victimIndex);		
	}
}
void CBoard::kickOutPawn(int index)
{
	if(m_fields[index].m_type == CPawn::WHITE)
		m_whites--;
	else if(m_fields[index].m_type == CPawn::BLACK)
    	m_blacks--;
	m_fields[index].setType(CPawn::EMPTY);
	//TODO: set value fot empy pawn
}
void CBoard::swapPawns(int from,int to)
{
	CPawn tmp;
	tmp = m_fields[from];
	m_fields[from].m_type  =  m_fields[to].m_type;
	m_fields[from].m_value =  m_fields[to].m_value;
	m_fields[to].m_type    = tmp.m_type;
	m_fields[to].m_value   = tmp.m_value;
}
vector<CPawn>& CBoard::getFields()
{
	return m_fields; 	
}
bool CBoard::isValidMove(int from,int to)
{
	m_attackMove = false;
	//does field is empty?
	if(!isEmpty(to))
		return false;

	//check moves for normal pawns	
	if( isBlackPawn(from) || isWhitePawn(from) )
	{
		CPawn fromPawn = getPawn(from);
		CPawn toPawn   = getPawn(to);
		//normal move ?... level up
		int level = abs(toPawn.m_position.y - fromPawn.m_position.y);
		if(level == 1) //normal move ..shouldn't appear when user had to attack
		{
			//check length of horizontal move
			int horizontalMoveLength = abs((toPawn.m_position.x - fromPawn.m_position.x));
			if( horizontalMoveLength == 1 )
			{
				if(shouldAttack(-1))
				{
					ShowMessage("You must attack!!!");                	
					return false;
				}
				return true;
			}
		}
		//attack move?
		if(level == 2)
		{
			TPoint tmpPawnPosition; //pawn who should be enemy
			tmpPawnPosition.y = fromPawn.m_position.y + getLevelUp() ;//for sure level down/up than current pos
			tmpPawnPosition.x = (fromPawn.m_position.x + toPawn.m_position.x) / 2;
			m_attackMove  = true;

//			if(isEnemy(getPawn(tmpPawnPosition)))
				TPoint p = getVictimPoint(make_pair<TPoint,TPoint>(fromPawn.m_position,toPawn.m_position));
				m_victimIndex = convertPointToIndex(p);
			if(isEnemy(p))
				return true;
			return false;
		}
	}

	//TODO: check moves for QUEENs
	return false;			
}
bool CBoard::isEmpty(int index)
{
	return getPawn(index).m_type == CPawn::EMPTY;	
}
bool CBoard::isPlayersPawn(int index)
{
	//hehe pawn can be also empty ;)
	//TODO: do it!!!
	return getPawn(index).m_type == m_player;
}
bool CBoard::isCPUsPawn(int index)
{
	return !isPlayersPawn(index);
}
bool CBoard::isWhitePawn(int index)
{
	return CPawn::WHITE == getPawn(index).m_type;	
}
bool CBoard::isBlackPawn(int index)
{
	return CPawn::BLACK == getPawn(index).m_type;
}
bool CBoard::shouldAttack(int index,bool checkOnly)
{
   m_shouldAttack = false;
   m_attackList.clear();
   attackClass tmpAttackClass;
   int i = (index >=0)?index:0;
   int count = (index >=0)?index+1:32;
   for(; i < count; ++i)
   {
		tmpAttackClass.attacker = getPawn(i).m_position;
		//check whether we are interested of this pawn
		if( getPawn(i).m_type != getCurrentPlayer())
					continue;
		if( index != -1 && i != index)
					continue;

        //PW
		tmpAttackClass.victim = getNextDiagonallyRight(tmpAttackClass.attacker);
		if(isValidPosition(tmpAttackClass.victim) && isEnemy(tmpAttackClass.victim))
		{
			//attack will be possible if next diagonally field is empty
			tmpAttackClass.destiny = getNextDiagonallyRight(tmpAttackClass.victim);
			if(isValidPosition(tmpAttackClass.destiny) && isEmpty(tmpAttackClass.destiny))
			{
				if(checkOnly)
					return true;
				m_shouldAttack = true;
				m_attackList.push_back(
									  make_pair<TPoint,TPoint>(
												tmpAttackClass.attacker,
												tmpAttackClass.destiny)
												);				
			}
		}		

		//PZ
		tmpAttackClass.victim = getNextDiagonallyLeft(tmpAttackClass.attacker);
		if(isValidPosition(tmpAttackClass.victim) && isEnemy(tmpAttackClass.victim))
		{
			//attack will be possible if next diagonally field is empty
			tmpAttackClass.destiny = getNextDiagonallyLeft(tmpAttackClass.victim);
			if(isValidPosition(tmpAttackClass.destiny) && isEmpty(tmpAttackClass.destiny))
			{
				if(checkOnly)
					return true;
				m_shouldAttack = true;				
				m_attackList.push_back(
									  make_pair<TPoint,TPoint>(
												tmpAttackClass.attacker,
												tmpAttackClass.destiny)
												);
			}
		}					

		//PdZ
		tmpAttackClass.victim = getBackDiagonallyRight(tmpAttackClass.attacker);
		if(isValidPosition(tmpAttackClass.victim) && isEnemy(tmpAttackClass.victim))
		{
			//attack will be possible if next diagonally field is empty
			tmpAttackClass.destiny = getBackDiagonallyRight(tmpAttackClass.victim);
			if(isValidPosition(tmpAttackClass.destiny) && isEmpty(tmpAttackClass.destiny))
			{
				if(checkOnly)
					return true;
				m_shouldAttack = true;				
				m_attackList.push_back(
									  make_pair<TPoint,TPoint>(
												tmpAttackClass.attacker,
												tmpAttackClass.destiny)
												);
			}
		}					

		//PdW
		tmpAttackClass.victim = getBackDiagonallyLeft(tmpAttackClass.attacker);
		if(isValidPosition(tmpAttackClass.victim) && isEnemy(tmpAttackClass.victim))
		{
			//attack will be possible if next diagonally field is empty
			tmpAttackClass.destiny = getBackDiagonallyLeft(tmpAttackClass.victim);
			if(isValidPosition(tmpAttackClass.destiny) && isEmpty(tmpAttackClass.destiny))
			{
				if(checkOnly)
					return true;
				m_shouldAttack = true;				
				m_attackList.push_back(
									  make_pair<TPoint,TPoint>(
												tmpAttackClass.attacker,
												tmpAttackClass.destiny)
												);
			}
		}					
		
		//TODO: do checks for QUEENS
   }		
   return m_shouldAttack;
}
bool CBoard::mayAttack(int from)
{
	return false;
}
bool CBoard::isEnemy(int index)
{
	return getPawn(index).m_type != getCurrentPlayer() &&
		   !isEmpty(index)
			;
}
bool CBoard::isEnemy(const CPawn& pawn)
{
	return pawn.m_type != getCurrentPlayer() &&
			!isEmpty(pawn);
}
bool CBoard::isEnemy(const TPoint& point,unsigned char type)
{
	if(type != CPawn::EMPTY)
	{
		return getPawn(point).m_type != type && !isEmpty(point);
	}
	else
	{
		return isEnemy(getPawn(point));
	}
}
bool CBoard::isEmpty(const CPawn& pawn)
{
	return pawn.m_type == CPawn::EMPTY;
}
bool CBoard::isEmpty(const TPoint& point)
{
	return isEmpty(getPawn(point));
}
TPoint CBoard::getNextDiagonallyRight(const TPoint& point)
{
	//check bounds
	if(point.x == 7 || !isLevelOk(point))
		return TPoint(-1,-1);

	return TPoint(point.x + 1,point.y + getLevelUp());
}
TPoint CBoard::getNextDiagonallyLeft(const TPoint& point)
{
	//check bounds
	if(point.x == 0 || !isLevelOk(point))
		return TPoint(-1,-1);
	return TPoint(point.x - 1,point.y + getLevelUp());
}
TPoint CBoard::getBackDiagonallyRight(const TPoint& point)
{
	if(point.x == 7 || !isLevelBackOk(point))
		return TPoint(-1,-1);
	return TPoint(point.x + 1,point.y + getLevelBack());				
}
TPoint CBoard::getBackDiagonallyLeft(const TPoint& point)
{
	if(point.x == 0 || !isLevelBackOk(point))
		return TPoint(-1,-1);
	return TPoint(point.x - 1,point.y + getLevelBack());		
}
int CBoard::getLevelBack()
{
	return (getCurrentPlayer() == CPawn::BLACK)?-1:1;	
}
bool CBoard::isLevelBackOk(const TPoint& point)
{
	int levelBack = getLevelBack();
	if( (point.y + levelBack) < 0 || 
		(point.y + levelBack) > 7
	   )
	   return false;

	return true;
}
bool CBoard::isLevelOk(const TPoint& point)
{
	int levelUp = getLevelUp();
	if( (point.y + levelUp) < 0 || 
		(point.y + levelUp) > 7
	   )
	   return false;

	return true;
}
int CBoard::getLevelUp()//level is +1 or -1 depends on current player
{
	return (getCurrentPlayer() == CPawn::BLACK)?1:-1;
}
bool CBoard::getShouldAttack()
{
	return m_shouldAttack;
}
bool CBoard::isAttackMove()
{
	return m_attackMove;
}
int CBoard::convertPointToIndex(const TPoint& point)
{
	PawnSearch p;
	p.setPoint(point);
	p = for_each(m_fields.begin(),m_fields.end(),p);
	return p.getIndex();
}
TPoint CBoard::convertIndexToPoint(int i)
{
	return TPoint(666,666);
}
CPawn CBoard::getPawn(const TPoint& point)
{
	return getPawn(convertPointToIndex(point));
}
bool CBoard::isPlayerCPU()
{
	return getCurrentPlayer() == CPawn::WHITE;
}
CBoard::SpecialMoveList CBoard::generateMoves(unsigned char type,bool onlyCheck)
{
	SpecialMoveList specialMoveList;
	MoveList moveList;
	TPoint second;
	for(int i = 0; i < 32; i++)
	{
		if(type != CPawn::EMPTY)
		{
			if(getPawn(i).m_type != type)
					continue;
		}else if(getPawn(i).m_type != getCurrentPlayer())
					continue;

		TPoint pawnCords = getPawn(i).m_position;

		second = getNextDiagonallyRight(pawnCords);
		if(isValidPosition(second) && isEmpty(second))
		{
			moveList.push_back(
							  make_pair<TPoint,TPoint>(pawnCords,second)
							  );
			specialMoveList.push_back(moveList);
			moveList.clear();
			if(onlyCheck)
				return specialMoveList;
		}


		second = getNextDiagonallyLeft(pawnCords);
		if(isValidPosition(second) && isEmpty(second))
		{
			moveList.push_back(
							  make_pair<TPoint,TPoint>(pawnCords,second)
							  );
			specialMoveList.push_back(moveList);
			moveList.clear();
			if(onlyCheck)
				return specialMoveList;
		}


	}
	return specialMoveList;
}
CBoard::SpecialMoveList CBoard::getAllMoves()
{
	
	if(shouldAttack())
	{
		SpecialMoveList attackMoveList;
		MoveListIter attack = m_attackList.begin();
		for(; attack != m_attackList.end();++attack)
		{
		   MoveList attacks;
		   attacks.push_back(*attack);
		   generateAttacks(*this,attacks);
		   attackMoveList.push_back(attacks);
		   attacks.clear();
		}
		return attackMoveList;    	
	}
    return generateMoves();
}
void CBoard::movePawn(const pair<TPoint,TPoint>& move)
{
	movePawn(convertPointToIndex(move.first),
             convertPointToIndex(move.second)
			);
		if(isAttackMove(move))
		{
        	TPoint p = getVictimPoint(move);
			kickOutPawn(convertPointToIndex(p));
		}			
}
void CBoard::movePawn(MoveList& moves)
{
	MoveListIter move = moves.begin();
	for(; move != moves.end(); move++)
	{
		movePawn(*move);
		if(isAttackMove(*move))
		{
        	const TPoint& p = getVictimPoint(*move);
			kickOutPawn(convertPointToIndex(p));
		}
	}	
	
	//change current player after move
	changePlayer();
}		
void CBoard::generateAttacks(CBoard board,MoveList& moveList)
{
	board.movePawn(moveList.back());
	if(board.shouldAttack( convertPointToIndex(moveList.back().second) ))
	{
		moveList.push_back( board.m_attackList.back() );
		generateAttacks(board,moveList);
	}
}
bool CBoard::hasEnded()
{

	bool a = !blackAvailable();
	bool b = !whiteAvailable();
	bool c = !blackCanMove();
	bool d = !whiteCanMove();
	return a || b || c || d;
}
int CBoard::countPawns(unsigned char type)
{
	if(type == CPawn::WHITE)
		return m_whites;
	else
    	return m_blacks;    	
}
bool CBoard::blackAvailable()
{
	return countPawns(CPawn::BLACK) != 0;
}
bool CBoard::whiteAvailable()
{
	return countPawns(CPawn::WHITE) != 0; 
}
bool CBoard::blackCanMove()
{
	if(getCurrentPlayer() == CPawn::BLACK)
		return shouldAttack(-1,true) || generateMoves(CPawn::BLACK,true).size() != 0;
	//we need to change player for a sec
	changePlayer();
 	bool moves   = generateMoves(CPawn::BLACK,true).size() != 0;
	bool attacks = shouldAttack(-1,true);
	changePlayer();
	return attacks || moves;
}
bool CBoard::whiteCanMove()
{	
	if(getCurrentPlayer() == CPawn::WHITE)
		return shouldAttack(-1,true) || generateMoves(CPawn::WHITE,true).size() != 0;
	//we need to change player for a sec
	changePlayer();
 	bool moves   = generateMoves(CPawn::WHITE,true).size() != 0;
	bool attacks = shouldAttack(-1,true);
	changePlayer();
	return attacks || moves;
}
bool CBoard::isAttackMove(const pair<TPoint,TPoint>& move)
{
	return abs(move.first.y - move.second.y) == 2;
}
TPoint  CBoard::getVictimPoint(const pair<TPoint,TPoint>& move)
{
	int x = (move.first.x + move.second.x)/2;
	int y;
	if(getCurrentPlayer() == CPawn::BLACK )
	{
		if((move.first.y - move.second.y) < 0)
		{
			y = move.first.y + getLevelUp();
		}else{
         	y = move.first.y + getLevelBack();
		}
	}
	if(getCurrentPlayer() == CPawn::WHITE )
	{
		if((move.first.y - move.second.y) < 0)
		{
			y = move.first.y + getLevelBack();
		}else{
         	y = move.first.y + getLevelUp();
		}
	}	

	return TPoint(x,y);	  
}
bool CBoard::isValidPosition(const TPoint& point)
{
	return point.x != -1;
}
