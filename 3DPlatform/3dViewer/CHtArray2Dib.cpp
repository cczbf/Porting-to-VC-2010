//## begin module%368281B601D4.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%368281B601D4.cm

//## begin module%368281B601D4.cp preserve=no
//## end module%368281B601D4.cp

//## Module: CHtArray2Dib%368281B601D4; Package body
//## Subsystem: bmpviewer%364BBA5B0050
//	d:\desktop\halftonetest\bmpviewer
//## Source file: D:\Desktop\Halftonetest\bmpviewer\CHtArray2Dib.cpp

//## begin module%368281B601D4.additionalIncludes preserve=no
#include "stdafx.h"
#include "resource.h"
//## end module%368281B601D4.additionalIncludes

//## begin module%368281B601D4.includes preserve=yes
//## end module%368281B601D4.includes

// CHtArray2Dib
#include "CHtArray2Dib.h"
//## begin module%368281B601D4.declarations preserve=no
//## end module%368281B601D4.declarations

//## begin module%368281B601D4.additionalDeclarations preserve=yes
//## end module%368281B601D4.additionalDeclarations


// Class CHtArray2Dib 







CHtArray2Dib::CHtArray2Dib()
  //## begin CHtArray2Dib::CHtArray2Dib%.hasinit preserve=no
      : m_dwLevels(0), m_dwLimitLevels(0), m_dwWidth(0), m_dwHeight(0), m_dwScaleX(1), m_dwScaleY(1)
  //## end CHtArray2Dib::CHtArray2Dib%.hasinit
  //## begin CHtArray2Dib::CHtArray2Dib%.initialization preserve=yes
  //## end CHtArray2Dib::CHtArray2Dib%.initialization
{
  //## begin CHtArray2Dib::CHtArray2Dib%.body preserve=yes
  //## end CHtArray2Dib::CHtArray2Dib%.body
}


CHtArray2Dib::~CHtArray2Dib()
{
  //## begin CHtArray2Dib::~CHtArray2Dib%.body preserve=yes
  //## end CHtArray2Dib::~CHtArray2Dib%.body
}



//## Other Operations (implementation)
HANDLE CHtArray2Dib::OpenHtArrayDIB (LPSTR lpFileName)
{
  //## begin CHtArray2Dib::OpenHtArrayDIB%914519251.body preserve=yes

	HANDLE hHtArray	= this->GetHtArrayData(lpFileName);
	HANDLE hDIB	= this->HtArray2Dib( hHtArray );
	GlobalFree( hHtArray );
	return ( hDIB );

  //## end CHtArray2Dib::OpenHtArrayDIB%914519251.body
}

BOOL CHtArray2Dib::IsValidNetFile (HANDLE hnetfile)
{
  //## begin CHtArray2Dib::IsValidNetFile%914537967.body preserve=yes
	
//	FILETIME MainProgramWriteTime;
	FILETIME writetime ;
//  if ( !GetMainProgramWriteTime ( &MainProgramWriteTime ) )
//        return FALSE ;

    uint32 readsize ;
    ReadFile ( hnetfile, &writetime, sizeof(FILETIME), (LPDWORD)&readsize, NULL ) ;
    if ( readsize != sizeof(FILETIME) )
        return FALSE ;

//    if ( writetime.dwLowDateTime != MainProgramWriteTime.dwLowDateTime ||
//         writetime.dwHighDateTime != MainProgramWriteTime.dwHighDateTime )
//        return FALSE ;

    return TRUE ;


  //## end CHtArray2Dib::IsValidNetFile%914537967.body
}

