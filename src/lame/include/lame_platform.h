/*
 * lame_platform.h
 *
 * Small platform helpers for page-size / aligned allocations on Android.
 * When compiled with -DLAME_ANDROID_RUNTIME_PAGESIZE this header replaces
 * malloc/calloc/free within LAME sources with aligned variants that use
 * a 16KB alignment (common on some Android devices).
 *
 * The replacement is intentionally conservative and only active when the
 * macro is defined by the build system (CMake). It uses posix_memalign.
 */
#ifndef LAME_PLATFORM_H
#define LAME_PLATFORM_H

#ifdef LAME_ANDROID_RUNTIME_PAGESIZE

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Target alignment in bytes. Use 16KB to support devices with 16KB page size. */
#ifndef LAME_PAGE_ALIGNMENT
#define LAME_PAGE_ALIGNMENT 16384
#endif

static inline void *lame_aligned_malloc(size_t size) {
    void *ptr = NULL;
    size_t align = (size_t)LAME_PAGE_ALIGNMENT;
    /* Round allocation size up to alignment to keep behavior consistent */
    size_t rounded = ((size + align - 1) / align) * align;
    if (rounded == 0) rounded = align;
    if (posix_memalign(&ptr, align, rounded) != 0) return NULL;
    return ptr;
}

static inline void *lame_aligned_calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void *p = lame_aligned_malloc(total);
    if (p) memset(p, 0, total);
    return p;
}

static inline void lame_aligned_free(void *p) {
    free(p);
}

/* Replace standard allocation symbols inside LAME when building for Android.
   These macros are intentionally simple and limited in scope (only active
   when LAME_ANDROID_RUNTIME_PAGESIZE is provided). */
#define malloc(x)      lame_aligned_malloc(x)
#define calloc(n,m)    lame_aligned_calloc((n),(m))
#define free(x)        lame_aligned_free(x)

#ifdef __cplusplus
}
#endif

#endif /* LAME_ANDROID_RUNTIME_PAGESIZE */

#endif /* LAME_PLATFORM_H */
