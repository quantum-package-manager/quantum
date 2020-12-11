# Quantum
## A Package Manager that doesn't screw with your system
### Version v0.1.5a

Quantum is primarily a non-root package Manager, that aims to be able to install a wide variety of software without messing with `/`
We also aim to be fast and user friendly, both to use and to write build files for. This was originally so [SEMC OS](https://github.com/semissioncontrol/semcOS) wouldnt have death by it's own package manager.<br>

Build Dependencies are `lua5.3` & `premake5` available on Void Linux, Arch Linux, & Gentoo Linux (there's probably more)<br>
Arch Linux: `lua53`<br>
Void Linux: `lua53`<br>
Gentoo Linux: `dev-lang/lua-5.3[.5]`
<br>Will add premake pkg names later

To install, install dependencies, clone and run
`./setup_env && premake5 gmake2 && make` and add `export PATH=~/quantum-lua/bin/:/usr/share/quantum/bin:$PATH` to your `.bashrc`, `.zshrc` or `.fishrc`<br>
Then you can move `build/release/quantum` to `/bin`

Syntax:
```bash
quantum install [pkg]
quantum install [pkg] [version]
quantum remove [pkg]
quantum update [pkg]
```

TODO:

Done:
* Make it so that you can run `quantum` from anywhere
* Add a (secondary) option for install pkgs systemwide
