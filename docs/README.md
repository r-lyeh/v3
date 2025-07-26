<h1 align="center"><a href="#">V·3</a></h1>
<p align="center">
Framework to build Game Engines.<br/>
<br/>
</p>

## Screenshots
Still WIP. TBD

## About
- [x] v3 is a minimal C framework to help you build custom, scalable game engines and creative apps.
- [x] v3 is modular at its core, allowing you to toggle or replace its APIs at runtime.
- [x] v3 is community-driven, enabling developers to create and share extensions without requiring approval.
- [x] v3 is a major overhaul of the previous [v2](https://github.com/r-lyeh/v2) and [v1](https://github.com/r-lyeh/v1) iterations, being lighter and more flexible than them.
- [x] v3 is released into the [**public domain**](https://choosealicense.com/licenses/unlicense/) or, at your discretion, licensed under the [0-BSD](https://choosealicense.com/licenses/0bsd/), [0MIT](https://choosealicense.com/licenses/mit-0/), or [uL](https://github.com/r-lyeh/uLicense).
- [x] v3 is minimalist by design. Key benefits include: <!-- v3 is a minimal C framework that lets you integrate your preferred tools and libraries via optional extensions-->
- [x] <!--**Customizable & Scalable**:-->Build only what you need, without unnecessary features.
- [x] <!--**Lightweight & Portable**:-->Small footprint that fits on low-end hardware and boosts development cycles.
- [x] <!--**Freedom of Choice**:-->Select libraries that match your preferred syntax or performance needs, avoiding rigid or unfamiliar APIs.
- [x] <!--**Community-Fueled**:-->The bring-your-own-tech approach encourages communities to experiment with v3 in new directions.

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
- [x] Archives: DIR, ZIP.
- [x] Audio: 2D, 3D, OGG, WAV.
- [x] Compression: Deflate.
- [x] Devices: Battery, Keyboard, Mouse, Gamepads.
- [x] Documents: INI, XML, JSON, JSON5, SJSON.
- [x] DS: Arrays, HashMaps and Sets.
- [x] Embed: resource files embedded automatically.
- [x] Images: WEBP, PNG, JPG, TGA, BMP, PSD, GIF...
- [x] Logger, callstacks, memory leaks finder, unit-tests.
- [x] OpenGL: GL2, GL3, GL4.
- [x] Scripts: Lua. <!-- move to an ext? -->
- [x] Time, threads, ~~timers, and coroutines (planned)~~ utilities.
- [x] UI: buttons, sliders, checkbox, colors, popups.

## Quickstart
- Double-click `v3.bat` file. Linux/OSX users can invoke `sh v3.bat` instead. That's all.
- Other commands,
    - `[sh] v3.bat tidy` to clean environment.
    - `[sh] v3.bat sync` to sync repo to latest.
    - `[sh] v3.bat ext`  to un/install extensions.
- Alternatively, you can compile via command-line:
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

## Tips
- Run `cc/cl v3.c -E > v3unity.h` to generate an amalgamated single-file header.
- This single-file header will work only with your specific dev environment, though.

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
