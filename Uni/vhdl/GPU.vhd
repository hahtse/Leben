----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:44:26 04/08/2011 
-- Design Name: 
-- Module Name:    GPU - Behavioral 
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

entity GPU is
    Port ( a : in  STD_LOGIC;
           b : in  STD_LOGIC;
           p : out  STD_LOGIC;
           g : out  STD_LOGIC);
end GPU;

architecture Behavioral of GPU is

begin
	p <= a xor b;
	g <= a and b;

end Behavioral;

