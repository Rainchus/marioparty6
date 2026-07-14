# Easy-port wave evidence

This ledger records the evidence used for the consolidated native-recovery
wave verified on 2026-07-14. It is deliberately narrower than a provenance
inventory: only source admitted as `Matching`, inline-assembly exceptions, and
semantic symbol changes retained by this wave are listed.

## Frozen references

| Key | Project | Commit | Use |
| --- | --- | --- | --- |
| M4 | [Mario Party 4](https://github.com/mariopartyrd/marioparty4) | `147b165a83187ac9e6cfdc3bf52f2e73437b1ffd` | Matching Game, Dolphin, Runtime, MSL, and TRK owners |
| M5 | [Mario Party 5](https://github.com/mariopartyrd/marioparty5) | `e246f9d9850ff53ac684b971068fbf87fdcf6acb` | Matching MusyX owners and Game corroboration |
| XD | [Pokemon XD](https://github.com/TeamOrre/xd-decomp) | `017351246c9a48fd8e7f68bbf600652d413752f3` | Matching matrix, vector, and quaternion inline bodies |
| MKDD | [Mario Kart: Double Dash](https://github.com/doldecomp/mkdd) | `8a91a6b4a32834dfb689fbdcf1dea8796be9c75f` | Matching `mtx44vec` owner |
| P2 | [Pikmin 2](https://github.com/projectPiki/pikmin2) | `46aecad6f060c303a735e6b3850c2a212c846b27` | Matching `GXLight` owner |

The consolidated wave was based on `fork/rel-quickwins` commit
`1a464ae4644077fb956c879dd5d2c710d4f12d52`, whose `fork/main` ancestor was
`353fa30f9e489d867966223e87aa84d286eafcfc`; the resulting `fork/main`
snapshot before the kerent policy correction is `685f514`. The exhaustive
source audit also inspected the local `mwcc-decomp` reference at
`094b96ca1df4a035b5f93c351f773306c0241f3f`; it supplied no additional
directly admissible owner in this wave.

## Target proof gate

`build/GP6E01/obj/...` is the DTK-split target object and
`build/GP6E01/src/...` is the object compiled from recovered source. An owner
is admitted only when the source object is selected by `configure.py`, its
retained instructions and effective relocations match the target, its linked
range is exact, and the final container hash passes. The final serial build
reported `137 files OK`; all 137 entries in `config/GP6E01/build.sha1` pass,
and `main.dol` has SHA-1
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`.

Useful independent views of the proof are:

```text
python configure.py
ninja -j1
build/tools/dtk.exe shasum -c config/GP6E01/build.sha1 -q
powerpc-eabi-objdump -dr build/GP6E01/obj/<owner>.o
powerpc-eabi-objdump -dr build/GP6E01/src/<owner>.o
powerpc-eabi-objdump -d -t build/GP6E01/main.elf
```

## Assembly admission

The user-authorized exception is applied at the owner and corresponding-body
level: an external sibling must configure the owner as Matching and carry the
same inline or standalone assembly operation/routine. Target-specific register
allocation or instruction differences are accepted only when the compiled MP6
source passes
the target object and linked-container gate above.

| MP6 owner | Matching sibling source |
| --- | --- |
| `src/dolphin/base/PPCArch.c` | M4 `src/dolphin/PPCArch.c` |
| `src/dolphin/os/OSAlarm.c` | M4 `src/dolphin/os/OSAlarm.c` |
| `src/dolphin/os/OSCache.c` | M4 `src/dolphin/os/OSCache.c` |
| `src/dolphin/os/OSContext.c` | M4 `src/dolphin/os/OSContext.c` |
| `src/dolphin/os/OSInterrupt.c` | M4 `src/dolphin/os/OSInterrupt.c` |
| `src/dolphin/os/OSSync.c` | M4 `src/dolphin/os/OSSync.c` |
| `src/dolphin/os/OSTime.c` | M4 `src/dolphin/os/OSTime.c` |
| `src/dolphin/os/__ppc_eabi_init.c` | M4 `src/dolphin/os/__ppc_eabi_init.c` |
| `src/dolphin/db/db.c` | M4 `src/dolphin/db.c` |
| `src/dolphin/mtx/mtx.c` | M4 `src/dolphin/mtx/mtx.c`; XD `src/dolphin/mtx/mtx.c` |
| `src/dolphin/mtx/mtxvec.c` | M4 `src/dolphin/mtx/mtxvec.c` |
| `src/dolphin/mtx/mtx44vec.c` | MKDD `libs/dolphin/mtx/mtx44vec.c` |
| `src/dolphin/mtx/vec.c` | M4 `src/dolphin/mtx/vec.c`; XD `src/dolphin/mtx/vec.c` |
| `src/dolphin/mtx/quat.c` | M4 `src/dolphin/mtx/quat.c`; XD `src/dolphin/mtx/quat.c` |
| `src/dolphin/mtx/psmtx.c` | M4 `src/dolphin/mtx/psmtx.c` |
| `src/dolphin/ai/ai.c` | M4 `src/dolphin/ai.c` |
| `src/dolphin/thp/THPDec.c` | M4 `src/dolphin/thp/THPDec.c` |
| `src/game/kerent.c` | M4 `src/game/kerent.c` |
| `src/game/jmp.c` | M4 `src/game/jmp.c` |
| `src/game/malloc.c` | M4 `src/game/malloc.c`; M5 `src/game/malloc.c` |
| `src/Runtime.PPCEABI.H/runtime.c` | M4 `src/Runtime.PPCEABI.H/runtime.c` |
| `src/Runtime.PPCEABI.H/__init_cpp_exceptions.cpp` | M4 `src/Runtime.PPCEABI.H/__init_cpp_exceptions.cpp` |
| `src/TRK_MINNOW_DOLPHIN/dolphin_trk_glue.c` | M4 `src/TRK_MINNOW_DOLPHIN/dolphin_trk_glue.c` |
| `src/TRK_MINNOW_DOLPHIN/flush_cache.c` | M4 `src/TRK_MINNOW_DOLPHIN/flush_cache.c` |
| `src/TRK_MINNOW_DOLPHIN/mpc_7xx_603e.c` | M4 `src/TRK_MINNOW_DOLPHIN/mpc_7xx_603e.c` |
| `src/TRK_MINNOW_DOLPHIN/targsupp.s` | M5 `src/TRK_MINNOW_DOLPHIN/targsupp.s`; identical M4 source |
| `src/dolphin/gx/GXLight.c` | P2 `src/Dolphin/gx/GXLight.c` |

The following text-level differences received a second block-level audit. They
are not inferred C or locally invented assembly:

| MP6 block(s) | Corresponding Matching-sibling body | Difference proved by the MP6 object gate |
| --- | --- | --- |
| `PSMTXRotTrig`, `__PSMTXRotAxisRadInternal` | XD `mtx.c` functions of the same names | MP6 separates two `frsp` operations and uses its target register allocation |
| `PSQUATSubtract`, `PSQUATScale`, `PSQUATDotProduct`, `PSQUATInverse` | XD `quat.c` functions of the same names | Explicit registers replace sibling symbolic operands; mnemonic order and paired-single operations correspond |
| `PSVECMag`, `PSVECDistance` | XD `vec.c` functions of the same names | MP6 block boundaries and register spellings follow the target emission |
| `DCInvalidateRange`, `DCFlushRange`, `DCStoreRange`, `DCFlushRangeNoSync`, `DCStoreRangeNoSync`, `DCZeroRange`, `ICInvalidateRange`, `LCQueueWait` | M4 `OSCache.c` functions of the same names | ABI parameter registers replace sibling symbolic operands |
| `OSLoadContext` | M4 `OSContext.c` function of the same name | Target labels and operand aliases differ textually |
| `OSDisableInterrupts` | M4 `OSInterrupt.c` function of the same name | Target labels differ textually |
| `__init_hardware` | M4 `__ppc_eabi_init.c` function of the same name | Target operand spelling differs textually |
| `HuMemDirectRealloc`, `HuMemDirectFree`, `HuMemDirectFreeNum` | M4/M5 Matching `malloc.c` owners use the same `mflr retaddr` inline operation; the latter two functions occur directly | `HuMemDirectRealloc` is MP6-only and repeats that authenticated one-instruction operation |

Ten complete staged C/header blobs are also byte-identical to M4: `OSFastCast.h`,
`runtime.c`, `ctype.c`, `dolphin_trk_glue.c`, `flush_cache.c`,
`mpc_7xx_603e.c`, `db.c`, `mtxvec.c`, `OSSync.c`, and `jmp.c`. The inherited
`OSFastCast.h` `//HACK` comment is therefore provenance, not a local
match-shaping change.

`game/kerent.c` is a structural sibling exception rather than a byte-identical
donor blob. M4 configures the owner `MatchingFor(USA,PAL)` and carries the same
single `asm void _kerent`/`nofralloc`/`entry`-plus-branch source shape. The MP6
owner restores its target-specific 2,472 branches from historical blob
`bd5b7b9448a659e186d733e3bc3e13dc8291d6a2`; target and source are both
`.text 0x26A0` with 2,472 `R_PPC_REL24` relocations and pass the full linked
container gate. This owner is never counted as clean C.

`TRK_MINNOW_DOLPHIN/targsupp.s` is the exact M5/M4 donor blob
`0244131bd8219c6f5839ae2cda9254c2f28e005c`. Its four `twui; blr` functions
fill the MP6 `.text 0x20` owner exactly. It is admitted only as authentic
standalone assembly and is not counted as clean C.

## Semantic symbol ledger

M5 configures `snd_synthapi.c`, `synth_adsr.c`, and `synth_vsamples.c` as
Matching. M4 configures `ctype.c` and `dolphin_trk_glue.c` as Matching. The
table below adds target-side evidence: a definition offset in an exact split
owner or an effective relocation in the DTK target object. Relocation offsets
are section-relative.

The donor side is independently pinned by translation-unit-relative layout.
M5 places `streamOutputModeChanged` at `stream.c` `.text+0xF38`,
`salChangeADSRState` at `synth_adsr.c` `.text+0x58`, `vsUpdateBuffer` at
`synth_vsamples.c` `.text+0x3AC`, and `vs` at `.bss+0`; all four offsets are
identical in MP6. Its `synth.c` layout places the five retained BSS names at
`+0xC14/+0xC34/+0xC54/+0xC74/+0xC94` and the six retained SBSS names at
`+0x4/+0xC/+0x14/+0x1C/+0x2C/+0x30`, again identical to the MP6 split. The
staged M4 `ctype.c` and `dolphin_trk_glue.c` blobs have Git blob IDs
`0a0ce04b75344da3f68988d184d16577cb05fe1d` and
`46eb11076a7930839d1461e7408d2a116f0706a1`; their data/BSS definition offsets
are therefore direct donor evidence rather than names inferred from MP6.
The staged `synth_adsr.c` and `synth_vsamples.c` blobs likewise equal their M5
donors at `2e8b1cbd769783b97420ef3ac2038ecb0e735d36` and
`217a6fd971e5619834aa7bb5136f93830e73c0d1`.

| Symbol | MP6 address / size | Target-side proof |
| --- | --- | --- |
| `streamOutputModeChanged` | `0x800D342C` / `0x130` | `snd_synthapi.o` `.text+0x438`, `R_PPC_REL24`; linked call at `0x800D2384` resolves to this address |
| `salChangeADSRState` | `0x800DED0C` / `0x26C` | exact `synth_adsr.o` definition at `.text+0x58` from split start `0x800DECB4`; target self-call relocations at `+0x2D8/+0x62C/+0x708` |
| `vsUpdateBuffer` | `0x800DF820` / `0x1F8` | exact `synth_vsamples.o` definition at `.text+0x3AC` from split start `0x800DF474`; target calls at `+0x66C/+0x698` |
| `vs` | `0x802A5830` / `0x950` | exact `synth_vsamples.o` definition at `.bss+0`; the target split is exactly `0x950` bytes and carries 40 address relocations |
| `synthITDDefault` | `0x80297FC4` / `0x10` | `snd_synthapi.o` `.text+0x12E/+0x136`, `R_PPC_ADDR16_HA/LO` |
| `synthFlags` | `0x802C0AE4` / `0x4` | eight `R_PPC_EMB_SDA21` relocations in target `snd_synthapi.o`, first at `.text+0x378` |
| `synthVoice` | `0x802C0AE8` / `0x4` | `snd_synthapi.o` `.text+0x400`, `R_PPC_EMB_SDA21` |
| `synthAuxAMIDI` | `0x802C0AD4` / `0x8` | two `R_PPC_EMB_SDA21` relocations in `snd_synthapi.o`, first at `.text+0x49C` |
| `synthAuxAMIDISet` | `0x802C0ACC` / `0x8` | `snd_synthapi.o` `.text+0x4BC`, `R_PPC_EMB_SDA21` |
| `synthAuxACallback` | `0x80297F64` / `0x20` | four `R_PPC_ADDR16_HA/LO` relocations in `snd_synthapi.o`, first at `.text+0x4B6` |
| `synthAuxAUser` | `0x80297F44` / `0x20` | `snd_synthapi.o` `.text+0x4BA/+0x4CA`, `R_PPC_ADDR16_HA/LO` |
| `synthAuxBMIDI` | `0x802C0AC4` / `0x8` | two `R_PPC_EMB_SDA21` relocations in `snd_synthapi.o`, first at `.text+0x510` |
| `synthAuxBMIDISet` | `0x802C0ABC` / `0x8` | `snd_synthapi.o` `.text+0x530`, `R_PPC_EMB_SDA21` |
| `synthAuxBCallback` | `0x80297FA4` / `0x20` | four `R_PPC_ADDR16_HA/LO` relocations in `snd_synthapi.o`, first at `.text+0x52A` |
| `synthAuxBUser` | `0x80297F84` / `0x20` | `snd_synthapi.o` `.text+0x52E/+0x53E`, `R_PPC_ADDR16_HA/LO` |
| `__ctype_map` | `0x802456D0` / `0x100` | exact `ctype.o` definition at `.data+0`; target references at `.text+0x26/+0x2E` and split data match the M4 table |
| `__lower_map` | `0x802457D0` / `0x100` | exact `ctype.o` definition at `.data+0x100`; target references at `.text+0x12/+0x1A` and split data match the M4 table |
| `gWriteBuf` | `0x802B4D54` / `0x110A` | exact `dolphin_trk_glue.o` definition at `.bss+0x111C`; six target HA/LO relocations reference it, and the split ends with two bytes of unnamed alignment |

Five names explored with the rejected `snd_service.c` candidate are
intentionally not promoted: the addresses at `0x80216548`, `0x80216554`,
`0x80242450`, `0x802BFB18`, and `0x802C2260` retain generic labels. Their
owner remains `NonMatching`, so sibling naming alone is not treated as MP6
proof.
