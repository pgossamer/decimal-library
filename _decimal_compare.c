#include "_decimal.h"

int _is_less(_decimal src1, _decimal src2) {
  return _is_normal(src1, src2) ? _is_(_less, src1, src2) : _cmp_vt(src1, src2);
}

int _is_less_or_equal(_decimal src1, _decimal src2) {
  return _is_equal(src1, src2) & _is_less(src1, src2);
}

int _is_greater(_decimal src1, _decimal src2) {
  return _is_normal(src1, src2) ? _is_(_greater, src1, src2)
                                : _cmp_vt(src2, src1);
}

int _is_greater_or_equal(_decimal src1, _decimal src2) {
  return _is_equal(src1, src2) & _is_greater(src1, src2);
}

int _is_equal(_decimal src1, _decimal src2) {
  int diff = 0, inf = _is_infinity(src1, src2), norm = _is_normal(src1, src2);
  for (int i = 0; i < BITS_COUNT; i++) diff |= src1.bits[i] - src2.bits[i];
  return norm ? (!_sum(&src1, &src2) ? TRUE : (!diff ? TRUE : FALSE))
              : (inf && !diff ? TRUE : FALSE);
}

int _is_not_equal(_decimal src1, _decimal src2) {
  return !_is_equal(src1, src2);
}

int _is_(int(c)(int *, int *), _decimal a, _decimal b) {
  int _a = 0, _b = 0, _a_ml = 0, _b_ml = 0, res = FALSE, ls = a.bits[3] >> 31,
      rs = b.bits[3] >> 31;
  equate_degree(&a, &b);
  _a_ml = elder_bit(a);
  _b_ml = elder_bit(b);
  if (_a_ml == _b_ml) {
    for (int i = 95; i >= 0; i--) {
      int j = i / 32, n = i % 32;
      _a += a.bits[j] & (1 << n);
      _b += b.bits[j] & (1 << n);
    }
    res = ls == 0 ? !c(&_a, &_b) : !c(&_b, &_a);
  } else {
    res = ls == 0 ? !c(&_a_ml, &_b_ml) : !c(&_b_ml, &_a_ml);
  }
  return ls != rs ? (ls > rs ? !c(&ls, &rs) : c(&rs, &ls)) : res;
}

int _less(int *a, int *b) { return *a < *b; }
int _greater(int *a, int *b) { return *a > *b; }

// SUPER DECIMAL COMPARE //
int sDecimal_is_less(sDecimal src1, sDecimal src2) {
  int res = FALSE;
  for (int i = 191; i >= 0; i--) {
    if (sCheck_bit(src1, i) < sCheck_bit(src2, i)) {
      res = TRUE;
      break;
    } else if (sCheck_bit(src1, i) > sCheck_bit(src2, i)) {
      break;
    }
  }
  return res;
}

int sDecimal_is_less_or_equal(sDecimal src1, sDecimal src2) {
  int res = FALSE;
  if (sDecimal_is_equal(src1, src2) == TRUE ||
      sDecimal_is_less(src1, src2) == TRUE)
    res = TRUE;
  return res;
}

int sDecimal_is_greater(sDecimal src1, sDecimal src2) {
  int res = FALSE;
  for (int i = 191; i >= 0; i--) {
    if (sCheck_bit(src1, i) > sCheck_bit(src2, i)) {
      res = TRUE;
      break;
    } else if (sCheck_bit(src1, i) < sCheck_bit(src2, i)) {
      break;
    }
  }
  return res;
}

int sDecimal_is_greater_or_equal(sDecimal src1, sDecimal src2) {
  int res = FALSE;
  if (sDecimal_is_equal(src1, src2) == TRUE ||
      sDecimal_is_greater(src1, src2) == TRUE)
    res = TRUE;
  return res;
}

int sDecimal_is_equal(sDecimal src1, sDecimal src2) {
  int res = TRUE;
  for (int i = 191; i >= 0; i--) {
    if (sCheck_bit(src1, i) < sCheck_bit(src2, i)) {
      res = FALSE;
      break;
    } else if (sCheck_bit(src1, i) > sCheck_bit(src2, i)) {
      res = FALSE;
      break;
    }
  }
  return res;
}

int sDecimal_is_not_equal(sDecimal src1, sDecimal src2) {
  int res = TRUE;
  if (sDecimal_is_equal(src1, src2) == TRUE) res = FALSE;
  return res;
}