HANDLE CHtArray2Dib::GetHtArrayData (LPSTR lpFileName)
{
  //## begin CHtArray2Dib::GetHtArrayData%914519252.body preserve=yes
    HANDLE hnetfile ;				  
    uint32 readsize, netfilesize ;

    hnetfile = CreateFile (
        (LPSTR)lpFileName ,
        GENERIC_READ ,
        0 ,                     // no share
        NULL ,                  // no security attributes
        OPEN_EXISTING ,
        FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN ,
        NULL ) ;

    if ( hnetfile == INVALID_HANDLE_VALUE )
        return FALSE ;

    // if the file is not a valid net table file, means the time fields in the
    // net table file is different from the photo.dll's creation file
    // cannot restore
	
    if ( ! IsValidNetFile ( hnetfile ) ) {
        CloseHandle ( hnetfile ) ;
        return NULL;
    }

    if ( !ReadFile ( hnetfile, &m_dwLevels, sizeof(uint32), (LPDWORD)&readsize, NULL ) ) {
        CloseHandle ( hnetfile ) ;
        return NULL;
    }
    if ( !ReadFile ( hnetfile, &m_dwLimitLevels, sizeof(uint32), (LPDWORD)&readsize, NULL ) ) {
        CloseHandle ( hnetfile ) ;
        return NULL;
    }
    if ( !ReadFile ( hnetfile, &m_dwWidth, sizeof(uint32), (LPDWORD)&readsize, NULL ) ) {
        CloseHandle ( hnetfile ) ;
        return NULL;
    }
    if ( !ReadFile ( hnetfile, &m_dwHeight, sizeof(uint32), (LPDWORD)&readsize, NULL ) ) {
        CloseHandle ( hnetfile ) ;
        return NULL;
    }

    // calculate net table data size and allocate memory
	
	netfilesize = m_dwWidth * m_dwHeight *m_dwScaleX*m_dwScaleY* 2 ;
	HANDLE hHtArray	= GlobalAlloc(GHND, netfilesize);
//    LPUINT16 lpFrm = (LPUINT16) new BYTE [netfilesize] ;
	LPUINT16 lpFrm = (LPUINT16)GlobalLock(hHtArray);

    if ( !lpFrm ) {
		GlobalUnlock(hHtArray);
		GlobalFree(hHtArray);
        CloseHandle ( hnetfile ) ;
        return NULL;
    }
    // read the net table data
    if ( !ReadFile ( hnetfile, lpFrm, netfilesize, (LPDWORD)&readsize, NULL ) ) {
        CloseHandle ( hnetfile ) ;
        return NULL;
    }
    CloseHandle ( hnetfile ) ;

    if ( readsize != netfilesize ) {
        return NULL;
    }

    this->DecodeFrmData ( lpFrm, m_dwWidth, m_dwHeight, 
					m_dwScaleX, m_dwScaleY) ;

	GlobalUnlock(hHtArray);
    return (hHtArray);


  //## end CHtArray2Dib::GetHtArrayData%914519252.body
}

HANDLE CHtArray2Dib::HtArray2Dib (HANDLE hHtArray)
{
  //## begin CHtArray2Dib::HtArray2Dib%914519253.body preserve=yes

	ASSERT(hHtArray!=NULL);
	HPALETTE hPal	= this->Create8BitGrayLinearPalette ();

	HANDLE hDib = this->CreateDib (	m_dwWidth ,//int width,
								m_dwHeight,//int height, 
								0,//DWORD biStyle, 
								8*3,//WORD biBits,
//								hPal//HPALETTE hpal
								NULL
								);
		DeleteObject( hPal );


	LPBYTE lpDibBits	= (LPBYTE)GlobalLock( hDib );

	lpDibBits	= (LPBYTE)this->FindDIBBits ((LPSTR)lpDibBits);
	
	LPUINT16 lpFrm = (LPUINT16)GlobalLock(hHtArray);
	
	int k = 0;
	int m = 0;
//	ASSERT(m_dwWidth%4==0);
	int nWidhtbytes =  WIDTHBYTES(m_dwWidth*24);


	for( unsigned int j = 0; j < m_dwHeight; j++ )
	{
//		m = ( m_dwHeight - j - 1)*m_dwWidth*3;
		m = ( m_dwHeight - j - 1)*nWidhtbytes;
		
		for( unsigned int i = 0; i < m_dwWidth; i++ )
		{
			lpDibBits[m++]	= (BYTE)lpFrm[k  ];
			lpDibBits[m++]	= (BYTE)lpFrm[k  ];
			lpDibBits[m++]	= (BYTE)lpFrm[k++];
//			lpDibBits[m++]	= (BYTE)lpFrm[k  ];
//			lpDibBits[m++]	= (BYTE)(lpFrm[k  ]>>8);
//			lpDibBits[m++]	= 0;
//			k++;
		}
	}

	GlobalUnlock( hDib );
	GlobalUnlock(hHtArray);
	return hDib;
  //## end CHtArray2Dib::HtArray2Dib%914519253.body
}

