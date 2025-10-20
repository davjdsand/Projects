library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity SevenSegmentDecoder is
    port(
        binary_input : in INTEGER range 0 to 12;
        segment_output : out STD_LOGIC_VECTOR(7 downto 0)
    );
end SevenSegmentDecoder;

architecture Behavioral of SevenSegmentDecoder is
begin
    process(binary_input)
    begin
        case binary_input is
            when 0 => segment_output <= "11000000";  -- Display 0
            when 1 => segment_output <= "11111001";  -- Display 1
            when 2 => segment_output <= "10100100";  -- Display 2
            when 3 => segment_output <= "10110000";  -- Display 3
            when 4 => segment_output <= "10011001";  -- Display 4
            when 5 => segment_output <= "10010010";  -- Display 5
            when 6 => segment_output <= "10000010";  -- Display 6
            when 7 => segment_output <= "11111000";  -- Display 7
            when 8 => segment_output <= "10000000";  -- Display 8
            when 9 => segment_output <= "10010000";  -- Display 9
            when 10 => segment_output <= "10001000"; -- Display A (for 10th floor)
            when 11 => segment_output <= "10000011"; -- Display b (for 11th floor)
            when 12 => segment_output <= "11000110"; -- Display C (for 12th floor)
            when others => segment_output <= "11111111"; -- Blank display
        end case;
    end process;
end Behavioral;