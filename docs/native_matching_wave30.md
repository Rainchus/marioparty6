# Native matching wave 30

Wave 30 batches the remaining direct Matching-MP5 library ports that survive
MP6 target proof. It promotes two mixed C/assembly owners under the documented
sibling-authentication exception:

- `TRK_MINNOW_DOLPHIN/dolphin_trk.c`
- `musyx/runtime/snd_service.c`

Neither owner is counted as clean C. Fallback-linked owners remain outside all
decompiled-source totals.

## Authentication

The source authority for both owners is Mario Party 5 commit
[`e246f9d`](https://github.com/mariopartyrd/marioparty5/tree/e246f9d9850ff53ac684b971068fbf87fdcf6acb),
whose `configure.py` marks both files `Matching`:

- [`src/TRK_MINNOW_DOLPHIN/dolphin_trk.c`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/dolphin_trk.c)
- [`src/musyx/runtime/snd_service.c`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/snd_service.c)

The exception applies only after the MP6 object, retained-range, and full
container gates pass. Matching sibling status does not substitute for MP6
proof.

## `dolphin_trk.c`

The retained MP6 owner is exact:

| Section | Target retained size | Result |
| --- | ---: | --- |
| `.init` | `0x104` | `__TRK_reset`, exact |
| `.text` | `0x140` | four retained functions, exact |
| `.data` | `0x40` | `TRK_ISR_OFFSETS` (`0x3C`) plus target split tail |
| `.bss` | `0x8` | `lc_base` (`0x4`) plus target split tail |

The four retained text functions are `InitMetroTRK`, `TRKInitializeTarget`,
`TRKTargetTranslate`, and `EnableMetroTRKInterrupts`. The donor also emits
out-of-line `__TRK_copy_vectors` (`0x104`) and `TRK_copy_vector` (`0x90`). Their
logic is already inlined into the exact `__TRK_reset`; the two out-of-line
copies are absent from the linked MP6 range and are linker-stripped.

The MP5 header supplies the missing `void __TRK_copy_vectors(void)` prototype.
The target split establishes the real local names and sizes for
`TRK_ISR_OFFSETS` and `lc_base`; the remaining four bytes in each extracted
section are alignment, not invented globals.

`InitMetroTRK` is a sibling-authenticated `asm void` routine. Therefore the
whole mixed owner is reported as an assembly exception and contributes no
clean-C owner.

Evidence artifact: `build/proof_wave30_dolphin_trk_named.json`.

## `snd_service.c`

All eight retained functions occupy the target's exact `0x2D4` linked text
range:

- exact C: `sndRand`, `sndSin`, `sndBSearch`, `sndConvertMs`,
  `sndConvertTicks`, and `sndConvert2Ms`
- authenticated assembly: `sndSqrt` and `sndCos`

The target-led data reshape recovers:

| Symbol | Section | Semantic size |
| --- | --- | ---: |
| `sqrtConsts` | `.rodata` | `0xC` |
| `_sinConsts` | `.rodata` | `0x24` |
| `sndSintab` | `.data` | `0x800` |
| `last_rnd` | `.sdata` | `0x4` |
| `i2fMagic` | `.sdata2` | `0x8` |

No source padding array was added. The extracted target carries a `0x10`
`.data` tail and a four-byte `.sdata` tail. The following target `.data`
owner begins at `0x80242C60` with proven 32-byte alignment; recording that
alignment makes the linker reproduce the original inter-owner `0x10` gap.

The reconstructed MP6 math header emits two unused weak inline constants that
are absent from both the target owner and the Matching donor's effective
closure. An owner-local `_MATH_H` guard restores that include boundary without
adding code or data.

Raw objdiff reports `99.96133%` for `.text` because donor `sndSqrt` carries two
additional lower-half relocations. The target encodes those same resolved
immediates relative to its `sqrtConsts + 8` anchor. All retained instructions
and the final linked range are byte-identical; source-object attribution is
not being presented as a raw 100% relocation-count match.

Evidence artifacts:

- `build/proof_wave30_snd_service_mathguard.json`
- `build/proof_wave30_snd_service_named.json`

`sndSqrt` and `sndCos` are sibling-authenticated inline assembly. The owner is
therefore an assembly exception, not clean C.

## Rejected direct ports

The same sweep compiled and rejected the other Matching-MP5 candidates rather
than retaining speculative source:

| Owner | Target/source `.text` | Current evidence |
| --- | ---: | --- |
| `musyx/runtime/seq.c` | `0x3FCC / 0x5168` | `69.73059%`; different implementation and closure |
| `musyx/runtime/hw_dolphin.c` | `0x4F8 / 0x5A4` | `90.73899%`; different function and constant closure |
| `TRK_MINNOW_DOLPHIN/targimpl.c` | `0x1894 / 0x1D4C` | 29/30 retained functions exact; `TRKTargetSupportRequest` remains `97.815384%` |

All rejected probe sources were removed and their owners remain
`NonMatching`. The MP5 board ports were also discarded before admission: the
MP6 targets expose different owner APIs and data closures, so they are not
direct ports.

## Final gate

The batch gate is intentionally run once, at the commit boundary:

```text
rtk ninja -j1
137 files OK

rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
137 files OK

rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
byte-identical (exit 0)
```

The accepted owners add `0x518` linked code bytes and `0x898` configured
data/BSS bytes. DOL policy totals become 234 clean-C matching owners, 32
matching sibling-exception owners, 129 `C-not-yet-matched` fallbacks, and one
`original-was-asm` fallback.
