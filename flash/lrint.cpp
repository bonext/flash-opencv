//lrint hack implementation for

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
