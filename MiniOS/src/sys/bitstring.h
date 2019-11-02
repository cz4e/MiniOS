#ifndef _SYS_BITSTRING_H_
#define _SYS_BITSTRING_H_

typedef unsigned long               bitstr_t;

#define _BITSTR_BITS                (sizeof(bitstr_t) * 8)
#define _bit_roundup2(a,b)          (((a) + ((b) - 1)) & (~((b) - 1)))
#define bitstr_size(nbits)          (_bit_roundup2(nbits,_BITSTR_BITS / 8))

#define bit_decl(name,nbits)        ((name)[bitstr_size(nbits) / sizeof(bitstr_t)])
#define bit_offset(_bit)            ((_bit) % _BITSTR_BITS)
#define bit_mask(_bit)              (1UL << bit_offset((_bit)))
#define bit_idx(_bit)               ((_bit) / _BITSTR_BITS)

#define bit_test(_bitstr,_bit)      \
(((_bitstr)[bit_idx((_bit))] & bit_mask((_bit))) == 0)
/* if setted return 0, if not setted return 1 */
#define bit_set(_bitstr,_bit)       \
((_bitstr)[bit_idx((_bit))] |= bit_mask((_bit)))

#define bit_clear(_bitstr,_bit)     \
((_bitstr)[bit_idx((_bit))] &= ~bit_mask((_bit)))

#endif

