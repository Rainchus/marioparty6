# Native matching wave 53: OSExec closure and Board utility recovery

Wave 53 closes `dolphin/os/OSExec.c` under the authenticated-source assembly
exception and materializes every easy Board utility tail found in the target
and donor sweep. The Board batch adds 39 mapped functions across seven owners;
28 are byte-exact and no previously mapped function regresses. Those owners
remain `NonMatching` because their larger state-machine closures are absent or
divergent.

## `dolphin/os/OSExec.c`: complete retained owner

[dolsdk2004 commit `2328b416`](https://github.com/doldecomp/dolsdk2004/blob/2328b4164b1a98422a2255d83ce9a5a7548990cc/src/os/OSExec.c)
reports the vendor OS library 100% complete and carries `Run` as a single
`nofralloc` assembly routine. Its 15 instructions and `0x3C` size are identical
to the MP6 target: save the entry, invalidate the instruction cache, execute
`sync`/`isync`, transfer through LR, and retain the unreachable epilogue.

The prior C spelling emitted `0x38`, was inlined into `BootDolSimple`, and left
only 2/8 retained functions exact. Restoring the authenticated source shape
makes every retained function exact, including effective relocations:

- `PackArgs 0x188`
- `Run 0x3C`
- `ReadDisc 0x6C`
- `Callback 0xC`
- `GetExecParams 0x40`
- `GetApploaderPosition 0xC4`
- `BootDolSimple 0x484`
- `BootDol 0x19C`

Target/source whole `.text` are `0x960/0xD28` with 90/129 total relocations.
The source-only SDK helpers and their relocations are linker-stripped. Target/
source `.data` are `0x10/0xB` and `.sdata` are `0x8/0x3`: the semantic version
and format strings agree and the target owns the split tail padding. `.sbss`
is exact at `0x8`. The final linked range and complete DOL are byte-identical,
so the owner is Matching under the authenticated-vendor exception. Its
assembly is not counted as clean decompiled C.

## `dolphin/exi/EXIBios.c`: target-backed source corrections

The exact dolsdk2004 source restores three `(REG & 1) == 1` busy-wait tests in
`EXIInit`; the target explicitly contains those comparisons. `EXIInit` becomes
exact-size `0x1D4` and improves from 71.410255% to 74.196580%. Matching M4 and
dolsdk2004 both authenticate the controller-local `EXIGetState` source shape;
the exact-size `0x18` function improves from 62.500000% to 64.166664%.

Whole target/source `.text` move from `0x19DC/0x20F4` at 68.572205% to
`0x19DC/0x2100` at 68.775220%. The exact count remains 1/23 and relocations
remain 187/219, so the owner remains fallback-linked. The dolsdk2004 Makefile's
owner-local `-O3,p` override was tested but changed no function tuple beyond
the source edits; the byte-neutral flag was discarded.

## Board easy-win batch

Every body below is proved by MP6 instructions, calls, or data. Matching MP5
`sai.c` authenticates the dice table/getter family. MP5 capsule sources are
Equivalent corroboration only; they are not used as authentication or as a
substitute for MP6 evidence.

| Owner | Target/source `.text` before -> after | Target-text score before -> after | Mapped/exact before -> after |
| --- | --- | --- | --- |
| `board/capevent.c` | `0x14EFC`, `0x178 -> 0x1C8` | 0.438453% -> 0.531741% | 11/11 -> 28/28 |
| `board/capsule.c` | `0x146CC`, `0x1130 -> 0x1148` | 5.065790% -> 5.094478% | 29/18 -> 33/22 |
| `board/config.c` | `0x63BC`, `0x560 -> 0x56C` | 5.294532% -> 5.341532% | 5/4 -> 6/5 |
| `board/dice.c` | `0x6390`, `0x130 -> 0x254` | 0.508475% -> 0.794884% | 6/0 -> 15/1 |
| `board/single.c` | `0x98F0`, `0x2B4 -> 0x304` | 1.049244% -> 1.234675% | 7/3 -> 11/6 |
| `board/star.c` | `0x5C00`, `0xDC -> 0xE0` | 0.934103% -> 0.951087% | 16/16 -> 17/17 |
| `board/wipe.c` | `0x3664`, `0x6DC -> 0x788` | 12.611319% -> 13.604712% | 14/14 -> 17/15 |

The recovered source includes authentic empty/constant capsule-event tails;
the capsule throw hook and stubs; three exact 21-byte dice tables and their
getters; the semantic special-NPC bitfield overlay on `OMOBJ` work; single-mode
prize, opponent, and team state; the pause-guide kill flag; the star no-display
stub; and the complete `0x2C` `WIPE_SPECIAL_DATA` layout used by check, wait,
and status accessors. Exact-function comparisons use relocation data values,
not coincidental local symbol names.

## Rejected MIC and compiler probes

The gc-mic definition order and direct-return wrappers were authentic source
shapes, but exposing the asynchronous definitions to `-inline auto` caused
the available compiler to inline three wrappers. Target/baseline/candidate
`.text` were `0x2020/0x1F40/0x2178`; the candidate score fell from 56.653% to
52.785020%, relocations rose from 291 to 309 versus target 299, and exact
functions stayed 0/41. `MICSetGain`, `MICStart`, and `MICStop` expanded from
baseline `0x44` to `0x108`, `0x128`, and `0xD4` versus target `0x48`.

GC/2.0 produced the same regression. Although it reports compiler metadata
`2.4.7.1`, it emits metadata version 10 rather than the target's version 11 and
does not reproduce the target inliner. The source candidate and compiler
override were removed. `m2s.c` was left unchanged because the target proves a
different history-copy length but no authentic original expression was found.

## Full gate

- `ninja -j1`: `137 files OK`
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- Progress: 9.12% code and 31.30% data overall; 46.76% code and 74.41% data
  in the DOL; 300/895 owners overall and 291/396 DOL owners Matching

