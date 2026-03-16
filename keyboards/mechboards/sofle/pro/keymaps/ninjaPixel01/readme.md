Refer to `users/ninjaPixel/readme.md` for more information and flashing instructions.


# Generating visual keymap SVGs with keymap-drawer

tl;dr:

```bash
# cd into your keymap directory
qmk c2json keymap.c | keymap parse -c 10 -q - >keymap_vis.yaml && keymap draw keymap_vis.yaml >keymap.svg
```

(MacOS instructions)

## Prerequisites

You will need Homebrew installed. If you do not have it, follow the instructions at [brew.sh](https://brew.sh).

---

## 1. Upgrade Python

Check your current Python version:

```bash
python3 --version
```

Check what version Homebrew has available:

```bash
brew info python
```

If Homebrew has a newer version, upgrade it:

```bash
brew upgrade python
```

---

## 2. Install pipx

pipx installs Python CLI tools into isolated environments, preventing dependency conflicts between tools.

```bash
brew install pipx
pipx ensurepath
```

Restart your terminal after running `ensurepath`, or reload your shell config:

```bash
source ~/.zshrc
```

---

## 3. Install keymap-drawer

```bash
pipx install keymap-drawer
```

Verify the installation:

```bash
keymap --help
```

---

## 4. Generate a visual layout from your keymap

Parse your `keymap.c` file into an intermediate YAML file:

```bash
# move into your dir
cd path/to/keymap_dir
qmk c2json keymap.c | keymap parse -c 10 -q - >keymap_vis.yaml
```

Render the YAML to an SVG diagram:

```bash
keymap draw keymap_vis.yaml >keymap.svg
```

Open the SVG in your browser to view all layers visually.

---

## Updating keymap-drawer

To update to the latest version later:

```bash
pipx upgrade keymap-drawer
```
