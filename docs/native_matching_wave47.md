# Native matching wave 47: GSSDK spectral subtraction

Wave 47 recovers all eight retained functions in `specsub.c` as compiled,
target-derived C. `ConstructSpecSub` is byte-exact. The core algorithm and
control/initialization source still differ substantially from the target
compiler shape, so the owner remains `NonMatching` and fallback-linked.

## Tool and evidence provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Compiler: project-pinned Game Speech SDK `GC/1.2.5n`, with floating-point
  contraction disabled as required by retained target instructions.
- Recovery inputs: MP6 target branches, constants, relocations, queue/session
  consumers, and the retained `logf_check`/`expf` call ABI.
- Sibling check: the local MP7 target at commit `f5ea780` has the same owner
  instruction stream apart from linked call, SDA, and function-address
  relocation words.

No public source donor was used. The retained symbols provide the function
names; source behavior and ownership come from the MP6 binary.

## Recovered ownership and layout

The target constructor allocates exactly `0x98` bytes. Field accesses recover
the common block, frame/update counters, spectrum dimensions, history-ring
pointers, five spectrum vectors, smoothing and gain parameters, segment
endpoints, and normalization coefficients. Compile-time assertions protect
the history, reserve, noise-scale, endpoint, normalization, feedback, and
whole-owner boundaries.

`InitSpecSub` allocates one zeroed float arena containing
`(historyLength + 6) * spectrumSize` elements. The first
`(historyLength + 1) * spectrumSize` elements are the history ring; the five
following spectrum-sized regions back noise power/log amplitude, prior and
posterior SNR, and gain state. A second allocation owns `segmentCount` endpoint
indices.

Session get/put prove the dynamic payload:

- frame count, update count, and normalization state;
- four spectrum-sized vectors;
- two history-ring indices;
- the complete `(historyLength + 1) * spectrumSize` ring.

Its byte size is `4 * (((historyLength + 5) * spectrumSize) + 5)`.

No MP6 or MP7 instruction reads or writes the four-byte slot at `0x64`. Both
targets use the proven field at `0x60`, skip to the proven floating field at
`0x68`, and allocate the same `0x98` owner. The source names this storage
`reserved64` without assigning a semantic role; it has no runtime use and is
not a code-generation aid.

The recovered functions implement reset, session transfer, history/noise
updates, posterior/prior SNR tracking, segmented maximum averaging, exponential
normalization, gain calculation, four-output queue processing, control,
profile initialization, and construction. No inline assembly, byte blob, fake
operation, or unproven semantic field is used.

## Batched object evidence

The integrated source was compiled once and compared once without score
tuning.

| Function | Target/source bytes | Raw score |
| --- | ---: | ---: |
| `Reset` | `100/256` | 0.000000% |
| `SpecSubPutSession` | `288/288` | 96.222220% |
| `SpecSubGetSession` | `256/256` | 94.671875% |
| `SpectralSubtraction` | `2128/2680` | 30.109022% |
| `ProcessSpecSub` | `180/184` | 77.000000% |
| `ControlSpecSub` | `440/744` | 9.227273% |
| `InitSpecSub` | `884/1152` | 50.027150% |
| `ConstructSpecSub` | `68/68` | 100% |

Whole target/source `.text` is `0x10F8/0x15FC` at 42.579190%.
Target/source `.sdata2` is `0x40/0x58` at 81.578950%, and target/source text
relocations are 92/102. These differences are documented as substantial real
C matching work; no promotion is attempted.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts remain 293/895 owners overall and 284/396 DOL owners.
