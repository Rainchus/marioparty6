# Native matching wave 26: board object closure

This ledger records the recovery and promotion of `src/board/object.c`. The
frozen base is `fork/main` commit
`f69d158d1dc69bacaa71d6e5c05d584323f6d1e6`. Tests use the pinned DTK 0.9.2
binary (`4d039140`) and the configured `GC/2.6` compiler.

The owner is now Matching clean C. No assembly, byte arrays, artificial
padding, register forcing, or source-level fake operations were added.

## Residual function recovery

Before this wave, 78 of the owner's 80 functions were exact. The residual
functions were both four source bytes larger than their targets. They now
match completely:

| Function | Target/source | Result |
| --- | ---: | ---: |
| `mbObjMotionNoCreate` | `0x128` / `0x128` | 100% |
| `mbObjMotionCreate` | `0x150` / `0x150` | 100% |

Two independent facts determine the recovered source shape.

First, both target functions keep the incoming `modelId` lifetime and replace
it with the newly created motion ID before storing it in `motId[motNo]`.
Mario Party 4 commit `147b165a83187ac9e6cfdc3bf52f2e73437b1ffd`, file
`src/game/board/model.c`, authenticates that exact pattern in
`BoardModelMotionCreate`: its `model` parameter receives both
`Hu3DJointMotion` and `CharMotionCreate` and is then stored. Mario Party 5
commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb`, file
`src/game/board/model.c`, independently preserves the same shape in
`MBMotionCreate` with the `modelId` parameter. Removing the unsupported
separate `motId` local produces the target register lifetime in MP6.

Second, the MP6 targets inline the day/night data-directory selection before
`HuDataSelHeapReadNum`; they do not call `mbObjDataNumGet`. The exact
instruction and branch sequence is already present in this owner in
`ObjManModelCreate`. Reusing that owner-local source shape with a
`readDataNum` lifetime makes both motion functions byte-exact. Local-variable
and label spellings are not asserted as independently recoverable facts; the
control flow, calls, values, and compiler output are target-proven.

## Small-data global ownership

The first promotion trial was rejected even though objdiff reported 100% for
the raw object sections. The full build stopped at `136 files OK`, and the
linked DOL differed inside `mbObjDataNumGet`. The cause was symbol placement in
the zero-filled `.sbss` section: equal section bytes do not prove equal global
addresses.

DTK symbol inspection established the target offsets:

| Global | Target `.sbss` offset |
| --- | ---: |
| `dataDirNight` | `0x0` |
| `dataDirDay` | `0x4` |
| `objManDispNum` | `0x8` |
| `objManNum` | `0xC` |
| `objManData` | `0x10` |
| `objManOMObj` | `0x14` |

`GC/2.6` emits these translation-unit statics in reverse declaration order.
Reordering the declarations to reproduce the observed target offsets fixes
the effective small-data relocations. A fresh source object reports the same
six offsets and 334 relocations.

## Whole-owner object proof

The final target/source comparison is exact:

| Evidence | Target | Source | Result |
| --- | ---: | ---: | --- |
| `.text` | `0x2838` | `0x2838` | 100% |
| Functions | 80 | 80 | all 100% |
| `.sbss` | `0x18` | `0x18` | exact symbols and size |
| `.sdata2` | `0x48` | `0x48` | 100% |
| `.rela.text` | `0xFA8` | `0xFA8` | 334/334 relocations |

The fresh machine-readable comparison was generated as
`build/proof_wave26_board_object_globals.json`; build outputs are not tracked.

## Rejected `board/audio.c` probes

`board/audio.c` was screened before moving to the object owner. Its whole
`.text` remains target/source `0x2BF0`/`0x2BF0` with all 444 relocations and
46 of 49 functions exact. The three residuals are bounded to one reversed
`cmplw` in each of `mbMusBoardPlay` and `MusBoardFade`, plus twelve stack-slot
offsets in `mbMusBoardFadeOut`.

Operand-order and cast probes were byte-neutral. Direct field access, changed
signed-local types, direct allocator assignment, and narrower allocator
scoping either regressed the object or were neutral. Every probe was reverted;
`board/audio.c` remains `NonMatching` and this wave retains no unsupported
source change for it.

## Full gate

After enabling only `board/object.c`, the complete build was regenerated
serially:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The final result is `137 files OK`; the DOLs compare byte-for-byte and the
built DOL SHA-1 is `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. DTK reports
268/895 Matching owners overall and 259/396 in the DOL. The final build was
also checked for generated `symbols.txt` changes before staging.
