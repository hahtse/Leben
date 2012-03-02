----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:44:57 05/19/2011 
-- Design Name: 
-- Module Name:    DeMUX - Behavioral 
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity DeMUX is
	port(
		sel	: in	std_logic_vector(2 downto 0);
		gate	: in	std_logic;
		outp	: out std_logic_vector(7 downto 0)
		);
end DeMUX;

architecture Behavioral of DeMUX is
begin
pdemux: process(sel,gate)

begin
	case sel is
		when('0','0','0')=>outp<=('0','0','0','0','0','0','0',gate);
		when('0','0','1')=>outp<=('0','0','0','0','0','0',gate,'0');
		when('0','1','0')=>outp<=('0','0','0','0','0',gate,'0','0');
		when('0','1','1')=>outp<=('0','0','0','0',gate,'0','0','0');
		when('1','0','0')=>outp<=('0','0','0',gate,'0','0','0','0');
		when('1','0','1')=>outp<=('0','0',gate,'0','0','0','0','0');
		when('1','1','0')=>outp<=('0',gate,'0','0','0','0','0','0');
		when('1','1','1')=>outp<=(gate,'0','0','0','0','0','0','0');
		when others => outp <= ('U','U','U','U','U','U','U','U');
	end case;
end process;

end Behavioral;

