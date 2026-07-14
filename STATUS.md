# Mario Party 6 recovery status

This is an evidence snapshot, not a completion claim. It was last verified on
2026-07-14 with the pinned project toolchain.

## Verification

- `ninja -j1`: `137 files OK` (the DOL and all configured REL outputs)
- `build/GP6E01/main.dol` SHA-1:
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`
- DTK progress at that build: 7.04% code and 24.84% data overall; 34.87% code
  and 57.76% data in the DOL
- Matching owners at that build: 202 of 895 overall and 193 of 396 in the
  DOL
- C and C++ inline assembly is limited to the one authenticated `PushLight`
  block described below

The exact build result includes extracted original objects and explicit
standalone assembly fallbacks for owners that are not yet byte-identical C.
Those owners remain `NonMatching` in `configure.py`; fallback-linked code is
not counted as decompiled source.

## `game/` ownership

`game/` is not fully decompiled. The current `Game` library has 54 of 61
object owners configured as matching. The remaining owners are:

- `game/init.c`
- `game/jmp.c`
- `game/malloc.c`
- `game/hsfdraw.c`
- `game/hsfanim.c`
- `game/kerent.c`
- `board/board.c`

`game/mic.c` is matching and is linked from recovered C.

## Assembly fallback boundary

Inline assembly is admitted only when an authenticated sibling project marks
the same owner `Matching`, carries the same source shape, and the MP6 compile,
relocation, and linked-container gates all pass. The sole current exception is
the 18-instruction paired-single `PushLight` helper in `GXLight.c`, imported
from Matching Pikmin 2 SDK reference commit `46aecad6`. Other
architecture-specific code remains isolated from source owners:

- the main and three REL runtime variants use explicit standalone `.s`
  fallbacks under `asm/`
- low-level context, interrupt, cache, startup, and timebase routines use DTK's
  extracted original-object fallback while their owners remain `NonMatching`
- THP paired-single decoder routines use extracted original-object fallback
- recovered scalar matrix, vector, and quaternion SDK implementations compile
  as clean C but remain `NonMatching` where the target used paired-single code

Promotion requires a compiled source object with matching bytes and
relocations. A successful container checksum alone does not promote a source
owner.

## Named DOL ownership

The branch is based on fork commit `353fa30`, which replaces every DOL
`auto_*` blob with 121 named Runtime, MSL, MusyX, MetroTRK, and support-library
owners. Neither `config/GP6E01/splits.txt` nor `configure.py` contains an
`auto_*` owner.

## Native library recovery

The current recovery series promotes 51 newly named DOL owners from
authenticated source. Together they account for 21,788 code bytes and 11,208
configured data bytes. The semantic source span is 32,976 bytes; the remaining
20 bytes are documented alignment tails in the `errno`, `nubinit`, `GXLight`,
and `uart_console_io` splits.

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
  632 data bytes.
- Eleven fdlibm owners: `s_tan`, `s_frexp`, `k_sin`, `s_cos`, `s_sin`,
  `s_modf`, `s_floor`, `s_ldexp`, `e_atan2`, `e_fmod`, and `k_rem_pio2`.
  Their 7,032 text bytes, 392 constant bytes, and 115 relocations match.
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
  bytes, 15 functions, and 51 relocations match. Authenticated GC/2.6
  `-inline deferred` emission produces the target function order without
  reordering source; `abort_exit` owns the exact `__atexit_funcs` table and
  four small-data globals.

Candidates rejected at the first structural mismatch remain `NonMatching`.
This includes `s_atan` (`0x200` target text versus `0x218` sibling source) and
MusyX `snd_init` (`0x120` target `sndInit` versus `0x118` sibling source with a
missing target call). Odemu `DebuggerDriver` remains rejected because its best
authenticated build has exact aggregate size and function sizes but the entire
function order is reversed. TRK `nubevent` and `serpoll` remain deferred rather
than naming an unproven queue field or inline helper.
