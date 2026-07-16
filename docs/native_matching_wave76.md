# Native matching wave 76: Board state and scroll closures

Wave 76 closes seven represented Board functions totaling `0x968` target text
bytes. It restores the player-collision bitfield initialization order, the Star
object's paired sentinel assignment, the Telop current-time predicate, an
MP5-authenticated scroll-view rotation helper, and the capsule-scroll wrapper.
No owner is promoted: Player, Star, and Telop retain represented or absent
work, while Scroll still lacks 14 target functions. Fallback-linked bytes are
not counted as decompiled source.

## Authorities and recovery boundary

The MP6 target objects, instructions, relocations, symbols, bitfield accesses,
and complete function ranges are authoritative. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` authenticates the shared Board
source family. Its `src/game/board/player.c` initialization order and
`src/game/board/scroll.c` rotation helper were retained only where MP6 proved
the corresponding state and ABI.

The local `mwcc-decomp` checkout at
`094b96ca1df4a035b5f93c351f773306c0241f3f` was used to distinguish source
order from allocator residue and to verify the nested-auto-inline source
shape. No assembly, byte body, register forcing, volatile match trick, opaque
work buffer, or compiler sweep was introduced.

## Player collision initialization

Matching MP5 `PlayerPosFixInit` assigns `motStartF` before `killF`. MP6 adds a
real `state` field between those writes, and its target instructions prove the
order `motStartF`, `state`, `killF`. Restoring that order in
`PlayerColCornerSnap` closes the out-of-line helper and both callers that pack
it through auto-inlining:

- `PlayerColCornerSnap` (`0x15C`);
- `mbPlayerPosReset` (`0x178`);
- `mbPlayerPosResetAll` (`0x228`).

All three become exact with no loss from the previous 134-function exact set.
Player reaches 137/165 exact functions.

## Star object sentinel assignment

`mbStarObjCreate` loads `-1` once, then stores it to `playerNo` at offset
`0x10` before `masuId` at offset `0x0C`. The reversed chained assignment
`work->masuId = work->playerNo = -1` is the C expression that preserves both
the single load and the target's right-to-left store order. The function
becomes exact at `0x78` and Star reaches 65/90 exact functions.

Two independent statements were compiled and rejected: MWCC emitted a second
`li -1`, grew the function, and reduced it to 96.666664%. That spelling is not
present in the retained source.

## Telop current-time predicate

The target `mbTelopTimeStarSet` extracts `GW_SYSTEM::curTime`; the previous
source extracted the adjacent `nextTime` bit. Using the target-proven field
makes the complete `0xCC` function exact. Telop now has 16/31 represented
functions and 10 data-value-exact functions.

## Scroll ports

MP5 `src/game/board/scroll.c:300-305` authenticates `RotateScrollView`'s
trigonometric expression and assignment order. MP6's complete
`0x80171A1C..0x80171B4C` range proves the adapted three-vector ABI. The port is
exact at `0x130`.

`mbev_ScrollCapsule` is the one-line wrapper
`mbev_Scroll(playerNo, FALSE)`. Its target range
`0x80174CC4..0x80174EBC` is instruction-for-instruction the nested auto-inline
expansion of this translation unit's already exact `mbev_Scroll` and
`ScrollMain`, including hooks, window cleanup, wipe selection, and the
constant-folded initial mode. It is exact at `0x1F8`.

All 17 represented Scroll functions are now exact under the required
data-value relocation comparison. Fourteen target functions remain absent.
The map-sprite family was deliberately not ported: MP6 proves a `0x44` entry
and `0x894` owner that diverge from MP5, but not enough field semantics to
declare a complete source structure without guessing.

## Bounded rejected probes

The batch also established three no-change boundaries:

- `mbNextTime` can be made exact only by regressing the already exact
  `mbNextTimeSet` inline source by the same three operands. MP5 has no
  authenticated alternate day/night accessor, so neither body changed.
- `mbMain` retains a six-operand party/day register-color cycle. All
  authenticated scope and temporary forms are already measured as neutral or
  worse; no synthetic getter was added.
- Swapping `mbMasuPlayerColorSet`'s adjacent `partyF` and `type` declarations
  regressed the function from 99.868420% to 99.736840%. The probe was reverted.

## Object and container evidence

Objdiff CLI `3.7.2`, using `functionRelocDiffs=data_value`, reports:

| Owner | Target/source `.text` | Data-value score | Paired | Exact |
| --- | ---: | ---: | ---: | ---: |
| `board/player.c` | `0xBEB0/0xBDC0` | 96.350130% | 165/165 | 137 |
| `board/star.c` | `0x5C00/0x5A08` | 93.604110% | 90/90 | 65 |
| `board/telop.c` | `0x329C/0x8E0` | 16.132448% | 16/31 | 10 |
| `board/scroll.c` | `0x3E4C/0x930` | 14.747931% | 17/31 | 17 |

No exact function regressed. The final serialized full build and explicit DTK
checksum both report `137 files OK`; `cmp` confirms the rebuilt and original
`main.dol` are byte-identical. No REL changed. The final build leaves
`config/GP6E01/symbols.txt` unchanged.
