--[[
Tile class description:

methods : 
	
members : 
	m_imageFileName
	m_isWalkable
]]

Tile = {};
Tile.__index = Tile;

----------------------------------------------------------------
function Tile:new(tileType)
	
	if tileType == nil then 
		tileType = grass 
	end;

	local retval = {};
	setmetatable(retval, Tile);

	--initialize the tile members
	if tileType == "grass" then
		retval.m_isWalkable = true
		retval.m_imageFileName = "images/land1.jpg";
	else
		retval.m_isWalkable = false
		retval.m_imageFileName = "images/rock1.png";
	end

	return retval;
end
