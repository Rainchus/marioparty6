# Native matching wave 25: MSL string and math closure

This ledger records a complete screen of the nine MSL owners that had no
source candidate at the start of the wave. The frozen base is `fork/main`
commit `cd3642f588272bc89935a6e5b79b5a7a42427a9e`. Tests use the pinned DTK
0.9.2 binary (`4d039140`) and the configured `GC/2.6` compiler unless a probe
below says otherwise.

Two owners are now Matching clean C: `string.c` and `math_ppc.c`. A third,
`printf.c`, now has an authenticated source candidate and nine byte-exact
functions, but remains `NonMatching`. No assembly, byte arrays, artificial
padding, register forcing, or source-level fake operations were added.

## `string.c`: exact seven-function owner

Mario Party 4 commit `147b165a83187ac9e6cfdc3bf52f2e73437b1ffd` and Mario
Party 5 commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb` preserve the
same MSL string implementation family. The MP6 owner has only seven of those
functions. The donor-only `strrchr` and `strstr` bodies were removed rather
than allowed to rely on linker stripping.

The sibling `strcat` transcription was not retained: it had a nonstandard
third `size_t` parameter and compiled as another bounded copy loop. The MP6
target instead proves the ordinary two-argument append operation. Recovering
that loop also exposed and corrected the corresponding `string.h` prototype.

Fresh object evidence is whole-owner exact:

| Evidence | Target | Source | Result |
| --- | ---: | ---: | --- |
| `.text` | `0x2DC` | `0x2DC` | 100% |
| Functions | 7 | 7 | all 100% |
| Relocations | 0 | 0 | exact |

The exact functions are `strchr`, `strncmp`, `strcmp`, `strcat`, `strncpy`,
`strcpy`, and `strlen`, in the target order emitted by `GC/2.6`.

## `math_ppc.c`: exact float wrapper closure

The MP4/MP5 source family authenticates the MSL pattern of implementing float
entry points as casts around the corresponding double-precision operation.
MP6's split contains eleven such wrappers. The target symbols, one-call
instruction bodies, and one relocation per function determine the expanded
owner without introducing semantic names beyond the exported APIs.

| Evidence | Target | Source | Result |
| --- | ---: | ---: | --- |
| `.text` | `0x18C` | `0x18C` | 100% |
| Functions | 11 | 11 | all 100% |
| `.rela.text` | `0x84` | `0x84` | 11/11 relocations |

The exact functions are `floorf`, `ceilf`, `powf`, `logf`, `expf`, `tanf`,
`sinf`, `cosf`, `atan2f`, `atanf`, and `acosf`.

Together the two promoted owners contribute 18 clean-C functions and `0x468`
exact DOL code bytes.

## `printf.c`: authenticated source, owner still divergent

The retained source is the exact Git-normalized blob
`5879d452f84d7393184114e3609efafc7491b08a` present in both the MP4 and MP5
`src/MSL_C.PPCEABI.bare.H/printf.c` paths at the commits above. Its required
MSL headers were restored from the same source family. The source compiles
cleanly with MP6's `GC/2.6` flags.

Nine functions, totaling `0xC78` code bytes, are exact:

| Function | Size | Result |
| --- | ---: | --- |
| `sprintf` | `0xE0` | 100% |
| `vsprintf` | `0x78` | 100% |
| `vsnprintf` | `0x74` | 100% |
| `vprintf` | `0x7C` | 100% |
| `printf` | `0xCC` | 100% |
| `__StringWrite` | `0x6C` | 100% |
| `__FileWrite` | `0x58` | 100% |
| `__pformatter` | `0x774` | 100% |
| `round_decimal` | `0x12C` | 100% |

The target split calls the final helper `fn_800F3DB8`. A temporary diagnostic
build changed only the static source name and its three call sites so objdiff
could pair it with `round_decimal`; the result was target/source `0x12C` with
zero instruction or relocation differences. The authentic `round_decimal`
name was restored before the final build. No address-derived name is retained
in source.

The owner cannot be promoted. Target/source `.text` are `0x2160`/`0x213C`,
and total relocations are 275/271. `fprintf` (`0xC4`) is absent from the donor.
The five mapped residuals are genuine:

| Function | Target/source | Objdiff |
| --- | ---: | ---: |
| `float2str` | `0x6EC` / `0x71C` | 92.164790% |
| `double2hex` | `0x338` / `0x338` | 97.038830% |
| `longlong2str` | `0x2DC` / `0x314` | 86.803276% |
| `long2str` | `0x220` / `0x258` | 80.625000% |
| `parse_format` | `0x504` / `0x504` | 98.130844% |

Configured data also remains target/source `.rodata` `0x28`/`0x25`, `.data`
`0x230`/`0x22C`, and `.sdata` `0x8`/`0x2`; `.sdata2` is `0x8`/`0x8` exact.
The original object therefore continues to supply every linked byte for this
owner, and none of the exact function slice is counted as a decompiled owner.

## Rejected and unavailable candidates

All six remaining no-source MSL owners were bounded before moving on:

- `alloc.c`: the authenticated MP4/MP5 donor under `GC/1.3` produced target/
  source whole text `0xEAC`/`0x8D0` at 22.023430%. `free` reached
  target/source `0x134`/`0x134` at 99.805190% and
  `deallocate_from_fixed_pools` reached `0x1FC`/`0x1FC` at 99.960630%, but
  `calloc`, `malloc`, and `allocate_from_fixed_pools` remained four-byte
  stubs. The source was removed.
- `e_exp.c`: the authenticated fdlibm donor compiled to target/source
  `0x21C`/`0x224` at 91.592590%. Constant sections and all 21 text
  relocations matched, but the instruction stream did not. The source was
  removed.
- `s_atan.c`: the authenticated MP4/MP5 donor under `GC/1.3` reached equal
  `0x200` text sizes, exact constants, and all 10 relocations, but `atan`
  remained 99.453125%. The source was removed.
- `assert.c`, `qsort.c`, and `w_log.c`: no authenticated implementation was
  present in the pinned MP4, MP5, Dolphin SDK, or local `mwcc-decomp`
  reference cache. No speculative decompiler output was materialized.

These outcomes leave six MSL owners in `NO-SOURCE` and move `printf.c` to
`SRC-DIVERGES`.

## Full gate

After enabling only `string.c` and `math_ppc.c`, the complete build was
regenerated serially:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The final result is `137 files OK`; the two DOLs compare byte-for-byte and
both SHA-1 hashes are
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. The final build was also checked
for generated `symbols.txt` changes before staging.
