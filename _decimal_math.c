#include "_decimal.h"

_decimal _add(_decimal src1, _decimal src2) {
  _decimal res;
  res.value_type = src1.value_type + src2.value_type;
  if (res.value_type == 0) {
    sDecimal src1_copy, src2_copy, result;
    zero_sDecimal(&result);
    from_decimal_to_sDecimal(src1, &src1_copy);
    from_decimal_to_sDecimal(src2, &src2_copy);
    int sign1 = src1_copy.bits[6], sign2 = src2_copy.bits[6];
    src1_copy.bits[6] = (src1_copy.bits[6] << 1) >> 1;
    src2_copy.bits[6] = (src2_copy.bits[6] << 1) >> 1;
    norm_exp(&src1_copy, &src2_copy);
    if ((sign1 >= 0 && sign2 >= 0) || (sign1 < 0 && sign2 < 0)) {
      result = sPlus(src1_copy, src2_copy);
      if (sign1 < 0) sSet_bit(&result, 223);
    } else if (sign1 >= 0 && sign2 < 0) {
      if (sDecimal_is_greater_or_equal(src1_copy, src2_copy) == TRUE) {
        result = sMinus(src1_copy, src2_copy);
      } else {
        result = sMinus(src2_copy, src1_copy);
        sSet_bit(&result, 223);
      }
    } else if (sign1 < 0 && sign2 >= 0) {
      if (sDecimal_is_greater_or_equal(src1_copy, src2_copy) == TRUE) {
        result = sMinus(src1_copy, src2_copy);
        sSet_bit(&result, 223);
      } else {
        result = sMinus(src2_copy, src1_copy);
      }
    }
    res = normalize(result);
  } else {
    if (res.value_type == 1) {
      res.value_type = _INFINITY;
    } else if (res.value_type == 2) {
      if (src1.value_type == 1 && src2.value_type == 1)
        res.value_type = _INFINITY;
      else
        res.value_type = _NEGATIVE_INFINITY;
    } else if (res.value_type == 4) {
      if (src1.value_type == 2 && src2.value_type == 2)
        res.value_type = _NEGATIVE_INFINITY;
      else
        res.value_type = _NAN;
    } else {
      res.value_type = _NAN;
    }
    res.bits[3] = res.bits[2] = res.bits[1] = res.bits[0] = 0;
  }
  return res;
}

_decimal _sub(_decimal src1, _decimal src2) {
  _decimal res;
  res.value_type = src1.value_type + src2.value_type;
  if (res.value_type == 0) {
    sDecimal src1_copy, src2_copy, result;
    zero_sDecimal(&result);
    from_decimal_to_sDecimal(src1, &src1_copy);
    from_decimal_to_sDecimal(src2, &src2_copy);
    int sign1 = src1_copy.bits[6], sign2 = src2_copy.bits[6];
    src1_copy.bits[6] = (src1_copy.bits[6] << 1) >> 1;
    src2_copy.bits[6] = (src2_copy.bits[6] << 1) >> 1;
    norm_exp(&src1_copy, &src2_copy);
    if ((sign1 < 0 && sign2 >= 0) || (sign1 >= 0 && sign2 < 0)) {
      result = sPlus(src1_copy, src2_copy);
      if (sign1 < 0) sSet_bit(&result, 223);
    } else if (sign1 >= 0 && sign2 >= 0) {
      if (sDecimal_is_greater_or_equal(src1_copy, src2_copy) == TRUE) {
        result = sMinus(src1_copy, src2_copy);
      } else {
        result = sMinus(src2_copy, src1_copy);
        sSet_bit(&result, 223);
      }
    } else if (sign1 < 0 && sign2 < 0) {
      if (sDecimal_is_less(src1_copy, src2_copy) == TRUE) {
        result = sMinus(src2_copy, src1_copy);
      } else {
        result = sMinus(src1_copy, src2_copy);
        sSet_bit(&result, 223);
      }
    }
    res = normalize(result);
  } else {
    if (res.value_type == 1) {
      if (src1.value_type == 0)
        res.value_type = _NEGATIVE_INFINITY;
      else
        res.value_type = _INFINITY;
    } else if (res.value_type == 2) {
      if (src1.value_type == 1 && src2.value_type == 1) {
        res.value_type = _NAN;
      } else {
        if (src1.value_type == 0)
          res.value_type = _INFINITY;
        else
          res.value_type = _NEGATIVE_INFINITY;
      }
    } else if (res.value_type == 3) {
      res.value_type = _NAN;
      if (src1.value_type == 1)
        res.value_type = _INFINITY;
      else
        res.value_type = _NEGATIVE_INFINITY;
    } else if (res.value_type == 4) {
      res.value_type = _NAN;
    } else {
      res.value_type = _NAN;
    }
    res.bits[3] = res.bits[2] = res.bits[1] = res.bits[0] = 0;
  }
  return res;
}

