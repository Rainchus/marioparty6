# Mario Party 6 recovery status

This is an evidence snapshot, not a completion claim. It was last verified on
2026-07-14 with the pinned project toolchain.

## Verification

- `ninja -j1`: `137 files OK` (the DOL and all configured REL outputs)
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- `build/GP6E01/main.dol` SHA-1:
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`
- DTK progress at that build: 8.85% code and 27.85% data overall; 45.21% code
  and 65.51% data in the DOL
- Matching owners at that build: 289 of 895 overall, 280 of 396 in the DOL,
  and 9 of 499 in the REL modules
- DOL policy split: 248 matching owners without the assembly exception, 32
  matching owners admitted under the sibling-authentication exception, 115
  `C-not-yet-matched` fallback owners, and 1 `original-was-asm` fallback owner
  awaiting target proof. These four numbers total all 396 DOL owners.

The current build also corrects the Game Speech SDK library from the global
GC/2.6 default to `GC/1.2.5n` and disables floating-point contraction for the
library. GC/2.6 produced systematic prologue and indirect-call instruction
divergence, while the numeric target objects use separate multiply/add
instructions where the default compiler contracts them. The corrected flags
reproduce the retained code and enabled nine clean-C owner promotions:
`gsapi/sid/sid.c`, `gsapi/extaudio.c`, `gsapi/mathusage.c`,
`common/csspi/csspi.c`, `common/osspi/osspi.c`, and
`common/safeh/safeh.c`, plus `gsapi/wrddata.c`,
`asrpho/common/fastallo/fastallo.c`, and
`asrpho/common/blocks/flblocks/spline.c`. This decision is based on emitted
bytes and the final container proof, not synthesized `.comment` metadata.

`gssdk_lib/common/rsrc/rsrc.c` is now an additional Matching clean-C owner.
Its callers prove that `SafeHandle` is an eight-byte aggregate passed by
value; correcting that shared ABI preserves the already-exact `safeh.c` and
allows the real `0x14` `Resource` layout, null resource, open/check/close
logic, all three retained functions, and all effective relocations to match.
The old `0x18` `.rodata` split incorrectly attributed a four-byte alignment
gap to the resource object; the proven semantic owner ends at `0x8021A4B4`.

Two additional Game Speech SDK owners are now Matching clean C.
`asrpho/common/blocks/flfxblks/lkahead.c` recovers the complete `0x4C`
look-ahead block, queue/flush behavior, history ring, and all four functions;
its `.text 0x2AC` and `.sdata2 0x8` match exactly. `asrpho/rec1600/initial.c`
recovers 45 typed profile key/value tables plus the typed `IniFile`,
`InitialProcs`, `QueueWriters`, and `QueueReaders` tables. Its `.rodata 0x9C8`
and all 102 relocations match exactly. MP7 binary parity was used only to
cross-check the table boundaries; the MP6 target symbols, bytes,
relocations, and consumers establish the retained definitions.
The ABI and per-owner evidence is retained in
[`docs/native_matching_wave34.md`](docs/native_matching_wave34.md).

The exact build result includes extracted original objects and explicit
standalone assembly fallbacks for owners that are not yet byte-identical C.
Those owners remain `NonMatching` in `configure.py`; fallback-linked code is
never counted as decompiled source. An exception-bearing matching owner is
reported separately from clean C; admitting authentic assembly does not turn
that assembly into decompiled C.

## `game/` ownership

The `src/game/` path is fully matched: all 57 owners are configured Matching,
54 as clean C and `game/kerent.c`, `game/jmp.c`, and `game/malloc.c` under
authenticated assembly exceptions. It is therefore not 100% clean C. The
broader `Game` library is 60 of 61 owners Matching because it also owns the
still-fallback-linked `board/board.c`.

`game/mic.c` is matching and is linked from recovered C.

## Assembly fallback boundary

Assembly is never admitted merely because a C transcription did not match.
Inline or standalone assembly is admitted only when an authenticated sibling
project marks the same owner `Matching`, carries the corresponding instruction
body/source shape, and the MP6 compile, object-byte, effective-relocation,
linked-range, and container gates all pass. The current tree has 32 matching
exception owners:

- 17 Dolphin SDK owners authenticated by Matching Mario Party 4 sources, with
  `mtx44vec` independently authenticated by Matching Mario Kart: Double Dash
- `game/kerent.c`, `game/jmp.c`, and `game/malloc.c`, plus the authenticated
  `OSFastCast` closure used by matching Game/Board consumers
- the DOL Runtime owner, `__init_cpp_exceptions`, authenticated Runtime
  `ptmf`, and five low-level TRK owners, including `dolphin_trk`
- the existing `GXLight` owner, whose paired-single `PushLight` helper is from
  Matching Pikmin 2 commit `46aecad6`
- MusyX `CheapReverb/creverb.c`, `Chorus/chorus_fx.c`, and `snd_service.c`,
  whose five retained assembly routines are authenticated by Matching Mario
  Party 5 sources

Commit-, path-, block-, and target-object-level evidence for the established
exception set is retained in
[`docs/easy_ports_wave.md`](docs/easy_ports_wave.md); the two subsequent
MusyX decisions are retained in
[`docs/native_matching_wave22.md`](docs/native_matching_wave22.md).

NonMatching owners still link extracted original objects or explicit
standalone `.s` fallbacks and are not counted as decompiled source. Promotion
requires matching retained instructions and effective relocations plus an
exact linked owner range and container; a container checksum by itself is not
sufficient. Linker-stripped donor helpers, `R_PPC_NONE` annotations, and split
padding are accepted only when documented by the object comparison and absent
from the target linked range.

### Two-bucket fallback taxonomy

Every current DOL fallback owner is in exactly one bucket:

- `original-was-asm`: the complete authentic owner was assembly. It is a
  policy/proof decision, recoverable only through the sibling-authentication
  exception above, and can never be counted as decompiled C.
- `C-not-yet-matched`: the owner is C or has a real C portion. Missing source,
  mixed C/assembly, or a divergent compiled object remains genuine
  decompilation work; an assembly substitute is not an acceptable escape.

The reason tags record why an owner is in its current state. `ASM-GATE-PENDING`
means the sibling authenticates assembly source shape but the MP6 object/link
gate has not passed. `SRC-DIVERGES` means a current source candidate was
compiled and objdiff still rejects it. `NO-SOURCE` means the named split was
never promoted on this lineage and no current source candidate exists; it was
not "de-flipped." Historical corrections use `ASM-BLANKET-REMOVAL` and
`HEADER-INVALIDATED` below.

#### Bucket 1: `original-was-asm` (1 current fallback owner)

| Owner and reason | Authenticating sibling and sibling status | Current MP6 status |
| --- | --- | --- |
| `TRK_MINNOW_DOLPHIN/__exception.s` (`ASM-GATE-PENDING`) | [Mario Party 4 `src/TRK_MINNOW_DOLPHIN/__exception.s` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/TRK_MINNOW_DOLPHIN/__exception.s), `MatchingFor(USA,PAL)` | M4 authenticates the standalone `.init` exception-vector form and `0x1F34` size, but MP6 starts at a different address and has different vector/padding offsets. M5 has the MP6 bounds but is `NonMatching` with no source. A target-specific reconstruction and full gate are still required. |

#### Bucket 2: `C-not-yet-matched` (115 current fallback owners)

Twenty-eight owners have current source candidates and are tagged `SRC-DIVERGES`.
These DTK 0.9.2 object comparisons were regenerated from the current source
and target splits on 2026-07-14; percentages are raw `.text` scores unless
otherwise noted.

| Owner | Objdiff reason for remaining fallback |
| --- | --- |
| `Runtime.PPCEABI.H/New.cp` | The real four-byte `std::__new_handler` global at `0x802C0C58` is recovered; the target `.sbss` size is `0x8` because of alignment, not a second global. Both delete functions remain exact (`0x4C` each). Canonical `operator new[]` and `operator new` bodies compile to `0x68` versus target `0x70`/`0x6C`, at 85.178570%/88.518520%, so the owner remains fallback-linked. |
| `Runtime.PPCEABI.H/NewMore.cp` | Recovered the sibling-authenticated `std::__throws_bad_alloc` name, direct `bad_alloc : exception` inheritance, RTTI, vtables, and all five functions. Both destructors and `extab`/`extabindex` are 100%; `__throw_bad_alloc` is target/source `0x58`/`0x58` at 99.545456% with two pooled-string label operands, while each `what()` is `0xC`/`0x10`. String pooling and aligned data closure still diverge. |
| `Runtime.PPCEABI.H/NMWException.cpp` | 7/8 functions exact (`0x3F4` bytes); `__construct_array` is target/source `0xF8`/`0xFC`, 93.854836%. Two authenticated loop-shape probes were byte-neutral, so the sibling form was restored and unsupported register forcing was rejected. |
| `Runtime.PPCEABI.H/Gecko_ExceptionPPC.cpp` | Matching M4 donor gives 11/13 raw-exact retained functions, but emits a linker-stripped base-exception closure and divergent exception/data sections. A trial flip produced only 115 files OK and a divergent DOL/22 RELs, so it was de-flipped (`SRC-DIVERGES`). |
| `MSL_C.PPCEABI.bare.H/printf.c` | The exact MP4/MP5 source blob gives 9/15 exact target functions totaling `0xC78`, including the target's hidden `0x12C` helper after a temporary name-only pairing probe. The retained authentic name is `round_decimal`. Target/source whole `.text` remain `0x2160`/`0x213C`; `fprintf` is absent, five numeric-format functions diverge, total relocations are 275/271, and `.rodata`/`.data`/`.sdata` sizes differ. |
| `board/board.c` | 99.583%; 31/35 functions exact; `mbObjectSetup`, `mbMain`, `mbNextTime`, and `mbSaveInit` diverge. |
| `dolphin/os/OS.c` | 59.719%; 0/14 functions exact. |
| `dolphin/os/OSExec.c` | 80.972%; 2/8 functions exact. |
| `dolphin/os/OSMemory.c` | 52.707%; 4/8 functions exact. |
| `dolphin/os/OSReset.c` | 71.478%; 2/7 functions exact. |
| `dolphin/os/__start.c` | Raw section bytes alone report 100%, but only 4/7 functions map exactly; `__start`/`__init_registers` are unresolved and `__init_data` is 86.667%. |
| `dolphin/gx/GXTransform.c` | 70.599%; 9/16 functions exact. |
| `dolphin/exi/EXIBios.c` | 68.572%; 1/23 functions exact. |
| `dolphin/mic/mic.c` | 56.653%; 0/41 functions exact. |
| `dolphin/mic/m2s.c` | 78.721%; 1/14 functions exact. |
| `msm/msmsys.c` | 99.734%; 18/23 functions exact. |
| `msm/msmstream.c` | 99.487020%; 23/28 functions exact. `msmStreamDvdCallback` and `msmStreamDvdCallback2` are exact. `msmStreamData` is now target/source `0x2EC/0x2EC` at 99.759360% after recovering the target pause/linked-slot/ARAM-update control flow; `msmStreamSlotInit` is `0x224/0x224` at 99.635040% after recovering its three distinct size/offset lifetimes. Five functions still diverge, so the owner remains fallback-linked. |
| `board/player.c` | Raw section pairing is 1.155%; mapped-function weighted score is 99.965%, but only 10/165 functions are currently exact. |
| `board/audio.c` | 99.98862%; 46/49 functions exact. The remaining code differences are one compare-operand reversal in each of `mbMusBoardPlay` and `MusBoardFade`, plus twelve stack-slot offsets in `mbMusBoardFadeOut`; target/source `.text` are both `0x2BF0` and all 444 text relocations match. |
| `board/masu.c` | Raw `.text` pairing is 64.42536%; mapped-function weighted score is 99.763774%, with 112/119 target functions represented and 103/119 exact. Wave 27 recovered 21 formerly absent functions and the real `0x1C` next-space work layout. Fourteen additions are fully exact; seven exact-size instruction streams retain only compiler-local constant/static-data relocation identities while the owner is incomplete. Seven large draw/event/color/value functions remain absent, so the owner stays fallback-linked. |
| `board/branch.c` | 99.816%; 16/17 functions exact. `ev_Branch` is now target/source `0x8F0`/`0x8F0`; all 19 remaining differences are one `choice`/`choiceTime` register cycle, with no inserted, deleted, or replaced instructions. Whole-owner `.text` is `0x10E0`/`0x10E0` and all 210 relocations match; configured `.data` remains `0x18`/`0x12`. |
| `board/effect.c` | 59.766%; 1/35 functions exact, target/source text sizes `0x3050`/`0x3074`, plus `.sdata` and `.sdata2` divergence. The restored FastCast header is not its blocker. |
| `gssdk_lib/asrpho/common/blocks/delaybl.c` | The target-derived `DelayBlock` and shared `TosBaseBlock` ABI recover all four functions. `ProcessDelayBlock`, `InitDelayBlock`, and `ConstructDelayBlock` are exact. `ControlDelayBlock` is target/source `0xE4/0xE4` at 98.947365%; its only retained instruction difference is target `addi r4,r5,0` versus source `li r4,0` in the clear loop. Evidence-backed declaration and assignment variants did not reproduce it, so the owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/undersam.c` | The target-derived `0x2C` undersampler, signed-sample grouping, profile defaults, and error path are recovered. `ProcessUndersampler`, `ControlUndersampler`, and `ConstructUndersampler` are exact. `InitUndersampler` is target/source `0xC0/0xC0` at 99.687500%; only the quotient's volatile register differs (`r5` target, `r4` source), so the whole owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/genfilt.c` | The target-derived `0x30` gender filter, first-non-null input selection, profile-sized ports, and destructor control are recovered. `ProcessGenderFilter`, `ControlGenderFilter`, and `InitGenderFilter` are exact. The `0x7C` constructor remains at 79.000000% because its profile-probe and callback-address scheduling diverge; the owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/median.c` | The target-derived `0x44` median block and `0xC` doubly-linked node recover the ring replacement, sorted insertion, median selection, allocation, and destruction logic. `ControlMedian`, `InitMedian`, and `ConstructMedian` are exact. `ProcessMedian` is target/source `0x2F4/0x2F4` at 78.952380%, so the owner remains fallback-linked. MP7 has the same retained machine-code shape but supplies no source donor. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/statio.c` | The target-derived `0x50` stationarity block, queue/context fields, constants, and four functions are recovered. `ControlStationarity`, `InitStationarity`, and `ConstructStationarity` are exact. `ProcessStationarity` is target/source `0x1A8/0x1A8` at 99.575470%, with seven floating-point register-assignment differences. The target `.sdata2` is `0x10` versus source `0xC`, including a four-byte alignment tail, so the owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/ctxdata/ctxdata.c` | The binary-proven `ContextDataV2` layout through offset `0xB8`, virtual table, and all accessors are recovered. Seventeen accessors are byte-exact; the three packed-tail pointer computations remain at 79.500000%, 66.933334%, and 68.000000%, while `FillContextV2VirtualTable` retains one source/target table-relocation identity difference. The owner remains fallback-linked. |

