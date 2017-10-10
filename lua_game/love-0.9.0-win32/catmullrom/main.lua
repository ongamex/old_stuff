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

end

function catmullrom(x, v0, v1, v2, v3)

	local M11 =	 0.0
	local M12 =	 1.0
	local M13 =	 0.0
	local M14 =	 0.0
	local M21 =	-0.5
	local M22 =	 0.0
	local M23 =	 0.5
	local M24 =	 0.0
	local M31 =	 1.0
	local M32 =	-2.5
	local M33 =	 2.0
	local M34 =	-0.5
	local M41 =	-0.5
	local M42 =	 1.5
	local M43 =	-1.5
	local M44 =	 0.5

	local c1,c2,c3,c4;

	c1 =          M12*v1;
	c2 = M21*v0          + M23*v2;
	c3 = M31*v0 + M32*v1 + M33*v2 + M34*v3;
	c4 = M41*v0 + M42*v1 + M43*v2 + M44*v3;

	return(((c4*x + c3)*x +c2)*x + c1);
end


---------------------------------------------------------------------------
g_xPos = 0;
function love.draw()
	love.graphics.print(g_debugOut, 0, 0)

	love.graphics.circle("fill", g_xPos, 300, 5, 8)
end

---------------------------------------------------------------------------
g_dt = 0;
function love.update(dt)
	g_dt = g_dt + dt;

	g_xPos = catmullrom(g_dt, 0, 100, 700, 800);

	if g_dt >= 1 then 
		g_dt = 0 
	end

end

---------------------------------------------------------------------------
function love.load()

end




function main()
	DebugOut("init\n");
	love.window.setMode(800, 600);
end

main();
