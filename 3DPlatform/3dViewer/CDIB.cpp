//## begin module%40DB9D33020F.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40DB9D33020F.cm

//## begin module%40DB9D33020F.cp preserve=no
//## end module%40DB9D33020F.cp

//## Module: CDIB%40DB9D33020F; Package body
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CDIB.cpp

//## begin module%40DB9D33020F.additionalIncludes preserve=no
#include "stdafx.h"
//## end module%40DB9D33020F.additionalIncludes

//## begin module%40DB9D33020F.includes preserve=yes
#include "stdafx.h"
#include <math.h>
#include <errno.h>
#include <FLOAT.H>

#define VISUALIZE
#ifdef VISUALIZE
#include "CGlobals.h"
#include "3dViewerDoc.h"
#endif
//## end module%40DB9D33020F.includes

// CDIB
#include "CDIB.h"
//## begin module%40DB9D33020F.declarations preserve=no
//## end module%40DB9D33020F.declarations

//## begin module%40DB9D33020F.additionalDeclarations preserve=yes

#ifndef WIN32
typedef char huge*      HPSTR;  // huge pointer to a string
#else
typedef char*      HPSTR;  // huge pointer to a string
#define huge
#endif

#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/* The following coefficients are the ones that PhotoStyler usees. */
#define R_Brightness   0.2980392156863
#define G_Brightness   0.5882352941176
#define B_Brightness   0.09803921568627

#define sign(x) ( ( x ) > 0 ? 1 : ( ( x ) == 0 ? 0 : (-1) ) )

//## end module%40DB9D33020F.additionalDeclarations


// Class CDIB 


//## Other Operations (implementation)
HANDLE CDIB::OpenDIB (LPSTR szFile)
{
  //## begin CDIB::OpenDIB%3692465A00C7.body preserve=yes
    unsigned            fh;
    BITMAPINFOHEADER    bi;
    LPBITMAPINFOHEADER  lpbi;
    DWORD               dwLen = 0;
    DWORD               dwBits;
    HANDLE              hdib;
    HANDLE              h;
    OFSTRUCT            of;

    /* Open the file and read the DIB information */
    fh = OpenFile(szFile, &of, OF_READ);
    if (fh == -1)
        return NULL;

    hdib = ReadDibBitmapInfo(fh);
    if (!hdib)
        return NULL;
    DibInfo(hdib,&bi);

    /* Calculate the memory needed to hold the DIB */
    dwBits = bi.biSizeImage;
    dwLen  = bi.biSize + (DWORD)PaletteSize (&bi) + dwBits;

    /* Try to increase the size of the bitmap info. buffer to hold the DIB */
    h = GlobalReAlloc(hdib, dwLen, GHND);
    if (!h){
        GlobalFree(hdib);
        hdib = NULL;
    }
    else
        hdib = h;

    /* Read in the bits */
    if (hdib){

//        lpbi = (VOID FAR *)GlobalLock(hdib);
        lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
        lread(fh, (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi), dwBits);
        GlobalUnlock(hdib);
    }
    _lclose(fh);

    return hdib;




  //## end CDIB::OpenDIB%3692465A00C7.body
}

BOOL CDIB::WriteDIB (LPSTR szFile, HANDLE hdib)
{
  //## begin CDIB::WriteDIB%3692465A00CB.body preserve=yes
	BITMAPFILEHEADER    hdr;
    LPBITMAPINFOHEADER  lpbi;
    int                 fh;
    OFSTRUCT            of;

    if (!hdib)
        return FALSE;

    fh = OpenFile (szFile, &of, OF_CREATE|OF_READWRITE);
    if (fh == -1)
        return FALSE;

//    lpbi = (VOID FAR *)GlobalLock (hdib);
    lpbi = (LPBITMAPINFOHEADER)GlobalLock (hdib);

    /* Fill in the fields of the file header */
    hdr.bfType          = BFT_BITMAP;
    hdr.bfSize          = GlobalSize (hdib) + sizeof (BITMAPFILEHEADER);
    hdr.bfReserved1     = 0;
    hdr.bfReserved2     = 0;
    hdr.bfOffBits       = (DWORD)sizeof(BITMAPFILEHEADER) + lpbi->biSize +
                          PaletteSize(lpbi);

    /* Write the file header */
	int iRet =
    _lwrite (fh, (LPSTR)&hdr, sizeof (BITMAPFILEHEADER));

    /* Write the DIB header and the bits */

    iRet = lwrite (fh, (LPSTR)lpbi, GlobalSize (hdib));

    GlobalUnlock (hdib);
	iRet =
    _lclose (fh);
    return TRUE;

  //## end CDIB::WriteDIB%3692465A00CB.body
}

WORD CDIB::PaletteSize (void* pv)
{
  //## begin CDIB::PaletteSize%3692465A00D2.body preserve=yes

	LPBITMAPINFOHEADER lpbi;
    WORD               NumColors;

    lpbi      = (LPBITMAPINFOHEADER)pv;
    NumColors = DibNumColors(lpbi);

    if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
        return NumColors * sizeof(RGBTRIPLE);
    else
        return NumColors * sizeof(RGBQUAD);




  //## end CDIB::PaletteSize%3692465A00D2.body
}

WORD CDIB::DibNumColors (void* pv)
{
  //## begin CDIB::DibNumColors%3692465A00D4.body preserve=yes

    int                 bits;
    LPBITMAPINFOHEADER  lpbi;
    LPBITMAPCOREHEADER  lpbc;

    lpbi = ((LPBITMAPINFOHEADER)pv);
    lpbc = ((LPBITMAPCOREHEADER)pv);

//    /*  With the BITMAPINFO format headers, the size of the palette
//     *  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
//     *  is dependent on the bits per pixel ( = 2 raised to the power of
//     *  bits/pixel).
//     */

    if (lpbi->biSize != sizeof(BITMAPCOREHEADER)){
        if (lpbi->biClrUsed != 0)
            return (WORD)lpbi->biClrUsed;
        bits = lpbi->biBitCount;
    }
    else
        bits = lpbc->bcBitCount;

    switch (bits){
        case 1:
                return 2;
        case 4:
                return 16;
        case 8:
                return 256;
        default:
                /* A 24 bitcount DIB has no color table */
                return 0;
    }

  //## end CDIB::DibNumColors%3692465A00D4.body
}

HPALETTE CDIB::CreateDibPalette (HANDLE hdib)
{
  //## begin CDIB::CreateDibPalette%3692465A00D6.body preserve=yes
    HPALETTE hpal;
	HANDLE hbi = hdib;
    if (!hbi)
        return NULL;
    hpal = CreateBIPalette((LPBITMAPINFOHEADER)GlobalLock(hbi));
    GlobalUnlock(hbi);
    return hpal;




  //## end CDIB::CreateDibPalette%3692465A00D6.body
}

HPALETTE CDIB::CreateBIPalette (LPBITMAPINFOHEADER lpbi)
{
  //## begin CDIB::CreateBIPalette%3692465A010F.body preserve=yes
    LOGPALETTE          *pPal;
    HPALETTE            hpal = NULL;
    WORD                nNumColors;
    BYTE                red;
    BYTE                green;
    BYTE                blue;
    WORD                i;
    RGBQUAD        FAR *pRgb;

    if (!lpbi)
        return NULL;

    if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
        return NULL;

    /* Get a pointer to the color table and the number of colors in it */
    pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + (WORD)lpbi->biSize);
    nNumColors = DibNumColors(lpbi);

    if (nNumColors){
        /* Allocate for the logical palette structure */
        pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        if (!pPal)
            return NULL;

        pPal->palNumEntries = nNumColors;
        pPal->palVersion    = PALVERSION;

        /* Fill in the palette entries from the DIB color table and
         * create a logical color palette.
         */
        for (i = 0; i < nNumColors; i++){
            pPal->palPalEntry[i].peRed   = pRgb[i].rgbRed;
            pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
            pPal->palPalEntry[i].peBlue  = pRgb[i].rgbBlue;
            pPal->palPalEntry[i].peFlags = (BYTE)0;
        }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE)pPal);
    }
    else if (lpbi->biBitCount == 24){
        /* A 24 bitcount DIB has no color table entries so, set the number of
         * to the maximum value (256).
         */
        nNumColors = MAXPALETTE;
        pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        if (!pPal)
            return NULL;

        pPal->palNumEntries = nNumColors;
        pPal->palVersion    = PALVERSION;

        red = green = blue = 0;

        /* Generate 256 (= 8*8*4) RGB combinations to fill the palette
         * entries.
         */
        for (i = 0; i < pPal->palNumEntries; i++){
            pPal->palPalEntry[i].peRed   = red;
            pPal->palPalEntry[i].peGreen = green;
            pPal->palPalEntry[i].peBlue  = blue;
            pPal->palPalEntry[i].peFlags = (BYTE)0;

            if (!(red += 32))
                if (!(green += 32))
                    blue += 64;
        }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE)pPal);
    }
    return hpal;
  //## end CDIB::CreateBIPalette%3692465A010F.body
}

