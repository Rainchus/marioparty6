# Native matching wave 45: TriggerLR speech trigger

Wave 45 recovers all five retained functions in `trigglr.c` as compiled,
target-derived C. The public constructor is byte-exact. The other four
functions and complete object still diverge, so the owner remains
`NonMatching` and fallback-linked.

## Tool and evidence provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Compiler: project-pinned Game Speech SDK `GC/1.2.5n`, with floating-point
  contraction disabled as required by retained target instructions.
- Recovery inputs: MP6 target instructions, branches, constants, relocations,
  queue calls, session-data consumers, and the shared typed `TriggerLR` fields.

No public source donor was found. Names are retained MP6 symbols; source shape
and semantics come from the MP6 binary and the already-recovered histogram,
voicing, and subsampler interfaces.

## Recovered behavior

`TriggerLR_FindSpeech` now represents the full three-state trigger search. It
maintains histogram and voicing criteria, energetic-frame runs, activity and
peak thresholds, lookback, silence timing, and trigger/end events. Every state
write is tied to a target offset in the proven `0x19C` structure.

`ProcessTriggerLR` performs subsampling, histogram updates, voicing input, and
trigger dispatch. `ControlTriggerLR` implements reset/start/hold queue modes,
sensitivity changes, session sizing/get/put, and destruction. The reset/hold
path deliberately evaluates all three `qQueueControl` calls and ORs their
results; short-circuit `||` was rejected because the target always issues all
three calls. `InitTriggerLR` recovers the profile defaults and mappings, and
`ConstructTriggerLR` closes the owner with the exact common-block constructor.

No inline assembly, byte blob, padding object, or fake operation is present.

## Batched object evidence

The final integrated source was compiled once after the all-three-queue-call
correction and compared once without match-score tuning.

| Function | Target/source bytes | Raw score |
| --- | ---: | ---: |
| `TriggerLR_FindSpeech` | `1764/1720` | 85.267570% |
| `ProcessTriggerLR` | `428/408` | 86.158880% |
| `ControlTriggerLR` | `1072/1072` | 65.037315% |
| `InitTriggerLR` | `960/952` | 87.937500% |
| `ConstructTriggerLR` | `68/68` | 100% |

Whole target/source `.text` is `0x10C4/0x107C` at 81.134200%.
Target/source `.sdata2` is `0x40/0x3C` at 96.774190%; source text has 97
relocations versus 100 in the target. The complete object therefore remains
real C work in the `C-not-yet-matched` bucket.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts remain 293/895 owners overall and 284/396 DOL owners.
