///\
\/// editor

// Basically, the editor is a mostly empty app, that composites multiple docked windows.
// - The editor collects user inputs, which are sent to focused windows.
// - Each window decides how to interpret user inputs and what to render from them.
// - Each window sends back a rendered texture to the editor, which composes the final picture.
//
//    editor ------> which reads inputs ---------> which are sent to window
//    ^--- which is displayed by <---------- which generates texture <----v
//
// editor = for each window[] : blit(.texture = window.draw(user.inputs))

// The window matrix is the place where all the existing windows are managed:
// - This large matrix is composed of events (columns) x plugins (rows).
// - Editor can be enlarged by inserting newer plugins (rows).
// - Editor can be extended by inserting newer events (columns).
//
// [editor.apps]
// name=dll|boot|init|play|stop|tick|draw|load|save|quit|cook|...
// Level Editor=level.dll|level_init|level_render|...
// #include "plugins/terrain.ini" ; embed external editor
// #include "plugins/blueprint.ini" ; embed external editor
// ...
//
#include "editor_app_enums.h"

// Each window is a standalone app that ticks and renders things into its own viewport.
// Ideally, each window should be able to:
// - Spawn things
// - Select things
// - Transform things
// - Align things
// - Delete things
// - Save things to disk
// - Load things from disk
// - Diff things state (ideally: OR,XOR,AND,NOT operations)
// - Import foreign things (read external > save lossless native type)
// - Export foreign things (read lossless > save external portable format)
// - Edit things (add/del/chg properties)
// - Cook things (read lossless > save lossy)
// - Pack things (specify how lossy files are compressed and grouped)
// - Reload things (if modified externally)
// - Debugdraw(view,proj) and Profile(time,mem,cost) its own things.
// - Drag n drop items to/from other windows: each drag n drop item is a url to a disk file.
//
// Window = viewport rect + dll plugins (tick,draw,load,save,heap) + dll tools (import,export)

#include "editor_app.h"

// Additionally, each window can inspect other global windows too [*game, *library, *settings, *play, *cvs] at any time.
// And communicate with them via `char*` messages. Drag and drop between windows will use this communication channel too.
// Because of human/debugging reasons, the messaging happens instantly and is never deferred or queued.

#include "editor_app_messages.h"

// The editor supports 4 built-in types: bool, int, float, char*
// (more complex types are built from these, e.g., vec2/3/4, mat44...)
//
// There is also a variant `var` type used to homogenize different
// return types in all editing call signatures: `var_t call();`

#include "editor_class_types.h"

// All actions (or verbs) that the editor can perform on any given type are defined in this header.
// The editor uses `var_t verb();` functors everywhere.
// These functors can point to external DLLs or symbols defined in current executable.

#include "editor_class_enums.h"

// The place where types and actions are linked together is the classmatrix (actions x types).
// Roughly speaking, this is a large .csv file, where columns are actions and rows are types. Therefore,
// - Define new editing operations by inserting new columns in the classmatrix.
// - Define new game types by inserting new rows in the classmatrix.
//
// [editor.classes]
// name,size,load,save,init,tick,draw,... ; header
// bool_name,bool_size,bool_load,bool_save,,,, ... ; built-in type
// float_name,float_size,float_load,float_save,,,, ... ; built-in type
// vec3_name,vec3_size,vec3_load,vec3_save,vec3_init,,, ... ; built-in type
// player_name,... ; custom type
// enemy1_name,... ; custom type
// #include "game/datatypes.ini" ; embed external type
//
// For convenience, you can also access each row type by using a generic `class_t` wrapper struct.

#include "editor_class.h"

//

#include "editor_compositor.h"



#if 0

### appendix A: scene url

/game/class/thing/
                  asset.ini -> contains howtos: edit,cook,draw,tick...
                  asset.png
                  asset.wav

resource ids are strings, which happen to be uris and match the physical location on disk
asset("/game/instance/player/2");

### appendix B: scene

levels are divided in chunks or scenes. each chunk decides:
- how many objects it contains: fixed, dynamic, adaptive, etc.
- how to allocate/free/pool the objects it contains.
- how to iterate the objects: eg, how to sort the objects in chunk.
- how to traverse the scene: scene_t *parent, *prev, *next, *child; nodes

levels receive top-bottom events (load,tick,draw,quit,...) and decide whether the child levels receive the event or not.
levels can generate top-bottom (child) and bottom-top (parent) events.

scene = objects[sorted by some criteria, allocated by some criteria] + scene *parent, *prev, *next, *child;
scene_t *level = scene_root;

this is a flexible system that enables you to:
- organize the level hierarchically, embracing PVS and preparing for culling opportunities.
- split large maps into pieces and stream contents from disk.
- organize the level per type basis, each chunk becoming an island thousand gameobjs of same type, if that`s what you want.
- etc

#endif