HANDLE CDIB::DibFromBitmap (HBITMAP hbm, DWORD biStyle, WORD biBits, HPALETTE hpal)
{
  //## begin CDIB::DibFromBitmap%3692465A0117.body preserve=yes

    BITMAP               bm;
    BITMAPINFOHEADER     bi;
    BITMAPINFOHEADER FAR *lpbi;
    DWORD                dwLen;
    HANDLE               hdib;
    HANDLE               h;
    HDC                  hdc;

    if (!hbm)
        return NULL;

    if (hpal == NULL)
        hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

    GetObject(hbm,sizeof(bm),(LPSTR)&bm);

    if (biBits == 0)
        biBits =  bm.bmPlanes * bm.bmBitsPixel;

    bi.biSize               = sizeof(BITMAPINFOHEADER);
    bi.biWidth              = bm.bmWidth;
    bi.biHeight             = bm.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = biBits;
    bi.biCompression        = biStyle;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    dwLen  = bi.biSize + PaletteSize(&bi);

    hdc = GetDC(NULL);
    hpal = SelectPalette(hdc,hpal,FALSE);
         RealizePalette(hdc);

    hdib = GlobalAlloc(GHND,dwLen);

    if (!hdib){
        SelectPalette(hdc,hpal,FALSE);
        ReleaseDC(NULL,hdc);
        return NULL;
    }

    lpbi = (BITMAPINFOHEADER FAR *)GlobalLock(hdib);

    *lpbi = bi;

    /*  call GetDIBits with a NULL lpBits param, so it will calculate the
     *  biSizeImage field for us
     */
    GetDIBits(hdc, hbm, 0, (WORD)bi.biHeight,
        NULL, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

    bi = *lpbi;
    GlobalUnlock(hdib);

    /* If the driver did not fill in the biSizeImage field, make one up */
    if (bi.biSizeImage == 0){
        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

        if (biStyle != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    /*  realloc the buffer big enough to hold all the bits */
    dwLen = bi.biSize + PaletteSize(&bi) + bi.biSizeImage;
    if (h = GlobalReAlloc(hdib,dwLen,0))
        hdib = h;
    else{
        GlobalFree(hdib);
        hdib = NULL;

        SelectPalette(hdc,hpal,FALSE);
        ReleaseDC(NULL,hdc);
        return hdib;
    }

    /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
     *  bits this time
     */
    lpbi = (BITMAPINFOHEADER FAR *)GlobalLock(hdib);

    if (GetDIBits( hdc,
                   hbm,
                   0,
                   (WORD)bi.biHeight,
                   (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi),
                   (LPBITMAPINFO)lpbi, DIB_RGB_COLORS) == 0){
         GlobalUnlock(hdib);
         hdib = NULL;
         SelectPalette(hdc,hpal,FALSE);
         ReleaseDC(NULL,hdc);
         return NULL;
    }

    bi = *lpbi;
    GlobalUnlock(hdib);

    SelectPalette(hdc,hpal,FALSE);
    ReleaseDC(NULL,hdc);
    return hdib;




  //## end CDIB::DibFromBitmap%3692465A0117.body
}

HANDLE CDIB::BitmapFromDib (HANDLE hdib, HPALETTE hpal)
{
  //## begin CDIB::BitmapFromDib%3692465A011C.body preserve=yes

    LPBITMAPINFOHEADER  lpbi;
    HPALETTE            hpalT;
    HDC                 hdc;
    HBITMAP             hbm;

    StartWait();

    if (!hdib)
        return NULL;

    lpbi = (BITMAPINFOHEADER FAR *)GlobalLock(hdib);

    if (!lpbi)
        return NULL;

    hdc = GetDC(NULL);

    if (hpal){
        hpalT = SelectPalette(hdc,hpal,FALSE);
        RealizePalette(hdc);     // GDI Bug...????
    }

    hbm = CreateDIBitmap(hdc,
                (LPBITMAPINFOHEADER)lpbi,
                (LONG)CBM_INIT,
                (LPSTR)lpbi + lpbi->biSize + PaletteSize(lpbi),
                (LPBITMAPINFO)lpbi,
                DIB_RGB_COLORS );

    if (hpal)
        SelectPalette(hdc,hpalT,FALSE);

    ReleaseDC(NULL,hdc);
    GlobalUnlock(hdib);

    EndWait();

    return hbm;

  //## end CDIB::BitmapFromDib%3692465A011C.body
}

BOOL CDIB::DibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, LONG rop)
{
  //## begin CDIB::DibBlt%3692465A0123.body preserve=yes

    LPBITMAPINFOHEADER   lpbi;
    LPSTR                pBuf;

    if (!hdib)
        return PatBlt(hdc,x0,y0,dx,dy,rop);

    lpbi = (BITMAPINFOHEADER FAR *)GlobalLock(hdib);

    if (!lpbi)
        return FALSE;

    pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi);
    SetDIBitsToDevice (hdc, x0, y0, dx, dy,
                       x1,y1,
                       x1,
                       dy,
                       pBuf, (LPBITMAPINFO)lpbi,
                       DIB_RGB_COLORS );

    GlobalUnlock(hdib);
    return TRUE;




  //## end CDIB::DibBlt%3692465A0123.body
}

BOOL CDIB::StretchDibBlt (HDC hdc, int x, int y, int dx, int dy, HANDLE hdib, int x0, int y0, int dx0, int dy0, LONG rop)
{
  //## begin CDIB::StretchDibBlt%3692465A012D.body preserve=yes

    LPBITMAPINFOHEADER lpbi;
    LPSTR        pBuf;
    BOOL         f;

    if (!hdib)
        return PatBlt(hdc,x,y,dx,dy,rop);

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

    if (!lpbi)
        return FALSE;

    pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi);

    f = StretchDIBits ( hdc,
                        x, y,
                        dx, dy,
                        x0, y0,
                        dx0, dy0,
                        pBuf, (LPBITMAPINFO)lpbi,
                        DIB_RGB_COLORS,
                        rop);

    GlobalUnlock(hdib);
    return f;




  //## end CDIB::StretchDibBlt%3692465A012D.body
}

BOOL CDIB::DibInfo (HANDLE hdib, LPBITMAPINFOHEADER lpbi)
{
  //## begin CDIB::DibInfo%3692465A0139.body preserve=yes
	HANDLE hbi = hdib;
    if (hbi){
        *lpbi = *(LPBITMAPINFOHEADER)GlobalLock (hbi);

        /* fill in the default fields */
        if (lpbi->biSize != sizeof (BITMAPCOREHEADER)){
            if (lpbi->biSizeImage == 0L)
                lpbi->biSizeImage =
                    WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;

            if (lpbi->biClrUsed == 0L)
                lpbi->biClrUsed = DibNumColors (lpbi);
        }
        GlobalUnlock (hbi);
        return TRUE;
    }
    return FALSE;
  //## end CDIB::DibInfo%3692465A0139.body
}

HANDLE CDIB::ReadDibBitmapInfo (int fh)
{
  //## begin CDIB::ReadDibBitmapInfo%3692465A013C.body preserve=yes
    DWORD     off;
    HANDLE    hbi = NULL;
    int       size;
    int       i;
    WORD      nNumColors;

    RGBQUAD FAR       *pRgb;
    BITMAPINFOHEADER   bi;
    BITMAPCOREHEADER   bc;
    LPBITMAPINFOHEADER lpbi;
    BITMAPFILEHEADER   bf;
    DWORD              dwWidth = 0;
    DWORD              dwHeight = 0;
    WORD               wPlanes, wBitCount;

    if (fh == -1)
        return NULL;

    /* Reset file pointer and read file header */
    off = _llseek(fh, 0L, SEEK_CUR);
    if (sizeof (bf) != _lread (fh, (LPSTR)&bf, sizeof (bf)))
        return FALSE;

    /* Do we have a RC HEADER? */
    if (!ISDIB (bf.bfType)) {
        bf.bfOffBits = 0L;
        _llseek (fh, off, SEEK_SET);
    }
    if (sizeof (bi) != _lread (fh, (LPSTR)&bi, sizeof(bi)))
        return FALSE;

    nNumColors = DibNumColors (&bi);

    /* Check the nature (BITMAPINFO or BITMAPCORE) of the info. block
     * and extract the field information accordingly. If a BITMAPCOREHEADER,
     * transfer it's field information to a BITMAPINFOHEADER-style block
     */
    switch (size = (int)bi.biSize){
        case sizeof (BITMAPINFOHEADER):
            break;

        case sizeof (BITMAPCOREHEADER):

            bc = *(BITMAPCOREHEADER*)&bi;

            dwWidth   = (DWORD)bc.bcWidth;
            dwHeight  = (DWORD)bc.bcHeight;
            wPlanes   = bc.bcPlanes;
            wBitCount = bc.bcBitCount;

            bi.biSize               = sizeof(BITMAPINFOHEADER);
            bi.biWidth              = dwWidth;
            bi.biHeight             = dwHeight;
            bi.biPlanes             = wPlanes;
            bi.biBitCount           = wBitCount;

            bi.biCompression        = BI_RGB;
            bi.biSizeImage          = 0;
            bi.biXPelsPerMeter      = 0;
            bi.biYPelsPerMeter      = 0;
            bi.biClrUsed            = nNumColors;
            bi.biClrImportant       = nNumColors;

            _llseek (fh, (LONG)sizeof (BITMAPCOREHEADER) - sizeof (BITMAPINFOHEADER), SEEK_CUR);
            break;

        default:
            /* Not a DIB! */
            return NULL;
    }

    /*  Fill in some default values if they are zero */
    if (bi.biSizeImage == 0){
        bi.biSizeImage = WIDTHBYTES ((DWORD)bi.biWidth * bi.biBitCount)
                         * bi.biHeight;
    }
    if (bi.biClrUsed == 0)
        bi.biClrUsed = DibNumColors(&bi);

    /* Allocate for the BITMAPINFO structure and the color table. */
    hbi = GlobalAlloc (GHND, (LONG)bi.biSize + nNumColors * sizeof(RGBQUAD));
    if (!hbi)
        return NULL;
    lpbi = (LPBITMAPINFOHEADER)GlobalLock (hbi);
    *lpbi = bi;

    /* Get a pointer to the color table */
    pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + bi.biSize);
    if (nNumColors){
        if (size == sizeof(BITMAPCOREHEADER)){
            /* Convert a old color table (3 byte RGBTRIPLEs) to a new
             * color table (4 byte RGBQUADs)
             */
            _lread (fh, (LPSTR)pRgb, nNumColors * sizeof(RGBTRIPLE));

            for (i = nNumColors - 1; i >= 0; i--){
                RGBQUAD rgb;

                rgb.rgbRed      = ((RGBTRIPLE FAR *)pRgb)[i].rgbtRed;
                rgb.rgbBlue     = ((RGBTRIPLE FAR *)pRgb)[i].rgbtBlue;
                rgb.rgbGreen    = ((RGBTRIPLE FAR *)pRgb)[i].rgbtGreen;
                rgb.rgbReserved = (BYTE)0;

                pRgb[i] = rgb;
            }
        }
        else
            _lread(fh,(LPSTR)pRgb,nNumColors * sizeof(RGBQUAD));
    }

    if (bf.bfOffBits != 0L)
        _llseek(fh,off + bf.bfOffBits,SEEK_SET);

    GlobalUnlock(hbi);
    return hbi;





  //## end CDIB::ReadDibBitmapInfo%3692465A013C.body
}

BOOL CDIB::DrawBitmap (HDC hdc, int x, int y, HBITMAP hbm, DWORD rop)
{
  //## begin CDIB::DrawBitmap%3692465A013E.body preserve=yes
    HDC       hdcBits;
    BITMAP    bm;
    BOOL      f;

    if (!hdc || !hbm)
        return FALSE;

    hdcBits = CreateCompatibleDC(hdc);
    GetObject(hbm,sizeof(BITMAP),(LPSTR)&bm);
    SelectObject(hdcBits,hbm);
    f = BitBlt(hdc,0,0,bm.bmWidth,bm.bmHeight,hdcBits,0,0,rop);
    DeleteDC(hdcBits);

    return f;

  //## end CDIB::DrawBitmap%3692465A013E.body
}

LPSTR CDIB::FindDIBBits (LPSTR lpbi)
{
  //## begin CDIB::FindDIBBits%3692465A0144.body preserve=yes
   return (lpbi + *(LPDWORD)lpbi + PaletteSize (lpbi));
  //## end CDIB::FindDIBBits%3692465A0144.body
}

DWORD CDIB::DIBHeight (LPSTR lpDIB)
{
  //## begin CDIB::DIBHeight%3692465A0146.body preserve=yes

   LPBITMAPINFOHEADER lpbmi;
   LPBITMAPCOREHEADER lpbmc;

   lpbmi = (LPBITMAPINFOHEADER) lpDIB;
   lpbmc = (LPBITMAPCOREHEADER) lpDIB;

   if (lpbmi->biSize == sizeof (BITMAPINFOHEADER))
      return lpbmi->biHeight;
   else
      return (DWORD) lpbmc->bcHeight;




  //## end CDIB::DIBHeight%3692465A0146.body
}

int CDIB::DIBHeight (HANDLE hDib)
{
  //## begin CDIB::DIBHeight%3692465A0148.body preserve=yes
	HPSTR	lpDib;
	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}
	
	int iHeight 	= (int)DIBHeight (lpDib);

	GlobalUnlock(hDib);
	return iHeight;

  //## end CDIB::DIBHeight%3692465A0148.body
}

DWORD CDIB::DIBWidth (LPSTR lpDIB)
{
  //## begin CDIB::DIBWidth%3692465A014A.body preserve=yes

   LPBITMAPINFOHEADER lpbmi;
   LPBITMAPCOREHEADER lpbmc;

   lpbmi = (LPBITMAPINFOHEADER) lpDIB;
   lpbmc = (LPBITMAPCOREHEADER) lpDIB;

   if (lpbmi->biSize == sizeof (BITMAPINFOHEADER))
      return lpbmi->biWidth;
   else
      return (DWORD) lpbmc->bcWidth;

  //## end CDIB::DIBWidth%3692465A014A.body
}

int CDIB::DIBWidth (HANDLE hDib)
{
  //## begin CDIB::DIBWidth%3692465A014C.body preserve=yes

	HPSTR	lpDib;
	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}
	
	int iWidth 	= (int)DIBWidth (lpDib);

	GlobalUnlock(hDib);
	return iWidth;

  //## end CDIB::DIBWidth%3692465A014C.body
}

