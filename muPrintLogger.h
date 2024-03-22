/*
muPrintLogger - Muukid
Public domain simple C header file that writes print calls to a log file.
No warranty implied; use at your own risk.

Licensed under MIT License or public domain, whichever you prefer.
More explicit license information at the end of the file.
*/

/* muu header commit a87a759 */

#ifndef MUU_H
	#define MUU_H
	
	#if !defined(MU_SECURE_WARNINGS) && !defined(_CRT_SECURE_NO_WARNINGS)
		#define _CRT_SECURE_NO_WARNINGS
	#endif

	#ifdef __cplusplus
	extern "C" { // }
	#endif

	#define MUU_VERSION_MAJOR 1
	#define MUU_VERSION_MINOR 0
	#define MUU_VERSION_PATCH 0

	/* C standard library dependencies */

		#if !defined(size_m)

			#include <stddef.h>

			#ifndef size_m
				#define size_m size_t
			#endif

		#endif

		#if !defined(MU_SIZE_MAX)

			#include <stdint.h>

			#ifndef MU_SIZE_MAX
				#define MU_SIZE_MAX SIZE_MAX
			#endif

		#endif

		#if !defined(muBool)   || \
			!defined(MU_TRUE)  || \
			!defined(MU_FALSE)

			#include <stdbool.h>

			#ifndef muBool
				#define muBool bool
			#endif

			#ifndef MU_TRUE
				#define MU_TRUE true
			#endif

			#ifndef MU_FALSE
				#define MU_FALSE false
			#endif

		#endif

	/* Useful macros */

		#ifndef MUDEF
			#ifdef MU_STATIC
				#define MUDEF static
			#else
				#define MUDEF extern
			#endif
		#endif

		#ifndef MU_ZERO_STRUCT
			#ifdef __cplusplus
				#define MU_ZERO_STRUCT(s) {}
			#else
				#define MU_ZERO_STRUCT(s) (s){0}
			#endif
		#endif

		#ifndef MU_ZERO_STRUCT_CONST
			#ifdef __cplusplus
				#define MU_ZERO_STRUCT_CONST(s) {}
			#else
				#define MU_ZERO_STRUCT_CONST(s) {0}
			#endif
		#endif

		#ifndef MU_NULL_PTR
			#define MU_NULL_PTR 0
		#endif

		#ifndef MU_NULL
			#define MU_NULL 0
		#endif

		#ifndef MU_NONE
			#define MU_NONE MU_SIZE_MAX
		#endif

		#ifndef MU_SET_RESULT
			#define MU_SET_RESULT(res, val) if(res!=MU_NULL_PTR){*res=val;}
		#endif

		#ifndef MU_ASSERT
			#define MU_ASSERT(cond, res, val, after) if(!(cond)){MU_SET_RESULT(res, val) after}
		#endif

		#define MU_ENUM(name, ...) enum _##name{__VA_ARGS__};typedef enum _##name _##name; typedef size_m name;

		#if !defined(MU_WIN32) && !defined(MU_UNIX)
			#if defined(WIN32) || defined(_WIN32)
				#define MU_WIN32
			#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
				#define MU_UNIX
			#endif
		#endif

		#define MU_HRARRAY_DEFAULT_FUNC(name) \
			muBool name##_comp(name t0, name t1) { \
				return t0.active == t1.active; \
			} \
			\
			void name##_on_creation(name* p) { \
				if (p != MU_NULL_PTR) { \
					MU_LOCK_CREATE(p->lock, p->lock_active) \
				} \
			} \
			void name##_on_destruction(name* p) { \
				if (p != MU_NULL_PTR) { \
					MU_LOCK_DESTROY(p->lock, p->lock_active) \
				} \
			} \
			void name##_on_hold(name* p) { \
				if (p != MU_NULL_PTR) { \
					MU_LOCK_LOCK(p->lock, p->lock_active) \
				} \
			} \
			void name##_on_release(name* p) { \
				if (p != MU_NULL_PTR) { \
					MU_LOCK_UNLOCK(p->lock, p->lock_active) \
				} \
			} \
			\
			mu_dynamic_hrarray_declaration( \
				name##_array, name, name##_, name##_comp, \
				name##_on_creation, name##_on_destruction, name##_on_hold, name##_on_release \
			)

		#define MU_SAFEFUNC(result, lib_prefix, context, fail_return) \
			MU_SET_RESULT(result, lib_prefix##SUCCESS) \
			MU_ASSERT(context != MU_NULL_PTR, result, lib_prefix##NOT_YET_INITIALIZED, fail_return) \

		#define MU_HOLD(result, item, da, context, lib_prefix, fail_return, da_prefix) \
			MU_ASSERT(item < da.length, result, lib_prefix##INVALID_ID, fail_return) \
			da_prefix##hold_element(0, &da, item); \
			MU_ASSERT(da.data[item].active, result, lib_prefix##INVALID_ID, da_prefix##release_element(0, &da, item); fail_return)

		#define MU_RELEASE(da, item, da_prefix) \
			da_prefix##release_element(0, &da, item);

	#ifdef __cplusplus
	}
	#endif

#endif /* MUU_H */

/* muma header (commit c0925a3) */

#ifndef MUMA_H
	#define MUMA_H
	
	#ifdef __cplusplus
		extern "C" {
	#endif

	#define MUMA_VERSION_MAJOR 1
	#define MUMA_VERSION_MINOR 0
	#define MUMA_VERSION_PATCH 0

	/* C standard library dependencies */

		#if !defined(mu_malloc)  || \
			!defined(mu_free)    || \
			!defined(mu_realloc)

			#include <stdlib.h>

			#ifndef mu_malloc
				#define mu_malloc malloc
			#endif

			#ifndef mu_free
				#define mu_free free
			#endif

			#ifndef mu_realloc
				#define mu_realloc realloc
			#endif

		#endif

		#if !defined(mu_memset) || \
			!defined(mu_memcpy)

			#include <string.h>

			#ifndef mu_memset
				#define mu_memset memset
			#endif

			#ifndef mu_memcpy
				#define mu_memcpy memcpy
			#endif

		#endif

	/* Enums */

		MU_ENUM(mumaResult, 
			MUMA_SUCCESS,

			MUMA_FAILED_TO_ALLOCATE,
			MUMA_INVALID_TYPE_SIZE,
			MUMA_INVALID_INDEX,
			MUMA_INVALID_SHIFT_AMOUNT,
			MUMA_INVALID_COUNT,
			MUMA_NOT_FOUND
		)

	/* Functions */

		#ifdef MUMA_NAMES
			MUDEF const char* muma_result_get_name(mumaResult result);
		#endif

	/* API macro functionality */

		#define mu_dynamic_array_declaration(struct_name, type, function_name_prefix, type_comparison_func) \
			\
			struct struct_name { \
				type* data; \
				size_m allocated_length; \
				size_m length; \
			}; typedef struct struct_name struct_name; \
			\
			struct_name function_name_prefix##create(mumaResult* result, size_m length) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				struct_name s = MU_ZERO_STRUCT(struct_name); \
				s.data = MU_NULL_PTR; \
				s.allocated_length = length; \
				s.length = length; \
				\
				if (s.length == 0) { \
					return s; \
				} \
				\
				s.data = (type*)mu_malloc(sizeof(type)*s.allocated_length); \
				if (s.data == 0) { \
					MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
					s.allocated_length = 0; \
					s.length = 0; \
					return s; \
				} \
				\
				mu_memset(s.data, 0, sizeof(type)*s.allocated_length);\
				return s;\
			} \
			\
			struct_name function_name_prefix##destroy(mumaResult* result, struct_name s) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (s.data != MU_NULL_PTR) { \
					mu_free(s.data); \
					s.data = MU_NULL_PTR; \
				} \
				\
				s.allocated_length = 0; \
				s.length = 0; \
				return s; \
			} \
			\
			struct_name function_name_prefix##resize(mumaResult* result, struct_name s, size_m length) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (s.length == length) { \
					return s; \
				} \
				\
				if (s.data == MU_NULL_PTR) { \
					if (length == 0) { \
						return s; \
					} \
					\
					mumaResult res = MUMA_SUCCESS; \
					s = function_name_prefix##create(&res, length); \
					if (res != MUMA_SUCCESS) { \
						MU_SET_RESULT(result, res) \
						return s; \
					} \
				} \
				\
				if (length == 0) { \
					s.length = 0; \
					mu_memset(s.data, 0, sizeof(type)*s.allocated_length); \
					return s; \
				} \
				\
				size_m old_length = s.length; \
				size_m old_allocated_length = s.allocated_length; \
				s.length = length; \
				\
				if ((s.length > s.allocated_length) || (s.length < s.allocated_length/2)) { \
					while (s.length > s.allocated_length) { \
						s.allocated_length *= 2; \
					} \
					while (s.length < s.allocated_length/2) { \
						s.allocated_length /= 2; \
					} \
					\
					type* new_data = (type*)mu_realloc(s.data, sizeof(type) * s.allocated_length); \
					if (new_data == 0) { \
						s.length = old_length; \
						s.allocated_length = old_allocated_length; \
						MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
						return s; \
					} \
					\
					s.data = new_data; \
				} \
				\
				if (old_length < s.length) { \
					mu_memset(&s.data[old_length], 0, sizeof(type)*(s.allocated_length-old_length)); \
				} \
				\
				return s; \
			} \
			\
			struct_name function_name_prefix##lshift(mumaResult* result, struct_name s, size_m index, size_m amount) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (index >= s.length) { \
					MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
					return s; \
				} \
				\
				if (amount == 0) { \
					return s; \
				} \
				\
				if (amount > index) { \
					MU_SET_RESULT(result, MUMA_INVALID_SHIFT_AMOUNT) \
					return s; \
				} \
				\
				mu_memcpy(&s.data[index-amount], &s.data[index], sizeof(type)*(s.length-index)); \
				\
				mumaResult res = MUMA_SUCCESS; \
				s = function_name_prefix##resize(&res, s, s.length-amount); \
				if (res != MUMA_SUCCESS) { \
					MU_SET_RESULT(result, res) \
					return s; \
				} \
				\
				return s; \
			} \
			\
			struct_name function_name_prefix##rshift(mumaResult* result, struct_name s, size_m index, size_m amount) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (index >= s.length) { \
					MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
					return s; \
				} \
				\
				if (amount == 0) { \
					return s; \
				} \
				\
				mumaResult res = MUMA_SUCCESS; \
				s = function_name_prefix##resize(&res, s, s.length+amount); \
				if (res != MUMA_SUCCESS) { \
					MU_SET_RESULT(result, res) \
					return s; \
				} \
				\
				mu_memcpy(&s.data[index+amount], &s.data[index], sizeof(type)*(s.length-index)); \
				mu_memset(&s.data[index], 0, sizeof(type)*(amount)); \
				\
				return s; \
			} \
			\
			struct_name function_name_prefix##multiinsert(mumaResult* result, struct_name s, size_m index, type* insert, size_m count) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				mumaResult res = MUMA_SUCCESS; \
				s = function_name_prefix##rshift(&res, s, index, count); \
				if (res != MUMA_SUCCESS) { \
					MU_SET_RESULT(result, res) \
					return s; \
				} \
				\
				mu_memcpy(&s.data[index], insert, sizeof(type)*count); \
				return s; \
			} \
			\
			struct_name function_name_prefix##insert(mumaResult* result, struct_name s, size_m index, type insert) { \
				return function_name_prefix##multiinsert(result, s, index, &insert, 1); \
			} \
			\
			struct_name function_name_prefix##multierase(mumaResult* result, struct_name s, size_m index, size_m count) { \
				return function_name_prefix##lshift(result, s, index+count, count); \
			} \
			\
			struct_name function_name_prefix##erase(mumaResult* result, struct_name s, size_m index) { \
				return function_name_prefix##multierase(result, s, index, 1); \
			} \
			struct_name function_name_prefix##clear(mumaResult* result, struct_name s) { \
				return function_name_prefix##destroy(result, s); \
			} \
			\
			struct_name function_name_prefix##multipush(mumaResult* result, struct_name s, type* push, size_m count) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				mumaResult res = MUMA_SUCCESS; \
				s = function_name_prefix##resize(&res, s, s.length+count); \
				if (res != MUMA_SUCCESS) { \
					MU_SET_RESULT(result, res) \
					return s; \
				} \
				\
				mu_memcpy(&s.data[s.length-count], push, sizeof(type)*count); \
				return s; \
			} \
			\
			struct_name function_name_prefix##push(mumaResult* result, struct_name s, type push) { \
				return function_name_prefix##multipush(result, s, &push, 1); \
			} \
			\
			struct_name function_name_prefix##multipop(mumaResult* result, struct_name s, size_m count) { \
				return function_name_prefix##resize(result, s, s.length-count); \
			} \
			\
			struct_name function_name_prefix##pop(mumaResult* result, struct_name s) { \
				return function_name_prefix##multipop(result, s, 1); \
			} \
			\
			size_m function_name_prefix##find(mumaResult* result, struct_name s, type find) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				for (size_m i = 0; i < s.length; i++) { \
					if (type_comparison_func(find, s.data[i])) { \
						return i; \
					} \
				} \
				\
				MU_SET_RESULT(result, MUMA_NOT_FOUND) \
				return MU_NONE; \
			} \
			\
			struct_name function_name_prefix##find_push(mumaResult* result, struct_name s, type find, size_m* p_index) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				mumaResult res = MUMA_SUCCESS; \
				size_m index = function_name_prefix##find(&res, s, find); \
				if (index != MU_NONE) { \
					if (p_index != MU_NULL_PTR) { \
						*p_index = index; \
					} \
					return s; \
				} \
				if (res != MUMA_SUCCESS && res != MUMA_NOT_FOUND) { \
					if (p_index != MU_NULL_PTR) { \
						*p_index = MU_NONE; \
					} \
					MU_SET_RESULT(result, res) \
					return s; \
				} \
				\
				s = function_name_prefix##push(&res, s, find); \
				if (res != MUMA_SUCCESS) { \
					if (p_index != MU_NULL_PTR) { \
						*p_index = MU_NONE; \
					} \
					MU_SET_RESULT(result, res) \
					return s; \
				} \
				\
				if (p_index != MU_NULL_PTR) { \
					*p_index = s.length-1; \
				} \
				return s; \
			}

		#define mu_dynamic_hrarray_declaration( \
			struct_name, type, function_name_prefix, type_comparison_func, \
			on_creation, on_destruction, on_hold, on_release \
		) \
			\
			struct struct_name { \
				type* data; \
				size_m allocated_length; \
				size_m length; \
			}; typedef struct struct_name struct_name; \
			\
			struct_name function_name_prefix##create(mumaResult* result, size_m length) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				struct_name s = MU_ZERO_STRUCT(struct_name); \
				s.data = MU_NULL_PTR; \
				s.allocated_length = length; \
				s.length = length; \
				\
				if (s.length == 0) { \
					return s; \
				} \
				\
				s.data = (type*)mu_malloc(sizeof(type)*s.allocated_length); \
				if (s.data == 0) { \
					MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
					s.allocated_length = 0; \
					s.length = 0; \
					return s; \
				} \
				\
				mu_memset(s.data, 0, sizeof(type)*s.allocated_length); \
				\
				for (size_m i = 0; i < s.length; i++) { \
					on_creation(&s.data[i]); \
				} \
				return s;\
			} \
			\
			void function_name_prefix##hold_element(mumaResult* result, struct_name* s, size_m index) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (index >= s->length) { \
					MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
					return; \
				} \
				\
				on_hold(&s->data[index]); \
			} \
			\
			void function_name_prefix##release_element(mumaResult* result, struct_name* s, size_m index) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (index >= s->length) { \
					MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
					return; \
				} \
				\
				on_release(&s->data[index]); \
			} \
			\
			void function_name_prefix##destroy(mumaResult* result, struct_name* s) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				for (size_m i = 0; i < s->length; i++) { \
					function_name_prefix##hold_element(0, s, i); \
				} \
				\
				for (size_m i = 0; i < s->length; i++) { \
					on_destruction(&s->data[i]); \
				} \
				\
				if (s->data != MU_NULL_PTR) { \
					mu_free(s->data); \
					s->data = MU_NULL_PTR; \
				} \
				\
				s->allocated_length = 0; \
				s->length = 0; \
			} \
			\
			void function_name_prefix##inner_resize(mumaResult* result, struct_name* s, size_m length, muBool cd) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (s->length == length) { \
					return; \
				} \
				\
				if (s->data == MU_NULL_PTR) { \
					if (length == 0) { \
						return; \
					} \
					\
					mumaResult res = MUMA_SUCCESS; \
					*s = function_name_prefix##create(&res, length); \
					if (res != MUMA_SUCCESS) { \
						MU_SET_RESULT(result, res) \
						return; \
					} \
				} \
				\
				if (length == 0) { \
					for (size_m i = 0; i < s->length; i++) { \
						on_destruction(&s->data[i]); \
					} \
					s->length = 0; \
					mu_memset(s->data, 0, sizeof(type)*s->allocated_length); \
					return; \
				} \
				\
				if (cd) { \
					for (size_m i = 0; i < s->length; i++) { \
						function_name_prefix##hold_element(0, s, i); \
					} \
				} \
				\
				size_m old_length = s->length; \
				size_m old_allocated_length = s->allocated_length; \
				s->length = length; \
				\
				/* Note: this is really dangerous, because it's not guaranteed that the  */ \
				/* reallocation will follow through. If it doesn't, we've now called the */ \
				/* destroy function on a bunch of elements that still exist. I can't     */ \
				/* really think of a better way of doing it than this right now, though. */ \
				if (cd && old_length > s->length) { \
					for (size_m i = s->length; i < old_length; i++) { \
						on_destruction(&s->data[i]); \
					} \
				} \
				\
				if ((s->length > s->allocated_length) || (s->length < s->allocated_length/2)) { \
					while (s->length > s->allocated_length) { \
						s->allocated_length *= 2; \
					} \
					while (s->length < s->allocated_length/2) { \
						s->allocated_length /= 2; \
					} \
					\
					type* new_data = (type*)mu_realloc(s->data, sizeof(type) * s->allocated_length); \
					if (new_data == 0) { \
						s->length = old_length; \
						s->allocated_length = old_allocated_length; \
						if (cd) { \
							for (size_m i = 0; i < s->length; i++) { \
								function_name_prefix##release_element(0, s, i); \
							} \
						} \
						MU_SET_RESULT(result, MUMA_FAILED_TO_ALLOCATE) \
						return; \
					} \
					\
					s->data = new_data; \
				} \
				\
				if (old_length < s->length) { \
					mu_memset(&s->data[old_length], 0, sizeof(type)*(s->allocated_length-old_length)); \
					\
					if (cd) { \
						for (size_m i = old_length; i < s->length; i++) { \
							on_creation(&s->data[i]); \
						} \
					} \
				} \
				\
				if (cd) { \
					for (size_m i = 0; i < s->length && i < old_length; i++) { \
						function_name_prefix##release_element(0, s, i); \
					} \
				} \
			} \
			\
			void function_name_prefix##resize(mumaResult* result, struct_name* s, size_m length) { \
				function_name_prefix##inner_resize(result, s, length, MU_TRUE); \
			} \
			\
			void function_name_prefix##inner_lshift(mumaResult* result, struct_name* s, size_m index, size_m amount, muBool cd) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (index >= s->length) { \
					MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
					return; \
				} \
				\
				if (amount == 0) { \
					return; \
				} \
				\
				if (amount > index) { \
					MU_SET_RESULT(result, MUMA_INVALID_SHIFT_AMOUNT) \
					return; \
				} \
				\
				if (cd) { \
					for (size_m i = 0; i < s->length; i++) { \
						function_name_prefix##hold_element(0, s, i); \
					} \
				} \
				\
				/* Dangerous; resize may fail after this */ \
				if (cd) { \
					for (size_m i = index-amount; i < index; i++) { \
						on_destruction(&s->data[i]); \
					} \
				} \
				\
				mu_memcpy(&s->data[index-amount], &s->data[index], sizeof(type)*(s->length-index)); \
				\
				mumaResult res = MUMA_SUCCESS; \
				function_name_prefix##inner_resize(&res, s, s->length-amount, MU_FALSE); \
				\
				if (cd) { \
					for (size_m i = 0; i < s->length; i++) { \
						function_name_prefix##release_element(0, s, i); \
					} \
				} \
				\
				if (res != MUMA_SUCCESS) { \
					MU_SET_RESULT(result, res) \
					return; \
				} \
			} \
			\
			void function_name_prefix##lshift(mumaResult* result, struct_name* s, size_m index, size_m amount) { \
				function_name_prefix##inner_lshift(result, s, index, amount, MU_TRUE); \
			} \
			\
			void function_name_prefix##inner_rshift(mumaResult* result, struct_name* s, size_m index, size_m amount, muBool cd) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				if (index >= s->length) { \
					MU_SET_RESULT(result, MUMA_INVALID_INDEX) \
					return; \
				} \
				\
				if (amount == 0) { \
					return; \
				} \
				\
				if (cd) { \
					for (size_m i = 0; i < s->length; i++) { \
						function_name_prefix##hold_element(0, s, i); \
					} \
				} \
				\
				mumaResult res = MUMA_SUCCESS; \
				function_name_prefix##inner_resize(&res, s, s->length+amount, MU_FALSE); \
				if (res != MUMA_SUCCESS) { \
					if (cd) { \
						for (size_m i = 0; i < s->length; i++) { \
							function_name_prefix##release_element(0, s, i); \
						} \
					} \
					MU_SET_RESULT(result, res) \
					return; \
				} \
				\
				mu_memcpy(&s->data[index+amount], &s->data[index], sizeof(type)*(s->length-index)); \
				\
				if (cd) { \
					for (size_m i = 0; i < index; i++) { \
						function_name_prefix##release_element(0, s, i); \
					} \
					for (size_m i = index+amount; i < s->length; i++) { \
						function_name_prefix##release_element(0, s, i); \
					} \
				} \
				\
				mu_memset(&s->data[index], 0, sizeof(type)*(amount)); \
				\
				if (cd) { \
					for (size_m i = index; i < index+amount; i++) { \
						on_creation(&s->data[i]); \
					} \
				} \
			} \
			\
			void function_name_prefix##rshift(mumaResult* result, struct_name* s, size_m index, size_m amount) { \
				function_name_prefix##inner_rshift(result, s, index, amount, MU_TRUE); \
			} \
			\
			void function_name_prefix##multiinsert(mumaResult* result, struct_name* s, size_m index, type* insert, size_m count) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				for (size_m i = 0; i < s->length; i++) { \
					function_name_prefix##hold_element(0, s, i); \
				} \
				\
				mumaResult res = MUMA_SUCCESS; \
				function_name_prefix##inner_rshift(&res, s, index, count, MU_FALSE); \
				if (res != MUMA_SUCCESS) { \
					for (size_m i = 0; i < s->length; i++) { \
						function_name_prefix##release_element(0, s, i); \
					} \
					MU_SET_RESULT(result, res) \
					return; \
				} \
				\
				mu_memcpy(&s->data[index], insert, sizeof(type)*count); \
				\
				for (size_m i = 0; i < index; i++) { \
					function_name_prefix##release_element(0, s, i); \
				} \
				for (size_m i = index+count; i < s->length; i++) { \
					function_name_prefix##release_element(0, s, i); \
				} \
				\
				for (size_m i = index; i < index+count; i++) { \
					on_creation(&s->data[i]); \
				} \
			} \
			\
			void function_name_prefix##insert(mumaResult* result, struct_name* s, size_m index, type insert) { \
				function_name_prefix##multiinsert(result, s, index, &insert, 1); \
			} \
			\
			void function_name_prefix##multierase(mumaResult* result, struct_name* s, size_m index, size_m count) { \
				function_name_prefix##lshift(result, s, index+count, count); \
			} \
			\
			void function_name_prefix##erase(mumaResult* result, struct_name* s, size_m index) { \
				function_name_prefix##multierase(result, s, index, 1); \
			} \
			void function_name_prefix##clear(mumaResult* result, struct_name* s) { \
				function_name_prefix##destroy(result, s); \
			} \
			\
			void function_name_prefix##multipush(mumaResult* result, struct_name* s, type* push, size_m count) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				size_m old_length = s->length; \
				for (size_m i = 0; i < s->length; i++) { \
					function_name_prefix##hold_element(0, s, i); \
				} \
				\
				mumaResult res = MUMA_SUCCESS; \
				function_name_prefix##inner_resize(&res, s, s->length+count, MU_FALSE); \
				if (res != MUMA_SUCCESS) { \
					for (size_m i = 0; i < s->length; i++) { \
						function_name_prefix##release_element(0, s, i); \
					} \
					MU_SET_RESULT(result, res) \
					return; \
				} \
				\
				mu_memcpy(&s->data[s->length-count], push, sizeof(type)*count); \
				\
				for (size_m i = 0; i < old_length; i++) { \
					function_name_prefix##release_element(0, s, i); \
				} \
				\
				for (size_m i = s->length-count; i < s->length; i++) { \
					on_creation(&s->data[i]); \
				} \
			} \
			\
			void function_name_prefix##push(mumaResult* result, struct_name* s, type push) { \
				function_name_prefix##multipush(result, s, &push, 1); \
			} \
			\
			void function_name_prefix##multipop(mumaResult* result, struct_name* s, size_m count) { \
				function_name_prefix##resize(result, s, s->length-count); \
			} \
			\
			void function_name_prefix##pop(mumaResult* result, struct_name* s) { \
				function_name_prefix##multipop(result, s, 1); \
			} \
			\
			size_m function_name_prefix##find(mumaResult* result, struct_name* s, type find) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				for (size_m i = 0; i < s->length; i++) { \
					function_name_prefix##hold_element(0, s, i); \
					if (type_comparison_func(find, s->data[i])) { \
						function_name_prefix##release_element(0, s, i); \
						return i; \
					} \
					function_name_prefix##release_element(0, s, i); \
				} \
				\
				MU_SET_RESULT(result, MUMA_NOT_FOUND) \
				return MU_NONE; \
			} \
			\
			void function_name_prefix##find_push(mumaResult* result, struct_name* s, type find, size_m* p_index) { \
				MU_SET_RESULT(result, MUMA_SUCCESS) \
				\
				mumaResult res = MUMA_SUCCESS; \
				size_m index = function_name_prefix##find(&res, s, find); \
				if (index != MU_NONE) { \
					if (p_index != MU_NULL_PTR) { \
						*p_index = index; \
					} \
					return; \
				} \
				if (res != MUMA_SUCCESS && res != MUMA_NOT_FOUND) { \
					if (p_index != MU_NULL_PTR) { \
						*p_index = MU_NONE; \
					} \
					MU_SET_RESULT(result, res) \
					return; \
				} \
				\
				function_name_prefix##push(&res, s, find); \
				if (res != MUMA_SUCCESS) { \
					if (p_index != MU_NULL_PTR) { \
						*p_index = MU_NONE; \
					} \
					MU_SET_RESULT(result, res) \
					return; \
				} \
				\
				if (p_index != MU_NULL_PTR) { \
					*p_index = s->length-1; \
				} \
			}

	#ifdef __cplusplus
		}
	#endif

