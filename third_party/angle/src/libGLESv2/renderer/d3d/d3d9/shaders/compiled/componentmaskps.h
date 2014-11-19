#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.16384
//
///
// Parameters:
//
//   float4 add;
//   float4 mult;
//   sampler2D tex;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   mult         c0       1
//   add          c1       1
//   tex          s0       1
//

    ps_2_0
    dcl t0.xy
    dcl_2d s0
    texld r0, t0, s0
    mov r1, c0
    mad r0, r0, r1, c1
    mov oC0, r0

// approximately 4 instruction slots used (1 texture, 3 arithmetic)
#endif

const BYTE g_ps20_componentmaskps[] =
{
      0,   2, 255, 255, 254, 255, 
     50,   0,  67,  84,  65,  66, 
     28,   0,   0,   0, 143,   0, 
      0,   0,   0,   2, 255, 255, 
      3,   0,   0,   0,  28,   0, 
      0,   0,   0,   1,   0,   0, 
    136,   0,   0,   0,  88,   0, 
      0,   0,   2,   0,   1,   0, 
      1,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
    108,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0, 113,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0, 120,   0,   0,   0, 
      0,   0,   0,   0,  97, 100, 
    100,   0,   1,   0,   3,   0, 
      1,   0,   4,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
    109, 117, 108, 116,   0, 116, 
    101, 120,   0, 171, 171, 171, 
      4,   0,  12,   0,   1,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0, 112, 115, 
     95,  50,  95,  48,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
     32,  72,  76,  83,  76,  32, 
     83, 104,  97, 100, 101, 114, 
     32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  57,  46, 
     51,  48,  46,  57,  50,  48, 
     48,  46,  49,  54,  51,  56, 
     52,   0, 171, 171,  31,   0, 
      0,   2,   0,   0,   0, 128, 
      0,   0,   3, 176,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      0,   8,  15, 160,  66,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 176,   0,   8, 
    228, 160,   1,   0,   0,   2, 
      1,   0,  15, 128,   0,   0, 
    228, 160,   4,   0,   0,   4, 
      0,   0,  15, 128,   0,   0, 
    228, 128,   1,   0, 228, 128, 
      1,   0, 228, 160,   1,   0, 
      0,   2,   0,   8,  15, 128, 
      0,   0, 228, 128, 255, 255, 
      0,   0
};