HANDLE CDIB::CreateDib (int width, int height, DWORD biStyle, WORD biBits, HPALETTE hpal)
{
  //## begin CDIB::CreateDib%3692465A014E.body preserve=yes
    BITMAPINFOHEADER     bi;
    BITMAPINFOHEADER FAR *lpbi;
    DWORD                dwLen;
    HANDLE               hdib;
    HANDLE               h;
    HDC                  hdc;
    if (hpal == NULL)
        hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

//    GetObject(hbm,sizeof(bm),(LPSTR)&bm);
    if (biBits == 0)
//        biBits =  bm.bmPlanes * bm.bmBitsPixel;
		return NULL;
		

    bi.biSize               = sizeof(BITMAPINFOHEADER);
//    bi.biWidth              = bm.bmWidth;
//    bi.biHeight             = bm.bmHeight;
    bi.biWidth              = width;
    bi.biHeight             = height;

    bi.biPlanes             = 1;
    bi.biBitCount           = biBits;
    bi.biCompression        = biStyle;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    dwLen  = bi.biSize + PaletteSize(&bi);

    hdc = GetDC(NULL);
    hpal = SelectPalette(hdc,hpal,FALSE);
         RealizePalette(hdc);

    hdib = GlobalAlloc(GHND,dwLen);

    if (!hdib){
        SelectPalette(hdc,hpal,FALSE);
        ReleaseDC(NULL,hdc);
        return NULL;
    }

    lpbi = (BITMAPINFOHEADER FAR *)GlobalLock(hdib);

    *lpbi = bi;

    /*  call GetDIBits with a NULL lpBits param, so it will calculate the
     *  biSizeImage field for us
     */
//    GetDIBits(hdc, hbm, 0, (WORD)bi.biHeight,
//        NULL, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

    bi = *lpbi;
    GlobalUnlock(hdib);

    /* If the driver did not fill in the biSizeImage field, make one up */
    if (bi.biSizeImage == 0){
//        bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;
        bi.biSizeImage = WIDTHBYTES((DWORD)width * biBits) * height;
        if (biStyle != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    /*  realloc the buffer big enough to hold all the bits */
    dwLen = bi.biSize + PaletteSize(&bi) + bi.biSizeImage;
    if (h = GlobalReAlloc(hdib,dwLen,0))
        hdib = h;
    else{
        GlobalFree(hdib);
        hdib = NULL;

        SelectPalette(hdc,hpal,FALSE);
        ReleaseDC(NULL,hdc);
        return hdib;
    }

    /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
     *  bits this time
     */
    hpal	= SelectPalette(hdc,hpal,FALSE);

    this->SetDIBPalette(hdib, hpal);
	lpbi = (BITMAPINFOHEADER FAR *)GlobalLock(hdib);
	/*
    if (GetDIBits( hdc,
                   hbm,
                   0,
                   (WORD)bi.biHeight,
                   (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi),
                   (LPBITMAPINFO)lpbi, DIB_RGB_COLORS) == 0){
         GlobalUnlock(hdib);
         hdib = NULL;
         SelectPalette(hdc,hpal,FALSE);
         ReleaseDC(NULL,hdc);
		 DeleteObject(hbm);
         return NULL;
    }
	*/
	
//	DeleteObject(hbm);
    bi = *lpbi;
    GlobalUnlock(hdib);

//    SelectPalette(hdc,hpal,FALSE);
    ReleaseDC(NULL,hdc);
    return hdib;

  //## end CDIB::CreateDib%3692465A014E.body
}

HPALETTE CDIB::Create8BitGrayLinearPalette ()
{
  //## begin CDIB::Create8BitGrayLinearPalette%3692465A0154.body preserve=yes

    LOGPALETTE          *pPal;
    HPALETTE            hpal = NULL;
    WORD                nNumColors;
//    BYTE                red;
//    BYTE                green;
//    BYTE                blue;
    WORD                i;
//    RGBQUAD        FAR *pRgb;


    nNumColors = 256;

    if (nNumColors){
        /* Allocate for the logical palette structure */
        pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        if (!pPal)
            return NULL;

        pPal->palNumEntries = nNumColors;
        pPal->palVersion    = PALVERSION;

        /* Fill in the palette entries with a linear gray scale.
         */
        for (i = 0; i < nNumColors; i++){
            pPal->palPalEntry[i].peRed   = (BYTE)i;
            pPal->palPalEntry[i].peGreen = (BYTE)i;
            pPal->palPalEntry[i].peBlue  = (BYTE)i;
            pPal->palPalEntry[i].peFlags = (BYTE)0;
        }
        hpal = CreatePalette(pPal);
        LocalFree((HANDLE)pPal);
    }
    return hpal;
  //## end CDIB::Create8BitGrayLinearPalette%3692465A0154.body
}

BOOL CDIB::SetDIBPalette (HANDLE hDib, HPALETTE hPal)
{
  //## begin CDIB::SetDIBPalette%3692465A0155.body preserve=yes
	LPBITMAPINFOHEADER lpbi;
//    LOGPALETTE          *pPal;
//    HPALETTE            hpal = NULL;
    WORD                nNumColors;
//    BYTE                red;
//    BYTE                green;
//    BYTE                blue;
    WORD                i;
    RGBQUAD        FAR *pRgb;

	lpbi	= (LPBITMAPINFOHEADER)GlobalLock(hDib);

    if (!lpbi)
	{
		GlobalUnlock(hDib);
        return NULL;
	}

    if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
	{
		GlobalUnlock(hDib);
        return NULL;
	}

    /* Get a pointer to the color table and the number of colors in it */
    pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + (WORD)lpbi->biSize);
    nNumColors = DibNumColors(lpbi);

    if (nNumColors<=256){
	LPPALETTEENTRY lppe	= new PALETTEENTRY[256];

	UINT uPalColors	=  GetPaletteEntries(
    hPal,			// handle of logical color palette
    0,				// first entry to retrieve
    nNumColors,		// number of entries to retrieve
    lppe 			// address of array receiving entries
   );

        // Fill in the palette data from lppe.

   for (i = 0; i < nNumColors; i++){
            pRgb[i].rgbRed		= lppe[i].peRed;
            pRgb[i].rgbGreen	= lppe[i].peGreen;
            pRgb[i].rgbBlue		= lppe[i].peBlue;
        }
	delete []lppe;
   }
	GlobalUnlock(hDib);
	return TRUE;
  //## end CDIB::SetDIBPalette%3692465A0155.body
}

BOOL CDIB::DIBCurveMapping (HANDLE hDibDst, HANDLE hDibSrc, BYTE  Curve[256])
{
  //## begin CDIB::DIBCurveMapping%3692465A015A.body preserve=yes

	HPSTR	lpDibDst, lpDibSrc;
	int	    iHeight, iWidth;
	long	i, l;
    LPBITMAPINFOHEADER  lpbi;
    WORD	m;
	
	if ( !hDibDst)
	 return 0;
	if ( !hDibSrc)
	 return 0;
	
	lpDibDst = (HPSTR)GlobalLock ( hDibDst );
	if (lpDibDst==NULL)
	{
	 GlobalUnlock(hDibDst);
	 return 0;
	}
	lpDibSrc = (HPSTR)GlobalLock ( hDibSrc );
	if (lpDibSrc==NULL)
	{
	 GlobalUnlock(hDibDst);
	 GlobalUnlock(hDibSrc);
	 return 0;
	}
	
	iWidth 	= (int)DIBWidth (lpDibSrc);
	iHeight	= (int)DIBHeight (lpDibSrc);
	
 	if(( iWidth!=(int)DIBWidth (lpDibDst))||(iHeight!=(int)DIBHeight(lpDibDst)))
 	{
 	 GlobalUnlock(hDibDst);
	 GlobalUnlock(hDibSrc);
	 return 0;
 	}

	lpbi = (LPBITMAPINFOHEADER)lpDibSrc; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;

 	lpDibSrc = FindDIBBits (lpDibSrc);
 	lpDibDst = FindDIBBits (lpDibDst);
 	
 	for ( i = 0; i < l; i++ )
 	{
 	 m = (WORD)((BYTE)lpDibSrc[i]);
 	 lpDibDst[i] = Curve[m];
 	}
 	GlobalUnlock(hDibDst);
	GlobalUnlock(hDibSrc);
	return 1;
  //## end CDIB::DIBCurveMapping%3692465A015A.body
}

BOOL CDIB::DIBCurveMapping (HANDLE hDibDst, HANDLE hDibSrc, BYTE  CurveR[256], BYTE  CurveG[256], BYTE  CurveB[256])
{
  //## begin CDIB::DIBCurveMapping%38AA171501AD.body preserve=yes

	HPSTR	lpDibDst, lpDibSrc;
	int	    iHeight, iWidth;
	long	i, l;
    LPBITMAPINFOHEADER  lpbi;
//    WORD	m;
	
	if ( !hDibDst)
	 return 0;
	if ( !hDibSrc)
	 return 0;
	
	lpDibDst = (HPSTR)GlobalLock ( hDibDst );
	if (lpDibDst==NULL)
	{
	 GlobalUnlock(hDibDst);
	 return 0;
	}
	lpDibSrc = (HPSTR)GlobalLock ( hDibSrc );
	if (lpDibSrc==NULL)
	{
	 GlobalUnlock(hDibDst);
	 GlobalUnlock(hDibSrc);
	 return 0;
	}
	
	iWidth 	= (int)DIBWidth (lpDibSrc);
	iHeight	= (int)DIBHeight (lpDibSrc);
	
 	if(( iWidth!=(int)DIBWidth (lpDibDst))||(iHeight!=(int)DIBHeight(lpDibDst)))
 	{
 	 GlobalUnlock(hDibDst);
	 GlobalUnlock(hDibSrc);
	 return 0;
 	}

	lpbi = (LPBITMAPINFOHEADER)lpDibSrc; 	
//    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);
 	lpDibSrc = FindDIBBits (lpDibSrc);
 	lpDibDst = FindDIBBits (lpDibDst);
 	
// 	for ( i = 0; i < l; i++ )
// 	{
// 	 m = (WORD)((BYTE)lpDibSrc[i]);
// 	 lpDibDst[i] = Curve[m];
// 	}


	WORD b,g,r;
	long k0, k1;
 	for ( i = 0; i < iHeight; i++ )
 	 {
	  k0 = k1 =0;
 	  for ( long j = 0; j <iWidth; j++ )
 		{
 	 		b = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		g = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		r = (WORD)((BYTE)lpDibSrc[k0++]);

 	 		lpDibDst[k1++] = (BYTE)CurveB[b];
 	 		lpDibDst[k1++] = (BYTE)CurveB[g];
	 		lpDibDst[k1++] = (BYTE)CurveB[r];
    	}
      lpDibDst += l;
      lpDibSrc += l;
 	 }

 	GlobalUnlock(hDibDst);
	GlobalUnlock(hDibSrc);
	return 1;


  //## end CDIB::DIBCurveMapping%38AA171501AD.body
}

BOOL CDIB::ColorDibToBWDib (HANDLE hDibDst, HANDLE hDibSrc)
{
  //## begin CDIB::ColorDibToBWDib%3692465A015E.body preserve=yes

	HPSTR	lpDibDst, lpDibSrc;
	int	    iHeight, iWidth;
	register long	i, l, j, k0, k1;
    LPBITMAPINFOHEADER  lpbi;
    WORD	r, g, b, brt;
	
	if ( !hDibDst)
	 return 0;
	if ( !hDibSrc)
	 return 0;
	
	lpDibDst = (HPSTR)GlobalLock ( hDibDst );
	if (lpDibDst==NULL)
	{
	 GlobalUnlock(hDibDst);
	 return 0;
	}
	lpDibSrc = (HPSTR)GlobalLock ( hDibSrc );
	if (lpDibSrc==NULL)
	{
	 GlobalUnlock(hDibDst);
	 GlobalUnlock(hDibSrc);

	 return 0;
	}
	
	iWidth 	= (int)DIBWidth (lpDibSrc);
	iHeight	= (int)DIBHeight (lpDibSrc);
	
 	if(( iWidth!=(int)DIBWidth (lpDibDst))||(iHeight!=(int)DIBHeight(lpDibDst)))
 	{
 	 GlobalUnlock(hDibDst);
	 GlobalUnlock(hDibSrc);
	 return 0;
 	}

	lpbi = (LPBITMAPINFOHEADER)lpDibSrc; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

 	lpDibSrc = FindDIBBits (lpDibSrc);
 	lpDibDst = FindDIBBits (lpDibDst);
 	float f = (float)(R_Brightness + G_Brightness + B_Brightness);
	float fR_Weight	= (float)R_Brightness / f;
	float fG_Weight	= (float)G_Brightness / f;
	float fB_Weight	= (float)B_Brightness / f;


 	for ( i = 0; i < iHeight; i++ )
 	 {
 	  k0 = k1 = 0;
 	  for ( j = 0; j <iWidth; j++ )
 		{
 	 		b = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		g = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		r = (WORD)((BYTE)lpDibSrc[k0++]);
            brt = (WORD)((float)r * fR_Weight + (float)g * fG_Weight + (float)b * fB_Weight);
			brt = BOUND(brt,0,255);

 	 		lpDibDst[k1++] = (BYTE)brt;
 	 		lpDibDst[k1++] = (BYTE)brt;
	 		lpDibDst[k1++] = (BYTE)brt;
    	}
      lpDibDst += l;
      lpDibSrc += l;
 	 }
 	GlobalUnlock(hDibDst);
	GlobalUnlock(hDibSrc);
	return 1;
  //## end CDIB::ColorDibToBWDib%3692465A015E.body
}

HANDLE CDIB::DibRotate90 (HANDLE hDibSrc)
{
  //## begin CDIB::DibRotate90%3692465A0161.body preserve=yes
	HPSTR	lpDibDst, lpDibSrc;
	HANDLE  hDibDst = 0;
	int	    iHeight, iWidth;
	register long	i, l, j, k0, k1, lDest;
    LPBITMAPINFOHEADER  lpbi;
    WORD	r, g, b;
	
	if ( !hDibSrc)
	 return 0;

	lpDibSrc = (HPSTR)GlobalLock ( hDibSrc );
	if (lpDibSrc==NULL)
	{
	 GlobalUnlock(hDibSrc);
	 return 0;
	}

	iWidth 	= (int)DIBWidth (lpDibSrc);
	iHeight	= (int)DIBHeight (lpDibSrc);

	lpbi = (LPBITMAPINFOHEADER)lpDibSrc; 	

	hDibDst = CreateDib ( iHeight, iWidth, 0, lpbi->biBitCount, NULL);
	
	if ( hDibDst == NULL )
	{
	 GlobalUnlock(hDibSrc);
	 return 0;
	}
	
	lpDibDst = (HPSTR)GlobalLock ( hDibDst );
	if (lpDibDst==NULL)
	{
	  GlobalFree(hDibDst);
	  return 0;
	}
	
	
// 	if(( iWidth!=(int)DIBWidth (lpDibDst))||(iHeight!=(int)DIBHeight(lpDibDst)))
// 	{
// 	 GlobalUnlock(hDibDst);
//	 GlobalUnlock(hDibSrc);
//	 return 0;
// 	}

    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);
    lDest    = WIDTHBYTES(lpbi->biHeight*lpbi->biBitCount);

 	lpDibSrc = FindDIBBits (lpDibSrc);
 	lpDibDst = FindDIBBits (lpDibDst);
 	
 	for ( i = 0; i < iHeight; i++ )
 	 {
 	  k0 = 0;
 	  for ( j = 0; j <iWidth; j++ )
 		{
 	 		b = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		g = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		r = (WORD)((BYTE)lpDibSrc[k0++]);
		    k1 = (iWidth - j - 1)*lDest + i*3;

 	 		lpDibDst[k1++] = (BYTE)b;
 	 		lpDibDst[k1++] = (BYTE)g;
	 		lpDibDst[k1++] = (BYTE)r;
    	}
      lpDibSrc += l;
 	 }
 	GlobalUnlock(hDibDst);
	GlobalUnlock(hDibSrc);
	return hDibDst;
  //## end CDIB::DibRotate90%3692465A0161.body
}

