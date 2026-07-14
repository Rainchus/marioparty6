# Native matching wave 24: Runtime allocation exception state

This ledger records source-level recovery inside three fallback-linked Runtime
owners. The frozen base is `fork/main` commit
`ba0fb9695852c0e90e5dd877b0ffab454697ee5f`. Tests use the pinned DTK 0.9.2
toolchain and the configured `GC/2.6` compiler.

All three owners remain `NonMatching`. Their extracted original objects still
provide the linked bytes, so none of the recovered source below is counted as
a decompiled owner. No assembly, byte arrays, padding objects, artificial
symbols, or register-forcing source was admitted.

## `New.cp`: the real new-handler state

The original MP6 linker map in `shibbo/MarioParty6` commit `ac74f071`, file
`map/main.elf.map`, names address `0x802C0C58` as
`std::__new_handler(void)`. The target split assigns `0x802C0C58..0x802C0C60`
to `New.cp`, while objdiff exposes one eight-byte `.sbss` section with only
four semantic bytes. The recovered declaration therefore emits one real
four-byte `std::new_handler` object initialized to null; the remaining four
bytes are section alignment, not a guessed second global.

The resulting source object contains `__new_handler__3std` as a four-byte
`.sbss` symbol. Both retained delete functions remain exact:

| Function | Target | Source | Result |
| --- | ---: | ---: | --- |
| `operator delete[]` | `0x4C` | `0x4C` | 100% |
| `operator delete` | `0x4C` | `0x4C` | 100% |

The canonical Metrowerks allocation bodies were compiled and rejected rather
than retained. `operator new[]` was target/source `0x70`/`0x68` at
85.178570%; scalar `operator new` was `0x6C`/`0x68` at 88.518520%. Missing
allocator code and exception records keep this owner fallback-linked.

## `NewMore.cp`: `bad_alloc`, RTTI, vtables, and throw state

The existing `Runtime/exception.h` carries the sibling-authenticated inline
`std::exception` class. The recovered `std::bad_alloc : std::exception`
declaration follows the Metrowerks Runtime shape preserved by
`Yotona/twewy` commit `c23d6f0b`, in `libs/cpp/include/new`; its Runtime
implementation in `libs/runtime/src/New.cpp` independently preserves
`__throws_bad_alloc = 1` and the conditional `throw bad_alloc()` body. MP6's
target RTTI, vtable relocations, stack vptr stores, and destructor order prove
the same direct one-vptr inheritance in this owner.

This source now emits the sibling-authenticated named closure instead of
anonymous split data. MP6 preserves the one-byte state and its uses, but not
the original symbol name:

- `std::__throws_bad_alloc`, one byte initialized to 1;
- the `std::exception` and `std::bad_alloc` RTTI records and vtables;
- `std::__throw_bad_alloc`;
- both virtual destructors and both `what()` methods.

Fresh object comparison gives:

| Evidence | Target | Source | Result |
| --- | ---: | ---: | --- |
| `.text` | `0x114` | `0x11C` | 96.666664% |
| `std::__throw_bad_alloc` | `0x58` | `0x58` | 99.545456%; instruction shape is exact, two operands name the pooled source string rather than the target split label |
| `std::bad_alloc::~bad_alloc` | `0x5C` | `0x5C` | 100% |
| `std::exception::~exception` | `0x48` | `0x48` | 100% |
| each `what()` | `0x0C` | `0x10` | one extra pooled-string-base add remains |
| `extab` | `0x18` | `0x18` | 100% |
| `extabindex` | `0x24` | `0x24` | 100% |

The target/source data closure is `.rodata` `0x70`/`0x56`, `.data`
`0x30`/`0x2C`, and `.sdata` `0x18`/`0x14`. Relocation counts are already
identical: 22 in `.text`, 7 in `.data`, 3 in `.sdata`, and 6 in
`extabindex`. The remaining string-pooling and alignment closure is real
matching work; no manual data or padding was introduced.

An intermediate isolation probe duplicated `std::exception` in `New.h` to
test deferred inline emission. It proved the class layout but not header
ownership and was discarded. Including the existing authenticated exception
header plus the complete `bad_alloc` declaration produces the same accepted
function and section scores.

## `NMWException.cpp`: bounded residual investigation

Seven of eight functions remain exact for a combined `0x3F4` source bytes.
The sole residual, `__construct_array`, remains target/source `0xF8`/`0xFC`
at 93.854836%. Its target keeps the element count and pointer in different
nonvolatile registers and schedules the reverse-destruction tail four bytes
shorter than the canonical source.

Two independently preserved loop spellings were tested: the combined
initializer/update form used by Matching MP4/MP5 Runtime sources, and a
separate pointer/update form used by another recovered Metrowerks Runtime.
Baseline and both probes compiled to byte-identical source objects; the final
objdiff reports share SHA-256
`d743da0ffc60b4fc29bb04af1eb818c3bfdd735f27da30a433bd9b213ae23aec`.
The authenticated combined form was restored. `extab` remains 100% and
semantic `extabindex` is 99.285710%. Register forcing would be unsupported,
so no NMW source edit was retained.

## Full gate

After the final authenticated header form was restored, the source objects and
the complete build were regenerated serially:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The final result is `137 files OK`; the two DOLs compare byte-for-byte and the
built `main.dol` SHA-1 is
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. The final build was also checked
for generated `symbols.txt` changes before staging.
