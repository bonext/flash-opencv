#include "AS3.h"
#include "opencv/cv.h"
#include <stdio.h>
#include "lrint.cpp"
//screen buffer size
long bufferSize;
char* buffer;
int frameWidth, frameHeight, frameChannels;

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

static AS3_Val setFramePtr(void* self, AS3_Val args)
{
  //parse parameters
  AS3_Val byteArr;
  long szByteArr;
  AS3_ArrayValue(args,"AS3ValType, IntType",&byteArr,&szByteArr);

  //alloc memory for transfers
  uchar *dst = new uchar[szByteArr];
  uchar *rgb_dst = new uchar[3*szByteArr/4];

  //read data
  AS3_ByteArray_readBytes((void*)dst,byteArr,szByteArr);

  //convert to RGB from RGBA
  for (int k=0, j=0; k<szByteArr; ++k)
  {
    if (k%4==0) {
      continue;
    } else {
      rgb_dst[j++]=dst[k];
    }
  }

  cv::Mat frame(640,480,CV_8UC3,(void*)rgb_dst,640);
  cv::Mat gs_frame;

  cv::cvtColor(frame,gs_frame,CV_RGB2GRAY);

  delete[] rgb_dst;
  delete[] dst;
  
  return AS3_Int(0);
}

//entry point
int main()
{
  AS3_Val initByteArrayMethod=AS3_Function(NULL, initByteArray);
  AS3_Val freeByteArrayMethod=AS3_Function(NULL, freeByteArray);
  AS3_Val setFrameParamsMethod=AS3_Function(NULL, setFrameParams);
  AS3_Val setFramePtrMethod = AS3_Function(NULL,setFramePtr);

  AS3_Val result = AS3_Object("initByteArray: AS3ValType,\
			freeByteArray: AS3ValType,\
			setFrameParams: AS3ValType,\
      setFramePtr: AS3ValType",
			initByteArrayMethod,freeByteArrayMethod,setFrameParamsMethod,
      setFramePtrMethod);

  AS3_Release(initByteArrayMethod);
  AS3_Release(freeByteArrayMethod);
  AS3_Release(setFrameParamsMethod);
  AS3_Release(setFramePtrMethod);

  AS3_LibInit(result);

  return 0;
}
