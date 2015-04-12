library ieee;
use ieee.std_logic_1164.all;

entity plain_top is
  port (
    CLOCK_50    : in    std_logic;
    EPCS_ASDO   : out   std_logic;
    EPCS_CSO_N  : out   std_logic;
    EPCS_DCLK   : out   std_logic;
    EPCS_DATA0  : in    std_logic;
    GPIO        : inout std_logic_vector(27 downto 0);
    RESET_N     : in    std_logic;
    SCI_SCLK    : in    std_logic;
    SCI_TXD     : in    std_logic;
    SCI_TXR_N   : out   std_logic;
    SCI_RXD     : out   std_logic;
    SCI_RXR_N   : in    std_logic;
    SDR_A       : out   std_logic_vector(12 downto 0);
    SDR_BA      : out   std_logic_vector( 1 downto 0);
    SDR_CAS_N   : out   std_logic;
    SDR_CKE     : out   std_logic;
    SDR_CLK     : out   std_logic;
    SDR_CS_N    : out   std_logic;
    SDR_DQ      : inout std_logic_vector(15 downto 0);
    SDR_DQM     : out   std_logic_vector( 1 downto 0);
    SDR_RAS_N   : out   std_logic;
    SDR_WE_N    : out   std_logic;
    START_LED   : out   std_logic
  );
end entity plain_top;

architecture rtl of plain_top is

  ----------------------------------------------------------------
  -- Component definitions
  --
  component plain_pll is
    port (
      areset  : in    std_logic := '0';
      inclk0  : in    std_logic := '0';
      c0      : out   std_logic;
      c1      : out   std_logic;
      c2      : out   std_logic;
      locked  : out   std_logic
    );
  end component plain_pll;

  component plain is
    port (
      clk_core_clk  : in    std_logic                     := 'X';             -- clk
      clk_peri_clk  : in    std_logic                     := 'X';             -- clk
      epcs_dclk     : out   std_logic;                                        -- dclk
      epcs_sce      : out   std_logic;                                        -- sce
      epcs_sdo      : out   std_logic;                                        -- sdo
      epcs_data0    : in    std_logic                     := 'X';             -- data0
      gpio_export   : inout std_logic_vector(27 downto 0) := (others => 'X'); -- export
      led_export    : out   std_logic;                                        -- export
      reset_reset_n : in    std_logic                     := 'X';             -- reset_n
      sci_sclk      : in    std_logic                     := 'X';             -- sclk
      sci_txd       : in    std_logic                     := 'X';             -- txd
      sci_txr_n     : out   std_logic;                                        -- txr_n
      sci_rxd       : out   std_logic;                                        -- rxd
      sci_rxr_n     : in    std_logic                     := 'X';             -- rxr_n
      sdr_addr      : out   std_logic_vector(11 downto 0);                    -- addr
      sdr_ba        : out   std_logic_vector(1 downto 0);                     -- ba
      sdr_cas_n     : out   std_logic;                                        -- cas_n
      sdr_cke       : out   std_logic;                                        -- cke
      sdr_cs_n      : out   std_logic;                                        -- cs_n
      sdr_dq        : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
      sdr_dqm       : out   std_logic_vector(1 downto 0);                     -- dqm
      sdr_ras_n     : out   std_logic;                                        -- ras_n
      sdr_we_n      : out   std_logic                                         -- we_n
    );
  end component plain;

  ----------------------------------------------------------------
  -- Signals
  --
  signal clk_sdr_w  : std_logic;
  signal clk_core_w : std_logic;
  signal clk_peri_w : std_logic;
  signal reset_n_w  : std_logic;

begin -- rtl

  u_pll : component plain_pll
    port map (
      areset  => not RESET_N,
      inclk0  => CLOCK_50,
      c0      => clk_sdr_w,
      c1      => clk_core_w,
      c2      => clk_peri_w,
      locked  => reset_n_w
    );

  u_system : component plain
    port map (
      clk_core_clk  => clk_core_w,
      reset_reset_n => reset_n_w,
      clk_peri_clk  => clk_peri_w,        -- clk_peri.clk

      sdr_addr      => SDR_A(11 downto 0),--      sdr.addr
      sdr_ba        => SDR_BA,            --         .ba
      sdr_cas_n     => SDR_CAS_N,         --         .cas_n
      sdr_cke       => SDR_CKE,           --         .cke
      sdr_cs_n      => SDR_CS_N,          --         .cs_n
      sdr_dq        => SDR_DQ,            --         .dq
      sdr_dqm       => SDR_DQM,           --         .dqm
      sdr_ras_n     => SDR_RAS_N,         --         .ras_n
      sdr_we_n      => SDR_WE_N,          --         .we_n

      epcs_dclk     => EPCS_DCLK,         --     epcs.dclk
      epcs_sce      => EPCS_CSO_N,        --         .sce
      epcs_sdo      => EPCS_ASDO,         --         .sdo
      epcs_data0    => EPCS_DATA0,        --         .data0

      led_export    => START_LED,         --      led.export
      gpio_export   => GPIO(27 downto 0), --     gpio.export

      sci_sclk      => SCI_SCLK,          --      sci.sclk
      sci_txd       => SCI_TXD,           --         .txd
      sci_txr_n     => SCI_TXR_N,         --         .txr_n
      sci_rxd       => SCI_RXD,           --         .rxd
      sci_rxr_n     => SCI_RXR_N          --         .rxr_n
    );

  SDR_A(12) <= '0';
  SDR_CLK <= clk_sdr_w;

end architecture rtl;
