
//--------------------------------------------
// File : BitAdjacentMatrix.h
//
// Desc : the Application class for adjacent matrix 
//        using the bit
//
// by xiedonghai, 2004,10.
//--------------------------------------------



//## begin module%418B2B360125.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%418B2B360125.cm

//## begin module%418B2B360125.cp preserve=no
//## end module%418B2B360125.cp

//## Module: BitAdjacentMatrix%418B2B360125; Pseudo Package specification
//## Source file: d:\Program Files\Rational\Rose 98i\C++\source\BitAdjacentMatrix.h

#ifndef BitAdjacentMatrix_h
#define BitAdjacentMatrix_h 1

//## begin module%418B2B360125.additionalIncludes preserve=no
//## end module%418B2B360125.additionalIncludes

//## begin module%418B2B360125.includes preserve=yes
//## end module%418B2B360125.includes

//## begin module%418B2B360125.additionalDeclarations preserve=yes
//## end module%418B2B360125.additionalDeclarations


//## begin BitAdjacentMatrix%418B2B360125.preface preserve=yes
//## end BitAdjacentMatrix%418B2B360125.preface

//## Class: BitAdjacentMatrix%418B2B360125
//	这个类的作用是将三维模型点和点之间的连通关系存放到一个
//	bit 为基本单元的连通矩阵当中。
//	设矩阵为M，矩阵的行列的数目相同，都是点的数目。设点数为
//	n , 那么矩阵的大小为n*n.
//	如果点 i  与 点j ( i, j  )之间存在连通关系，就将 M[i][j]
//	的值设置为1。
//	M 的每个元素都申明为一个bit。
//	由于c ，c++中最小的类型为 BYTE,即一个字节，占8个bit位。
//	为了用字节来容纳所有的bit，我们需要将行列的长度转换为字节
//	数。方法是：
//	nBytesOfRow = n / 8 + 1;
//	需要分配的字节数为：  col * nBytesOfRow
//	可以推导出按位存放的行列(i ,j )号与对应的字节的关系为：
//
//	 index = i * nBytesOfRow  + j / 8 ;
//
//	在字节中的偏移值为 j % 8 ;
//
//	要注意的是偏移值是从左往右数得到的。
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n
#include "stdafx.h"
#include "winnt.h"

class BitAdjacentMatrix 
{
  //## begin BitAdjacentMatrix%418B2B360125.initialDeclarations preserve=yes
  //## end BitAdjacentMatrix%418B2B360125.initialDeclarations

  public:
    //## Constructors (generated)
      BitAdjacentMatrix();

      BitAdjacentMatrix(const BitAdjacentMatrix &right);

    //## Destructor (generated)
      ~BitAdjacentMatrix();

    //## Assignment Operation (generated)
      BitAdjacentMatrix & operator=(const BitAdjacentMatrix &right);

    //## Equality Operations (generated)
      int operator==(const BitAdjacentMatrix &right) const;

      int operator!=(const BitAdjacentMatrix &right) const;


    //## Other Operations (specified)
      //## Operation: SetPos%418B301300EA
      //	该函数的作用是将一个字节中指定的位置为1；
      //	其原理是：
      //
      //	1 | 0  =  1;
      //	1 | 1  =  1;
      //	0 | 0  =  0;
      //	0 | 1  =  1;
      //
      //	1 和 0， 1 的或操作都位1。
      //
      //	首先根据行列号来取出该点对应的字节 a
      //	，并计算出偏移的bit数 off 。
      //	然后将128 进行右移操作：
      //	 1000 0000   》off
      //	将1移到需要进行置1操作的位上。
      //
      //	然后对该字节进行位或操作，
      //
      //	a  |=  128 >>off;
      void SetPos (int i, int j);

      //## Operation: IsPosAdjacent%418B305C02CB
      //	该函数的作用则是判断 行列号 对应的位是否为1
      //
      //
      //	利用的原理是：
      //
      //	1 & 1  =  1 ;
      //	1 & 0  =  0 ;
      //
      //	0 & 0 =  0;
      //	0 & 1 =  0;
      //
      //	构造一个字节，将需要检测的那个bit 位置为 1
      //	，其他的设置为0，然后同指定的字节进行 与操作即可。
      //
      //	寻找字节和偏移数量的方法和 SetPos是相同的。
      bool IsPosAdjacent (int i, int j);

      //## Operation: Init%418B319F0074
      //	位矩阵的初始化，需要根据行列的大小来确定字节数，然后将申?
//	氲谋淞咳恐梦?。
      bool Init (LONG wid, LONG hei);

      //## Operation: IsInit%418B7AA9015D
      bool IsInit ();

    // Additional Public Declarations
      //## begin BitAdjacentMatrix%418B2B360125.public preserve=yes
      //## end BitAdjacentMatrix%418B2B360125.public

  protected:
    // Additional Protected Declarations
      //## begin BitAdjacentMatrix%418B2B360125.protected preserve=yes
      //## end BitAdjacentMatrix%418B2B360125.protected

