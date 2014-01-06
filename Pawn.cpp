//---------------------------------------------------------------------------


#pragma hdrstop

#include "Pawn.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
CPawn::CPawn()
{
}
CPawn::CPawn(unsigned char type)
{
   m_type = type;
}
CPawn::CPawn(unsigned char type,TPoint position)
{
	m_type = type;
	m_position = position;
}
CPawn::CPawn(unsigned char type,unsigned char value)
{
	m_type = type;
	m_value = value;
}
void CPawn::setType(unsigned char type)
{
	m_type = type;
}
void CPawn::setValue(unsigned char value)
{
	m_value = value;
}
