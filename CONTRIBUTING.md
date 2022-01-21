# Contributing

* Use clang-formatting, clang file is at the root, so before doing a PR make sure that it follows clang formatting
* File names should be camelCase or PascalCase
* Code structure should be similar to that in the other MCUs, this for in case it gets abstracted in the future, ex: have a while loop in main that updates they key state, and check if the kb is disabled, if it is then release they if it has not been released before
* If necessary, add the .vscode folder of the board so its easy to edit and manage inside VSCode

* File structure is the next:
```
- images
  \_ (Images used in the proyect, for example the README images)
- src
	\_ (MCU Name, Pico, ATMega32u4, etc)
		\_ (Files needed for it to compile in that MCU, for exmaple, Pico has a folder named src inside so that when you build it the build foder it is in the Pico folder instead next to the source code, while the ATMega32u4 has some shell files to upload or compile the code)
		- images
			\_ (Images of the board)
- .clang-format
- .gitignore
- CONTRIBUTING.md
- README.md
- LICENSE
```
