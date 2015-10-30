/*
 * alignment_option.hpp
 *
 *  Created on: Oct 30, 2015
 *      Author: expand
 */

#pragma once

#if defined(__i386) || defined(WIN32)
// DCAS operands need to be 8 bytes aligned
#define HASHXX_ALIGNMENT 8
#endif // defined(__i386) || defined(WIN32)

#if defined(__x86_64__) || defined(WIN64)
// DCAS operands need to be 16 bytes aligned
#define HASHXX_ALIGNMENT 16
#endif // defined(__x86_64__) || defined(WIN64)

#ifndef HASHXX_ALIGNMENT
#error "cannot configure memory alignment"
#endif

