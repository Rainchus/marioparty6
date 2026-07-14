# Native matching wave 37

This wave batches five main-DOL owners into retained source-level C. The MP6
target instructions, data, symbols, relocations, and consumers are the primary
evidence. Matching Mario Party 5 source shapes and Mario Party 7 binary parity
are used only where the MP6 object independently confirms the same owner.
There is no inline assembly, standalone assembly, opaque byte storage, explicit
layout padding, or match-only register forcing in this batch.

No owner is promoted to Matching. Object comparisons were used while building
the batch, followed by one full final DOL/REL gate.

## Matrix ownership

`asrpho/common/blocks/flblocks/mtx.c` recovers the complete parallel matrix
descriptors:

- `FloatMatrix` and `IntMatrix` are both naturally `0x18` bytes;
- offsets `0x00`, `0x04`, and `0x08` hold rows, columns, and the retained
  element count;
- offsets `0x0C` and `0x10` hold the value range;
- offset `0x14` is the allocation-ownership flag.

The MP6 target proves all field accesses and allocation sizes. Existing
`mtxopt.c` independently proved the shared prefix, while the MP7 owner at
`0x80132E94` has 487 of 511 linked words identical; its 24 differing words are
relocation-bearing. The recovered owner contains all 12 functions, including
the real Givens-rotation QR column deletion rather than a substituted numerical
routine.

Nine functions are byte-exact: both constructors, both destructors, `mtxInit`,
`imtxInit`, `mtxInitCopy`, `mtxAssociateAll`, and `mtxMax`.
`imtxDeleteCol` is target/source `0x88/0x88` at 98.676470%, `mtxCompress` is
`0xC4/0xBC` at 94.795920%, and `QrDeleteCol` is `0x308/0x314` at 94.118550%.
Whole target/source `.text` is `0x7FC/0x800`, and both objects have 25
relocations. The source emits `.sdata2 0x40` versus target `0x30` because its
compiler retains an additional pair of `sqrtf` constants.

`mtxopt.c` now consumes the shared full definition instead of carrying a
private truncated prefix. Its prior proof is unchanged: `mtxFillCopy` and
`mtxFillX` remain exact, while `QrPreMult` remains `0x1A8/0x1A4` at
92.528305%.

## Feature combiner

`asrpho/common/blocks/flfxblks/combiner.c` recovers the complete natural
`0x5C` owner: the `TosBaseBlock`, frame count, rotating history index, band and
vector widths, eight `f32 *` history lanes, allocation owner, and flushing
flag. Target pointer arithmetic proves the eight-lane ring and both
second-order feature passes. The four constants are the target values `0.2f`,
`2.0f`, `0.375f`, and `0.0f`. MP7 at `0x80138004` carries the same complete
function sequence, with all linked-word differences explained by relocations.

All four functions are present. `CombinerControl` (`0xEC`) and
`ConstructCombiner` (`0x44`) are exact. `CombinerInit` is exact-size `0x128`
at 98.378380%, and `CombinerProcess` is target/source `0x348/0x34C` at
87.404760%. Whole `.text` is `0x5A0/0x5A4`; `.sdata2 0x10` and all 21
relocations align.

## Pitch window

`asrpho/common/blocks/pitchwin.c` recovers the complete natural `0x44` block:
the window coefficients, signed-sample circular history and its three active
pointers, frame count, frame length, and total window length. The five target
functions establish the Hamming construction
`0.54 - 0.46 * cos(2*pi*t/duration)`, symmetric coefficient fill, profile
validation, queue sizes, and circular-history process/control lifecycle. MP7
at `0x8012F718` has the same `0x148/0xF4/0x16C/0x1D8/0x44` sequence.

The target retains a relocation-bearing call from `InitPitchWindow` to the
local `CreateWindow`; preserving that authentic non-inline source shape reduces
the compiled owner from `.text 0x6CC` to `0x5C0` versus target `0x5C4` and
brings relocations to 37/37. `ConstructPitchWindow` is exact.
`ProcessPitchWindow` is `0x148/0x140` at 93.512190%, `ControlPitchWindow` is
exact-size `0xF4` at 99.016396%, `CreateWindow` is exact-size `0x16C` at
76.703300%, and `InitPitchWindow` is `0x1D8/0x1DC` at 96.652540%.

## Board math cluster

`board/math.c` restores 22 functions from `mbVecMagXZ` through
`mbMathDistScale`: vector-distance checks, angle wrapping/interpolation,
look-at and projection helpers, Bezier and Hermite evaluation, and distance
scaling. Matching MP5 `src/board/main.c` supplies the source shapes; MP6
instructions and constants authenticate each retained function, and MP7
provides identical function-size parity.

The default board `-O0,p` output was structurally oversized. The MP6 target
proves the owner-local `-O4,p` override: with it, 19 of the 22 recovered
functions have exact target sizes and the recovered target/source range is
`.text 0xFAC/0xF80`. No function is byte-exact yet. `mbAngleWrap2`,
`mbPos3DtoNorm`, `mbNormPosto3D`, and `mbNormPosto2D` remain absent because no
authenticated sibling source shape was available; the earlier and later owner
closure is also still real work.

## Board telop cluster

`board/telop.c` restores eight compact functions authenticated by the MP5
source shape and MP6/MP7 objects: both main/sub-stick selectors, taunt init and
close, language get and set, board-data number translation, and board-data
directory loading. The retained declarations include the real taunt work bit,
four-entry sound table, two `6 x 2` language maps, six-entry directory table,
and the target-proven directory selection helper. Their declaration order now
matches the target data order.

`mbTauntInit` (`0x80`) is exact. The recovered target/source text range is
`0x640/0x62C`; `mbBoardDataNumGet` is exact-size `0xE0` at 99.285710%, and
`mbBoardDataDirRead` is `0x248/0x240` at 97.534250%. `TauntOMExec` and the
larger telop/time closure remain absent rather than being represented by a
stub, blob, or guessed structure.

## Rejected probes and deferred owner

Object-only probes for `undersam.c`, `statio.c`, and `logexp.c` did not improve
their target instruction shapes and were fully reverted. A GC/2.6 override for
`mtx.c` regressed the nine already-exact functions and was also reverted.
`fft_maye.c` was screened but left absent: MP7 authenticates its binary owner,
not the original Hartley-transform C control flow, so importing a familiar FFT
implementation would be a hypothesis rather than recovery.

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
`original-was-asm` owner. This wave changes the source inventory from 40 to 45
current candidates and reduces `NO-SOURCE` from 73 to 68.
