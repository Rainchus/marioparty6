# Native matching wave 31

Wave 31 batches eight clean-C DOL owner recoveries. No assembly exception is
used, and fallback-linked owners remain outside the decompiled-source total.

## Compiler-family correction

The Game Speech SDK library was incorrectly inheriting the project's GC/2.6
linker version as its compiler. Target-led probes exposed a family-wide
signature: GC/2.6 changed function prologues and used CTR-shaped indirect
calls, while `GC/1.2.5n` reproduced the retained target instructions. The
library now records `GC/1.2.5n` explicitly.

The decision does not rely on the extracted objects' `.comment` sections.
DTK synthesizes that metadata according to the split configuration. It rests
on the emitted owner bytes and the byte-identical final DOL.

## MusyX runtime

Mario Party 5 commit
[`e246f9d`](https://github.com/mariopartyrd/marioparty5/tree/e246f9d9850ff53ac684b971068fbf87fdcf6acb)
provides the authenticated MusyX 2 source family. MP6 target instructions,
relocations, and data closure then establish the version-specific differences.

- `musyx/runtime/snd_init.c` restores the initialization API, `sndActive`,
  `synthIdleWaitActive`, voice/studio limits, ARAM base handling, shutdown,
  playback-info access, and low-pass range forwarding. The MP6 path passes
  `aramGetFirstUserAddress()` into the two-argument `dataInitStack` flow.
- `musyx/runtime/hw_dolphin.c` restores the DSP task callbacks, AI buffer
  state, interrupt/logic handshake, and `salInitDsp(u32 flags)` ABI. Semantic
  target names replace six address labels: `dspInitCallback`,
  `dspResumeCallback`, `dspSlave`, `dspSlaveLength`, `dspCmdFirstSize`, and
  `dspCmdList`.

Both owners are clean C. Their Matching sibling provenance is source-shape
evidence, not an assembly exception.

## Game Speech SDK

Six owners were recovered directly from the MP6 target objects:

| Owner | Recovered source facts |
| --- | --- |
| `gsapi/sid/sid.c` | Eight-function microphone state machine; the allocation size and `sth`/`lhz` accesses prove `SidState { u16 recording; }`. MP7 independently carries the same `0x1C4` instruction stream apart from resolved heap-call displacements. |
| `gsapi/extaudio.c` | `ExtAudioState { u32 active; }`, heap lifecycle, start/stop state, and the `gGSAPI` sample callback at offset `0x20`. |
| `gsapi/mathusage.c` | Zero maps to the local `0xFF800000` negative-infinity bit pattern; nonzero input tail-calls `logf`. The target's extra four `.sdata2` bytes were an inter-owner alignment tail, so the split now ends at the semantic four-byte constant. |
| `common/csspi/csspi.c` | The platform critical-section enter/leave hooks both return success and emit the complete `0x10` owner. |
| `common/osspi/osspi.c` | Read/write pointer checks reject `NULL` with `0x80060183` and otherwise return success. |
| `common/safeh/safeh.c` | Real two-word handle layout (`address`, `type`), null-handle global, type/address validation, read/write checks, and equality. The target boolean materialization proves the small inline error predicate's ternary source shape. |

Each accepted source object produced an empty mismatch list under objdiff. The
retained evidence artifacts are:

- `build/proof_wave31_musyx_snd_init_fixed.json`
- `build/proof_wave31_musyx_hw_dolphin_fixed.json`
- `build/proof_wave31_gssdk_sid.json`
- `build/proof_wave31_gssdk_extaudio_fixed.json`
- `build/proof_wave31_gssdk_mathusage_split.json`
- `build/proof_wave31_gssdk_csspi_fixed.json`
- `build/proof_wave31_gssdk_osspi_fixed.json`
- `build/proof_wave31_gssdk_safeh_inline.json`

## Rejected probes

Near-matches were not promoted or retained as recovered owners:

- `gsapi/wrddata.c`: `WrdDestroyWordList` was exact, but
  `WrdCreateWordList` remained 97.448980% because the source scheduled its
  pointer increment after two loop initializers instead of before them.
- `asrpho/common/ctxdata/ctxdata.c`: all scalar accessors matched, while the
  three packed-layout pointer accessors retained genuine expression-shape
  divergence.
- `asrpho/common/blocks/flfxblks/genfilt.c`: three of four functions matched;
  the constructor retained callback-address register allocation differences.
- `asrpho/common/blocks/flblocks/logexp.c`: its `0x584` LUT has no authentic
  source provenance, so it is not an easy clean-C recovery.

All four owners remain `NonMatching`; no register forcing, data blob, or
fakematch was admitted.

## Final gate

The batch gate was run once at the commit boundary:

```text
rtk ninja -j1
137 files OK

rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
137 files OK

rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
byte-identical (exit 0)
```

The eight owners add `0xA38` linked code bytes. DOL policy totals become 242
clean-C matching owners, 32 matching sibling-exception owners, 121
`C-not-yet-matched` fallbacks, and one `original-was-asm` fallback.
