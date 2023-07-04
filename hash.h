/*
 * Using FNV-1a hash algorithm from http://www.isthe.com/chongo/tech/comp/fnv/
 */

#ifndef __HASH_H
#define __HASH_H

#include <stddef.h>
#include <stdint.h>

// Prime number for the hash
#define FNV_PRIME_32 ((1U<<24) + (1<<8) + 0x93)
#define FNV_PRIME_64 ((1UL<<40) + (1<<8) + 0xb3)

/*
Avoid using numbers exceeding max ulong size for as long as possible
#define FNV_PRIME_128  (1<<88 + 1<<8 + 0x3b)
#define FNV_PRIME_256  (1<<168 + 1<<8 + 0x63)
#define FNV_PRIME_512  (1<<344 + 1<<8 + 0x57)
#define FNV_PRIME_1024 (1<<680 + 1<<8 + 0x8d)
*/

// Magic numbers used in FNV-1a hash
#define OFFSET_BASIS_32 2166136261
#define OFFSET_BASIS_64 14695981039346656037

/* 
Avoid using numbers exceeding max ulong size for as long as possible

#define OFFSET_BASIS_128 144066263297769815596495629667062367629
#define OFFSET_BASIS_256 \
    10002925795805258090707096862062570483709279601424119394522528450174147192\
    5557
#define OFFSET_BASIS_512 \
    96593031294966694980094354007163104660904187456726378961083743294344626579\
    94582932197716438449813051892206539805784495328239340083876191928701583869\
    517785
#define OFFSET_BASIS_1024 \
    14197795064947621068722070641403218320880622795441933960878474914617582723\
    25229673230371772215086409652120235554936562817466910857181476047101507614\
    80297559698040773201576924585630032153049571501574036444603635505054127112\
    85966361610267868082893823963790439336411086884584107735010676915
*/

//uint32_t hash32_str(char *str);
//uint64_t hash64_str(char *str);

// Indirection here because there is a good chance we could change the hash
// size at some point later on
typedef uint32_t hash_t;
#define OFFSET_BASIS OFFSET_BASIS_32
#define FNV_PRIME FNV_PRIME_32

hash_t hash_str(char *str, size_t length);

#endif // __HASH_H
