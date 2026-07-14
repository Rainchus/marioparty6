# Native matching wave 18: delayed board audio

This ledger records exact function-level source recovery inside an owner that
still links its fallback object. The frozen base is `fork/main` commit
`57aeb275795c8eea84d164eaf59a19d86bc087ae`. Tests used DTK 0.9.2 commit
`4d039140f2d2ed80572b1949b76a5ff9b3094e06`; the local `dtk.exe` SHA-256 is
`94a3ae31212d070d1ae72bd51461e7c361b46820fd620750576f7b61a9df7108`.

## Accepted recovery: `board/audio.c`

The reconstructed `AudFXPlay` abstraction compiled the delay queue with the
wrong call-site lifetimes. The recovered form keeps only the small
`AudFXPosSet` position-copy helper and spells out the delayed and immediate
paths in each public wrapper. `mbAudGuidePlay` requires two separately scoped
delay snapshots around its table-selected and direct-ID paths. This source
shape reproduces the target code and relocations for all four affected
functions.

| Function | Target/source bytes | Objdiff result |
| --- | ---: | --- |
| `mbAudFXPlay` | `216` / `216` | 100%, zero instruction or relocation differences |
| `mbAudFXPosPlay` | `236` / `236` | 100%, zero instruction or relocation differences |
| `mbAudFXEmitterPlay` | `220` / `220` | 100%, zero instruction or relocation differences |
| `mbAudGuidePlay` | `632` / `632` | 100%, zero instruction or relocation differences |

Together these functions account for `0x518` bytes of exact source. The fresh
whole-object comparison has 46/49 exact functions. Target and source `.text`
are both `0x2BF0` bytes at 99.98862%, and `.rela.text` is `0x14D0` bytes on
both sides (444 relocations, no relocation differences). The target and source
object Git blobs are `fc17cf43812e7000371dfb4014843ac155178714` and
`b0970169f1e94b8a11151a09cfce8e2993e24d58`, respectively.

The owner remains NonMatching and is not counted as decompiled source. Its
three unresolved functions are:

- `mbMusBoardPlay`: target/source `300`/`300` bytes, 99.86667%; target
  `cmplw r0,r29`, source `cmplw r29,r0`.
- `mbMusBoardFadeOut`: target/source `1008`/`1008` bytes, 99.95238%; twelve
  stack-slot offsets differ, with no inserted, deleted, or replaced
  instructions.
- `MusBoardFade`: target/source `428`/`428` bytes, 99.90654%; the same single
  compare-operand reversal as `mbMusBoardPlay`.

The configured `.sdata` and `.sbss` ends also include four-byte alignment
gaps that the compiler does not emit. No fake padding was added.

## Rejected probes

Swapping the source equality operands in both board-music loops is compiler-
neutral and leaves the generated comparisons unchanged. Pointer-alias order,
initializer scope, loop-entry pointer, direct-board-number, cast, and branch-
shape variants either left the three differences unchanged or regressed other
instructions; all were reverted. Replacing the duplicated `MusBoardFade`
selection block with an auto-inlined `mbMusBoardPlay` call was also byte-
neutral. The explicit `else` chains in Mario Party 5 commit `e246f9d`,
`src/game/board/audio.c`, for `mbMusCheck`, `mbMusStopCheck`, and
`mbMusEndCheck` left both the standalone functions and the hidden inline stack
slots unchanged, so they were reverted.

An `msm/msmsys.c` declaration-order probe improved a partial similarity score
but did not make any additional function exact and has no independent source
authentication. It was reverted rather than retaining a hypothesis.

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
