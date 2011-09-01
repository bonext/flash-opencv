#include "AS3.h"
#include "opencv/cv.h"

//screen buffer size
int bufferSize;
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

//FLASH: OpenCV image fiddling
static AS3_Val testCV(void* self, AS3_Val args)
{
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
