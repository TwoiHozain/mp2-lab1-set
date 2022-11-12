// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len>0)
	{
		BitLen = len;
		MemLen = BitLen / (sizeof(TELEM)*8);
		MemLen += static_cast<int>(MemLen* sizeof(TELEM) * 8<len);
		pMem = new TELEM[MemLen];

		memset(pMem,0,MemLen*sizeof(TELEM));
	}
	else
	{
		throw exception("len may not to be equal or less than zero");
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (int i=0;i< bf.MemLen;i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	if(pMem!=nullptr || pMem==NULL)
		delete[] pMem;
	BitLen = 0;
	MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return ((TELEM)1) << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= BitLen || n < 0)
		throw std::out_of_range("inex of bit out of a range");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= BitLen || n < 0)
		throw std::out_of_range("inex of bit out of a range");
	pMem[GetMemIndex(n)] &= (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen || n < 0)
		throw std::out_of_range("inex of bit out of a range");
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;

	BitLen = bf.BitLen;
	if(MemLen != bf.MemLen )
	{
		TELEM *tmp = new TELEM[bf.MemLen];
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = tmp;
	}
	memcpy(pMem,bf.pMem, MemLen * sizeof(TELEM));
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	for (int i = 0; i < bf.MemLen; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;

	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this==bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int l = BitLen;
	if (l < bf.BitLen)
		l = bf.BitLen;

	TBitField tmp(l);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] |= pMem[i];

	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];

	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int l = BitLen;
	if (l < bf.BitLen)
		l = bf.BitLen;

	TBitField tmp(l);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] |= pMem[i];

	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] &= bf.pMem[i];

	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(*this);

	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = ~tmp.pMem[i];

	if(sizeof(TELEM)*8!=tmp.BitLen)
		tmp.pMem[tmp.MemLen - 1] &= ~((~(TELEM)0)<<(tmp.BitLen));

	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	unsigned short tmp=0,one=1,zero=0;
	int length = bf.GetLength();

	for (size_t i = 0; i < length; i++)
	{
		istr >> tmp;

		if (tmp)
			bf.SetBit(i);
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int size = bf.BitLen;
	for (int i=0; i < size; i++)
		cout << bf.GetBit(i);
	return ostr;
}
