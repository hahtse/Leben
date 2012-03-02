
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   10:56:27 05/13/2011
-- Design Name:   MULT
-- Module Name:   C:/Dokumente und Einstellungen/HWET/Eigene Dateien/Xilinx/MULT/tb_mult.vhd
-- Project Name:  MULT
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: MULT
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends 
-- that these types always be used for the top-level I/O of a design in order 
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;
use ieee.std_logic_arith.ALL;

ENTITY tb_mult_vhd IS
END tb_mult_vhd;

ARCHITECTURE behavior OF tb_mult_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT MULT
Port ( xIn : in  STD_LOGIC_VECTOR (3 downto 0);
       yIn : in  STD_LOGIC_VECTOR (3 downto 0);
		 clk : in STD_LOGIC;
       zOut : out  STD_LOGIC_VECTOR (7 downto 0));
	END COMPONENT;

	--Inputs
	SIGNAL xIn :  std_logic_vector(3 downto 0) := (others=>'0');
	SIGNAL yIn :  std_logic_vector(3 downto 0) := (others=>'0');
	SIGNAL clk :  std_logic := '0';
	--Outputs
	SIGNAL zOut :  std_logic_vector(7 downto 0);
	
BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: MULT PORT MAP(
		xIn => xIn,
		yIn => yIn,
		clk => clk,
		zOut => zOut
	);

clkp : process
	begin
	
		loop
			wait for 20 ns;
			clk <= not clk;
		end loop;
	end process;

	tb : PROCESS
	BEGIN

		-- Wait 100 ns for global reset to finish
		wait for 100 ns;

		-- Place stimulus here
		for i in 0 to 15 loop
			xIn <= conv_std_logic_vector(i,4);
			for j in 0 to 15 loop 
				yIn <= conv_std_logic_vector(j,4);
				wait until (falling_edge(clk));
			end loop;
		end loop;
		wait; -- will wait forever
	END PROCESS;

END;
