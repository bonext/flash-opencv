#include <opencv/cv.h>
#include <iostream>
#include "AS3.h"

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
