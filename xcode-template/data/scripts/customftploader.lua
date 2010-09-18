require "string"
require "io"
require "socket.ftp"

module("customftploader", package.seeall)

-- provide your own ftp server url here
local ftpprefix = "ftp://ftpuser:ftpuser@localhost/devel/"

local function load(modulename)
  local errmsg = ""
  -- Find source
  local modulepath = string.gsub(modulename, "%.", "/")
  for path in string.gmatch(package.path, "([^;]+)") do
    local filename = string.gsub(path, "%?", modulepath)
    filename = ftpprefix .. filename
    local f, e = socket.ftp.get(filename)
    if e == nil then
    	return assert(loadstring(f))
    end
    errmsg = errmsg.."\n\tno file: "..filename.." (checked with custom ftp loader)"
  end
  return errmsg
end

-- Install the loader so that it's called just before the normal Lua loader
table.insert(package.loaders, 2, load)
