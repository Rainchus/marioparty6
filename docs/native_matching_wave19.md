# Native matching wave 19: MusyX DVD callbacks

This ledger records exact function-level source recovery inside the still-
fallback-linked `msm/msmstream.c` owner. The frozen base is `fork/main` commit
`650f59f4cb089f6c764a09a9559398897ce00480`. Tests used DTK 0.9.2 commit
`4d039140f2d2ed80572b1949b76a5ff9b3094e06`; the local `dtk.exe` SHA-256 is
`94a3ae31212d070d1ae72bd51461e7c361b46820fd620750576f7b61a9df7108`.

## Accepted recovery: `msm/msmstream.c`

Both asynchronous DVD callbacks receive the completed read result as their
first argument. The target preserves that value through the slot search and
branches on `result > 0`; it does not call `DVDGetCommandBlockStatus` again.
Recovering that control flow removes the extra call, frame growth, and saved
register from `msmStreamDvdCallback2`.

The main callback's status-5 path also calls `sndStreamDeactivate`, clears
`shutdownF` and `streamOffF` with a chained assignment, and then closes the
slot. Those operations are visible directly in the target's call and bitfield
masks. Finally, declaring `readSize` before `slot` in the shorter callback
reproduces its target `r8`/`r9` allocation.

| Function | Target/source bytes | Objdiff result |
| --- | ---: | --- |
| `msmStreamDvdCallback` | `1100` / `1100` | 100%, zero instruction or relocation differences |
| `msmStreamDvdCallback2` | `260` / `260` | 100%, zero instruction or relocation differences |

Together these functions account for `0x550` bytes of exact source. The fresh
whole-object comparison has 23/28 exact functions and raises raw `.text`
similarity from 96.71663% to 97.29357%. The configured target/source `.text`
sizes remain `0x3290`/`0x3648`, so the owner is not eligible for promotion.
The target and source object Git blobs are
`77d72f4c9624247c0d7f6a3deca1aaf8d3fcf737` and
`755ac254e1c1d96ed9903d0b51c07a0b81d30145`, respectively.

The five unresolved functions are:

- `msmStreamSetParam`: 652 bytes, 98.55828%.
- `msmStreamStopAll`: 180 bytes, 99.22222%.
- `msmStreamPauseOff`: 720 bytes, 92.80556%.
- `msmStreamData`: target/source `748`/`808` bytes, 64.63636%.
- `msmStreamSlotInit`: 548 bytes, 95.766426%.

The owner remains NonMatching and is not counted as decompiled source.

## Rejected nearby probes

Reversing the `slot` and `i` declarations in `msmStreamStopAll` expanded its
five-register allocation cycle to eleven differences and was reverted.
`board/board.c` was also rechecked as the other near-matching owner:
declaration-order changes could trade exactness between `mbNextTimeSet` and
`mbNextTime`, but did not produce a net exact function. The old function-scope
`s8 handicap` form for `mbSaveInit` is disproven by its eight-byte size deficit
and extra conversion register. No board probe was retained.

## Full gate

The final source-object comparison was followed by the serial project build
and both independent container checks:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The final result is `137 files OK`; the two DOLs compare byte-for-byte and the
built `main.dol` SHA-1 is
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. The final build was also checked
for generated `symbols.txt` changes before staging.
