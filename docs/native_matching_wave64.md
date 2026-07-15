# Native matching wave 64: Board main-loop shape

Wave 64 advances the source-complete `board/board.c` owner without changing
its fallback boundary. Target/source `.text` are now both `0x1CFC`, up from a
source size of `0x1CF8`, and strict and data-value comparison both reach
99.902960%. All 35 target functions pair, 31 are exact, and both objects carry
618 relocations.

`mbMain` is now target/source `0x744/0x744` at 99.935486%, up from
`0x744/0x740` at 99.541940%. The recovered shape uses distinct `s32`
lifetimes for the current-time copy and the derived night value, keeps
`mgCallF` at function scope, preserves the short-lived day predicate, and
spells the party tests with the target's explicit zero comparisons. This
restores the target frame, save-register range, missing `mr`, and exact
instruction count. Six register-color operands remain; no instruction,
padding, or assembly substitute was introduced to hide them.

## Authentication and compiler evidence

Mario Party 5 commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb`,
especially `src/game/board/main.c`, authenticates the shared board-main loop,
party/story split, opening flow, time transition, Last-5 path, and minigame
handoff. MP6 target instructions and relocations establish the MP6-only
conditions and the exact scalar widths.

The MWCC source oracle at
`094b96ca1df4a035b5f93c351f773306c0241f3f` was used to distinguish real
source lifetimes from register forcing. `CInline.c` explains the reversed
local-list clone order in the inlined `mbNextTimeSet`; `CParser.c`,
`StackFrame.c`, and `Coloring.c` explain why lexical scope and dead locals
cannot lawfully reserve the remaining colors.

The following narrow probes were measured and rejected:

- a shared party temporary shrank `mbMain` by eight bytes and changed its
  save-register range;
- function-scope `dayF`, direct day expressions, and `== TRUE` conditions
  changed target opcodes or worsened register allocation;
- split-declaration, direct-field, one-local, and explicit-inline forms of
  `mbNextTimeSet` could make either the standalone function or its inlined
  copy exact, but not both, and the direct forms changed their sizes;
- the MP5 handicap accessor body was tested both as an ordinary inline and
  with the repository's plain-definition/inline-redeclaration lever; GC/2.6
  emitted a real call and shrank `mbSaveInit`, so the five-difference local
  baseline was retained;
- moving the night local and restoring MP5 `GWMgPackSet` header ownership were
  byte-neutral for `mbObjectSetup`.

## Remaining exact residuals

| Function | Target/source size | Strict/data-value score | Remaining evidence |
| --- | ---: | ---: | --- |
| `mbObjectSetup` | `0x618/0x618` | 99.743590% | Twenty register-color operand differences. |
| `mbMain` | `0x744/0x744` | 99.935486% | Six register-color operand differences. |
| `mbNextTime` | `0xDC/0xDC` | 99.636360% | Three operands swap the two inlined helper locals; standalone `mbNextTimeSet` is exact. |
| `mbSaveInit` | `0x280/0x280` | 99.812500% | Five operands swap the party/handicap local colors. |

`.data 0x18`, `.sdata 0x10`, and `.sbss 0x30` are exact. The first `0xC`
bytes of `.sdata2` are exact; the target retains the documented four-byte
split tail. The owner remains the sole `Object(NonMatching,
"board/board.c")` entry in `configure.py`. This was an object-only WIP slice:
no Matching flip, clean-C ledger increase, full-DOL gate, or byte-identical
container claim is made.
