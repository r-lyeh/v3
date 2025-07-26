-- Lua demo
-- Run: `./hello.exe hello.lua`

print('v3, hello from ' .. _VERSION)

-- preload Teal
local libs = require("docs") -- mount docs/
tl = libs.tl -- global teal
tl.loader() -- install teal
package.path = "?.tl;" .. package.path -- prefer .tl over .lua extensions from this point

-- jump to hello.tl
local main = require("hello")