  public:
    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_pBytes%418B2F8A03D3
      const unsigned char* get_m_pBytes () const;
      void set_m_pBytes (unsigned char* value);

      //## Attribute: m_nWid%418B2FC803DF
      const int get_m_nWid () const;
      void set_m_nWid (int value);

      //## Attribute: m_nHei%418B2FD503D7
      const int get_m_nHei () const;
      void set_m_nHei (int value);

      //## Attribute: m_nBytesOfRow%418B5ADE0273
      const int get_m_nBytesOfRow () const;
      void set_m_nBytesOfRow (int value);

    // Additional Private Declarations
      //## begin BitAdjacentMatrix%418B2B360125.private preserve=yes
      //## end BitAdjacentMatrix%418B2B360125.private

  private: //## implementation
    // Data Members for Class Attributes

      //## begin BitAdjacentMatrix::m_pBytes%418B2F8A03D3.attr preserve=no  private: unsigned char* {U} 
      unsigned char* m_pBytes;
      //## end BitAdjacentMatrix::m_pBytes%418B2F8A03D3.attr

      //## begin BitAdjacentMatrix::m_nWid%418B2FC803DF.attr preserve=no  private: int {U} 
      LONG m_nWid;
      //## end BitAdjacentMatrix::m_nWid%418B2FC803DF.attr

      //## begin BitAdjacentMatrix::m_nHei%418B2FD503D7.attr preserve=no  private: int {U} 
      LONG m_nHei;
      //## end BitAdjacentMatrix::m_nHei%418B2FD503D7.attr

      //## begin BitAdjacentMatrix::m_nBytesOfRow%418B5ADE0273.attr preserve=no  private: int {U} 
      LONG m_nBytesOfRow;
      //## end BitAdjacentMatrix::m_nBytesOfRow%418B5ADE0273.attr

    // Additional Implementation Declarations
      //## begin BitAdjacentMatrix%418B2B360125.implementation preserve=yes
      //## end BitAdjacentMatrix%418B2B360125.implementation

};

//## begin BitAdjacentMatrix%418B2B360125.postscript preserve=yes
//## end BitAdjacentMatrix%418B2B360125.postscript

// Class BitAdjacentMatrix 

//## Get and Set Operations for Class Attributes (inline)

inline const unsigned char * BitAdjacentMatrix::get_m_pBytes () const
{
  //## begin BitAdjacentMatrix::get_m_pBytes%418B2F8A03D3.get preserve=no
  return m_pBytes;
  //## end BitAdjacentMatrix::get_m_pBytes%418B2F8A03D3.get
}

inline void BitAdjacentMatrix::set_m_pBytes (unsigned char * value)
{
  //## begin BitAdjacentMatrix::set_m_pBytes%418B2F8A03D3.set preserve=no
  m_pBytes = value;
  //## end BitAdjacentMatrix::set_m_pBytes%418B2F8A03D3.set
}

inline const int BitAdjacentMatrix::get_m_nWid () const
{
  //## begin BitAdjacentMatrix::get_m_nWid%418B2FC803DF.get preserve=no
  return m_nWid;
  //## end BitAdjacentMatrix::get_m_nWid%418B2FC803DF.get
}

inline void BitAdjacentMatrix::set_m_nWid (int value)
{
  //## begin BitAdjacentMatrix::set_m_nWid%418B2FC803DF.set preserve=no
  m_nWid = value;
  //## end BitAdjacentMatrix::set_m_nWid%418B2FC803DF.set
}

inline const int BitAdjacentMatrix::get_m_nHei () const
{
  //## begin BitAdjacentMatrix::get_m_nHei%418B2FD503D7.get preserve=no
  return m_nHei;
  //## end BitAdjacentMatrix::get_m_nHei%418B2FD503D7.get
}

inline void BitAdjacentMatrix::set_m_nHei (int value)
{
  //## begin BitAdjacentMatrix::set_m_nHei%418B2FD503D7.set preserve=no
  m_nHei = value;
  //## end BitAdjacentMatrix::set_m_nHei%418B2FD503D7.set
}

inline const int BitAdjacentMatrix::get_m_nBytesOfRow () const
{
  //## begin BitAdjacentMatrix::get_m_nBytesOfRow%418B5ADE0273.get preserve=no
  return m_nBytesOfRow;
  //## end BitAdjacentMatrix::get_m_nBytesOfRow%418B5ADE0273.get
}

inline void BitAdjacentMatrix::set_m_nBytesOfRow (int value)
{
  //## begin BitAdjacentMatrix::set_m_nBytesOfRow%418B5ADE0273.set preserve=no
  m_nBytesOfRow = value;
  //## end BitAdjacentMatrix::set_m_nBytesOfRow%418B5ADE0273.set
}

//## begin module%418B2B360125.epilog preserve=yes
//## end module%418B2B360125.epilog


#endif
