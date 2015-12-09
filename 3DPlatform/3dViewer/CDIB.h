//## begin module%40DB9D310067.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%40DB9D310067.cm

//## begin module%40DB9D310067.cp preserve=no
//## end module%40DB9D310067.cp

//## Module: CDIB%40DB9D310067; Package specification
//## Subsystem: <Top Level>
//## Source file: C:\zbf\Research\3dViewer\CDIB.h

#ifndef CDIB_h
#define CDIB_h 1

//## begin module%40DB9D310067.additionalIncludes preserve=no
//## end module%40DB9D310067.additionalIncludes

//## begin module%40DB9D310067.includes preserve=yes
#include "stdafx.h"
#include "windef.h"
//## end module%40DB9D310067.includes

//## begin module%40DB9D310067.declarations preserve=no
//## end module%40DB9D310067.declarations

//## begin module%40DB9D310067.additionalDeclarations preserve=yes





/* Macro to restrict a given value to an upper or lower boundary value */
#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

// Macro to swap two values
#define SWAP(x,y)   ((x)^=(y)^=(x)^=(y))


#define MIN(x,y) (((x) <= (y)) : x ? y)

// Macros to display/remove hourglass cursor for lengthy operations
#define StartWait() hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT))
#define EndWait()   SetCursor((HICON)hcurSave)

// Macro to determine to round off the given value to the closest byte
#define WIDTHBYTES(i)   ((i+31)/32*4)


#define MINBAND     50     // Minimum band size used by the program
#define BANDINCREMENT   20      // Decrement for band size while trying
                   				// to determine optimum band size.

// Bit values for the DIB attributes flag (fFileOptions). Also used as control
// IDs for the radiobuttons for DIB bitcount in the File/Open dialog
//

#define F_1BPP      DLGOPEN_1BPP
#define F_4BPP      DLGOPEN_4BPP
#define F_8BPP      DLGOPEN_8BPP
#define F_24BPP     DLGOPEN_24BPP

// Bit values for the DIB attributes flag (fFileOptions), also used as control
// IDs for the radiobuttons for DIB compression type in the File/Open dialog

#define F_RLE4      DLGOPEN_RLE4
#define F_RLE8      DLGOPEN_RLE8
#define F_RGB       DLGOPEN_RGB

//  flags for _lseek
#define  SEEK_CUR 1
#define  SEEK_END 2
#define  SEEK_SET 0

// struct to be passed in for the SETDIBSCALING printer escape
typedef struct {
    short ScaleMode;
    short dx, dy;
} DIBPARAMS;

#define MAXREAD  32768              // Number of bytes to be read during
                       				// each read operation.

// Header signatutes for various resources
#define BFT_ICON   0x4349   // 'IC'
#define BFT_BITMAP 0x4d42   // 'BM'
#define BFT_CURSOR 0x5450   // 'PT'

// macro to determine if resource is a DIB
#define ISDIB(bft) ((bft) == BFT_BITMAP)

// Macro to align given value to the closest DWORD (unsigned long )
#define ALIGNULONG(i)   ((i+3)/4*4)

#define PALVERSION      0x300
#define MAXPALETTE  256   // max. NUMBER supported palette entries









//	Class :CDIB
//
//	DESCRIPTION : Routines for dealing with Device
//	Independent Bitmaps.
//
//	AUTHER          : Zhou BingFeng
//	DATE of CEATION  :1997.5






























//	each read operation.
//	Header signatutes for various resources
//	macro to determine if resource is a DIB
//	Macro to align given value to the closest DWORD
//	(unsigned long )
//	Class :CDIB
//
//	DESCRIPTION : Routines for dealing with Device
//	Independent Bitmaps.
//
//	AUTHER          : Zhou BingFeng
//	DATE of CEATION  :1997.5









//## end module%40DB9D310067.additionalDeclarations


//## begin CDIB%364BA69102AB.preface preserve=yes
//## end CDIB%364BA69102AB.preface

