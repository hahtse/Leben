----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:03:22 05/19/2011 
-- Design Name: 
-- Module Name:    MUX - Behavioral 
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
library work;
use work.modules.all;

entity MUXn is
 generic (
			n : positive -- ld von Muxbreite
			);
    Port ( chan : in t_muxN( 0 to 7 );--(2 ** n-1)
			  sel : in STD_LOGIC_VECTOR (2 downto 0 );
           y : out STD_LOGIC_VECTOR(7 downto 0));
end MUXn;

architecture Behavioral of MUXn is

--SIGNAL
--		signal reg_out : t_muxN (0 to 7 ) of std_logic_vector ( 7 downto 0);
--		signal opout : std_logic_vector ( 7 downto 0 );
--		signal op : std_logic_vector (2 downto 0 );

begin
	y <= chan(conv_integer(sel));

end Behavioral;