HANDLE CDIB::CopyDib (HANDLE hDibSrc)
{
  //## begin CDIB::CopyDib%3692465A0163.body preserve=yes
	HPSTR	lpDibDst, lpDibSrc;
	HANDLE  hDibDst = 0;
	int	    iHeight, iWidth;
	register long	i, l, j, k0, k1;
    LPBITMAPINFOHEADER  lpbi;
    WORD	r, g, b;
	
	if ( !hDibSrc)
	 return 0;

	lpDibSrc = (HPSTR)GlobalLock ( hDibSrc );
	if (lpDibSrc==NULL)
	{
	 GlobalUnlock(hDibSrc);
	 return 0;
	}

	iWidth 	= (int)DIBWidth (lpDibSrc);
	iHeight	= (int)DIBHeight (lpDibSrc);

	lpbi = (LPBITMAPINFOHEADER)lpDibSrc; 	

	hDibDst = CreateDib ( iWidth, iHeight, 0, lpbi->biBitCount, NULL);
	
	if ( hDibDst == NULL )
	{
	 GlobalUnlock(hDibSrc);
	 return 0;
	}
	
	lpDibDst = (HPSTR)GlobalLock ( hDibDst );
	if (lpDibDst==NULL)
	{
	  GlobalFree(hDibDst);
	  return 0;
	}
	
	

    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

 	lpDibSrc = FindDIBBits (lpDibSrc);
 	lpDibDst = FindDIBBits (lpDibDst);
 	
 	for ( i = 0; i < iHeight; i++ )
 	 {
 	  k0 = k1 = 0;
 	  for ( j = 0; j <iWidth; j++ )
 		{
 	 		b = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		g = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		r = (WORD)((BYTE)lpDibSrc[k0++]);

 	 		lpDibDst[k1++] = (BYTE)b;
 	 		lpDibDst[k1++] = (BYTE)g;
	 		lpDibDst[k1++] = (BYTE)r;
    	}
      lpDibDst += l;
      lpDibSrc += l;
 	 }
 	GlobalUnlock(hDibDst);
	GlobalUnlock(hDibSrc);
	return hDibDst;
  //## end CDIB::CopyDib%3692465A0163.body
}

HANDLE CDIB::CropDIB (HANDLE hDibSrc, int x0, int y0, int x1, int y1)
{
  //## begin CDIB::CropDIB%3692465A0165.body preserve=yes
  HANDLE hDibDst = NULL;

  RECT	rcCrop;
  RECT	rc0;

  HPSTR					lpDibDst, lpDibSrc;
  LPBITMAPINFOHEADER	lpbi;
  WORD	r, g, b;
  long	l, lDest;

  lpDibSrc = (HPSTR)GlobalLock ( hDibSrc );

  if (lpDibSrc==NULL)
  {
	 GlobalUnlock(hDibSrc);
	 return 0;
  }

  int iWidthSrc 	= (int)DIBWidth (lpDibSrc);
  int iHeightSrc	= (int)DIBHeight (lpDibSrc);

  rcCrop.left	= x0;
  rcCrop.top	= y0;
  rcCrop.right	= x1;
  rcCrop.bottom	= y1;

  rc0.left		= 0;
  rc0.top		= 0;
  rc0.right	    = iWidthSrc  - 1;
  rc0.bottom	= iHeightSrc - 1;

  IntersectRect( &rcCrop, &rc0, &rcCrop);

  lpbi = (LPBITMAPINFOHEADER)lpDibSrc; 	

  int iWidthDst 	= rcCrop.right  - rcCrop.left;
  int iHeightDst	= rcCrop.bottom - rcCrop.top;


  hDibDst = CreateDib ( iWidthDst,
  						iHeightDst,
						0, lpbi->biBitCount, NULL);
	
  if ( hDibDst == NULL )
  {
   GlobalUnlock(hDibSrc);
   return 0;
  }
	
  lpDibDst = (HPSTR)GlobalLock ( hDibDst );
  if (lpDibDst==NULL)
  {
	  GlobalFree(hDibDst);
      GlobalUnlock(hDibSrc);
	  return 0;
  }
	
	
  l			= WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);
  lDest		= WIDTHBYTES(iWidthDst*lpbi->biBitCount);

  lpDibSrc = FindDIBBits (lpDibSrc);
  lpDibDst = FindDIBBits (lpDibDst);

  long k0, k1, m;

  m = 0;
  for ( int i = rcCrop.top; i < rcCrop.bottom; i++)
  {
	  k0	= (iHeightSrc - i - 1)*l + rcCrop.left * 3;
	  k1	= (iHeightDst - m - 1)*lDest;

	  for( int j = rcCrop.left; j < rcCrop.right; j++ )
	  {
 		b = (WORD)((BYTE)lpDibSrc[k0++]);
 		g = (WORD)((BYTE)lpDibSrc[k0++]);
 		r = (WORD)((BYTE)lpDibSrc[k0++]);

		lpDibDst[k1++] = (BYTE)b;
 	 	lpDibDst[k1++] = (BYTE)g;
	 	lpDibDst[k1++] = (BYTE)r;
	  }
	  m++;
  }
 	 
 	GlobalUnlock(hDibDst);
	GlobalUnlock(hDibSrc);
  return hDibDst;
  //## end CDIB::CropDIB%3692465A0165.body
}

DWORD CDIB::lread (int fh, void* pv, DWORD ul)
{
  //## begin CDIB::lread%3692465A016B.body preserve=yes




    DWORD     ulT = ul;
    BYTE huge *hp = (BYTE huge *)pv;

    while (ul > (DWORD)MAXREAD) {
        if (_lread(fh, (LPSTR)hp, (WORD)MAXREAD) != MAXREAD)
                return 0;
        ul -= MAXREAD;
        hp += MAXREAD;
    }
    if (_lread(fh, (LPSTR)hp, (WORD)ul) != (WORD)ul)
        return 0;
    return ulT;




  //## end CDIB::lread%3692465A016B.body
}

DWORD CDIB::lwrite (int fh, void* pv, DWORD ul)
{
  //## begin CDIB::lwrite%3692465A016F.body preserve=yes




    DWORD     ulT = ul;
    BYTE huge *hp = (BYTE huge *)pv;

    while (ul > MAXREAD) {
        if (_lwrite(fh, (LPSTR)hp, (WORD)MAXREAD) != MAXREAD)
                return 0;
        ul -= MAXREAD;
        hp += MAXREAD;
    }
    if (_lwrite(fh, (LPSTR)hp, (WORD)ul) != (WORD)ul)
        return 0;
    return ulT;




  //## end CDIB::lwrite%3692465A016F.body
}

