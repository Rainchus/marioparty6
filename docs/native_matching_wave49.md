# Native matching wave 49: GSSDK gender, log arithmetic, and VQ shape

Wave 49 batches one complete target-derived owner recovery, one clean owner
promotion, and one compiler-shape improvement. All 12 target functions in
`gender.c` now exist as typed C, `logexp.c` is byte-identical and Matching, and
the two search phases in `vq1500.c` now have distinct source lifetimes. No
inline assembly, byte blob, fake operation, or register-forcing construct was
used.

## Tool and evidence provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Compiler: project-pinned Game Speech SDK `GC/1.2.5n` with floating-point
  contraction disabled.
- Recovery inputs: MP6 target instructions, symbols, relocations, access
  widths, object offsets, callers, neighboring GSSDK owners, and linked data.
- MWCC implementation reference: local `mwcc-decomp` commit
  `094b96ca1df4a035b5f93c351f773306c0241f3f`, especially
  `InstrSelection.c::fp_binary_operator` and the floating-point opcode flags in
  `OpcodeInfo.c`.

A bounded donor search across the public MP4, MP5, MP7, and MP8 trees, known
forks, GitHub, and Sourcegraph found no authenticated C donor for `gender.c` or
the remaining small GSSDK leaves. The shibbo MP6 map supplies same-binary names
only, not source. This wave therefore uses direct MP6 reverse engineering.

## `gender.c`: complete target-function recovery

The target accesses close a `0x94` `Gender` owner with its TinyOS base block,
state and gender selection, input/output frame pointers, score array and
weights, a `0x14` code-book descriptor, enabled-gender table, frame-ring
indices and capacities, thresholds, and language owner. Session copies prove a
variable score array followed by a fixed `0x10` tail. The recovered language
calls type the virtual slots at `0x88` (`getpWarpFactors`) and `0xD0`
(`checkBitField`).

The source now covers all 12 target functions: session read/write, code-book
loading, fixed-gender selection, controls, reset, score computation, frame
input/output, initialization, processing, and construction. MP7 binary parity
independently confirms the session and reset access pattern; it is not used as
a source donor.

Object evidence:

| Evidence | Target/source | Result |
| --- | ---: | ---: |
| `.text` | `0x10A8/0x124C` | 86.567540% |
| `.sdata2` | `0x38/0x44` | 71.666664% |
| relocations | `70/72` | structurally close |

`LoadCodeBook` (`0x170`) and `ConstructGender` (`0x44`) are byte-exact.
`GenderGetSession`, `GenderPutSession`, `ResetGender`, `WriteFrameToOutput`,
`InitGender`, and `ProcessGender` are all above 96% raw `.text` similarity.
The main remaining structural difference is helper visibility/order: source
emits an out-of-line `OutputFrame` and changes `ControlGender` inlining. The
owner therefore remains `NonMatching` and fallback-linked.

## `logexp.c`: clean promotion

The lookup helper and its 353-float table were already exact. The remaining
four instruction differences in `LogAdd` came from the expression tree, not
the algorithm. Restoring the natural source spelling `difference / 2.0`
instead of the algebraically rewritten `difference * 0.5` gives MWCC the
target operand order in both branches.

The final object has exact `.text 0x1C8`, exact `.sdata2 0x48`, and 31/31 text
relocations. `HLnOnePlusExpHFloat` (`0x90`) and `LogAdd` (`0x138`) are both
byte-exact. Objdiff reports semantic `.rodata` bytes exact; target `0x588`
versus source `0x584` is only the target object's four-byte alignment tail.
The linked owner passes the full gate and is now `Matching`.

## `vq1500.c`: two-pass lifetime recovery

The first and second nearest-vector searches have separate compound scopes and
pointer identities while keeping the shared label and loop state outside both
passes. This matches the distinct semantic lifetimes and permits the target's
register reuse without dummy variables, `register`, `volatile`, or declaration
shuffling. The second-code-book offset now retains its semantic source order,
`secondSearchCount * label`.

Eight of nine functions remain exact. `GetLabel` improves from 98.109760% to
99.512190% at the exact target size `0x148`; only six instructions differ, all
the second pass's `i`/`indices` register coloring. Target/source owner `.text`
remain `0x4F0/0x4F0` with 25/25 text relocations. The owner remains
`NonMatching` because register coloring alone is not evidence for another
source rewrite.

## Batched near-match triage

The same object batch covered `delaybl.c`, `undersam.c`, `genfilt.c`,
`statio.c`, `msmstream.c`, `board/audio.c`, and `board/branch.c`. Their
remaining differences are isolated register allocation, scheduling, or larger
constructor/control shapes. Bounded semantic probes either regressed or were
neutral: changing the undersampler output count width, narrowing stationarity
extrema lifetimes, removing filter casts, using an aggregate initializer, and
redeclaration/declaration-order variants in VQ. Those probes were reverted;
none was admitted merely to perturb code generation.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts: 294/895 owners overall and 285/396 DOL owners.
