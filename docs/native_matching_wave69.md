# Native matching wave 69: Mode-select state-machine recovery

Wave 69 keeps `mdseldll.rel` at Priority 1a and closes the remaining bounded
mode-select callbacks before the two unresolved owners inside
`REL/mdseldll/mdsel.c`. The source now represents 111 of 113 target functions.
Every represented function is data-value-exact under objdiff, accounting for
`0xD614` of the target's `0x10010` text bytes. The only absent functions are
`fn_1_5BF0` (`0x2B4`) and `fn_1_BAB4` (`0x2748`), a combined `0x29FC`.

The application owner and compiler-runtime owner remain `NonMatching`. This is
object proof for source work in progress, not a REL promotion and not a claim
that fallback-linked bytes are decompiled source.

## Object result

The final owner-local comparison reports:

- target functions: 113 / `0x10010` bytes
- represented functions: 111 / `0xD614` bytes
- data-value-exact functions: 111 / `0xD614` bytes
- represented residuals: none
- target/source `.text`: `0x10010/0xD614`, 83.60386%
- target/source `.rodata`: `0x260/0x258`, 98.013245%
- target/source `.data`: `0x370/0x370`, 100%
- target/source `.bss`: `0x1A4E/0x1A50`; all target bytes compare exact and
  the source object has a two-byte alignment tail

The object was rebuilt immediately before comparison:

```powershell
rtk ninja build/GP6E01/src/REL/mdseldll/mdsel.o
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdseldll/REL/mdseldll/mdsel `
  -o build/GP6E01/mdseldll/mdsel_objdiff.json `
  --format json-pretty -c functionRelocDiffs=data_value
```

This wave adds 19 exact functions and `0x6974` target text bytes:

`fn_1_11D4`, `fn_1_1370`, `fn_1_2A78`, `fn_1_346C`, `fn_1_3910`,
`fn_1_3CC0`, `fn_1_485C`, `fn_1_4CF4`, `fn_1_5290`, `fn_1_5614`,
`fn_1_607C`, `fn_1_651C`, `fn_1_70BC`, `fn_1_7868`, `fn_1_809C`,
`fn_1_92BC`, `fn_1_9910`, `fn_1_E1FC`, and `fn_1_EF48`.

## Recovered ownership and source shape

The model-motion closure recovers the paired Bezier callbacks, their target
scale and rotation smoothing, the model/effect setup callbacks, and the
destruction path. The target establishes division by `2.0f`, direct nesting of
`fn_1_50C` as the fifth Bezier argument, `fn_1_550` for the rotation path, and
the preincremented completion test. Equivalent arithmetic spellings that
changed instruction order were rejected.

`lbl_1_bss_24C` is now a 12-record owner with an instruction-proven `0x88`
stride. Each record has signed 16-bit fields at `0x00/0x02`, floats at
`0x04/0x08`, vectors at `0x0C/0x18`, floats at `0x30/0x34`, and opaque storage
where no field meaning is established. `fn_1_485C` proves the fixed ninth
record access independently of its model-index loop. `fn_1_5290` proves the
two-state signed 16-bit selector in the first record. `fn_1_5614` proves the
three model groups and their target motion indices.

The mode-input closure recovers `lbl_1_bss_1B0` as a six-by-three signed
16-bit table and `lbl_1_bss_1A28` as the 32-bit positional-audio handle used by
`fn_1_70BC`. The mic callback loads establish the owner-local response layout:
signed status at `0x00`, signed result count at `0x04`, and a signed 16-bit
result vector pointer at `0x08`. `game/mic.c` authenticates the raw callback
transport. Keeping that public callback type while giving `fn_1_651C` its
recovered response-structure parameter, with an owner-local registration cast,
reproduces all 442 target instructions and the target register coloring.

`fn_1_9910` recovers the complete mode-object initialization call graph. The
standalone helper bodies and their later expansions are retained with
post-definition `inline` redeclarations; this is the source form explained by
the pinned `mwcc-decomp` `CInline.c` implementation and confirmed by the
target's calls versus copied instruction bodies.

Two target-emitted but semantically overwritten values remain because their
stores are byte evidence: the unused `2.0f` scalar in `fn_1_11D4` and the base
pointer assignment in `fn_1_4CF4`. They are not used to claim an unproved
field meaning.

## Remaining application work

`fn_1_5BF0` remains absent. Its setup and loop body are understood, but the
target-emitted boolean at the loop tail does not yet distinguish the authentic
C expression. No candidate was retained merely because it could be coerced
into the bytes.

`fn_1_BAB4` remains absent. At `0x2748`, it is the final monolithic mode-select
driver and requires state-by-state recovery; no partial body is linked as
source. These two functions, final translation-unit ordering, the eight
missing rodata bytes, the separate authenticated runtime decision, and the
complete REL/DOL gate remain before promotion.

No full `mdseldll.rel` or DOL gate was run for this object-only WIP slice, and
no `Matching` flip is present. A future promotion still requires full source
ownership, per-object relocation proof, full `rtk ninja`, `137 files OK`, the
build SHA check, and byte-identical `mdseldll.rel` plus `main.dol` comparisons.
