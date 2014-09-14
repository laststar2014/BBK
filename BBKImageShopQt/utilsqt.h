#ifndef _UTILSQT_H_
#define _UTILSQT_H_

#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include "surflib.h"
#include "kmeans.h"
#include <ctime>
#include <iostream>
using namespace std;
using namespace cv;
//-------------------------------------------------------
// In order to you use OpenSURF, the following illustrates
// some of the simple tasks you can do.  It takes only 1
// function call to extract described SURF features!
// Define PROCEDURE as:
//  - 1 and supply image path to run on static image
//  - 2 to capture from a webcam
//  - 3 to match find an object in an image (work in progress)
//  - 4 to display moving features (work in progress)
//  - 5 to show matches between static images
#define PROCEDURE 5

#define CV_PROFILE(code)	\
{\
	std::cout<<std::endl;\
	double __time_in_ticks = (double)cv::getTickCount();\
{ code }\
	std::cout<<"_ _ _ _ _ _ _ _"<<std::endl;\
	std::cout << #code<<std::endl;\
	std::cout << "DONE :" << ((double)cv::getTickCount() - __time_in_ticks)/cv::getTickFrequency() << "s" << std::endl;\
}
//-------------------------------------------------------

int mainImage(void)
{
  // Declare Ipoints and other stuff
  IpVec ipts;
  IplImage *img=cvLoadImage("imgs/demo.jpg");

  // Detect and describe interest points in the image
  clock_t start = clock();
  surfDetDes(img, ipts, false, 5, 4, 2, 0.0004f); 
  clock_t end = clock();

  std::cout<< "OpenSURF found: " << ipts.size() << " interest points" << std::endl;
  std::cout<< "OpenSURF took: " << float(end - start) / CLOCKS_PER_SEC  << " seconds" << std::endl;

  // Draw the detected points
  drawIpoints(img, ipts);
  
  // Display the result
  showImage(img);

  return 0;
}

//-------------------------------------------------------

int mainVideo(void)
{
  // Initialise capture device
  CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
  if(!capture) error("No Capture");

  // Initialise video writer
  //cv::VideoWriter vw("c:\\out.avi", CV_FOURCC('D','I','V','X'),10,cvSize(320,240),1);
  //vw << img;

  // Create a window 
  cvNamedWindow("OpenSURF", CV_WINDOW_AUTOSIZE );

  // Declare Ipoints and other stuff
  IpVec ipts;
  IplImage *img=NULL;

  // Main capture loop
  while( 1 ) 
  {
    // Grab frame from the capture source
    img = cvQueryFrame(capture);

    // Extract surf points
    surfDetDes(img, ipts, false, 4, 4, 2, 0.004f);    

    // Draw the detected points
    drawIpoints(img, ipts);

    // Draw the FPS figure
    drawFPS(img);

    // Display the result
    cvShowImage("OpenSURF", img);

    // If ESC key pressed exit loop
    if( (cvWaitKey(10) & 255) == 27 ) break;
  }

  cvReleaseCapture( &capture );
  cvDestroyWindow( "OpenSURF" );
  return 0;
}


//-------------------------------------------------------