The other 87 owners are tagged `NO-SOURCE`. Each explicit brace group below
expands to the named owner files; the count audit is
`3 + 1 + 14 + 47 + 22 = 87`.

- `MSL_C.PPCEABI.bare.H/` (3): `alloc.c`, `qsort.c`, and `e_exp.c`.
- `TRK_MINNOW_DOLPHIN/` C owner (1): `targimpl.c`.
- `musyx/runtime/` (14): `seq.c`, `synth.c`, `stream.c`, `synthdata.c`,
  `synthmacros.c`, `synthvoice.c`, `s_data.c`, `hw_dspctrl.c`, `snd3d.c`,
  `snd_midictrl.c`, `hardware.c`, `dsp_import.c`, `hw_aramdma.c`, and
  `StdReverb/reverb.c`.
- `gssdk_lib/` (47):
  - `gsapi/{callbacks,ctxfuncs,gsapi}.c`;
  - `asrpho/asrspi.c` and
    `asrpho/rec1600/{convert,creasp,creaspch,creaspt,creatree,crsptrch,ctrl,spi1600,train,userword}.c`;
  - `asrpho/common/blocks/{dpgenuw,dpscruw,exev_dp,fft_maye,fftmod,isoword,nbestdp,pitchdp,pitchwin,stacker}.c`;
  - `asrpho/common/blocks/flblocks/{acne,dctlift,gender,logexp,mel,mtx,mtxopt,smoother,specsub,vad,vq1500,window}.c`;
  - `asrpho/common/blocks/flfxblks/{combiner,dist16,pitchco,shs_vuv,slidhist,subsamp,trigglr,voicing}.c`;
  - `asrpho/common/ctxdata/langdata.c`,
    `asrpho/common/tos/{mqueue,tinyos}.c`;
