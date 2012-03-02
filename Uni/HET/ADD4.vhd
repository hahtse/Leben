----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:46:40 04/08/2011 
-- Design Name: 
-- Module Name:    ADD4 - Behavioral 
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

entity ADD4 is
    Port ( a : in  STD_LOGIC_VECTOR (3 downto 0);
           b : in  STD_LOGIC_VECTOR (3 downto 0);
           cin : in  STD_LOGIC;
           s : out  STD_LOGIC_VECTOR (3 downto 0);
           cout : out  STD_LOGIC);
end ADD4;

architecture Behavioral of ADD4 is
	component GPU
		Port ( a : in  STD_LOGIC;
				b : in  STD_LOGIC;
				p : out  STD_LOGIC;
				g : out  STD_LOGIC);
	end Component;
	signal p,g,c : std_logic_vector(3 downto 0);
begin
GPUgen:	for i in 0 to 3 generate
GPUInst: GPU port map(a(i),b(i),p(i),g(i));
	end generate;
	c(0) <= cin;
carrygen: for i in 1 to 3 generate
	c(i) <= (p(i-1) and c(i-1)) or g(i-1);
	end generate;
	cout <= (p(3) and c(3))or g(3);
sumgen: for i in 0 to 3 generate
	s(i) <= p(i) xor c(i);
	end generate;

end Behavioral;