BOOL CDIB::ThresholdingDIB (HANDLE hDib, int nThreshold, BYTE nUpperColor, BYTE nLowerColor, float* pfDensity)
{
  //## begin CDIB::ThresholdingDIB%369247EA0086.body preserve=yes

	HPSTR	lpDib;
	int	    iHeight, iWidth;
	long	i, l;
    LPBITMAPINFOHEADER  lpbi;
    BYTE	m, t = (BYTE)nThreshold;
	
	if ( !hDib)
	 return 0;

	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}
	
	iWidth 	= (int)DIBWidth (lpDib);
	iHeight	= (int)DIBHeight (lpDib);
	
	lpbi = (LPBITMAPINFOHEADER)lpDib; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;

 	lpDib = FindDIBBits (lpDib);
 	
	if ( pfDensity == NULL )
 		for ( i = 0; i < l; i++ )
 		{
 		m = ((BYTE)lpDib[i]);
		lpDib[i] = (m>=t?  nUpperColor:nLowerColor);
 		}
	else
	{
		int nTotal=0, nUpper=0;
 		for ( i = 0; i < l; i++ )
 		{
 			m = ((BYTE)lpDib[i]);
//			if (m>=t)
			if (m > t)
			{
				lpDib[i] = nUpperColor;
				nUpper++;
			}
			else
			{
				lpDib[i] = nLowerColor;
 			}
			nTotal++;
		}
		if (nTotal != 0 )
		{
//			*pfDensity	= (float)(1.0 - (nUpper / nTotal));         //强制转换  Modified by yangjz  99/4/20 (Warning)
			*pfDensity	= (float)(1.0 - ((float)(nUpper) /(float)(nTotal)));        
		}
	}
 	GlobalUnlock(hDib);
	return 1;

  //## end CDIB::ThresholdingDIB%369247EA0086.body
}

BOOL CDIB::MaxAndMin (HANDLE hDib, int& nMax, int& nMin)
{
  //## begin CDIB::MaxAndMin%369FC6FD00D7.body preserve=yes
	HPSTR	lpDib;
	int	    iHeight, iWidth;
	long	i, l;
    LPBITMAPINFOHEADER  lpbi;
//    BYTE	t = (BYTE)nThreshold;
	int		iMax = 0, iMin = 255, m;
	
	if ( !hDib)
	 return 0;

	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}
	
	iWidth 	= (int)DIBWidth (lpDib);
	iHeight	= (int)DIBHeight (lpDib);
	
	lpbi = (LPBITMAPINFOHEADER)lpDib; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;

 	lpDib = FindDIBBits (lpDib);
 	
 	for ( i = 0; i < l; i++ )
 	{
 	 m = (int)((BYTE)lpDib[i]);
	 nMax = max(nMax, m);
 	 nMin = min(nMin, m);
 	}
 	GlobalUnlock(hDib);
	return 1;
  //## end CDIB::MaxAndMin%369FC6FD00D7.body
}

BOOL CDIB::Histogram (HANDLE hDib, DWORD pHistoR[256], DWORD pHistoG[256], DWORD pHistoB[256])
{
  //## begin CDIB::Histogram%375E96EE0205.body preserve=yes
#define USEFUL
#ifdef USEFUL
	HPSTR	lpDib;
	int	    iHeight, iWidth;
	long	i, l;//, w;
    LPBITMAPINFOHEADER  lpbi;
//    BYTE	t;// = (BYTE)nThreshold;
	
	if ( !hDib)
	 return 0;

	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}

	for ( i = 0; i <256; i++ )
	{
		pHistoR[i] =
		pHistoG[i] =
		pHistoB[i] = 0;
	}
	iWidth 	= (int)DIBWidth (lpDib);
	iHeight	= (int)DIBHeight (lpDib);
	
	lpbi = (LPBITMAPINFOHEADER)lpDib; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

	ASSERT(lpbi->biBitCount == 24);

 	lpDib = FindDIBBits (lpDib);
 	
	WORD b,g,r;
	long k0;//		, k1;

 	for ( i = 0; i < iHeight; i++ )
 	 {
	  k0 = 0;
 	  for ( long j = 0; j <iWidth; j++ )
 		{
 	 		b = (WORD)((BYTE)lpDib[k0++]);
 	 		g = (WORD)((BYTE)lpDib[k0++]);
 	 		r = (WORD)((BYTE)lpDib[k0++]);

			pHistoR[r]	+= 1;
			pHistoG[g]	+= 1;
			pHistoB[b]  += 1;

    	}
      lpDib += l;
 	 }

	GlobalUnlock(hDib);
	return 1;
#else
this->CrossSection ( hDib, 1, pHistoR);
this->CrossSection ( hDib, 1, pHistoG);
this->CrossSection ( hDib, 1, pHistoB);
return 1;
#endif
  //## end CDIB::Histogram%375E96EE0205.body
}

BOOL CDIB::HistogramEqualization (DWORD pHisto[256], BYTE pHEqMap[256], WORD nW, WORD nH)
{
  //## begin CDIB::HistogramEqualization%38AA0DF0014D.body preserve=yes
	int i, j;
	double dHEq[256], dMax = 0.0;
	double dA = ((double)nW*(double)nH);

	for ( i = 0; i < 256; i++ )
	{
		dHEq[i]	= 0.0;
		for ( j = 0; j <= i; j++ )
		{
//			pHEqMap[i]	+= (BYTE)(255.0*(float)pHisto[j]/((float)nW*(float)nH));
			dHEq[i]	+= (double)pHisto[j];
		}
		dHEq[i]	= dHEq[i] / dA;
		if ( dHEq[i] > dMax )
			dMax	= dHEq[i];
	}

//	ASSERT(dMax != 0.0);
//	for ( i = 0; i < 256; i++ )
//	{//Scale dHEq to range [0.0, 1.0] 
//		dHEq[i] /= dMax;
//	}

	for ( i = 0; i < 256; i++ )
	{
		pHEqMap[i]	= (BYTE)(dHEq[i]*255.0);
	}

	return TRUE;
  //## end CDIB::HistogramEqualization%38AA0DF0014D.body
}

HANDLE CDIB::Dib2BWRaw16 (HANDLE hDib0, int* nW, int* nH)
{
  //## begin CDIB::Dib2BWRaw16%387BE8AC01EC.body preserve=yes



	ASSERT(hDib0 != NULL);

	HANDLE hDib	= this->CopyDib(hDib0);
	this->ColorDibToBWDib(hDib, hDib0);

	int nWidth	= this->DIBWidth(hDib);
	int nHeight	= this->DIBHeight(hDib);

	LPBYTE lpDibBits	= (LPBYTE)GlobalLock( hDib );

	lpDibBits	= (LPBYTE)this->FindDIBBits ((LPSTR)lpDibBits);

	HANDLE hRaw	= GlobalAlloc(	GHND, 	
								nWidth * nHeight * sizeof(unsigned _int16)
								);


	unsigned _int16 *lpFrm = (unsigned _int16 *)GlobalLock(hRaw);
	
	int k = 0;
	int m = 0;
//	ASSERT(m_dwWidth%4==0);
	int nWidhtbytes =  WIDTHBYTES(nWidth*24);


	for( int j = 0; j < nHeight; j++ )
	{
		m = ( nHeight - j - 1)*nWidhtbytes;
		
		for( int i = 0; i < nWidth; i++ )
		{
			lpFrm[k++]		= lpDibBits[m];
			m += 3;
		}
	}

	GlobalUnlock( hDib );
	GlobalFree(hDib);
	GlobalUnlock(hRaw);

	if (nW != NULL)
		*nW = nWidth;

	if (nH != NULL)
		*nH	= nHeight;

	return hRaw;


  //## end CDIB::Dib2BWRaw16%387BE8AC01EC.body
}

HANDLE CDIB::Dib2BWRaw8 (HANDLE hDib0, int* nW, int* nH)
{
  //## begin CDIB::Dib2BWRaw8%40EDBCA503C8.body preserve=yes
//HANDLE CDIB::Dib2BWRaw16 (HANDLE hDib0, int* nW, int* nH)
{
	ASSERT(hDib0 != NULL);

	HANDLE hDib	= this->CopyDib(hDib0);
	this->ColorDibToBWDib(hDib, hDib0);

	int nWidth	= this->DIBWidth(hDib);
	int nHeight	= this->DIBHeight(hDib);

	LPBYTE lpDibBits	= (LPBYTE)GlobalLock( hDib );

	lpDibBits	= (LPBYTE)this->FindDIBBits ((LPSTR)lpDibBits);

	HANDLE hRaw	= GlobalAlloc(	GHND, 	
								nWidth * nHeight * sizeof(unsigned _int8)
								);


	unsigned _int8 *lpFrm = (unsigned _int8 *)GlobalLock(hRaw);
	
	int k = 0;
	int m = 0;
//	ASSERT(m_dwWidth%4==0);
	int nWidhtbytes =  WIDTHBYTES(nWidth*24);


	for( int j = 0; j < nHeight; j++ )
	{
		m = ( nHeight - j - 1)*nWidhtbytes;
		
		for( int i = 0; i < nWidth; i++ )
		{
			lpFrm[k++]		= lpDibBits[m];
			m += 3;
		}
	}

	GlobalUnlock( hDib );
	GlobalFree(hDib);
	GlobalUnlock(hRaw);

	if (nW != NULL)
		*nW = nWidth;

	if (nH != NULL)
		*nH	= nHeight;

	return hRaw;
}
  //## end CDIB::Dib2BWRaw8%40EDBCA503C8.body
}

BOOL CDIB::PutPixel (HANDLE hDib, int x, int y, WORD nR, WORD nG, WORD nB)
{
  //## begin CDIB::PutPixel%3907EF13000D.body preserve=yes

  HPSTR					lpDibSrc;
  LPBITMAPINFOHEADER	lpbi;
  long	l;

  int iWidthSrc 	= (int)DIBWidth (hDib);
  int iHeightSrc	= (int)DIBHeight (hDib);

  if ( x >=0 && x < iWidthSrc && y >=0 && y < iHeightSrc )
  {
  	lpDibSrc = (HPSTR)GlobalLock ( hDib );

	if (lpDibSrc==NULL)
	{
		GlobalUnlock(hDib);
		return 0; 
	}

	lpbi	= (LPBITMAPINFOHEADER)lpDibSrc; 	
	l		= WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

	lpDibSrc = FindDIBBits (lpDibSrc);

	long k0;

    k0	= (iHeightSrc - y - 1)*l + x * 3;

 	lpDibSrc[k0++]	= (BYTE)nB;
 	lpDibSrc[k0++]	= (BYTE)nG;
 	lpDibSrc[k0++]	= (BYTE)nR;
   	GlobalUnlock(hDib);
	return TRUE;
  }	
  else
	  return FALSE;
	
  //## end CDIB::PutPixel%3907EF13000D.body
}

BOOL CDIB::GetPixel (HANDLE hDib, int x, int y, WORD* nR, WORD* nG, WORD* nB)
{
  //## begin CDIB::GetPixel%4212BB360375.body preserve=yes

  HPSTR					lpDibSrc;
  LPBITMAPINFOHEADER	lpbi;
  long	l;

  int iWidthSrc 	= (int)DIBWidth (hDib);
  int iHeightSrc	= (int)DIBHeight (hDib);

  if ( x >=0 && x < iWidthSrc && y >=0 && y < iHeightSrc )
  {
  	lpDibSrc = (HPSTR)GlobalLock ( hDib );

	if (lpDibSrc==NULL)
	{
		GlobalUnlock(hDib);
		return 0; 
	}

	lpbi	= (LPBITMAPINFOHEADER)lpDibSrc; 	
	l		= WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

	lpDibSrc = FindDIBBits (lpDibSrc);

	long k0;

    k0	= (iHeightSrc - y - 1)*l + x * 3;

 	*nB = WORD(BYTE(lpDibSrc[k0++]));
 	*nG = WORD(BYTE(lpDibSrc[k0++]));
 	*nR = WORD(BYTE(lpDibSrc[k0++]));
   	GlobalUnlock(hDib);
	return TRUE;
  }	
  else
	  return FALSE;
	
 
  //## end CDIB::GetPixel%4212BB360375.body
}

