<h1 align="center"><a href="#">V·3</a></h1>
<p align="center">
Framework to build Game Engines.<br/>
<br/>
</p>

## Screenshots
TBD

## About
- v3 is a minimal C framework to help you build custom, scalable game engines.
- v3 is modular at its core, allowing you to toggle or replace its APIs at runtime.
- v3 is community-driven, enabling developers to create and share extensions without requiring approval.
- v3 is a major overhaul of the previous [v2](https://github.com/r-lyeh/v2) and [v1](https://github.com/r-lyeh/v1) iterations, being lighter and more flexible than them.

## Features and roadmap ᕕ(ᐛ)ᕗ
- [x] ~~Modern C++~~ Simple C.
- [x] ~~Royaltie fee~~ Free.
- [x] ~~Licensing clauses~~ Nope.
- [x] ~~Full featured~~ Extendable.
- [x] ~~Rich build system~~ `gcc v3.c`
- [x] Ecosystem: Community-fueled by design, via decentralized extensions.
- [x] Modular: Module APIs can be toggled, replaced or switched at runtime.
- [x] Compilers: vs2022, gcc, clang, clang-cl, mingw.
- [x] Desktops: Windows, Linux, OSX. ~~Emscripten (untested).~~

## Preinstalled extensions
- [x] App: window, icon, cursors, alerts.
- [x] Input: Keyboard, mouse, battery and gamepad devices.
- [x] Archives: DIR, ZIP.
- [x] Images: PNG, PUG, JPG, TGA, BMP, PSD, GIF...
- [x] Scripts: Lua and Teal languages included. <!-- move to an ext? -->
- [x] UI: buttons, sliders, checkbox, colors, popups.
- [x] Time, threads, ~~timers, and coroutines (planned)~~ utilities.
- [x] Logger, callstacks, memory leaks finder, unit-tests.
- [x] Resources: embedded into binary automatically.
- [x] Compression: Deflate.
- [x] Documents: INI, XML, JSON, JSON5, SJSON.
- [x] OpenGL: 2, 3, 4.

## Minimalism everywhere. Why?
v3 is a minimal C framework that omits many standard systems by default, unless specific extensions are added. This deliberate choice lets developers integrate their preferred tools to build a tailored game engine, enabling game creation with minimal bloat. Key benefits include:
- [x] **Customizable & Scalable**: Build only what you need, without unnecessary features.
- [x] **Freedom of Choice**: Developers can select libraries that match their preferred syntax or performance needs, avoiding rigid or unfamiliar APIs.
- [x] **Lightweight & Portable**: Default small footprint that fits on low-end hardware and simplifies cross-platform development.
- [x] **Community-Fueled**: The "bring your own tech" approach encourages communities to experiment with v3 in new directions, driving its future through innovative contributions.

## Quickstart
- Double-click `v3.bat` file. Linux/OSX users can invoke `sh v3.bat` instead. That's all.
- Other commands,
    - `[sh] v3.bat tidy` to clean environment.
    - `[sh] v3.bat sync` to sync repo to latest.
    - `[sh] v3.bat ext`  to un/install extensions.
- Alternatively, you can compile the demo launcher manually.
```bash
echo win/vc       && cl v3.c                  
echo win/clang    && clang v3.c                
echo win/clang-cl && clang-cl v3.c             
echo win/mingw    && gcc v3.c -lgdi32 -ldbghelp -lole32 -lwininet
echo linux        && cc v3.c -lm -ldl -lpthread -lasound
echo osx          && SDKROOT=macosx10.14 cc -ObjC v3.c -framework cocoa -framework iokit -framework audiotoolbox
```

## Un/installing extensions
- Currently, [all these extensions💜 are hosted on GitHub](https://github.com/topics/ext-v3?s=updated).
- It is planned that the v3 launcher will be used to un/install extensions in the future.
- For now, you can un/install extensions via `v3 ext` script:
  - `v3 ext dir`
  - `v3 ext [add|del|syn] *`
  - `v3 ext [add|del|syn] user/repo`
  - `v3 ext [add|del|syn] @filelist.txt`

## Publishing and Sharing your extensions. <!-- [vfs] [mojoal] [physics(tokamak), ddraw] [scene, object, culling, pvs] [spine, spritesheets, tiles], [compression, encryption,] [pathfinding, boids, goap, behavior trees] [profiler, stats] [font, lockit]  [media: ffmpeg,assimp,pvrtoolcli,cuttlefish ... ase,tmx,fur,mid,mod,sfxr] [cloud services: steam/steamworks] [context hints: OpenGL, ~~Vulkan, MSAA, Debug~~] -->
TBD

## Credits
**Runtime and extensions**
[Andre Weissflog](https://github.com/floooh/sokol "for their many single-header libraries! (Zlib)"),
[Arne Koenig](https://github.com/arnkoen/headers "for their many single-header libraries! (μLicense)"),
[Dale Weiler and Bach Le](https://github.com/bullno1/libs "for XINCBIN (PD)"),
[James McLean](https://github.com/empyreanx/pico_headers "for their many single-header libraries! (PD)"),
[Lua authors and Eduardo Bart](https://www.lua.org/ "for Lua language and its almalgamation (MIT)"),
[Riley Mabb](https://github.com/ColleagueRiley/RGFW "for XDL and RGFW (Zlib)"),
[Rxi](https://github.com/rxi/microui "for microui (MIT)"),
[Sean Barrett](https://github.com/nothings/stb "for their awesome libraries!! (PD)"),
[Teal authors](https://teal-language.org "for Teal language (MIT)"),

## License
This software is released into the [public domain](https://choosealicense.com/licenses/unlicense/). Also multi-licensed as [0-BSD](https://choosealicense.com/licenses/0bsd/), [0MIT](https://choosealicense.com/licenses/mit-0/) or [uLicense](https://github.com/r-lyeh/uLicense) at your will for those countries where public domain is a concern (sigh). Any contribution to this repository is implicitly subjected to the same licensing terms aforementioned.

## Links
Still looking for alternatives? 
[amulet](https://github.com/ianmaclarty/amulet), 
[aroma](https://github.com/leafo/aroma/), 
[astera](https://github.com/tek256/astera), 
[blendelf](https://github.com/jesterKing/BlendELF), 
[bullordengine](https://github.com/MarilynDafa/Bulllord-Engine), 
[candle](https://github.com/EvilPudding/candle), 
[cave](https://github.com/kieselsteini/cave), 
[chickpea](https://github.com/ivansafrin/chickpea), 
[corange](https://github.com/orangeduck/Corange), 
[cute](https://github.com/RandyGaul/cute_framework), 
[dos-like](https://github.com/mattiasgustavsson/dos-like), 
[ejoy2d](https://github.com/ejoy/ejoy2d), 
[exengine](https://github.com/exezin/exengine), 
[game-framework](https://github.com/Planimeter/game-framework), 
[gunslinger](https://github.com/MrFrenik/gunslinger), 
[hate](https://github.com/excessive/hate), 
[horde3d](https://github.com/horde3d/Horde3D), 
[island](https://github.com/island-org/island), 
[juno](https://github.com/rxi/juno), 
[l](https://github.com/Lyatus/L), 
[limbus](https://github.com/redien/limbus), 
[love](https://github.com/love2d/love/), 
[lovr](https://github.com/bjornbytes/lovr), 
[mini3d](https://github.com/mini3d/mini3d), 
[mintaro](https://github.com/mackron/mintaro), 
[mio](https://github.com/ccxvii/mio), 
[ofx](https://openframeworks.cc), 
[olive.c](https://github.com/tsoding/olive.c), 
[opensource](https://github.com/w23/OpenSource), 
[ouzel](https://github.com/elnormous/ouzel/), 
[pez](https://github.com/prideout/pez), 
[pixie](https://github.com/mattiasgustavsson/pixie), 
[punity](https://github.com/martincohen/Punity), 
[r96](https://github.com/badlogic/r96), 
[ricotech](https://github.com/dbechrd/RicoTech), 
[rizz](https://github.com/septag/rizz), 
[rvnicraven](https://github.com/Captain4LK/RvnicRaven-ray), 
[tigr](https://github.com/erkkah/tigr), 
[yourgamelib](https://github.com/duddel/yourgamelib), 

<a href="https://github.com/r-lyeh/v3/issues"><img alt="Issues" src="https://img.shields.io/github/issues-raw/r-lyeh/v3.svg?label=Issues&logo=github&logoColor=white"/></a> <a href="https://discord.gg/yyjCkUQKPV"><img alt="Discord" src="https://img.shields.io/discord/354670964400848898?color=5865F2&label=Chat&logo=discord&logoColor=white"/></a> 

<!-- 
🖤
💜🟣🟪
💙🔵🟦🔷
💚🟢🟩
💛🟡🟨
🧡🟠🟧🔶
❤️🔴🟥
🤎🟤🟫
[x]What, []When, []Where, [x]Why, [x]How, [x]Which, [x]Who, and [x]Whom
-->
