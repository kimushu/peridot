--
-- Avalon-MM IRQ sender
--
library ieee;
use ieee.std_logic_1164.all;

entity avm_irq_sender is
  generic (
    IRQ_WIDTH : integer := 1
  );
  port (
    clk       : in  std_logic;
    reset_n   : in  std_logic;

    address   : in  std_logic_vector(0 downto 0);
    write_n   : in  std_logic;
    writedata : in  std_logic_vector(31 downto 0);
    read_n    : in  std_logic;
    readdata  : out std_logic_vector(31 downto 0) := (others => '0');

    irq       : out std_logic_vector(IRQ_WIDTH-1 downto 0)
  );
end entity avm_irq_sender;

architecture rtl of avm_irq_sender is

  signal irq_r    : std_logic_vector(IRQ_WIDTH-1 downto 0);

begin -- rtl

  readdata(IRQ_WIDTH-1 downto 0) <= irq_r;
  irq <= irq_r;

  process (clk) begin
    if (rising_edge(clk)) then
      if (reset_n = '0') then
        irq_r <= (others => '0');
      elsif (write_n = '0') then
        if (address(0) = '0') then
          irq_r <= irq_r or writedata(IRQ_WIDTH-1 downto 0);
        else
          irq_r <= irq_r and (not writedata(IRQ_WIDTH-1 downto 0));
        end if;
      end if;
    end if;
  end process;

end architecture rtl;
