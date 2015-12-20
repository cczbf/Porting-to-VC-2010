//## begin module%4213ECF803D5.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%4213ECF803D5.cm

//## begin module%4213ECF803D5.cp preserve=no
//## end module%4213ECF803D5.cp

//## Module: CDIBFlood%4213ECF803D5; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CDIBFlood.cpp

//## begin module%4213ECF803D5.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%4213ECF803D5.additionalIncludes

//## begin module%4213ECF803D5.includes preserve=yes
#include <stdlib.h>


#define VISUALIZE
#ifdef VISUALIZE
#include "CGlobals.h"
#include "3dViewerDoc.h"
#endif

//## end module%4213ECF803D5.includes

// CDIBFlood
#include "CDIBFlood.h"
//## begin module%4213ECF803D5.declarations preserve=no
//## end module%4213ECF803D5.declarations

//## begin module%4213ECF803D5.additionalDeclarations preserve=yes

int __cdecl   compare( const void *arg1, const void *arg2 )
{
	stParam *p1 = (stParam *)arg1;
	stParam *p2 = (stParam *)arg2;
	int ret = (p1->dblKey > p2->dblKey ?1:-1);
	return ret;
}
//## end module%4213ECF803D5.additionalDeclarations


// Class CDIBFlood 





CDIBFlood::CDIBFlood()
  //## begin CDIBFlood::CDIBFlood%.hasinit preserve=no
      : m_lParamLength(0), m_lStackTop(-1)
  //## end CDIBFlood::CDIBFlood%.hasinit
  //## begin CDIBFlood::CDIBFlood%.initialization preserve=yes
  //## end CDIBFlood::CDIBFlood%.initialization
{
  //## begin CDIBFlood::CDIBFlood%.body preserve=yes
	m_pParam = NULL;
	m_pstStack = NULL;
  //## end CDIBFlood::CDIBFlood%.body
}


CDIBFlood::~CDIBFlood()
{
  //## begin CDIBFlood::~CDIBFlood%.body preserve=yes
	if ( this->m_pParam!= NULL)
		free(this->m_pParam);
	this->m_lParamLength = 0;
	this->m_pParam = NULL;

	if ( this->m_pstStack!= NULL)
		free(this->m_pstStack);
	this->m_lStackTop = 0;
	this->m_pstStack = NULL;

  //## end CDIBFlood::~CDIBFlood%.body
}



//## Other Operations (implementation)
long CDIBFlood::FloodFill4 (HANDLE hDIB, int nSeedX, int nSeedY, int nROld, int nGOld, int nBOld, int nRNew, int nGNew, int nBNew, double dMin, double dMax)
{
  //## begin CDIBFlood::FloodFill4%4213EDAB03D2.body preserve=yes
  	WORD nR , nG , nB;
	this->GetPixel(hDIB, nSeedX, nSeedY, &nR , &nG , &nB );

	if ( nR == nROld && nG == nGOld && nB == nBOld )
	{
		this->PutPixel(hDIB, nSeedX, nSeedY, nRNew, nGNew, nBNew );

#ifdef VISUALIZE
 
		CMy3dViewerDoc *pDoc = g_CGlobals.m_pCMy3dViewerDoc;
		HANDLE hDib1 = this->CopyDib (hDIB);
		pDoc->m_CBmpViewerData->SetDIBHandle(NULL);// call this function with NULL to delete hDib0
		pDoc->m_CBmpViewerData->SetDIBHandle(hDib1);
 		pDoc->UpdateView();	
#endif
#define  dGap ((dMax-dMin)/4.0)
		this->FloodFill4(	hDIB, nSeedX+1, nSeedY, 
							nR, nG, nB, nRNew, nGNew, nBNew, dMin,dMin + dGap );
		this->FloodFill4(	hDIB, nSeedX, nSeedY+1, 
							nR, nG, nB, nRNew, nGNew, nBNew, dMin + dGap, dMin + dGap*2.0 );
		this->FloodFill4(	hDIB, nSeedX-1, nSeedY, 
							nR, nG, nB, nRNew, nGNew, nBNew, dMin + dGap*2.0, dMin + dGap*3.0 );
		this->FloodFill4(	hDIB, nSeedX, nSeedY-1, 
							nR, nG, nB, nRNew, nGNew, nBNew, dMin + dGap*3.0, dMax );

	}
	else
	{
		if ( nR != nRNew || nG != nGNew || nB != nBNew )
		{//Border
			this->m_pParam[this->m_lParamLength].dblKey = (dMin + dMax) / 2.0;
			this->m_pParam[this->m_lParamLength].x		= double(nSeedX);
			this->m_pParam[this->m_lParamLength].y		= double(nSeedY);
			this->m_lParamLength++;
		}

	}
 return 0;
  //## end CDIBFlood::FloodFill4%4213EDAB03D2.body
}

