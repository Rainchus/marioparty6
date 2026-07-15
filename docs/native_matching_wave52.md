# Native matching wave 52: OS linked-owner proof and Board cycle recovery

Wave 52 promotes `dolphin/os/OS.c` only after its authentic source passes the
effective linked-owner and full-container gates. Because the file contains
sibling-authenticated assembly, this is an exception owner rather than clean
decompiled C. The same patch recovers additional target control flow in
`board/board.c`; that owner improves materially but remains `NonMatching`.

## `dolphin/os/OS.c`: authenticated source, effective relocation proof

Mario Party 4 commit `147b165` configures `src/dolphin/os/OS.c` as
`MatchingFor(USA,PAL)` and authenticates the FPR, exception-vector,
default-exception, paired-single, and initialization assembly family already
restored in MP6. Twelve of the 14 retained MP6 functions are raw-object exact.

The remaining reports are compiler-local symbol identity, not a recoverable C
source difference:

- `OSInit` is target/source `0x4E0/0x4E0` at 97.500000%.
- `OSExceptionInit` is target/source `0x280/0x280` at 98.500000%.
- Both instruction streams and relocation addends agree. The target names a
  local `@1` at `.data+0`; GC/1.2.5n names the same address through its
  generated `OS.c ...data.0` section symbol.
- The relevant text relocations are at `0x1B6/0x1BA` in `OSInit` and
  `0x6A2/0x6AE` in `OSExceptionInit`.
- Target/source `.data` are `0x1F8/0x1F7`. Bytes `0x000..0x1F6` and their
  semantic order are identical; the target contributes one trailing zero.
  `.sdata` is byte-identical at `0x10`.
- Target/source `.text` are `0xACC/0xC2C` with 166/191 relocations. The extra
  source routines and relocations are outside the retained linked owner and
  are stripped, as in the authenticated sibling shape.

The target object records CodeWarrior metadata `2.4.7.1`, while the available
GC/1.2.5n compiler records `2.3.0.1`. Matching `OSContext.c` also demonstrates
that the target-local name versus generated section-symbol name is benign once
resolved. Inventing an `@1` declaration or source padding would therefore be a
fakematch and was rejected. Selecting the authentic source for the link
instead reproduces the complete DOL byte for byte, so `OS.c` is admitted under
the sibling-authentication exception and removed from the real-C fallback
bucket.

## `board/board.c`: recovered `mbMain` cycle joins

The MP6 control-flow graph and fully matching MP5 source corroborate five
source-shape corrections in `mbMain`:

- the party setup tests teams through the typed `GWTeamFGet()` accessor;
- `nextTime` synchronizes only when the current time changed;
- `FLAG_BOARD_MOVE_DONE` is cleared only on the party return path;
- tutorial and no-minigame paths continue directly to the next turn cycle;
- the player-zero reset remains on the regular party path rather than the
  story-mode join.

`mbMain` improves from 99.133330% to 99.541940%, with target/source sizes
`0x744/0x740`. Its residual is 24 operand-color differences and one missing
target `mr`. Whole-owner target/source `.text` are `0x1CFC/0x1CF8`; 31/35
functions are exact and all 618 relocations pair. `.data`, `.sdata`, and
`.sbss` retain exact sizes, while the pre-existing split-only `.sdata2`
`0x10/0xC` difference is unchanged. `mbObjectSetup`, `mbMain`, `mbNextTime`,
and `mbSaveInit` still diverge, so the owner remains fallback-linked.

No register forcing, dummy operation, byte blob, synthetic padding, or
unauthenticated assembly was accepted.

## Full gate

- `ninja -j1`: `137 files OK`
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- Progress: 9.10% code and 31.30% data overall; 46.65% code and 74.41% data
  in the DOL; 299/895 owners overall and 290/396 DOL owners Matching