- `board/` (22): `math.c`, `snpc.c`, `scroll.c`, `coin.c`, `star.c`,
  `dice.c`, `opening.c`, `tutorial.c`, `capselect.c`, `capmove.c`,
  `capthrow.c`, `captrap.c`, `capspecial.c`, `capsule.c`, `capevent.c`,
  `shopevent.c`, `mgcall.c`, `config.c`, `last5.c`, `telop.c`, `wipe.c`, and
  `single.c`.

Mixed C/assembly owners such as `TRK_MINNOW_DOLPHIN/targimpl.c` remain in the
C bucket: authenticating one assembly routine would not complete their C
work.

### Applied sibling-exception decisions

| MP6 owner | Sibling authentication | Decision and MP6 proof |
| --- | --- | --- |
| `TRK_MINNOW_DOLPHIN/targsupp.s` | [M5 `src/TRK_MINNOW_DOLPHIN/targsupp.s` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/targsupp.s), `Matching`; M4 carries the identical blob as `MatchingFor(USA,PAL)`. | **Admitted.** The donor blob is `0244131bd8219c6f5839ae2cda9254c2f28e005c`. Its four 8-byte `twui r0,0; blr` functions exactly fill the MP6 `0x20` target owner; source-object and final 137-file/hash/DOL gates pass. It is authentic standalone assembly, not decompiled C. |
| `TRK_MINNOW_DOLPHIN/dolphin_trk.c` | [M5 `src/TRK_MINNOW_DOLPHIN/dolphin_trk.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/dolphin_trk.c), configured `Matching`. | **Admitted.** The retained `0x104` `.init` and `0x140` `.text` ranges reproduce all five target functions; donor-only out-of-line copies of `__TRK_copy_vectors` (`0x104`) and `TRK_copy_vector` (`0x90`) are linker-stripped after their logic is inlined into `__TRK_reset`. The semantic `TRK_ISR_OFFSETS` and `lc_base` globals plus target alignment tails are named and the full 137-file/hash/DOL gate passes. `InitMetroTRK` is authentic sibling assembly, so the mixed owner is excluded from clean-C totals. |
| `musyx/runtime/snd_service.c` | [M5 `src/musyx/runtime/snd_service.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/snd_service.c), configured `Matching`. | **Admitted.** All eight retained routines fill the exact MP6 `0x2D4` linked text range. The two additional source-object lower-half relocations in `sndSqrt` resolve to the target immediates; the linked owner and final DOL are byte-identical. `sndSintab`, `last_rnd`, `sqrtConsts`, `_sinConsts`, and `i2fMagic` recover the semantic data closure without source padding. `sndSqrt` and `sndCos` are authenticated sibling assembly, so the mixed owner is excluded from clean-C totals. |
| `game/kerent.c` | [M4 `src/game/kerent.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/game/kerent.c), `MatchingFor(USA,PAL)`, authenticates a single `asm void _kerent` with `nofralloc` and `entry`/`b` pairs. | **Admitted.** `ae2977e` de-flipped it through blanket inline-assembly removal (`ASM-BLANKET-REMOVAL`), despite the written exception. The exact MP6 source was restored from `git show ae2977e^:src/game/kerent.c` (blob `bd5b7b9448a659e186d733e3bc3e13dc8291d6a2`). Target/source `.text` are `0x26A0` and 100% identical; each has 2,472 `R_PPC_REL24` relocations. The final 137-file/hash/DOL gate passes. It is an authenticated assembly jump table, not decompiled C. |
| `Runtime.PPCEABI.H/ptmf.c` | [Mario Kart: Double Dash `libs/PowerPC_EABI_Support/src/Runtime/ptmf.c` at `8a91a6b`](https://github.com/doldecomp/mkdd/blob/8a91a6b4a32834dfb689fbdcf1dea8796be9c75f/libs/PowerPC_EABI_Support/src/Runtime/ptmf.c), configured `Matching`. | **Admitted.** The authenticated `__ptmf_test`, `__ptmf_cmpr`, and `__ptmf_scall` assembly bodies reproduce all `0x94` retained target text bytes with no relocations. Donor-only `__ptmf_scall4` (`0x28`) and `__ptmf_null` (`0xC`) are linker-stripped. The full 137-file/hash/DOL gate passes. This is authentic Runtime assembly and is excluded from clean-C totals. |
| `dolphin/os/OSCache.c` | [M4 `src/dolphin/os/OSCache.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSCache.c), `MatchingFor(USA,PAL)`. | **Restored after de-flip.** `93d9438` replaced the authentic bodies and de-flipped the owner while isolating assembly (`ASM-BLANKET-REMOVAL`). All 14 retained MP6 assembly bodies have same-name sibling bodies. Whole-file blobs differ because M4 carries linker-stripped helpers; the audited retained differences are ABI register/symbol spelling, and the MP6 object/effective-relocation/full-container gate passes. |
| `dolphin/os/OSContext.c` | [M4 `src/dolphin/os/OSContext.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSContext.c), `MatchingFor(USA,PAL)`. | **Restored after de-flip.** `93d9438` replaced the authentic bodies and de-flipped the owner while isolating assembly (`ASM-BLANKET-REMOVAL`). All nine retained MP6 assembly bodies have same-name sibling bodies. `OSLoadContext` differs textually only in target labels/operand aliases; object/effective-relocation/full-container proof passes. |
| `game/jmp.c` | [M4 `src/game/jmp.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/game/jmp.c), `Matching`. | **Restored after de-flip.** `ae2977e` de-flipped the owner through blanket inline-assembly removal (`ASM-BLANKET-REMOVAL`). MP6 and M4 are the exact Git blob `8313bd8cb5c154bca116a9a75a518e8faa0a5856`; the owner contains C `gcsetjmp` plus authentic assembly `gclongjmp`, and passes the MP6 full gate. |
| `musyx/runtime/Chorus/chorus_fx.c` | [M5 `src/musyx/runtime/Chorus/chorus_fx.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/Chorus/chorus_fx.c), `Matching`. | **Admitted.** The authenticated `do_src1` and `do_src2` `static asm` bodies accompany three retained C functions. Target/source `.text` are `0x868/0x900`; the donor-only `0x98` update helper and its constants are linker-stripped, and the linked owner/full-container gate passes. This mixed owner is not clean decompiled C. |
| `musyx/runtime/CheapReverb/creverb.c` | [M5 `src/musyx/runtime/CheapReverb/creverb.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/CheapReverb/creverb.c), `Matching`. | **Admitted.** The authenticated `HandleReverb` `static asm` body accompanies three retained C functions. Target/source `.text` are `0x8BC/0xBA4`; four donor-only C helpers and their constants are linker-stripped, and the linked owner/full-container gate passes. This mixed owner is not clean decompiled C. |

