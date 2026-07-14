# Native matching wave 34

This wave recovered the shared TinyOS block ABI, promoted two complete Game
Speech SDK owners, and materialized three more block owners as typed C. Only
the two whole-object matches are configured Matching. Every partial owner
continues to link its extracted target object and remains in the
`C-not-yet-matched` bucket.

## Target-proven TinyOS ABI

The retained `tinyos.o` call sites establish the `0x28` `TosBaseBlock` layout:

- context pointer at `0x00` and block index byte at `0x04`;
- input/output count bytes at `0x08` and `0x09`;
- input/output port pointers at `0x0C` and `0x10`;
- init, process, and control callbacks at `0x14`, `0x18`, and `0x1C`;
- enabled byte at `0x20`.

`tosRun` loads the input count from byte `0x08` and calls the process callback
with the block, input-pointer array, and count in `r3`/`r4`/`r5`. The gender
filter's retained signed `cmpw` proves the third argument's promoted `s32`
type. TinyOS calls init with the block alone and control with block, command,
argument, and argument size. Constructor call sites establish
`(TosContext *context, u32 blockIndex)` and the count-byte arguments to
`tosBaseBlockConstruct`.

These facts replace the old reserved-byte placeholder in `gssdk/tos.h` and
remove a systematic callback frame mismatch. The local `mwcc-decomp`
CodeWarrior stack-allocation implementation was consulted to distinguish real
parameter/local ABI effects from cosmetic source edits. No dead locals,
register-forcing constructs, inline assembly, or byte blobs were introduced.

## Complete owner promotions

`asrpho/common/blocks/flfxblks/lkahead.c` recovers the `0x4C` look-ahead
block, its history ring, threshold accumulation, queue delay, flush control,
allocation, and destruction. DTK 0.9.2 reports exact `.text 0x2AC` and
`.sdata2 0x8` sections. All four functions and all relocations match.

`asrpho/rec1600/initial.c` recovers a data-only owner as semantic typed
definitions: 45 zero-terminated profile key/value tables, `IniFile[87]`, the
41-entry `InitialProcs` table, 52 queue-writer triplets, and 74 queue-reader
pairs. DTK reports exact `.rodata 0x9C8` and all 102 relocations. MP7 carries
the same binary table layout and was used as a boundary cross-check only; no
sibling source donor was available, so the retained MP6 symbols, bytes,
relocations, and consumer field accesses are the authority.

## Typed fallback recovery

The same ABI recovery moved five fallback-linked owners substantially closer
without relaxing the owner gate:

- `delaybl.c`: process, init, and constructor are exact. The `0xE4` control
  function is 98.947365%; its sole instruction difference is target
  `addi r4,r5,0` versus source `li r4,0` in the clear loop.
- `statio.c`: control, init, and constructor are exact. The `0x1A8` process
  function is 99.575470% with seven floating-point register-assignment
  differences. Target/source `.sdata2` sizes are `0x10/0xC` because the
  target split includes a four-byte alignment tail.
- `undersam.c`: process, control, and constructor are exact. The `0xC0` init
  is 99.687500%; only the quotient's volatile register differs (`r5` versus
  `r4`).
- `genfilt.c`: process, control, and init are exact. The `0x7C` constructor
  retains profile-probe and callback-address scheduling differences.
- `median.c`: control, init, and constructor are exact. The recovered `0x44`
  block and `0xC` node express the target ring replacement, sorted insertion,
  allocation, and median selection, but the `0x2F4` process routine remains
  78.952380%. MP7 authenticates the same machine-code behavior, not a source
  spelling.

Evidence-backed declaration-order passes were retained where they improved
the emitted object. Byte-neutral or worse alternatives were reverted. None
of these five owners was promoted because one divergent function is enough to
fail the whole-owner gate.

## Full gate

After linking only the two complete owners from C, the pinned DTK 0.9.2 build
passed the complete gate:

- `ninja -j1`: `137 files OK`;
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`;
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical.

DTK reports 289/895 Matching owners overall and 280/396 in the DOL. The DOL
policy split is 248 clean-C owners, 32 authenticated assembly-exception
owners, 115 `C-not-yet-matched` owners, and one `original-was-asm` owner.
