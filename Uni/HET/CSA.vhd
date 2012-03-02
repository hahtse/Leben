----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    11:01:44 05/12/2011 
-- Design Name: 
-- Module Name:    CSA - Behavioral 
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

entity CSA is
    Port ( a : in  STD_LOGIC_VECTOR;
           b : in  STD_LOGIC_VECTOR;
           sum : out  STD_LOGIC_VECTOR;
           cout : out  STD_LOGIC);
begin
--	assert(((A'HIGH-A'LOW) = (b'HIGH-b'LOW))) and ((a'HIGH-a'LOW) = (sum'HIGH-sum'LOW))
--	report "illegal vector lenght for instance CSA"
--	severity FAILURE;
end CSA;

architecture Behavioral of CSA is
	type zwerg is array(0 to (a'HIGH-a'LOW)) of std_logic_vector((a'HIGH-a'LOW) downto 0);
	type t_cchain is array(0 to (a'HIGH-a'LOW)) of std_logic;
	signal zwsum : zwerg;
	signal carry : zwerg;
	signal cchain : t_cchain;
	component GPU
		port ( 	a : in std_logic;
					b : in std_logic;
					P : out std_logic;
					G : out std_logic
		);
	end component;


begin
	-- stage 0
S0gen:	for i in 0 to (a'HIGH-a'LOW)generate
GPU0inst: 	GPU port map(a(i+a'LOW),b(i+b'LOW),zwsum(0)(i),carry(0)(i));
			end generate;
			cchain(0) <= carry(0)(A'HIGH-A'LOW);
CSAGen:   for i in 1 to (a'HIGH-a'LOW) generate
SnGen:		for j in i to (a'HIGH-a'LOW) generate
GPUnInst:		GPU port map(zwsum(i-1)(j),carry(i-1)(j-1),zwsum(i)(j),carry(i)(j));
				end generate;
Prop:			for j in 0 to i-1 generate
						zwsum(i)(j) <= zwsum(i-1)(j);
				end generate;
				cchain(i) <= carry(i)(A'HIGH-A'LOW) or cchain(i-1);
			 end generate;
			 sum <= zwsum(A'HIGH-A'LOW);
			 cout <= cchain(A'HIGH-A'LOW);
end Behavioral;

