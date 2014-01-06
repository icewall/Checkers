//---------------------------------------------------------------------------

#ifndef PawnH
#define PawnH
//---------------------------------------------------------------------------
#include <Types.hpp>
class CPawn
{
	public:
	CPawn();
	CPawn(unsigned char type);
	CPawn(unsigned char type,TPoint position);	
	CPawn(unsigned char type,unsigned char value);
	void setType(unsigned char type);
	void setValue(unsigned char value);

	/* types of pawn*/
	static const unsigned char WHITE = 0;
	static const unsigned char BLACK = 1;
	static const unsigned char EMPTY = 2;
	static const unsigned char WHITE_QUEEN = 3;
	static const unsigned char BLACK_QUEEN = 4;

	TPoint m_position;
	unsigned char m_type;
	unsigned char m_value;

};
#endif
