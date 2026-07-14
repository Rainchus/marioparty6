# Mario Party 6 recovery status

This is an evidence snapshot, not a completion claim. It was last verified on
2026-07-14 with the pinned project toolchain.

## Verification

- `ninja -j1`: `137 files OK` (the DOL and all configured REL outputs)
- `build/GP6E01/main.dol` SHA-1:
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`
- DTK progress at that build: 8.38% code and 26.73% data overall; 42.54% code
  and 62.62% data in the DOL
- Matching owners at that build: 254 of 895 overall, 245 of 396 in the DOL,
  and 9 of 499 in the REL modules
- Every tracked C or C++ inline-assembly owner is `Matching` and has the
  authenticated sibling provenance described below

The exact build result includes extracted original objects and explicit
standalone assembly fallbacks for owners that are not yet byte-identical C.
Those owners remain `NonMatching` in `configure.py`; fallback-linked code is
not counted as decompiled source.

## `game/` ownership

`game/` is not fully decompiled. The current `Game` library has 59 of 61
object owners configured as matching. The remaining owners are
`game/kerent.c` and `board/board.c`.

`game/mic.c` is matching and is linked from recovered C.

## Assembly fallback boundary

Inline assembly is admitted only when an authenticated sibling project marks
the same owner `Matching`, carries the corresponding instruction body, and the
MP6 compile, effective-relocation, linked-range, and container gates all pass.
The current tree has 25 such object owners:

- 17 Dolphin SDK owners authenticated by Matching Mario Party 4 sources, with
  `mtx44vec` independently authenticated by Matching Mario Kart: Double Dash
- `game/jmp.c` and `game/malloc.c`, plus their authenticated `OSFastCast`
  closure
- the DOL Runtime owner, `__init_cpp_exceptions`, and three low-level TRK
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

## Named DOL ownership

The branch is based on fork commit `353fa30`, which replaces every DOL
`auto_*` blob with 121 named Runtime, MSL, MusyX, MetroTRK, and support-library
owners. Neither `config/GP6E01/splits.txt` nor `configure.py` contains an
`auto_*` owner.

## Native library recovery

The current recovery series contains 103 accepted DOL owners: 97 net-new
promotions relative to `fork/main` plus six restorations of owners that were
already `Matching` there. Together they account for 188,368 code bytes and
46,100 configured data/BSS bytes. The latest consolidated wave contributes 52
of those owners, 166,580 code bytes, and 34,860 data/BSS bytes.

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

Unpromoted candidates remain `NonMatching`. Known candidates include MSL
`file_io`, `mbstring`, `mem_funcs`, `e_pow`, and `s_atan`; Runtime
`Gecko_ExceptionPPC`; MusyX `snd_service` and `snd_init`; Odemu
`DebuggerDriver`; Dolphin `EXIBios`; `game/kerent.c`; and the three REL Runtime
variants. Per-candidate failure counts and causes are omitted unless a durable
proof artifact makes them reproducible.