int mainMatch(void)
{
  // Initialise capture device
  CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
  if(!capture) error("No Capture");

  // Declare Ipoints and other stuff
  IpPairVec matches;
  IpVec ipts, ref_ipts;
  
  // This is the reference object we wish to find in video frame
  // Replace the line below with IplImage *img = cvLoadImage("imgs/object.jpg"); 
  // where object.jpg is the planar object to be located in the video
  IplImage *img = cvLoadImage("imgs/object.jpg"); 
  if (img == NULL) error("Need to load reference image in order to run matching procedure");
  CvPoint src_corners[4] = {{0,0}, {img->width,0}, {img->width, img->height}, {0, img->height}};
  CvPoint dst_corners[4];

  // Extract reference object Ipoints
  surfDetDes(img, ref_ipts, false, 3, 4, 3, 0.004f);
  drawIpoints(img, ref_ipts);
  showImage(img);

  // Create a window 
  cvNamedWindow("OpenSURF", CV_WINDOW_AUTOSIZE );

  // Main capture loop
  while( true ) 
  {
    // Grab frame from the capture source
    img = cvQueryFrame(capture);
     
    // Detect and describe interest points in the frame
    surfDetDes(img, ipts, false, 3, 4, 3, 0.004f);

    // Fill match vector
    getMatches(ipts,ref_ipts,matches);
    
    // This call finds where the object corners should be in the frame
    if (translateCorners(matches, src_corners, dst_corners))
    {
      // Draw box around object
      for(int i = 0; i < 4; i++ )
      {
        CvPoint r1 = dst_corners[i%4];
        CvPoint r2 = dst_corners[(i+1)%4];
        cvLine( img, cvPoint(r1.x, r1.y),
          cvPoint(r2.x, r2.y), cvScalar(255,255,255), 3 );
      }

      for (unsigned int i = 0; i < matches.size(); ++i)
        drawIpoint(img, matches[i].first);
    }

    // Draw the FPS figure
    drawFPS(img);

    // Display the result
    cvShowImage("OpenSURF", img);

    // If ESC key pressed exit loop
    if( (cvWaitKey(10) & 255) == 27 ) break;
  }

  // Release the capture device
  cvReleaseCapture( &capture );
  cvDestroyWindow( "OpenSURF" );
  return 0;
}


//-------------------------------------------------------


int mainMotionPoints(void)
{
  // Initialise capture device
  CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
  if(!capture) error("No Capture");

  // Create a window 
  cvNamedWindow("OpenSURF", CV_WINDOW_AUTOSIZE );

  // Declare Ipoints and other stuff
  IpVec ipts, old_ipts, motion;
  IpPairVec matches;
  IplImage *img;

  // Main capture loop
  while( 1 ) 
  {
    // Grab frame from the capture source
    img = cvQueryFrame(capture);

    // Detect and describe interest points in the image
    old_ipts = ipts;
    surfDetDes(img, ipts, true, 3, 4, 2, 0.0004f);

    // Fill match vector
    getMatches(ipts,old_ipts,matches);
    for (unsigned int i = 0; i < matches.size(); ++i) 
    {
      const float & dx = matches[i].first.dx;
      const float & dy = matches[i].first.dy;
      float speed = sqrt(dx*dx+dy*dy);
      if (speed > 5 && speed < 30) 
        drawIpoint(img, matches[i].first, 3);
    }
        
    // Display the result
    cvShowImage("OpenSURF", img);

    // If ESC key pressed exit loop
    if( (cvWaitKey(10) & 255) == 27 ) break;
  }

  // Release the capture device
  cvReleaseCapture( &capture );
  cvDestroyWindow( "OpenSURF" );
  return 0;
}


//-------------------------------------------------------
/***********************************************************
名    称: transformPoint
功    能: 像素点坐标透视矩阵变换
输入参数: pointToTransform ： 源点坐标
	      matrix ： 透视矩阵
返回  值：变换后点坐标
修改历史:  -
版本    日期       作者    改动内容和原因
V1.0    2013 09.19   Last_star     程序开始编写
***********************************************************/
CvPoint transformPoint(const CvPoint pointToTransform, const CvMat* matrix) 
{
	double coordinates[3] = {pointToTransform.x, pointToTransform.y, 1};
	CvMat originVector = cvMat(3, 1, CV_64F, coordinates);
	CvMat transformedVector = cvMat(3, 1, CV_64F, coordinates);
	cvMatMul(matrix, &originVector, &transformedVector);
	CvPoint outputPoint = cvPoint((int)(cvmGet(&transformedVector, 0, 0) / cvmGet(&transformedVector, 2, 0)), (int)(cvmGet(&transformedVector, 1, 0) / cvmGet(&transformedVector, 2, 0)));
	return outputPoint;
}