#endif /* MUMA_H */

#ifdef MU_THREADSAFE

	/* mum header commit acf8f90 */

	#ifndef MUM_H
		#define MUM_H
		
		#ifdef __cplusplus
		extern "C" { // }
		#endif

		#define MUM_VERSION_MAJOR 1
		#define MUM_VERSION_MINOR 0
		#define MUM_VERSION_PATCH 0

		/* Enums */

			MU_ENUM(mumResult,
				MUM_SUCCESS,

				MUM_MUMA_FAILED_TO_ALLOCATE,
				MUM_MUMA_INVALID_TYPE_SIZE,
				MUM_MUMA_INVALID_INDEX,
				MUM_MUMA_INVALID_SHIFT_AMOUNT,
				MUM_MUMA_INVALID_COUNT,
				MUM_MUMA_NOT_FOUND,

				MUM_ALLOCATION_FAILED,

				MUM_ALREADY_INITIALIZED,
				MUM_ALREADY_TERMINATED,
				MUM_NOT_YET_INITIALIZED,

				MUM_CREATE_CALL_FAILED,
				MUM_DESTROY_CALL_FAILED,
				MUM_WAIT_CALL_FAILED,
				MUM_LOCK_CALL_FAILED,
				MUM_UNLOCK_CALL_FAILED,
				MUM_GET_RETURN_VALUE_CALL_FAILED,

				MUM_INVALID_ID,

				MUM_THREAD_TIMED_OUT,
				MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK
			)

		/* Macros */

			#define muThread size_m
			#define muMutex size_m
			#define muSpinlock size_m

			#ifdef MU_SPINLOCK
				#define muLock muSpinlock
				#define mu_lock_create mu_spinlock_create
				#define mu_lock_destroy mu_spinlock_destroy
				#define mu_lock_lock mu_spinlock_lock
				#define mu_lock_unlock mu_spinlock_unlock
			#else
				#define muLock muMutex
				#define mu_lock_create mu_mutex_create
				#define mu_lock_destroy mu_mutex_destroy
				#define mu_lock_lock mu_mutex_lock
				#define mu_lock_unlock mu_mutex_unlock
			#endif

		/* Incomplete types */

			typedef struct mumContext mumContext;

		/* Global variables */

			MUDEF mumContext* mum_global_context;

		/* Functions */

			/* Names */

				#ifdef MUM_NAMES
					MUDEF const char* mum_result_get_name(mumResult result);
				#endif

			/* Initiation/Termination */

				MUDEF void mum_init(mumResult* result);
				MUDEF void mum_term(mumResult* result);

			/* Thread */

				MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args);
				MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread);

				MUDEF void mu_thread_exit(void* ret);
				MUDEF void mu_thread_wait(mumResult* result, muThread thread);
				MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread);

			/* Mutex */

				MUDEF muMutex mu_mutex_create(mumResult* result);
				MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex);

				MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex);
				MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex);

			/* Spinlock */

				MUDEF muSpinlock mu_spinlock_create(mumResult* result);
				MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock);

				MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock);
				MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock);

		#ifdef __cplusplus
		}
		#endif

	#endif

