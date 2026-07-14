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
- DTK progress at that build: 8.59% code and 26.80% data overall; 43.71% code
  and 62.80% data in the DOL
- Matching owners at that build: 257 of 895 overall, 248 of 396 in the DOL,
  and 9 of 499 in the REL modules
- DOL policy split: 221 matching owners without the assembly exception, 27
  matching owners admitted under the sibling-authentication exception, 147
  `C-not-yet-matched` fallback owners, and 1 `original-was-asm` fallback owner
  awaiting target proof. These four numbers total all 396 DOL owners.

The exact build result includes extracted original objects and explicit
standalone assembly fallbacks for owners that are not yet byte-identical C.
Those owners remain `NonMatching` in `configure.py`; fallback-linked code is
never counted as decompiled source. An exception-bearing matching owner is
reported separately from clean C; admitting authentic assembly does not turn
that assembly into decompiled C.

## `game/` ownership

`game/` is not fully decompiled. The current `Game` library has 60 of 61
object owners configured as matching: 57 without the assembly exception and
`game/kerent.c`, `game/jmp.c`, and `game/malloc.c` under authenticated
assembly exceptions. The remaining fallback owner is `board/board.c`, which
is real C matching work.

`game/mic.c` is matching and is linked from recovered C.

## Assembly fallback boundary

Assembly is never admitted merely because a C transcription did not match.
Inline or standalone assembly is admitted only when an authenticated sibling
project marks the same owner `Matching`, carries the corresponding instruction
body/source shape, and the MP6 compile, object-byte, effective-relocation,
linked-range, and container gates all pass. The current tree has 27 matching
exception owners:

- 17 Dolphin SDK owners authenticated by Matching Mario Party 4 sources, with
  `mtx44vec` independently authenticated by Matching Mario Kart: Double Dash
- `game/kerent.c`, `game/jmp.c`, and `game/malloc.c`, plus the authenticated
  `OSFastCast` closure used by matching Game/Board consumers
- the DOL Runtime owner, `__init_cpp_exceptions`, and four low-level TRK
  owners
- the existing `GXLight` owner, whose paired-single `PushLight` helper is from
  Matching Pikmin 2 commit `46aecad6`

Commit-, path-, block-, and target-object-level evidence for every exception
is retained in [`docs/easy_ports_wave.md`](docs/easy_ports_wave.md).

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

#### Bucket 2: `C-not-yet-matched` (147 current fallback owners)

Eighteen owners have current source candidates and are tagged `SRC-DIVERGES`.
These DTK 0.9.2 object comparisons were regenerated from the current source
and target splits on 2026-07-14; percentages are raw `.text` scores unless
otherwise noted.

| Owner | Objdiff reason for remaining fallback |
| --- | --- |
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
| `msm/msmstream.c` | 96.710%; 21/28 functions exact. |
| `board/player.c` | Raw section pairing is 1.155%; mapped-function weighted score is 99.965%, but only 10/165 functions are currently exact. |
| `board/object.c` | 99.754%; 77/80 functions exact. |
| `board/audio.c` | 99.98862%; 46/49 functions exact. The remaining code differences are one compare-operand reversal in each of `mbMusBoardPlay` and `MusBoardFade`, plus twelve stack-slot offsets in `mbMusBoardFadeOut`; target/source `.text` are both `0x2BF0` and all 444 text relocations match. |
| `board/masu.c` | Raw section pairing is 40.144%; mapped-function weighted score is 99.771%, with 89/119 functions exact. |
| `board/branch.c` | 99.816%; 16/17 functions exact. `ev_Branch` is now target/source `0x8F0`/`0x8F0`; all 19 remaining differences are one `choice`/`choiceTime` register cycle, with no inserted, deleted, or replaced instructions. Whole-owner `.text` is `0x10E0`/`0x10E0` and all 210 relocations match; configured `.data` remains `0x18`/`0x12`. |
| `board/effect.c` | 59.766%; 1/35 functions exact, target/source text sizes `0x3050`/`0x3074`, plus `.sdata` and `.sdata2` divergence. The restored FastCast header is not its blocker. |