_decimal _mul(_decimal src1, _decimal src2) {
  _decimal result;
  zero_decimal(&result);
  int val = src1.value_type | src2.value_type;
  if (val == 0) {
    sDecimal tmp1, tmp2;
    zero_sDecimal(&tmp1);
    zero_sDecimal(&tmp2);
    tmp1.bits[6] = src1.bits[3];
    tmp2.bits[6] = src2.bits[3];
    tmp1.bits[2] = src1.bits[2];
    tmp2.bits[2] = src2.bits[2];
    tmp1.bits[1] = src1.bits[1];
    tmp2.bits[1] = src2.bits[1];
    tmp1.bits[0] = src1.bits[0];
    tmp2.bits[0] = src2.bits[0];
    int sgn1 = sCheck_bit(tmp1, 223);
    int sgn2 = sCheck_bit(tmp2, 223);
    int sgn = sgn1 ^ sgn2;
    norm_exp(&tmp1, &tmp2);
    sMul(&tmp1, &tmp2);
    int exp = ((tmp1.bits[6] << 1) >> 1);
    tmp1.bits[6] = sgn << 31 | exp << 16;
    result = normalize(tmp1);
  } else {
    zero_decimal(&result);
    result.bits[3] = 0;
    if ((src1.value_type == 1 && src2.value_type == 1) ||
        (src1.value_type == 1 && src2.value_type == 0 &&
         elder_bit(src2) != -1 && check_bit(src2, 127) == 0) ||
        (src1.value_type == 2 && src2.value_type == 0 &&
         elder_bit(src2) != -1 && check_bit(src2, 127) == 1) ||
        (src1.value_type == 0 && elder_bit(src1) != -1 &&
         check_bit(src1, 127) == 1 && src2.value_type == 2) ||
        (src1.value_type == 0 && elder_bit(src1) != -1 &&
         check_bit(src1, 127) == 0 && src2.value_type == 1))
      result.value_type = 1;
    else if ((src1.value_type == 2 && src2.value_type == 2) ||
             (src1.value_type == 2 && src2.value_type == 0 &&
              elder_bit(src2) != -1 && check_bit(src2, 127) == 0) ||
             (src1.value_type == 0 && elder_bit(src1) != -1 &&
              check_bit(src1, 127) == 0 && src2.value_type == 2) ||
             (src1.value_type == 1 && src2.value_type == 0 &&
              elder_bit(src2) != -1 && check_bit(src2, 127) == 1) ||
             (src1.value_type == 0 && elder_bit(src1) != -1 &&
              check_bit(src1, 127) == 1 && src2.value_type == 1) ||
             (src1.value_type == 1 && src2.value_type == 2) ||
             (src1.value_type == 1 && src2.value_type == 2))
      result.value_type = 2;
    else
      result.value_type = 3;
  }
  return result;
}

_decimal _div(_decimal src1, _decimal src2) {
  int val = src1.value_type | src2.value_type;
  _decimal res;
  zero_decimal(&res);
  res.bits[3] = 0;
  if (val == 0 && elder_bit(src2) != -1) {
    sDecimal tmp1, tmp2, result, float_res, ten;
    zero_sDecimal(&ten);
    ten.bits[6] = 0;
    ten.bits[0] = 10;
    zero_sDecimal(&result);
    result.bits[6] = 0;
    zero_sDecimal(&float_res);
    float_res.bits[6] = 0;
    from_decimal_to_sDecimal(src1, &tmp1);
    from_decimal_to_sDecimal(src2, &tmp2);
    norm_exp(&tmp1, &tmp2);
    sDecimal buf = tmp1;
    int cnt1 = elder_sBit(tmp1);
    int cnt2 = elder_sBit(tmp2);
    int cnt = cnt1 - cnt2;
    for (int i = cnt; i > 0; i--) sDecimal_shift_right(&buf);
    for (int j = cnt; j > 0; j--) {
      if (sCheck_bit(tmp1, j)) sSet_bit(&buf, 0);
      if (sDecimal_is_greater_or_equal(buf, tmp2) == TRUE) {
        result.bits[0] += 1;
        sDecimal_shift_left(&result);
        buf = sMinus(buf, tmp2);
        sDecimal_shift_left(&buf);
      } else {
        sDecimal_shift_left(&result);
        sDecimal_shift_left(&buf);
      }
    }
    sDecimal_shift_right(&buf);
    if (sDecimal_is_greater_or_equal(buf, tmp2) == TRUE) {
      result.bits[0] += 1;
      buf = sMinus(buf, tmp2);
    } else {
      sDecimal_shift_left(&buf);
    }
    if (elder_sBit(buf) != -1) {
      sDecimal ten;
      zero_sDecimal(&ten);
      ten.bits[6] = 0;
      ten.bits[0] = 10;
      int exp = 0;
      while (elder_sBit(buf) < 189 && exp < 28) {
        sMul(&buf, &ten);
        exp++;
      }
      float_res = sDiv(buf, tmp2);
      float_res.bits[6] = exp << 16;
      norm_exp(&result, &float_res);
      result = sPlus(result, float_res);
      result.bits[6] = exp << 16;
    }
    res = normalize(result);
    res.bits[3] |= ((src1.bits[3] >> 31) << 31) ^ ((src2.bits[3] >> 31) << 31);
    res.value_type = _NORMAL_VALUE;
  } else {
    zero_decimal(&res);
    res.bits[3] = 0;
    if ((src1.value_type == (1 || 2) && src1.value_type == (1 || 2)) ||
        (src1.value_type == 3 || src2.value_type == 3) ||
        (elder_bit(src1) == -1 && elder_bit(src2) == -1))
      res.value_type = 3;
    else if ((src1.value_type == 0 && check_bit(src1, 127) == 0 &&
              src2.value_type == 2) ||
             (src1.value_type == 0 && check_bit(src1, 127) == 1 &&
              src2.value_type == 1) ||
             (src1.value_type == 0 && check_bit(src1, 127) == 1 &&
              elder_bit(src2) == -1 && check_bit(src2, 127) == 0 &&
              src2.value_type == 0) ||
             (src1.value_type == 0 && check_bit(src1, 127) == 0 &&
              elder_bit(src2) == -1 && check_bit(src2, 127) == 1 &&
              src2.value_type == 0))
      res.value_type = 2;
    else
      res.value_type = 1;
  }
  return res;
}

