# amix-system

The Commodore AMIX 2.1 and 2.1c system files: the stock kernels, the
installed `/usr` tree and the retail 2.1 floppies. They are the one system
input of [amigaux/amix](https://github.com/amigaux/amix), which builds its
cross toolchains, kernels and modules against them. Nothing here is built or
changed by the project; the files are as Commodore shipped them or as an
installation made them.

## Layout

| Path | What |
|------|------|
| `kernels/2.1/unix` | the retail AMIX 2.1 kernel, `stand/unix` as shipped |
| `kernels/2.1c/unix` | an AMIX 2.1c kernel; 2.1c is relinked on the machine at install time, so this is one such link, the one the 68040/68060 port patches |
| `etc/` | the stock `/etc` at 2.1p2a as the packages ship it plus the patch disk's changes; owners, modes, hard links, empty directories and the three named pipes git cannot hold are listed in `etc.md` |
| `usr/sys` | the relink kit: the top `Makefile`, `master.d` and the pre-linked objects the 68030 kernel is cut from |
| `usr/include` | the system headers |
| `usr/lib`, `usr/ccs` | the libraries, startup objects and the C compilation system |
| `usr/X` | X11: headers, libraries, fonts and clients |
| `usr/amiga` | the Amiga-specific part: tools, `/usr/amiga/include/amiga` headers, `libscreen.so` and the sources Commodore shipped |
| `usr/ucblib` | the BSD compatibility part: `libucb.a`, `libdbm.a` and the other BSD libraries, doctools and the dictionary |
| `usr/sbin/syslogd` | the 2.1p2a `syslogd`, from the patch disk |
| `floppies/2.1/amix_21_boot.adf` | the retail 2.1 boot floppy: the bootstrap and the installation kernel |
| `floppies/2.1/amix_21_root.adf` | the retail 2.1 root floppy: the miniroot the installer runs from |
| `floppies/2.1/amix_21_patch.adf` | the patch disk: a shell script and a compressed archive of patches that bring an installed 2.1 to 2.1p2a; it holds no kernel, and the 2.1c kernel is relinked on the machine when the patches are applied |

The `etc/` tree is not an installed system's copy: it has no site edits, users or keys (see `etc.md`).

The `usr/` tree is AMIX 2.1c at patch level 2a, as installed, with symbolic
links kept as the installation made them (several point at absolute paths on
the machine). One install-time choice is left out: the timezone link
`usr/lib/locale/TZ/localtime`. `usr/amiga/bin/system.setup` keeps the
execute bit it ships with (it clears it itself on the first boot), and the
empty directory `usr/amiga/lib/audio` is not kept by git. It is the sysroot
both of amix's cross toolchains are built against; see `toolchain/README.md`
there.

## Verifying a checkout

amix keeps the sha256 of every regular file here in `system/MANIFEST.sha256`
and every symbolic link in `system/SYMLINKS`. From the top of a checkout of
this repository, with an amix checkout at `$AMIX`:

```sh
sha256sum -c --strict --quiet "$AMIX/system/MANIFEST.sha256"
find etc usr -type l | LC_ALL=C sort | while read -r l; do
	printf '%s -> %s\n' "$l" "$(readlink "$l")"; done |
	diff - "$AMIX/system/SYMLINKS"
```

Both are silent on a good checkout. amix's `toolchain/container-build.sh`
runs the same two checks before it builds anything.

## Licensing

Amiga Corporation approved releasing Amiga UNIX on 2026-09-23, and these
files are published under that approval. They are not covered by amix's MIT
licence.

This site and its developers are not affiliated with, endorsed by, or sponsored by Amiga Corporation. Amiga is a registered trademark of Amiga Corporation.

Amiga UNIX is Commodore's port of AT&T UNIX System V Release 4; portions are
copyright AT&T, UNIX System Laboratories and their successors.
