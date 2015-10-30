/*
 * pop_pack_option.hpp
 *
 *  Created on: Oct 30, 2015
 *      Author: expand
 */

#ifdef WIN32
#undef HASHXX_PACKED
#pragma pack(pop)
#endif

#ifdef UNIX
#undef HASHXX_PACKED
#undef HASHXX_ALIGNED
#undef HASHXX_ALIGNED_AND_PACKED
#endif
