//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
	m_boardView.setForm(this);	
	//m_boardView.resizeForm();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormPaint(TObject *Sender)
{
	m_boardView.paintBoard();
	m_boardView.paintPawns();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClick(TObject *Sender)
{
	TPoint point = m_boardView.toBoardPoint(CalcCursorPos());
	if(m_boardView.isBlackField(point))
	{
		if(m_boardView.isSelected(point))
			m_boardView.removeSelected();
		else
		{
			if(m_boardView.hasSelected())
			{//moze to bedzie ruch?
				CBoard& tmpBoard = m_boardView.getBoard();
				int to   = m_boardView.convertPointToIndex(point);
				int from = m_boardView.convertPointToIndex(m_boardView.getSelected());
				//check move validity
				if(tmpBoard.isValidMove(from,to))
				{
                	//it's attack move,,let's see whether player can attack again
					if(tmpBoard.isAttackMove())
					{
						//maybe he can attack again
						CBoard tmpCloneBoard = tmpBoard;
						tmpCloneBoard.movePawn(from,to);
						m_boardView.addBoard(tmpCloneBoard);
						if(tmpCloneBoard.shouldAttack(to))
						{
							m_boardView.addSelected(point);
						}
						else
						{//end of attack moves let's apply changes
							
							m_boardView.mergeBoards();
							m_boardView.clearSelected();
							m_boardView.getBoard().changePlayer();															                        	
						}
					}
					else
					{//normal move ? so let's do it
						tmpBoard.movePawn(from,to);
						TPoint fromP,toP;
						fromP = m_boardView.toBoardPoint(m_boardView.convertIndexToPoint(from));
						toP   = point;
							Form1->Memo1->Lines->Add(
							Format("[PLAYER] from {%d,%d} to {%d,%d}",OPENARRAY(TVarRec,
							(fromP.x,
							 fromP.y,
							 toP.x,
							 toP.y
																				)))
							);
						m_boardView.clearSelected();
						//another player time
						m_boardView.getBoard().changePlayer();			
					}					 	
				}
			}
			else if(m_boardView.isBlackPawn(point))//jesli nie ma zaznaczonych pol to zaznacz jesli to czarny pionek
			{
            	m_boardView.addSelected(point);
			}
		}
  	  this->Repaint();
	  //maybe it's time for CPU moves?
	  if(m_boardView.getBoard().isPlayerCPU())
	  {
	       this->Cursor = crHourGlass;
		   m_cpu.setBoard(&m_boardView.getBoard());
		   m_cpu.play();
		   m_boardView.getBoard().changePlayer2Player();
		   this->Cursor = crDefault;		   
	  }
	  if(m_boardView.getBoard().hasEnded())
	  {
		 m_boardView.showVerdict();
		 ShowMessage("The end");		      	
	  }
  	  this->Repaint();
	}

}
//---------------------------------------------------------------------------
