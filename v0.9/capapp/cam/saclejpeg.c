#include "scalejpeg.h"


int scalejpeg(char *srcName, char *destName, unsigned int width, unsigned int height)
{
	IplImage *src = 0;			//源图像指针
	IplImage *dst = 0;			//目标图像指针
	IplImage *mid = 0;
    CvSize dst_cvsize;			//目标图像尺寸
 
    src = cvLoadImage(srcName, -1);    //载入工作目录下文件名为“tiger.jpg”的图片。
    
    dst_cvsize.width = width;		//目标图像的宽
    dst_cvsize.height = height;		//目标图像的高

 	printf("before !\n");
    //dst = cvCreateImage(dst_cvsize, src->depth, src->nChannels);	//构造目标图象

    int img_width = src->width;
    int img_height = src->height;

    cvSetImageROI(src , cvRect(0 + 10, 0, img_width - 20, img_height - 10));
    mid = cvCreateImage(cvSize(img_width - 20, img_height - 10), src->depth, src->nChannels);
    cvCopy(src, mid, 0);

    dst = cvCreateImage(dst_cvsize, src->depth, src->nChannels);	//构造目标图象
    cvResize(src, dst, CV_INTER_LINEAR);							//缩放源图像到目标图像
    cvSaveImage(destName, dst, NULL);
	printf("end !\n");

    cvReleaseImage(&src);	//释放源图像占用的内存
    cvReleaseImage(&dst);	//释放目标图像占用的内存	
    cvReleaseImage(&mid);	//释放目标图像占用的内存	
}

