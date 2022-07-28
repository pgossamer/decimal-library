#include "_decimal.h"

int get_degree(int *bits) { return (*bits & ~SIGN) >> 16; }
int get_float_sign(float *src) { return *(int *)src >> 31; }
int get_float_exp(float *src) { return ((*(int *)src & ~SIGN) >> 23) - 127; }
int _sum(_decimal *a, _decimal *b) {
  return a->bits[0] | a->bits[1] | a->bits[2] | b->bits[0] | b->bits[1] |
         b->bits[2];
}

// COMPARE FUNCTIONS
int dec_types(_decimal a, value_type_t t1, _decimal b, value_type_t t2) {
  return (a.value_type == t1) && (b.value_type == t2);
}

int dec_type(_decimal a, _decimal b, value_type_t t) {
  return (a.value_type == t) && (b.value_type == t);
}

int _cmp_vt(_decimal a, _decimal b) {
  return (dec_types(a, _NEGATIVE_INFINITY, b, _INFINITY) |
          dec_types(a, _NEGATIVE_INFINITY, b, _NORMAL_VALUE) |
          dec_types(a, _NORMAL_VALUE, b, _INFINITY)) == 0;
}

int _is_infinity(_decimal a, _decimal b) {
  return (dec_type(a, b, _INFINITY) | dec_type(a, b, _NEGATIVE_INFINITY)) == 1;
}

int _is_normal(_decimal a, _decimal b) {
  return dec_type(a, b, _NORMAL_VALUE) == 1;
}

// MATH FUNCTIONS
void equate_degree(_decimal *a, _decimal *b) {
  char d1 = get_degree(&a->bits[3]), d2 = get_degree(&b->bits[3]);
  if (d1 != d2) increase_degree((d1 < d2 ? a : b), abs(d1 - d2));
}

void increase_degree(_decimal *dec, char deg) {
  _decimal ten;
  _from_float_to_decimal(10, &ten);
  while (deg--) mul(dec, &ten);
}

// BINAR OPERATIONS
int elder_bit(_decimal dst) {
  int result = -1;
  for (int i = 95; i >= 0; i--) {
    if (check_bit(dst, i)) {
      result = i;
      break;
    }
  }
  return result;
}

int elder_sBit(sDecimal dst) {
  int result = -1;
  for (int i = 191; i >= 0; i--) {
    if (sCheck_bit(dst, i)) {
      result = i;
      break;
    }
  }
  return result;
}

void sDecimal_shift_right(sDecimal *dst) {
  sDecimal copy = *dst;
  zero_sDecimal(&copy);
  for (int i = elder_sBit(*dst); i > 0; i--) {
    if (sCheck_bit(*dst, i)) sSet_bit(&copy, i - 1);
  }
  *dst = copy;
}

void sDecimal_shift_left(sDecimal *dst) {
  sDecimal copy = *dst;
  ;
  zero_sDecimal(&copy);
  for (int i = 191; i > 0; i--) {
    if (sCheck_bit(*dst, i - 1)) sSet_bit(&copy, i);
  }
  *dst = copy;
}

int check_bit(_decimal dst, int bit_number) {
  int indexBits = bit_number / 32;
  int bit_numberToBits = bit_number % 32;
  return ((dst.bits[indexBits] & (1 << bit_numberToBits)) != 0);
}

void set_bit(_decimal *dst, int bit_number) {
  int indexBits = bit_number / 32;
  int bit_numberToBits = bit_number % 32;
  dst->bits[indexBits] = dst->bits[indexBits] | (1 << bit_numberToBits);
}

int sCheck_bit(sDecimal dst, int bit_number) {
  int indexBits = bit_number / 32;
  int bit_numberToBits = bit_number % 32;
  return ((dst.bits[indexBits] & (1 << bit_numberToBits)) != 0);
}

void sSet_bit(sDecimal *dst, int bit_number) {
  int indexBits = bit_number / 32;
  int bit_numberToBits = bit_number % 32;
  dst->bits[indexBits] = dst->bits[indexBits] | (1 << bit_numberToBits);
}

// MATH ALGORITHMS

void mul(_decimal *src1, _decimal *src2) {
  sDecimal tmp, cp;
  zero_sDecimal(&tmp);
  zero_sDecimal(&cp);
  cp.bits[2] = src2->bits[2];
  cp.bits[1] = src2->bits[1];
  cp.bits[0] = src2->bits[0];
  int cnt = elder_bit(*src1);
  for (int i = 0; i <= cnt; i++) {
    if (check_bit(*src1, i)) {
      tmp = sPlus(tmp, cp);
      sDecimal_shift_left(&cp);
    } else {
      sDecimal_shift_left(&cp);
    }
  }
  src1->bits[0] = tmp.bits[0];
  src1->bits[1] = tmp.bits[1];
  src1->bits[2] = tmp.bits[2];
}

