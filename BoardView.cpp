//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Forms.hpp>
#pragma hdrstop

#include "BoardView.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


CBoardView::CBoardView()
{
	m_fieldSize = 50;
	m_boards.push_back(CBoard());
}
void CBoardView::setForm(TForm* form)
{
	m_form = form;
}
void CBoardView::resizeForm()
{
	m_form->ClientHeight = m_fieldSize * 8;
	m_form->ClientWidth  = m_fieldSize * 8;
}
void CBoardView::paintBoard()
{
	//temporary values for x,y
	int tmpX;
	int tmpY;
	//set proper values canvas elements
	m_form->Canvas->Pen->Color = clBlack;		
	for(int vertical = 0;vertical < 8;vertical++)
	{
		for(int horizontal = 0; horizontal < 8;horizontal++)
		{
			if( isBlackField(TPoint(horizontal,vertical)) )
			{
				if( onSelectedList(TPoint(horizontal,vertical)) )
                	m_form->Canvas->Brush->Color = clGreen;
            	else
					m_form->Canvas->Brush->Color = clBlack;
			}
			else
            	m_form->Canvas->Brush->Color = clWhite;

			tmpX = horizontal*m_fieldSize;
			tmpY = vertical*m_fieldSize;
			m_form->Canvas->Rectangle(toBoardRectangle(TPoint(horizontal,vertical)));			
		}
	}
	int pos = 8 * m_fieldSize;
	for(int i = 1; i < 9;i++)
	{
		m_form->Canvas->TextOutA(pos,(i * m_fieldSize) - 25,IntToStr(i-1));    	
	}
	for(int i = 1; i < 9;i++)
	{
		m_form->Canvas->TextOutA((i * m_fieldSize) - 25,pos,IntToStr(i-1));    	
	}	
}
void CBoardView::paintPawns()
{
	for(int i=0; i < 32; i++)
	{
    	CPawn pawn = m_boards[0].getPawn(i); //always board to paint is first one
		switch(pawn.m_type)
		{
			case CPawn::BLACK:
				m_form->Canvas->Brush->Color = clBlue;
				m_form->Canvas->Ellipse(toBoardRectangle(pawn.m_position));
			break;

		   	case CPawn::WHITE:
				m_form->Canvas->Brush->Color = clRed;
				m_form->Canvas->Ellipse(toBoardRectangle(pawn.m_position));				
			break;
		}
	}	
}
bool CBoardView::isSelected(TPoint point)
{
//	vector<TPoint>::iterator it = find(m_selectedFields.begin(),m_selectedFields.end(),point);
	if(m_selectedFields.empty())
    	return false;
	return m_selectedFields.back() == point;
}
void CBoardView::addSelected(TPoint point)
{
	m_selectedFields.push_back(point);	
}
bool CBoardView::hasSelected()
{
	return !m_selectedFields.empty();
}
TPoint CBoardView::toBoardPoint(TPoint point)
{
	return TPoint(point.x / m_fieldSize, point.y / m_fieldSize);
}
TRect  CBoardView::toBoardRectangle(TPoint point)
{
	int tmpX = point.x*m_fieldSize;
	int tmpY = point.y*m_fieldSize;
	return TRect(tmpX,
				 tmpY,
				 tmpX + m_fieldSize,
				 tmpY + m_fieldSize);
}
void CBoardView::removeSelected()
{
	m_selectedFields.pop_back();		
}
bool CBoardView::isBlackField(TPoint point)
{
	return (point.x + point.y) % 2;
}
TPoint CBoardView::convertIndexToPoint(int i)
{
	int y,x;
	y = i / 4;
	x = (i % 4) * 2 + (y % 2 == 0 ? 1 : 0);
    return TPoint(x*m_fieldSize,y*m_fieldSize);
}
int CBoardView::convertPointToIndex(TPoint point)
{
	return m_boards[0].convertPointToIndex(point);
}
unsigned char CBoardView::checkPawn(TPoint point,unsigned char type)
{
	//TODO: unsecure check	
	return m_boards[0].getPawn(convertPointToIndex(point)).m_type == type;
}
bool CBoardView::isBlackPawn(TPoint point)
{
	return checkPawn(point,CPawn::BLACK);	
}
bool CBoardView::isWhitePawn(TPoint point)
{
	return checkPawn(point,CPawn::WHITE);
}
bool CBoardView::isEmptyField(TPoint point)
{
	return checkPawn(point,CPawn::EMPTY);
}
CBoard& CBoardView::getBoard()
{
	return m_boards.back();
}
void CBoardView::createTmpBoard()
{
	if(m_boards.empty())
	{
		m_boards.push_back(getBoard());
	}	
}
bool CBoardView::isTmpBoard()
{
    return !m_boards.empty();
}
TPoint CBoardView::getSelected()
{
	return m_selectedFields.back();
}
void CBoardView::addBoard(CBoard& board)
{
	m_boards.push_back(board);
}
void CBoardView::mergeBoards()
{
	//merge changes
	CBoard tmpBoard = m_boards.back();
	m_boards.clear();
	m_boards.push_back(tmpBoard);
}
void CBoardView::clearSelected()
{
	m_selectedFields.clear();
}
bool CBoardView::onSelectedList(TPoint point)
{
	vector<TPoint>::iterator it = m_selectedFields.begin();
	for(; it != m_selectedFields.end(); ++it)
		if(*it == point)
			return true; 
	return false;
}
void CBoardView::showVerdict()
{
	if( getBoard().countPawns(CPawn::BLACK) > getBoard().countPawns(CPawn::WHITE) )
	{
		m_form->Caption = "Black won!!!";
	}
	else if(getBoard().countPawns(CPawn::BLACK) < getBoard().countPawns(CPawn::WHITE))
	{
    	m_form->Caption = "White won!!!";
	}
	else
	{
		m_form->Caption = "Remis :P";     	
	}
}
