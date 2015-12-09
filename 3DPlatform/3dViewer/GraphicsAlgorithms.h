#ifndef GRAPHICS_H_
#define GRAPHICS_H_


#include"typedef.h"


/* 扫描二值图像的算法 */
//返回扫描所得到的交点的数目和坐标
//int ScanBnrImg(LINE l,CBmp &bmp,POINT2D **pPts=NULL);

//int ScanBnrImgDDA(LINE l,CBmp &bmp,POINT2D **pPts=NULL);

//扫描整张图像，求出边缘点：
//int   FullScan(CBmp &bmp,POINT2D **pPts=NULL);
void  OrderPts(POINT2D *pPts,int &nPtSum,int nWid,int nHei);


//空间直线和平面的交点
bool IsBackgrd(COLORREF c);

//EDGE  EdgeIntersection(EDGE e1,EDGE e2,POINT3D vp);

void DrawPt_(POINT2D pt, CDC *pDC);

POINT2D LineCross(LINE l1,LINE l2);

POINT3D CrossValue(POINT3D p1,POINT3D p2);


#endif