### OSFastCast re-verification after `93d9438`

Commit `93d9438` replaced the authenticated paired-single implementation in
`OSFastCast.h` with scalar C and de-flipped four consumers solely because that
header/include-graph change invalidated their prior byte proof
(`HEADER-INVALIDATED`). Its separate OSCache/OSContext assembly de-flips are
recorded above. The current header is again byte-identical to the M4 and M5
Matching donor blob `b52cabfe94bf7882b26318063efba922fb977534`.

| Owner | Current decision | Fresh object evidence |
| --- | --- | --- |
| `game/hsfdraw.c` | `Matching`; `93d9438` reason was `HEADER-INVALIDATED` | 54/54 functions and `.text 0xCF00` are 100%; raw `.rodata`/`.sdata` attribution differences are compiler-local split labels, and the linked range/container are exact. |
| `game/hsfanim.c` | `Matching`; `93d9438` reason was `HEADER-INVALIDATED` | 71/71 functions and `.text 0x65CC` are 100%; linked range/container exact. |
| `game/init.c` | `Matching`; `93d9438` reason was `HEADER-INVALIDATED` | `.text 0x998` is 99.943% in raw objdiff because seven `LoadMemInfo` operands pair target labels with compiler-local labels at the same offsets; effective relocations and linked bytes are exact. |
| `board/roulette.c` | `Matching`; `93d9438` reason was `HEADER-INVALIDATED` | `.text 0x1838` is 99.735% in raw objdiff; all reported differences are target `@sda21` names versus compiler-local constant names at the same effective addresses. Linked bytes and the container are exact. |
| `board/effect.c` | `NonMatching` | Genuine divergence: target/source `.text` are `0x3050`/`0x3074`, score 59.766%, only 1/35 functions is exact, `.sdata` is `0x18`/`0x14`, and `.sdata2` scores 57.353%. It was already NonMatching before `93d9438`; it was not de-flipped by the header rework. |