void CHtArray2Dib::DecodeFrmData (LPUINT16 lpFrm, uint32 dwWidth, uint32 dwHeight, uint32 dwScaleX, uint32 dwScaleY)
{
  //## begin CHtArray2Dib::DecodeFrmData%914523962.body preserve=yes

    for ( uint32 j=0; j < dwHeight*dwScaleY ; j++ )
    {
        for ( uint32 i = 0; i < dwWidth*dwScaleX; i++ )
        {
            *lpFrm = *lpFrm ^ uint16((i+j+i*j)*177) ;
            lpFrm ++ ;
        }
    }

  //## end CHtArray2Dib::DecodeFrmData%914523962.body
}

// Additional Declarations
  //## begin CHtArray2Dib%3681913401AE.declarations preserve=yes
HANDLE CHtArray2Dib::CreateDibFromColorBuffer(int nWid, int nHei, BYTE *pBuffer)
{
	if (pBuffer == NULL)
		return NULL ;

	m_dwWidth = nWid;
	m_dwHeight = nHei;

	HANDLE hDib = this->CreateDib (	m_dwWidth ,//int width,
								m_dwHeight,//int height, 
								0,//DWORD biStyle, 
								8*3,//WORD biBits,
								NULL
								);

	LPBYTE lpDibBits	= (LPBYTE)GlobalLock( hDib );
	lpDibBits	= (LPBYTE)this->FindDIBBits ((LPSTR)lpDibBits);
	
	int k = 0;
	int m = 0;
//	ASSERT(m_dwWidth%4==0);

	int nWidhtbytes =  WIDTHBYTES(m_dwWidth*24);

	for( unsigned int j = 0; j < m_dwHeight; j++ )
	{
		//m = ( m_dwHeight - j - 1)*nWidhtbytes;
		m = j*nWidhtbytes;
		
		for( unsigned int i = 0; i < m_dwWidth; i++ )
		{
			lpDibBits[m++]	= (BYTE)pBuffer[(j*m_dwWidth+i)*4 + 2 ];
			lpDibBits[m++]	= (BYTE)pBuffer[(j*m_dwWidth+i)*4 + 1 ];
			lpDibBits[m++]	= (BYTE)pBuffer[(j*m_dwWidth+i)*4  ];
		}
	}

	GlobalUnlock( hDib );
	return hDib;
}
//## end CHtArray2Dib%3681913401AE.declarations

//## begin module%368281B601D4.epilog preserve=yes
HANDLE CHtArray2Dib::CreateDibFromRawData(int nWid, int nHei, BYTE *pRawData )
{

	if (pRawData == NULL)
		return NULL ;

	m_dwWidth = nWid;
	m_dwHeight = nHei;

	HANDLE hDib = this->CreateDib (	m_dwWidth ,//int width,
								m_dwHeight,//int height, 
								0,//DWORD biStyle, 
								8*3,//WORD biBits,
								NULL
								);

	LPBYTE lpDibBits	= (LPBYTE)GlobalLock( hDib );
	lpDibBits	= (LPBYTE)this->FindDIBBits ((LPSTR)lpDibBits);
	
	int k = 0;
	int m = 0;
//	ASSERT(m_dwWidth%4==0);

	int nWidhtbytes =  WIDTHBYTES(m_dwWidth*24);

	for( unsigned int j = 0; j < m_dwHeight; j++ )
	{
		//m = ( m_dwHeight - j - 1)*nWidhtbytes;
		m = j*nWidhtbytes;
		
		for( unsigned int i = 0; i < m_dwWidth; i++ )
		{
			lpDibBits[m++]	= (BYTE)pRawData[(j*m_dwWidth+i)*3 + 0 ];
			lpDibBits[m++]	= (BYTE)pRawData[(j*m_dwWidth+i)*3 + 1 ];
			lpDibBits[m++]	= (BYTE)pRawData[(j*m_dwWidth+i)*3 + 2 ];
		}
	}

	GlobalUnlock( hDib );
	return hDib;

}
//## end module%368281B601D4.epilog
