# Quantum
## A Package Manager that doesn't screw with your system

Quantum is primarily a non-root pkg manager that installs things to a users home directory.<br>This feature was originally created so that computers running SEMCOS wouldn't have death by package manager, but then we split off into a seperate project. 

Build Dependencies are `premake` available on Void Linux, Arch Linux, & Gentoo LInux (there's probably more) or just `gcc`

To install clone and run
`./setup_env && premake5 gmake2 && make` and add `export PATH=~/quantum-lua/bin/:$PATH` to your `.bashrc` or `.zshrc`<br>
Then you can run `build/release/quantum` to, well run quantum (you can move the file to the local quantum directory)<br>
To Run:
Be in the directory and run:<br>
  `./quantum install <pkg>`<br>
  `./quantum remove <pkg>`<br>
  
TODO:
* Make it so that you can run `quantum` from anywhere
* Add a (secondary) option for install pkgs systemwide