void CDIBFlood::BoundryFill4 (HANDLE hDIB, int nSeedX, int nSeedY, int nRNew, int nGNew, int nBNew)
{
  //## begin CDIBFlood::BoundryFill4%4213EDC7036E.body preserve=yes

	int h = this->DIBHeight(hDIB);
	int w = this->DIBWidth(hDIB);

	if ( this->m_pParam!= NULL)
		free(this->m_pParam);
	this->m_pParam = (stParam*)malloc(w*h*sizeof(stParam));
	this->m_lParamLength = 0;

 	WORD nR,nG, nB;
	this->GetPixel(hDIB, nSeedX, nSeedY, &nR , &nG , &nB );
	this->FloodFill4(	hDIB, nSeedX, nSeedY, 
						nR , nG , nB , nRNew , nGNew , nBNew, 0.0, 1.0 );


//	qsort( (void *)(this->m_pParam), (size_t)(this->m_lParamLength), sizeof(stParam), compare );

	TRACE("main()\r\n{\r\n");
	TRACE(" int p[%d];\r\n int i; \r\n i = 0;\r\n",this->m_lParamLength*2);
	for ( int i = 0; i < this->m_lParamLength ; i++)
	{
		TRACE(" p[%d] = %f; p[%d] = %f + i;\r\n", 
				i*2, 
				this->m_pParam[i].x/double(w) * 320.0,
				i*2+1,
				this->m_pParam[i].y/double(h) * 320.0 );
	}
	TRACE (" polyline(p,%d);\r\n}\r\n", this->m_lParamLength * 2);
 
	for (int i = 0; i < this->m_lParamLength ; i++)
		TRACE ( "%4d\t %1.22e\t%f\t%f\t \r\n", 
				i, 
				this->m_pParam[i].dblKey,
				this->m_pParam[i].x,
				this->m_pParam[i].y
				);
  //## end CDIBFlood::BoundryFill4%4213EDC7036E.body
}

void CDIBFlood::BoundryFill4_Stack (HANDLE hDIB, int nSeedX, int nSeedY, int nRNew, int nGNew, int nBNew)
{
  //## begin CDIBFlood::BoundryFill4_Stack%42143E260085.body preserve=yes

	int h = this->DIBHeight(hDIB);
	int w = this->DIBWidth(hDIB);

	if ( this->m_pParam!= NULL)
		free(this->m_pParam);
	this->m_pParam = (stParam*)malloc(w*h*sizeof(stParam));
	this->m_lParamLength = 0;

	if ( this->m_pstStack!= NULL)
		free(this->m_pstStack);
	this->m_pstStack = (stStack*)malloc(w*h*sizeof(stStack));
	this->m_lStackTop = -1;

 	WORD nR,nG, nB;
	int dx[4] = {1,0,-1, 0};
	int dy[4] = {0,1, 0,-1};

	this->GetPixel(hDIB, nSeedX, nSeedY, &nR , &nG , &nB );
	this->PutPixel(hDIB, nSeedX, nSeedY, nRNew, nGNew, nBNew );
	this->Push(nSeedX, nSeedY, 0.0, 1.0);
	int i, x, y;
	double dMin0, dMax0,dGap0, dMin1, dMax1;
	while ( !(this->StackEmpty()))
	{
		Pop(&x, &y, &dMin0, &dMax0);
		dGap0 = (dMax0 - dMin0)/4.0;
		for ( int i = 0; i < 4; i++)
		{
			nSeedX = x+dx[i];
			nSeedY = y+dy[i];
			dMin1 = dMin0 + i * dGap0;
			dMax1 = dMin1 + dGap0;
			if (IsInterior(hDIB,nSeedX,nSeedY, nR , nG , nB))
			{
				this->PutPixel(hDIB, nSeedX, nSeedY, nRNew, nGNew, nBNew );
				this->Push(nSeedX, nSeedY, dMin1, dMax1);

#ifdef VISUALIZE
				for ( int j = 0; j <1; j++ )
				{
					CMy3dViewerDoc *pDoc = g_CGlobals.m_pCMy3dViewerDoc;
					HANDLE hDib1 = this->CopyDib (hDIB);
					pDoc->m_CBmpViewerData->SetDIBHandle(NULL);// call this function with NULL to delete hDib0
					pDoc->m_CBmpViewerData->SetDIBHandle(hDib1);
			 		pDoc->UpdateView();	
				}
#endif
			}
			else
				if ( IsBorder(hDIB,nSeedX,nSeedY, nRNew, nGNew, nBNew))
				{

					this->m_pParam[this->m_lParamLength].dblKey = (dMin1 + dMax1) / 2.0;
					this->m_pParam[this->m_lParamLength].x		= double(nSeedX);
					this->m_pParam[this->m_lParamLength].y		= double(nSeedY);
					this->m_lParamLength++;
				} 

			
				
		}
	}
	qsort( (void *)(this->m_pParam), (size_t)(this->m_lParamLength), sizeof(stParam), compare );

	TRACE("main()\r\n{\r\n");
	TRACE(" int p[%d];\r\n int i; \r\n i = 0;\r\n",this->m_lParamLength*2);
	for ( i = 0; i < this->m_lParamLength ; i++)
	{
		TRACE(" p[%d] = %f; p[%d] = %f + i;\r\n", 
				i*2, 
				this->m_pParam[i].x/double(w) * 320.0,
				i*2+1,
				this->m_pParam[i].y/double(h) * 320.0 );
	}
	TRACE (" polyline(p,%d);\r\n}\r\n", this->m_lParamLength * 2);
 
	for ( i = 0; i < this->m_lParamLength ; i++)
		TRACE ( "%4d\t %1.22e\t%f\t%f\t \r\n", 
				i, 
				this->m_pParam[i].dblKey,
				this->m_pParam[i].x,
				this->m_pParam[i].y
				);

  //## end CDIBFlood::BoundryFill4_Stack%42143E260085.body
}

