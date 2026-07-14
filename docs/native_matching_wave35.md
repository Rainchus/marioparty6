# Native matching wave 35

This wave batches the remaining genuinely easy main-DOL ports found in the
Game Speech SDK and MSL scans. Two complete owners are promoted. Seven other
owners gain semantic source, real layouts, and exact subfunctions but remain
fallback-linked because their complete objects do not match.

## Complete owner promotions

`asrpho/common/blocks/stacker.c` recovers the target `0x38` block and all four
functions. Its source scans for any available input, conditionally copies
enabled inputs into one queue element, implements control commands 100, 101,
150, 151, and 255, allocates the profiled element-size and enable arrays, and
derives the input/output port sizes. Target/source `.text` are both `0x384`;
all instructions and all 19 relocations match.

`asrpho/rec1600/ctrl.c` is a data-only owner recovered as typed control
records rather than a byte array. It names command headers, target pairs,
state records, callback pointers, architecture tables, and `ControlTable`.
The MP6 target symbols, section bytes, callback relocations, and `_tosControl`
consumer behavior establish the layouts. DTK 0.9.2 reports exact
`.rodata 0x1000`, `.sdata2 0x28`, and all 390 relocations.

Neither owner contains assembly, opaque byte storage, or a fallback packet.

## GSSDK source materialization

Five additional GSSDK owners move from `NO-SOURCE` to compiled, typed C:

- `flblocks/mtxopt.c` recovers the real `0x10` float-matrix header and packed
  QR update. `mtxFillCopy` and `mtxFillX` are exact. `QrPreMult` remains
  target/source `0x1A8/0x1A4` at 92.528305%.
- `gsapi/callbacks.c` recovers six exact notification wrappers and exact
  callback tables (`0x18` data and `0x8` small data). The target's `0x580`
  result callback is not guessed and remains absent.
- `gsapi/ctxfuncs.c` recovers five small routines. Four are exact;
  `ContextGetParam` is 84.500000%. Ten larger context/session functions are
  left for later reverse engineering.
- `ctxdata/langdata.c` and its owner-local header recover the real language
  prefix, code-book prefix, and 36 exact direct accessors. The first divergent
  packed-tail computation is `_langGetpErgodicPenalty` at 77.040000%; deeper
  offsets and the virtual-table closure remain explicit work.
- `tos/mqueue.c` and its owner-local header recover the queue/list/reader
  prefix. `qQueueNbrElements` is exact; dequeue and construction compile at
  89.500000% and 84.200000%. Complex queue control and allocation routines
  remain absent.

The shared TinyOS header now names `TosQueue` rather than exposing queue
fields as `void *`. This is a type-only refinement and preserves the exact
wave-34 block objects.

## Authenticated MSL source

`MSL_C.PPCEABI.bare.H/e_exp.c` comes from the Matching Pikmin 2 fdlibm source
at commit `46aecad6`. MP6's `.rodata 0x30`, `.sdata2 0x78`, and all 21
relocations match, while `__ieee754_exp` remains `0x21C/0x224` at 91.592590%.

`MSL_C.PPCEABI.bare.H/alloc.c` is a target-derived adaptation cross-checked
against the Matching FFCC commit `ff63985d` and the Matching Super Mario
Strikers source family. It restores the `Block`, `SubBlock`, fixed-block, and
pool layouts,
the six fixed sizes, and all 11 MP6 routines. The fixed-size `.rodata 0x18`
matches, but GC/2.6 code generation and retained helper identities diverge
substantially from the GC/2.7 donor, so the owner remains NonMatching.

The known `qsort.c` donor mismatch was not re-probed in this batch.

## Gate and policy result

Only `stacker.c` and `ctrl.c` were switched to Matching. The final pinned
toolchain gate passed:

- `ninja -j1`: `137 files OK`;
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`;
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical.

DTK reports 291/895 Matching owners overall and 282/396 in the DOL. The DOL
policy split is 250 clean-C owners, 32 authenticated assembly-exception
owners, 113 `C-not-yet-matched` owners, and one `original-was-asm` owner.