_decimal _mod(_decimal src1, _decimal src2) {
  _decimal res;
  zero_decimal(&res);
  res.bits[3] = 0;
  int sgn = ((src1.bits[3] >> 31) << 31);
  int val = src1.value_type | src2.value_type;
  src1.bits[3] = ((src1.bits[3] << 1) >> 1);
  src2.bits[3] = ((src2.bits[3] << 1) >> 1);
  if (_is_greater(src1, src2) == TRUE && val == 0) {
    sDecimal tmp1, tmp2, result, ten;
    zero_sDecimal(&ten);
    ten.bits[6] = 0;
    ten.bits[0] = 10;
    zero_sDecimal(&result);
    result.bits[6] = 0;
    from_decimal_to_sDecimal(src1, &tmp1);
    from_decimal_to_sDecimal(src2, &tmp2);
    norm_exp(&tmp1, &tmp2);
    sDecimal buf = tmp1;
    int cnt1 = elder_sBit(tmp1);
    int cnt2 = elder_sBit(tmp2);
    int cnt = cnt1 - cnt2;
    for (int i = cnt; i > 0; i--) sDecimal_shift_right(&buf);
    for (int j = cnt; j >= 0; j--) {
      if (sCheck_bit(tmp1, j)) sSet_bit(&buf, 0);
      if (sDecimal_is_greater_or_equal(buf, tmp2) == TRUE) {
        result.bits[0] += 1;
        sDecimal_shift_left(&result);
        buf = sMinus(buf, tmp2);
        sDecimal_shift_left(&buf);
      } else {
        sDecimal_shift_left(&result);
        sDecimal_shift_left(&buf);
      }
    }
    sDecimal_shift_right(&buf);
    if (sDecimal_is_greater_or_equal(buf, tmp2) == TRUE) {
      result.bits[0] += 1;
      buf = sMinus(buf, tmp2);
    }
    res = normalize(buf);
    res.bits[3] |= sgn;
    res.value_type = _NORMAL_VALUE;
  } else {
    if (_is_greater(src1, src2) == FALSE && val == 0) {
      res = src1;
      res.bits[3] |= sgn;
    } else {
      zero_decimal(&res);
      res.bits[3] = 0;
    }
  }
  return res;
}

void norm_exp(sDecimal *tmp1, sDecimal *tmp2) {
  sDecimal ten;
  zero_sDecimal(&ten);
  ten.bits[6] = 0;
  ten.bits[0] = 10;
  int sgn1 = sCheck_bit(*tmp1, 223);
  int sgn2 = sCheck_bit(*tmp2, 223);
  int exp1 = ((tmp1->bits[6] << 1) >> 17);
  int exp2 = ((tmp2->bits[6] << 1) >> 17);
  int exp_r = exp1 - exp2;
  if (exp_r > 0) {
    for (int i = exp_r; i > 0; i--) {
      sMul(tmp2, &ten);
      exp2++;
    }
  } else {
    for (int i = -exp_r; i > 0; i--) {
      sMul(tmp1, &ten);
      exp1++;
    }
  }
  tmp1->bits[6] = sgn1 << 31 | exp1 << 16;
  tmp2->bits[6] = sgn2 << 31 | exp2 << 16;
}

_decimal normalize(sDecimal src) {
  _decimal result;
  sDecimal ten;
  zero_sDecimal(&ten);
  ten.bits[6] = 0;
  ten.bits[0] = 10;
  int exp = ((src.bits[6] << 1) >> 17);
  int sgn = sCheck_bit(src, 223);
  int rd = 0;
  while (elder_sBit(src) > 95) {
    src = sDiv(src, ten);
    exp--;
    rd = 0;
  }
  if (rd != 0) src.bits[0] += 1;
  if (exp >= 0) {
    result.bits[3] = exp << 16 | sgn << 31;
    result.bits[2] = src.bits[2];
    result.bits[1] = src.bits[1];
    result.bits[0] = src.bits[0];
    result.value_type = 0;
  } else {
    zero_decimal(&result);
    result.bits[3] = 0;
  }
  return result;
}