void sMul(sDecimal *src1, sDecimal *src2) {
  sDecimal tmp, cp;
  zero_sDecimal(&tmp);
  zero_sDecimal(&cp);
  cp.bits[2] = src2->bits[2];
  cp.bits[1] = src2->bits[1];
  cp.bits[0] = src2->bits[0];
  int cnt = elder_sBit(*src1);
  for (int i = 0; i <= cnt; i++) {
    if (sCheck_bit(*src1, i)) {
      tmp = sPlus(tmp, cp);
      sDecimal_shift_left(&cp);
    } else {
      sDecimal_shift_left(&cp);
    }
  }
  src1->bits[0] = tmp.bits[0];
  src1->bits[1] = tmp.bits[1];
  src1->bits[2] = tmp.bits[2];
  src1->bits[3] = tmp.bits[3];
  src1->bits[4] = tmp.bits[4];
  src1->bits[5] = tmp.bits[5];
}

sDecimal sDiv(sDecimal src1, sDecimal src2) {
  sDecimal tmp = src1, result;
  zero_sDecimal(&result);
  result.bits[6] = 0;
  int cnt1 = elder_sBit(src1);
  int cnt2 = elder_sBit(src2);
  int cnt = cnt1 - cnt2;
  for (int i = cnt; i > 0; i--) sDecimal_shift_right(&tmp);
  for (int j = cnt; j > 0; j--) {
    if (sCheck_bit(src1, j)) sSet_bit(&tmp, 0);
    if (sDecimal_is_greater_or_equal(tmp, src2) == TRUE) {
      result.bits[0] += 1;
      sDecimal_shift_left(&result);
      tmp = sMinus(tmp, src2);
      sDecimal_shift_left(&tmp);
    } else {
      sDecimal_shift_left(&result);
      sDecimal_shift_left(&tmp);
    }
  }
  if (sDecimal_is_greater_or_equal(tmp, src2) == TRUE) result.bits[0] += 1;
  return result;
}

sDecimal sPlus(sDecimal dst, sDecimal src) {
  sDecimal tmp;
  zero_sDecimal(&tmp);
  int carry = 0;
  for (int i = 0; i < 191; i++) {
    if (sCheck_bit(dst, i) + sCheck_bit(src, i) + carry == 1) {
      sSet_bit(&tmp, i);
      if (carry == 1) carry = 0;
    } else if (sCheck_bit(dst, i) + sCheck_bit(src, i) + carry == 2) {
      carry = 1;
    } else if (sCheck_bit(dst, i) + sCheck_bit(src, i) + carry == 3) {
      sSet_bit(&tmp, i);
      carry = 1;
    }
  }
  tmp.bits[6] = dst.bits[6] | src.bits[6];
  return tmp;
}

sDecimal sMinus(sDecimal src1, sDecimal src2) {
  sDecimal tmp, sOne;
  zero_sDecimal(&tmp);
  int carry = 0;
  _decimal one;
  _from_int_to_decimal(1, &one);
  from_decimal_to_sDecimal(one, &sOne);
  src2.bits[0] = ~src2.bits[0];
  src2.bits[1] = ~src2.bits[1];
  src2.bits[2] = ~src2.bits[2];
  src2.bits[3] = ~src2.bits[3];
  src2.bits[4] = ~src2.bits[4];
  src2.bits[5] = ~src2.bits[5];
  src2 = sPlus(src2, sOne);
  int cnt = elder_sBit(src1);
  for (int i = 0; i <= cnt; i++) {
    if (sCheck_bit(src1, i) + sCheck_bit(src2, i) + carry == 1) {
      sSet_bit(&tmp, i);
      if (carry == 1) carry = 0;
    } else if (sCheck_bit(src1, i) + sCheck_bit(src2, i) + carry == 2) {
      carry = 1;
    } else if (sCheck_bit(src1, i) + sCheck_bit(src2, i) + carry == 3) {
      sSet_bit(&tmp, i);
      carry = 1;
    }
  }
  tmp.bits[6] = src1.bits[6] | src2.bits[6];
  return tmp;
}

void zero_sDecimal(sDecimal *src) {
  src->bits[6] = src->bits[5] = src->bits[4] = 0;
  src->bits[3] = src->bits[2] = src->bits[1] = src->bits[0] = 0;
}

void zero_decimal(_decimal *src) {
  src->bits[3] = src->bits[2] = src->bits[1] = src->bits[0] = 0;
  src->value_type = _NORMAL_VALUE;
}
