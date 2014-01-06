//---------------------------------------------------------------------------
#ifndef BoardViewH
#define BoardViewH
//---------------------------------------------------------------------------
#include <vector>
#include "Board.h"
using namespace std;


class CBoardView
{
	public:
	CBoardView();
	void paintBoard();
	void paintPawns();
	void resizeForm();
	void setForm(TForm* form);
	CBoard& getBoard();
	void addBoard(CBoard& board);
	void mergeBoards();
	
	void addSelected(TPoint point);
	void removeSelected();
	bool isSelected(TPoint point);
	bool onSelectedList(TPoint point);
	bool hasSelected();
	bool isBlackField(TPoint point);
	TPoint getSelected();
	void clearSelected();
	void showVerdict();

	unsigned char checkPawn(TPoint point,unsigned char type);
	bool isBlackPawn(TPoint point);
	bool isWhitePawn(TPoint point);
	bool isEmptyField(TPoint point);
	
	TPoint toBoardPoint(TPoint point);	
	TRect  toBoardRectangle(TPoint point);
	int    convertPointToIndex(TPoint point);

    vector<TPoint> m_selectedFields;

	void createTmpBoard();
	bool isTmpBoard();
	vector<CBoard> m_boards;
	TPoint convertIndexToPoint(int index);	
	private:

	int m_fieldSize;
	TForm *m_form;

};
#endif
