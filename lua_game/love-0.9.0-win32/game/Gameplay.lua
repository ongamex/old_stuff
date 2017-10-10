Gameplay = {};
Gameplay.__index = Gameplay;


local GAME_STATE_PICK_PATH = 1;
local GAME_STATE_WAIT_TO_CONFIRM_PATH = 2;

function sameCoord(a, b)
	return (a.x == b.x) and (a.y == b.y);
end

function copyCoord(a)
	local r = { x = a.x, y = a.y};
	return r;
end

function Gameplay:new(tilemap)
	local retval = {}
	setmetatable(retval, Gameplay)

	retval.m_turnsOrder = {};
	retval.m_currentTurnIdx = 1;
	retval.m_tilemap = tilemap;

	retval.gamestate = GAME_STATE_PICK_PATH;

	retval.isMiceReleased = false;
	retval.miceReleasedPos = {x = 1; y = 1};

	retval.pickedPath = {x = 1; y = 1};
	retval.pathToDisplay = {};

	return retval;
end

function Gameplay:update()
	if self.isMiceReleased then
		if self.gamestate == GAME_STATE_PICK_PATH then
			self:update_pick_path();
		elseif self.gamestate == GAME_STATE_WAIT_TO_CONFIRM_PATH then
			
			self:update_confirm_path();
		end

		self.isMiceReleased = false;
	end
end

function Gameplay:update_pick_path()
	self.pickedPath = copyCoord(self.miceReleasedPos);
	self.gamestate = GAME_STATE_WAIT_TO_CONFIRM_PATH;

	self.pathToDisplay = self.m_tilemap:FindPath(self.m_turnsOrder[self.m_currentTurnIdx].m_position, self.pickedPath);
	
	if self.m_tilemap:GetWarriorAt(self.pickedPath) == nil then
		table.insert(self.pathToDisplay, copyCoord(self.pickedPath));
	end
end

function Gameplay:update_confirm_path()
	if sameCoord(self.pickedPath, self.miceReleasedPos) then

		local currWar = self.m_turnsOrder[self.m_currentTurnIdx];
		
		currWar.m_position = copyCoord( self.pathToDisplay[math.min(currWar.m_speed, #self.pathToDisplay)] );


		self.m_currentTurnIdx = self.m_currentTurnIdx + 1;
		if self.m_currentTurnIdx > #self.m_turnsOrder then
			self.m_currentTurnIdx = 1;
		end
		self.pathToDisplay = {};
	else
		self.gamestate = GAME_STATE_PICK_PATH;
		self:update_pick_path();
	end
			
	self.gamestate = GAME_STATE_PICK_PATH;
end

