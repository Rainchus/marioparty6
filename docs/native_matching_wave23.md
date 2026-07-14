# Native matching wave 23: object and stream control flow

This wave recovers one exact function and two target-proven control/data-flow
slices inside owners that remain fallback-linked. The frozen base is
`fork/main` commit `310e5d02e35d4f7340f6aae68f6f838c9fea8e2b`.
Tests used the pinned DTK 0.9.2 toolchain.

Neither owner is configured `Matching`, so none of its fallback-linked bytes
are counted as a decompiled owner. No assembly, padding, dummy data, or
synthetic split-label global was added.

## Exact `board/object.c` slice

`ObjManModelCreate` now reproduces the target's caller-local data-directory
selection. The target branches on `GwSystem.curTime`, swaps the day/night
directory while retaining `FILENUM(dataNum)`, and joins at the heap-read call.
Expressing that control flow locally removes the source-only move emitted
after auto-inlining `mbObjDataNumGet` and restores the target register-save
boundary.

| Evidence | Before | After |
| --- | ---: | ---: |
| exact functions | 77/80 | 78/80 |
| `ObjManModelCreate` | target/source `0x1C0/0x1C4`, 98.660710% | `0x1C0/0x1C0`, 100%, zero instruction or relocation differences |
| owner `.text` | target/source `0x2838/0x2844` | `0x2838/0x2840`, 99.812744% |
| owner relocations | 334/334 | 334/334 |

The two remaining functions are deliberately not reshaped further:

- `mbObjMotionNoCreate`: target/source `0x128/0x12C`, 96.743240%.
- `mbObjMotionCreate`: target/source `0x150/0x154`, 97.130950%.

Both still contain one source-only move after the same auto-inline, followed
by stack-frame and model-ID register fallout. Repeating the recovered local
control flow made `mbObjMotionNoCreate` the target size but left allocator-only
differences at 98.297295%; that incomplete probe and a regressing cast probe
were reverted.

## Recovered `msm/msmstream.c` behavior

### `msmStreamData`

The previous source had the wrong busy-DVD path: it inlined a stream-mix
parameter update and omitted linked-slot handling. Target disassembly proves
this sequence instead:

1. pause the primary stream with `msmStreamPauseOn(streamNo, 0)`;
2. clear its complete stream buffer and update its complete ARAM stream;
3. choose `slotL`, otherwise `slotR`, otherwise no linked slot;
4. for a valid linked slot not already marked for ARAM update, set its flag,
   pause it, copy the primary pause length, clear its buffer, and update its
   complete ARAM stream.

The target ready path also advances the stream and issues the asynchronous DVD
read directly; it does not clear the ARAM-update flag or call
`msmStreamUpdateBaseParam` there. Recovering those memory effects and calls
changes the source from `0x328` and 64.636360% to the exact target size
`0x2EC` and 99.759360%. The nine remaining differences are one linked-slot
pointer register allocation (`r26` in source versus target `r25`); two
declaration-order/scope probes were byte-neutral and reverted.

### `msmStreamSlotInit`

The target keeps three distinct values alive: half-buffer size, the selected
read size, and the wrap-distance remainder. An explicit `halfSize` local and
use of the selected `ret` value for the zero-fill offset recover that
dependency structure. Target/source remain `0x224/0x224`; objdiff improves
from 95.766426% with mixed insert/delete/replace differences to 99.635040%
with eight argument-only register differences. Declaration reorder and donor
temporary-type probes were neutral or regressing and were reverted.

The owner remains 23/28 exact functions. Whole `.text` improves from the
wave-19 97.293570% to 99.487020%; final target/source sizes are
`0x3290/0x360C` and relocation counts are 213/233. The other three divergent
functions are bounded allocator/control-scheduling work:

- `msmStreamSetParam`: `0x28C/0x28C`, 98.558280%, argument-only differences
  in the second auto-inline of `msmStreamUpdateBaseParam`;
- `msmStreamStopAll`: `0xB4/0xB4`, 99.222220%, five induction/pointer register
  swaps;
- `msmStreamPauseOff`: `0x2D0/0x2D0`, 92.805560%, localized half-size and
  wrap-offset scheduling differences.

## Bounded neighboring audits

Fresh `board/audio.c` proof still has 46/49 exact functions and only fourteen
allocator operands: one compare-operand reversal each in `mbMusBoardPlay` and
`MusBoardFade`, plus twelve spill-slot offsets in `mbMusBoardFadeOut`. The
already-exhausted source-shape probes only moved those allocations, so no
change was retained.

Fresh `msm/msmsys.c` proof remains 18/23 exact. Four residual functions are
frame/register allocation problems. `msmSysInit` already has identical
instruction words and relocation types/addends; its two objdiff items are
only target split-label identity versus the compiler's constant-pool symbol.
No fake source-level split global was introduced.

## Validation

The two changed source objects were rebuilt and compared with objdiff. The
final retained tree was then checked with:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The configured build remains `137 files OK`; the original and rebuilt DOLs
compare byte-for-byte, and the built `main.dol` SHA-1 is
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. This is a validation result, not
a Matching promotion for either fallback owner.
