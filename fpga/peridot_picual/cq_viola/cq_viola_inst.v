	cq_viola u0 (
		.adc_MISO      (<connected-to-adc_MISO>),      //      adc.MISO
		.adc_MOSI      (<connected-to-adc_MOSI>),      //         .MOSI
		.adc_SCLK      (<connected-to-adc_SCLK>),      //         .SCLK
		.adc_SS_n      (<connected-to-adc_SS_n>),      //         .SS_n
		.clk_core_clk  (<connected-to-clk_core_clk>),  // clk_core.clk
		.clk_peri_clk  (<connected-to-clk_peri_clk>),  // clk_peri.clk
		.lcdc_rst_n    (<connected-to-lcdc_rst_n>),    //     lcdc.rst_n
		.lcdc_cs_n     (<connected-to-lcdc_cs_n>),     //         .cs_n
		.lcdc_rs       (<connected-to-lcdc_rs>),       //         .rs
		.lcdc_wr_n     (<connected-to-lcdc_wr_n>),     //         .wr_n
		.lcdc_d        (<connected-to-lcdc_d>),        //         .d
		.led_export    (<connected-to-led_export>),    //      led.export
		.mmc_nCS       (<connected-to-mmc_nCS>),       //      mmc.nCS
		.mmc_SCK       (<connected-to-mmc_SCK>),       //         .SCK
		.mmc_SDO       (<connected-to-mmc_SDO>),       //         .SDO
		.mmc_SDI       (<connected-to-mmc_SDI>),       //         .SDI
		.mmc_CD        (<connected-to-mmc_CD>),        //         .CD
		.mmc_WP        (<connected-to-mmc_WP>),        //         .WP
		.reset_reset_n (<connected-to-reset_reset_n>), //    reset.reset_n
		.sdr_addr      (<connected-to-sdr_addr>),      //      sdr.addr
		.sdr_ba        (<connected-to-sdr_ba>),        //         .ba
		.sdr_cas_n     (<connected-to-sdr_cas_n>),     //         .cas_n
		.sdr_cke       (<connected-to-sdr_cke>),       //         .cke
		.sdr_cs_n      (<connected-to-sdr_cs_n>),      //         .cs_n
		.sdr_dq        (<connected-to-sdr_dq>),        //         .dq
		.sdr_dqm       (<connected-to-sdr_dqm>),       //         .dqm
		.sdr_ras_n     (<connected-to-sdr_ras_n>),     //         .ras_n
		.sdr_we_n      (<connected-to-sdr_we_n>),      //         .we_n
		.sound_export  (<connected-to-sound_export>)   //    sound.export
	);

