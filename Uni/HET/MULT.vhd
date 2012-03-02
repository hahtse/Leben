----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:03:11 05/06/2011 
-- Design Name: 
-- Module Name:    MULT - Behavioral 
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

entity MULT is
    Port ( xIn : in  STD_LOGIC_VECTOR (3 downto 0);
           yIn : in  STD_LOGIC_VECTOR (3 downto 0);
			  clk : in STD_LOGIC;
           zOut : out  STD_LOGIC_VECTOR (7 downto 0));
end MULT;

architecture Behavioral of MULT is
	component GPU
		Port ( a : in  STD_LOGIC;
				b : in  STD_LOGIC;
				p : out  STD_LOGIC;
				g : out  STD_LOGIC);
	end Component;
	signal p,g,c : std_logic_vector(3 downto 0);
	
	component VA
		    Port ( a : in  STD_LOGIC;
           b : in  STD_LOGIC;
           cin : in  STD_LOGIC;
           s : out  STD_LOGIC;
           cout : out  STD_LOGIC);
	end Component;
	
	Component CSA
    Port ( a : in  STD_LOGIC_VECTOR;
           b : in  STD_LOGIC_VECTOR;
           sum : out  STD_LOGIC_VECTOR;
           cout : out  STD_LOGIC);
	end Component;
	
	Component DFFp is
    Port ( D : in  STD_LOGIC_VECTOR ;
           clk : in  STD_LOGIC;
           Q : out  STD_LOGIC_VECTOR );
	end Component;
	
	type t_sum is array (0 to 3) of std_logic_vector(3 downto 0);
	type t_und is array (1 to 3) of std_logic_vector(3 downto 0);
	type t_carry is array (1 to 3) of std_logic_vector(2 downto 0);
	
	Signal sum : t_sum;
	signal und : t_und;
	signal carry : t_carry;
	signal carry2 : std_logic_vector(2 downto 0);
	signal sum2 : std_logic_vector(3 downto 1);
	signal x , y, z : std_logic_vector (3 downto 0);
	
begin

DFFpxInst : DFFp port map ( xIn , clk , x);

DFFpyInst : DFFp port map ( yIn , clk , y);

--stufe 0
sum0gen: for i in 0 to 3 generate --generate läuft gleichzeitig ab. hardwarenah. loop ist sequenuiell
				sum(0)(i) <= x(i) and y(0);
			end generate;
			z(0) <= sum(0)(0);
--stufe 1	
und1gen:for i in 0 to 3 generate
				und(1)(i) <= x(i) and y(1);
			end generate;
sum1gen: for i in 0 to 2 generate
GPU1inst:   GPU port map(sum(0)(i+1),und(1)(i),sum(1)(i),carry(1)(i));
			end generate;
			sum(1)(3) <= und(1)(3);
			z(1) <= sum(1)(0);
--Stufe 2 bis n
stageNgen: for k in 2 to 3 generate
undNgen:for i in 0 to 3 generate
				und(k)(i) <= x(i) and y(k);
			end generate;
sumNgen: for i in 0 to 2 generate
VANinst:   VA port map(sum(k-1)(i+1),und(k)(i),carry(k-1)(i),sum(k)(i),carry(k)(i));
			end generate;
			sum(k)(3) <= und(k)(3);
			z(k) <= sum(k)(0);
			end generate;

DFFpcInst : DFFp port map ( carry(3) , clk , carry2);
DFFpsInst : DFFp port map ( sum(3)(3 downto 1) , clk , sum2);
DFFpzInst : DFFp port map ( z , clk , zOut(3 downto 0));

CSAinst : CSA port map(sum2,carry2,zOut(6 downto 4),zOut(7));

end Behavioral;