//## Class: CDIB%364BA69102AB
//	each read operation.
//	Header signatutes for various resources
//	macro to determine if resource is a DIB
//	Macro to align given value to the closest DWORD
//	(unsigned long )
//	Class :CDIB
//
//	DESCRIPTION : Routines for dealing with Device
//	Independent Bitmaps.
//
//	AUTHER          : Zhou BingFeng
//	DATE of CEATION  :1997.5
//	each read operation.
//	Header signatutes for various resources
//	macro to determine if resource is a DIB
//	Macro to align given value to the closest DWORD
//	(unsigned long )
//	Class :CDIB
//
//	DESCRIPTION : Routines for dealing with Device
//	Independent Bitmaps.
//
//	AUTHER          : Zhou BingFeng
//	DATE of CEATION  :1997.5
//## Category: 3dViewer%40C7B40F0349
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3692465A00C0; { -> }
//## Uses: <unnamed>%3692465A00C2; { -> }
//## Uses: <unnamed>%3692465A00CA; { -> }
//## Uses: <unnamed>%3692465A00D1; { -> }
//## Uses: <unnamed>%3692465A010E; { -> }
//## Uses: <unnamed>%3692465A0112; { -> }
//## Uses: <unnamed>%3692465A0122; { -> }
//## Uses: <unnamed>%3692465A0159; { -> }

class CDIB 
{
  //## begin CDIB%364BA69102AB.initialDeclarations preserve=yes
  HANDLE hcurSave;
  //## end CDIB%364BA69102AB.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: OpenDIB%3692465A00C7; C++
      //	huge pointer to a string
      //	The following coefficients are the ones that PhotoStyler
      //	usees.
      HANDLE OpenDIB (LPSTR szFile);

      //## Operation: WriteDIB%3692465A00CB; C++
      BOOL WriteDIB (LPSTR szFile, HANDLE hdib);

      //## Operation: PaletteSize%3692465A00D2; C++
      WORD PaletteSize (void* pv);

      //## Operation: DibNumColors%3692465A00D4; C++
      WORD DibNumColors (void* pv);

      //## Operation: CreateDibPalette%3692465A00D6; C++
      HPALETTE CreateDibPalette (HANDLE hdib);

      //## Operation: CreateBIPalette%3692465A010F; C++
      HPALETTE CreateBIPalette (LPBITMAPINFOHEADER lpbi);

      //## Operation: DibFromBitmap%3692465A0117; C++
      //	FUNCTION   :
      //	BOOL StretchDibBlt (HDC hdc,
      //	int x, y,
      //	int dx, dy,
      //	HANDLE hdib,
      //	int x0, y0,
      //	int dx0, dy0,
      //	LONG rop)
      HANDLE DibFromBitmap (HBITMAP hbm, DWORD biStyle, WORD biBits, HPALETTE hpal);

      //## Operation: BitmapFromDib%3692465A011C; C++
      HANDLE BitmapFromDib (HANDLE hdib, HPALETTE hpal);

