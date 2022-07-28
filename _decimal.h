#ifndef SRC__DECIMAL_H_
#define SRC__DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIGN 0x80000000
#define SCALE 0xFF0000

#define FALSE 1
#define TRUE 0

#define BITS_COUNT 4
#define SBITS_COUNT 7

typedef unsigned long long uint64;
typedef unsigned int uint32;

typedef union {
  uint32 _int;
  float _float;
} val;

typedef enum {
  _NORMAL_VALUE = 0,
  _INFINITY = 1,
  _NEGATIVE_INFINITY = 2,
  _NAN = 3
} value_type_t;

typedef struct {
  int bits[BITS_COUNT];
  value_type_t value_type;
} _decimal;

typedef struct {
  int bits[SBITS_COUNT];
} sDecimal;

// MATH //
_decimal _add(_decimal src1, _decimal src2);
_decimal _negate(_decimal src);
_decimal _sub(_decimal src1, _decimal src2);
_decimal _mul(_decimal src1, _decimal src2);
_decimal _div(_decimal src1, _decimal src2);
_decimal _mod(_decimal src1, _decimal src2);
_decimal _truncate(_decimal dst);
_decimal _floor(_decimal dst);
_decimal _round(_decimal dst);

// CONVERTERS AND PARSERS //
int _from_int_to_decimal(int src, _decimal *dst);
int _from_float_to_decimal(float src, _decimal *dst);
int _from_decimal_to_int(_decimal src, int *dst);
int _from_decimal_to_float(_decimal src, float *dst);

// COMPARE FUNCTIONS //
int _is_equal(_decimal src1, _decimal src2);
int _is_not_equal(_decimal src1, _decimal src2);
int _is_less(_decimal src1, _decimal src2);
int _is_greater(_decimal src1, _decimal src2);
int _is_less_or_equal(_decimal src1, _decimal src2);
int _is_greater_or_equal(_decimal src1, _decimal src2);
int _is_(int(c)(int *, int *), _decimal a, _decimal b);
int _less(int *a, int *b);
int _greater(int *a, int *b);

// SUBSIDIARY FUNCTIONS //
_decimal *cpy_dec(_decimal *dst, _decimal src);
void increase_degree(_decimal *dec, char deg);
void equate_degree(_decimal *a, _decimal *b);

int _cmp_vt(_decimal a, _decimal b);
int dec_types(_decimal a, value_type_t t1, _decimal b, value_type_t t2);
int dec_type(_decimal a, _decimal b, value_type_t t);
int _is_infinity(_decimal a, _decimal b);
int _is_normal(_decimal a, _decimal b);

int get_float_sign(float *src);
int get_float_exp(float *src);
int _sum(_decimal *a, _decimal *b);
int get_sign(int *bits);
int get_degree(int *bits);

// NEW DECIMAL //

int check_bit(_decimal dst, int bit_number);
void set_bit(_decimal *dst, int bit_number);
int sCheck_bit(sDecimal dst, int bit_number);
void sSet_bit(sDecimal *dst, int bit_number);

void sDecimal_shift_right(sDecimal *dst);
void sDecimal_shift_left(sDecimal *dst);

void plus(_decimal *src1, _decimal *src2);
sDecimal sPlus(sDecimal dst, sDecimal src);
void mul(_decimal *src1, _decimal *src2);
void sMul(sDecimal *src1, sDecimal *src2);
void minus(_decimal *src1, _decimal *src2);
sDecimal sMinus(sDecimal src1, sDecimal src2);
sDecimal sDiv(sDecimal src1, sDecimal src2);
sDecimal sMod(sDecimal src1, sDecimal src2);

int elder_bit(_decimal dst);
int elder_sBit(sDecimal dst);

void zero_sDecimal(sDecimal *src);
void zero_decimal(_decimal *src);
void from_decimal_to_sDecimal(_decimal src, sDecimal *result);

void norm_exp(sDecimal *tmp11, sDecimal *tmp2);
_decimal normalize(sDecimal src);

int sDecimal_is_less(sDecimal src1, sDecimal src2);
int sDecimal_is_less_or_equal(sDecimal src1, sDecimal src2);
int sDecimal_is_greater(sDecimal src1, sDecimal src2);
int sDecimal_is_greater_or_equal(sDecimal src1, sDecimal src2);
int sDecimal_is_equal(sDecimal src1, sDecimal src2);
int sDecimal_is_not_equal(sDecimal src1, sDecimal src2);

#endif  //  SRC__DECIMAL_H_