#endif /* MU_THREADSAFE */

#ifndef MUPL_H
	#define MUPL_H
	
	#ifdef __cplusplus
		extern "C" {
	#endif

	#define MUPL_VERSION_MAJOR 1
	#define MUPL_VERSION_MINOR 0
	#define MUPL_VERSION_PATCH 1

	/* C standard library dependencies */

		#if !defined(MU_FILE)       || \
			!defined(mu_fopen)      || \
			!defined(mu_fclose)     || \
			!defined(mu_fprintf)    || \
			!defined(mu_std_printf) || \
			!defined(mu_vsprintf)   || \
			!defined(mu_vsnprintf)

			#include <stdio.h>

			#ifndef MU_FILE
				#define MU_FILE FILE
			#endif

			#ifndef mu_fopen
				#define mu_fopen fopen
			#endif

			#ifndef mu_fclose
				#define mu_fclose fclose
			#endif

			#ifndef mu_fprintf
				#define mu_fprintf fprintf
			#endif

			#ifndef mu_std_printf
				#define mu_std_printf printf
			#endif

			#ifndef mu_vsprintf
				#define mu_vsprintf vsprintf
			#endif

			#ifndef mu_vsnprintf
				#define mu_vsnprintf vsnprintf
			#endif

		#endif

		#if !defined(mu_va_list)  || \
			!defined(mu_va_start) || \
			!defined(mu_va_end)

			#include <stdarg.h>

			#ifndef mu_va_list
				#define mu_va_list va_list
			#endif

			#ifndef mu_va_start
				#define mu_va_start va_start
			#endif

			#ifndef mu_va_end
				#define mu_va_end va_end
			#endif

		#endif

		#if !defined(mu_strlen)

			#include <string.h>

			#ifndef mu_strlen
				#define mu_strlen strlen
			#endif

		#endif

	/* Incomplete types */

		typedef struct muplContext muplContext;

	/* Global variables */

		MUDEF muplContext* mupl_global_context;

	/* Macros */

		#define muLog size_m

	/* Enums */

		MU_ENUM(muplResult,
			MUPL_SUCCESS,

			MUPL_MUMA_SUCCESS,
			MUPL_MUMA_FAILED_TO_ALLOCATE,
			MUPL_MUMA_INVALID_TYPE_SIZE,
			MUPL_MUMA_INVALID_INDEX,
			MUPL_MUMA_INVALID_SHIFT_AMOUNT,
			MUPL_MUMA_INVALID_COUNT,
			MUPL_MUMA_NOT_FOUND,

			MUPL_ALREADY_INITIALIZED,
			MUPL_ALREADY_TERMINATED,
			MUPL_NOT_YET_INITIALIZED,

			MUPL_ALLOCATION_FAILED,

			MUPL_FAILED_TO_OPEN_FILE,

			MUPL_INVALID_ID
		)

	/* Functions */

		/* Names */

			#ifdef MUPL_NAMES
				MUDEF const char* mupl_result_get_name(muplResult result);
			#endif

		/* Initiation / Termination */

			MUDEF void mupl_init(muplResult* result);
			MUDEF void mupl_term(muplResult* result);

		/* Log */

			MUDEF muLog mu_log_create(muplResult* result, char* filename, muBool clear);
			MUDEF muLog mu_log_destroy(muplResult* result, muLog mlog);

			MUDEF void mu_printf(muplResult* result, muLog mlog, const char* p, ...);
			MUDEF void mu_log(muplResult* result, muLog mlog, const char* p, ...);

	#ifdef __cplusplus
		}
	#endif