      //## Operation: DibBlt%3692465A0123; C++
      BOOL DibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, LONG rop);

      //## Operation: StretchDibBlt%3692465A012D; C++
      BOOL StretchDibBlt (HDC hdc, int x, int y, int dx, int dy, HANDLE hdib, int x0, int y0, int dx0, int dy0, LONG rop);

      //## Operation: DibInfo%3692465A0139; C++
      BOOL DibInfo (HANDLE hdib, LPBITMAPINFOHEADER lpbi);

      //## Operation: ReadDibBitmapInfo%3692465A013C; C++
      HANDLE ReadDibBitmapInfo (int fh);

      //## Operation: DrawBitmap%3692465A013E; C++
      BOOL DrawBitmap (HDC hdc, int x, int y, HBITMAP hbm, DWORD rop);

      //## Operation: FindDIBBits%3692465A0144; C++
      LPSTR FindDIBBits (LPSTR lpbi);

      //## Operation: DIBHeight%3692465A0146; C++
      //	Additional Public Declarations
      DWORD DIBHeight (LPSTR lpDIB);

      //## Operation: DIBHeight%3692465A0148; C++
      int DIBHeight (HANDLE hDib = NULL);

      //## Operation: DIBWidth%3692465A014A; C++
      //	FUNCTION:
      //	DibRotate90 ( hDibSrc : HANDLE ) : HANDLE
      //
      //	DESCRIPTION:
      //	Rotate the DIB by 90 degress Clock-wise.
      //
      //	PARAMETER:
      //	hDibSrc:Handle to the source DIB.
      //
      //	RETURN:Handle to the  rotated DIB.
      DWORD DIBWidth (LPSTR lpDIB);

      //## Operation: DIBWidth%3692465A014C; C++
      //	*********************************************************
      //	Declarations of functions used in drawdib. c module
      //	*********************************************************
      //	*********************************************************
      //	Declarations of functions used in the print.c module
      //	*********************************************************
      //	Additional Public Declarations
      int DIBWidth (HANDLE hDib = NULL);

      //## Operation: CreateDib%3692465A014E; C++
      //	用0-255的灰度生成8位调色板
      HANDLE CreateDib (int width, int height, DWORD biStyle, WORD biBits, HPALETTE hpal);

      //## Operation: Create8BitGrayLinearPalette%3692465A0154; C++
      HPALETTE Create8BitGrayLinearPalette ();

      //## Operation: SetDIBPalette%3692465A0155; C++
      BOOL SetDIBPalette (HANDLE hDib = NULL, HPALETTE hPal = NULL);

      //## Operation: DIBCurveMapping%3692465A015A; C++
      BOOL DIBCurveMapping (HANDLE hDibDst, HANDLE hDibSrc, BYTE Curve[256]);

      //## Operation: DIBCurveMapping%38AA171501AD
      BOOL DIBCurveMapping (HANDLE hDibDst, HANDLE hDibSrc, BYTE CurveR[256], BYTE CurveG[256], BYTE CurveB[256]);

      //## Operation: ColorDibToBWDib%3692465A015E; C++
      BOOL ColorDibToBWDib (HANDLE hDibDst, HANDLE hDibSrc);

      //## Operation: DibRotate90%3692465A0161; C++
      HANDLE DibRotate90 (HANDLE hDibSrc);

      //## Operation: CopyDib%3692465A0163; C++
      HANDLE CopyDib (HANDLE hDibSrc);

      //## Operation: CropDIB%3692465A0165; C++
      //	Additional Public Declarations
      HANDLE CropDIB (HANDLE hDibSrc, int x0 = -1, int y0 = -1, int x1 = -1, int y1 = -1);

      //## Operation: ThresholdingDIB%369247EA0086
      BOOL ThresholdingDIB (HANDLE hDib, int nThreshold = 128, BYTE nUpperColor = 0xff, BYTE nLowerColor = 0, float* pfDensity = NULL);

      //## Operation: MaxAndMin%369FC6FD00D7
      BOOL MaxAndMin (HANDLE hDib, int& nMax, int& nMin);

      //## Operation: Histogram%375E96EE0205
      BOOL Histogram (HANDLE hDib, DWORD pHistoR[256], DWORD pHistoG[256], DWORD pHistoB[256]);

      //## Operation: HistogramEqualization%38AA0DF0014D
      //	Convert the histograms in the pHistoR, pHistoG, pHistoB
      //	into corresponding mapping curves used for histogram
      //	equalization. The mapping curves are stored in pHEMapR, p
      //	HEMapG, pHEMapB.
      BOOL HistogramEqualization (DWORD pHisto[256], BYTE pHEqMap[256], WORD nW, WORD nH);

      //## Operation: Dib2BWRaw16%387BE8AC01EC
      HANDLE Dib2BWRaw16 (HANDLE hDib0, int* nW = NULL, int* nH = NULL);

      //## Operation: Dib2BWRaw8%40EDBCA503C8
      HANDLE Dib2BWRaw8 (HANDLE hDib0, int* nW = NULL, int* nH = NULL);

      //## Operation: PutPixel%3907EF13000D
      BOOL PutPixel (HANDLE hDib, int x, int y, WORD nR, WORD nG, WORD nB);

      //## Operation: GetPixel%4212BB360375
      BOOL GetPixel (HANDLE hDib, int x, int y, WORD* nR, WORD* nG, WORD* nB);

      //## Operation: LowFilterDib%39220EBF03BD
      //	对dib进行低通滤波。滤波矩阵为5x5
      BOOL LowFilterDib (HANDLE hDstDib, HANDLE hSrcDib, double* hMap	// 滤波矩阵，5x5,其和必须是1.0
      );

      //## Operation: CrossSection%3B4A74630107
      //	Function: CrossSection
      //	CrossSection(hDib : HANDLE, nDirection : WORD, *hCross
      //	Section : HANDLE) : int
      //	In:hDib
      //	nDirection:截面方向：
      //	0:向右水平
      //	1：右上
      //	2：向上
      //	3：左上
      //	4：向左
      //	5：右下
      //	6：向下
      //	7：右下
      //	Out: hCrossSection: 截面曲线
      //	return: 截面曲线长度
      int CrossSection (HANDLE hDib, WORD nDirection, DWORD pbCrossSection[256]);

      //## Operation: RadiallyAvragedPowerSpectrum%3B4BFA2E0309
      int RadiallyAvragedPowerSpectrum (HANDLE hDib, WORD nDirection, DWORD pdwPowerSpectrum[256], const double dblGrayLevelOfImage);

      //## Operation: RadiallyAvragedPowerSpectrum%3B4BF95202B3
      int RadiallyAvragedPowerSpectrum (HANDLE hDib, WORD nStartAngle, WORD nEndAngle, DWORD pdwPowerSpectrum[256]);

      //## Operation: GetMMD%3B53C066003E
      double GetMMD (DWORD dwLength, DWORD* pData);

      //## Operation: RadiallyMMDSpectrum%3B54E5BF02A5
      void RadiallyMMDSpectrum (HANDLE hDib, DWORD nDirection, double pdblPowerSpectrumVariance[256], double pdblPowerSpectrumAnisotropy[256], double* dblMeanVariance = NULL, double* dblMeanAnisotropy = NULL, double* dblMaxAnisotropy = NULL);

      //## Operation: RadiallyAvragedPowerSpectrumAnisotropy%3B64F943024E
      int RadiallyAvragedPowerSpectrumAnisotropy (HANDLE hDib, WORD nStartAngle, WORD nEndAngle, const DWORD pdwPowerSpectrum[256], double pdblPowerSpectrumAnisotropy[256]);

      //## Operation: RadiallyAvragedPowerSpectrumAnisotropy%3B64FB610081
      void RadiallyAvragedPowerSpectrumAnisotropy (HANDLE hDib, DWORD nDirection, double pdblPowerSpectrumAnisotropy[256]);

      //## Operation: Raw8Bw2Dib%40DFCC0500D7
      HANDLE Raw8Bw2Dib (HANDLE hRaw, unsigned int w, unsigned int h);

      //## Operation: RawRgb2Dib%40DFCC48012D
      HANDLE RawRgb2Dib (HANDLE hRaw, unsigned int w, unsigned int h);

      //## Operation: OrderedDither%419D48820345
      HANDLE OrderedDither (HANDLE hDIB);

      //## Operation: FloodFill4%4212B6F401FB
      long FloodFill4 (HANDLE hDIB, int nSeedX, int nSeedY, int nROld, int nGOld, int nBOld, int nRNew, int nGNew, int nBNew);

      //## Operation: BoundryFill4%4212F47C01CE
      void BoundryFill4 (HANDLE hDIB, int nSeedX, int nSeedY, int nRNew, int nGNew, int nBNew);

    // Additional Public Declarations
      //## begin CDIB%364BA69102AB.public preserve=yes
      //## end CDIB%364BA69102AB.public

  protected:
    // Additional Protected Declarations
      //## begin CDIB%364BA69102AB.protected preserve=yes
      //## end CDIB%364BA69102AB.protected

  private:

    //## Other Operations (specified)
      //## Operation: lread%3692465A016B; C++
      //	Additional Implementation Declarations
      DWORD lread (int fh, void* pv, DWORD ul);

      //## Operation: lwrite%3692465A016F; C++
      //	Additional Private Declarations
      DWORD lwrite (int fh, void* pv, DWORD ul);

      //## Operation: FilterProcess%39220FA2019D
      void FilterProcess (BYTE* lpBuffer, double* lpMap, int iWidth, int iHeight);

      //## Operation: LinearDDA%3B4A9AA6010B
      void LinearDDA (int xs, int ys, int xe, int ye, int* pResult);

      //## Operation: RadiallyAvragedPowerSpectrumVariance%3B5E82FF028B
      void RadiallyAvragedPowerSpectrumVariance (HANDLE hDib, DWORD nDirection, double pdblPowerSpectrumVariance[256]);

      //## Operation: RadiallyAvragedPowerSpectrumVariance%3B5E821A0173
      int RadiallyAvragedPowerSpectrumVariance (HANDLE hDib, WORD nStartAngle, WORD nEndAngle, const DWORD pdwPowerSpectrum[256], double pdblPowerSpectrumVariance[256]);

    // Additional Private Declarations
      //## begin CDIB%364BA69102AB.private preserve=yes
      //## end CDIB%364BA69102AB.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin CDIB%364BA69102AB.implementation preserve=yes
      //## end CDIB%364BA69102AB.implementation

};

//## begin CDIB%364BA69102AB.postscript preserve=yes
//## end CDIB%364BA69102AB.postscript

// Class CDIB 

//## begin module%40DB9D310067.epilog preserve=yes
//## end module%40DB9D310067.epilog


#endif