BOOL CDIB::LowFilterDib (HANDLE hDstDib, HANDLE hSrcDib, double *hMap)
{
  //## begin CDIB::LowFilterDib%39220EBF03BD.body preserve=yes
	HPSTR	lpDibDst, lpDibSrc;
	int	    iHeight, iWidth;
	long	i, l;
    LPBITMAPINFOHEADER  lpbi;
	
	if ( !hDstDib)
	 return 0;
	if ( !hSrcDib)
	 return 0;
	
	lpDibDst = (HPSTR)GlobalLock ( hDstDib );
	if (lpDibDst==NULL)
	{
	 GlobalUnlock(hDstDib);
	 return 0;
	}
	lpDibSrc = (HPSTR)GlobalLock ( hSrcDib );
	if (lpDibSrc==NULL)
	{
	 GlobalUnlock(hDstDib);
	 GlobalUnlock(hSrcDib);
	 return 0;
	}
	
	iWidth 	= (int)DIBWidth (lpDibSrc);
	iHeight	= (int)DIBHeight (lpDibSrc);
	
 	if(( iWidth!=(int)DIBWidth (lpDibDst))||(iHeight!=(int)DIBHeight(lpDibDst)))
 	{
 	 GlobalUnlock(hDstDib);
	 GlobalUnlock(hSrcDib);
	 return 0;
 	}

	lpbi = (LPBITMAPINFOHEADER)lpDibSrc; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);
 	lpDibSrc = FindDIBBits (lpDibSrc);
 	lpDibDst = FindDIBBits (lpDibDst);

	BYTE * lpBuffer_B = (BYTE *) new BYTE[iWidth * iHeight] ;
	BYTE * lpBuffer_G = (BYTE *) new BYTE[iWidth * iHeight] ;
	BYTE * lpBuffer_R = (BYTE *) new BYTE[iWidth * iHeight] ;

	long k;
 	for ( i = 0; i < iHeight; i++ )
	{
	  k=0;
 	  for ( long j = 0; j <iWidth; j++ )
	  {
	
 	 		lpBuffer_B[i * iWidth +j] = ((BYTE)lpDibSrc[k++]);
 	 		lpBuffer_G[i * iWidth +j] = ((BYTE)lpDibSrc[k++]);
 	 		lpBuffer_R[i * iWidth +j] = ((BYTE)lpDibSrc[k++]);
	  }
  //  lpDibDst += l;
      lpDibSrc += l;
	}
	FilterProcess(lpBuffer_B, hMap, iWidth, iHeight) ;//ready do
	FilterProcess(lpBuffer_G, hMap, iWidth, iHeight) ;
	FilterProcess(lpBuffer_R, hMap, iWidth, iHeight) ;

 	for ( i = 0; i < iHeight; i++ )
	{
	  k=0;
 	  for ( long j = 0; j <iWidth; j++ )
	  {
	
			lpDibDst[k++] = lpBuffer_B[i * iWidth +j] ;
			lpDibDst[k++] = lpBuffer_G[i * iWidth +j] ;
			lpDibDst[k++] = lpBuffer_R[i * iWidth +j] ;
	  }
      lpDibDst += l;

	}
 	GlobalUnlock(hDstDib);
	GlobalUnlock(hSrcDib);

	delete [] lpBuffer_B ;
	delete [] lpBuffer_G ;
	delete [] lpBuffer_R ;

	lpBuffer_B = NULL ;
	lpBuffer_G = NULL ;
	lpBuffer_R = NULL ;

	return 1 ;
  //## end CDIB::LowFilterDib%39220EBF03BD.body
}

void CDIB::FilterProcess (BYTE *lpBuffer, double *lpMap, int iWidth, int iHeight)
{
  //## begin CDIB::FilterProcess%39220FA2019D.body preserve=yes
	int iWidth0 = iWidth + 4 ;
	int iHeight0 = iHeight + 4 ;
	int x0, y0, x1, y1 ;

	BYTE * lpTmp = (BYTE *) new BYTE[iWidth0 * iHeight0] ;

	for(int i = 0 ; i < iWidth0 * iHeight0 ; i ++)
		lpTmp[i] = 0 ;
/*
	for(i = 0 ; i < iHeight ; i ++)
	{
		for(int j = 0 ; j < iWidth ; j ++)
			lpTmp[(i+2) * iWidth0 + 2 + j] = lpBuffer[i * iWidth + j] ;
	}
*/

	int i1, i2, j1, j2 ; 
	for(int i = 0 ; i < iHeight0 ; i ++)
	{
		i1 = i + iHeight - 2 ;
		i2 = i1 % iHeight ;
		for(int j = 0 ; j < iWidth0 ; j ++)
		{
			j1 = j + iWidth - 2 ;
			j2 = j1 % iWidth ;
			lpTmp[i * iWidth0 + j] = lpBuffer[i2 * iWidth + j2] ;
		}
	}

	double count = 0.0 ;
	for(int i = 2 ; i < iHeight + 2 ; i ++)
	{
		y0 = i - 2 ;
		y1 = i + 2 ;
		for(int j = 2 ; j < iWidth + 2 ; j ++)
		{
			x0 = j - 2 ;
			x1 = j + 2 ;
			count = 0 ; 
			for(int m = 0 ; m < 5 ; m ++)
			{
				for(int n = 0 ; n < 5 ; n ++)
					count = ((double)count + ((double)lpTmp[(m+y0)*iWidth0+x0+n])*lpMap[m*5+n]) ;   
			}
			lpBuffer[(i-2) * iWidth + (j-2)] = (int)count ;
		}

	}

	if(lpTmp)
	{
		delete [] lpTmp ;
		lpTmp = NULL ;
	}

  //## end CDIB::FilterProcess%39220FA2019D.body
}

int CDIB::CrossSection (HANDLE hDib, WORD nDirection, DWORD pbCrossSection[256])
{
  //## begin CDIB::CrossSection%3B4A74630107.body preserve=yes

	HPSTR	lpDib;
	int	    iHeight, iWidth;
	long	i, l;//, w;
    LPBITMAPINFOHEADER  lpbi;
	int pX[256], pY[256];

//    BYTE	t;// = (BYTE)nThreshold;
	
	if ( !hDib)
	 return 0;

	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}

	iWidth 	= (int)DIBWidth (lpDib);
	iHeight	= (int)DIBHeight (lpDib);
	
	lpbi = (LPBITMAPINFOHEADER)lpDib; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

	ASSERT(lpbi->biBitCount == 24);

 	lpDib = FindDIBBits (lpDib);
 	
//	WORD b,g,r;
	long k0;//		, k1;
	switch(nDirection)
	{
	case 0:
		this->LinearDDA (0, iWidth/2, 255, iWidth-1, pX);
		this->LinearDDA (0, iHeight/2, 255, iHeight/2, pY);
	break;
	case 1:
		this->LinearDDA (0, iWidth/2, 255, iWidth-1, pX);
		this->LinearDDA (0, iHeight/2, 255, iHeight-1, pY);
	break;
	case 2:
		this->LinearDDA (0, iWidth/2, 255, iWidth/2, pX);
		this->LinearDDA (0, iHeight/2, 255, iHeight-1, pY);
	break;
	case 3:
		this->LinearDDA (0, iWidth/2, 255, 0, pX);
		this->LinearDDA (0, iHeight/2, 255, iHeight-1, pY);
	break;
	case 4:
		this->LinearDDA (0, iWidth/2, 255, 0, pX);
		this->LinearDDA (0, iHeight/2, 255, iHeight/2, pY);
	break;
	case 5:
		this->LinearDDA (0, iWidth/2, 255, 0, pX);
		this->LinearDDA (0, iHeight/2, 255, 0, pY);
	break;
	case 6:
		this->LinearDDA (0, iWidth/2, 255, iWidth/2, pX);
		this->LinearDDA (0, iHeight/2, 255, 0, pY);
	break;
	case 7:
	default:
		this->LinearDDA (0, iWidth/2, 255, iWidth-1, pX);
		this->LinearDDA (0, iHeight/2, 255, 0, pY);
	break;
	}

 	for ( i = 0; i < 256; i++ )
 	 {
		k0				  = pY[i]*l + pX[i]*3;
		pbCrossSection[i] =	(BYTE)lpDib[k0];
 	 }

	GlobalUnlock(hDib);
	return 1;

  //## end CDIB::CrossSection%3B4A74630107.body
}

