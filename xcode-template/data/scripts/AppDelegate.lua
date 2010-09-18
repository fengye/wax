-- force reload the socket.lua and mime.lua, which is required by hybrid
-- luasocket library
loadfile("./data/scripts/socket.lua")()
loadfile("./data/scripts/mime.lua")()

-- Uncomment the following require statement if you want to load your .lua module
-- from an dedicated FTP server.
--require "customftploader"

-- Comment out the following require and remdebug start statement, if you don't
-- need remote debugger for lua.
require "remdebug.engine"
remdebug.engine.start()

require "MyScene"

waxClass{"AppDelegate", protocols = {"UIApplicationDelegate"}}

-- Well done! You are almost ready to run a lua app on your iPhone!
--
-- Just run the app (⌘↵) in the simulator or on a device! 
-- You should see a sprite moving around in the screen!
--
-- I prefer using TextMate to edit the Lua code. If that is your editor of
-- choice just type 'rake tm' from the command line to setup a wax
-- TextMate project.
--
-- What's next?
-- 1. Check out some of the example apps to learn how to do
--    more complicated apps.
-- 2. Then you start writing your app!
-- 3. Let us know if you need any help at http://groups.google.com/group/iphonewax

function applicationDidFinishLaunching(self, application)
  local frame = UIScreen:mainScreen():bounds()
  self.window = UIWindow:initWithFrame(frame)

  CCDirector:setDirectorType(CCDirectorTypeDisplayLink)

-- Use RGBA_8888 buffers. Default is: RGB_565 buffers
  CCDirector:sharedDirector():setPixelFormat(kPixelFormatRGBA8)  
  
  CCDirector:sharedDirector():setDeviceOrientation(CCDeviceOrientationPortrait)
  CCDirector:sharedDirector():setAnimationInterval(1.0/60)

  CCDirector:sharedDirector():setDisplayFPS(true)
  
  -- create an openGL view inside a window
  CCDirector:sharedDirector():attachInView(self.window)  
  CCDirector:sharedDirector():runWithScene(MyScene:node())
  
  self.window:makeKeyAndVisible()
end

function applicationWillResignActive(self, application)
  CCDirector:sharedDirector():pause()
end


function applicationDidBecomeActive(self, application)
  CCDirector:sharedDirector():resume()
end

function applicationDidReceiveMemoryWarning(self, application)
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
end

function applicationWillTerminate(self, application)
  -- we need to call CCDirector:sharedDirector():end() but
  -- end is a keyword in lua, so we have to fake it out
  -- note we still have to provide an pointer to CCDirector itself as "self" argument, manually
  local director = CCDirector:sharedDirector()
  director["end"](director)
end

function applicationSignificantTimeChange(self, application)
  CCDirector:sharedDirector():setNextDeltaTimeZero(true)
end