The four flips above are accepted only because their compiled source objects
are selected exactly once by `configure.py` and the final `ninja`, explicit
DTK SHA manifest, and original-vs-built `main.dol` comparison all pass.

### Board status recovery and branch evidence

`board/status.c` is now Matching clean C. `StatusUpdate` reuses the existing
function-scoped `i` and `j` locals for the single-board space-number path and
uses explicit comparisons around `GWPartyGet`; that source shape produces the
target register lifetimes and exact `0x718` function. All 44 functions,
`.text 0x3CB8`, `.rodata 0x18`, `.data 0x3F4`, `.bss 0xD8`, `.sbss 0x8`,
`.sdata2 0x58`, and 807 relocations match. The old `.data` end at `0x802485A8`
incorrectly assigned the four-byte linker-alignment gap before
`board/opening.c` to this owner; the proven owner ends at `0x802485A4`.

`board/branch.c` remains NonMatching. Reusing function-scoped `i` for the
tutorial result and placing `padNo` before `masuPlayer` removed its stack
spill, eight-byte size excess, and an unrelated register cycle. The only
remaining `ev_Branch` differences are 19 operands in the coupled
`choice`/`choiceTime` `r26`/`r27` assignment. No speculative declaration or
scope rewrite is accepted without resolving that cycle. Full proof details
are retained in [`docs/native_matching_wave17.md`](docs/native_matching_wave17.md).

`board/audio.c` remains NonMatching, but four delayed-effect functions now
have exact source: `mbAudFXPlay`, `mbAudFXPosPlay`, `mbAudFXEmitterPlay`, and
`mbAudGuidePlay` (combined target/source code size `0x518`). The generic
reconstructed helper did not preserve the target call-site lifetimes; spelling
out the delayed queue paths and retaining the small position-copy helper makes
all four instruction streams and their relocations exact. This function-level
recovery is not counted as a matching owner. The object and rejected-probe
evidence is retained in
[`docs/native_matching_wave18.md`](docs/native_matching_wave18.md).