void CDIBFlood::Push (int x, int y, double dMin, double dMax)
{
  //## begin CDIBFlood::Push%421440F8007A.body preserve=yes
	this->m_lStackTop++;
	this->m_pstStack[this->m_lStackTop].x = x; 
	this->m_pstStack[this->m_lStackTop].y = y;
	this->m_pstStack[this->m_lStackTop].dMax = dMax;
	this->m_pstStack[this->m_lStackTop].dMin = dMin;

  //## end CDIBFlood::Push%421440F8007A.body
}

void CDIBFlood::Pop (int* x, int* y, double* dMin, double* dMax)
{
  //## begin CDIBFlood::Pop%4214475F004E.body preserve=yes
	*x = this->m_pstStack[this->m_lStackTop].x; 
	*y = this->m_pstStack[this->m_lStackTop].y;
	*dMax = this->m_pstStack[this->m_lStackTop].dMax;
	*dMin = this->m_pstStack[this->m_lStackTop].dMin;
	this->m_lStackTop--;

  //## end CDIBFlood::Pop%4214475F004E.body
}

BOOL CDIBFlood::IsInterior (HANDLE hDIB, int nSeedX, int nSeedY, WORD nR, WORD nG, WORD nB)
{
  //## begin CDIBFlood::IsInterior%421445B0018F.body preserve=yes
	WORD nR0 , nG0 , nB0;
	this->GetPixel(hDIB, nSeedX, nSeedY, &nR0 , &nG0 , &nB0 );
	return(nR == nR0 && nG == nG0 && nB == nB0);
  //## end CDIBFlood::IsInterior%421445B0018F.body
}

BOOL CDIBFlood::IsBorder (HANDLE hDIB, int nSeedX, int nSeedY, WORD nRNew, WORD nGNew, WORD nBNew)
{
  //## begin CDIBFlood::IsBorder%4214693501DA.body preserve=yes
	WORD nR0 , nG0 , nB0;
	this->GetPixel(hDIB, nSeedX, nSeedY, &nR0 , &nG0 , &nB0 );
	return!(nR0 == nRNew && nG0 == nGNew && nB0 == nBNew);
  //## end CDIBFlood::IsBorder%4214693501DA.body
}

BOOL CDIBFlood::StackEmpty ()
{
  //## begin CDIBFlood::StackEmpty%421446610284.body preserve=yes
	return ( this->m_lStackTop == -1 );
  //## end CDIBFlood::StackEmpty%421446610284.body
}

// Additional Declarations
  //## begin CDIBFlood%4213EC310194.declarations preserve=yes
  //## end CDIBFlood%4213EC310194.declarations

// Class stParam 





// Additional Declarations
  //## begin stParam%4213EC9C0350.declarations preserve=yes
  //## end stParam%4213EC9C0350.declarations

// Class stStack 






// Additional Declarations
  //## begin stStack%42143E6B017F.declarations preserve=yes
  //## end stStack%42143E6B017F.declarations

//## begin module%4213ECF803D5.epilog preserve=yes
//## end module%4213ECF803D5.epilog
