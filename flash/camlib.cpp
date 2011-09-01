#include "AS3.h"
#include "opencv/cv.h"
#include <stdio.h>

extern long int lrintf(float f)
{
  /* Implements the default IEEE 754-1985 rounding mode */
  long * fp = (long *)&f;
  int sign = (*fp) >> 31;
  int exponent = 23 + 0x7f - (((*fp) >> 23) & 0xff);
  unsigned int fraction = ((*fp) & 0x7fffff) | 0x800000;
  long result = fraction >> exponent;
  -- exponent;
  if (fraction & (1 << exponent)) { // fraction >= 0.5
    if (!(fraction & ~(-1 << exponent))) // fraction == 0.5
      if (!(result & 1)) // result is even
        return sign ? -result : result;
    ++ result;
  }
  return sign ? -result : result;
}

extern long int lrint(double d)
{
  return lrintf((float)d);
}

extern long int lrintl(long double ld)
{
  return lrint((double)ld);
}

extern long long int llrint(double x)
{
  return (long long int) lrint(x);
}

extern long long int llrintf(float x)
{
  return (long long int) lrintf(x);
}

extern long long int llrintl(long double x)
{
  return (long long int) lrintl(x);
}

//screen buffer size
int bufferSize;
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
  cvMagic();

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
