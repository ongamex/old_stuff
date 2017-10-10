Coord2D = {};
Coord2D.__index = Coord2D;

function Coord2D:new(_x, _y)
	local retval = {}
	setmetatable(retval, Coord2D)

	retval.x = _x;
	retval.y = _y;

	return retval;
end
