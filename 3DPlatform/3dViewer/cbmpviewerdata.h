//## begin module%364BA69102A8.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%364BA69102A8.cm

//## begin module%364BA69102A8.cp preserve=no
//## end module%364BA69102A8.cp

//## Module: CBmpViewerData%364BA69102A8; Pseudo Package specification
//## Subsystem: bmpviewer::bmpviewer\:\:Core%387016280189
//## Source file: d:\desktop\halftonetest\bmpviewer\CBmpViewerData.h

#ifndef CBmpViewerData_h
#define CBmpViewerData_h 1

//## begin module%364BA69102A8.additionalIncludes preserve=no
//## end module%364BA69102A8.additionalIncludes

//## begin module%364BA69102A8.includes preserve=yes
//## end module%364BA69102A8.includes

// cdib
#include "cdib.h"
// CRawImage16
#include "CRawImage16.h"
//## begin module%364BA69102A8.additionalDeclarations preserve=yes
//## end module%364BA69102A8.additionalDeclarations


//## begin CBmpViewerData%364BA69102A8.preface preserve=yes
//## end CBmpViewerData%364BA69102A8.preface

//## Class: CBmpViewerData%364BA69102A8
//## Category: bmpviewer::Core%369F6C81018D
//## Subsystem: bmpviewer::bmpviewer\:\:Core%387016280189
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%387BE27102B1;CDIB { -> }
//## Uses: <unnamed>%388D6C98032C;CRawImage16 { -> }

class CBmpViewerData 
{
  //## begin CBmpViewerData%364BA69102A8.initialDeclarations preserve=yes
  //## end CBmpViewerData%364BA69102A8.initialDeclarations

  public:
    //## Constructors (generated)
      CBmpViewerData();

    //## Destructor (generated)
      ~CBmpViewerData();


    //## Other Operations (specified)
      //## Operation: SetDIBHandle%364BA69300E0; C++
      HANDLE SetDIBHandle (HANDLE hDib = NULL, int w = 0, int h = 0);

      //## Operation: IsCursorInsideImage%364F7090029E
      BOOL IsCursorInsideImage (int x = 32767, int y = 32767);

      //## Operation: SetSelFlag%364F71100398
      BOOL SetSelFlag (BOOL bSelected = TRUE);

      //## Operation: GetSelFlag%364F715003C0
      BOOL GetSelFlag ();

      //## Operation: SetNewPosition%364F975A017C
      void SetNewPosition (int x0 = 0, int y0 = 0, int w0 = 0, int h0 = 0, int x1 = 0, int y1 = 0, int w1 = 0, int h1 = 0);

      //## Operation: GetNewPosition%364F987C0302
      void GetNewPosition (int& x0, int& y0, int& w0, int& h0, int& x1, int& y1, int& w1, int& h1);

      //## Operation: GetDIBHandle%3682E1DB0302
      HANDLE GetDIBHandle ();

      //## Operation: CopyDibRawHandle%387BD69F0048
      HANDLE CopyDibRawHandle (int* w, int* h);

      //## Operation: Undo%38AA6315004B
      void Undo ();

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_x0%364BA69300F0
      const int& get_m_x0 () const;

      //## Attribute: m_y0%364BA69300F1
      const int& get_m_y0 () const;

      //## Attribute: m_w0%364BA69300F2
      const int& get_m_w0 () const;

      //## Attribute: m_h0%364BA69300F3
      const int& get_m_h0 () const;

      //## Attribute: m_x1%364BA69300F4
      const int& get_m_x1 () const;

      //## Attribute: m_y1%364BA69300F5
      const int& get_m_y1 () const;

      //## Attribute: m_w1%364BA69300F6
      const int& get_m_w1 () const;

      //## Attribute: m_h1%364BA69300F7
      //	Additional Public Declarations
      const int& get_m_h1 () const;

