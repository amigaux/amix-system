# etc/ — the stock /etc of Amiga UNIX 2.1 at patch level 2.1p2a

Added 2026-10-30 as the source of the stock defaults amix's base package carries (amix P-36 D-N24). It is `/etc` **as the packages ship it** plus what the 2.1p2a patch disk changes in `/etc`. It is not an installed box's copy: there are no site edits, users or keys.

## Counts

- 333 paths under etc/: 206 regular files, 86 symbolic links, 38 directories, 3 named pipes.
- In git: 292 entries (files and symlinks). Directories hold no mode in git, and 8 of them are empty, so git drops them (listed below).
- 45 of the regular files are hard links: git stores each as its own copy. Each one is marked in the table.
- The 3 named pipes cannot be stored in git. They are listed below and must be created by the package that installs this tree.

## Sources

Stock Commodore packages, the 2.1 package datastreams. Only these 8 of the 27 stock packages ship anything under /etc:

| package | sha256 | bytes |
|---|---|--:|
| `core-2.1.pkg` | `12cfa10323048a2149959932d4444f656a5881d1d198e1f35bf22ed0d28610f1` | 11247104 |
| `bsd-2.1.pkg` | `2b95aee7ccaf61650d456c0da816960f8bb3f7820525111359577e671d775dcc` | 1654784 |
| `lp-2.1.pkg` | `280f5e7a5eae3fdf4804e8014d5d1ff5e0f6986188e808e100cf9ac20907b17f` | 2183168 |
| `net-2.1.pkg` | `9ee47b51f6b01c605882424195ac15d1a2e0acdcd6b3076ef5c729eaf59f17fc` | 3042816 |
| `public-2.1.pkg` | `96b9155f650e12db4b447d28c89a7729ee89b45fa01484abb37f575f5cd28ace` | 7547904 |
| `sysadm-2.1.pkg` | `d81bc74534d0f905592563aefb3f8c91fe89a590fc4ea7483c8a1ab21a276e84` | 6002176 |
| `terminfo-2.1.pkg` | `a838f594db76aa5560aae36d942e65d40deda518e92b3e971def943744b570bf` | 2171904 |
| `uucp-2.1.pkg` | `73536c75ed651efb6f1f7f36c268af2a09d7f105cd6583296209d31b954fbd6f` | 888832 |

The other 19 (Cdev, X11r5src, Xbasic, Xcore, Xdev, Xsource, Xtras, amigasrc, conf, emacs, emacsrc, games, gnusrc, gnusrc2, man, olcore, oldev, pubsrc, text) declare nothing under /etc. `m4-2.1.pkg`, a later repackaging, is not a Commodore package and is not a source.

Patch level **2.1p2a**, the same as amix-system's `usr/`. It comes from the retail patch floppy `floppies/2.1/amix_21_patch.adf`, the one this repository carries:

| file | sha256 |
|---|---|
| `archive.lha` | `afc579b73d92cd6e1ffd9bce043cbc18097f1c37994a49fe8f1b14ccf11bcefa` |
| `apply` | `b8203071b014846fee727e9d2eff868cfcd3f3ef2c8eef7ce6348c4520bb2ee5` |
| `changes` | `b13ab8f86b0a58207c7b1b7195fd3ee2997a675613b6bd73e650b5bbd58672e9` |
| `modify.list` | `a41ce1313d5f2d2f66a62c0715a25e7e5975496b25603c3af7f9c8d588d9045b` |
| `delete.list` | `76b0ebcf93b97bd56c45cf2288396c4af15ebbde4770ea4adb39fdc295134c64` |
| `amix_21_patch.adf` | `674f3ac691fd032c2c79c2fe90e2cd158e3f7d6004da03c4820373c50060fdfe` |

## How it was made

