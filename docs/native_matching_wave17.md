# Native matching wave 17: board status

This ledger records the accepted source recovery and the nearby work that was
deliberately left NonMatching. The frozen base is `fork/main` commit
`5e2367556f09d53d480e85c61a180a068207a689`. Tests used DTK 0.9.2 commit
`4d039140f2d2ed80572b1949b76a5ff9b3094e06`; the local `dtk.exe` SHA-256 is
`94a3ae31212d070d1ae72bd51461e7c361b46820fd620750576f7b61a9df7108`.

## Accepted recovery: `board/status.c`

The remaining function was `StatusUpdate`. The exact matching form keeps the
outer loop locals alive and reuses them in the later single-board path.
Reconstructed `masuNo` and `digit10` locals changed those lifetimes and
register allocation;
using the existing `i` and `j`, together with explicit boolean comparisons for
the two `GWPartyGet` branches, reproduces the target instruction stream.

The original split also attributed `0x802485A4..0x802485A8` to the owner. Those
four zero bytes are linker alignment before `board/opening.c`, whose data
starts at `0x802485A8`. The source object proves the semantic status data ends
at `0x802485A4`; the split was narrowed instead of adding fake source padding.

| Evidence | Result |
| --- | --- |
| `StatusUpdate` | target/source `1816`/`1816` bytes, 100%, zero instruction differences |
| Functions | 44/44 exact |
| Sections | `.text 0x3CB8`, `.rodata 0x18`, `.data 0x3F4`, `.bss 0xD8`, `.sbss 0x8`, `.sdata2 0x58` |
| Relocations | 807/807 exact |
| Integrated object identity | Git blob `598efea95e667691f504d363ea2fed3716c93c2b`, identical to the isolated proof worktree object |

## Retained improvement: `board/branch.c`

This owner was not flipped. Reusing function-scoped `i` for the tutorial call
result and declaring `padNo` before `masuPlayer` are independently supported by
the generated code: they remove the stack spill and unrelated register cycle
without changing behavior.

Before those corrections, `ev_Branch` was target/source `2288`/`2296` bytes at
98.6014%. It is now `2288`/`2288` bytes at 99.816440%, with no inserted,
deleted, or replaced instructions. The 19 remaining operand differences are a
single coupled allocation: target `choiceTime`/`choice` use `r26`/`r27`, while
the current source uses `r27`/`r26`. All other 16 functions, whole-owner
`.text 0x10E0`, and 210/210 relocations are exact. Configured `.data` is still
target/source `0x18`/`0x12`; no ownership adjustment is accepted while the
text gate still fails. Declaration-order, initializer, rename, and
narrower-scope probes that did not resolve the cycle were reverted. The
integrated proof object is Git blob
`4a9a695af91760ce271fb7a2790ef1495ad0b677` and is identical to the isolated
proof worktree object.

## Rejected nearby probe: `board/object.c`

The baseline remains 77/80 exact functions with 334/334 relocations. Parameter
width, accessor, explicit-narrowing, declaration-lifetime, and named-return
probes were neutral or regressed, so every probe was reverted. No source was
retained and the owner remains NonMatching.

## Full gate

The accepted flip is gated by the serial project build and both independent
container checks:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The final result is `137 files OK`; the two DOLs compare byte-for-byte and the
built `main.dol` SHA-1 is
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. The final build was also checked
for generated `symbols.txt` changes and duplicate `Object()` entries before
staging.
