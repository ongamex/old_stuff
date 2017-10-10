--[[]
public methods : 
	Draw

private methods :
	LoadImages

members :

	m_tiles
	m_size {x, y}
	m_loadedImages
]]

require("Tile");
require("Warrior")

Tilemap = {};
Tilemap.__index = Tilemap;

----------------------------------------------------------------
function Tilemap:new(tiles)
	local retval = {}
	setmetatable(retval, Tilemap)

	retval.m_loadedImages = {}
	retval.m_size = {x = #tiles[1], y = #tiles};
	retval.m_tiles = {};
	retval.m_warriors = {};

	local x, y;
	
	for y = 1, retval.m_size.y do
		retval.m_tiles[y] = {};
		for x = 1, retval.m_size.x do
			retval.m_tiles[y][x] = Tile:new(tiles[y][x]);
		end
	end

	return retval;
end

----------------------------------------------------------------
function Tilemap:LoadImages()
	self.m_loadedImages["images/land1.jpg"] = love.graphics.newImage("images/land1.jpg");
	self.m_loadedImages["images/rock1.png"] = love.graphics.newImage("images/rock1.png");
	self.m_loadedImages["images/war1.png"] = love.graphics.newImage("images/war1.png");
end

----------------------------------------------------------------
function Tilemap:Draw()
	local x, y;
	for x = 1, self.m_size.x do
		for y = 1, self.m_size.y do
			local tileFileName = self.m_tiles[y][x].m_imageFileName;
			love.graphics.draw(self.m_loadedImages[tileFileName], (x - 1)*64, (y - 1)*64);
		end
	end

	local t;
	for t = 1, #self.m_warriors do
		if self.m_warriors[t] ~= nil then
			local pos = self.m_warriors[t].m_position;
			local filename = self.m_warriors[t].m_imageFileName;
			local scale = 1;
			if self.m_warriors[t].m_armyid == 1 then scale = -1; end
			love.graphics.draw( self.m_loadedImages[filename], (pos.x - 1)*64 + 32, (pos.y - 1)*64 + 32,
			 0, scale, 1, 32, 32, 0, 0);
		end
	end
end

----------------------------------------------------------------
function Tilemap:BuildPathmap()
	
	local pathmap = {};
	local x, y;
	for y = 1, self.m_size.y do
		pathmap[y] = {};
		for x = 1, self.m_size.x do
			pathmap[y][x] = {}
			pathmap[y][x].isWalkable = self.m_tiles[y][x].m_isWalkable and (self:GetWarriorAt({x = x, y = y}) == nil);
			pathmap[y][x].distanceStart = 10000;
			pathmap[y][x].pathProcessed = false or (not pathmap[y][x].isWalkable);
		end
	end

	return pathmap;
end

----------------------------------------------------------------

function getNeigthbours(map, point)
	local sy = #map;
	local sx = #map[1];
	local retval = {}

	if (point.x - 1) >= 1 then 
		table.insert(retval, {x = point.x - 1, y = point.y});
	end

	if (point.y - 1) >= 1 then 
		table.insert(retval, {x = point.x, y = point.y - 1});
	end

	if (point.x + 1) <= sx then 

		table.insert(retval, {x = point.x + 1, y = point.y});
	end

	if (point.y + 1) <= sy then 
		table.insert(retval, {x = point.x, y = point.y + 1});
	end

	return retval;
end

function Tilemap:FindPath(start, finish)

	local pathmap = self:BuildPathmap();
	pathmap[start.y][start.x].distanceStart = 0;

	local function BuildTileWeigths(pathmap, point)

		local neigthbours = getNeigthbours(pathmap, point);

		local currDist = pathmap[point.y][point.x].distanceStart;
		pathmap[point.y][point.x].pathProcessed = true;

		local pathsToProcess = {}

		local t;
		for t = 1, #neigthbours do
			local pt = neigthbours[t];

			if(pathmap[pt.y][pt.x].isWalkable) then
				local reproc = pathmap[pt.y][pt.x].distanceStart < currDist+1;
				pathmap[pt.y][pt.x].distanceStart = math.min(pathmap[pt.y][pt.x].distanceStart, currDist+1);

				if not pathmap[pt.y][pt.x].pathProcessed or reproc then
					table.insert(pathsToProcess, {x = pt.x, y = pt.y});
				end
			end
		end

		for t = 1, #pathsToProcess do
			BuildTileWeigths(pathmap, pathsToProcess[t]);
		end
	end

	BuildTileWeigths(pathmap, start);

	local function iFindPath(pathmap, start, finish)
		local retval = {};
		local bLoop = true;

		local ptCurrent = finish;

		while bLoop do
			local neigthbours = getNeigthbours(pathmap, ptCurrent);

			local t;
			local minNIdx;
			local minVal = 100000;


			for t = 1, #neigthbours do
				local pt = neigthbours[t];
				if pathmap[pt.y][pt.x].distanceStart < minVal then
					minNIdx = t;
					minVal = pathmap[pt.y][pt.x].distanceStart;
					ptCurrent = pt;
				end
			end

			if (ptCurrent.x == start.x) and
			   (ptCurrent.y == start.y) then
			   	bLoop = false;
			else
				table.insert(retval, {x = ptCurrent.x, y = ptCurrent.y});
			end

			if #retval > 100 then 
				bLoop = false;
			end

		end
		
		return retval;
	end

	return iFindPath(pathmap, start, finish);
end

------------------------------------------------------------------
function Tilemap:AddWarrior(war)
	table.insert(self.m_warriors, war);
end

------------------------------------------------------------------
function Tilemap:GetWarriorAt(coord)
	local t;
	for t = 1, #self.m_warriors do
		if self.m_warriors[t] ~= nil then 

			if (self.m_warriors[t].m_position.x == coord.x) and
				(self.m_warriors[t].m_position.y == coord.y) then
				return self.m_warriors[t];
			end
		end
	end
	return nil;
end