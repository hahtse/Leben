
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   12:08:22 04/29/2011
-- Design Name:   CoyALU
-- Module Name:   C:/Dokumente und Einstellungen/HWET/Eigene Dateien/Xilinx/CoyALU/tb_COY_ALU.vhd
-- Project Name:  CoyALU
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: CoyALU
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
USE ieee.std_logic_arith;

ENTITY tb_COY_ALU_vhd IS
END tb_COY_ALU_vhd;

ARCHITECTURE behavior OF tb_COY_ALU_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT CoyALU
	PORT(
		a : IN std_logic_vector(3 downto 0);
		b : IN std_logic_vector(3 downto 0);
		cin : IN std_logic;
		cmd : IN std_logic_vector(2 downto 0);          
		Summe : OUT std_logic_vector(3 downto 0);
		cout : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL cin :  std_logic := '0';
	SIGNAL a :  std_logic_vector(3 downto 0) := (others=>'0');
	SIGNAL b :  std_logic_vector(3 downto 0) := (others=>'0');
	SIGNAL cmd :  std_logic_vector(2 downto 0) := (others=>'0');

	--Outputs
	SIGNAL Summe :  std_logic_vector(3 downto 0);
	SIGNAL cout :  std_logic;
	
	--Signal
	
	Signal s : std_logic_vector(4 downto 0);

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: CoyALU PORT MAP(
		a => a,
		b => b,
		cin => cin,
		Summe => Summe,
		cout => cout,
		cmd => cmd
	);

	tb : PROCESS
	BEGIN



		-- Wait 100 ns for global reset to finish
		wait for 100 ns;

		-- Place stimulus here
		for i in 0 to 1 loop
			for j in 0 to 1 loop
				wait for 100 ns;
				 s <= cout & summe;
				--artih
				assert (s = conv_std_logic_vector(i+j,5));
				report "Fehler";

				--logic
				
				assert (summe = conv_std_logic_vector(i) and conv_std_logic_vector(j));
			end loop;
		end loop;		
		wait; -- will wait forever
	END PROCESS;

END;
