# Native matching wave 20: Runtime exception and heap support

This ledger records one whole-owner Runtime promotion and four source-level
recovery slices that remain fallback-linked. The frozen base is `fork/main`
commit `e0710b36b2bb8bb93a0f3e7fb7293971d1054f1e`. Tests used the pinned DTK
0.9.2 toolchain.

Fallback-linked owners are not counted as decompiled source. A matching
sibling establishes source provenance, but it does not replace the MP6 object
and final-container gates.

## Accepted whole owner: `GCN_mem_alloc.c`

Mario Party 4's Matching GC/2.6 owner supplies the natural
`InitDefaultHeap`/`__sys_free` source shape. The allocation counterpart is
independently corroborated by a Matching sibling Runtime source and is proved
against the MP6 target here. The MP6 compile requires the sibling's
`-str reuse,readonly` setting; retaining the previous pooled-string setting
made each function four bytes too large and was rejected.

| Evidence | Target | Source | Result |
| --- | ---: | ---: | --- |
| `.text` | `0x170` | `0x170` | `__sys_free` and `__sys_alloc` are both exact at `0xB8` |
| relocations | 32 | 32 | same structure and effective targets |
| `.rodata` | `0x78` | `0x71` | the 113 semantic bytes are identical; the target's final seven bytes are zero alignment padding |

The linker reproduces that alignment tail, and the linked owner range and full
container are byte-identical. This owner is therefore configured `Matching`.
Its target and source object Git IDs are
`1c0a842adfaeccccd1988eee6583c1b02180cdca` and
`d8982653694e7e6c04d80436cbffa31c23beeefd`.

## Exact source inside fallback owners

### `New.cp`

Matching Mario Party 4 and Mario Party 5 Runtime sources authenticate the
exception-aware `operator delete` body. MP6 contains the same body for both
scalar and array delete. The recovered source compiles each function to the
target `0x4C` instruction stream and its `0x28` exception record with zero
function-level objdiff differences. The two functions recover `0x98` bytes of
exact code.

The owner remains `NonMatching`: `operator new`, `operator new[]`, their
exception metadata, and the two-word `.sbss` new-handler state are not
recovered. The target and current partial-source object Git IDs are
`6ad8dfa7df1f5c8ccc099aa8c33380da8014243e` and
`a28fc3764cddcf6999766e4d0133167d58934cab`.

### `NMWException.cpp`

The authenticated Runtime source family reproduces seven of the eight MP6
functions exactly:

| Recovered function group | Exact bytes |
| --- | ---: |
| `__destroy_arr` | `0x78` |
| partial-array destructor | `0xB8` |
| `__throw_catch_compare` | `0x22C` |
| `unexpected`, `terminate`, `duhandler`, `dthandler` | `0x98` |
| **Total** | **`0x3F4`** |

`__construct_array` is the sole rejected function: target/source sizes are
`0xF8`/`0xFC`, with a 93.854836% instruction score and 16 differences. The
canonical sibling loop was retained rather than reshaped speculatively. The
owner remains `NonMatching`. Target and source object Git IDs are
`caffb6e913d335979b68e996293c83248518c7bf` and
`92b9d2af01280dcddae58bcc789b2e5d95fd0384`.

## Authenticated candidates that are not byte-exact promotions

### `NewMore.cp`

The retained source is the Matching Mario Party 5 Runtime source for
`std::exception::~exception` and `std::exception::what`. It reproduces their
target sizes (`0x48` and `0xC`), but raw objdiff still reports two symbol
operands in each function: the target uses split-local labels while the source
uses semantic vtable/string symbols. More importantly, MP6's remaining
`0xC0` `bad_alloc` closure and its data are absent. The owner therefore stays
`NonMatching`; no byte-exact claim is made for this slice. Target and source
object Git IDs are `cf1027fb4675008f87a9a66db6ad8adf176824c6` and
`94b8e80e8d84acf9c713d56fcd182329b5c3045c`.

### `Gecko_ExceptionPPC.cpp`

The retained source and headers are byte-for-byte Mario Party 4 Matching
donors. Eleven of the thirteen retained MP6 functions compare exactly. The
other two, `__unexpected` and `std::bad_exception::~bad_exception`, differ only
in references to the MP6 split label for `std::exception`'s vtable, but the
compiled donor also emits the base exception destructor/`what` closure that
MP6 assigns to `NewMore.cp`. Its exception/data section closure consequently
does not fit the MP6 owner.

A trial whole-owner flip failed the final gate: only 115 configured files
matched and `main.dol` plus 22 RELs diverged. Mario Party 4's build cleans an
exception-table padding sequence that the pinned MP6 DTK 0.9.2 pipeline does
not expose. No padding, symbol, or byte workaround was admitted; the owner is
`NonMatching`. The two assembly functions (`ExPPC_LongJump` and `__throw`) are
proven authentic by the Matching sibling, but the owner is still in the
`C-not-yet-matched` bucket because real C/linker-closure work remains. Target
and source object Git IDs are `577876377f23b9f78bffdd7f74fb2a29999366c5`
and `4994856e8c8ef8e7f205e7964684fe4bb9ae5abf`.

## Full gate

The accepted configuration was validated serially after all rejected trial
flips were reverted:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The final result is `137 files OK`; the two DOLs compare byte-for-byte and the
built `main.dol` SHA-1 is
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. The final build was also checked
for generated `symbols.txt` changes before staging.
