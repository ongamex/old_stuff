require("Tilemap");
require("Warrior");

local g_pointSprite;
local g_crossSprite;
local g_startSprite;

local startPt = {x = 1, y = 1};
local endPt = { x = 12, y = 3};
local g_recomputePath = true;
local g_path;

local turnOrder = {}

local g_Tilemap = Tilemap:new(
{
	{"grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass"},
	{"grass", "grass", "grass", "grass", "grass", "rock", "grass", "rock", "rock", "rock", "grass", "grass"},
	{"grass", "grass", "grass", "grass", "grass", "rock", "rock", "rock", "grass", "rock", "grass", "grass"},
	{"grass", "grass", "grass", "grass", "grass", "grass", "grass", "rock", "grass", "rock", "grass", "grass"},
	{"grass", "grass", "grass", "rock", "rock", "grass", "rock", "rock", "grass", "grass", "grass", "grass"},
	{"grass", "grass", "grass", "grass", "rock", "grass", "grass", "grass", "grass", "grass", "grass", "grass"},
	{"grass", "grass", "grass", "grass", "rock", "grass", "grass", "rock", "grass", "grass", "grass", "grass"},
	{"grass", "grass", "grass", "grass", "rock", "rock", "rock", "rock", "rock", "grass", "grass", "grass"},
	{"grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass", "grass"},
});

----------------------------------------------------------------------------
--global resources


function DebugOut(str)
	if g_debugOut == nil then g_debugOut = "" end
	g_debugOut = g_debugOut .. str.."\n";
end

---------------------------------------------------------------------------
function love.keyreleased(key)
   if key == "escape" then
      love.event.quit()
   end
end

function love.mousereleased(x, y, button)
	local hit_x = math.floor(x / 64) + 1;
	local hit_y = math.floor(y / 64) + 1;
	g_recomputePath = true
   if button == 'l' then
   	if(love.keyboard.isDown("q")) then
   		g_Tilemap.m_tiles[hit_y][hit_x] = Tile:new("grass");
   	else
   		startPt = { x = hit_x, y = hit_y}
   		
   	end
   else
   	if(love.keyboard.isDown("q")) then
   		g_Tilemap.m_tiles[hit_y][hit_x] = Tile:new("rock");
   	else
   		endPt = { x = hit_x, y = hit_y}
   		
   end
end

end

---------------------------------------------------------------------------
function love.draw()

    g_Tilemap:Draw();
    
    love.graphics.draw(g_startSprite, (startPt.x - 1)*64, (startPt.y - 1)*64);
    love.graphics.draw(g_crossSprite, (endPt.x - 1)*64, (endPt.y - 1)*64);

    local t;
    for t = 1, #g_path do
    	local x = g_path[t].x;
    	local y = g_path[t].y;

    	love.graphics.draw(g_pointSprite, (x - 1)*64, (y - 1)*64);
    end

end

---------------------------------------------------------------------------
function love.update(dt)
	if g_recomputePath then
  	  g_path = g_Tilemap:FindPath(startPt, endPt);
  	  g_recomputePath = false;
	end
end

love.conf = function(t)
t.vsync = false
end

---------------------------------------------------------------------------
function love.load()
	g_Tilemap:LoadImages();
	g_pointSprite = love.graphics.newImage("images/point.png");
	g_crossSprite = love.graphics.newImage("images/cross.png");
	g_startSprite = love.graphics.newImage("images/start.png");

	do
		local war1 = Warroir:new(0, 5, 20, 5, {x = 1, y = 1}, "images/war1.png"); 
		g_Tilemap:AddWarrior(war1);
		
	end

	do
		local war1 = Warroir:new(1, 5, 20, 5, {x = 12, y = 1}, "images/war1.png"); 
		g_Tilemap:AddWarrior(war1);
		table.insert(turnOrder, war1);
	end


	love.graphics.setPointSize(32);
	table.insert(turnOrder, war1);

end




function main()
	DebugOut("init\n");
	love.window.setMode(800, 600);



end

main();