/***********************************************************
名    称: DivTransform
功    能: 差异化显示
输入参数: 
返回  值：
修改历史:  -
版本    日期       作者    改动内容和原因
V1.0    2013 09.19   Last_star     程序开始编写
V2.0	2014 05.23   Last_star     彩色图差值
***********************************************************/
void DivTransform(IplImage* src1,IplImage* src2,IplImage* dst)
{
	cvAbsDiff(src1,src2,dst);
	//cvSub(src1,src2,dst);
}


int mainStaticMatch(IplImage* img1,IplImage* img2)
{
  if (img1 == NULL || img2 == NULL)
  {
	  cout<<"image read error!"<<endl;
	  return 0;
  }
  // 图像尺寸变换
  IplImage* pImgR1 = cvCreateImage(cvSize(img1->width*2/3,img1->height*2/3),8,3);
  cvResize(img1,pImgR1);
  // 图像尺寸变换
  IplImage* pImgR2 = cvCreateImage(cvSize(img2->width*2/3,img2->height*2/3),8,3);
  cvResize(img2,pImgR2);
  IpVec ipts1, ipts2;
  surfDetDes(pImgR1,ipts1,false,4,4,2,0.0001f);
  surfDetDes(pImgR2,ipts2,false,4,4,2,0.0001f);

  IpPairVec matches;
  getMatches(ipts1,ipts2,matches);
  for (unsigned int i = 0; i < matches.size(); i=i + 5)
  {
	 
	  drawPoint(pImgR1,matches[i].first);
	  drawPoint(pImgR2,matches[i].second);
  }
  // 求解透视变换矩阵
  // 求透视变换4个点对
  CvPoint2D32f srcPoint[4];
  CvPoint2D32f dstPoint[4];
  srand( (unsigned)time( NULL ) );
  int point[4] = {0};
  int randan = -1;

  // 随机种子数生成
  randan = (int)((rand()/32767.0)*matches.size());
  float rate = 1.0;
  CvMat *h = cvCreateMat( 3, 3, CV_64FC1 );
  CvMat *best = cvCreateMat(3, 3, CV_64FC1);

  // 选点约束条件 dx > 50 dy > 50
  //int Xedge = 50,Yedge = 50,Kedge = 100;

  // 迭代计数
  int times = 0;
  float rateX = 0.3;
  float ref = 1.1;
  while(rate > rateX)
  { 
	  if (times > 30)
	  {
		  break;
	  }
	  times ++;
	 
		// 随机在四个分区里面各取一个点做透视矩阵求解,全size随机收敛的概率可能大点

		// 应加“-1”,发现的一个向量越界bug add by last_star 09_26_2013
		point[0] = (int)((rand()/32767.0)*(matches.size() - 1));
		point[1] = (int)((rand()/32767.0)*(matches.size() - 1));
		point[2] = (int)((rand()/32767.0)*(matches.size() - 1));
		point[3] = (int)((rand()/32767.0)*(matches.size() - 1));

		// 原图坐标
		srcPoint[0].x = matches[point[0]].first.x; srcPoint[0].y = matches[point[0]].first.y;
		srcPoint[1].x = matches[point[1]].first.x; srcPoint[1].y = matches[point[1]].first.y;
		srcPoint[2].x = matches[point[2]].first.x; srcPoint[2].y = matches[point[2]].first.y;
		srcPoint[3].x = matches[point[3]].first.x; srcPoint[3].y = matches[point[3]].first.y;

		// 目标图坐标
		dstPoint[0].x = matches[point[0]].second.x; dstPoint[0].y = matches[point[0]].second.y;
		dstPoint[1].x = matches[point[1]].second.x; dstPoint[1].y = matches[point[1]].second.y;
		dstPoint[2].x = matches[point[2]].second.x; dstPoint[2].y = matches[point[2]].second.y;
		dstPoint[3].x = matches[point[3]].second.x; dstPoint[3].y = matches[point[3]].second.y;


	  // 求解透视矩阵
	  cvGetPerspectiveTransform(srcPoint,dstPoint,h);
	  //cvGetPerspectiveTransform(dstPoint,srcPoint,h2);;

	  // 对其余匹配点进行变换矩阵有效性验证
	  int count = 0;
	  for (unsigned int i = 0; i < matches.size(); i++)
	  {
		  CvPoint pp;
		  pp.x = matches[i].first.x;
		  pp.y = matches[i].first.y;
		  CvPoint rp;

		  rp = transformPoint(pp,h);

		  if (abs(rp.x - matches[i].second.x) < 3 && abs(rp.y - matches[i].second.y) < 3)
		  {
			  count ++;
		  }
	  }

	  // rate : 不匹配率
	  rate = (float)(matches.size()-count)/matches.size();
	  if (rate < ref)
	  {
		  ref = rate;
		  best = cvCloneMat(h);
	  }
	  //std::cout << "rate:" <<rate <<"   "<<std::endl;
  }


  // 仿射变换进行两图配准
  IplImage* pWarp = cvCloneImage(pImgR2);
  cvWarpPerspective(pImgR1,pWarp,best,CV_INTER_LINEAR+(CV_WARP_FILL_OUTLIERS),cvScalarAll(0));
  /*cvShowImage("before warp r1",pImgR1);
  cvShowImage("before warp r2",pImgR2);
  cvShowImage("after warp",pWarp);*/
  cvSaveImage("imgs/before.jpg",pImgR2);
  cvSaveImage("imgs/after.jpg",pWarp);


  // 差异化显示
  IplImage* dst = cvCloneImage(pImgR2);
  DivTransform(pWarp,pImgR2,dst);
  cvSaveImage("imgs/result.jpg",dst);
  //cvWaitKey(0);

  std::cout<< "Matches: " << matches.size();

  /*cvNamedWindow("1", CV_WINDOW_AUTOSIZE );
  cvNamedWindow("2", CV_WINDOW_AUTOSIZE );
  cvNamedWindow("3", CV_WINDOW_AUTOSIZE );
  cvShowImage("1", pImgR2);
  cvShowImage("2",pWarp);
  cvShowImage("3",dst);*/
  //cvWaitKey(0);
  return 0;
}


