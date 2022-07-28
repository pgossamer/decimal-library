#include "_decimal.h"

_decimal _round(_decimal dst) {
  _decimal result = dst;
  if (result.value_type == _NORMAL_VALUE) {
    result = _truncate(dst);
    _decimal ten, res;
    res = _truncate(dst);
    _from_int_to_decimal(10, &ten);
    int exp = (dst.bits[3] << 1) >> 17;
    int sign = dst.bits[3];
    dst.bits[3] = res.bits[3] = 0;
    for (int i = 0; i < exp; i++) res = _mul(res, ten);
    res = _sub(dst, res);
    sDecimal sRes, sTen, sFinal;
    from_decimal_to_sDecimal(ten, &sTen);
    from_decimal_to_sDecimal(res, &sRes);
    for (int i = 0; i < exp - 1; i++) sRes = sDiv(sRes, sTen);
    if (sRes.bits[0] >= 5)
      sRes.bits[0] = 1;
    else
      sRes.bits[0] = 0;
    from_decimal_to_sDecimal(result, &sFinal);
    sFinal = sPlus(sFinal, sRes);
    result = normalize(sFinal);
    if (sign < 0) result.bits[3] |= SIGN;
  }
  return result;
}

_decimal _floor(_decimal dst) {
  _decimal result = dst;
  if (result.value_type == _NORMAL_VALUE) {
    result = _truncate(dst);
    _decimal res, ten;
    _from_int_to_decimal(10, &ten);
    int exp = (dst.bits[3] << 1) >> 17;
    int sign = (dst.bits[3] >> 31) << 31;
    res = _truncate(dst);
    for (int i = 0; i < exp; i++) res = _mul(res, ten);
    res = _sub(dst, res);
    sDecimal sRes, sOne, sZero, sFinal;
    zero_sDecimal(&sOne);
    zero_sDecimal(&sZero);
    sOne.bits[0] = 1;
    from_decimal_to_sDecimal(result, &sFinal);
    from_decimal_to_sDecimal(res, &sRes);
    if (sign < 0) {
      if (sDecimal_is_not_equal(sRes, sZero) == TRUE)
        sFinal = sPlus(sFinal, sOne);
      result = normalize(sFinal);
      result.bits[3] |= SIGN;
    }
  }
  return result;
}

_decimal _truncate(_decimal dst) {
  _decimal result = dst;
  if (result.value_type == _NORMAL_VALUE) {
    sDecimal ten, zero, sResult;
    zero_sDecimal(&ten);
    ten.bits[0] = 10;
    zero_sDecimal(&zero);
    int exp = (dst.bits[3] << 1) >> 17;
    int sign = dst.bits[3];
    result.bits[3] = 0;
    from_decimal_to_sDecimal(result, &sResult);
    for (int i = 0; i < exp; i++) {
      if (sDecimal_is_equal(sResult, zero) == TRUE) break;
      sResult = sDiv(sResult, ten);
    }
    result = normalize(sResult);
    if (sign < 0) result.bits[3] |= SIGN;
  }
  return result;
}

_decimal _negate(_decimal src) {
  if (src.value_type == _INFINITY)
    src.value_type = _NEGATIVE_INFINITY;
  else if (src.value_type == _NEGATIVE_INFINITY)
    src.value_type = _INFINITY;
  else if (src.value_type != _NAN)
    src.bits[3] ^= SIGN;
  return src;
}
