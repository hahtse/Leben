----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    11:41:13 05/13/2011 
-- Design Name: 
-- Module Name:    DFFp - Behavioral 
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

entity DFFp is
    Port ( D : in  STD_LOGIC_VECTOR ;
           clk : in  STD_LOGIC;
           Q : out  STD_LOGIC_VECTOR );
end DFFp;

architecture Behavioral of DFFp is

begin
dp: process (clk)
-- process erfordert waits. in klammern steht die sensivitylist -> clk

		variable Qint : std_logic_vector(D'High downto D'Low);

	begin
		if(rising_edge(clk)) then -- nach ' stehen attribute bsp event low high 
		-- eine zweite möglichkeit : clk'event and (clk = 1)
			
				Qint := D;
		end if;
		Q <= Qint;
		end process;
end Behavioral;