int CDIB::RadiallyAvragedPowerSpectrum (HANDLE hDib, WORD nDirection, DWORD pdwPowerSpectrum[256], const double dblGrayLevelOfImage)
{
  //## begin CDIB::RadiallyAvragedPowerSpectrum%3B4BFA2E0309.body preserve=yes

	switch(nDirection)
	{
	case 0:
		this->RadiallyAvragedPowerSpectrum (hDib, 0, 22, pdwPowerSpectrum
//			, dblGrayLevelOfImage
			);
	break;
	case 1:
		this->RadiallyAvragedPowerSpectrum (hDib,45-22, 45+23, pdwPowerSpectrum
//			, dblGrayLevelOfImage
			);
	break;
	case 2:
		this->RadiallyAvragedPowerSpectrum (hDib, 90-22, 90+23,  pdwPowerSpectrum
//			, dblGrayLevelOfImage
			);
	break;
	case 3:
		this->RadiallyAvragedPowerSpectrum (hDib, 135-22, 135+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
	break;
	case 4:
		this->RadiallyAvragedPowerSpectrum (hDib,  180-22, 180+23, pdwPowerSpectrum
//			, dblGrayLevelOfImage
);
	break;
	case 5:
		this->RadiallyAvragedPowerSpectrum (hDib, 225-22, 225+23, pdwPowerSpectrum
//			, dblGrayLevelOfImage
);
	break;
	case 6:
		this->RadiallyAvragedPowerSpectrum (hDib,  270-22, 270+23, pdwPowerSpectrum
//			, dblGrayLevelOfImage
			);
	break;
	case 7:
		this->RadiallyAvragedPowerSpectrum (hDib, 315-22, 315+23, pdwPowerSpectrum
//			, dblGrayLevelOfImage
			);
	break;
	default:
		this->RadiallyAvragedPowerSpectrum (hDib, 0, 359, pdwPowerSpectrum
//			, dblGrayLevelOfImage
);
	break;
	}
	return 256;
  //## end CDIB::RadiallyAvragedPowerSpectrum%3B4BFA2E0309.body
}

int CDIB::RadiallyAvragedPowerSpectrum (HANDLE hDib, WORD nStartAngle, WORD nEndAngle, DWORD pdwPowerSpectrum[256])
{
  //## begin CDIB::RadiallyAvragedPowerSpectrum%3B4BF95202B3.body preserve=yes

//#define USEPeriodogram
#ifndef	USEPeriodogram
//{
	HPSTR	lpDib;
	int	    iHeight, iWidth;
	long	i,j,l;
    LPBITMAPINFOHEADER  lpbi;
//	double dStartAngle, dEndAngle;
	double dPI = 3.1415926535897932384626433832795;
/*
	if ( nStartAngle >180 )
		nStartAngle = nStartAngle - 360;

	if ( nEndAngle >180 )
		nEndAngle = nEndAngle - 360;

	dStartAngle	= (double)nStartAngle * dPI / 180.0;
	dEndAngle	= (double)nEndAngle * dPI / 180.0;
*/	
	if ( !hDib)
	 return 0;

	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}

	iWidth 	= (int)DIBWidth (lpDib);
	iHeight	= (int)DIBHeight (lpDib);
	
	lpbi = (LPBITMAPINFOHEADER)lpDib; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

	ASSERT(lpbi->biBitCount == 24);

 	lpDib = FindDIBBits (lpDib);
 	
	double dAngle;
	DWORD  pdwCount[256], k0;
	WORD   nAngle;

	for ( i = 0; i < 256; i++ )
	{
		pdwCount[i]			= 0;
		pdwPowerSpectrum[i] = 0;
	}

	double	x, y, r;
	long nR;
	
	for ( i = 0; i < iWidth; i++ )
	 	for ( j = 0; j < iHeight; j++ )
 	 {
		x = ((double)i - (double)iWidth/2.0)/((double)iWidth/2.0);
		y = ((double)j - (double)iHeight/2.0)/((double)iHeight/2.0);

		dAngle = atan2( y, x );
		if ( dAngle < 0.0 )
			dAngle += 2.0*dPI;

		nAngle = (WORD)(dAngle * 180.0 / dPI);
		
		ASSERT(nStartAngle<nEndAngle);

		if ( (nAngle >= nStartAngle) && ((nAngle <= nEndAngle)))
		{
			r = sqrt(x*x+y*y)/sqrt(2.0);
			nR = (DWORD)(r*255.0);
			ASSERT (nR>=0&&nR<256);
			pdwCount[nR]++;
			k0					= j*l + i*3;
			pdwPowerSpectrum[nR] += (DWORD)(BYTE)lpDib[k0];
		}
 	 }

	for ( i = 0; i < 256; i++ )
	{
		if ( pdwCount[i] != 0 )
			pdwPowerSpectrum[i]	= pdwPowerSpectrum[i] / pdwCount[i];
	}
	GlobalUnlock(hDib);
	return 256;
//}
#else
//{
	HPSTR	lpDib;
	int	    iHeight, iWidth;
	long	i,j,l;
    LPBITMAPINFOHEADER  lpbi;
//	double dStartAngle, dEndAngle;
	double dPI = 3.1415926535897932384626433832795;

	if ( !hDib)
	 return 0;

	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}

	iWidth 	= (int)DIBWidth (lpDib);
	iHeight	= (int)DIBHeight (lpDib);
	
	lpbi = (LPBITMAPINFOHEADER)lpDib; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

	ASSERT(lpbi->biBitCount == 24);

 	lpDib = FindDIBBits (lpDib);
 	
	double dAngle;
	DWORD  pdwCount[256], k0;
	WORD   nAngle;

	double pdblPowerSpectrum[256];

	for ( i = 0; i < 256; i++ )
	{
		pdwCount[i]			= 0;
		pdblPowerSpectrum[i] = 0.0;
	}

	double	x, y, r, d, s;
	long nR;
	
	s = (double)iWidth*(double)iHeight;

	for ( i = 0; i < iWidth; i++ )
	 	for ( j = 0; j < iHeight; j++ )
 	 {
		x = ((double)i - (double)iWidth/2.0)/((double)iWidth/2.0);
		y = ((double)j - (double)iHeight/2.0)/((double)iHeight/2.0);

		dAngle = atan2( y, x );
		if ( dAngle < 0.0 )
			dAngle += 2.0*dPI;

		nAngle = (WORD)(dAngle * 180.0 / dPI);
		
		ASSERT(nStartAngle<nEndAngle);

		if ( (nAngle >= nStartAngle) && ((nAngle <= nEndAngle)))
		{
			r = sqrt(x*x+y*y)/sqrt(2.0);
			nR = (DWORD)(r*255.0);
			ASSERT (nR>=0&&nR<256);
			pdwCount[nR]++;
			k0					= j*l + i*3;
			d	= (double)(DWORD)(BYTE)lpDib[k0];
			pdblPowerSpectrum[nR] += d*d/s;
		}
 	 }

	double sigma = dblGrayLevelOfImage*(255.0 - dblGrayLevelOfImage), sigma1= 0.0;

	for ( i = 0; i < 256; i++ )
	{
		sigma1	+= pdblPowerSpectrum[i];
	}

	r = sigma1 / sigma;

	
	for ( i = 0; i < 256; i++ )
	{
		if ( pdwCount[i] != 0 )
			pdblPowerSpectrum[i]	= pdblPowerSpectrum[i] / (double)(pdwCount[i]);
	}


	for ( i = 0; i < 256; i++ )
	{
			pdwPowerSpectrum[i]	= (DWORD)(pdblPowerSpectrum[i]*255.0/sigma + 0.5);
	}

	GlobalUnlock(hDib);
	return 256;
//}
#endif
  //## end CDIB::RadiallyAvragedPowerSpectrum%3B4BF95202B3.body
}

void CDIB::LinearDDA (int xs, int ys, int xe, int ye, int* pResult)
{
  //## begin CDIB::LinearDDA%3B4A9AA6010B.body preserve=yes
#define output(x, y) pResult[(x)-xs] = (y)
	if( pResult == NULL )
		return;

	unsigned __int64 k, ry;
	unsigned __int64 x, y, inc_x, inc_y, s;
	s     = sign ( ye - ys );
	inc_y = abs ( ye - ys );
	inc_x = xe - xs;
	k     = inc_y;
	ry    = 0;
	y     = ys;
	x     = xs;
	output ( (int)x, (int)y );
	for ( x = xs + 1;x <= xe; x++ )
	{
		ry = ry + k;
		if ( ry >= inc_x )
	    {
			y	= y + s * ( ry / inc_x );
			ry = ry % inc_x;
		}
		output ( (int)x, (int)y );
	}


  //## end CDIB::LinearDDA%3B4A9AA6010B.body
}

double CDIB::GetMMD (DWORD dwLength, DWORD* pData)
{
  //## begin CDIB::GetMMD%3B53C066003E.body preserve=yes
	double dC = 0.0 ; //均值

	for(DWORD i = 0 ; i < dwLength ; i ++)
		dC = dC +  (double)(i * pData[i]) ;
	dC = dC / (double) dwLength ;

	double dM = 0.0 ; //方差

	for(DWORD i = 0 ; i < dwLength ; i ++)
		dM = dM + (double)(pData[i] * (i-dC)*(i-dC)) ;

	dM = sqrt( dM / (double)dwLength ) ;

	return dM ;

  //## end CDIB::GetMMD%3B53C066003E.body
}

void CDIB::RadiallyMMDSpectrum (HANDLE hDib, DWORD nDirection, double pdblPowerSpectrumVariance[256], double pdblPowerSpectrumAnisotropy[256], double* dblMeanVariance, double* dblMeanAnisotropy, double* dblMaxAnisotropy)
{
  //## begin CDIB::RadiallyMMDSpectrum%3B54E5BF02A5.body preserve=yes
//#define UseVariance
//#ifdef UseVariance
	this->RadiallyAvragedPowerSpectrumVariance( hDib, 
												nDirection, 
												pdblPowerSpectrumVariance
//												,dblGrayLevelOfImage
												);
//#else
	this->RadiallyAvragedPowerSpectrumAnisotropy( hDib, 
												nDirection, 
												pdblPowerSpectrumAnisotropy
//												,dblGrayLevelOfImage
												);

//#endif
//	for ( int i = 0; i < 256; i++ )
//		pdblPowerSpectrumVariance[i] = 128;

if (dblMeanVariance!=NULL) 
{
	*dblMeanVariance  = 0.0;

	for ( int i = 0; i <256; i++ )
		*dblMeanVariance	+= pdblPowerSpectrumVariance[i];
	*dblMeanVariance  /= 256.0;
}


if (dblMeanAnisotropy!=NULL) 
{
	*dblMeanAnisotropy  = 0.0;
	double dblAnisotropyMax	= FLT_MIN;
	for ( int i = 0; i <256; i++ )
	{
		*dblMeanAnisotropy	+= pdblPowerSpectrumAnisotropy[i];
		dblAnisotropyMax	=  max(pdblPowerSpectrumAnisotropy[i], dblAnisotropyMax);
	}
	*dblMeanAnisotropy  /= 256.0;
	if ( dblMaxAnisotropy != NULL )
		*dblMaxAnisotropy	= dblAnisotropyMax;
}

//for ( int i = 0; i < 256; i++ )
//	pdblPowerSpectrumAnisotropy[i] = 64;

  //## end CDIB::RadiallyMMDSpectrum%3B54E5BF02A5.body
}

