#include "Test.h"
#include "SkVarAlloc.h"

DEF_TEST(VarAlloc, r) {
    SkVarAlloc va;
    char* p = va.alloc(128, SK_MALLOC_THROW);
    sk_bzero(p, 128);  // Just checking this is safe.

#ifndef SK_BUILD_FOR_ANDROID
    // This method will always return 0 on Android.
    REPORTER_ASSERT(r, va.approxBytesAllocated() >= 128);
#endif
}
