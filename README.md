# Quantum
## A Package Manager that doesn't screw with your system

Quantum is primarily a non-root package Manager, that aims to be able to install a wide variety of software without messing with `/`
We also aim to be fast and user friendly, both to use and to write build files for. This was originally so [SEMC OS](https://github.com/semissioncontrol/semcOS) wouldnt have death by it's own package manager.<br>

Build Dependencies are `lua5.3` & `premake5` available on Void Linux, Arch Linux, & Gentoo Linux (there's probably more)<br>
Arch Linux: `lua53`<br>
Void Linux: `lua53`<br>
Gentoo Linux: `dev-lang/lua-5.3[.5]`
<br>Will add premake pkg names later

To install, install dependencies, clone and run
`./setup_env && premake5 gmake2 && make` and add `export PATH=~/quantum-lua/bin/:$PATH` to your `.bashrc` or `.zshrc`<br>
Then you can run `./build/release/quantum`, to, well run quantum (you can move the file to the local quantum directory)<br>
To Run:
Be in the directory and run:<br>
  `./quantum install <pkg>`<br>
  `./quantum remove <pkg>`<br>

TODO:
* Make it so that you can run `quantum` from anywhere
* Add a (secondary) option for install pkgs systemwide