`msm/msmstream.c` now has exact source for both asynchronous DVD callbacks.
The target tests the callback `result` argument directly rather than issuing a
second DVD status query, and its status-5 path deactivates and clears the
stream before closing it. Together `msmStreamDvdCallback` and
`msmStreamDvdCallback2` account for `0x550` bytes of newly exact source. The
owner remains NonMatching at 23/28 exact functions, so none of its fallback-
linked bytes are counted as decompiled source. Proof and rejected-probe details
are retained in
[`docs/native_matching_wave19.md`](docs/native_matching_wave19.md).

`Runtime.PPCEABI.H/GCN_mem_alloc.c` is now Matching clean C. Both `0xB8`
functions, all 32 relocations, and the 113 semantic constant bytes match; the
target's seven-byte zero tail is reproduced by linker alignment. Four other
Runtime owners now have authenticated source candidates without being
promoted: `New.cp` has two exact delete functions (`0x98` code bytes),
`NMWException.cpp` has seven exact functions (`0x3F4` bytes), while
`NewMore.cp` and `Gecko_ExceptionPPC.cpp` retain documented objdiff/linker-
closure differences. All four remain fallback-linked and are not counted as
decompiled owners. The accepted and rejected evidence is retained in
[`docs/native_matching_wave20.md`](docs/native_matching_wave20.md).

Seven recent MSL owners are now Matching clean C. The authenticated sibling
sources for `ansi_fp.c`, `file_io.c`, `mbstring.c`, `mem_funcs.c`, and
`e_pow.c` require owner-local `GC/1.3`; direct probes proved that `GC/2.6`
changes their inline and fdlibm code shape. The target-led `string.c` and
`math_ppc.c` closure instead matches under the configured `GC/2.6`. Together
the seven owners contribute 34 exact functions, `0x1F28` linked code bytes,
`0x358` configured constant/data bytes, and 245 relocations. `printf.c` now
retains authenticated source and nine exact functions totaling `0xC78`, but
remains fallback-linked because the formatter closure genuinely diverges.
The accepted and rejected evidence is retained in
[`docs/native_matching_wave21.md`](docs/native_matching_wave21.md) and
[`docs/native_matching_wave25.md`](docs/native_matching_wave25.md).

Wave 28 adds three more Matching clean-C MSL owners: `assert.c`, `s_atan.c`,
and `w_log.c`. The target establishes the older three-argument assertion
routine and its exact diagnostic string; the source function and all six
relocations match, while the target object's `0x30` rodata includes the
linker's zero alignment tail. The Matching-MP5 fdlibm `s_atan.c` source under
owner-local `GC/1.3` reproduces `0x200` text bytes, `0xC0` constant bytes, and
ten effective relocations. `w_log.c` reproduces its authentic old-style
`0x20` wrapper and sole relocation under `GC/2.6`. Together the three owners
add `0x26C` linked code bytes, `0xF0` configured constant bytes, and 17
relocations. Evidence and rejected donor probes are retained in
[`docs/native_matching_wave28.md`](docs/native_matching_wave28.md).

Wave 29 adds `OdemuExi2/DebuggerDriver.c` as Matching clean C and
`Runtime.PPCEABI.H/ptmf.c` under the sibling-assembly exception. The Odemu
owner recovers 14 retained functions, seven real local globals, `0xA80` text
bytes, `0x20` configured small-data/BSS bytes, and 57 effective relocations
under its authenticated `GC/1.2.5n` deferred-inline build shape. Six unused
SDK helpers totaling `0x270` are linker-stripped. Runtime `ptmf` contributes
three exact retained assembly routines totaling `0x94`; Matching MKDD
authenticates their original source form, so none of those bytes enter the
clean-C total. Evidence is retained in
[`docs/native_matching_wave29.md`](docs/native_matching_wave29.md).

Three MusyX owners are now Matching under the sibling-assembly exception:
`CheapReverb/creverb.c`, `Chorus/chorus_fx.c`, and `snd_service.c`. Their 17
retained functions contribute `0x13F8` exact linked code bytes. Each owner
retains authenticated Matching-MP5 assembly and is therefore excluded from
clean-C totals. Fresh probes of the other Matching-MP5 candidates
`hw_dolphin.c` and `seq.c` were rejected on target/source text sizes
`0x4F8/0x5A4` and `0x3FCC/0x5168` respectively, and fully removed. The first
two accepted owners are documented in
[`docs/native_matching_wave22.md`](docs/native_matching_wave22.md).

Wave 30 adds the Matching-MP5 `dolphin_trk.c` and `snd_service.c` mixed owners
under the sibling-assembly exception. `dolphin_trk` contributes `0x104` init
and `0x140` text bytes; its five retained functions, real interrupt-vector
table, and local-cache base match, while two out-of-line helpers totaling
`0x194` are linker-stripped. `snd_service` contributes eight retained
functions and `0x2D4` text bytes. Its six C routines plus authenticated
assembly `sndSqrt`/`sndCos`, five real data symbols, and target-proven
inter-owner alignment reproduce the exact linked range. Together the owners
add `0x518` linked code bytes and `0x898` configured data/BSS bytes, but no
clean-C owner. The close `targimpl.c` donor was rejected at 29/30 exact
functions and fully removed. Evidence is retained in
[`docs/native_matching_wave30.md`](docs/native_matching_wave30.md).

