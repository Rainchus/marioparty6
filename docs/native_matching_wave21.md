# Native matching wave 21: MSL runtime easy ports

This wave promotes five whole MSL owners from authenticated Mario Party 4 or
Mario Party 5 source. The frozen base is `fork/main` commit
`05da86b0ef8e00e1f03c7f83a6c9b68a3e7539f8`. Tests used the pinned DTK 0.9.2
toolchain.

The donor libraries build these files with CodeWarrior `GC/1.3`. Direct
`GC/2.6` probes were rejected: `file_io.c` inlined `fflush` into `fclose`,
`e_pow.c` grew from target `0x88C` to `0x8B4`, and `s_atan.c` grew from target
`0x200` to `0x218`. Per-owner `GC/1.3` selection reproduces the retained MP6
code. This is a compile result, not a claim that the target object's unlinked
`.comment` metadata names that compiler release.

The local `mwcc-decomp` compiler repository was consulted to distinguish
deferred-inline and optimizer behavior. Source acceptance still rests on the
authenticated sibling source, the actual CodeWarrior compile, objdiff, and
the exact linked-container gate.

## Provenance and object evidence

The authenticated sibling revisions are Mario Party 4 commit
`147b165a83187ac9e6cfdc3bf52f2e73437b1ffd` and Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb`. Each named donor owner is marked
Matching in the sibling configuration.

| MP6 owner | Authenticated source | Object evidence |
| --- | --- | --- |
| `ansi_fp.c` | M4/M5 tree blob `e8d7f0c743326c869f02d3f4616763ce5b18434a` | Target/source `.text` are both `0xC18`; exported `__num2dec` is exact at `0x1A4`. Raw section pairing is 13.565891% because six target split-local helpers do not map to the donor's compiler-local symbols, but the complete linked owner range is exact. All 161 text relocations resolve identically. Target `.rodata 0xE0` and `.data 0x128` contain the donor's `0xDD` and `0x124` semantic bytes plus linker alignment tails of three and four bytes; `.sdata2 0x8` is exact. |
| `file_io.c` | M4/M5 tree blob `6ca5b36a0cbf1546c0099a945aa53fc3f9cd8397` | `.text 0x1F8` is 100%: `fflush 0x138`, `fclose 0xC0`, and all five relocations match. |
| `mbstring.c` | M5 tree blob `191d05185b151cb0b2beec509c79365c926bedf9` | `.text 0x144` is 100%: `wcstombs 0xA0`, `unicode_to_UTF8 0xA4`, and all three relocations match. The source's four semantic `.sdata2` bytes are followed by the target split's four-byte linker-alignment tail. |
| `mem_funcs.c` | M4/M5 tree blob `a22984c44edbf75ccd831df9fa94eb40d1effc39` | All four copy kernels are exact, with sizes `0xB0`, `0xC4`, `0xAC`, and `0xC0`; whole `.text` is `0x2E0` and there are no relocations. |
| `e_pow.c` | M4/M5 tree blob `55b9b1c14f4473f415503d984bef98f8e9329c50` | Target/source `.text 0x88C`, `.rodata 0x30`, and `.sdata2 0x110` have identical sizes and bytes; all 65 relocations resolve identically. Raw objdiff is 99.98172% because two byte-identical loads (`lfd f6, 0x20(r8)` and `lfd f1, 0x10(r8)`) are attributed to a whole target rodata label versus donor component labels. The linked owner range is exact. |

`ansi_fp.h` retains the authenticated MSL donor text, with only trailing
blank-line normalization. The nested MSL `math.h` retains the donor
declarations and inline `sqrt`/`fabs` definitions needed by `e_pow.c`;
unrelated project convenience macros were excluded, including a donor macro
explicitly annotated as a probable fakematch. No inline or standalone
assembly is introduced by this wave.

## Rejected nearby port

`s_atan.c` was tested rather than promoted. `GC/2.6` emitted `0x218` bytes for
the target's `0x200`. `GC/1.3` restored the exact section sizes (`.text 0x200`,
`.rodata 0x98`, `.sdata2 0x28`) but still scored 99.453125% with genuine
instruction differences in `atan`. No declaration reshuffle or compiler
workaround was guessed. The probe source was removed and the owner remains
`NonMatching`/`NO-SOURCE`.

## Full gate

After the rejected probe was removed, the accepted configuration was checked
serially with:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The final result is `137 files OK`; the original and rebuilt DOLs compare
byte-for-byte, and the built `main.dol` SHA-1 is
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. DTK reports 263/895 matching
owners overall and 254/396 in the DOL. The final build was also checked for
generated `symbols.txt` changes before staging.
