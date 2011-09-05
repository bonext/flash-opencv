#include "AS3.h"
#include "opencv/cv.h"
#include <stdio.h>
#include "lrint.cpp"
//screen buffer size
long bufferSize;
char* buffer;
int frameWidth, frameHeight, frameChannels;

//cv:: Mat frame,gs_frame;

//FLASH: C memory init function
static AS3_Val initByteArray(void* self, AS3_Val args)
{
  //read buffer size as int from input
  AS3_ArrayValue(args,"IntType",&bufferSize);
  //allocate buffer size
	buffer=new char[bufferSize];
  //return ptr to buffer location in memory
  return AS3_Ptr(buffer);
}

//FLASH: C memory free function
static AS3_Val freeByteArray(void* self, AS3_Val args)
{
  delete[] buffer;
	*buffer=0;
  bufferSize=-1;
  return 0;
}

//FLASH: Camera parameters setup
static AS3_Val setFrameParams(void* self, AS3_Val args)
{
  AS3_ArrayValue(args,"IntType, IntType, IntType",&frameWidth,&frameHeight,&frameChannels);
  return 0;
}

void cvMagic()
{
  cv::Mat frame(frameHeight,frameWidth,CV_8UC3,(void*)buffer);
  cv::Mat gs_frame(frameHeight,frameWidth,CV_8UC1);
}

//FLASH: OpenCV image fiddling
static AS3_Val testCV(void* self, AS3_Val args)
{
  cv::Mat gs_frame(320,240,CV_8UC1,cv::Scalar(1.0));
//  while(1)
//  {
//  long size = (1<<15); // 2^16 = 65536
//  uchar *bytes = new uchar[size];//alloc a 320x240x3 bytes in memory
//
//  delete[] bytes;
//  }
  return 0;
}

//entry point
int main()
{
  AS3_Val initByteArrayMethod=AS3_Function(NULL, initByteArray);
  AS3_Val freeByteArrayMethod=AS3_Function(NULL, freeByteArray);
  AS3_Val setFrameParamsMethod=AS3_Function(NULL, setFrameParams);
  AS3_Val testCVMethod = AS3_Function(NULL, testCV);

  AS3_Val result = AS3_Object("initByteArray: AS3ValType,\
			freeByteArray: AS3ValType,\
			setFrameParams: AS3ValType,\
      testCV: AS3ValType",
			initByteArrayMethod,freeByteArrayMethod,setFrameParamsMethod,testCVMethod);

  AS3_Release(initByteArrayMethod);
  AS3_Release(freeByteArrayMethod);
  AS3_Release(setFrameParamsMethod);

  AS3_LibInit(result);

  return 0;
}
