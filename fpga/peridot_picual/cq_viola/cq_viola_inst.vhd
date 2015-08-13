	component cq_viola is
		port (
			adc_MISO      : in    std_logic                     := 'X';             -- MISO
			adc_MOSI      : out   std_logic;                                        -- MOSI
			adc_SCLK      : out   std_logic;                                        -- SCLK
			adc_SS_n      : out   std_logic;                                        -- SS_n
			clk_core_clk  : in    std_logic                     := 'X';             -- clk
			clk_peri_clk  : in    std_logic                     := 'X';             -- clk
			lcdc_rst_n    : out   std_logic;                                        -- rst_n
			lcdc_cs_n     : out   std_logic;                                        -- cs_n
			lcdc_rs       : out   std_logic;                                        -- rs
			lcdc_wr_n     : out   std_logic;                                        -- wr_n
			lcdc_d        : inout std_logic_vector(7 downto 0)  := (others => 'X'); -- d
			led_export    : out   std_logic;                                        -- export
			mmc_nCS       : out   std_logic;                                        -- nCS
			mmc_SCK       : out   std_logic;                                        -- SCK
			mmc_SDO       : out   std_logic;                                        -- SDO
			mmc_SDI       : in    std_logic                     := 'X';             -- SDI
			mmc_CD        : in    std_logic                     := 'X';             -- CD
			mmc_WP        : in    std_logic                     := 'X';             -- WP
			reset_reset_n : in    std_logic                     := 'X';             -- reset_n
			sdr_addr      : out   std_logic_vector(11 downto 0);                    -- addr
			sdr_ba        : out   std_logic_vector(1 downto 0);                     -- ba
			sdr_cas_n     : out   std_logic;                                        -- cas_n
			sdr_cke       : out   std_logic;                                        -- cke
			sdr_cs_n      : out   std_logic;                                        -- cs_n
			sdr_dq        : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdr_dqm       : out   std_logic_vector(1 downto 0);                     -- dqm
			sdr_ras_n     : out   std_logic;                                        -- ras_n
			sdr_we_n      : out   std_logic;                                        -- we_n
			sound_export  : out   std_logic                                         -- export
		);
	end component cq_viola;

	u0 : component cq_viola
		port map (
			adc_MISO      => CONNECTED_TO_adc_MISO,      --      adc.MISO
			adc_MOSI      => CONNECTED_TO_adc_MOSI,      --         .MOSI
			adc_SCLK      => CONNECTED_TO_adc_SCLK,      --         .SCLK
			adc_SS_n      => CONNECTED_TO_adc_SS_n,      --         .SS_n
			clk_core_clk  => CONNECTED_TO_clk_core_clk,  -- clk_core.clk
			clk_peri_clk  => CONNECTED_TO_clk_peri_clk,  -- clk_peri.clk
			lcdc_rst_n    => CONNECTED_TO_lcdc_rst_n,    --     lcdc.rst_n
			lcdc_cs_n     => CONNECTED_TO_lcdc_cs_n,     --         .cs_n
			lcdc_rs       => CONNECTED_TO_lcdc_rs,       --         .rs
			lcdc_wr_n     => CONNECTED_TO_lcdc_wr_n,     --         .wr_n
			lcdc_d        => CONNECTED_TO_lcdc_d,        --         .d
			led_export    => CONNECTED_TO_led_export,    --      led.export
			mmc_nCS       => CONNECTED_TO_mmc_nCS,       --      mmc.nCS
			mmc_SCK       => CONNECTED_TO_mmc_SCK,       --         .SCK
			mmc_SDO       => CONNECTED_TO_mmc_SDO,       --         .SDO
			mmc_SDI       => CONNECTED_TO_mmc_SDI,       --         .SDI
			mmc_CD        => CONNECTED_TO_mmc_CD,        --         .CD
			mmc_WP        => CONNECTED_TO_mmc_WP,        --         .WP
			reset_reset_n => CONNECTED_TO_reset_reset_n, --    reset.reset_n
			sdr_addr      => CONNECTED_TO_sdr_addr,      --      sdr.addr
			sdr_ba        => CONNECTED_TO_sdr_ba,        --         .ba
			sdr_cas_n     => CONNECTED_TO_sdr_cas_n,     --         .cas_n
			sdr_cke       => CONNECTED_TO_sdr_cke,       --         .cke
			sdr_cs_n      => CONNECTED_TO_sdr_cs_n,      --         .cs_n
			sdr_dq        => CONNECTED_TO_sdr_dq,        --         .dq
			sdr_dqm       => CONNECTED_TO_sdr_dqm,       --         .dqm
			sdr_ras_n     => CONNECTED_TO_sdr_ras_n,     --         .ras_n
			sdr_we_n      => CONNECTED_TO_sdr_we_n,      --         .we_n
			sound_export  => CONNECTED_TO_sound_export   --    sound.export
		);

