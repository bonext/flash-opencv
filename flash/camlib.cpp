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

  //read data
  AS3_ByteArray_readBytes((void*)dst,byteArr,szByteArr);

  //convert to RGB from RGBA
  cv::Mat frame(640,480,CV_8UC4,(void*)dst);
  
  cv::Mat gs_frame;

  cv::cvtColor(frame,gs_frame,CV_RGBA2GRAY);

  int from_to[]={0,1, 0,2, 0,3};
  mixChannels(&gs_frame,1,&frame,1,from_to,3);

  AS3_ByteArray_seek(byteArr,0,SEEK_SET);
  AS3_ByteArray_writeBytes(byteArr,frame.data,szByteArr);
  
  delete[] dst;
  
  return byteArr;
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
