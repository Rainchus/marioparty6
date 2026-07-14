# Native matching wave 28: MSL runtime closure

This ledger records three clean-C main-DOL owner promotions:
`MSL_C.PPCEABI.bare.H/assert.c`, `s_atan.c`, and `w_log.c`. Tests use pinned
DTK 0.9.2 (`4d039140`) with the configured CodeWarrior compilers. No assembly,
byte arrays, source padding, fake operations, or register forcing were added.

## Accepted owners

### `assert.c`

The target object establishes the older three-argument MSL signature:

```c
void __msl_assertion_failed(const char *condition, const char *filename,
                            int line);
```

Its register flow forwards incoming `r3`, `r4`, and `r5` to `fprintf`, and its
rodata contains the exact diagnostic
`Assertion (%s) failed in "%s" on line %d\n`. That evidence rejects the later
four-argument MSL variant carrying a function-name parameter.

The recovered function is target/source `0x4C/0x4C` with identical
instructions and all six relocations. Target rodata is `0x30`; the C string is
`0x2A` bytes including its terminator and the remainder is zero linker
alignment. The linked owner range and final DOL are byte-identical.

### `s_atan.c`

Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` supplies the fdlibm source shape.
The MP6 owner requires `GC/1.3`, as do several adjacent accepted MSL owners;
`GC/2.6` changes its generated code size.

Under `GC/1.3`, target and source both contain `0x200` text bytes, `0x98`
rodata bytes, `0x28` small-constant bytes, and ten effective relocations.
Objdiff's 14 remaining argument annotations name compiler-local split/pool
symbols at the same offsets; the instructions, literal data, effective
relocation targets, linked owner range, and final DOL all match.

### `w_log.c`

The recovered wrapper preserves the old MSL/fdlibm source form with an
unprototyped `__ieee754_log` declaration. That is significant: the incoming
floating-point argument remains in the ABI register for the tail call without
an artificial local or explicit forwarding expression.

The `GC/2.6` object has the exact target `0x20` text and sole relocation.
Objdiff reports no mismatching symbol or section, and the linked owner range
is byte-identical.

Together the accepted owners contribute `0x26C` linked code bytes, `0xF0`
configured constant bytes, three functions, and 17 relocations.

## Rejected probes

- The Mario Party 5 `alloc.c` donor is a different MSL feature/version
  closure. Even under `GC/1.3`, source text is `0x8D0` versus the target's
  `0xEAC`; BSS/small-data sizes and relocation closure also diverge. The probe
  source and compiler override were removed.
- The Matching `yannicksuter/smstrikers-decomp` MSL `qsort.c` donor recovers
  the real heap-sort and byte-swap source shape, but MP6 remains target/source
  `0x170/0x16C` at 98.260870%. The residual is a real `li 2; mullw` target
  sequence versus a source `slwi`; the probe was removed rather than
  strength-forced or marked matching.

These rejected owners remain fallback-linked and are not counted as
decompiled source.

## Gate

The first batch link rejected a later four-argument assertion variant. A
linked-byte comparison localized every differing byte to its diagnostic
string; target disassembly and target rodata then established the older
three-argument routine above. After that correction, the final serial gate
passed:

- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- `build/GP6E01/main.dol` SHA-1:
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`

The machine-readable object reports are retained under `build/` locally and
are intentionally not tracked.
