--[[

members : 

	m_armyid
	m_attack
	m_health
	m_speed
	m_position (x,y)
	m_imageFileName
]]

Warroir = {};
Warroir.__index = Warroir;

------------------------------------------------------------
function Warroir:new(armyid, attack, health, speed, position, image)
	local retval = {}
	setmetatable(retval, Warroir)

	retval.m_armyid = armyid;
	retval.m_health = health;
	retval.m_speed = speed;
	retval.m_imageFileName = image;
	retval.m_position = position;

	return retval
end

------------------------------------------------------------
function Warroir:IsEnemy(other)
	return other.m_armyid ~= self.m_armyid;
end

------------------------------------------------------------
function Warroir:IsDead()
	return self.m_health <= 0;
end

