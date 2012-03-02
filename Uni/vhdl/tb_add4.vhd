
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   09:36:59 04/15/2011
-- Design Name:   ADD4
-- Module Name:   C:/Dokumente und Einstellungen/HWET/Eigene Dateien/Xilinx/Fulladd/tb_add4.vhd
-- Project Name:  Fulladd
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: ADD4
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
use ieee.std_logic_arith.all;

ENTITY tb_add4_vhd IS
END tb_add4_vhd;

ARCHITECTURE behavior OF tb_add4_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT ADD4
	PORT(
		a : IN std_logic_vector(3 downto 0);
		b : IN std_logic_vector(3 downto 0);
		cin : IN std_logic;          
		s : OUT std_logic_vector(3 downto 0);
		cout : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL cin :  std_logic := '0';
	SIGNAL a :  std_logic_vector(3 downto 0) := (others=>'0');
	SIGNAL b :  std_logic_vector(3 downto 0) := (others=>'0');

	--Outputs
	SIGNAL s :  std_logic_vector(3 downto 0);
	SIGNAL cout :  std_logic;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: ADD4 PORT MAP(
		a => a,
		b => b,
		cin => cin,
		s => s,
		cout => cout
	);

	tb : PROCESS
	BEGIN

		-- Wait 100 ns for global reset to finish
		wait for 100 ns;
			for i in 0 to 1 loop
				for j in 0 to 15 loop
					a <= conv_std_logic_vector(j,4);
					for k in 0 to 15 loop
						b <= conv_std_logic_vector(k,4);
						wait for 10 ns;
					end loop;
				end loop;
			cin <= not cin;
		end loop;
		wait;
				
		-- Place stimulus here

		wait; -- will wait forever
	END PROCESS;

END;