The other 129 owners are tagged `NO-SOURCE`. Each explicit brace group below
expands to the named owner files; the count audit is
`6 + 14 + 2 + 19 + 1 + 65 + 22 = 129`.

- `Runtime.PPCEABI.H/` (6): `New.cp`, `NewMore.cp`, `NMWException.cpp`,
  `ptmf.c`, `Gecko_ExceptionPPC.cpp`, `GCN_mem_alloc.c`.
- `MSL_C.PPCEABI.bare.H/` (14): `alloc.c`, `ansi_fp.c`, `assert.c`,
  `file_io.c`, `mbstring.c`, `mem_funcs.c`, `printf.c`, `qsort.c`, `string.c`,
  `e_exp.c`, `e_pow.c`, `s_atan.c`, `w_log.c`, `math_ppc.c`.
- `TRK_MINNOW_DOLPHIN/` C owners (2): `targimpl.c`, `dolphin_trk.c`.
- `musyx/runtime/` (19): `seq.c`, `synth.c`, `stream.c`, `synthdata.c`,
  `synthmacros.c`, `synthvoice.c`, `s_data.c`, `hw_dspctrl.c`, `snd3d.c`,
  `snd_init.c`, `snd_midictrl.c`, `snd_service.c`, `hardware.c`,
  `dsp_import.c`, `hw_aramdma.c`, `hw_dolphin.c`,
  `CheapReverb/creverb.c`, `StdReverb/reverb.c`, and
  `Chorus/chorus_fx.c`.
- `OdemuExi2/DebuggerDriver.c` (1).
- `gssdk_lib/` (65):
  - `gsapi/sid/sid.c` and
    `gsapi/{callbacks,ctxfuncs,extaudio,gsapi,mathusage,wrddata}.c`;
  - `asrpho/asrspi.c` and
    `asrpho/rec1600/{convert,creasp,creaspch,creaspt,creatree,crsptrch,ctrl,initial,spi1600,train,userword}.c`;
  - `asrpho/common/blocks/{delaybl,dpgenuw,dpscruw,exev_dp,fft_maye,fftmod,isoword,nbestdp,pitchdp,pitchwin,stacker,undersam}.c`;
  - `asrpho/common/blocks/flblocks/{acne,dctlift,gender,logexp,mel,mtx,mtxopt,smoother,spline,specsub,vad,vq1500,window}.c`;
  - `asrpho/common/blocks/flfxblks/{combiner,dist16,genfilt,lkahead,median,pitchco,shs_vuv,slidhist,statio,subsamp,trigglr,voicing}.c`;
  - `asrpho/common/ctxdata/{ctxdata,langdata}.c`,
    `asrpho/common/tos/{mqueue,tinyos}.c`, and
    `asrpho/common/fastallo/fastallo.c`;
  - `common/csspi/csspi.c`, `common/safeh/safeh.c`,
    `common/osspi/osspi.c`, and `common/rsrc/rsrc.c`.
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
| `game/kerent.c` | [M4 `src/game/kerent.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/game/kerent.c), `MatchingFor(USA,PAL)`, authenticates a single `asm void _kerent` with `nofralloc` and `entry`/`b` pairs. | **Admitted.** `ae2977e` de-flipped it through blanket inline-assembly removal (`ASM-BLANKET-REMOVAL`), despite the written exception. The exact MP6 source was restored from `git show ae2977e^:src/game/kerent.c` (blob `bd5b7b9448a659e186d733e3bc3e13dc8291d6a2`). Target/source `.text` are `0x26A0` and 100% identical; each has 2,472 `R_PPC_REL24` relocations. The final 137-file/hash/DOL gate passes. It is an authenticated assembly jump table, not decompiled C. |
| `dolphin/os/OSCache.c` | [M4 `src/dolphin/os/OSCache.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSCache.c), `MatchingFor(USA,PAL)`. | **Restored after de-flip.** `93d9438` replaced the authentic bodies and de-flipped the owner while isolating assembly (`ASM-BLANKET-REMOVAL`). All 14 retained MP6 assembly bodies have same-name sibling bodies. Whole-file blobs differ because M4 carries linker-stripped helpers; the audited retained differences are ABI register/symbol spelling, and the MP6 object/effective-relocation/full-container gate passes. |
| `dolphin/os/OSContext.c` | [M4 `src/dolphin/os/OSContext.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSContext.c), `MatchingFor(USA,PAL)`. | **Restored after de-flip.** `93d9438` replaced the authentic bodies and de-flipped the owner while isolating assembly (`ASM-BLANKET-REMOVAL`). All nine retained MP6 assembly bodies have same-name sibling bodies. `OSLoadContext` differs textually only in target labels/operand aliases; object/effective-relocation/full-container proof passes. |
| `game/jmp.c` | [M4 `src/game/jmp.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/game/jmp.c), `Matching`. | **Restored after de-flip.** `ae2977e` de-flipped the owner through blanket inline-assembly removal (`ASM-BLANKET-REMOVAL`). MP6 and M4 are the exact Git blob `8313bd8cb5c154bca116a9a75a518e8faa0a5856`; the owner contains C `gcsetjmp` plus authentic assembly `gclongjmp`, and passes the MP6 full gate. |

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

