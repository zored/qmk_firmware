# QMK Ergodox EX Zored keymap
Differences from basic EZ config:
- Shorten finger movement:
	- Letters-modifiers (`z` is `ctrl` for example).
	- Numbers are F-keys.
	- Symbols layer has numbers.
- [Plover](http://www.openstenoproject.org/) layer.
- Easy to access symbols for programming.
- For fast touch-typing (no distracting lights, low timeouts).
- **Modifier dance** for `Z`, `X`, `C` or `/`, `.`, `,`: 
	- Hold for `Ctrl`, `Alt`, `GUI` (Command or Windows key).
	- Double hold to add Shift.
- And many other [features of QMK](https://docs.qmk.fm/#/features).

**See [keymap](./keymap.c)** for details.

## TODO  
- Move to C++ or even Rust ([here is repo](https://github.com/zored/alebastr-qmk-whitefox-keymap) for inspiration).
- CapsLock tap dance with shift.
