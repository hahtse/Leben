----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    16:10:20 05/26/2011 
-- Design Name: 
-- Module Name:    FSM - Behavioral 
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
use work.modules.all;

entity FSM is
	Port ( 	opcode : in STD_LOGIC_VECTOR(7 downto 0);
				Steuervektor : out STD_LOGIC_VECTOR(7 downto 0);
				clk : in STD_LOGIC;
				re: out STD_LOGIC);
end FSM;

architecture Behavioral of FSM is

	type states is (
		inst_fetch,
		decode,
		arith,
		arith_c,
		load,
		store,
		undefined
	);
	signal inst_load : std_logic;
	signal befehl : std_logic_vector(7 downto 0);
	signal state : states;
	signal next_state : states;

begin
BefReg: DFFmiten port map (opcode, inst_load, clk, befehl);
DecLogic:
	process(state,befehl) is
		begin
			case state is
				when inst_fetch =>
					inst_load <= '1';
					next_state <=decode;
				when decode =>
					inst_load <= '0';
					case befehl(7 downto 5) is
						when('0','1','-') =>
							next_state <= arith;
						when ('1','1','-') =>
							next_state <= arith_c;
						when ('1','0','1') =>
							next_state <= load;
						when ('1','0','0') =>
							next_state <= store;
						when others =>
							next_state <= undefined;
					end case;
				when arith =>
					next_state <= inst_fetch;
				when arith_c =>
					next_state <= inst_fetch;
				when load =>
					next_state <= inst_fetch;
				when store =>
					next_state <= inst_fetch;
				when others =>
					next_state <= undefined;
			end case;
		end process;
		
	state_proc:
		process(clk) is
			variable act_state : states := inst_fetch;
		begin
			if (falling_edge(clk)) then
				act_state := next_state;
			end if;
			state <= act_state;
		end process;

end Behavioral;

