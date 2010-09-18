require "MyLayer"

waxClass{"MyScene", CCScene}

function init(self)
	self.super:init(self)
	local myLayer = MyLayer:init()
	self:addChild(myLayer)
	return self
end