void CDIB::RadiallyAvragedPowerSpectrumVariance (HANDLE hDib, DWORD nDirection, double pdblPowerSpectrumVariance[256])
{
  //## begin CDIB::RadiallyAvragedPowerSpectrumVariance%3B5E82FF028B.body preserve=yes
	DWORD pdwPowerSpectrum[256];
	switch(nDirection)
	{
	case 0:
		this->RadiallyAvragedPowerSpectrum (hDib, 0, 22, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 0, 22, pdwPowerSpectrum
			,pdblPowerSpectrumVariance
			);
	break;
	case 1:
		this->RadiallyAvragedPowerSpectrum (hDib,45-22, 45+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 45-22, 45+23, pdwPowerSpectrum
			,pdblPowerSpectrumVariance
			);
		break;
	case 2:
		this->RadiallyAvragedPowerSpectrum (hDib, 90-22, 90+23,  pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 90-22, 90+23, pdwPowerSpectrum
			,pdblPowerSpectrumVariance
			);
	break;
	case 3:
		this->RadiallyAvragedPowerSpectrum (hDib, 135-22, 135+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 135-22, 135+23, pdwPowerSpectrum
			,pdblPowerSpectrumVariance);
	break;
	case 4:
		this->RadiallyAvragedPowerSpectrum (hDib,  180-22, 180+23, pdwPowerSpectrum
//,			dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 180-22, 180+23, pdwPowerSpectrum
			,pdblPowerSpectrumVariance
			);
	break;
	case 5:
		this->RadiallyAvragedPowerSpectrum (hDib, 225-22, 225+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 225-22, 225+23, pdwPowerSpectrum
			,pdblPowerSpectrumVariance
			);
	break;
	case 6:
		this->RadiallyAvragedPowerSpectrum (hDib,  270-22, 270+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 270-22, 270+23, pdwPowerSpectrum
			,pdblPowerSpectrumVariance
			);
	break;
	case 7:
		this->RadiallyAvragedPowerSpectrum (hDib, 315-22, 315+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 315-22, 315+23, pdwPowerSpectrum
			,pdblPowerSpectrumVariance
			);
	break;
	default:
		this->RadiallyAvragedPowerSpectrum (hDib, 0, 359, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumVariance (hDib, 0, 359, pdwPowerSpectrum
			,pdblPowerSpectrumVariance
			);
	break;
	}

  //## end CDIB::RadiallyAvragedPowerSpectrumVariance%3B5E82FF028B.body
}

int CDIB::RadiallyAvragedPowerSpectrumVariance (HANDLE hDib, WORD nStartAngle, WORD nEndAngle, const DWORD pdwPowerSpectrum[256], double pdblPowerSpectrumVariance[256])
{
  //## begin CDIB::RadiallyAvragedPowerSpectrumVariance%3B5E821A0173.body preserve=yes

	HPSTR	lpDib;
	int	    iHeight, iWidth;
	long	i,j,l;
    LPBITMAPINFOHEADER  lpbi;
//	double dStartAngle, dEndAngle;
	double dPI = 3.1415926535897932384626433832795;

	if ( !hDib)
	 return 0;

	lpDib = (HPSTR)GlobalLock ( hDib );
	if (lpDib==NULL)
	{
	 GlobalUnlock(hDib);
	 return 0;
	}

	iWidth 	= (int)DIBWidth (lpDib);
	iHeight	= (int)DIBHeight (lpDib);
	
	lpbi = (LPBITMAPINFOHEADER)lpDib; 	
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

	ASSERT(lpbi->biBitCount == 24);

 	lpDib = FindDIBBits (lpDib);
 	
	double dAngle;
	DWORD  pdwCount[256], k0;
	WORD   nAngle;

	for ( i = 0; i < 256; i++ )
	{
		pdwCount[i]			= 0;
		pdblPowerSpectrumVariance[i] = 0.0;
	}

	double	x, y, r;
	long nR;
	
	for ( i = 0; i < iWidth; i++ )
	 	for ( j = 0; j < iHeight; j++ )
 	 {
		x = ((double)i - (double)iWidth/2.0)/((double)iWidth/2.0);
		y = ((double)j - (double)iHeight/2.0)/((double)iHeight/2.0);

		dAngle = atan2( y, x );
		if ( dAngle < 0.0 )
			dAngle += 2.0*dPI;

		nAngle = (WORD)(dAngle * 180.0 / dPI);
		
		ASSERT(nStartAngle<nEndAngle);

		if ( (nAngle >= nStartAngle) && ((nAngle <= nEndAngle)))
		{
			r = sqrt(x*x+y*y)/sqrt(2.0);
			nR = (DWORD)(r*255.0);
			ASSERT (nR>=0&&nR<256);
			pdwCount[nR]++;
			k0					= j*l + i*3;
//			pdwPowerSpectrum[nR] += (DWORD)(BYTE)lpDib[k0];
			double d = (double)(DWORD)(BYTE)lpDib[k0];
			d = (d-(double)pdwPowerSpectrum[nR]);

			pdblPowerSpectrumVariance[nR] += d*d;
		}
 	 }

	for ( i = 0; i < 256; i++ )
	{
		if ( pdwCount[i] > 1 )
			pdblPowerSpectrumVariance[i]	= sqrt(pdblPowerSpectrumVariance[i] / (double)(pdwCount[i]-1));
	}
	GlobalUnlock(hDib);
	return 256;

  //## end CDIB::RadiallyAvragedPowerSpectrumVariance%3B5E821A0173.body
}

int CDIB::RadiallyAvragedPowerSpectrumAnisotropy (HANDLE hDib, WORD nStartAngle, WORD nEndAngle, const DWORD pdwPowerSpectrum[256], double pdblPowerSpectrumAnisotropy[256])
{
  //## begin CDIB::RadiallyAvragedPowerSpectrumAnisotropy%3B64F943024E.body preserve=yes
	double pdblPowerSpectrumVariance[256];

	this->RadiallyAvragedPowerSpectrumVariance (	hDib, 
													nStartAngle, 
													nEndAngle,
													pdwPowerSpectrum,
													pdblPowerSpectrumVariance);
	double s, p, a;
	double dblMin = FLT_MAX, dblMax = -FLT_MAX;
	int i;
	for ( i = 0; i < 256; i++ )
	{
		p	= pdwPowerSpectrum[i];
		s	= pdblPowerSpectrumVariance[i];
		if (p==0)
		{//如果信号没有，那么噪音肯定没有
			p+=1.0;
			s = 0.0;
		}
		a	= (s*s)/(p*p);
		dblMin	= min(a,dblMin);
		dblMax	= max(a,dblMax);
		pdblPowerSpectrumAnisotropy[i]	= a;
	}

	for ( i = 0; i < 256; i++ )
	{
		a	= pdblPowerSpectrumAnisotropy[i];
		if ( a ==  0.0 )
			a = -127.0;
		else
			a = log10(a);

		pdblPowerSpectrumAnisotropy[i]	= BOUND(a*10.0 + 127.0, 0.0, 255.);//
	}


	return 256;
  //## end CDIB::RadiallyAvragedPowerSpectrumAnisotropy%3B64F943024E.body
}

void CDIB::RadiallyAvragedPowerSpectrumAnisotropy (HANDLE hDib, DWORD nDirection, double pdblPowerSpectrumAnisotropy[256])
{
  //## begin CDIB::RadiallyAvragedPowerSpectrumAnisotropy%3B64FB610081.body preserve=yes
	DWORD pdwPowerSpectrum[256];
	switch(nDirection)
	{
	case 0:
		this->RadiallyAvragedPowerSpectrum (hDib, 0, 22, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 0, 22, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy
			);
	break;
	case 1:
		this->RadiallyAvragedPowerSpectrum (hDib,45-22, 45+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 45-22, 45+23, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy
			);
		break;
	case 2:
		this->RadiallyAvragedPowerSpectrum (hDib, 90-22, 90+23,  pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 90-22, 90+23, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy
			);
	break;
	case 3:
		this->RadiallyAvragedPowerSpectrum (hDib, 135-22, 135+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 135-22, 135+23, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy);
	break;
	case 4:
		this->RadiallyAvragedPowerSpectrum (hDib,  180-22, 180+23, pdwPowerSpectrum
//,			dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 180-22, 180+23, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy
			);
	break;
	case 5:
		this->RadiallyAvragedPowerSpectrum (hDib, 225-22, 225+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 225-22, 225+23, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy
			);
	break;
	case 6:
		this->RadiallyAvragedPowerSpectrum (hDib,  270-22, 270+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 270-22, 270+23, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy
			);
	break;
	case 7:
		this->RadiallyAvragedPowerSpectrum (hDib, 315-22, 315+23, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 315-22, 315+23, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy
			);
	break;
	default:
		this->RadiallyAvragedPowerSpectrum (hDib, 0, 359, pdwPowerSpectrum
//			,dblGrayLevelOfImage
			);
		this->RadiallyAvragedPowerSpectrumAnisotropy (hDib, 0, 359, pdwPowerSpectrum
			,pdblPowerSpectrumAnisotropy
			);
	break;
	}

  //## end CDIB::RadiallyAvragedPowerSpectrumAnisotropy%3B64FB610081.body
}

HANDLE CDIB::Raw8Bw2Dib (HANDLE hRaw, unsigned int w, unsigned  int h)
{
  //## begin CDIB::Raw8Bw2Dib%40DFCC0500D7.body preserve=yes

	ASSERT(hRaw!=NULL);
	HPALETTE hPal	= this->Create8BitGrayLinearPalette ();

	HANDLE hDib = this->CreateDib (	w ,//int width,
									h ,//int height, 
									0,//DWORD biStyle, 
									8*3,//WORD biBits,
	//								hPal//HPALETTE hpal
									NULL
									);
	DeleteObject( hPal );

	LPBYTE lpDibBits	= (LPBYTE)GlobalLock( hDib );

	lpDibBits	= (LPBYTE)this->FindDIBBits ((LPSTR)lpDibBits);
	
	LPBYTE lpFrm = (LPBYTE)GlobalLock(hRaw);
	
	int k = 0;
	int m = 0;
//	ASSERT(m_dwWidth%4==0);
	int nWidhtbytes =  WIDTHBYTES( w*24);

	for( unsigned int j = 0; j < h; j++ )
	{
		m = ( h - j - 1)*nWidhtbytes;
		
		for( unsigned int i = 0; i < w; i++ )
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
	GlobalUnlock(hRaw);
	return hDib;
  //## end CDIB::Raw8Bw2Dib%40DFCC0500D7.body
}

HANDLE CDIB::RawRgb2Dib (HANDLE hRaw, unsigned  int w, unsigned  int h)
{
  //## begin CDIB::RawRgb2Dib%40DFCC48012D.body preserve=yes
	return NULL;//Not implemented yet.
  //## end CDIB::RawRgb2Dib%40DFCC48012D.body
}

HANDLE CDIB::OrderedDither (HANDLE hDIB)
{
  //## begin CDIB::OrderedDither%419D48820345.body preserve=yes
 	HPSTR	lpDibDst, lpDibSrc;
	HANDLE  hDibDst = 0;
	int	    iHeight, iWidth;
	register long	i, l, j, k0, k1;
    LPBITMAPINFOHEADER  lpbi;
    WORD	r, g, b, Dij;
	WORD    D4[16] = {
			 0,  8,  2, 10,
			12,  4, 14,  6,
			 3, 11,  1,  9,
			15,  7,  13, 5};
	HANDLE hDibSrc = hDIB;
	
	if ( !hDibSrc)
	 return 0;

	lpDibSrc = (HPSTR)GlobalLock ( hDibSrc );
	if (lpDibSrc==NULL)
	{
	 GlobalUnlock(hDibSrc);
	 return 0;
	}

	iWidth 	= (int)DIBWidth (lpDibSrc);
	iHeight	= (int)DIBHeight (lpDibSrc);

	lpbi = (LPBITMAPINFOHEADER)lpDibSrc; 	

	hDibDst = CreateDib ( iWidth, iHeight, 0, lpbi->biBitCount, NULL);
	
	if ( hDibDst == NULL )
	{
	 GlobalUnlock(hDibSrc);
	 return 0;
	}
	
	lpDibDst = (HPSTR)GlobalLock ( hDibDst );
	if (lpDibDst==NULL)
	{
	  GlobalFree(hDibDst);
	  return 0;
	}
    l    = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount);

 	lpDibSrc = FindDIBBits (lpDibSrc);
 	lpDibDst = FindDIBBits (lpDibDst);
 	int i0, j0;
 	for ( i = 0; i < iHeight; i++ )
 	 {
 	  k0 = k1 = 0;
 	  for ( j = 0; j <iWidth; j++ )
 		{
 	 		b = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		g = (WORD)((BYTE)lpDibSrc[k0++]);
 	 		r = (WORD)((BYTE)lpDibSrc[k0++]);
			i0 = i % iHeight;
			j0 = j % iWidth;
			Dij = D4[i0*4+j0];
			r = ( r < Dij ? 0 : 255 );
			g = ( g < Dij ? 0 : 255 );
			b = ( b < Dij ? 0 : 255 );

 	 		lpDibDst[k1++] = (BYTE)b;
 	 		lpDibDst[k1++] = (BYTE)g;
	 		lpDibDst[k1++] = (BYTE)r;
    	}
      lpDibDst += l;
      lpDibSrc += l;
 	 }
 	GlobalUnlock(hDibDst);
	GlobalUnlock(hDibSrc);
	return hDibDst;
	 
  //## end CDIB::OrderedDither%419D48820345.body
}

long CDIB::FloodFill4 (HANDLE hDIB, int nSeedX, int nSeedY, int nROld, int nGOld, int nBOld, int nRNew, int nGNew, int nBNew)
{
  //## begin CDIB::FloodFill4%4212B6F401FB.body preserve=yes
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
 		this->FloodFill4(	hDIB, nSeedX+1, nSeedY, 
							nR, nG, nB, nRNew, nGNew, nBNew );
		this->FloodFill4(	hDIB, nSeedX, nSeedY+1, 
							nR, nG, nB, nRNew, nGNew, nBNew );
		this->FloodFill4(	hDIB, nSeedX-1, nSeedY, 
							nR, nG, nB, nRNew, nGNew, nBNew );
		this->FloodFill4(	hDIB, nSeedX, nSeedY-1, 
							nR, nG, nB, nRNew, nGNew, nBNew );
	}
	else
	{
		int i = i;
	}
	return 0;
  //## end CDIB::FloodFill4%4212B6F401FB.body
}

void CDIB::BoundryFill4 (HANDLE hDIB, int nSeedX, int nSeedY, int nRNew, int nGNew, int nBNew)
{
  //## begin CDIB::BoundryFill4%4212F47C01CE.body preserve=yes
	WORD nR,nG, nB;
	this->GetPixel(hDIB, nSeedX, nSeedY, &nR , &nG , &nB );
	double *pParam;
	long nPCount;
	nPCount = this->FloodFill4(	hDIB, nSeedX, nSeedY, 
								nR , nG , nB , nRNew , nGNew , nBNew );
  //## end CDIB::BoundryFill4%4212F47C01CE.body
}

// Additional Declarations
  //## begin CDIB%364BA69102AB.declarations preserve=yes
  //## end CDIB%364BA69102AB.declarations

//## begin module%40DB9D33020F.epilog preserve=yes
//## end module%40DB9D33020F.epilog
