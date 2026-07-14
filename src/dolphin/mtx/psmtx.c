#include <dolphin.h>
#include <dolphin/mtx.h>

#define qr0 0
#define qr1 1
#define qr6 6

asm void PSMTXReorder(const register Mtx src, register ROMtx dest)
{
    /* clang-format off */
    psq_l f0, 0(src), 0, qr0
    psq_l f2, 16(src), 0, qr0
    psq_l f4, 32(src), 0, qr0
    psq_l f1, 8(src), 0, qr0
    ps_merge00 f6, f0, f2
    psq_l f3, 24(src), 0, qr0
    ps_merge01 f12, f4, f0
    psq_l f5, 40(src), 0, qr0
    ps_merge11 f7, f2, f4
    psq_st f6, 0(dest), 0, qr0
    ps_merge00 f8, f1, f3
    psq_st f12, 8(dest), 0, qr0
    ps_merge01 f9, f5, f1
    psq_st f7, 16(dest), 0, qr0
    ps_merge11 f10, f3, f5
    psq_st f8, 24(dest), 0, qr0
    psq_st f9, 32(dest), 0, qr0
    psq_st f10, 40(dest), 0, qr0
    /* clang-format on */
}

asm void PSMTXROMultVecArray(const register ROMtx m, const register Vec *srcBase, register Vec *dstBase, register u32 count)
{
    /* clang-format off */
    nofralloc
    stwu r1, -64(r1)
    stfd f14, 8(r1)
    subi r7, count, 1
    stfd f15, 16(r1)
    srwi r7, r7, 1
    stfd f16, 24(r1)
    stfd f17, 32(r1)
    stfd f18, 40(r1)
    mtctr r7
    psq_l f0, 0(m), 0, qr0
    subi srcBase, srcBase, 8
    psq_l f1, 8(m), 1, qr0
    subi dstBase, dstBase, 4
    psq_l f6, 36(m), 0, qr0
    psq_lu f8, 8(srcBase), 0, qr0
    psq_l f7, 44(m), 1, qr0
    psq_lu f9, 8(srcBase), 0, qr0
    ps_madds0 f11, f0, f8, f6
    psq_l f2, 12(m), 0, qr0
    ps_madds0 f12, f1, f8, f7
    psq_l f3, 20(m), 1, qr0
    ps_madds1 f13, f0, f9, f6
    psq_lu f10, 8(srcBase), 0, qr0
    ps_madds1 f14, f1, f9, f7
    psq_l f5, 32(m), 1, qr0
    ps_madds1 f11, f2, f8, f11
    ps_madds1 f12, f3, f8, f12
    psq_l f4, 24(m), 0, qr0
    ps_madds0 f13, f2, f10, f13
    psq_lu f8, 8(srcBase), 0, qr0
    ps_madds0 f14, f3, f10, f14
    ps_madds0 f15, f4, f9, f11
    ps_madds0 f16, f5, f9, f12
    psq_lu f9, 8(srcBase), 0, qr0
    ps_madds1 f17, f4, f10, f13
    ps_madds1 f18, f5, f10, f14
    psq_lu f10, 8(srcBase), 0, qr0
loop:
    ps_madds0 f11, f0, f8, f6
    psq_stu f15, 4(dstBase), 0, qr0
    ps_madds0 f12, f1, f8, f7
    psq_stu f16, 8(dstBase), 1, qr0
    ps_madds1 f13, f0, f9, f6
    psq_stu f17, 4(dstBase), 0, qr0
    ps_madds1 f14, f1, f9, f7
    psq_stu f18, 8(dstBase), 1, qr0
    ps_madds1 f11, f2, f8, f11
    ps_madds1 f12, f3, f8, f12
    psq_lu f8, 8(srcBase), 0, qr0
    ps_madds0 f13, f2, f10, f13
    ps_madds0 f14, f3, f10, f14
    ps_madds0 f15, f4, f9, f11
    ps_madds0 f16, f5, f9, f12
    psq_lu f9, 8(srcBase), 0, qr0
    ps_madds1 f17, f4, f10, f13
    ps_madds1 f18, f5, f10, f14
    psq_lu f10, 8(srcBase), 0, qr0
    bdnz loop
    psq_stu f15, 4(dstBase), 0, qr0
    clrlwi. r7, count, 31
    psq_stu f16, 8(dstBase), 1, qr0
    bne exit
    psq_stu f17, 4(dstBase), 0, qr0
    psq_stu f18, 8(dstBase), 1, qr0
exit:
    lfd f14, 8(r1)
    lfd f15, 16(r1)
    lfd f16, 24(r1)
    lfd f17, 32(r1)
    lfd f18, 40(r1)
    addi r1, r1, 64
    blr
    /* clang-format on */
}
