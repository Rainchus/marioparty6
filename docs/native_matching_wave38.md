# Native matching wave 38

Wave 38 changes the recovery cadence from per-function probing to one source
harvest followed by one batched object comparison and one full container gate.
It materializes 142 retained target functions: 112 board functions and all 30
retained functions in `TRK_MINNOW_DOLPHIN/targimpl.c`. Eighty-four of those
functions are byte-exact. No register forcing, opaque padding, proof-only byte
storage, or unauthenticated assembly was added.

## MetroTRK owner promotion

Mario Party 5
[`src/TRK_MINNOW_DOLPHIN/targimpl.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/targimpl.c)
is a Matching authenticated sibling. Mario Party 4 carries the same source.
The restored source contains seven authentic inline-assembly routines:
`__TRK_get_MSR`, `__TRK_set_MSR`, `TRK_ppc_memcpy`,
`TRKInterruptHandler`, `TRKExceptionHandler`, `TRKSwapAndGo`, and
`TRKInterruptHandlerEnableInterrupts`. They are admitted only under the
sibling-authentication exception and are not clean-C recovery.

The MP6 `support.h` had retained only the `TRKRequestSend` declaration. The
Matching MP5 header and the already-Matching MP6 `support.c` definitions prove
the full `TRKSuppAccessFile` and file-support prototypes. Restoring the real
`u8` arguments adds the two target `clrlwi` coercions in
`TRKTargetSupportRequest`, changing it from source `0x200` at 97.815384% to an
exact `0x208` function.

All 30 retained target functions now match. Target/source `.text` are
`0x1894/0x1D54`, but objdiff maps the complete target text at 100%. The source
emits eight additional helpers totaling `0x4C4`: the three unused PPC-access
helpers, four step/trace helpers, and `TRKTargetReadInstruction`. MP4 and MP5
emit and linker-strip the same helpers, and their bodies are inlined into the
same retained callers. MP6 target disassembly independently proves those
inlined bodies. `.rodata 0x88` and `.data 0x30` are exact; target/source `.bss`
are `0x580/0x57C` because the target split retains a four-byte alignment tail.
The linked owner and complete container are byte-identical, so `targimpl.c` is
Matching under the exception. It is not counted as clean C.

## Board source harvest

The table records the single post-harvest DTK 0.9.2 comparison. Target function
counts include the unrecovered remainder of each owner.

| Owner | Recovered source in this wave | Target/source `.text` | Exact mapped functions |
| --- | ---: | ---: | ---: |
| `board/math.c` | 29 functions | `0x2A44/0x2230` | 1/51 (`mbMathClose`) |
| `board/telop.c` | 8 functions | `0x329C/0x8E0` | 6/16 total; 5 of the 8 additions |
| `board/coin.c` | 22 functions | `0x4550/0xF44` | 5/22 |
| `board/dice.c` | 6 functions | `0x6390/0x130` | 0/6 |
| `board/single.c` | 7 functions | `0x98F0/0x2B4` | 3/7 |
| `board/star.c` | 16 functions | `0x5C00/0xDC` | 16/16 |
| `board/tutorial.c` | 10 functions | `0x2C34/0x80` | 10/10 |
| `board/wipe.c` | 14 functions | `0x3664/0x6DC` | 14/14 |

`math.c` now represents 51 of 56 target functions. The additions cover board
math lifecycle, degree/radian trigonometry, X/Y/Z and arbitrary-axis matrix
construction, rotation/scale combinations, matrix concatenation, angle wrap,
and normalized/3D projection. Matching MP5 source shape and MP6/MP7 target
parity authenticate the retained expressions. `mbRandMod` and the cull/object
tail remain absent because their source ownership is not yet proven.

`telop.c` adds the telop-presence check and seven time-display helpers. The
target proves the eight-entry alpha table, sprite loops, four owner pointers,
and the two real bit fields used by the time-change work prefix. The larger OM
state machines remain absent.

`coin.c` recovers the complete `0x40` public `MBCOINOBJ`: position, rotation,
scale, alpha, and six words of caller-owned work. Target allocation proves the
`0x1144` bank with 64 attributes, 64 motion numbers, and 64 objects. The
`board/last5.c` target consumes the work region at `+0x28` as per-instance
delay and vertical-velocity storage, so it is not padding. The target also
proves the `0x160` model/bank owner, ID packing, attribute flags, allocation,
kill, transform, alpha, display, layer, and motion operations. MP7 has the
same `0x40` object stride and accessor instruction shapes.

`dice.c` recovers the five-slot result/process/hook arrays and six layout-free
accessors. Negative player numbers map to slot four while all-player polling
covers players zero through three. Matching MP5 `sai.c` and `sai.h` authenticate
the hook signatures and source shape; state-machine work remains absent.

`single.c` recovers the complete four-word, 128-bit minigame-unlock set and all
seven operations. Target instructions prove the `GW_MGNO_BASE` 601 adjustment,
word and signed-modulo bit selection, and write-through to `GWMgUnlockSet`.

`star.c` deliberately retains only layout-free functions: callback/global
setters, Num/Flag/Next accessors, target-empty legacy entry points, and the
no-random result. An initial object-wrapper draft required an incomplete
`STARWORK` prefix and was removed before compilation. All 16 retained functions
are exact.

`tutorial.c` recovers the real `0x14` call work (`scene`, `callNum`, `result`,
`stat`, and `mode`), the guide object, process-entry pointer, and exit flags.
`mbTutorialMultiCall` stores `callNum`, while `mbTutorialCall` decrements it and
waits for exhaustion; this proves the formerly unnamed field. All ten retained
accessors are exact.

`wipe.c` recovers the common create/wait wrappers and all twelve fixed-time and
caller-time fade, white-fade, and dissolve wrappers. MP6 calls and flags prove
the tutorial-exit suppression, wipe constants, black color application, and
sleep loops. All 14 functions are exact.

## Deliberate skips

The `0x50` `MBPLAYERWORK` stride is proven, but several intervening fields and
flag membership are not. The tempting player wrapper cluster was therefore
left untouched. `board/snpc.c` was also left absent: its apparent wrappers
depend on unrecovered `SNPCSAVEWORK`, `SNPCWORK`, and effect-work layouts, and
no local authenticated sibling source exists. `board/opening.c` offered only
isolated helpers rather than a substantial safe cluster.

## Gate result

The only promotion is the authenticated mixed C/assembly `targimpl.c` owner.
Every other new board owner remains `NonMatching` because the owner is
incomplete, even when every retained function is exact.

- `ninja -j1`: `137 files OK`;
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`;
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical.

DTK reports 292/895 Matching owners overall and 283/396 in the DOL. The DOL
policy split is 250 clean-C owners, 33 sibling-authenticated assembly-exception
owners, 112 `C-not-yet-matched` owners, and one `original-was-asm` owner. The
fallback source inventory is now 51 current candidates and 61 `NO-SOURCE`
owners.
