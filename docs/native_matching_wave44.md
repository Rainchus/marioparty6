# Native matching wave 44: GSSDK smoother and mel blocks

Wave 44 recovers the complete `smoother.c` and `mel.c` source owners from MP6
target instructions, relocations, calls, field accesses, and shared GSSDK ABI.
All eight retained functions compile and pair. Four are byte-exact; neither
whole object is exact, so both remain `NonMatching` and fallback-linked.

## Tool and evidence provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Compiler: project-pinned Game Speech SDK `GC/1.2.5n`, with floating-point
  contraction disabled as required by retained target instructions.
- Recovery inputs: MP6 target function boundaries, instructions, constants,
  relocations, callees, data flow, and cross-owner matrix/queue consumers.

No public source donor was available for either owner. Names came from the
retained MP6 symbols; implementation shape and every admitted field came from
the MP6 binary and already-proven shared GSSDK interfaces.

## Recovered ownership and ABI

`smoother.h` recovers the real `0x20` `Smoother` state: six floating-matrix
objects, one integer active-column matrix, and the rejection threshold. Its
`0x18` static-info descriptor holds the spline, QR-spline, and beta matrix
views. The three functions now express construction and association of those
matrices, owned destruction, iterative active-column rejection, QR column
deletion, and final spline-coefficient evaluation.

`mel.c` recovers the complete `0x40` block. The target establishes the band
count, per-band weight arrays, first-bin and bin-count arrays, and sample-scale
field after the common `TosBaseBlock`. Its five functions cover queued filter
processing, control/destruction, mel-band construction, profile-driven
initialization, and the public constructor. The target calls `powf`; using the
double-precision `pow` declaration was rejected because it emitted a different
call ABI.

## Batched object evidence

| Function | Target/source bytes | Raw score |
| --- | ---: | ---: |
| `smtConstruct` | `684/684` | 100% |
| `smtDestruct` | `144/144` | 100% |
| `Smoothing` | `856/860` | 75.154205% |
| `ProcessMel` | `440/440` | 91.681816% |
| `ControlMel` | `344/392` | 72.534880% |
| `MelInitBands` | `1284/1288` | 93.875390% |
| `InitMel` | `108/108` | 100% |
| `ConstructMel` | `68/68` | 100% |

Whole `smoother.c` target/source `.text` is `0x694/0x698` at 87.370544%; its
target/source `.sdata2` is `0x18/0x14` at 90.909096%. Whole `mel.c`
target/source `.text` is `0x8C4/0x8F8` at 90.654190%; its target/source
`.sdata2` is `0x60/0x68` at 96.000000%.

The exact lifecycle functions do not justify owner promotion while the core
algorithms still differ. Both owners therefore remain real C work in the
`C-not-yet-matched` bucket.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts remain 293/895 owners overall and 284/396 DOL owners.