Wave 23 advanced two fallback owners at function level. `board/object.c` has
since passed the complete object and DOL gates and is reported below as
Matching clean C. `msm/msmstream.c::msmStreamData` retains the target pause,
linked-slot, buffer-clear, and ARAM-update behavior at the exact `0x2EC` size;
`msmStreamSlotInit` retains the target's distinct half-buffer/read-size/
wrap-distance lifetimes. Those two stream functions remain 99.759360% and
99.635040% respectively, with only operand-allocation differences. The stream
owner stays `NonMatching`, and none of its fallback-linked bytes are counted
as decompiled source. Evidence and bounded rejected probes are retained in
[`docs/native_matching_wave23.md`](docs/native_matching_wave23.md).

`board/object.c` is now Matching clean C. Recovering the two motion-create
functions and the target `.sbss` emission order makes all 80 functions,
`0x2838` text bytes, `0x18` BSS bytes, `0x48` constant bytes, and 334 text
relocations exact. The first linked trial was deliberately rejected despite a
100% raw object score because its zero-filled BSS globals had different symbol
offsets; the corrected declaration order reproduces every target offset and
passes the final DOL gate. Source provenance, the rejected `board/audio.c`
probes, and full proof are retained in
[`docs/native_matching_wave26.md`](docs/native_matching_wave26.md).

Wave 27 materially advances `board/masu.c` without promoting the owner.
Twenty-one formerly absent functions now have clean C, including the
lifecycle/display setup, next-space state, event-bit transforms, corner
ranking/rotation, capsule ownership checks, and player prize reset. Fourteen
new functions totaling `0xA28` bytes are fully exact. Seven more totaling
`0xCAC` have target-identical sizes and instruction sequences; objdiff reports
only compiler-local numeric-pool or static-table relocation identities caused
by the still-incomplete owner. The source object now maps 112/119 functions
and has 103 exact. The seven remaining absent functions are the large GX draw
paths, event state machines, player-color logic, and mushroom-value routine;
they were left unimplemented rather than guessed. Evidence is retained in
[`docs/native_matching_wave27.md`](docs/native_matching_wave27.md).

Runtime allocation-exception recovery now names the map-authenticated
`std::__new_handler` global and the sibling-authenticated
`std::__throws_bad_alloc` state, and restores the `std::bad_alloc :
std::exception` class closure, including RTTI and vtables.
In `NewMore.cp`, both virtual destructors plus `extab` and `extabindex` are
exact; the exact-size throw helper differs only in two pooled-string label
operands. The canonical allocation bodies and two authenticated
`__construct_array` spellings were compiled and rejected where they still
diverged. `New.cp`, `NewMore.cp`, and `NMWException.cpp` therefore remain
`NonMatching`, and none of their fallback-linked bytes are counted as
decompiled owners. Evidence is retained in
[`docs/native_matching_wave24.md`](docs/native_matching_wave24.md).

## Named DOL ownership

The current snapshot descends from fork commit `353fa30`, which replaced every
DOL `auto_*` blob with 121 named Runtime, MSL, MusyX, MetroTRK, and
support-library owners. The pre-wave-26 `fork/main` tip was `f69d158`. Neither
`config/GP6E01/splits.txt` nor `configure.py` contains an `auto_*` owner.

## Native library recovery

The consolidated recovery series through `685f514` contains 103 accepted DOL
owners: 97 net-new promotions relative to `353fa30` plus six restorations of
owners that were already `Matching` there. Together they account for 188,368
code bytes and 46,100 configured data/BSS bytes. The latest consolidated wave
contributes 52 of those owners, 166,580 code bytes, and 34,860 data/BSS bytes.
The kerent exception restored by this policy correction is reported
separately and is deliberately excluded from clean-C totals. The subsequent
clean-C `board/status.c` recovery adds `0x3CB8` code bytes and `0x544`
configured data/BSS bytes.

- Nine core MSL owners: `errno`, `arith`, `float`, `s_copysign`, `w_acos`,
  `w_asin`, `w_atan2`, `w_fmod`, and `w_pow`. Seven owners are whole-section
  exact. `arith` retains an exact `abs` while the sibling-authenticated unused
  `labs` is linker-stripped; `errno` owns an exact four-byte symbol followed by
  the target split's four-byte alignment gap.
- Six GC/1.3-shaped MSL owners: `ansi_fp`, `file_io`, `mbstring`,
  `mem_funcs`, `e_pow`, and `s_atan`. Their sibling-authenticated C contributes
  7,360 exact linked code bytes and 1,048 configured data bytes. Object-level symbol
  attribution differences in `ansi_fp` and `e_pow` are resolved by the exact
  linked owner ranges; `s_atan` likewise resolves compiler-local pool labels
  through its exact linked range. No assembly or source padding was added.
- Four GC/2.6-shaped MSL owners: `string`, `math_ppc`, `assert`, and `w_log`.
  Their 20 clean-C functions contribute `0x4D4` exact linked code bytes,
  `0x30` configured constant bytes, and 18 relocations.
  `printf` additionally retains nine exact functions but remains a fallback
  owner and is not counted as decompiled source.
- Two Runtime owners: `__va_arg` is exact at `0xC8`; `__mem` is exact at
  `0x138`, including the target order `memset`, `__fill_mem`, `memcpy` and its
  sole `R_PPC_REL24` relocation.
