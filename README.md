# Quantum
## A Package Manager that doesn't screw with your system

Quantum is primarily a non-root pkg manager that installs things to a users home directory.<br>This feature was originally created so that computers running SEMCOS wouldn't have death by package manager, but then we split off into a seperate project. 

To install clone and run
`./setup_env && ./build` and add `export PATH=~/quantum/bin/:$PATH` to your `.bashrc` or `.zshrc`

To Run:
Be in the directory and run:<br>
  `./quantum install <pkg>`<br>
  `./quantum remove <pkg>`<br>
  
TODO:
* Make it so that you can run `quantum` from anywhere
* Add a (secondary) option for install pkgs systemwide