#endif


#ifdef MUPL_IMPLEMENTATION

	/* muma implementation (commit 4683cd1) */

	#define MUMA_IMPLEMENTATION

	#ifdef MUMA_IMPLEMENTATION

		#ifdef __cplusplus
			extern "C" {
		#endif

		#ifdef MUMA_NAMES
			MUDEF const char* muma_result_get_name(mumaResult result) {
				switch (result) {
					default: return "MUMA_UNKNOWN"; break;
					case MUMA_SUCCESS: return "MUMA_SUCCESS"; break;
					case MUMA_FAILED_TO_ALLOCATE: return "MUMA_FAILED_TO_ALLOCATE"; break;
					case MUMA_INVALID_TYPE_SIZE: return "MUMA_INVALID_TYPE_SIZE"; break;
					case MUMA_INVALID_INDEX: return "MUMA_INVALID_INDEX"; break;
					case MUMA_INVALID_SHIFT_AMOUNT: return "MUMA_INVALID_SHIFT_AMOUNT"; break;
					case MUMA_INVALID_COUNT: return "MUMA_INVALID_COUNT"; break;
				}
			}
		#endif

		#ifdef __cplusplus
			}
		#endif

	#endif /* MUMA_IMPLEMENTATION */

	#ifdef MU_THREADSAFE

		/* mum implementation commit acf8f90 */

		#define MUM_IMPLEMENTATION

		#ifdef MUM_IMPLEMENTATION

			#ifdef __cplusplus
			extern "C" { // }
			#endif

			/* Shared functions */

				/* Names */

					#ifdef MUM_NAMES
						MUDEF const char* mum_result_get_name(mumResult result) {
							switch (result) {
								default: return "MUM_UNKNOWN"; break;
								case MUM_SUCCESS: return "MUM_SUCCESS"; break;
								case MUM_MUMA_FAILED_TO_ALLOCATE: return "MUM_MUMA_FAILED_TO_ALLOCATE"; break;
								case MUM_MUMA_INVALID_TYPE_SIZE: return "MUM_MUMA_INVALID_TYPE_SIZE"; break;
								case MUM_MUMA_INVALID_INDEX: return "MUM_MUMA_INVALID_INDEX"; break;
								case MUM_MUMA_INVALID_SHIFT_AMOUNT: return "MUM_MUMA_INVALID_SHIFT_AMOUNT"; break;
								case MUM_MUMA_INVALID_COUNT: return "MUM_MUMA_INVALID_COUNT"; break;
								case MUM_MUMA_NOT_FOUND: return "MUM_MUMA_NOT_FOUND"; break;
								case MUM_ALLOCATION_FAILED: return "MUM_ALLOCATION_FAILED"; break;
								case MUM_ALREADY_INITIALIZED: return "MUM_ALREADY_INITIALIZED"; break;
								case MUM_ALREADY_TERMINATED: return "MUM_ALREADY_TERMINATED"; break;
								case MUM_NOT_YET_INITIALIZED: return "MUM_NOT_YET_INITIALIZED"; break;
								case MUM_CREATE_CALL_FAILED: return "MUM_CREATE_CALL_FAILED"; break;
								case MUM_DESTROY_CALL_FAILED: return "MUM_DESTROY_CALL_FAILED"; break;
								case MUM_WAIT_CALL_FAILED: return "MUM_WAIT_CALL_FAILED"; break;
								case MUM_LOCK_CALL_FAILED: return "MUM_LOCK_CALL_FAILED"; break;
								case MUM_UNLOCK_CALL_FAILED: return "MUM_UNLOCK_CALL_FAILED"; break;
								case MUM_GET_RETURN_VALUE_CALL_FAILED: return "MUM_GET_RETURN_VALUE_CALL_FAILED"; break;
								case MUM_INVALID_ID: return "MUM_INVALID_ID"; break;
								case MUM_THREAD_TIMED_OUT: return "MUM_THREAD_TIMED_OUT"; break;
								case MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK: return "MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK"; break;
							}
						}
					#endif

				/* Get result */

					mumResult muma_result_to_mum_result(mumaResult res) {
						switch (res) {
							default: case MUMA_SUCCESS: return MUM_SUCCESS; break;
							case MUMA_FAILED_TO_ALLOCATE: return MUM_MUMA_FAILED_TO_ALLOCATE; break;
							case MUMA_INVALID_TYPE_SIZE: return MUM_MUMA_INVALID_TYPE_SIZE; break;
							case MUMA_INVALID_INDEX: return MUM_MUMA_INVALID_INDEX; break;
							case MUMA_INVALID_SHIFT_AMOUNT: return MUM_MUMA_INVALID_SHIFT_AMOUNT; break;
							case MUMA_INVALID_COUNT: return MUM_MUMA_INVALID_COUNT; break;
							case MUMA_NOT_FOUND: return MUM_MUMA_NOT_FOUND; break;
						}
					}

			/* Unix */

			#ifdef MU_UNIX

				#include <pthread.h>

				/* Useful things */

					// These two functions are essentially stolen from:
					// https://github.com/stepancheg/no-mutex-c

					static inline muBool mum_atomic_compare_exchange(int* ptr, int compare, int exchange) {
						return __atomic_compare_exchange_n(ptr, &compare, exchange, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
					}

					static inline void mum_atomic_store(int* ptr, int value) {
						if (value) {}
						__atomic_store_n(ptr, 0, __ATOMIC_SEQ_CST);
					}

					#ifdef MU_THREADSAFE

						#define MU_LOCK pthread_mutex_t

						#define MU_LOCK_CREATE(lock, lock_active) \
							if (pthread_mutex_init(&lock, 0) == 0) { \
								lock_active = MU_TRUE; \
							} else { \
								lock_active = MU_FALSE; \
							}

						#define MU_LOCK_DESTROY(lock, lock_active) \
							if (lock_active) { \
								if (pthread_mutex_destroy(&lock) == 0) { \
									lock_active = MU_FALSE; \
								} \
							}

						#define MU_LOCK_LOCK(lock, lock_active) \
							if (lock_active) { \
								pthread_mutex_lock(&lock); \
							}

						#define MU_LOCK_UNLOCK(lock, lock_active) \
							if (lock_active) { \
								pthread_mutex_unlock(&lock); \
							}

					#else

						#define MU_LOCK_CREATE(lock, active)
						#define MU_LOCK_DESTROY(lock, active)
						#define MU_LOCK_LOCK(lock, active)
						#define MU_LOCK_UNLOCK(lock, active)

					#endif

				/* Arrays */

					/* Thread */

						struct mu_unix_thread {
							muBool active;
							pthread_t handle;
							void* ret;

							#ifdef MU_THREADSAFE
							muBool lock_active;
							MU_LOCK lock;
							#endif
						};
						typedef struct mu_unix_thread mu_unix_thread;

						MU_HRARRAY_DEFAULT_FUNC(mu_unix_thread)

					/* Mutex */

						struct mu_unix_mutex {
							muBool active;
							pthread_mutex_t handle;

							#ifdef MU_THREADSAFE
							muBool lock_active;
							MU_LOCK lock;
							#endif
						};
						typedef struct mu_unix_mutex mu_unix_mutex;

						MU_HRARRAY_DEFAULT_FUNC(mu_unix_mutex)

					/* Spinlock */

						struct mu_unix_spinlock {
							muBool active;
							int locked;

							#ifdef MU_THREADSAFE
							muBool lock_active;
							MU_LOCK lock;
							#endif
						};
						typedef struct mu_unix_spinlock mu_unix_spinlock;

						MU_HRARRAY_DEFAULT_FUNC(mu_unix_spinlock)

				/* Initiation / Termination */

					/* Setup */

						struct mumContext {
							mu_unix_thread_array threads;
							#define MUM_GTHREADS mum_global_context->threads
							mu_unix_mutex_array mutexes;
							#define MUM_GMUTEXES mum_global_context->mutexes
							mu_unix_spinlock_array spinlocks;
							#define MUM_GSPINLOCKS mum_global_context->spinlocks
						};

					/* API-level */

						mumContext* mum_global_context = MU_NULL_PTR;

						MUDEF void mum_init(mumResult* result) {
							MU_SET_RESULT(result, MUM_SUCCESS)

							MU_ASSERT(mum_global_context == MU_NULL_PTR, result, MUM_ALREADY_INITIALIZED, return;)

							mum_global_context = (mumContext*)mu_malloc(sizeof(mumContext));
							MU_ASSERT(mum_global_context != 0, result, MUM_ALLOCATION_FAILED, return;)

							MUM_GTHREADS = MU_ZERO_STRUCT(mu_unix_thread_array);
							MUM_GMUTEXES = MU_ZERO_STRUCT(mu_unix_mutex_array);
							MUM_GSPINLOCKS = MU_ZERO_STRUCT(mu_unix_spinlock_array);
						}

						MUDEF void mum_term(mumResult* result) {
							MU_SET_RESULT(result, MUM_SUCCESS)

							MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_ALREADY_TERMINATED, return;)

							for (size_m i = 0; i < MUM_GTHREADS.length; i++) {
								mu_thread_destroy(0, i);
							}
							mu_unix_thread_destroy(0, &MUM_GTHREADS);

							for (size_m i = 0; i < MUM_GMUTEXES.length; i++) {
								mu_mutex_destroy(0, i);
							}
							mu_unix_mutex_destroy(0, &MUM_GMUTEXES);

							for (size_m i = 0; i < MUM_GSPINLOCKS.length; i++) {
								mu_spinlock_destroy(0, i);
							}
							mu_unix_spinlock_destroy(0, &MUM_GSPINLOCKS);

							mu_free(mum_global_context);
							mum_global_context = MU_NULL_PTR;
						}

				/* Threads */

					/* API-level */

						MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args) {
							MU_SET_RESULT(result, MUM_SUCCESS)

							MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

							size_m thread = MU_NONE;
							mumaResult muma_res = MUMA_SUCCESS;
							mu_unix_thread_find_push(&muma_res, &MUM_GTHREADS, MU_ZERO_STRUCT(mu_unix_thread), &thread);
							MU_ASSERT(muma_res == MUMA_SUCCESS && thread != MU_NONE, result, muma_result_to_mum_result(muma_res), 
								return MU_NONE;
							)

							mu_unix_thread_hold_element(0, &MUM_GTHREADS, thread);
							void* (*func)(void*);
							mu_memcpy(&func, &start, sizeof(void*));
							if (pthread_create(&MUM_GTHREADS.data[thread].handle, 0, func, args) != 0) {
								MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
								MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
								return MU_NONE;
							}

							MUM_GTHREADS.data[thread].active = MU_TRUE;
							MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
							return thread;
						}

						MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return thread;)
							MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return thread;, mu_unix_thread_)

							MU_ASSERT(pthread_cancel(MUM_GTHREADS.data[thread].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
								MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_) return thread;
							)
							MUM_GTHREADS.data[thread].active = MU_FALSE;

							MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
							return MU_NONE;
						}

						MUDEF void mu_thread_exit(void* ret) {
							pthread_exit(ret);
						}

						MUDEF void mu_thread_wait(mumResult* result, muThread thread) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return;, mu_unix_thread_)

							MU_ASSERT(pthread_join(MUM_GTHREADS.data[thread].handle, &MUM_GTHREADS.data[thread].ret) == 0, result, MUM_WAIT_CALL_FAILED,
								MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_) return;
							)
							MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
						}

						MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return MU_NULL_PTR;)
							MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return MU_NULL_PTR;, mu_unix_thread_)

							void* ret = MUM_GTHREADS.data[thread].ret;
							MU_RELEASE(MUM_GTHREADS, thread, mu_unix_thread_)
							return ret;
						}

				/* Mutexes */

					/* API-level */

						MUDEF muMutex mu_mutex_create(mumResult* result) {
							MU_SET_RESULT(result, MUM_SUCCESS)
							MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

							size_m mutex = MU_NONE;
							mumaResult muma_res = MUMA_SUCCESS;
							mu_unix_mutex_find_push(&muma_res, &MUM_GMUTEXES, MU_ZERO_STRUCT(mu_unix_mutex), &mutex);
							MU_ASSERT(muma_res == MUMA_SUCCESS && mutex != MU_NONE, result, muma_result_to_mum_result(muma_res),
								return MU_NONE;
							)

							mu_unix_mutex_hold_element(0, &MUM_GMUTEXES, mutex);
							if (pthread_mutex_init(&MUM_GMUTEXES.data[mutex].handle, 0) != 0) {
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
								MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
								return MU_NONE;
							}

							MUM_GMUTEXES.data[mutex].active = MU_TRUE;
							MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
							return mutex;
						}

						MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return mutex;)
							MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return mutex;, mu_unix_mutex_)

							MU_ASSERT(pthread_mutex_destroy(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_) return mutex;
							)
							MUM_GMUTEXES.data[mutex].active = MU_FALSE;
							MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
							return MU_NONE;
						}

						// This poses an odd structural challenge. If we make both the lock & unlock
						// function hold the mutex, you reach a deadlock pretty easily, ie:

						// * Thread 1 locks mutex, quickly holding and releasing it.
						// * Thread 2 attempts to lock mutex, holding it and waiting for it to be unlocked,
						// * Thread 1 is done, and attempts to unlock the mutex, so it tries to hold it,
						// but it's still being held by thread 2 who is attempting to lock it, so you reach
						// a deadlock.

						// This can be solved by:
						// A) Making the 'lock mutex' function release the function early, allowing it to
						// wait to lock it without holding it during that wait time.
						// B) Make the 'unlock mutex' function not hold/release the mutex.

						// I've chosen A because the only way it *doesn't* work well is if the user deletes
						// a mutex while a thread is running with it locked AND another one is waiting on
						// it, which is already a horrible situation to be in. Pretty sure deleting a mutex
						// with a thread having it locked is already undefined behavior anyway.

						// Note that this same logic applies to the other locks.

						MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_unix_mutex_)

							MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
							MU_ASSERT(pthread_mutex_lock(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_LOCK_CALL_FAILED, 
								return;
							)
						}

						MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_unix_mutex_)

							MU_ASSERT(pthread_mutex_unlock(&MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_UNLOCK_CALL_FAILED, 
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_) return;
							)
							MU_RELEASE(MUM_GMUTEXES, mutex, mu_unix_mutex_)
						}

				/* Spinlocks */

					/* API-level */

						MUDEF muSpinlock mu_spinlock_create(mumResult* result) {
							MU_SET_RESULT(result, MUM_SUCCESS)
							MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

							size_m spinlock = MU_NONE;
							mumaResult muma_res = MUMA_SUCCESS;
							mu_unix_spinlock_find_push(&muma_res, &MUM_GSPINLOCKS, MU_ZERO_STRUCT(mu_unix_spinlock), &spinlock);
							MU_ASSERT(muma_res == MUMA_SUCCESS && spinlock != MU_NONE, result, muma_result_to_mum_result(muma_res),
								return MU_NONE;
							)

							mu_unix_spinlock_hold_element(0, &MUM_GSPINLOCKS, spinlock);
							MUM_GSPINLOCKS.data[spinlock].locked = 0;

							MUM_GSPINLOCKS.data[spinlock].active = MU_TRUE;
							MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
							return spinlock;
						}

						MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return spinlock;)
							MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return spinlock;, mu_unix_spinlock_)

							MUM_GSPINLOCKS.data[spinlock].locked = 0;
							MUM_GSPINLOCKS.data[spinlock].active = MU_FALSE;
							MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
							return MU_NONE;
						}

						MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_unix_spinlock_)

							MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
							// Note that this is a bit less safe than mutexes, but there's little I can do
							// in this position.
							while (!mum_atomic_compare_exchange(&MUM_GSPINLOCKS.data[spinlock].locked, 0, 1)) {}
						}

						MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_unix_spinlock_)

							mum_atomic_store(&MUM_GSPINLOCKS.data[spinlock].locked, 0);
							MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_unix_spinlock_)
						}

			#endif /* MU_UNIX */

			#ifdef MU_WIN32

				#include <windows.h>

				/* Useful things */

					static inline muBool mum_atomic_compare_exchange(LONG volatile* ptr, LONG compare, LONG exchange) {
						return InterlockedCompareExchange(ptr, exchange, compare) != exchange;
					}

					// Kind of a hack
					static inline void mum_atomic_store(LONG volatile* ptr, long value) {
						if (value == 0) {
							_interlockedbittestandreset(ptr, 0);
						} else {
							_interlockedbittestandset(ptr, 0);
						}
					}

					#ifdef MU_THREADSAFE

						#define MU_LOCK HANDLE

						#define MU_LOCK_CREATE(lock, lock_active) \
							lock = CreateMutex(0, MU_FALSE, 0); \
							if (lock != 0) { \
								lock_active = MU_TRUE; \
							} else { \
								lock_active = MU_FALSE; \
							}

						#define MU_LOCK_DESTROY(lock, lock_active) \
							if (lock_active) { \
								if (CloseHandle(lock) != 0) { \
									lock_active = MU_FALSE; \
								} \
							}

						#define MU_LOCK_LOCK(lock, lock_active) \
							if (lock_active) { \
								WaitForSingleObject(lock, INFINITE); \
							}

						#define MU_LOCK_UNLOCK(lock, lock_active) \
							if (lock_active) { \
								ReleaseMutex(lock); \
							}

					#else

						#define MU_LOCK_CREATE(lock, active)
						#define MU_LOCK_DESTROY(lock, active)
						#define MU_LOCK_LOCK(lock, active)
						#define MU_LOCK_UNLOCK(lock, active)

					#endif

				/* Arrays */

					/* Thread */

						struct mu_win32_thread {
							muBool active;
							HANDLE handle;

							#ifdef MU_THREADSAFE
							muBool lock_active;
							MU_LOCK lock;
							#endif
						};
						typedef struct mu_win32_thread mu_win32_thread;

						MU_HRARRAY_DEFAULT_FUNC(mu_win32_thread)

					/* Mutex */

						struct mu_win32_mutex {
							muBool active;
							HANDLE handle;

							#ifdef MU_THREADSAFE
							muBool lock_active;
							MU_LOCK lock;
							#endif
						};
						typedef struct mu_win32_mutex mu_win32_mutex;

						MU_HRARRAY_DEFAULT_FUNC(mu_win32_mutex)

					/* Spinlock */

						struct mu_win32_spinlock {
							muBool active;
							LONG volatile locked;

							#ifdef MU_THREADSAFE
							muBool lock_active;
							MU_LOCK lock;
							#endif
						};
						typedef struct mu_win32_spinlock mu_win32_spinlock;

						MU_HRARRAY_DEFAULT_FUNC(mu_win32_spinlock)

				/* Initiation / Termination */

					/* Setup */

						struct mumContext {
							mu_win32_thread_array threads;
							#define MUM_GTHREADS mum_global_context->threads
							mu_win32_mutex_array mutexes;
							#define MUM_GMUTEXES mum_global_context->mutexes
							mu_win32_spinlock_array spinlocks;
							#define MUM_GSPINLOCKS mum_global_context->spinlocks
						};

					/* API-level */

						mumContext* mum_global_context = MU_NULL_PTR;

						MUDEF void mum_init(mumResult* result) {
							MU_SET_RESULT(result, MUM_SUCCESS)

							MU_ASSERT(mum_global_context == MU_NULL_PTR, result, MUM_ALREADY_INITIALIZED, return;)

							mum_global_context = (mumContext*)mu_malloc(sizeof(mumContext));
							MU_ASSERT(mum_global_context != 0, result, MUM_ALLOCATION_FAILED, return;)

							MUM_GTHREADS = MU_ZERO_STRUCT(mu_win32_thread_array);
							MUM_GMUTEXES = MU_ZERO_STRUCT(mu_win32_mutex_array);
							MUM_GSPINLOCKS = MU_ZERO_STRUCT(mu_win32_spinlock_array);
						}

						MUDEF void mum_term(mumResult* result) {
							MU_SET_RESULT(result, MUM_SUCCESS)

							MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_ALREADY_TERMINATED, return;)

							for (size_m i = 0; i < MUM_GTHREADS.length; i++) {
								mu_thread_destroy(0, i);
							}
							mu_win32_thread_destroy(0, &MUM_GTHREADS);

							for (size_m i = 0; i < MUM_GMUTEXES.length; i++) {
								mu_mutex_destroy(0, i);
							}
							mu_win32_mutex_destroy(0, &MUM_GMUTEXES);

							for (size_m i = 0; i < MUM_GSPINLOCKS.length; i++) {
								mu_spinlock_destroy(0, i);
							}
							mu_win32_spinlock_destroy(0, &MUM_GSPINLOCKS);

							mu_free(mum_global_context);
							mum_global_context = MU_NULL_PTR;
						}

				/* Threads */

					/* API-level */

						MUDEF muThread mu_thread_create(mumResult* result, void (*start)(void* args), void* args) {
							MU_SET_RESULT(result, MUM_SUCCESS)

							MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

							size_m thread = MU_NONE;
							mumaResult muma_res = MUMA_SUCCESS;
							mu_win32_thread_find_push(&muma_res, &MUM_GTHREADS, MU_ZERO_STRUCT(mu_win32_thread), &thread);
							MU_ASSERT(muma_res == MUMA_SUCCESS && thread != MU_NONE, result, muma_result_to_mum_result(muma_res), 
								return MU_NONE;
							)

							mu_win32_thread_hold_element(0, &MUM_GTHREADS, thread);
							LPTHREAD_START_ROUTINE lp_start;
							mu_memcpy(&lp_start, &start, sizeof(void*));
							DWORD id;
							MUM_GTHREADS.data[thread].handle = CreateThread(0, 0, lp_start, args, 0, &id);
							if (MUM_GTHREADS.data[thread].handle == 0) {
								MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_);
								MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
								return MU_NONE;
							}

							MUM_GTHREADS.data[thread].active = MU_TRUE;
							MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
							return thread;
						}

						MUDEF muThread mu_thread_destroy(mumResult* result, muThread thread) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return thread;)
							MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return thread;, mu_win32_thread_)

							MU_ASSERT(CloseHandle(MUM_GTHREADS.data[thread].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
								MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_) return thread;
							)
							MUM_GTHREADS.data[thread].active = MU_FALSE;

							MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
							return MU_NONE;
						}

						MUDEF void mu_thread_exit(void* ret) {
							DWORD d;
							mu_memcpy(&d, &ret, sizeof(DWORD));
							ExitThread(d);
						}

						MUDEF void mu_thread_wait(mumResult* result, muThread thread) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return;, mu_win32_thread_)

							DWORD wait_result = WaitForSingleObject(MUM_GTHREADS.data[thread].handle, INFINITE);

							switch (wait_result) {
								case WAIT_TIMEOUT: {
									MU_SET_RESULT(result, MUM_THREAD_TIMED_OUT)
								} break;

								case WAIT_FAILED: {
									MU_SET_RESULT(result, MUM_WAIT_CALL_FAILED)
								} break;
							}

							MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
						}

						MUDEF void* mu_thread_get_return_value(mumResult* result, muThread thread) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return MU_NULL_PTR;)
							MU_HOLD(result, thread, MUM_GTHREADS, mum_global_context, MUM_, return MU_NULL_PTR;, mu_win32_thread_)

							DWORD exit_code = 0;
							MU_ASSERT(GetExitCodeThread(MUM_GTHREADS.data[thread].handle, &exit_code) != 0, result, MUM_GET_RETURN_VALUE_CALL_FAILED, 
								MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_) return MU_NULL_PTR;
							)

							void* p;
							mu_memcpy(&p, &exit_code, sizeof(DWORD));

							MU_RELEASE(MUM_GTHREADS, thread, mu_win32_thread_)
							return p;
						}

				/* Mutexes */

					/* API-level */

						MUDEF muMutex mu_mutex_create(mumResult* result) {
							MU_SET_RESULT(result, MUM_SUCCESS)
							MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

							size_m mutex = MU_NONE;
							mumaResult muma_res = MUMA_SUCCESS;
							mu_win32_mutex_find_push(&muma_res, &MUM_GMUTEXES, MU_ZERO_STRUCT(mu_win32_mutex), &mutex);
							MU_ASSERT(muma_res == MUMA_SUCCESS && mutex != MU_NONE, result, muma_result_to_mum_result(muma_res),
								return MU_NONE;
							)

							mu_win32_mutex_hold_element(0, &MUM_GMUTEXES, mutex);
							MUM_GMUTEXES.data[mutex].handle = CreateMutex(0, MU_FALSE, 0);
							if (MUM_GMUTEXES.data[mutex].handle == 0) {
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
								MU_SET_RESULT(result, MUM_CREATE_CALL_FAILED)
								return MU_NONE;
							}

							MUM_GMUTEXES.data[mutex].active = MU_TRUE;
							MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
							return mutex;
						}

						MUDEF muMutex mu_mutex_destroy(mumResult* result, muMutex mutex) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return mutex;)
							MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return mutex;, mu_win32_mutex_)

							MU_ASSERT(CloseHandle(MUM_GMUTEXES.data[mutex].handle) == 0, result, MUM_DESTROY_CALL_FAILED, 
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_) return mutex;
							)
							MUM_GMUTEXES.data[mutex].active = MU_FALSE;
							MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
							return MU_NONE;
						}

						MUDEF void mu_mutex_lock(mumResult* result, muMutex mutex) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_win32_mutex_)

							MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
							DWORD wait_result = WaitForSingleObject(MUM_GMUTEXES.data[mutex].handle, INFINITE);

							switch (wait_result) {
								// The mutex has most likely been closed. This should pretty much never happen with
								// the way mum is set up, but if it has, that's really bad. Most likely, rather mum
								// is not working as intended at all, or the user has modified things that they
								// shouldn't.
								case WAIT_FAILED: {
									MU_SET_RESULT(result, MUM_INVALID_ID)
								} break;

								// The thread holding the mutex has died. This can be due to a few things:
								// * The thread crashed or otherwise imploded in on itself.
								// * I forgot to put an unlock call on an error return case.
								// * The user has fiddled around with values they shouldn't.
								// Either way, this is REALLY, REALLY bad, and will lead to sporadic random bugs &
								// crashes.
								// Note: we still have ownership due to this, but ehhhhh.
								// https://devblogs.microsoft.com/oldnewthing/20050912-14/?p=34253
								// (Raymond Chen is awesome btw)
								case WAIT_ABANDONED: {
									MU_SET_RESULT(result, MUM_PREVIOUS_THREAD_CLOSED_BEFORE_LOCK)
								} break;
							}
						}

						MUDEF void mu_mutex_unlock(mumResult* result, muMutex mutex) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, mutex, MUM_GMUTEXES, mum_global_context, MUM_, return;, mu_win32_mutex_)

							MU_ASSERT(ReleaseMutex(MUM_GMUTEXES.data[mutex].handle), result, MUM_UNLOCK_CALL_FAILED, 
								MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_) return;
							)
							MU_RELEASE(MUM_GMUTEXES, mutex, mu_win32_mutex_)
						}

				/* Spinlocks */

					/* API-level */

						MUDEF muSpinlock mu_spinlock_create(mumResult* result) {
							MU_SET_RESULT(result, MUM_SUCCESS)
							MU_ASSERT(mum_global_context != MU_NULL_PTR, result, MUM_NOT_YET_INITIALIZED, return MU_NONE;)

							size_m spinlock = MU_NONE;
							mumaResult muma_res = MUMA_SUCCESS;
							mu_win32_spinlock_find_push(&muma_res, &MUM_GSPINLOCKS, MU_ZERO_STRUCT(mu_win32_spinlock), &spinlock);
							MU_ASSERT(muma_res == MUMA_SUCCESS && spinlock != MU_NONE, result, muma_result_to_mum_result(muma_res),
								return MU_NONE;
							)

							mu_win32_spinlock_hold_element(0, &MUM_GSPINLOCKS, spinlock);
							MUM_GSPINLOCKS.data[spinlock].locked = 0;

							MUM_GSPINLOCKS.data[spinlock].active = MU_TRUE;
							MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
							return spinlock;
						}

						MUDEF muSpinlock mu_spinlock_destroy(mumResult* result, muSpinlock spinlock) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return spinlock;)
							MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return spinlock;, mu_win32_spinlock_)

							MUM_GSPINLOCKS.data[spinlock].locked = 0;
							MUM_GSPINLOCKS.data[spinlock].active = MU_FALSE;
							MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
							return MU_NONE;
						}

						MUDEF void mu_spinlock_lock(mumResult* result, muSpinlock spinlock) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_win32_spinlock_)

							MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
							while (!mum_atomic_compare_exchange(&MUM_GSPINLOCKS.data[spinlock].locked, 0, 1)) {}
						}

						MUDEF void mu_spinlock_unlock(mumResult* result, muSpinlock spinlock) {
							MU_SAFEFUNC(result, MUM_, mum_global_context, return;)
							MU_HOLD(result, spinlock, MUM_GSPINLOCKS, mum_global_context, MUM_, return;, mu_win32_spinlock_)

							mum_atomic_store(&MUM_GSPINLOCKS.data[spinlock].locked, 0);
							MU_RELEASE(MUM_GSPINLOCKS, spinlock, mu_win32_spinlock_)
						}

			#endif /* MU_WIN32 */

			#ifdef __cplusplus
			}
			#endif

		#else

		#endif /* MUM_IMPLEMENTATION */

	#else

		#define MU_LOCK_CREATE(lock, active)
		#define MU_LOCK_DESTROY(lock, active)
		#define MU_LOCK_LOCK(lock, active)
		#define MU_LOCK_UNLOCK(lock, active)

	#endif /* MU_THREADSAFE */

	#ifdef __cplusplus
		extern "C" { // }
	#endif

	/* Arrays */

		/* Log */

			struct mupl_log {
				muBool active;
				char* filename;

				#ifdef MU_THREADSAFE
				muBool lock_active;
				MU_LOCK lock;
				#endif
			};
			typedef struct mupl_log mupl_log;

			MU_HRARRAY_DEFAULT_FUNC(mupl_log)

	/* Functions */

		/* Result conversion */

			muplResult muma_result_to_mupl_result(mumaResult result) {
				switch (result) {
					default: case MUMA_SUCCESS: return MUPL_MUMA_SUCCESS; break;
					case MUMA_FAILED_TO_ALLOCATE: return MUPL_MUMA_FAILED_TO_ALLOCATE; break;
					case MUMA_INVALID_TYPE_SIZE: return MUPL_MUMA_INVALID_TYPE_SIZE; break;
					case MUMA_INVALID_INDEX: return MUPL_MUMA_INVALID_INDEX; break;
					case MUMA_INVALID_SHIFT_AMOUNT: return MUPL_MUMA_INVALID_SHIFT_AMOUNT; break;
					case MUMA_INVALID_COUNT: return MUPL_MUMA_INVALID_COUNT; break;
					case MUMA_NOT_FOUND: return MUPL_MUMA_NOT_FOUND; break;
				}
			}

		/* Names */

			#ifdef MUPL_NAMES
				MUDEF const char* mupl_result_get_name(muplResult result) {
					switch (result) {
						default: return "MUPL_UNKNOWN"; break;
						case MUPL_SUCCESS: return "MUPL_SUCCESS"; break;
						case MUPL_MUMA_SUCCESS: return "MUPL_MUMA_SUCCESS"; break;
						case MUPL_MUMA_FAILED_TO_ALLOCATE: return "MUPL_MUMA_FAILED_TO_ALLOCATE"; break;
						case MUPL_MUMA_INVALID_TYPE_SIZE: return "MUPL_MUMA_INVALID_TYPE_SIZE"; break;
						case MUPL_MUMA_INVALID_INDEX: return "MUPL_MUMA_INVALID_INDEX"; break;
						case MUPL_MUMA_INVALID_SHIFT_AMOUNT: return "MUPL_MUMA_INVALID_SHIFT_AMOUNT"; break;
						case MUPL_MUMA_INVALID_COUNT: return "MUPL_MUMA_INVALID_COUNT"; break;
						case MUPL_MUMA_NOT_FOUND: return "MUPL_MUMA_NOT_FOUND"; break;
						case MUPL_ALREADY_INITIALIZED: return "MUPL_ALREADY_INITIALIZED"; break;
						case MUPL_ALREADY_TERMINATED: return "MUPL_ALREADY_TERMINATED"; break;
						case MUPL_NOT_YET_INITIALIZED: return "MUPL_NOT_YET_INITIALIZED"; break;
						case MUPL_ALLOCATION_FAILED: return "MUPL_ALLOCATION_FAILED"; break;
						case MUPL_FAILED_TO_OPEN_FILE: return "MUPL_FAILED_TO_OPEN_FILE"; break;
						case MUPL_INVALID_ID: return "MUPL_INVALID_ID"; break;
					}
				}
			#endif

		/* Initiation / Termination */

			struct muplContext {
				mupl_log_array logs;
				#define MUPL_GLOGS mupl_global_context->logs
			};

			muplContext* mupl_global_context = MU_NULL_PTR;

			MUDEF void mupl_init(muplResult* result) {
				MU_SET_RESULT(result, MUPL_SUCCESS)

				MU_ASSERT(mupl_global_context == MU_NULL_PTR, result, MUPL_ALREADY_INITIALIZED, return;)

				mupl_global_context = (muplContext*)mu_malloc(sizeof(muplContext));
				MU_ASSERT(mupl_global_context != 0, result, MUPL_ALLOCATION_FAILED, return;)

				MUPL_GLOGS = MU_ZERO_STRUCT(mupl_log_array);
			}

			MUDEF void mupl_term(muplResult* result) {
				MU_SET_RESULT(result, MUPL_SUCCESS)
				MU_ASSERT(mupl_global_context != MU_NULL_PTR, result, MUPL_ALREADY_TERMINATED, return;)

				for (size_m i = 0; i < MUPL_GLOGS.length; i++) {
					mu_log_destroy(0, i);
				}
				mupl_log_destroy(0, &MUPL_GLOGS);

				mu_free(mupl_global_context);
				mupl_global_context = MU_NULL_PTR;
			}

		/* Log */

			MUDEF muLog mu_log_create(muplResult* result, char* filename, muBool clear) {
				MU_SAFEFUNC(result, MUPL_, mupl_global_context, return MU_NONE;)

				size_m mfilesize = mu_strlen(filename)+1;
				char* mfilename = (char*)mu_malloc(sizeof(char) * mfilesize);
				MU_ASSERT(mfilename != 0, result, MUPL_ALLOCATION_FAILED, return MU_NONE;)
				mu_memcpy(mfilename, filename, mfilesize);
				mfilename[mfilesize-1] = '\0';

				size_m mlog = MU_NONE;
				mumaResult muma_res = MUMA_SUCCESS;
				mupl_log_find_push(&muma_res, &MUPL_GLOGS, MU_ZERO_STRUCT(mupl_log), &mlog);
				MU_ASSERT(muma_res == MUMA_SUCCESS && mlog != MU_NONE, result, muma_result_to_mupl_result(muma_res),
					mu_free(mfilename); return MU_NONE;
				)
				mupl_log_hold_element(0, &MUPL_GLOGS, mlog);
				MUPL_GLOGS.data[mlog].active = MU_TRUE;
				MUPL_GLOGS.data[mlog].filename = mfilename;

				if (clear) {
					MU_FILE* f = mu_fopen(mfilename, "w");
					MU_ASSERT(f != 0, result, MUPL_FAILED_TO_OPEN_FILE, 
						MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_) return mlog;
					)
					mu_fclose(f);
				}

				MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_)
				return mlog;
			}

			MUDEF muLog mu_log_destroy(muplResult* result, muLog mlog) {
				MU_SAFEFUNC(result, MUPL_, mupl_global_context, return mlog;)
				MU_HOLD(result, mlog, MUPL_GLOGS, mupl_global_context, MUPL_, return mlog;, mupl_log_)

				if (MUPL_GLOGS.data[mlog].filename != MU_NULL_PTR) {
					mu_free(MUPL_GLOGS.data[mlog].filename);
					MUPL_GLOGS.data[mlog].filename = MU_NULL_PTR;
				}
				MUPL_GLOGS.data[mlog].active = MU_FALSE;

				MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_)
				return MU_NONE;
			}

			MUDEF void mu_printf(muplResult* result, muLog mlog, const char* p, ...) {
				MU_SAFEFUNC(result, MUPL_, mupl_global_context, return;)
				MU_HOLD(result, mlog, MUPL_GLOGS, mupl_global_context, MUPL_, return;, mupl_log_)

				mu_va_list args;
				mu_va_start(args, p);

				size_m size = mu_vsnprintf(0, 0, p, args);
				char* s = (char*)mu_malloc(size+1);
				MU_ASSERT(s != 0, result, MUPL_ALLOCATION_FAILED, 
					MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_) mu_va_end(args); return;
				)
				mu_va_end(args);
				mu_va_start(args, p);
				mu_vsprintf(s, p, args);
				s[size] = '\0';

				mu_std_printf("%s", s);

				MU_FILE* f = mu_fopen(MUPL_GLOGS.data[mlog].filename, "a");
				MU_ASSERT(f != 0, result, MUPL_FAILED_TO_OPEN_FILE, 
					MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_) mu_free(s); mu_va_end(args); return;
				)
				mu_fprintf(f, "%s", s);
				mu_fclose(f);

				mu_free(s);

				mu_va_end(args);
				MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_)
			}

			MUDEF void mu_log(muplResult* result, muLog mlog, const char* p, ...) {
				MU_SAFEFUNC(result, MUPL_, mupl_global_context, return;)
				MU_HOLD(result, mlog, MUPL_GLOGS, mupl_global_context, MUPL_, return;, mupl_log_)

				mu_va_list args;
				mu_va_start(args, p);

				size_m size = mu_vsnprintf(0, 0, p, args);
				char* s = (char*)mu_malloc(size+1);
				MU_ASSERT(s != 0, result, MUPL_ALLOCATION_FAILED, 
					MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_) mu_va_end(args); return;
				)
				mu_va_end(args);
				mu_va_start(args, p);
				mu_vsprintf(s, p, args);
				s[size] = '\0';

				MU_FILE* f = mu_fopen(MUPL_GLOGS.data[mlog].filename, "a");
				MU_ASSERT(f != 0, result, MUPL_FAILED_TO_OPEN_FILE, 
					MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_) mu_free(s); mu_va_end(args); return;
				)
				mu_fprintf(f, "%s", s);
				mu_fclose(f);

				mu_free(s);

				mu_va_end(args);
				MU_RELEASE(MUPL_GLOGS, mlog, mupl_log_)
			}

	#ifdef __cplusplus
		}
	#endif

#endif /* MUPL_IMPLEMENTATION */

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2024 Hum
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/

