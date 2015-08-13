
module cq_viola (
	adc_MISO,
	adc_MOSI,
	adc_SCLK,
	adc_SS_n,
	clk_core_clk,
	clk_peri_clk,
	lcdc_rst_n,
	lcdc_cs_n,
	lcdc_rs,
	lcdc_wr_n,
	lcdc_d,
	led_export,
	mmc_nCS,
	mmc_SCK,
	mmc_SDO,
	mmc_SDI,
	mmc_CD,
	mmc_WP,
	reset_reset_n,
	sdr_addr,
	sdr_ba,
	sdr_cas_n,
	sdr_cke,
	sdr_cs_n,
	sdr_dq,
	sdr_dqm,
	sdr_ras_n,
	sdr_we_n,
	sound_export);	

	input		adc_MISO;
	output		adc_MOSI;
	output		adc_SCLK;
	output		adc_SS_n;
	input		clk_core_clk;
	input		clk_peri_clk;
	output		lcdc_rst_n;
	output		lcdc_cs_n;
	output		lcdc_rs;
	output		lcdc_wr_n;
	inout	[7:0]	lcdc_d;
	output		led_export;
	output		mmc_nCS;
	output		mmc_SCK;
	output		mmc_SDO;
	input		mmc_SDI;
	input		mmc_CD;
	input		mmc_WP;
	input		reset_reset_n;
	output	[11:0]	sdr_addr;
	output	[1:0]	sdr_ba;
	output		sdr_cas_n;
	output		sdr_cke;
	output		sdr_cs_n;
	inout	[15:0]	sdr_dq;
	output	[1:0]	sdr_dqm;
	output		sdr_ras_n;
	output		sdr_we_n;
	output		sound_export;
endmodule
