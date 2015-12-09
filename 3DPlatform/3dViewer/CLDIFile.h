//## begin module%40DFDC0D0309.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40DFDC0D0309.cm

//## begin module%40DFDC0D0309.cp preserve=no
//## end module%40DFDC0D0309.cp

//## Module: CLDIFile%40DFDC0D0309; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CLDIFile.h

#ifndef CLDIFile_h
#define CLDIFile_h 1

//## begin module%40DFDC0D0309.additionalIncludes preserve=no
//## end module%40DFDC0D0309.additionalIncludes

//## begin module%40DFDC0D0309.includes preserve=yes
#include "stdafx.h"
//## end module%40DFDC0D0309.includes

//## begin module%40DFDC0D0309.declarations preserve=no
//## end module%40DFDC0D0309.declarations

//## begin module%40DFDC0D0309.additionalDeclarations preserve=yes
//## end module%40DFDC0D0309.additionalDeclarations


//## begin CLDIFile%40DFDB6803DE.preface preserve=yes
//## end CLDIFile%40DFDB6803DE.preface

//## Class: CLDIFile%40DFDB6803DE
//## Category: 3dViewer::Core%40DB9BAD036C
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class CLDIFile 
{
  //## begin CLDIFile%40DFDB6803DE.initialDeclarations preserve=yes
  //## end CLDIFile%40DFDB6803DE.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Ldi2Raw%40DFDC4401EF
      HANDLE Ldi2Raw (LPSTR lpStr, int* w, int* h, HANDLE* hTxtrRaw = NULL);

      //## Operation: Ldi2RawTxtr%40DFDC87021E
      HANDLE Ldi2RawTxtr (LPSTR lpStr, int* w, int* h);

    // Additional Public Declarations
      //## begin CLDIFile%40DFDB6803DE.public preserve=yes
      //## end CLDIFile%40DFDB6803DE.public

  protected:
    // Additional Protected Declarations
      //## begin CLDIFile%40DFDB6803DE.protected preserve=yes
      //## end CLDIFile%40DFDB6803DE.protected

  private:
    // Additional Private Declarations
      //## begin CLDIFile%40DFDB6803DE.private preserve=yes
      //## end CLDIFile%40DFDB6803DE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CLDIFile%40DFDB6803DE.implementation preserve=yes
      //## end CLDIFile%40DFDB6803DE.implementation

};

//## begin CLDIFile%40DFDB6803DE.postscript preserve=yes
//## end CLDIFile%40DFDB6803DE.postscript

// Class CLDIFile 

//## begin module%40DFDC0D0309.epilog preserve=yes
//## end module%40DFDC0D0309.epilog


#endif
