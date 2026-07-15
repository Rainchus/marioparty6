# Native matching wave 48: GSSDK queue and TinyOS runtime

Wave 48 recovers all 16 retained functions in `mqueue.c` and all 25 retained
functions in `tinyos.c` as compiled, target-derived C. This is one shared ABI
slice: the queue owner consumes the TinyOS context and error/profile services,
while TinyOS constructs, connects, schedules, controls, and destroys those
queues. Both complete owners remain `NonMatching` because their generated
objects still diverge from the target.

## Tool and evidence provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Compiler: project-pinned Game Speech SDK `GC/1.2.5n` with floating-point
  contraction disabled.
- Recovery inputs: MP6 target instructions, symbols, allocations, field
  offsets, call signatures, relocations, queue/block consumers, profile tables,
  architecture tables, and the neighboring `rec1600` owner boundary.

No public source donor was used. Function names come from the retained target
symbols; behavior, ownership, and layouts come from the MP6 binary.

## Recovered shared ABI

The allocator instructions and already-exact `fastallo.c` establish a `0x118`
`FastAllocator`: heap, chunk sizing/count state, chunk pointers, current chunk,
and 65 word-count free lists. Moving that definition into `fastallo.h` leaves
all three allocator functions and `.text 0x1EC` byte-exact, with all five text
relocations preserved.

TinyOS access widths and offsets recover the complete `0x16C` `TosContext`:
block and queue ranges, pointer banks, six architecture callbacks, profile and
control tables, state/error/warning counters, architecture definitions,
process-input storage, heap ownership, and the embedded allocator at `0x54`.
The next `rec1600` owner boundary independently closes the context size.

The same instructions recover the `0x28` `TosBaseBlock`, its queue ports and
process/init/control callbacks, the `0x14` fixed queue prefix followed by a
reader-pointer array, queue elements with inline payloads, typed profile
entries, architecture construction records, and state/control records. Bytes
5 through 7 of block and queue owners are retained only as the proven
alignment between the shared `(context, owner index)` profile prefix and the
next accessed field; no semantic role is assigned to them.

The recovered queue source covers control, reader rewind, initialization and
redimensioning, enqueue/dequeue, reader advancement, unused-element recycling,
input readiness, reset, counting, and construction. TinyOS covers architecture
construction/destruction/initialization, queue connection, block construction,
scheduling, controls, profile lookup, diagnostics, and base-block lifecycle.
There is no inline assembly, byte blob, fake operation, or unproven semantic
field.

## Batched object evidence

The integrated allocator, queue, and TinyOS sources were compiled together and
compared once.

| Owner | Target/source `.text` | Raw score | Text relocations |
| --- | ---: | ---: | ---: |
| `fastallo.c` | `0x1EC/0x1EC` | 100% | `5/5` |
| `mqueue.c` | `0x102C/0xFD4` | 82.081160% | `29/29` |
| `tinyos.c` | `0x1D90/0x1D88` | 74.948204% | `70/70` |

`qQueueInit` (`0xF0`), `qQueueNbrElements` (`0x20`), `tosDestruct`
(`0x48`), and `DisabledProcess` (`0x4`) are byte-exact. TinyOS target/source
`.sdata2` is `0x8/0x8` and exact. The remaining differences are retained as
real compiler-shape work; neither owner is promoted.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts remain 293/895 owners overall and 284/396 DOL owners.
