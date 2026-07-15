# Native matching wave 51: authenticated Dolphin OS/GX recovery

Wave 51 promotes four Dolphin SDK owners under the existing authenticated-
assembly exception and advances real source shape in one further Dolphin owner
and one Board owner. `GXTransform.c`, `OSMemory.c`, `OSReset.c`, and
`__start.c` pass the complete MP6 gate. `OS.c` and `board/board.c` improve but
remain `NonMatching`. Assembly in this wave is admitted only where the fully
matching Mario Party 4 repository carries the same SDK routine and source
shape; it is reported separately from clean C.

## Tool and evidence provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Dolphin compiler: project-pinned `GC/1.2.5n`; Board compiler: `GC/2.6`.
- Recovery inputs: MP6 target instructions, object symbols, relocations,
  linked ranges, current callers/types, the local `dolsdk2004` source, and
  fully matching Mario Party 4 commit `147b165`.

The Mario Party 4 project states that both USA revisions are completely
matching. Its SDK bodies therefore authenticate original assembly source
shape under the STATUS exception; binary similarity alone would not have been
sufficient.

## `GXTransform.c`: complete paired-single recovery

Seven formerly divergent routines all consume the same paired-single FIFO
helpers. MP6 target instructions contain the exact `psq_l`/`psq_st` sequences
carried by matching M4 `WriteProjPS`, `Copy6Floats`, `WriteMTXPS4x3`,
`WriteMTXPS3x3from3x4`, and `WriteMTXPS4x2`. Restoring those authenticated
helpers closes projection copies, immediate position/normal/texture matrix
writes, and viewport retrieval. Restoring the SDK-authentic `reg` local in
`GXLoadTexMtxImm` closes the final encoded-register operand lifetime.

All 16 retained functions, target `.text 0x72C`, `.sdata2 0x10`, and 31
retained relocations match exactly. The source object additionally emits the
six authenticated helpers and `__GXSetProjection`, totaling `0x10C`; their
bodies are inlined into retained functions and the out-of-line copies are
linker-stripped. The linked owner and full DOL/container gate pass.

## `OSMemory.c`: complete memory-mode recovery

Matching M4 authenticates `Config24MB`, `Config48MB`, and `RealMode` as the
original low-level assembly routines. The same SDK family supplies the real
`OSProtectRange` and non-debug `__OSInitMemoryProtection` C shape. All eight
MP6 target functions are exact. Target/source `.text` is `0x2F0/0x3B4`;
source-only `OSProtectRange` is `0xC4` with five source-only relocations and is
linker-stripped. The 26 retained target relocations and linked owner are exact.

## `OSReset.c`: complete reset recovery

Matching M4 authenticates `Reset`, replacing the incorrect C transcription
that both mismatched the function and changed `__OSDoHotReset` through
inlining. All seven retained MP6 functions now match. Target/source `.text` is
`0x484/0x634`; source-only `__OSShutdownDevices` is `0x1B0` with 11 extra
relocations and is linker-stripped. All 47 retained relocations are exact.
Target `.data 0x50` versus semantic source `.data 0x4E` is the proven target
split's two-byte tail padding; the linked range and full container match.

## `__start.c`: complete startup recovery

Matching M4 authenticates `__start` and `__init_registers` and supplies the
startup family around the already recovered C initialization path. All seven
retained target functions now match. Target/source `.init` is `0x300/0x38C`;
source-only copy/BSS helpers total `0x8C` with three extra relocations and are
linker-stripped. All 30 retained relocations and the final linked range match.

## `OS.c`: mixed owner remains fallback-linked

The same matching sibling authenticates the omitted FPR/vector/default-
exception/paired-single/init assembly family. Restoring it moves the owner
from 0/14 to 12/14 exact functions. `OSInit` remains exact-size `0x4E0` at
97.500000%; `OSExceptionInit` remains exact-size `0x280` at 98.500000%.
Their residuals are same-op pooled-string relocation-owner differences.
Target/source `.text` is `0xACC/0xC2C` with 166/191 relocations, so the mixed
owner correctly stays `NonMatching` and in the C-not-yet-matched bucket.

## `board/board.c`: turn-loop source recovery

MP6 control flow and the fully matching MP5 board main authenticate four
source corrections in `mbMain`: time/minigame-return handling belongs before
the turn loop, Last Five joins the shared `else` path, a normal minigame return
does not set the interrupt flag, and turn reset is shared at the loop bottom.
This is control-flow recovery, not register allocation tuning.

`mbMain` remains exact-size `0x744` and improves from 98.660220% to
99.133330%. Whole target/source `.text` remains exact-size `0x1CFC`, all 618
relocations pair, and 31/35 functions remain exact. Four functions still
diverge, so the owner stays `NonMatching`.

## Clean deferrals

- `board/audio.c`: the only remaining function has exactly 12 stack-slot
  offset differences; no authenticated semantic source change was found.
- `board/branch.c`: the only remaining function has a 19-use `r26`/`r27`
  operand-color swap; no source rewrite was retained.
- `dolphin/exi/EXIBios.c`: porting the fully matching M4 source family was
  byte-neutral for the retained functions. Broad structural/register
  divergence remains, so the neutral port was discarded.
- `dolphin/mic/{mic,m2s}.c`: a proposed `GC/1.2.5n` switch was rejected because
  the target compiler metadata identifies `2.4.7.1` while the candidate
  objects identify `2.3.0.1`; no compiler hypothesis survived.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts: 298/895 owners overall and 289/396 DOL owners.
- Progress: 9.08% code and 31.26% data overall; 46.52% code and 74.31% data
  in the DOL.
