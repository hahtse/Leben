
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   10:24:06 04/08/2011
-- Design Name:   FA
-- Module Name:   C:/Dokumente und Einstellungen/HWET/Eigene Dateien/Xilinx/Fulladd/tb_FA.vhd
-- Project Name:  Fulladd
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: FA
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

ENTITY tb_FA_vhd IS
END tb_FA_vhd;

ARCHITECTURE behavior OF tb_FA_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT FA
	PORT(
		a : IN std_logic;
		b : IN std_logic;
		cin : IN std_logic;          
		s : OUT std_logic;
		cout : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL a :  std_logic := '0';
	SIGNAL b :  std_logic := '0';
	SIGNAL cin :  std_logic := '0';

	--Outputs
	SIGNAL s :  std_logic;
	SIGNAL cout :  std_logic;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: FA PORT MAP(
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

		-- Place stimulus here
	for i in 0 to 1 loop
		for j in 0 to 1 loop
			for k in 0 to 1 loop
				wait for 10 ns;
				a <= not a;
			end loop;
			b <= not b;
		end loop;
		cin <= not cin;
	end loop;
		wait; -- will wait forever
	END PROCESS;

END;