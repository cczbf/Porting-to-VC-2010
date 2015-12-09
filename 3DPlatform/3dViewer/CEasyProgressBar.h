//## begin module%41859D7D00C5.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%41859D7D00C5.cm

//## begin module%41859D7D00C5.cp preserve=no
//## end module%41859D7D00C5.cp

//## Module: CEasyProgressBar%41859D7D00C5; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CEasyProgressBar.h

#ifndef CEasyProgressBar_h
#define CEasyProgressBar_h 1

//## begin module%41859D7D00C5.additionalIncludes preserve=no
//## end module%41859D7D00C5.additionalIncludes

//## begin module%41859D7D00C5.includes preserve=yes
#include "progressbar.h"
//## end module%41859D7D00C5.includes

// 3dViewer
#include "3dViewer.h"
//## begin module%41859D7D00C5.declarations preserve=no
//## end module%41859D7D00C5.declarations

//## begin module%41859D7D00C5.additionalDeclarations preserve=yes
//## end module%41859D7D00C5.additionalDeclarations


//## begin CEasyProgressBar%41859D0302DD.preface preserve=yes
//## end CEasyProgressBar%41859D0302DD.preface

//## Class: CEasyProgressBar%41859D0302DD
//		CProgressBar * pbar = NULL;
//	    pbar = new CProgressBar(_T("Setting Adjacent
//	Matrix"), 100, 100, TRUE);
//	    pbar->SetText("Setting Adjacent Matrix...");
//	    float step = 0;
//		float prog = 0;
//		step = (float)100.0/(float)(row);
//
//		for ( int i=0; i <row; i++)
//		{
//			prog += step;
//			pbar->SetPos((int)prog);
//
//			for (int  j=0; j<col; j++)
//			{
//				m_pCAdjancentMtrx[i][j] = 0;
//			}
//		}
//		delete pbar;
//## Category: 3dViewer::ShellClasses::Progress Bar%41859CCE007E
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CEasyProgressBar 
{
  //## begin CEasyProgressBar%41859D0302DD.initialDeclarations preserve=yes
  //## end CEasyProgressBar%41859D0302DD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: CEasyProgressBar%41859F730185
      CEasyProgressBar (char* strTitle, int nScale);

    //## Destructor (specified)
      //## Operation: ~CEasyProgressBar%4185B118027B
      ~CEasyProgressBar ();


    //## Other Operations (specified)
      //## Operation: UpdateProgress%41859ED80024
      void UpdateProgress ();

    // Data Members for Associations

      //## Association: 3dViewer::ShellClasses::Progress Bar::<unnamed>%4185B0920336
      //## Role: CEasyProgressBar::m_pCProgressBar%4185B093034C
      //## begin CEasyProgressBar::m_pCProgressBar%4185B093034C.role preserve=no  public: CProgressBar { -> RHgAN}
      CProgressBar *m_pCProgressBar;
      //## end CEasyProgressBar::m_pCProgressBar%4185B093034C.role

    // Additional Public Declarations
      //## begin CEasyProgressBar%41859D0302DD.public preserve=yes
      //## end CEasyProgressBar%41859D0302DD.public

  protected:
    // Additional Protected Declarations
      //## begin CEasyProgressBar%41859D0302DD.protected preserve=yes
      //## end CEasyProgressBar%41859D0302DD.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: step%41859FE803BE
      //## begin CEasyProgressBar::step%41859FE803BE.attr preserve=no  private: float {UA} 0.0
      float step;
      //## end CEasyProgressBar::step%41859FE803BE.attr

      //## Attribute: prog%4185A0B9029D
      //## begin CEasyProgressBar::prog%4185A0B9029D.attr preserve=no  private: float {UA} 0.0
      float prog;
      //## end CEasyProgressBar::prog%4185A0B9029D.attr

    // Additional Private Declarations
      //## begin CEasyProgressBar%41859D0302DD.private preserve=yes
      //## end CEasyProgressBar%41859D0302DD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CEasyProgressBar%41859D0302DD.implementation preserve=yes
      //## end CEasyProgressBar%41859D0302DD.implementation

};

//## begin CEasyProgressBar%41859D0302DD.postscript preserve=yes
//## end CEasyProgressBar%41859D0302DD.postscript

// Class CEasyProgressBar 


//## Other Operations (inline)
inline void CEasyProgressBar::UpdateProgress ()
{
  //## begin CEasyProgressBar::UpdateProgress%41859ED80024.body preserve=yes
	
		this->prog += step;
		this->m_pCProgressBar->SetPos((int)(this->prog));
	MSG msg;
	HACCEL hAccelTable=NULL;
 //	if (GetMessage(&msg, NULL, 0, 0)) 
	if ( PeekMessage(
						&msg,         // message information
						NULL,           // handle to window
						0,  // first message
						0,  // last message
						PM_REMOVE      // removal options
		)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
		
  //## end CEasyProgressBar::UpdateProgress%41859ED80024.body
}

//## begin module%41859D7D00C5.epilog preserve=yes
//## end module%41859D7D00C5.epilog


#endif
