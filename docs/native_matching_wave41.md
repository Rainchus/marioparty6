# Native matching wave 41

Wave 41 materializes the remaining ordinary MusyX runtime source in one batch:
11,033 lines and 461 compiled functions across thirteen owners. One serial
compile and one thirteen-owner DTK/objdiff pass pair 212 functions with retained
MP6 targets. Of those, 140 are byte-exact and cover `0xB940` target text bytes;
all paired functions cover `0x139DC` target text bytes.

No owner is promoted. Every owner retains a divergent or unpaired closure and
therefore stays `NonMatching` and fallback-linked. Exact helper functions are
reported as recovery evidence, not as full-owner matches.

## Source provenance

The primary donor is Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb`. Its MusyX configuration is the
same `2.0.4` version used by MP6, and its complete `include/musyx/` directory is
byte-identical to MP6. MP5 configures `seq.c` as `Matching`; the other donor
owners remain `NonMatching`, so their source is not treated as proof by itself.

Mario Party 4 commit `147b165a83187ac9e6cfdc3bf52f2e73437b1ffd`
independently configures all fourteen corresponding owners as
`MatchingFor(USA,PAL)` and pins upstream MusyX commit
`adc8df9a959f1e37f71bdf3155e229f9f87ad166`. MP4 uses an earlier conditional
MusyX version, so this authenticates the common source family and the
`StdReverb` assembly bodies without claiming that every MP6 conditional branch
matches. MP6 target instructions, relocations, symbol ranges, and section data
remain authoritative.

One donor placeholder was rejected: MP5's `salBuildCommandList` is empty while
the MP6 target is a real `0x255C` function. The empty definition is absent from
the recovered `hw_dspctrl.c`. The remaining `dsp_import.c` donor is a DSP
firmware byte array rather than clean C recovery, so it also remains absent and
fallback-owned.

The only compile adaptation is a local 32-bit `uintptr_t` alias in `seq.c`.
The donor included an unavailable MSL `stdint.h`; the ARR macros and 32-bit
PowerPC ABI establish the alias without changing emitted code. The local MWCC
decomp reference was checked for the missing MSL header and does not carry it.

## Recovered state

The source restores semantic MusyX ownership rather than anonymous storage:

- `seq.c` owns 256 notes, eight sequence instances, the MIDI-priority matrix,
  public-ID lists, and active/paused/free sequence roots. Its `.data 0x50`,
  `.bss 0xD940`, and `.sbss 0x20` sizes match the target exactly.
- `stream.c` restores 64 `STREAM_INFO` records and the complete target
  `.bss 0x1900`.
- `synth.c`, `synthdata.c`, `synthmacros.c`, and `synthvoice.c` restore the
  job queue, controller destinations, voices, macro tables, sample/keymap/
  layer/curve registries, priority lists, and time queues. `synthmacros.c`
  reproduces target `.data 0x448` and `.bss 0x20`; `synthvoice.c` reproduces
  target `.bss 0xFC0`.
- `snd_midictrl.c`, `snd3d.c`, `hardware.c`, `hw_dspctrl.c`, and
  `hw_aramdma.c` restore MIDI controller state, emitters/listeners, studio and
  voice control, ARAM jobs, stream buffers, and hardware callback ownership.
  `hardware.c` reproduces target `.rodata 0x100`.
- `StdReverb/reverb.c` restores the typed delay-line/reverb state and exact
  `.data 0x20`. Its two inline-assembly routines are authenticated by the
  Matching MP4 sibling, but remain assembly and are excluded from clean-C
  recovery totals.

## Batched object results

All comparisons use objdiff v3.7.3 with target on the left, source on the
right, and `functionRelocDiffs=data_value`. DTK is the project-pinned 0.9.2
build `4d039140f2d2ed80572b1949b76a5ff9b3094e06`.

| Owner | Target/source `.text` | Mapped/exact functions | Remaining object evidence |
| --- | ---: | ---: | --- |
| `musyx/runtime/seq.c` | `0x3FCC/0x5168` | 12/11 | `seqStartPlay` is 99.971054%; donor-only/unpaired helpers and extra `.sdata2` remain. |
| `musyx/runtime/synth.c` | `0x349C/0x433C` | 22/20 | `synthStartSound` is 98.035710% and `synthFXStart` is 90.094340%; the source BSS is four bytes shorter. |
| `musyx/runtime/stream.c` | `0x3C04/0x4728` | 14/12 | `streamHandle` is 99.594600% and `sndStreamFrq` is 71.000000%; `.bss 0x1900` is exact. |
| `musyx/runtime/synthdata.c` | `0x1D98/0x1D90` | 22/20 | Sample-reference add/remove are 95.818184%/95.128204%; five comparison helpers are unpaired. |
| `musyx/runtime/synthmacros.c` | `0x4FA0/0x619C` | 11/8 | `varGet`, `macHandleActive`, and `macStart` are 97.607410%-99.423080%; the large macro-command closure remains unpaired. |
| `musyx/runtime/synthvoice.c` | `0x1D64/0x1F98` | 18/16 | `voiceAllocate` does not align and `voiceBlock` is 51.884956%; seven list/priority helpers are unpaired. |
| `musyx/runtime/s_data.c` | `0xBF0/0x1584` | 5/0 | The five mapped stack/group/sequence functions range from 14.687500% to 90.250000%; registry helpers remain unpaired. |
| `musyx/runtime/hw_dspctrl.c` | `0x35D4/0x12A4` | 8/3 | Five mapped lifecycle/voice/aux functions are 99.797470%-99.875000%; the real `salBuildCommandList` remains absent. |
| `musyx/runtime/snd3d.c` | `0x2238/0x360C` | 13/2 | Eleven mapped emitter/listener handlers diverge; room/door and 3D setup closures remain unpaired. |
| `musyx/runtime/snd_midictrl.c` | `0x2300/0x1E00` | 32/23 | Nine RPN/controller/default functions diverge; semantic `.data` aligns and `.rodata` is 99.631000%. |
| `musyx/runtime/hardware.c` | `0x1094/0x1130` | 41/15 | Twenty-six mapped hardware wrappers remain 93.775510%-99.829270%; target/source `.rodata` are both `0x100`. |
| `musyx/runtime/hw_aramdma.c` | `0xCE0/0xCF4` | 11/7 | Init/store/remove/stream allocation are 73.500000%-98.594600%; queue/setup helpers remain unpaired. |
| `musyx/runtime/StdReverb/reverb.c` | `0xD38/0x1058` | 3/3 | Create/callback/free are exact. Two target functions remain address placeholders; donor `DoCrossTalk` is `0x190` versus the first placeholder's `0x184`, so no symbol rename or promotion is claimed. |

The objdiff artifacts are retained under `build/GP6E01/` as
`wave41_musyx_<owner>.json`.

## Deliberate boundaries

- No target placeholder was semantically renamed merely to increase pairing.
- Divergent functions remain authentic source candidates, not Matching claims.
- `salBuildCommandList` is omitted instead of represented by the donor's false
  empty body.
- `dsp_import.c` remains a binary-data owner and is not counted as clean C.
- The authenticated `StdReverb` inline assembly is retained under the sibling
  exception, but the owner remains fallback-linked and exception-bearing.

## Full gate

- `ninja -j1`: `137 files OK`.
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`: `137 files OK`.
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical.

DTK remains at 292/895 Matching owners overall and 283/396 in the DOL. The
thirteen owners move from `NO-SOURCE` to `SRC-DIVERGES`; no Matching total is
changed.
