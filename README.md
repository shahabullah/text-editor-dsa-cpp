# Text Editor (DSA Project)

A console text editor written in C++ for my Data Structures course. The document is stored as a **doubly linked list** of lines, and **undo/redo** is built with **stacks**.

## Features

- Insert a line at any position
- Delete a line
- Edit a line
- Show the whole text with line numbers
- Undo and redo (for insert, delete and edit)
- Save the text to a `.txt` file
- Load a text from a `.txt` file

## Data structures used

| Data structure | Where it is used |
| --- | --- |
| Doubly linked list | The document, one node per line of text |
| Stack (array) | Undo history |
| Stack (array) | Redo history |

## How undo and redo work

Every insert, delete or edit is saved as an action record (type, line number, old text, new text) and pushed onto the undo stack.

- **Undo** pops the last action, reverses it, and pushes it onto the redo stack.
- **Redo** pops from the redo stack, applies the action again, and pushes it back onto the undo stack.
- Any new action clears the redo stack.

## How to run

Compile with g++:

```
g++ project.cpp -o editor
```

Run:

```
.\editor.exe
```

The program uses `system("cls")` to clear the screen, so it is written for Windows.

## Menu

```
1. Insert line
2. Delete line
3. Show text
4. Undo
5. Edit line
6. Redo
7. Save to file
8. Load from file
0. Exit
```

Saved files are created as `name.txt` in the folder where the program is run.

## Planned improvements

- Clipboard history using a queue (copy and paste)
- Search a word
- Find and replace
- Word and line count
