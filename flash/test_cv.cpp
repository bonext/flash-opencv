#include <opencv/cv.h>
#include <iostream>
#include "AS3.h"
#include "lrint.cpp"

int testCV()
{
  cv::Mat a(3,3,CV_8UC1,cv::Scalar(1.0));
  cv::Mat b(3,3,CV_8UC1,cv::Scalar(2.0));
  cv::Mat c;
  c=a+b;
  return (int)c.at<unsigned char>(0,0);
}

static AS3_Val test(void* self, AS3_Val args)
{
  int res = testCV();
  return AS3_Int(res);
}

int main()
{
  AS3_Val testMethod = AS3_Function(NULL,test);
  AS3_Val result = AS3_Object("test: AS3ValType",testMethod);
  AS3_Release(testMethod);

  AS3_LibInit(result);

  return 0;
}