1. Each package was unpacked with the host heirloom `pkgtrans`. Every pkgmap entry under `/etc` was laid down in package order (core first): files (`f`, `e`, `v`) from the datastream, directories, symlinks as links (`s`), and hard links (`l`) as real hard links. No `/etc` path is declared by two packages, and no hard link reaches outside etc/.
2. The patch archive was read (`lha pq archive.lha`, a newc cpio). Its 13 `etc/` members replace the package versions, with their own mode and owner. uid and gid are mapped to names through the stock core `/etc/passwd` and `/etc/group`. Its two three-link groups (`news`, `syslogd` and their rc links) are hard links. Patched: `/etc/inet/inetd.conf`, `/etc/lp/filter.table`, `/etc/lp/filter.table.i`, `/etc/profile`, `/etc/init.d/news`, `/etc/rc0.d/K40news`, `/etc/rc2.d/S91news`, `/etc/init.d/syslogd`, `/etc/rc0.d/K50syslogd`, `/etc/rc2.d/S70syslogd`, `/etc/sysinit`, `/etc/syslog.conf`, `/etc/uucp/Grades`.
3. The patch's `apply` script edits these `/etc` files, and the edits are applied: `/etc/motd`; `/etc/saf/serial/_pmtab`; `/etc/rc0.d/K74smtpd -> /etc/rc0.d/K50smtpd`.
   - motd becomes `\tSystem V Release 4.0\t\tAmiga Version 2.1p2a` (apply's own echo, VER=2a).
   - `_pmtab`: `sed s/:ldterm:/:ldterm,ttcompat:/`.
   - `K74smtpd` is renamed `K50smtpd`.
4. **Not applied: `apply`'s `useradd vmsys` and `useradd oasys`** (it also lists `etc/passwd` and `etc/shadow` in `modify.list`). `useradd` picks the UID on the box at patch time, so the result depends on the box: one installed 2.1c p2a system has `vmsys:x:102:1:The Vmsys User:/home/vmsys:/usr/bin/sh`, `oasys:x:103:1:The Oasys User:/home/oasys:/usr/bin/sh` and `*LK*` shadow lines, where a stock passwd (highest ordinary UID 100) would most likely get 101 and 102. passwd and shadow are therefore kept as core ships them; an installer that applies the patch adds the two users itself.
5. Hashes are taken over the staged files. The table carries the owner and mode git cannot.

For comparison, that system's `/etc/motd` reads `Amiga Version 2.1`, without `p2a`, and it has no `/usr/include/patchlevel.h`. It got the patch content without `apply`'s last two steps. This tree follows `apply`.

## Checks

- Only `etc/` and this file were added.
- No Kickstart ROM: no file starts with a ROM header (`0x1111`, `0x1114`, `0x1116`, `AMIROMTYPE`). The largest files are the stock `bkup/method` ELF executables (≤ 179 KB).
- amix's privacy check over the tree finds no private addresses or host names; these are Commodore defaults.

## Named pipes (not in git)

| path | mode | owner | group |
|---|---|---|---|
| `/etc/saf/inetd/_pmpipe` | 0600 | root | root |
| `/etc/saf/tcp/_pmpipe` | 0600 | root | root |
| `/etc/saf/xdm/_pmpipe` | 0600 | root | root |

## Empty directories (not kept by git)

- `etc/lp/classes/`
- `etc/lp/forms/`
- `etc/lp/interfaces/`
- `etc/lp/printers/`
- `etc/lp/pwheels/`
- `etc/mail/lists/`
- `etc/opt/`
- `etc/perms/`

## Every path

type: `f` file, `l` symlink (target shown), `d` directory, `p` named pipe. Source: the package, or `patch` / `apply`.

| path | type | mode | owner | group | sha256 / target | source | note |
|---|---|---|---|---|---|---|---|
| `etc` | d | 0755 | root | root |  | uucp 2.1 |  |
| `etc/.login` | f | 0644 | bin | bin | `0104968158140ad46e9e15e97bd7255fd95808d593ad69bbe76e590677ad0396` | bsd 2.1 |  |
| `etc/TIMEZONE` | f | 0444 | root | sys | `0e34bac958a24cb240406eb0d363ac0d3d7eca2cc1d1d86541554b2042f194f0` | core 2.1 |  |
| `etc/acct` | d | 0775 | adm | adm |  | core 2.1 |  |
| `etc/acct/holidays` | f | 0664 | bin | bin | `a31b46e9090aebbdb29e6211bd578c41a0ffadffc507d1ec3cdd36448ff25326` | core 2.1 |  |
| `etc/amiga.ap` | f | 0644 | bin | bin | `70f01a663c925ae996913f5c74ee7353f22c4871e55d6ad1683e5c9f7aee757a` | core 2.1 |  |
| `etc/autopush` | l | - | - | - | → `/sbin/autopush` | core 2.1 |  |
| `etc/bcheckrc` | l | - | - | - | → `/sbin/bcheckrc` | core 2.1 |  |
| `etc/bkup` | d | 0755 | bin | bin |  | sysadm 2.1 |  |
| `etc/bkup/bkexcept.tab` | f | 0600 | root | sys | `ab0cbb5f42236b8fd1b8009f1ad3da5cfec80055f328af1d7e65eb73c2da9a1f` | sysadm 2.1 |  |
| `etc/bkup/bkreg.tab` | f | 0600 | root | sys | `7acc84ae97e372df5fab151fe021608519dd478bb01c60a024c30ec1c5db2832` | sysadm 2.1 |  |
| `etc/bkup/method` | d | 0755 | bin | bin |  | sysadm 2.1 |  |
| `etc/bkup/method/fdisk` | f | 0700 | root | sys | `abc9fc06a146a58e144c02ed77b89392f745e6f5962d682f71e40e1ad3caf506` | sysadm 2.1 |  |
| `etc/bkup/method/fdp` | f | 0700 | root | sys | `058919016e9bad92a8e5963b229d7517bc0c13d129cdd9841931fda1f4a28fbb` | sysadm 2.1 |  |
| `etc/bkup/method/ffile` | f | 0700 | root | sys | `f028b8b841e3ea9f0a4da144efb6bc82c427480b4e744bf77b1604e3cc5c1c4b` | sysadm 2.1 |  |
| `etc/bkup/method/fimage` | f | 0700 | root | sys | `058919016e9bad92a8e5963b229d7517bc0c13d129cdd9841931fda1f4a28fbb` | sysadm 2.1 |  |
| `etc/bkup/method/incfile` | f | 0700 | root | sys | `f028b8b841e3ea9f0a4da144efb6bc82c427480b4e744bf77b1604e3cc5c1c4b` | sysadm 2.1 |  |
| `etc/bkup/method/migration` | f | 0700 | root | sys | `6a7c11afd8a9be6201fe89f64fff2d3e9531f3c5274b985169e1519d401d7ff9` | sysadm 2.1 |  |
| `etc/bkup/rsmethod.tab` | f | 0644 | root | sys | `3f07444eec65bf42e57b8dd097a621eb4c375ce1ba99df5c600e70af2e24d6f6` | sysadm 2.1 |  |
| `etc/bkup/rsstrat.tab` | f | 0644 | root | sys | `5d45468a3b1e53a14df5f7e8880eac9636306c4b76f0f74a579a73560e0bac1b` | sysadm 2.1 | |
| `etc/boot_tab` | f | 0744 | root | sys | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/brc` | l | - | - | - | → `/sbin/brc` | core 2.1 |  |
| `etc/brutab` | f | 0644 | bin | bin | `8ec524edc9e4158e6746a1e6b423355e7ac4d2c9e8760b39afaf6d70ea70adeb` | core 2.1 |  |
| `etc/buildsys` | l | - | - | - | → `/sbin/buildsys` | core 2.1 |  |
| `etc/bupsched` | f | 0644 | bin | bin | `decd60299b3330e192fee5f41809665a74080540f96044cf2e121eee3779d374` | sysadm 2.1 |  |
| `etc/checkall` | f | 0644 | bin | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/checklist` | f | 0644 | bin | bin | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/chroot` | l | - | - | - | → `/usr/sbin/chroot` | core 2.1 |  |
| `etc/ckbupscd` | l | - | - | - | → `/usr/sbin/ckbupscd` | core 2.1 |  |
| `etc/clri` | l | - | - | - | → `/sbin/clri` | core 2.1 |  |
| `etc/coredirs` | f | 0644 | bin | bin | `008bef3cb6f16cb483c1e307d83e6ed6aeabe7c8747d635f4435d9812d5ab71a` | core 2.1 | |
| `etc/crash` | l | - | - | - | → `/usr/sbin/crash` | core 2.1 |  |
| `etc/cron` | l | - | - | - | → `/usr/sbin/cron` | core 2.1 |  |
| `etc/cron.d` | d | 0755 | root | root |  | core 2.1 |  |
| `etc/cron.d/.proto` | f | 0744 | root | root | `1cc4302fc904b2156f1d2c4418dc51973963db4ecfb0f432f57f57771d06386a` | core 2.1 |  |
| `etc/cron.d/at.deny` | f | 0644 | root | sys | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/cron.d/cron.deny` | f | 0644 | root | sys | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/cron.d/logchecker` | f | 0544 | root | sys | `ae3518a6f2dbb1f414aabff39019fb728b35ea1efd06fb5c6ea3a552f5cef73c` | core 2.1 | |
| `etc/cron.d/queuedefs` | f | 0644 | root | sys | `9c899cd1d14ba94e73792f8979b59662a1d3afa5b3a166ba9ee7290841fa6147` | core 2.1 |  |
| `etc/datemsk` | f | 0444 | root | sys | `38b070b63bc60b3c2128b19e16d6d668b773f5ff6886637f6d9d4613a56e1eb5` | core 2.1 |  |
| `etc/dcopy` | l | - | - | - | → `/usr/sbin/dcopy` | core 2.1 |  |
| `etc/default` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/default/cron` | f | 0444 | root | sys | `d691280bdca88d8190a443601fe6457f24fbb1eb698c2dd669c93cd809be7b56` | core 2.1 |  |
| `etc/default/login` | f | 0444 | root | sys | `3495d606c4912e6fba8ca725b772667a52568e02242e40d4c71433e775b1b203` | core 2.1 |  |
| `etc/default/passwd` | f | 0444 | root | sys | `edb4b87817aada4c1c10959affa19c3572b4e0136586946888fffe1f2d7489cd` | core 2.1 |  |
| `etc/default/su` | f | 0444 | root | sys | `fff47db7d9b3b1e3cfc07f795fd418ed4e5c8b543ff09b37b8129b70665e50f1` | core 2.1 |  |
| `etc/default/tar` | f | 0444 | root | sys | `cf266a63fffbdb15885900661fe48b2e2f97ac8b59b74f3a46fcac5f1dce8448` | core 2.1 |  |
| `etc/device.tab` | f | 0440 | root | sys | `f294c537e6879a014bcd89481c0333209dcb65d3b99e7017506b5f95f01c6229` | core 2.1 |  |
| `etc/devnm` | f | 0755 | root | root | `65cda12f69b7e5a8afc0bbe06d51b5f31c5788e2708c70175b28206126333397` | core 2.1 |  |
| `etc/dfs` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/dfs/dfstab` | f | 0644 | root | sys | `5f9d72980de9c99fd58256de6e23339fa43846db93fc49da23cdffff5b622a77` | core 2.1 |  |
| `etc/dfs/fstypes` | f | 0644 | root | root | `b7723e31c15b1146d5132466ea32e443b9de16e1d6fb0323de4e4bb73ba920c3` | core 2.1 |  |
| `etc/dfsck` | l | - | - | - | → `/usr/sbin/dfsck` | core 2.1 |  |
| `etc/dgroup.tab` | f | 0440 | root | sys | `c65ec126b425840c6f7d7bafd292ddc55f4087735a09b71ac9d53b459c2e6b38` | core 2.1 |  |
| `etc/disketteparm` | f | 0444 | root | sys | `7ccfe4642deab650db04b7805e05c4236b2431b23a3c9b5240b8b6aac889946b` | core 2.1 |  |
| `etc/domain` | f | 0644 | bin | bin | `f6eb187c4451dda79d62d25830dc78b27ff5cbdb889f88244cb33521a34a7b6d` | core 2.1 |  |
| `etc/drvinstall` | l | - | - | - | → `/usr/sbin/drvinstall` | core 2.1 |  |
| `etc/ff` | l | - | - | - | → `/usr/sbin/ff` | core 2.1 |  |
| `etc/filesave` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/fs` | l | - | - | - | → `/usr/lib/fs` | core 2.1 |  |
| `etc/fsba` | l | - | - | - | → `/usr/sbin/fsba` | core 2.1 |  |
| `etc/fsck` | l | - | - | - | → `/sbin/fsck` | core 2.1 |  |
| `etc/fsdb` | l | - | - | - | → `/sbin/fsdb` | core 2.1 |  |
| `etc/fsstat` | l | - | - | - | → `/sbin/fsstat` | core 2.1 |  |
| `etc/fstab` | f | 0644 | bin | bin | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/fstyp` | l | - | - | - | → `/sbin/fstyp` | core 2.1 |  |
| `etc/fuser` | l | - | - | - | → `/usr/sbin/fuser` | core 2.1 |  |
| `etc/getscr` | l | - | - | - | → `/usr/amiga/etc/getscr` | core 2.1 |  |
| `etc/getty` | l | - | - | - | → `/usr/lib/saf/ttymon` | core 2.1 |  |
| `etc/gettydefs` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/group` | f | 0644 | bin | bin | `44c0bb0bda6fb524aa5d74ff303658c2271cb29c32d0ca51309f6d6a47df1970` | core 2.1 |  |
| `etc/grpck` | l | - | - | - | → `/usr/sbin/grpck` | core 2.1 |  |
| `etc/haltsys` | l | - | - | - | → `/sbin/haltsys` | core 2.1 |  |
| `etc/hosts` | l | - | - | - | → `/etc/inet/hosts` | core 2.1 |  |
| `etc/inet` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/inet/hosts` | f | 0444 | root | sys | `d70d38dedf229ffda8c849134b49055d2f0c02d370a73e3db9471774f464edd8` | core 2.1 |  |
| `etc/inet/inetd.conf` | f | 0444 | root | sys | `972fda14019fe17bb123d7a8926526c8e053ed907a3701608c5c237908d5914d` | patch 2.1p2a archive |  |
| `etc/inet/named.boot` | f | 0444 | root | sys | `403a9e857a87bd25a957cba3e9024e56508bcec11c9eacdc0ba9a52d85bc89c5` | core 2.1 |  |
| `etc/inet/network-config` | f | 0444 | root | sys | `e2c4dce7c4d188eb6124951c6f961016905ecec66ca2b56c724b51a287ae0087` | core 2.1 |  |
| `etc/inet/networks` | f | 0444 | root | sys | `ad0419f86d0a6d4cbe321a9d2dec5ad0236b15de707b5c6836fae4f975374030` | core 2.1 |  |
| `etc/inet/protocols` | f | 0444 | root | sys | `afca3fe686f31261d952261767c63d53815e13c4dfdb49121956f49a3a62edd6` | core 2.1 |  |
| `etc/inet/rc.inet` | f | 0444 | root | sys | `e33947d1c4848b12b7a94c68765629e46bd92ae9e76baec07714c29d6a1967e9` | core 2.1 |  |
| `etc/inet/routes` | f | 0444 | root | sys | `db16ad5d801ca70ddc79023f472c73adf2ce932d69e6b823420e98a51fee9131` | core 2.1 |  |
| `etc/inet/services` | f | 0444 | root | sys | `8348dfe2ad52cc72c956ddce17fdf5a22ce16149e750104bfa33e112487ae2f5` | core 2.1 |  |
| `etc/inet/shells` | f | 0444 | root | sys | `ab3442d371fb953a61d117496e06b8bd7db471cf12b782306dc0c1ea1337bff2` | core 2.1 |  |
| `etc/inet/strcf` | f | 0444 | root | sys | `e0e225826ebd276d204dafd5f3cef333b9a906d1746f1828991071ed804e63c8` | core 2.1 |  |
| `etc/inetd.conf` | l | - | - | - | → `/etc/inet/inetd.conf` | core 2.1 |  |
| `etc/init` | l | - | - | - | → `/sbin/init` | core 2.1 |  |
| `etc/init.d` | d | 0755 | bin | bin |  | uucp 2.1 |  |
| `etc/init.d/ANNOUNCE` | f | 0744 | root | sys | `872ba68f6edc288df97f8c3b4720f0d66945c229af8832f1e7ebcd67eccb1ad0` | core 2.1 | hard link of /etc/rc1.d/K00ANNOUNCE |
| `etc/init.d/MOUNTFSYS` | f | 0744 | root | sys | `4c5cb0948af748bc7a0ef2e67f78fc6af8e8fed5acda53c4a4220fa71af5eb71` | core 2.1 | hard link of /etc/rc2.d/S01MOUNTFSYS |
| `etc/init.d/PRESERVE` | f | 0444 | root | sys | `eca78bd9be60c4cf16e945a3d9ce52077b3bbe1acb2620b9573921cb8801dfe6` | core 2.1 | hard link of /etc/rc2.d/S02PRESERVE |
| `etc/init.d/README` | f | 0744 | root | sys | `7f1f02d2d3f00ac3a66046da826c50bd597afcb81542d121d0eb37a91e13f0ed` | core 2.1 | |
| `etc/init.d/RMTMPFILES` | f | 0744 | root | sys | `aadd94304c4d39cee0f2af521b2970dafbb9e3003c9731e27f0ce31f59497955` | core 2.1 | hard link of /etc/rc2.d/S05RMTMPFILES |
| `etc/init.d/acct` | f | 0444 | root | sys | `53cf6db59d2ce9b83e24f001770cd7fa55c537b164c1144bda36e308e14de82e` | core 2.1 | hard link of /etc/rc2.d/S80acct |
| `etc/init.d/adv` | f | 0744 | root | sys | `84f6253b6f8b98da042131e1ffc8048282122dead2a60b0362b50ea67e4356de` | core 2.1 | |
| `etc/init.d/cron` | f | 0744 | root | sys | `45d7356b24f31365d5a1390d11ba6645c1a514fd4725020a0ceb602c1dba4f2d` | core 2.1 | hard link of /etc/rc2.d/S75cron |
| `etc/init.d/fumounts` | f | 0744 | root | sys | `ac8871ae28623bd5721626d1a79297dd0e4e1d29f61cec169a8a0005b03f4ea2` | core 2.1 | hard link of /etc/rc2.d/K30fumounts |
| `etc/init.d/inetinit` | f | 0444 | root | sys | `cf9a3e966fdc1700e35cf804ed61e6c6fdda6d8078120f7846c419f053e31a98` | core 2.1 | hard link of /etc/rc2.d/S69inet |
| `etc/init.d/lp` | f | 0444 | root | sys | `54ecc14690e11efd69c95e94330a7b2c78d237cbc2cc2ceb789d03968a0ffd5e` | lp 2.1 | hard link of /etc/rc2.d/S80lp |
| `etc/init.d/mkdtab` | f | 0744 | root | sys | `080963fdc1feb38f02b0663bb5cf2e2dea486839945d27f4a2c3ae4b9349ebcb` | core 2.1 | hard link of /etc/rc2.d/S15mkdtab |
| `etc/init.d/news` | f | 0544 | news | news | `f88205bb86f833366685c353be527575e1310aa6fd4c4bd435f3fc0bc6fe2cec` | patch 2.1p2a archive | hard link with /etc/rc0.d/K40news, /etc/rc2.d/S91news |
| `etc/init.d/nfs` | f | 0744 | root | sys | `80be2745b61386051d1f2ae95113399c549059804d3889402e61154edb3fb4a7` | net 2.1 | hard link of /etc/rc3.d/S22nfs |
| `etc/init.d/perf` | f | 0444 | root | sys | `cf3de44de0845d075a08b223ed2ee1189c4200a52da2f7abcaaee66b5487f79e` | core 2.1 |  |
| `etc/init.d/rfs` | f | 0744 | root | sys | `bcb2d43853c3e1e7e64782caed0c4f2b799c8ddce967454bc4e89c7e93c1dabb` | net 2.1 | hard link of /etc/rc3.d/S21rfs |
| `etc/init.d/rpc` | f | 0744 | root | sys | `8967be2870c19cd452d69944447f3fcc73e9825f1195a8e7a4f07927b6f001a4` | net 2.1 | hard link of /etc/rc2.d/S75rpc |
| `etc/init.d/rumounts` | f | 0744 | root | sys | `8500bea0b92f112693cda58bb13eeb2214172f3b9051b0ead1ed30d5cbe7be3a` | net 2.1 | |
| `etc/init.d/smtpd` | f | 0444 | root | sys | `ef6d1a2437c403649eab2cc6cd144ee65684010683d2a94f62b975bb6ad5eedf` | net 2.1 | hard link of /etc/rc3.d/S88smtpd |
| `etc/init.d/swap` | f | 0755 | bin | bin | `a0cfb8067771cd74e4cef461fb8565f67769a4f02a2ce261bab59dbf5a6191ab` | core 2.1 | hard link of /etc/rc2.d/S90swap |
| `etc/init.d/sysetup` | f | 0744 | root | sys | `9ad9e2a294c29a773e952f588f1df92dcae6fe847a9c8873bc7716ed0edb07fa` | core 2.1 | hard link of /etc/rc2.d/S20sysetup |
| `etc/init.d/syslogd` | f | 0544 | root | root | `2050893ff756d8d2be4d9a2c6ec3ed9ccb6aa6b58d25fbedc6785eecd02bdb91` | patch 2.1p2a archive | hard link with /etc/rc0.d/K50syslogd, /etc/rc2.d/S70syslogd |
| `etc/init.d/uucp` | f | 0744 | root | sys | `497c4fd4ef6307aff3b3d16a8a0556c7634027e93635ad155d512f68faa42a9b` | uucp 2.1 | hard link of /etc/rc2.d/S70uucp |
| `etc/inittab` | f | 0664 | root | sys | `60a4e432628650cedbb01d4a366637abfef0f803d70920a3156e86ed4518530e` | core 2.1 |  |
| `etc/install` | l | - | - | - | → `/usr/sbin/install` | core 2.1 |  |
| `etc/ioctl.syscon` | f | 0644 | bin | bin | `4904d1f50c78cf7699c4453e8899ae66411ff2a6018b0506e8b51da479afaa4b` | core 2.1 |  |
| `etc/killall` | l | - | - | - | → `/usr/sbin/killall` | core 2.1 |  |
| `etc/labelit` | l | - | - | - | → `/usr/sbin/labelit` | core 2.1 |  |
| `etc/ldsysdump` | l | - | - | - | → `/usr/sbin/ldsysdump` | core 2.1 |  |
| `etc/link` | l | - | - | - | → `/usr/sbin/link` | core 2.1 |  |
| `etc/lp` | d | 0775 | lp | lp |  | lp 2.1 |  |
| `etc/lp/Systems` | f | 0644 | lp | tty | `8cf79c0e6c211fe084cea5f759886b78fe26f426e5e0b2e29cc1a5c31bb483b1` | lp 2.1 |  |
| `etc/lp/classes` | d | 0775 | lp | lp |  | lp 2.1 |  |
| `etc/lp/fd` | d | 0775 | lp | lp |  | lp 2.1 |  |
| `etc/lp/fd/download.fd` | f | 0644 | root | root | `843e2d627bff9cb8244e0df0ba5fb287924ef4fe2d0b09abbee5b6a878899889` | lp 2.1 |  |
| `etc/lp/fd/dpost.fd` | f | 0664 | root | root | `aec6fd8ab3f05ddfdd07ad63330569d9ededd9eef2ef2c5d9eec8e654c8253ce` | lp 2.1 | |
| `etc/lp/fd/postdaisy.fd` | f | 0664 | root | root | `1f55f3227dc45008e907402c4d0e8193f67fbad6446b02bd91e1891ec7349b70` | lp 2.1 |  |
| `etc/lp/fd/postdmd.fd` | f | 0664 | root | root | `0ba62b27639fbd1989afd4b673b79c5bcbb188ac04a365d57f11cb09d4221b2a` | lp 2.1 | |
| `etc/lp/fd/postio.fd` | f | 0664 | root | root | `1f7b25d5bcd04c5074c36e61d91c928991918f26876fcf25fe808bbfc754ec67` | lp 2.1 |  |
| `etc/lp/fd/postio_b.fd` | f | 0664 | root | root | `e27b01aab8f958e4275a92e41cc28153d182ededd84e3882371ffc58bf46003a` | lp 2.1 |  |
| `etc/lp/fd/postio_br.fd` | f | 0664 | root | root | `79dddbc06c12e2a76f3a53f5804b17a03c4dbc098b7d86fff99509ab42da81d5` | lp 2.1 |  |
| `etc/lp/fd/postio_r.fd` | f | 0664 | root | root | `abcfa646541be38aaa30e9b6bc875b3a036c4291abd65d77d65fc1ba32ed4c01` | lp 2.1 |  |
| `etc/lp/fd/postior.fd` | f | 0664 | root | root | `3a2afa3db65364de320e876e619016a8a74796d0dbc07a53410a014bf09d058e` | lp 2.1 |  |
| `etc/lp/fd/postmd.fd` | f | 0664 | root | root | `0dcad3fc77922d21252ae952acfa406b13a6a22d952d3fbbeba26b0f24fc52cf` | lp 2.1 | |
| `etc/lp/fd/postplot.fd` | f | 0664 | root | root | `4349286de95189fca17ff6a4022d65252d98ccc52fb2feee218e835c6cd9242c` | lp 2.1 |  |
| `etc/lp/fd/postprint.fd` | f | 0664 | root | root | `85ecf6374b4bd0cb611016fc725c34ca34004f5d357500703add10d59e179b86` | lp 2.1 | |
| `etc/lp/fd/postreverse.fd` | f | 0664 | root | root | `7c6029d839c57a606046154eb986e80335dd2e24ba991438db783bde0143856d` | lp 2.1 |  |
| `etc/lp/fd/posttek.fd` | f | 0664 | root | root | `9e9fcab07b8a3e2023aef38444d746a7e4c275fb3030edb86c061d74bcad8e5a` | lp 2.1 | |
| `etc/lp/filter.table` | f | 0664 | lp | lp | `43d5cfeaa78a1d4e39574e68f899bbff6a4cededa11fc716ae6880e7525532be` | patch 2.1p2a archive |  |
| `etc/lp/filter.table.i` | f | 0444 | lp | lp | `43d5cfeaa78a1d4e39574e68f899bbff6a4cededa11fc716ae6880e7525532be` | patch 2.1p2a archive |  |
| `etc/lp/forms` | d | 0775 | lp | lp |  | lp 2.1 |  |
| `etc/lp/interfaces` | d | 0775 | lp | lp |  | lp 2.1 |  |
| `etc/lp/logs` | l | - | - | - | → `/var/lp/logs` | lp 2.1 |  |
| `etc/lp/printers` | d | 0775 | lp | lp |  | lp 2.1 |  |
| `etc/lp/pwheels` | d | 0775 | lp | lp |  | lp 2.1 |  |
| `etc/magic` | f | 0444 | bin | bin | `ab1f74f0fdc540efe40f55b65c135c0f1d593294341fdb4b37b60938455155fe` | core 2.1 |  |
| `etc/mail` | d | 0775 | root | mail |  | core 2.1 |  |
| `etc/mail/binarsys` | f | 0644 | bin | mail | `db8555491cfc0120346377f23fbef58f7538a2a54729d5fa6c83c8d0f2ec8048` | core 2.1 |  |
| `etc/mail/lists` | d | 0775 | bin | mail |  | core 2.1 |  |
| `etc/mail/mailcnfg` | f | 0644 | bin | mail | `66f89727bc448e6642de26be7e74c1a0ae85887dc2d86a1ac9cb03d4159fdbfc` | core 2.1 |  |
| `etc/mail/mailsurr` | f | 0644 | bin | mail | `19d9164d7a49d47700da66de52da57d5ff7898b16b71751f8363dc3340b1599d` | core 2.1 |  |
| `etc/mail/namefiles` | f | 0644 | bin | mail | `d65b126bb553cf964cdf82abd07c2b2f521e4cc08b3fc3bda9a85c148f29ed36` | core 2.1 |  |
| `etc/mail/names` | f | 0644 | bin | mail | `64a11bc68606352bdee040df71c0a2e71b54ca75b99b73aff09d2ff1fe1a389c` | core 2.1 |  |
| `etc/masters` | f | 0644 | root | sys | `443ca5f931011b4861e78cd4ff2c474a701e5c0a6b076a041c4071153afb9baa` | core 2.1 | |
| `etc/mini_system` | f | 0644 | bin | bin | `a91d3993d07e0312d0d14be4980b3add299f9d2153e10cb7363801dbc8cc98bc` | core 2.1 | |
| `etc/mkfs` | l | - | - | - | → `/sbin/mkfs` | core 2.1 |  |
| `etc/mknod` | l | - | - | - | → `/sbin/mknod` | core 2.1 |  |
| `etc/mnttab` | f | 0644 | bin | bin | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/motd` | f | 0644 | bin | bin | `8710d1dac7da288f8e7cb20ea5f2c839d43dd8192ebd1f384526f0f1423e16a5` | core 2.1 + apply (motd line) | rewritten by apply: Amiga Version 2.1p2a |
| `etc/mount` | l | - | - | - | → `/sbin/mount` | core 2.1 |  |
| `etc/mountall` | l | - | - | - | → `/sbin/mountall` | core 2.1 |  |
| `etc/mvdir` | l | - | - | - | → `/usr/sbin/mvdir` | core 2.1 |  |
| `etc/named.boot` | l | - | - | - | → `/etc/inet/named.boot` | core 2.1 |  |
| `etc/ncheck` | l | - | - | - | → `/usr/sbin/ncheck` | core 2.1 |  |
| `etc/net` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/net/servers` | l | - | - | - | → `/usr/net/servers` | core 2.1 |  |
| `etc/net/ticlts` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/net/ticlts/hosts` | f | 0444 | root | sys | `4e8e8444b4d266126ac30084fcb5d3e6b28c7052b49114767e760e7197a36593` | core 2.1 |  |
| `etc/net/ticlts/services` | f | 0444 | root | sys | `2f417e3ea6e3792d9a768941f39eda0b44f22ace29ce43282d479542b07c35ec` | core 2.1 |  |
| `etc/net/ticots` | l | - | - | - | → `/etc/net/ticlts` | core 2.1 |  |
| `etc/net/ticotsord` | l | - | - | - | → `/etc/net/ticlts` | core 2.1 |  |
| `etc/netconfig` | f | 0644 | bin | bin | `e1a77d55b830042f8ce0088d0fec7da75db0573256fd3680e9290a45f8387070` | core 2.1 |  |
| `etc/netconfig.DNS` | f | 0644 | bin | bin | `6ae2dd6dcd22b79f787d9ef27afae1887dd32233d8879e22f59bcb292e4ddf40` | core 2.1 |  |
| `etc/netid` | f | 0644 | root | sys | `000cddff1bc0458f208f510ead15198abf43a6cfdfd4ce7d520b270736ef0e03` | core 2.1 |  |
| `etc/networks` | l | - | - | - | → `/etc/inet/networks` | core 2.1 |  |
| `etc/nfs` | d | 0755 | bin | bin |  | net 2.1 |  |
| `etc/nfs/exportfs` | l | - | - | - | → `/usr/lib/nfs/exportfs` | net 2.1 |  |
| `etc/nodename` | f | 0644 | bin | bin | `d906aecb61d076a967d9ffe8821c7b04b063f72df9d9e35b33ef36b1c0d98f16` | core 2.1 |  |
| `etc/opt` | d | 0755 | root | root |  | core 2.1 |  |
| `etc/passwd` | f | 0644 | root | root | `9a7c21a1105aaeb083c427ac6bf5dbfda96cacdfdff1c0b1e56bb9df102a3215` | core 2.1 |  |
| `etc/perms` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/powerfail` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/prfdc` | l | - | - | - | → `/usr/sbin/prfdc` | core 2.1 |  |
| `etc/prfld` | l | - | - | - | → `/usr/sbin/prfld` | core 2.1 |  |
| `etc/prfpr` | l | - | - | - | → `/usr/sbin/prfpr` | core 2.1 |  |
| `etc/prfsnap` | l | - | - | - | → `/usr/sbin/prfsnap` | core 2.1 |  |
| `etc/prfstat` | l | - | - | - | → `/usr/sbin/prfstat` | core 2.1 |  |
| `etc/profile` | f | 0644 | root | sys | `c691b951f10d96446fd2fefa0744bbe406d39a3b56da1c030767057b156e8219` | patch 2.1p2a archive |  |
| `etc/protocols` | l | - | - | - | → `/etc/inet/protocols` | core 2.1 |  |
| `etc/publickey` | f | 0644 | root | sys | `2ec0d64f68f3c20f90a5cf613630ebdbb5ca7d84c8b689f95999bcf54110fb6c` | core 2.1 | |
| `etc/pwck` | l | - | - | - | → `/usr/sbin/pwck` | core 2.1 |  |
| `etc/rc` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/rc.d` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/rc.d/sysetup` | f | 0644 | bin | bin | `4fd120e0397af497aa66b2c802bff584ba9260231c278c960853881bc8bdcf7a` | core 2.1 | |
| `etc/rc0` | l | - | - | - | → `/sbin/rc0` | core 2.1 |  |
| `etc/rc0.d` | d | 0755 | bin | bin |  | net 2.1 |  |
| `etc/rc0.d/K00ANNOUNCE` | f | 0744 | root | sys | `872ba68f6edc288df97f8c3b4720f0d66945c229af8832f1e7ebcd67eccb1ad0` | core 2.1 | hard link of /etc/rc1.d/K00ANNOUNCE |
| `etc/rc0.d/K05swap` | f | 0755 | bin | bin | `a0cfb8067771cd74e4cef461fb8565f67769a4f02a2ce261bab59dbf5a6191ab` | core 2.1 | hard link of /etc/rc2.d/S90swap |
| `etc/rc0.d/K20lp` | f | 0444 | root | sys | `54ecc14690e11efd69c95e94330a7b2c78d237cbc2cc2ceb789d03968a0ffd5e` | lp 2.1 | hard link of /etc/rc2.d/S80lp |
| `etc/rc0.d/K40acct` | f | 0444 | root | sys | `53cf6db59d2ce9b83e24f001770cd7fa55c537b164c1144bda36e308e14de82e` | core 2.1 | hard link of /etc/rc2.d/S80acct |
| `etc/rc0.d/K40news` | f | 0544 | news | news | `f88205bb86f833366685c353be527575e1310aa6fd4c4bd435f3fc0bc6fe2cec` | patch 2.1p2a archive | hard link with /etc/init.d/news, /etc/rc2.d/S91news |
| `etc/rc0.d/K40nfs` | f | 0744 | root | sys | `80be2745b61386051d1f2ae95113399c549059804d3889402e61154edb3fb4a7` | net 2.1 | hard link of /etc/rc3.d/S22nfs |
| `etc/rc0.d/K50fumounts` | f | 0744 | root | sys | `ac8871ae28623bd5721626d1a79297dd0e4e1d29f61cec169a8a0005b03f4ea2` | core 2.1 | hard link of /etc/rc2.d/K30fumounts |
| `etc/rc0.d/K50rpc` | f | 0744 | root | sys | `8967be2870c19cd452d69944447f3fcc73e9825f1195a8e7a4f07927b6f001a4` | net 2.1 | hard link of /etc/rc2.d/S75rpc |
| `etc/rc0.d/K50smtpd` | f | 0444 | root | sys | `c709ae13bed1e8dba749e563184f4e522dd05b19d5c3ece46e449170f842d7e1` | net 2.1 + apply (mv) | apply: renamed from K74smtpd |
| `etc/rc0.d/K50syslogd` | f | 0544 | root | root | `2050893ff756d8d2be4d9a2c6ec3ed9ccb6aa6b58d25fbedc6785eecd02bdb91` | patch 2.1p2a archive | hard link with /etc/init.d/syslogd, /etc/rc2.d/S70syslogd |
| `etc/rc0.d/K60rumounts` | f | 0744 | root | sys | `8500bea0b92f112693cda58bb13eeb2214172f3b9051b0ead1ed30d5cbe7be3a` | net 2.1 | |
| `etc/rc0.d/K65rfs` | f | 0744 | root | sys | `bcb2d43853c3e1e7e64782caed0c4f2b799c8ddce967454bc4e89c7e93c1dabb` | net 2.1 | hard link of /etc/rc3.d/S21rfs |
| `etc/rc0.d/K69inet` | f | 0444 | root | sys | `cf9a3e966fdc1700e35cf804ed61e6c6fdda6d8078120f7846c419f053e31a98` | core 2.1 | hard link of /etc/rc2.d/S69inet |
| `etc/rc0.d/K70cron` | f | 0744 | root | sys | `45d7356b24f31365d5a1390d11ba6645c1a514fd4725020a0ceb602c1dba4f2d` | core 2.1 | hard link of /etc/rc2.d/S75cron |
| `etc/rc1` | l | - | - | - | → `/sbin/rc1` | core 2.1 |  |
| `etc/rc1.d` | d | 0755 | bin | bin |  | net 2.1 |  |
| `etc/rc1.d/K00ANNOUNCE` | f | 0744 | root | sys | `872ba68f6edc288df97f8c3b4720f0d66945c229af8832f1e7ebcd67eccb1ad0` | core 2.1 |  |
| `etc/rc1.d/K40acct` | f | 0444 | root | sys | `53cf6db59d2ce9b83e24f001770cd7fa55c537b164c1144bda36e308e14de82e` | core 2.1 | hard link of /etc/rc2.d/S80acct |
| `etc/rc1.d/K50fumounts` | f | 0744 | root | sys | `ac8871ae28623bd5721626d1a79297dd0e4e1d29f61cec169a8a0005b03f4ea2` | core 2.1 | hard link of /etc/rc2.d/K30fumounts |
| `etc/rc1.d/K50rpc` | f | 0744 | root | sys | `8967be2870c19cd452d69944447f3fcc73e9825f1195a8e7a4f07927b6f001a4` | net 2.1 | hard link of /etc/rc2.d/S75rpc |
| `etc/rc1.d/K60rumounts` | f | 0744 | root | sys | `8500bea0b92f112693cda58bb13eeb2214172f3b9051b0ead1ed30d5cbe7be3a` | core 2.1 | hard link of /etc/rc2.d/K40rumounts |
| `etc/rc1.d/K65rfs` | f | 0744 | root | sys | `bcb2d43853c3e1e7e64782caed0c4f2b799c8ddce967454bc4e89c7e93c1dabb` | net 2.1 | hard link of /etc/rc3.d/S21rfs |
| `etc/rc1.d/K69inet` | f | 0444 | root | sys | `cf9a3e966fdc1700e35cf804ed61e6c6fdda6d8078120f7846c419f053e31a98` | core 2.1 | hard link of /etc/rc2.d/S69inet |
| `etc/rc1.d/K70cron` | f | 0744 | root | sys | `45d7356b24f31365d5a1390d11ba6645c1a514fd4725020a0ceb602c1dba4f2d` | core 2.1 | hard link of /etc/rc2.d/S75cron |
| `etc/rc1.d/S01MOUNTFSYS` | f | 0744 | root | sys | `4c5cb0948af748bc7a0ef2e67f78fc6af8e8fed5acda53c4a4220fa71af5eb71` | core 2.1 | hard link of /etc/rc2.d/S01MOUNTFSYS |
| `etc/rc2` | l | - | - | - | → `/sbin/rc2` | core 2.1 |  |
| `etc/rc2.d` | d | 0755 | bin | bin |  | uucp 2.1 |  |
| `etc/rc2.d/K20nfs` | f | 0744 | root | sys | `80be2745b61386051d1f2ae95113399c549059804d3889402e61154edb3fb4a7` | net 2.1 | hard link of /etc/rc3.d/S22nfs |
| `etc/rc2.d/K30fumounts` | f | 0744 | root | sys | `ac8871ae28623bd5721626d1a79297dd0e4e1d29f61cec169a8a0005b03f4ea2` | core 2.1 |  |
| `etc/rc2.d/K40rumounts` | f | 0744 | root | sys | `8500bea0b92f112693cda58bb13eeb2214172f3b9051b0ead1ed30d5cbe7be3a` | core 2.1 | |
| `etc/rc2.d/K50rfs` | f | 0744 | root | sys | `bcb2d43853c3e1e7e64782caed0c4f2b799c8ddce967454bc4e89c7e93c1dabb` | net 2.1 | hard link of /etc/rc3.d/S21rfs |
| `etc/rc2.d/S01MOUNTFSYS` | f | 0744 | root | sys | `4c5cb0948af748bc7a0ef2e67f78fc6af8e8fed5acda53c4a4220fa71af5eb71` | core 2.1 |  |
| `etc/rc2.d/S02PRESERVE` | f | 0444 | root | sys | `eca78bd9be60c4cf16e945a3d9ce52077b3bbe1acb2620b9573921cb8801dfe6` | core 2.1 |  |
| `etc/rc2.d/S05RMTMPFILES` | f | 0744 | root | sys | `aadd94304c4d39cee0f2af521b2970dafbb9e3003c9731e27f0ce31f59497955` | core 2.1 | |
| `etc/rc2.d/S15mkdtab` | f | 0744 | root | sys | `080963fdc1feb38f02b0663bb5cf2e2dea486839945d27f4a2c3ae4b9349ebcb` | core 2.1 |  |
| `etc/rc2.d/S20sysetup` | f | 0744 | root | sys | `9ad9e2a294c29a773e952f588f1df92dcae6fe847a9c8873bc7716ed0edb07fa` | core 2.1 | |
| `etc/rc2.d/S69inet` | f | 0444 | root | sys | `cf9a3e966fdc1700e35cf804ed61e6c6fdda6d8078120f7846c419f053e31a98` | core 2.1 |  |
| `etc/rc2.d/S70syslogd` | f | 0544 | root | root | `2050893ff756d8d2be4d9a2c6ec3ed9ccb6aa6b58d25fbedc6785eecd02bdb91` | patch 2.1p2a archive | hard link with /etc/init.d/syslogd, /etc/rc0.d/K50syslogd |
| `etc/rc2.d/S70uucp` | f | 0744 | root | sys | `497c4fd4ef6307aff3b3d16a8a0556c7634027e93635ad155d512f68faa42a9b` | uucp 2.1 |  |
| `etc/rc2.d/S72rwhod` | f | 0644 | root | root | `30d3df4ea00bc2517b2c0933e224319301f655461ec289a5b1c2c00952a118f7` | net 2.1 |  |
| `etc/rc2.d/S75cron` | f | 0744 | root | sys | `45d7356b24f31365d5a1390d11ba6645c1a514fd4725020a0ceb602c1dba4f2d` | core 2.1 |  |
| `etc/rc2.d/S75rpc` | f | 0744 | root | sys | `8967be2870c19cd452d69944447f3fcc73e9825f1195a8e7a4f07927b6f001a4` | net 2.1 |  |
| `etc/rc2.d/S80acct` | f | 0444 | root | sys | `53cf6db59d2ce9b83e24f001770cd7fa55c537b164c1144bda36e308e14de82e` | core 2.1 |  |
| `etc/rc2.d/S80lp` | f | 0444 | root | sys | `54ecc14690e11efd69c95e94330a7b2c78d237cbc2cc2ceb789d03968a0ffd5e` | lp 2.1 | |
| `etc/rc2.d/S88smtpd` | f | 0444 | root | sys | `ef6d1a2437c403649eab2cc6cd144ee65684010683d2a94f62b975bb6ad5eedf` | net 2.1 | hard link of /etc/rc3.d/S88smtpd |
| `etc/rc2.d/S90swap` | f | 0755 | bin | bin | `a0cfb8067771cd74e4cef461fb8565f67769a4f02a2ce261bab59dbf5a6191ab` | core 2.1 |  |
| `etc/rc2.d/S91news` | f | 0544 | news | news | `f88205bb86f833366685c353be527575e1310aa6fd4c4bd435f3fc0bc6fe2cec` | patch 2.1p2a archive | hard link with /etc/init.d/news, /etc/rc0.d/K40news |
| `etc/rc3` | l | - | - | - | → `/sbin/rc3` | core 2.1 |  |
| `etc/rc3.d` | d | 0755 | bin | bin |  | net 2.1 |  |
| `etc/rc3.d/S21rfs` | f | 0744 | root | sys | `bcb2d43853c3e1e7e64782caed0c4f2b799c8ddce967454bc4e89c7e93c1dabb` | net 2.1 |  |
| `etc/rc3.d/S22nfs` | f | 0744 | root | sys | `80be2745b61386051d1f2ae95113399c549059804d3889402e61154edb3fb4a7` | net 2.1 |  |
| `etc/rc3.d/S88smtpd` | f | 0444 | root | sys | `ef6d1a2437c403649eab2cc6cd144ee65684010683d2a94f62b975bb6ad5eedf` | net 2.1 |  |
| `etc/rc6` | l | - | - | - | → `/sbin/rc6` | core 2.1 |  |
| `etc/rfs` | d | 0755 | bin | bin |  | net 2.1 |  |
| `etc/rfs/rfuadmin` | f | 0554 | bin | bin | `2369b904f85b150d447353f97ed0544ce70b92ee3985eb995b3d838822dfec69` | net 2.1 |  |
| `etc/rfs/rmnttry` | f | 0550 | root | sys | `c29da626eda974ac5e31c799231477e01fd51b90f25311025132c73fb5c6fb65` | net 2.1 |  |
| `etc/rfs/rmount` | f | 0555 | root | sys | `8d0eef717047a26959740b4cf59d409c0b25a0f7b4b5d35b1448197db0667fab` | net 2.1 |  |
| `etc/rfs/rumount` | f | 0555 | root | sys | `a5cfc08a0669145f62845848ec8b0005e46a500bd0a713dc0780381aaa56ea6b` | net 2.1 |  |
| `etc/rmount` | l | - | - | - | → `/usr/sbin/rmount` | core 2.1 |  |
| `etc/rmountall` | l | - | - | - | → `/usr/sbin/rmountall` | core 2.1 |  |
| `etc/routes` | l | - | - | - | → `/etc/inet/routes` | core 2.1 |  |
| `etc/rpc` | f | 0644 | root | sys | `2a4269c8e04604ffdef696f7905de3b598667c08e13825d3c0203f78276d3e0f` | net 2.1 | |
| `etc/rumountall` | l | - | - | - | → `/usr/sbin/rumountall` | core 2.1 |  |
| `etc/rwhod` | l | - | - | - | → `/usr/sbin/in.rwhod` | core 2.1 |  |
| `etc/saf` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/saf/_sactab` | f | 0644 | root | sys | `ad0b887950f7f53a597e89709c7de04f48da8c66fe04a3edad5b0c525fe50cc7` | core 2.1 |  |
| `etc/saf/_sysconfig` | f | 0644 | root | sys | `308cdbc07fc02420bae720ca97479bd8cad3b3cd5444b34cc00b32b1fd57ce17` | core 2.1 |  |
| `etc/saf/inetd` | d | 0755 | root | root |  | core 2.1 |  |
| `etc/saf/inetd/_pid` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/saf/inetd/_pmpipe` | p | 0600 | root | root |  | core 2.1 | device node or pipe: not stageable in git |
| `etc/saf/inetd/_pmtab` | f | 0644 | root | root | `e7f5a3aaab70f7c83279048a42e8e66071507213354ea9a8e71ad3a7d6f4e19a` | core 2.1 |  |
| `etc/saf/screens` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/saf/screens/_pmtab` | f | 0644 | root | root | `af34eb8e598045055439fd1cad42ef338e3be430f8d9520c57d8e0c8be6169b3` | core 2.1 |  |
| `etc/saf/serial` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/saf/serial/_pmtab` | f | 0644 | root | root | `344055807083acc1742bd2131c15427ebcdc13f475d9d7f65e9a5e2760df3eb1` | core 2.1 + apply (sed) | apply: :ldterm: -> :ldterm,ttcompat: |
| `etc/saf/tcp` | d | 0755 | root | root |  | core 2.1 |  |
| `etc/saf/tcp/_pid` | f | 0644 | root | root | `487d8b409a45337202e6edf0d7cadcbba6587680d6a15723883ced12008936df` | core 2.1 |  |
| `etc/saf/tcp/_pmpipe` | p | 0600 | root | root |  | core 2.1 | device node or pipe: not stageable in git |
| `etc/saf/tcp/_pmtab` | f | 0644 | root | root | `9228c013595f7ea6fcc80a8a04432d71703076c02727285bf1df1446842bc621` | core 2.1 |  |
| `etc/saf/xdm` | d | 0755 | root | root |  | core 2.1 |  |
| `etc/saf/xdm/_pid` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/saf/xdm/_pmpipe` | p | 0600 | root | root |  | core 2.1 | device node or pipe: not stageable in git |
| `etc/saf/xdm/_pmtab` | f | 0644 | root | root | `e7f5a3aaab70f7c83279048a42e8e66071507213354ea9a8e71ad3a7d6f4e19a` | core 2.1 |  |
| `etc/save.d` | d | 0755 | bin | bin |  | core 2.1 |  |
| `etc/save.d/except` | f | 0444 | root | sys | `1327d574528194c6be7c2ab9d8ead059aec60a54a9923bae75324f890f9c837e` | core 2.1 |  |
| `etc/savecpio` | l | - | - | - | → `/usr/sbin/savecpio` | core 2.1 |  |
| `etc/screendefs` | f | 0644 | bin | bin | `00aa3a20381ea4cf7426098294655e882bd676bbe37b5256ce3a08938ce4e7bb` | core 2.1 |  |
| `etc/services` | l | - | - | - | → `/etc/inet/services` | core 2.1 |  |
| `etc/setclk` | l | - | - | - | → `/sbin/setclk` | core 2.1 |  |
| `etc/setmnt` | l | - | - | - | → `/sbin/setmnt` | core 2.1 |  |
| `etc/shadow` | f | 0400 | root | root | `72abf4f7500c44d4ac9e4c839ebe41fcd7c6cf7aff57a67a890a1521a6b6bd78` | core 2.1 |  |
| `etc/shells` | l | - | - | - | → `/etc/inet/shells` | core 2.1 |  |
| `etc/shutdown` | l | - | - | - | → `/sbin/shutdown` | core 2.1 |  |
| `etc/skel` | d | 0755 | root | root |  | core 2.1 |  |
| `etc/skel/.profile` | f | 0644 | root | other | `2207d6bca766c2080a4dea0f00c3eba55ec72e93b12dc5c29fe78b82cd116f3b` | core 2.1 | hard link of /etc/stdprofile |
| `etc/slaves` | f | 0644 | root | sys | `4b51849668d0bbbd72d9bdb2030a5d9f0d2625e5b9f2b592fcd8b38f20740c82` | core 2.1 | |
| `etc/stdprofile` | f | 0644 | root | other | `2207d6bca766c2080a4dea0f00c3eba55ec72e93b12dc5c29fe78b82cd116f3b` | core 2.1 |  |
| `etc/strcf` | l | - | - | - | → `/etc/inet/strcf` | core 2.1 |  |
| `etc/sulogin` | f | 4555 | root | bin | `c8b5f473da7440c85c965443ecae4aa567350841a989f9e48fab3965244064d5` | core 2.1 |  |
| `etc/swap` | l | - | - | - | → `/usr/sbin/swap` | core 2.1 |  |
| `etc/sysdef` | l | - | - | - | → `/usr/sbin/sysdef` | core 2.1 |  |
| `etc/sysinit` | f | 0555 | bin | bin | `33306105d8b702f53683c408ee4cfd65eb8a1e8e22a426d4305f20a0b7bab253` | patch 2.1p2a archive |  |
| `etc/syslog.conf` | f | 0444 | root | root | `fa943663e864fd9a3220f6c8981bd02ba40ebd2f87687e354abf97cda5fdda50` | patch 2.1p2a archive |  |
| `etc/system` | f | 0644 | bin | bin | `6f52f54d8fbe23a88e8d9d62b33eeb17c951b111ac9166e4e4056c9b92c27dc8` | core 2.1 | |
| `etc/system.mtc11` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/system.mtc12` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/system.un32` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/tapesave` | f | 0644 | root | root | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` | core 2.1 |  |
| `etc/tar` | l | - | - | - | → `/usr/sbin/tar` | core 2.1 |  |
| `etc/telinit` | l | - | - | - | → `/sbin/init` | core 2.1 |  |
| `etc/termcap` | l | - | - | - | → `/usr/share/lib/termcap` | terminfo 2.1 |  |
| `etc/ttydefs` | f | 0644 | root | sys | `da458830dded2a7b0791db4adfafd999b925ede20d1d5a26c60b3eb467624eeb` | core 2.1 |  |
| `etc/ttysrch` | f | 0644 | bin | bin | `adb8509eea3cf54e2eaa32beb5a1373a1ace2428eac8378b5773ed5732e7413c` | core 2.1 |  |
| `etc/uadmin` | l | - | - | - | → `/sbin/uadmin` | core 2.1 |  |
| `etc/umount` | l | - | - | - | → `/sbin/umount` | core 2.1 |  |
| `etc/umountall` | l | - | - | - | → `/sbin/umountall` | core 2.1 |  |
| `etc/unlink` | l | - | - | - | → `/usr/sbin/unlink` | core 2.1 |  |
| `etc/utmp` | l | - | - | - | → `/var/adm/utmp` | core 2.1 |  |
| `etc/utmpx` | l | - | - | - | → `/var/adm/utmpx` | core 2.1 |  |
| `etc/uucp` | d | 0755 | uucp | uucp |  | uucp 2.1 |  |
| `etc/uucp/Config` | f | 0644 | uucp | uucp | `4582c31ce603b9cff24082d726933aa565c49f8daf7a551181007be90e3a113c` | uucp 2.1 | |
| `etc/uucp/Devconfig` | f | 0644 | uucp | uucp | `8f2076d3d74f0e462f2a00d6866da37063ad65780f4cd4310316790f3b3d7c25` | uucp 2.1 |  |
| `etc/uucp/Devices` | f | 0644 | uucp | uucp | `dccc7492e32217175837a715b27856610072387fc9ee988fc034f3ffe64d38ed` | uucp 2.1 |  |
| `etc/uucp/Dialcodes` | f | 0644 | uucp | uucp | `1518b2a92bf0753893fac9aa70e86c4663589cc369e65b5840b4de1e9c806c33` | uucp 2.1 |  |
| `etc/uucp/Dialers` | f | 0644 | uucp | uucp | `8e15c809d12a5d0fe494a5aa89372789ccba035adb923ed4a22384ea26638eea` | uucp 2.1 |  |
| `etc/uucp/Grades` | f | 0644 | uucp | uucp | `3a503f49ffa3aff9fb185ce3d0a4a00a5729dcdd95c0ea0a94fae49211a2d41d` | patch 2.1p2a archive |  |
| `etc/uucp/Limits` | f | 0644 | uucp | uucp | `92dfcf54e6becc73ffb14f5b9596028981322869d0d816d7a31c1d4b4965d181` | uucp 2.1 | |
| `etc/uucp/Permissions` | f | 0600 | uucp | uucp | `f445d34593b48f789f655844dea217b916bf6ec6181626c39080dfe88027eb3a` | uucp 2.1 |  |
| `etc/uucp/Poll` | f | 0644 | uucp | uucp | `7ffc83e7c32f9d29616eadeef042ed4ea089eacbea0a3483d9411156ea81fab6` | uucp 2.1 |  |
| `etc/uucp/Sysfiles` | f | 0644 | uucp | uucp | `1f003a5e296b6e30dd41521a5af1be1b7905a77422a89b92354ee0d5d957c728` | uucp 2.1 | |
| `etc/uucp/Systems` | f | 0600 | uucp | uucp | `1afdb4d891a892ab8ae94be83a8e8126c7b804792fceb2d543d05a688db19aed` | uucp 2.1 |  |
| `etc/uucp/remote.unknown` | l | - | - | - | → `/usr/lib/uucp/remote.unknown` | uucp 2.1 |  |
| `etc/vfstab` | f | 0744 | root | sys | `bf7301110c3b34c9409f71daa844763f72f76809560a662e463f900bdff3f0ad` | core 2.1 |  |
| `etc/volcopy` | l | - | - | - | → `/usr/sbin/volcopy` | core 2.1 |  |
| `etc/wall` | l | - | - | - | → `/usr/sbin/wall` | core 2.1 |  |
| `etc/whodo` | l | - | - | - | → `/usr/sbin/whodo` | core 2.1 |  |
| `etc/wtmp` | l | - | - | - | → `/var/adm/wtmp` | core 2.1 |  |
| `etc/wtmpx` | l | - | - | - | → `/var/adm/wtmpx` | core 2.1 |  |
