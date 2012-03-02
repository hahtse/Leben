----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:40:17 04/15/2011 
-- Design Name: 
-- Module Name:    COY_GPU - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity COY_GPU is
    Port ( a : in  STD_LOGIC;
           b : in  STD_LOGIC;
           cmd : in  STD_LOGIC_VECTOR (2 downto 0);
           g : out  STD_LOGIC;
           p : out  STD_LOGIC);
end COY_GPU;

architecture Behavioral of COY_GPU is
	signal x,y,z,w,d,f : std_logic; --zwischensignale def.

begin
	x <= cmd(1) and (not b); --and verknüpfungen realisieren
	y <= b and cmd(2); 
	z <= (not b) and (not cmd(0)) and cmd(1) and (not cmd(2));
	w <= b and (not cmd(0)) and cmd(1) and (not cmd(2));
	d <= x or y;
	f <= z or w or a;
	p <= transport d xor f after 5 ns; --gpu realisieren
	g <= transport d and f and cmd(0) after 5 ns;
	
end Behavioral;

