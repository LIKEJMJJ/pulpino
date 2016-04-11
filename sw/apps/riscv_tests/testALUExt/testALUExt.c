// This testbench checks the basic functionality of:
//
// p.min
// p.minu
// p.max
// p.maxu
// p.avg
// p.avgu
// p.abs
//
// p.exthz
// p.exths
// p.extbz
// p.extbs

#include <stdio.h>
#include "utils.h"
#include "bench.h"

#include "testALUExt_stimuli.h"


void check_min(testresult_t *result, void (*start)(), void (*stop)());
void check_max(testresult_t *result, void (*start)(), void (*stop)());
void check_avg(testresult_t *result, void (*start)(), void (*stop)());
void check_abs(testresult_t *result, void (*start)(), void (*stop)());
void check_ext(testresult_t *result, void (*start)(), void (*stop)());

testcase_t testcases[] = {
  { .name = "min", .test = check_min },
  { .name = "max", .test = check_max },
  { .name = "avg", .test = check_avg },
  { .name = "abs", .test = check_abs },
  { .name = "ext", .test = check_ext },
  {0, 0},
};


int main()
{
  if(get_core_id() == 0) {
    return run_suite(testcases);
  }

  return 0;
}


#define util_check_rr(asm_str, i, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a], %[b]\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i]), \
                      [b] "r" (prefix ## _b[i])); \
      check_uint32(result, asm_str, act, prefix ## _exp[i]); \
    }

#define util_check_r(asm_str, i, prefix) \
    for(i = 0; i < (sizeof(prefix ## _a)/4); i++) { \
      asm volatile (asm_str " %[c], %[a]\n" \
                    : [c] "+r" (act) \
                    : [a] "r"  (prefix ## _a[i])); \
      check_uint32(result, asm_str, act, prefix ## _exp[i]); \
    }


void check_avg(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  util_check_rr("p.avg",  i, g_avg);
  util_check_rr("p.avgu", i, g_avgu);
}

void check_min(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  util_check_rr("p.min",  i, g_min);
  util_check_rr("p.minu", i, g_minu);
}

void check_max(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  util_check_rr("p.max",  i, g_max);
  util_check_rr("p.maxu", i, g_maxu);
}


void check_abs(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  util_check_r("p.abs",  i, g_abs);
}

void check_ext(testresult_t *result, void (*start)(), void (*stop)()) {
  unsigned int i;
  unsigned int act;

  //-----------------------------------------------------------------
  // Check lv.ext
  //-----------------------------------------------------------------
  util_check_r("p.exths", i, g_ext_hs);
  util_check_r("p.exthz", i, g_ext_hz);

  util_check_r("p.extbs", i, g_ext_bs);
  util_check_r("p.extbz", i, g_ext_bz);
}

