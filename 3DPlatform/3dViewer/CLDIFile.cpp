//## begin module%40DFDC11023C.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40DFDC11023C.cm

//## begin module%40DFDC11023C.cp preserve=no
//## end module%40DFDC11023C.cp

//## Module: CLDIFile%40DFDC11023C; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CLDIFile.cpp

//## begin module%40DFDC11023C.additionalIncludes preserve=no
//## end module%40DFDC11023C.additionalIncludes

//## begin module%40DFDC11023C.includes preserve=yes

#include <stdlib.h>
#include <string.h>
#include "LIMITS.H"
#include "FLOAT.H"

//## end module%40DFDC11023C.includes

// CLDIFile
#include "CLDIFile.h"
//## begin module%40DFDC11023C.declarations preserve=no
//## end module%40DFDC11023C.declarations

//## begin module%40DFDC11023C.additionalDeclarations preserve=yes

/* Macro to restrict a given value to an upper or lower boundary value */
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

//## end module%40DFDC11023C.additionalDeclarations


// Class CLDIFile 


//## Other Operations (implementation)
HANDLE CLDIFile::Ldi2Raw (LPSTR lpStr, int* w, int* h, HANDLE* hTxtrRaw)
{
  //## begin CLDIFile::Ldi2Raw%40DFDC4401EF.body preserve=yes
	// ldi2raw.cpp : Defines the entry point for the console application.
{
	

	char lpszPathName[1024];
		
	int i,j;

	FILE *fp=NULL;

	strcpy(lpszPathName, lpStr);

	fp=fopen(lpszPathName,"rb");
//	ASSERT(fp!=NULL);
	if(fp==NULL)
	{
		return  NULL;	
	}
	int m_bState, m_bState1;
	fread(&m_bState,sizeof(int),1,fp);
	fread(&m_bState1,sizeof(int),1,fp);
	float Xs, Ys, Zs, O, P, K, f;
	fread(&Xs,sizeof(float),1,fp);
	fread(&Ys,sizeof(float),1,fp);
	fread(&Zs,sizeof(float),1,fp);

	fread(&O,sizeof(float),1,fp);
	fread(&P,sizeof(float),1,fp);
	fread(&K,sizeof(float),1,fp);
	
	fread(&f,sizeof(float),1,fp);
	
	int m_nWid,m_nHei;

	fread(&m_nWid,sizeof(int),1,fp);
	fread(&m_nHei,sizeof(int),1,fp);

	double m_Rt[3][3];

	for(j=0;j<3;j++)
		for(i=0;i<3;i++)
			fread(&m_Rt[j][i],sizeof(double),1,fp);

	float Tx, Ty, Tz;

	fread(&Tx,sizeof(double),1,fp);
	fread(&Ty,sizeof(double),1,fp);
	fread(&Tz,sizeof(double),1,fp);

	float *pfDepth = (float*)malloc(m_nWid*m_nHei*sizeof(float));
	int *fpRGB = (int*)malloc(m_nWid*m_nHei*sizeof(int)*3);
	int *fpR,*fpG,*fpB;
	fpR = fpRGB; fpG = fpR + m_nWid*m_nHei; fpB = fpG + m_nWid*m_nHei;
	
	//read depth pixel
//	float z;
	int /*r, g, b, */numLayers;

	int   nI=0,nJ=0;
	float fmin = FLT_MAX;
	float fmax = FLT_MIN;
	for(i=0;i<m_nWid*m_nHei;i++)
	{
		fread(&nI,sizeof(int),1,fp);
		fread(&nJ,sizeof(int),1,fp);

		fread(&numLayers,sizeof(int),1,fp);
		
		if ( numLayers > 0 )
		for(int j=0; j<numLayers; j++)
		{
			fread(&(pfDepth[nJ*m_nWid+nI]),sizeof(float),1,fp);

			fmin = __min(pfDepth[nJ*m_nWid+nI], fmin);
			fmax = __max(pfDepth[nJ*m_nWid+nI], fmax);

			fread(&(fpR[nJ*m_nWid+nI]),sizeof(int),1,fp);
			fread(&(fpG[nJ*m_nWid+nI]),sizeof(int),1,fp);
			fread(&(fpB[nJ*m_nWid+nI]),sizeof(int),1,fp);
		}
		else
		{
			pfDepth[nJ*m_nWid+nI] = FLT_MAX;
		}
	}
	fclose(fp);

	HANDLE hRGB = GlobalAlloc ( GHND, m_nWid*m_nHei*sizeof(char)*3 );
//	unsigned char *pcR = (unsigned char*)malloc(m_nWid*m_nHei*sizeof(char)*4);
	unsigned char *pcR = (unsigned char*)GlobalLock(hRGB);
	unsigned char *pcG = pcR + m_nWid*m_nHei;
	unsigned char *pcB = pcG + m_nWid*m_nHei;
	HANDLE hDIRaw = GlobalAlloc ( GHND, m_nWid*m_nHei*sizeof(char));
	unsigned char *pcD = (unsigned char*)GlobalLock(hDIRaw);

	for(i=0;i<m_nWid*m_nHei;i++)
	{
		pcD[i] = unsigned char(BOUND((pfDepth[i] - fmin)*255.0/(fmax-fmin),0.0,255.0));
		pcR[i] = unsigned char(fpR[i]);
		pcG[i] = unsigned char(fpG[i]);
		pcB[i] = unsigned char(fpB[i]);
	}

//	strcpy(strchr(lpszPathName,'.'),".raw");
//	fp = fopen( lpszPathName, "wb" );
//	fwrite( pcR, sizeof( char ), m_nWid*m_nHei*sizeof(char)*4, fp );
//	fclose(fp);
	GlobalUnlock(hRGB);
	GlobalUnlock(hDIRaw);
	if ( hTxtrRaw == NULL )
	{
		GlobalFree ( hRGB );
	}
	else
		*hTxtrRaw = hRGB;
	*w = m_nWid;
	*h = m_nHei;
	return hDIRaw;
}


  //## end CLDIFile::Ldi2Raw%40DFDC4401EF.body
}

HANDLE CLDIFile::Ldi2RawTxtr (LPSTR lpStr, int* w, int* h)
{
  //## begin CLDIFile::Ldi2RawTxtr%40DFDC87021E.body preserve=yes
	return NULL;
  //## end CLDIFile::Ldi2RawTxtr%40DFDC87021E.body
}

// Additional Declarations
  //## begin CLDIFile%40DFDB6803DE.declarations preserve=yes
  //## end CLDIFile%40DFDB6803DE.declarations

//## begin module%40DFDC11023C.epilog preserve=yes
//## end module%40DFDC11023C.epilog
