require "gl"
local math = require "math"

-- Implementing the CCXNodeUpdateProtocol is an limitation of Wax:
-- Wax cannot automatically detect an selector's signature. If MyLayer don't
-- implement CCXNodeUpdateProtocol, then "update:" selector will be 
-- considered an selector that only has an argument of type id. This will
-- fail this update(self, dt). However once supplied the information via
-- protocol CCXNodeUpdateProtocol, Wax can determine it's signature because
-- it can query the Obj-C runtime the signature of the method, from the protocol.
waxClass {"MyLayer", CCLayer, protocols = {"CCXNodeUpdateProtocol"} }

function init(self)
	self.super:init(self)
	self.player = CCSprite:initWithFile("grossini.png")
	self.player:setPosition(CGPoint(100, 100))
	self:addChild(self.player)
	self:schedule("update:")
	self.timer = 0;
	return self
end

function draw(self)
	self.super:draw(self)
	-- This shows how to use direct OpenGL ES functions, feel free to remote them.
	gl.ClearColor(1.0, 1.0, 0.0, 1.0)
	gl.Clear("COLOR_BUFFER_BIT")
end

function update(self, dt)
	local position = self.player:position()
	-- Move around the sprite
	self.timer = self.timer + dt
	position.x = 150 + math.sin(self.timer) * 100
	position.y = 200 + math.cos(self.timer) * 100
	self.player:setPosition(position)
end
