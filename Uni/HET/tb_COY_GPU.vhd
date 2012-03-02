
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   10:51:22 04/15/2011
-- Design Name:   COY_GPU
-- Module Name:   C:/Dokumente und Einstellungen/HWET/Eigene Dateien/Xilinx/Fulladd/tb_COY_GPU.vhd
-- Project Name:  Fulladd
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: COY_GPU
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

ENTITY tb_COY_GPU_vhd IS
END tb_COY_GPU_vhd;

ARCHITECTURE behavior OF tb_COY_GPU_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT COY_GPU
	PORT(
		a : IN std_logic;
		b : IN std_logic;
		cmd : IN std_logic_vector(2 downto 0);          
		g : OUT std_logic;
		p : OUT std_logic
		);
	END COMPONENT;

	--Inputs
	SIGNAL a :  std_logic := '0';
	SIGNAL b :  std_logic := '0';
	SIGNAL cmd :  std_logic_vector(2 downto 0) := (others=>'0');

	--Outputs
	SIGNAL g :  std_logic;
	SIGNAL p :  std_logic;

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: COY_GPU PORT MAP(
		a => a,
		b => b,
		cmd => cmd,
		g => g,
		p => p
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

		wait; -- will wait forever
	END PROCESS;

END;