## Named DOL ownership

The current snapshot descends from fork commit `353fa30`, which replaced every
DOL `auto_*` blob with 121 named Runtime, MSL, MusyX, MetroTRK, and
support-library owners. The pre-wave `fork/main` tip was `5e23675`. Neither
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
- Two Runtime owners: `__va_arg` is exact at `0xC8`; `__mem` is exact at
  `0x138`, including the target order `memset`, `__fill_mem`, `memcpy` and its
  sole `R_PPC_REL24` relocation.
- Six MusyX owners: `hw_memory`, `synth_ac`, `synth_dbtab`, `seq_api`,
  `creverb_fx`, and `reverb_fx`. These promote 1,392 code bytes and 2,976 data
  bytes and recover the `dspAttenuationTab`, `dspScale2IndexTab`,
  `dspDLSVolTab`, `seqMIDIPriority`, `synthInfo`, and `salHooks` owners.
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
- Seventeen Dolphin SDK owners: `PPCArch`, `OSAlarm`, `OSCache`, `OSContext`,
  `OSInterrupt`, `OSSync`, `OSTime`, `__ppc_eabi_init`, `db`, `mtx`, `mtxvec`,
  `mtx44vec`, `vec`, `quat`, `psmtx`, `ai`, and `THPDec`. Their 48,164 code
  bytes, 2,264 configured data/BSS bytes, 179 retained functions, and 770
  target relocations match. Each inline-assembly owner is also `Matching` in
  an authenticated MP4 or MKDD sibling.
- Twenty-nine Runtime, MSL, TRK, MusyX, and stub owners contribute 29,160 code
  bytes, 14,260 configured data/BSS bytes, 115 retained functions, and 792
  target relocations. This includes the DOL Runtime owner,
  `__init_cpp_exceptions`; three low-level and five clean-C TRK owners; 14 MSL
  owners; three MusyX synth owners; and the AmcExi2 and Odemu stubs. Target
  object order is preserved for the exception init/fini and AmcExi2 functions.
  Relocations also recover real ownership for
  `__ctype_map`, `__lower_map`, `synthVoice`, the aux callback and MIDI globals,
  `synthFlags`, `vs`, and `gWriteBuf`. The address, definition, and target
  relocation ledger is retained in `docs/easy_ports_wave.md`.

The 148 current DOL fallback owners and their causes are exhaustive in the
two-bucket ledger above. Three REL Runtime variants also remain `NonMatching`;
they are outside this main-DOL-first taxonomy and are not included in the 147
C-work/1-assembly-policy remaining counts.