int thread_stil_match()
{

	return 0;
}
//-------------------------------------------------------

int mainKmeans(void)
{
  IplImage *img = cvLoadImage("imgs/img1.jpg");
  IpVec ipts;
  Kmeans km;
  
  // Get Ipoints
  surfDetDes(img,ipts,true,3,4,2,0.0006f);

  for (int repeat = 0; repeat < 10; ++repeat)
  {

    IplImage *img = cvLoadImage("imgs/img1.jpg");
    km.Run(&ipts, 5, true);
    drawPoints(img, km.clusters);

    for (unsigned int i = 0; i < ipts.size(); ++i)
    {
      cvLine(img, cvPoint(ipts[i].x,ipts[i].y), cvPoint(km.clusters[ipts[i].clusterIndex].x ,km.clusters[ipts[i].clusterIndex].y),cvScalar(255,255,255));
    }

    showImage(img);
  }

  return 0;
}

//-------------------------------------------------------

//int main_surf(void) 
//{
//  if (PROCEDURE == 1) return mainImage();
//  if (PROCEDURE == 2) return mainVideo();
//  if (PROCEDURE == 3) return mainMatch();
//  if (PROCEDURE == 4) return mainMotionPoints();
//  if (PROCEDURE == 5) return thread_stil_match();
//  if (PROCEDURE == 6) return mainKmeans();
//}

void customMessageHandler(QtMsgType type, const char *msg)
{
	QString txt;
	switch (type) {
		//调试信息提示
	case QtDebugMsg:
		txt = QString("Debug: %1").arg(msg);
		break;

		//一般的warning提示
	case QtWarningMsg:
		txt = QString("Warning: %1").arg(msg);
		break;
		//严重错误提示
	case QtCriticalMsg:
		txt = QString("Critical: %1").arg(msg);
		break;
		//致命错误提示
	case QtFatalMsg:
		txt = QString("Fatal: %1").arg(msg);
		abort();
	}

	QFile outFile("debuglog.txt");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
}


#endif