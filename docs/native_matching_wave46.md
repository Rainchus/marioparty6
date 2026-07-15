# Native matching wave 46: GSSDK ACNE block

Wave 46 recovers all eight retained functions in `acne.c` as compiled,
target-derived C. `Reset` and `ConstructAcne` are byte-exact. The other six
functions and complete object still diverge, so the owner remains
`NonMatching` and fallback-linked.

## Tool and evidence provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Compiler: project-pinned Game Speech SDK `GC/1.2.5n`, with floating-point
  contraction disabled as required by retained target instructions.
- Recovery inputs: MP6 target instructions, constants, relocations, queue and
  session consumers, and the already-recovered smoother/log-domain interfaces.
- Sibling check: the local MP7 target at commit `f5ea780` has byte-identical
  `InitAcne`/`ConstructAcne` shape and the same allocation/field boundaries.

No public source donor was used. Names are retained target symbols; source
shape and all admitted behavior come from the MP6 binary.

## Recovered ownership and layout

The target constructor allocates exactly `0x98` bytes. Field accesses recover
the common block, private queue and depth, state flags, three band vectors,
adaptation scalars/counters, two 21-band table pointers, smoother descriptor,
and live smoother object. Compile-time assertions protect offsets `0x7C`,
`0x80`, `0x84`, `0x94`, and the complete size.

The band vectors are not three independent unknown buffers. `InitAcne`
allocates one `3 * bandCount` float region and places `smoothedMean`,
`adaptiveMean`, and `noiseEstimate` at consecutive band-sized offsets. The
session functions prove an exact `12 * bandCount + 40` payload: three fixed
state fields, the three vectors, and a 28-byte scalar tail.

No MP6 instruction reads or writes the four-byte slot at `0x80`. MP7 has the
same `0x98` allocation, uses `0x7C` and then `0x84..0x94`, and likewise never
accesses `0x80`. The source therefore calls the proven four-byte slot
`reserved80` without asserting a semantic role; it has no runtime use and is
not a code-generation aid.

The recovered `0xA8` read-only data consists of the exact 21-float initial-mean
and noise-floor tables. Reset/session, normalization/enqueue, process,
control, initialization, and construction logic are all present. No inline
assembly, byte blob, fake operation, or unproven semantic field is used.

## Batched object evidence

The integrated source was compiled once and compared once without score
tuning.

| Function | Target/source bytes | Raw score |
| --- | ---: | ---: |
| `Reset` | `264/264` | 100% |
| `AcneGetSession` | `208/208` | 93.942310% |
| `AcnePutSession` | `208/208` | 93.942310% |
| `NormalizeAndEnQ` | `368/388` | 76.673910% |
| `ProcessAcne` | `1144/1184` | 78.391610% |
| `ControlAcne` | `316/320` | 60.177216% |
| `InitAcne` | `532/532` | 91.045110% |
| `ConstructAcne` | `68/68` | 100% |

Whole target/source `.text` is `0xC24/0xC64` at 82.891890%.
`.rodata 0xA8` is exact. Target/source `.sdata2` is `0x50/0x4C` at
97.435900%, and target/source text relocations are 95/96. The owner therefore
remains real C work in the `C-not-yet-matched` bucket.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts remain 293/895 owners overall and 284/396 DOL owners.
