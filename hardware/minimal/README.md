# Minimal Word Processor

This is a resource-limited variation of the word processor with both minimal and an alternative "full-function" shortcut version. Actual implementation will likely be the minimal version as a proof of concept.

The keyboard is an exploration of the bare minimum set of keys necessary to have a functional word processor while following the author's personal aesthetics.

## Keyboard layout

The key arrangement will be identical between shortcuts.

![keyboard layout](https://github.com/cypnk/WordProcessor/raw/master/hardware/minimal/layout.png)

|Key size|Count|
|--------|-----|
|1u      |  47 |
|1.5u    |   2 |
|1.75u   |   2 |
|2.25u   |   2 |
|6.25u   |   1 |

Total, 54 keys with 2 sets of 2.25u Costar stabilizers and 1 set of 6.25u spacebar stabilizers.

## Shortcuts

[Minimal command](https://github.com/cypnk/WordProcessor/blob/master/hardware/minimal/layout_cmd.json) version with only 11 shortcuts:

![minimal keyboard layout](https://github.com/cypnk/WordProcessor/raw/master/hardware/minimal/layout_cmd.png)

The cursor behavior is to remain in the same position on screen when scrolling and only to move while typing and while using the movement key commands. This set of shortcuts is easier to implement directly on microcontroller hardware.

Navigation (^ = Command):
| Key / Combo	| Function		|
|-------------|-------------|
| ^W | Up   |
| ^A | Left |
| ^S | Down |
| ^D | Right |
| ^E | Page up |
| ^X | Page down |

Editing:
| Key / Combo	| Function		|
|-------------|-------------|
| ^Z | Undo |
| ^Y | Redo |
| Shift + Shift | Caps lock |
| ^N | New document |
| ^O | Open document |

[Full command](https://github.com/cypnk/WordProcessor/blob/master/hardware/minimal/layout_cmd_full.json) version which is mostly inspired by WordStar and a bit of WordPerfect:

![full keyboard layout](https://github.com/cypnk/WordProcessor/raw/master/hardware/minimal/layout_cmd_full.png)

The full set of commands will likely only be practical on a system with a dedicated editor application running on top of an underlying operating system.

Navigation:
| Key / Combo | Function |
|------------|--------|
|^E | Up|
|^X | Down|
|^S | Left|
|^D | Right|
|^K | Line start|
|^L | Line end|
|^W | Scroll up|
|^Z | Scroll down|
|^R | Page up|
|^C | Page down|
|^, | Document start|
|^. | Document end|
|^J | Return cursor to previous location|

Editing:
| Key / Combo | Function |
|------------|--------|
|^A | Select word left of cursor|
|^F | Select word right of cursor|
|^Q | Query / Find in document|
|^G | Delete left of cursor|
|^H | Delete right of cursor|
|^T | Delete current word|
|^Y | Delete current line|
|^V | Select from clipboard|
|^B | Line break|
|^shift Return | Page break|
|^[ | Copy selected text to clipboard|
|^] | Paste last copied text|
|^M | Memo / Citation on selected text|
|^P | Program / Create or execute subroutine|
|^U | Undo|
| ^shift U | Redo|
|^N | New document|
|^O | Open existing document|

Formatting:
| Key / Combo | Function |
|------------|--------|
|^I | Indent|
|^shift I | Outdent|
|^shift T | Superscript|
|^shift V | Subscript|
|^shift B | Bold|
|^shift Y | Italic|
|^shift S | Underline|
