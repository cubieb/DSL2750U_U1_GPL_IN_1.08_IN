#ifndef BASIC_H
#define BASIC_H

/*___________________________________________________________________________
 |                                                                           |
 |   Constants and Globals                                                   |
 |___________________________________________________________________________|
*/
extern Flag Overflow;
extern Flag Carry;

/*___________________________________________________________________________
 |                                                                           |
 |   Operators prototypes                                                    |
 |___________________________________________________________________________|
*/

Word16 add_safe(Fast16 var1, Fast16 var2);     /* Short add,           1 */
Word16 sub_safe(Fast16 var1, Fast16 var2);     /* Short sub,           1 */
Word16 shr_safe(Fast16 var1, Fast16 var2);     /* Short shift right,   1 */
Word16 shl_safe(Fast16 var1, Fast16 var2);     /* Short shift right,   1 */
Word32 L_mult_safe(Word16 var1, Word16 var2);  /* Long mult,           1 */
Word32 L_mult_no_overflow(Word16 var1, Word16 var2);  /* Long mult,           1 */
Word32 L_add_w_overflow(Word32 L_var1, Word32 L_var2);
Word32 L_sub_w_overflow(Word32 L_var1, Word32 L_var2);
Word32 L_mac_w_overflow(Word32 L_var3, Word16 var1, Word16 var2);
Word32 L_msu_w_overflow(Word32 L_var3, Word16 var1, Word16 var2);
Word32 L_shl_w_overflow(Word32 L_var1, Fast16 var2);
Word16 add_w_overflow(Word16 var1,Word16 var2);
Word16 sub_w_overflow(Word16 var1,Word16 var2);

Word16 sature(Word32 L_var1);             /* Limit to 16 bits,    1 */
Word16 add(Word16 var1, Word16 var2);     /* Short add,           1 */
Word16 sub(Word16 var1, Word16 var2);     /* Short sub,           1 */
Word16 abs_s(Word16 var1);                /* Short abs,           1 */
Word16 shl(Word16 var1, Fast16 var2);     /* Short shift left,    1 */
Word16 shr(Word16 var1, Fast16 var2);     /* Short shift right,   1 */
Word16 mult(Word16 var1, Word16 var2);    /* Short mult,          1 */
Word32 L_mult(Word16 var1, Word16 var2);  /* Long mult,           1 */
Word16 negate(Word16 var1);               /* Short negate,        1 */
Word16 extract_h(Word32 L_var1);          /* Extract high,        1 */
Word16 extract_l(Word32 L_var1);          /* Extract low,         1 */
Word16 round(Word32 L_var1);              /* Round,               1 */
Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2); /* Mac,    1 */
Word32 L_msu(Word32 L_var3, Word16 var1, Word16 var2); /* Msu,    1 */
Word32 L_macNs(Word32 L_var3, Word16 var1, Word16 var2);/* Mac without sat, 1*/
Word32 L_msuNs(Word32 L_var3, Word16 var1, Word16 var2);/* Msu without sat, 1*/

Word32 L_add(Word32 L_var1, Word32 L_var2);   /* Long add,        2 */
Word32 L_sub(Word32 L_var1, Word32 L_var2);   /* Long sub,        2 */
Word32 L_add_c(Word32 L_var1, Word32 L_var2); /*Long add with c,  2 */
Word32 L_sub_c(Word32 L_var1, Word32 L_var2); /*Long sub with c,  2 */
Word32 L_negate(Word32 L_var1);               /* Long negate,     2 */
Word16 mult_r(Word16 var1, Word16 var2);  /* Mult with round,     2 */
Word32 L_shl(Word32 L_var1, Fast16 var2); /* Long shift left,     2 */
Word32 L_shr(Word32 L_var1, Fast16 var2); /* Long shift right,    2 */
Word16 shr_r(Word16 var1, Word16 var2);/* Shift right with round, 2 */
Word16 mac_r(Word32 L_var3, Word16 var1, Word16 var2);/* Mac with rounding, 2*/
Word16 msu_r(Word32 L_var3, Word16 var1, Word16 var2);/* Msu with rounding, 2*/
Word32 L_deposit_h(Word16 var1);       /* 16 bit var1 -> MSB,     2 */
Word32 L_deposit_l(Word16 var1);       /* 16 bit var1 -> LSB,     2 */

Word32 L_shr_r(Word32 L_var1, Word16 var2);/* Long shift right with round,  3*/
Word32 L_abs(Word32 L_var1);            /* Long abs,              3 */

Word32 L_sat(Word32 L_var1);            /* Long saturation,       4 */

Word16 norm_s(Word16 var1);             /* Short norm,           15 */

Word16 div_s(Word16 var1, Word16 var2); /* Short division,       18 */

Word16 norm_l(Word32 L_var1);           /* Long norm,            30 */


Word16 div_s_asm(Word16 var1, Word16 var2); /* Short division,       18 */
void Log2(
  Word32 L_x,       /* (i) Q0 : input value                                 */
  Word16 *exponent, /* (o) Q0 : Integer part of Log2.   (range: 0<=val<=30) */
  Word16 *fraction  /* (o) Q15: Fractional  part of Log2. (range: 0<=val<1) */
);

Word16   Sqrt( Word32 Num );
unsigned short fixed_sqrt(unsigned long Num);
#endif
