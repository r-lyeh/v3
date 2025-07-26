-- Teal demo: this is actually invoked from hello.lua file
-- Run: `./hello.exe hello.lua` instead

local function add(a: number, b: number): number
   return a + b
end

local s = add(1,2)
print('v'..s..', hello from Teal ' .. tl.version())