- Runtime `GCN_mem_alloc` adds `0x170` exact code bytes and `0x78` configured
  constant bytes. The target's final seven constant bytes are documented
  linker-alignment padding, not fabricated source data.
- `OdemuExi2/DebuggerDriver` adds `0xA80` exact clean-C code bytes, `0x20`
  configured small-data/BSS bytes, 14 retained functions, seven named local
  globals, and 57 effective relocations. Six authenticated unused SDK helpers
  are linker-stripped from the exact owner range.
- Runtime `ptmf` adds `0x94` exact retained code bytes under the Matching-MKDD
  sibling exception. Its three `nofralloc` routines are authentic assembly,
  so this owner is reported separately from clean C.
- Six MusyX owners: `hw_memory`, `synth_ac`, `synth_dbtab`, `seq_api`,
  `creverb_fx`, and `reverb_fx`. These promote 1,392 code bytes and 2,976 data
  bytes and recover the `dspAttenuationTab`, `dspScale2IndexTab`,
  `dspDLSVolTab`, `seqMIDIPriority`, `synthInfo`, and `salHooks` owners.
- Three MusyX sibling-exception owners, `CheapReverb/creverb`,
  `Chorus/chorus_fx`, and `snd_service`, add 5,112 exact linked code bytes.
  Their five retained assembly routines are authenticated by Matching Mario
  Party 5 sources, so these owners are reported separately from clean C.
- Five additional native owners: MSL `misc_io` and `signal`, plus MusyX
  `delay_fx`, `hw_volconv`, and `snd_math`. They promote 3,868 code bytes and
  664 data bytes.
- Eleven fdlibm owners: `s_tan`, `s_frexp`, `k_sin`, `s_cos`, `s_sin`,
  `s_modf`, `s_floor`, `s_ldexp`, `e_atan2`, `e_fmod`, and `k_rem_pio2`.
  Their 7,032 text bytes, 392 constant bytes, and 108 relocations match.
- Seven MetroTRK core owners: `dispatch`, `main_TRK`, `mem_TRK`, `msgbuf`,
  `mutex_TRK`, `target_options`, and `usr_put`. They promote 3,292 code bytes
  and 6,768 data bytes.
- Five MetroTRK control-path owners: `mainloop`, `mslsupp`, `notify`,
  `nubinit`, and `targcont`. Their eight retained functions contribute 1,276
  code bytes, 40 configured data bytes, and 45 exact relocations. `nubinit`'s
  semantic data is followed by four-byte rodata and BSS alignment tails.
- MetroTRK `dolphin_trk` adds `0x244` exact retained code bytes under the
  Matching-MP5 sibling exception. `InitMetroTRK` is authentic assembly; two
  donor-only out-of-line helpers totaling `0x194` are linker-stripped.
- `GXLight` contributes 1,760 code bytes, 104 configured data bytes, 13 exact
  functions, and 62 relocations. Its only inline assembly is the authenticated
  Matching-donor paired-single upload helper; the target split ends with a
  four-byte constant-data alignment tail.
- Five deferred-emission MSL owners: `mem`, `uart_console_io`, `direct_io`,
  `FILE_POS`, and `abort_exit`. Their 2,440 text bytes, 280 configured data/BSS
  bytes, 13 functions, and 51 relocations match. Authenticated GC/2.6
  `-inline deferred` emission produces the target function order without
  reordering source; `abort_exit` owns the exact `__atexit_funcs` table and
  four small-data globals.
- Six game and board owners: `init`, `jmp`, `malloc`, `hsfdraw`, `hsfanim`,
  and `roulette`. They contribute 89,256 code bytes and 18,336 data/BSS bytes;
  all 172 retained functions and 4,304 target relocations pass the strict
  linked-range gate.
- Board object management contributes 10,296 exact clean-C code bytes, 96
  configured BSS/constant bytes, 80 functions, and 334 target relocations.
  Its small-data global offsets are target-proven rather than inferred from
  zero-filled section bytes.
- Seventeen Dolphin SDK owners: `PPCArch`, `OSAlarm`, `OSCache`, `OSContext`,
  `OSInterrupt`, `OSSync`, `OSTime`, `__ppc_eabi_init`, `db`, `mtx`, `mtxvec`,
  `mtx44vec`, `vec`, `quat`, `psmtx`, `ai`, and `THPDec`. Their 48,164 code
  bytes, 2,264 configured data/BSS bytes, 179 retained functions, and 770
  target relocations match. Each inline-assembly owner is also `Matching` in
  an authenticated MP4 or MKDD sibling.
- Thirty Runtime, MSL, TRK, MusyX, and stub owners contribute 29,740 code
  bytes, 14,332 configured data/BSS bytes, 120 retained functions, and 827
  target relocations. This includes the DOL Runtime owner,
  `__init_cpp_exceptions`; four low-level and five clean-C TRK owners; 14 MSL
  owners; three MusyX synth owners; and the AmcExi2 and Odemu stubs. Target
  object order is preserved for the exception init/fini and AmcExi2 functions.
  Relocations also recover real ownership for
  `__ctype_map`, `__lower_map`, `synthVoice`, the aux callback and MIDI globals,
  `synthFlags`, `vs`, and `gWriteBuf`. The address, definition, and target
  relocation ledger is retained in `docs/easy_ports_wave.md`.

The 116 current DOL fallback owners and their causes are exhaustive in the
two-bucket ledger above. Three REL Runtime variants also remain `NonMatching`;
they are outside this main-DOL-first taxonomy and are not included in the 115
C-work/1-assembly-policy remaining counts.
