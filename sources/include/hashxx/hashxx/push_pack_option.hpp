/*
 * push_pack_option.hpp
 *
 *  Created on: Oct 30, 2015
 *      Author: expand
 */

#ifdef WIN32
#define HASHXX_PACKED
#pragma pack(push,1)
#endif

#ifdef UNIX
#define HASHXX_PACKED __attribute__((packed))
#define HASHXX_ALIGNED(ALIGNMENT) __attribute__((aligned(ALIGNMENT)))
#define HASHXX_ALIGNED_AND_PACKED(ALIGNMENT) __attribute__((aligned(ALIGNMENT), packed))
#endif