      //## Attribute: m_hDib%364BA7B20028
      const HANDLE get_m_hDib () const;
      void set_m_hDib (HANDLE value);

    // Data Members for Class Attributes

      //## begin CBmpViewerData::m_x0%364BA69300F0.attr preserve=no  public: int {VA} 0
      int m_x0;
      //## end CBmpViewerData::m_x0%364BA69300F0.attr

      //## begin CBmpViewerData::m_y0%364BA69300F1.attr preserve=no  public: int {VA} 0
      int m_y0;
      //## end CBmpViewerData::m_y0%364BA69300F1.attr

      //## begin CBmpViewerData::m_w0%364BA69300F2.attr preserve=no  public: int {VA} 0
      int m_w0;
      //## end CBmpViewerData::m_w0%364BA69300F2.attr

      //## begin CBmpViewerData::m_h0%364BA69300F3.attr preserve=no  public: int {VA} 0
      int m_h0;
      //## end CBmpViewerData::m_h0%364BA69300F3.attr

      //## begin CBmpViewerData::m_x1%364BA69300F4.attr preserve=no  public: int {VA} 0
      int m_x1;
      //## end CBmpViewerData::m_x1%364BA69300F4.attr

      //## begin CBmpViewerData::m_y1%364BA69300F5.attr preserve=no  public: int {VA} 0
      int m_y1;
      //## end CBmpViewerData::m_y1%364BA69300F5.attr

      //## begin CBmpViewerData::m_w1%364BA69300F6.attr preserve=no  public: int {VA} 0
      int m_w1;
      //## end CBmpViewerData::m_w1%364BA69300F6.attr

      //## begin CBmpViewerData::m_h1%364BA69300F7.attr preserve=no  public: int {VA} 0
      int m_h1;
      //## end CBmpViewerData::m_h1%364BA69300F7.attr

      //## begin CBmpViewerData::m_hDib%364BA7B20028.attr preserve=no  public: HANDLE {UA} NULL
      HANDLE m_hDib;
      //## end CBmpViewerData::m_hDib%364BA7B20028.attr

    // Additional Public Declarations
      //## begin CBmpViewerData%364BA69102A8.public preserve=yes
      //## end CBmpViewerData%364BA69102A8.public

  protected:
    // Additional Protected Declarations
      //## begin CBmpViewerData%364BA69102A8.protected preserve=yes
      //## end CBmpViewerData%364BA69102A8.protected

  private:

    //## Other Operations (specified)
      //## Operation: SetUndoHandle%38AA62A3037C
      void SetUndoHandle (HANDLE hDib);

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_bSelected%364F963C01AE
      const BOOL get_m_bSelected () const;
      void set_m_bSelected (BOOL value);

      //## Attribute: m_hDibUndo%38AA624203D7
      const HANDLE get_m_hDibUndo () const;
      void set_m_hDibUndo (HANDLE value);

    // Data Members for Class Attributes

      //## begin CBmpViewerData::m_bSelected%364F963C01AE.attr preserve=no  private: BOOL {UA} 
      BOOL m_bSelected;
      //## end CBmpViewerData::m_bSelected%364F963C01AE.attr

      //## begin CBmpViewerData::m_hDibUndo%38AA624203D7.attr preserve=no  private: HANDLE {UA} NULL
      HANDLE m_hDibUndo;
      //## end CBmpViewerData::m_hDibUndo%38AA624203D7.attr

    // Additional Private Declarations
      //## begin CBmpViewerData%364BA69102A8.private preserve=yes
      //## end CBmpViewerData%364BA69102A8.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CBmpViewerData%364BA69102A8.implementation preserve=yes
      //## end CBmpViewerData%364BA69102A8.implementation

};

//## begin CBmpViewerData%364BA69102A8.postscript preserve=yes
//## end CBmpViewerData%364BA69102A8.postscript

// Class CBmpViewerData 

//## Get and Set Operations for Class Attributes (inline)

