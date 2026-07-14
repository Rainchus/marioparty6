# Native matching wave 40

Wave 40 closes the board `NO-SOURCE` inventory. It adds 58 source functions
across `capmove.c`, `capthrow.c`, `captrap.c`, `capspecial.c`, `capevent.c`,
and `mgcall.c`. One serial compile and one six-owner DTK/objdiff pass pair all
58 functions with retained MP6 targets. Fifty are byte-exact and cover
`0x494` target text bytes; all paired functions cover `0x78C` target text
bytes.

No inline assembly, byte storage, register forcing, opaque padding, or
unknown/reserved structure members were added. The six incomplete owners stay
`NonMatching` and fallback-linked. Exact helper functions therefore do not
promote or inflate any full-owner Matching count.

## Recovery evidence

The MP6 split objects are authoritative: symbol boundaries, instruction
streams, calls, relocations, initialized data, indexed extents, allocation
sizes, and field loads/stores establish the recovered source. The local Mario
Party 5 snapshot is commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb`.
Its `configure.py` records `src/game/board/mgcall.c` and
`src/game/board/capmachine.c` as `Matching`, while
`src/game/board/capmain.c` is `Equivalent`. The Matching `mgcall.c` supplies
source-shape parity for the list/history helpers. The `capmain.c` getter and
hook forms are corroborating shape evidence only; the exact MP6 functions and
globals below are established independently by the MP6 target.

The recovered layouts and data are semantic:

- `TERESA_FADE_WORK` is exactly `0x28`: texture pointer/size, active and copy
  flags, alpha, owning `OMOBJ *`, and four screen/texture dimensions. The
  target allocation and every field access close all ten words without a
  synthetic tail.
- `MGLISTWORK` is `0x10`: the target bitfield word, four `s16` window/sprite/
  timing members, and the hidden-message ID. Roulette helpers address this
  work at the target object's work offset.
- The minigame-call history closure owns nine offsets, 16 four-player entries,
  two six-entry team arrays, two battle entries, fourteen duel entries, three
  four-player status-position arrays, and four list-object pointers. The
  recovered source reproduces the target owner's complete `.bss 0xD8`.
- Four exact status-position tables occupy the recovered `.rodata 0x70`.
  The nine history pointers and nine history sizes occupy the recovered
  `.data 0x48` with the target's null slots and extents.
- Capsule-event state recovers the `10`/`5` duel coin defaults, four `-1`
  player/space sentinels, the six-argument bubble hook, and the bank field in
  `GwSystem`.

The 27 empty capsule kill callbacks are not guessed stubs. Each named MP6
target is exactly four bytes and disassembles to a single `blr`; compiling an
empty `void` body reproduces every target function exactly.

## Batched object results

All comparisons use objdiff v3.7.3 with target on the left, source on the
right, and `functionRelocDiffs=data_value`. DTK is the project-pinned 0.9.2
build `4d039140f2d2ed80572b1949b76a5ff9b3094e06`.

| Owner | Target/source `.text` | Mapped/exact functions | Evidence retained |
| --- | ---: | ---: | --- |
| `board/capmove.c` | `0x6F68/0x24` | 9/9 | Nine capsule movement kill callbacks, each one exact `blr`. |
| `board/capthrow.c` | `0x8CB0/0x20` | 8/8 | Eight capsule throw kill callbacks, each one exact `blr`. |
| `board/captrap.c` | `0x6A40/0x14` | 5/5 | Five capsule trap kill callbacks, each one exact `blr`. |
| `board/capspecial.c` | `0xE744/0x1E8` | 13/9 | Teresa steal state, Miracle/Koopa hooks, and five empty kill callbacks are exact. The target-backed fade kill, fade object executor, fade setter, and Koopa motion hook are 92.826090%, 92.142860%, 88.214290%, and 91.142860%. |
| `board/capevent.c` | `0x14EFC/0x178` | 11/11 | Move/stop setters, bank and duel-coin accessors, bubble-hook set/call paths, and bank initialization are exact. |
| `board/mgcall.c` | `0x75FC/0x3DC` | 12/8 | Name-color scanning, roulette slide/focus/check, history lookup, single-mode wrapper, VS-effect cleanup, and the single-on getter are exact. Init and focus-kill are 99.965515%/99.875000%; data-close and battle-message selection are 87.307690%/89.000000%. |

The objdiff artifacts are retained under `build/GP6E01/` as
`wave40_board_<owner>.json`.

## Deliberate boundaries

This wave stops before unclosed event state:

- `capmove.c`, `capthrow.c`, and `captrap.c` retain only callbacks whose
  complete bodies are proven by their four-byte target ranges.
- Teresa fade work is closed, but the other large capsule-special event work
  objects and Miracle/Koopa state machines are not.
- `capevent.c` has 237 retained target functions; only the eleven closed
  state/hook helpers are represented.
- `mgcall.c` retains the list/history state needed by the recovered helpers,
  but the larger roulette, UI, and selection executors remain absent.

Those closures were omitted instead of being represented with guessed fields,
byte arrays, padding, or assembly.

## Full gate

- `ninja -j1`: `137 files OK`.
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`: `137 files OK`.
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical.

DTK remains at 292/895 Matching owners overall and 283/396 in the DOL. The
six recovered owners move from `NO-SOURCE` to `SRC-DIVERGES`; every one stays
`NonMatching` until its complete object, relocations, linked range, and final
container pass.
