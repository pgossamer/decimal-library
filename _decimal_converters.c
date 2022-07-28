#include "_decimal.h"

int _from_int_to_decimal(int src, _decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
  dst->value_type = _NORMAL_VALUE;
  char success = FALSE;
  if (dst) {
    dst->bits[3] = ((src >> 31) << 31);
    dst->bits[0] = abs(src);
    success = TRUE;
  }
  return success;
}

int _from_float_to_decimal(float src, _decimal *dst) {
  dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
  dst->value_type = _NORMAL_VALUE;
  int success = FALSE, power = 0, sign = get_float_sign(&src),
      exp = get_float_exp(&src);

  if (isinf(src) && !sign)
    dst->value_type = _INFINITY;
  else if (isinf(src) && sign)
    dst->value_type = _NEGATIVE_INFINITY;
  else if (isnan(src))
    dst->value_type = _NAN;

  if (dst && dst->value_type == _NORMAL_VALUE && src != 0) {
    double tmp = (double)fabs(src);
    for (; power < 28 && (int)tmp / (int)pow(2, 21) == 0; tmp *= 10, power++) {
    }
    tmp = round(tmp);
    if (power <= 28 && (exp > -94 && exp < 96)) {
      val mant;
      tmp = (float)tmp;
      for (; fmod(tmp, 10) == 0 && power > 0; power--, tmp /= 10) {
      }
      mant._float = tmp;
      exp = get_float_exp(&mant._float);
      dst->bits[exp / 32] |= 1 << exp % 32;
      for (int i = exp - 1, j = 22; j >= 0; i--, j--)
        if ((mant._int & (1 << j)) != 0) dst->bits[i / 32] |= 1 << i % 32;
      dst->bits[3] = (sign << 31) | (power << 16);
      success = TRUE;
    }
  }
  return success;
}

int _from_decimal_to_int(_decimal src, int *dst) {
  int success = FALSE;
  if (src.value_type == _NORMAL_VALUE) {
    *dst = src.bits[0];
    *dst *= src.bits[3] >> 31 ? -1 : 1;
    success = TRUE;
  }
  return success;
}

int _from_decimal_to_float(_decimal src, float *dst) {
  int success = FALSE, power = 0;
  if (src.value_type == _NORMAL_VALUE) {
    double dstTemp = 0;
    for (int i = 0; i < 96; i++)
      if ((src.bits[i / 32] & (1 << i % 32)) != 0) dstTemp += pow(2, i);
    if ((power = (src.bits[3] & ~SIGN) >> 16) > 0) {
      for (int i = power; i > 0; i--) dstTemp /= 10.0;
    }
    *dst = (float)dstTemp;
    *dst *= src.bits[3] >> 31 ? -1 : 1;
    success = TRUE;
  }
  return success;
}

void from_decimal_to_sDecimal(_decimal src, sDecimal *result) {
  zero_sDecimal(result);
  result->bits[6] = src.bits[3];
  result->bits[2] = src.bits[2];
  result->bits[1] = src.bits[1];
  result->bits[0] = src.bits[0];
}