inline const int& CBmpViewerData::get_m_x0 () const
{
  //## begin CBmpViewerData::get_m_x0%364BA69300F0.get preserve=no
  return m_x0;
  //## end CBmpViewerData::get_m_x0%364BA69300F0.get
}

inline const int& CBmpViewerData::get_m_y0 () const
{
  //## begin CBmpViewerData::get_m_y0%364BA69300F1.get preserve=no
  return m_y0;
  //## end CBmpViewerData::get_m_y0%364BA69300F1.get
}

inline const int& CBmpViewerData::get_m_w0 () const
{
  //## begin CBmpViewerData::get_m_w0%364BA69300F2.get preserve=no
  return m_w0;
  //## end CBmpViewerData::get_m_w0%364BA69300F2.get
}

inline const int& CBmpViewerData::get_m_h0 () const
{
  //## begin CBmpViewerData::get_m_h0%364BA69300F3.get preserve=no
  return m_h0;
  //## end CBmpViewerData::get_m_h0%364BA69300F3.get
}

inline const int& CBmpViewerData::get_m_x1 () const
{
  //## begin CBmpViewerData::get_m_x1%364BA69300F4.get preserve=no
  return m_x1;
  //## end CBmpViewerData::get_m_x1%364BA69300F4.get
}

inline const int& CBmpViewerData::get_m_y1 () const
{
  //## begin CBmpViewerData::get_m_y1%364BA69300F5.get preserve=no
  return m_y1;
  //## end CBmpViewerData::get_m_y1%364BA69300F5.get
}

inline const int& CBmpViewerData::get_m_w1 () const
{
  //## begin CBmpViewerData::get_m_w1%364BA69300F6.get preserve=no
  return m_w1;
  //## end CBmpViewerData::get_m_w1%364BA69300F6.get
}

inline const int& CBmpViewerData::get_m_h1 () const
{
  //## begin CBmpViewerData::get_m_h1%364BA69300F7.get preserve=no
  return m_h1;
  //## end CBmpViewerData::get_m_h1%364BA69300F7.get
}

inline const HANDLE CBmpViewerData::get_m_hDib () const
{
  //## begin CBmpViewerData::get_m_hDib%364BA7B20028.get preserve=no
  return m_hDib;
  //## end CBmpViewerData::get_m_hDib%364BA7B20028.get
}

inline void CBmpViewerData::set_m_hDib (HANDLE value)
{
  //## begin CBmpViewerData::set_m_hDib%364BA7B20028.set preserve=no
  m_hDib = value;
  //## end CBmpViewerData::set_m_hDib%364BA7B20028.set
}

inline const BOOL CBmpViewerData::get_m_bSelected () const
{
  //## begin CBmpViewerData::get_m_bSelected%364F963C01AE.get preserve=no
  return m_bSelected;
  //## end CBmpViewerData::get_m_bSelected%364F963C01AE.get
}

inline void CBmpViewerData::set_m_bSelected (BOOL value)
{
  //## begin CBmpViewerData::set_m_bSelected%364F963C01AE.set preserve=no
  m_bSelected = value;
  //## end CBmpViewerData::set_m_bSelected%364F963C01AE.set
}

inline const HANDLE CBmpViewerData::get_m_hDibUndo () const
{
  //## begin CBmpViewerData::get_m_hDibUndo%38AA624203D7.get preserve=no
  return m_hDibUndo;
  //## end CBmpViewerData::get_m_hDibUndo%38AA624203D7.get
}

inline void CBmpViewerData::set_m_hDibUndo (HANDLE value)
{
  //## begin CBmpViewerData::set_m_hDibUndo%38AA624203D7.set preserve=no
  m_hDibUndo = value;
  //## end CBmpViewerData::set_m_hDibUndo%38AA624203D7.set
}

//## begin module%364BA69102A8.epilog preserve=yes
//## end module%364BA69102A8.epilog


#endif
