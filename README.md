# File Organizer

C++ utility for automatic file organization.

---

## Features

- Organizes files by extension
- Creates category folders automatically
- Uses C++17 filesystem
- Handles filesystem errors

---

## Dry Run

Use `--dry-run` key, to see what will happen to your files

Example:

```bash
organizer.exe Downloads --dry-run
```

---

## Categories

- Images
- Music
- Code
- Executables

---

## Build

```bash
cmake -B build
cmake --build build
```