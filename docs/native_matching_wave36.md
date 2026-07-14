# Native matching wave 36

This wave batches the next small main-DOL recovery owners without promoting
fallback-linked code. Every retained implementation is source-level C derived
from target instructions, data, relocations, consumers, or authenticated
sibling evidence. No assembly transcription, opaque byte storage, register
forcing, or fabricated match was added.

## Re-proved existing owners

`board/audio.c` now gives the board-music table its target-proven unsigned
board number and reuses the same local `BoardNoGet` inline already present in
Matching `board/camera.c`. This produces the target field-first unsigned
comparison in both music-table consumers. `mbMusBoardPlay` (`0x12C`) and
`MusBoardFade` (`0x1AC`) are now exact, bringing the owner to 48/49 exact
functions. Only `mbMusBoardFadeOut` remains divergent: target/source are both
`0x3F0`, with twelve stack-slot operand differences. The owner remains
`NonMatching`.

`gsapi/extaudio.c` remains Matching. It now includes the shared
`gssdk/gsapi.h` definition instead of redeclaring a truncated translation-unit
copy of `GSAPIState` and its callback. The recompiled owner retains exact
`.text 0xC8`, all six functions, and all four relocations.

## New complete source candidates

`MSL_C.PPCEABI.bare.H/qsort.c` restores the authenticated MSL heapsort from
Matching `yannicksuter/smstrikers-decomp` commit
`795ee4831df62e73a9c728a81a8f79a0e3684ab4`. The MP6 target/source function is
`0x170/0x16C` at 98.260870%. Its only structural difference is the target's
`li 2; mullw` sequence versus the donor compiler's `slwi`; GC/2.7 is
byte-neutral and prior authenticated multiplication spellings did not close
that difference. The source is retained as real MSL C and the owner remains
`NonMatching`.

`asrpho/common/blocks/flblocks/vq1500.c` recovers the real `0x30` TinyOS
block, `0x14` code-book descriptor, target-proven language/code-book method
interface, both
nearest-vector searches, queue process/control lifecycle, and all nine owner
functions. Eight functions are exact. `GetLabel` is exact-size `0x148` at
98.109760%, with only compiler register assignment remaining. Whole target
and source `.text` are both `0x4F0` at 99.509490%; `.sdata2`'s semantic float
is exact, the target owns one four-byte alignment tail, and both objects have
25 text relocations. The owner remains `NonMatching`.

`asrpho/common/blocks/flblocks/logexp.c` restores the typed 353-float lookup
table and both functions. MP7 provides an independently identical owner body
apart from linked relocation fields. `HLnOnePlusExpHFloat` (`0x90`), the
semantic `.rodata 0x584`, and `.sdata2 0x48` are exact. `LogAdd` is
target/source `0x138/0x138` at 99.743590%, with four floating-point operand
register differences. Target/source `.text` are `0x1C8/0x1C8`; the target's
`.rodata 0x588` includes one four-byte alignment tail, and relocations are
31/31. The owner remains `NonMatching`.

`asrpho/common/blocks/flblocks/dctlift.c` recovers the complete `0x3C` block,
matrix multiply, DCT coefficient generation, profile fields, heap lifecycle,
and four functions. MP7 target-binary parity independently confirms the same
owner shape. `ControlDCTLift` and `ConstructDCTLift` are exact;
`ProcessDCTLift` is exact-size `0x1E4` at 98.950420%, and `InitDCTLift` is
target/source `0x1FC/0x200` at 85.968506%. Whole `.text` is `0x484/0x488` at
93.394460%; `.sdata2 0x38` and all 27 relocations align. The owner remains
`NonMatching`.

`asrpho/common/blocks/exev_dp.c` recovers the complete `0x6C` extra-event
dynamic-programming block, `0x18` context-information layout, allocation and
control lifecycle, and all six functions. MP7 binary parity authenticates the
core Viterbi and dynamic-programming shapes, while the MP6 `convert.c`
consumer establishes the context layout. `ProcessExtraEventDP` (`0x68`) and
`ConstructExtraEventDp` (`0x44`) are exact. Whole target/source `.text` are
`0x4D8/0x54C` at 72.774190%, with target/source relocation-section sizes
`0xCC/0xC0`; the larger control closure remains genuine C matching work.

## Deferred owner

`smoother.c` was screened and left absent. Its `0x358` smoothing routine is a
nested QR back-substitution and column-deletion algorithm with aliased matrix
storage. MP7 authenticates the binary owner but supplies no source. A first
pass would require unsupported control-flow and register-shape assumptions,
so no transcription was retained.

`slidhist.c` was also left absent. Its eight routines operate on a histogram
embedded at offset `0xE8` in the still-unrecovered `TriggerLearner` owner from
`trigglr.c`. Introducing a guessed shared layout or an opaque `0xE8` pad would
not be source recovery, so the owner was deferred without a compile probe.

`subsamp.c` received one complete algorithm transcription and object probe,
then was removed by the ownership audit. The functions operate on the same
`0x19C` `TriggerLR` object: TinyOS base `0x000..0x027`, trigger core
`0x028..0x0E7`, sliding histogram `0x0E8..0x113`, voicing state
`0x114..0x157`, and subsampler state `0x158..0x19B`. The first 48 trigger-core
words still lack a recovered shared declaration. Keeping the transcription
would therefore require a large opaque pad disguised as a `Voicing` class;
the file was deleted and remains `NO-SOURCE` until `trigglr.c` establishes the
real common owner.

## Gate result

No owner in this batch is switched to Matching. The single final batch gate
passed with the pinned toolchain:

- `ninja -j1`: `137 files OK`;
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`;
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical.

DTK remains at 291/895 Matching owners overall and 282/396 in the DOL. The
DOL policy split remains 250 clean-C owners, 32 authenticated assembly-
exception owners, 113 `C-not-yet-matched` owners, and one
`original-was-asm` owner. This wave changes the C/source inventory, not the
fallback-linked build count.
