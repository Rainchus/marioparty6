# Native matching wave 50: GSSDK pitch and user-word dynamic programming

Wave 50 moves five complete Game Speech SDK owners from `NO-SOURCE` to
target-derived C: `dpscruw.c`, `dpgenuw.c`, `pitchdp.c`, `pitchco.c`, and
`shs_vuv.c`. The batch materializes all 58 retained functions and five real
owner layouts. Twenty-one functions are byte-exact, but every complete object
still diverges, so all five owners remain `NonMatching` and fallback-linked.
No inline assembly, byte blob, fake operation, opaque owner buffer, or
register-forcing construct was used.

## Tool and evidence provenance

- DTK: project-pinned `dtk.exe 0.9.2`.
- Objdiff CLI: `3.7.3`.
- Compiler: project-pinned Game Speech SDK `GC/1.2.5n` with floating-point
  contraction disabled.
- Recovery inputs: MP6 target instructions, symbols, relocations, access
  widths, object offsets, callers, neighboring recovered GSSDK owners, and
  linked constants/data.

No sibling or third-party source was transplanted into these owners. Their
source, types, and control flow were recovered directly from the MP6 target.
A bounded repeated-bound probe in `dpscruw.c` regressed the object and was
reverted; no neutral or regressing compiler perturbation was retained.

## User-word score and generation owners

`dpscruw.c` closes a real `0x80` score owner with 12-byte backtrace entries,
language state, Viterbi rows, score/result storage, active-distance work, and
all 14 target functions. The recovered signed dynamic-programming result is
also proven by its caller shape. Seven functions are exact:
`InitBacktrace`, `FreeBacktrace`, `InitViterbi`, `InitDpScoreUw`,
`IdleDpScoreUwProcess`, `DpScoreUwProcess`, and `ConstructDpScoreUw`.

| `dpscruw.c` evidence | Target/source | Result |
| --- | ---: | ---: |
| `.text` | `0x11B4/0x129C` | 73.921450% |
| `.sdata2` | `0x10/0x10` | exact |
| relocations | `60/62` | divergent |

`dpgenuw.c` closes a real `0xF4` generator owner. Its target accesses prove a
22-state lattice, 12-byte backtrace cells, eight-byte state records, chunk and
free-list ownership, language-method inputs, frame/scores, and active
backtrace count. All 12 target functions are recovered. `FreeBacktrace`,
`InitDpGenUw`, `MakeUserWordsActiveDist`, `IdleDpGenUwProcess`, and
`ConstructDpGenUw` are exact; `DpGenUwProcess` is `0x160/0x15C` at
95.602270%.

| `dpgenuw.c` evidence | Target/source | Result |
| --- | ---: | ---: |
| `.text` | `0x1330/0x1484` | divergent |
| `.sdata2` | `0x10/0x10` | exact |
| relocations | `64/66` | divergent |

These two owners replace reserved virtual slots in `LanguageData` with the
target-proven recognition, phenome, speech-unit, user-word, ergodic-state,
and penalty accessors that their indirect calls consume. The names are
authenticated by the retained accessor definitions in `langdata.c`; the slot
offsets and signatures are established by the MP6 call sites.

## Pitch dynamic programming

`pitchdp.c` recovers the complete `0x68` owner and all 11 functions: 12-byte
Viterbi cells, state and jump factors, the dynamic-programming pass,
backtracking, buffered output, post-processing, flush, reset, control,
initialization, and construction. `ConstructPitchDP` (`0x44`) is exact.
`Output` is exact-size `0xFC` at 99.603170%, `InitPitchDP` is
`0x36C/0x368` at 99.086760%, and `ControlPitchDP` is exact-size `0x104` at
88.800000%.

| `pitchdp.c` evidence | Target/source | Result |
| --- | ---: | ---: |
| `.text` | `0x15C0/0x15CC` | about 63.19% target-byte-weighted |
| `.sdata2` | `0x30/0x2C` | 78.260870% |
| relocations | `70/70` | paired |

## Pitch combiner

`pitchco.c` recovers the complete `0xC4` owner and all 13 functions: voiced
boundary detection, energy normalization, logarithmic pitch, buffered output,
state reset, control, and initialization. `MaxFloatInArray`,
`CheckBeginVoicedFlag`, `CheckEndVoicedFlag`, `OutputProcedure`,
`ControlPitchCombiner`, and `ConstructPitchCombiner` are exact. `Status` is
99.875000%, `NormE` is 99.933334%, and `ProcessPitchCombiner` is 99.829930%.

| `pitchco.c` evidence | Target/source | Result |
| --- | ---: | ---: |
| `.text` | `0x13F8/0x13C8` | 96.021910% |
| `.sdata2` | `0x48/0x40` | 91.176470% |
| relocations | `81/81` | paired |

## SHS/VUV analysis

`shs_vuv.c` recovers the complete `0x60` owner and all eight functions. The
source expresses the 65-bin linear spectrum, peak enhancement, cubic-spline
work arrays, five-octave/50-bin logarithmic interpolation, harmonic tables,
subharmonic summation, V/UV threshold, process path, and initialization.
`ControlSHS_VUV` (`0x10C`) and `ConstructSHS_VUV` (`0x44`) are exact.
`ProcessSHS_VUV` is exact-size `0x290` at 91.975610%; `InitSHS_VUV` is
`0x49C/0x4A8` at 88.959320%.

| `shs_vuv.c` evidence | Target/source | Result |
| --- | ---: | ---: |
| `.text` | `0x151C/0x14F0` | divergent |
| `.sdata2` | `0x78/0x74` | 57.627117% |
| relocations | `92/93` | divergent |

## Batched verification discipline

The five owners were recovered in parallel worktrees, while local MWCC/objdiff
execution stayed serialized in two bounded batches to avoid concurrent build
I/O. Each completed worktree and temporary branch was removed immediately
after integration. Owner-level comparisons were collected once; the full
repository gate was reserved for the integrated patch.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts remain 294/895 owners overall and 285/396 DOL owners; these
  five source-recovered owners remain `NonMatching`.
