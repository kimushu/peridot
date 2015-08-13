----------------------------------------------------------------------
-- TITLE : 8bitPCM Sound (PCM Playback)
--
--     VERFASSER : S.OSAFUNE (J-7SYSTEM Works)
--     DATUM     : 2009/07/07 -> 2009/07/08 (HERSTELLUNG)
--               : 2009/07/27 (FESTSTELLUNG)
----------------------------------------------------------------------

-- +0 STATUS  bit7:IRQENA bit6:IRQ  bit0:FIFORST
-- +1 VOLUME  bit7:MUTE  bit6-0:VOL
-- +2 FS8DIV  bit7-0:DIVNUM
-- +3 FIFOWR  bit7-0:WRDATA

-- STATUSレジスタ 
--      IRQENA : 0 IRQリクエストをマスク 
--               1 IRQリクエストを許可 
--
--      IRQ    : 0 FIFOが空いていない 
--               1 FIFOハーフエンプティ(256バイト以上の空きがある)
--
--      FIFORST: 0 FIFO動作をする 
--               1 FIFOをリセットする 
--
-- VOLUMEレジスタ 
--      MUTE   : 0 MUTEをオフ 
--               1 出力ミューティング 
--
--      VOL    : 0-127 出力音量(0:最小 - 127:最大)
--
-- FS8DIVレジスタ 
--      DIVNUM : システムクロック÷((DIVNUM+1)+128)がfs8タイミングとして使われる 
--               73.728MHz時、各fsの値は下記の通り 
--                 fs=48kHz   : 63
--                 fs=44.1kHz : 79
--                 fs=32kHz   : 159
--                 fs=24kHz   : 255
--
-- FIFOWRレジスタ 
--      WRDATA : このレジスタに書き込むとPCMデータがFIFOにキューイングされる 
--               読み出し値は不定 


library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity avalonif_sound is
	port(
		csi_global_reset	: in  std_logic;
		csi_global_clk		: in  std_logic;

		----- Avalonバス信号(メモリスレーブ) -----------
		avs_s1_address		: in  std_logic_vector(2 downto 1);
		avs_s1_read			: in  std_logic;
		avs_s1_readdata		: out std_logic_vector(7 downto 0);
		avs_s1_write		: in  std_logic;
		avs_s1_writedata	: in  std_logic_vector(7 downto 0);

		avs_s1_irq			: out std_logic;

		----- オーディオ信号 -----------
		dac_out				: out std_logic
	);
end avalonif_sound;

architecture RTL of avalonif_sound is
	constant CLOCK_EDGE		: std_logic := '1';
	constant RESET_LEVEL	: std_logic := '1';
	signal divcount			: std_logic_vector(8 downto 0);
	signal fsdiv_count		: std_logic_vector(2 downto 0);
	signal fs8_timing_sig	: std_logic;
	signal fs_timing_sig	: std_logic;

	signal irqena_reg		: std_logic;
	signal mute_reg			: std_logic;
	signal vol_reg			: std_logic_vector(6 downto 0);
	signal fiforeset_reg	: std_logic;
	signal fs8div_reg		: std_logic_vector(7 downto 0);
	signal countdiv_sig		: std_logic_vector(8 downto 0);

	signal mul_a_reg		: std_logic_vector(6 downto 0);
	signal muladd_a_sig		: std_logic_vector(8 downto 0);
	signal muladd_b_sig		: std_logic_vector(8 downto 0);
	signal muladd_q_sig		: std_logic_vector(8 downto 0);
	signal muladd_reg		: std_logic_vector(14 downto 0);


	constant DACBITWIDTH	: integer := 12;
--	signal dacdivcount		: std_logic_vector(3 downto 0);
	signal dacdivcount		: std_logic_vector(1 downto 0);
	signal pcmout_sig		: std_logic_vector(DACBITWIDTH-1 downto 0);
	signal ovsin_reg		: std_logic_vector(DACBITWIDTH-1 downto 0);
	signal delta_reg		: std_logic_vector(DACBITWIDTH-1+1 downto 0);
	signal ovspcm_reg		: std_logic_vector(DACBITWIDTH-1+3 downto 0);
	signal pcmin_sig		: std_logic_vector(DACBITWIDTH-1 downto 0);

	signal din_sig			: std_logic_vector(DACBITWIDTH-1 downto 0);
	signal add_sig			: std_logic_vector(DACBITWIDTH-1+1 downto 0);
	signal z_reg			: std_logic_vector(DACBITWIDTH-1 downto 0);
	signal dout_reg			: std_logic;

--	signal dsmin_sig		: std_logic_vector(DACBITWIDTH downto 0);
--	signal dsmz1_reg		: std_logic_vector(DACBITWIDTH-3 downto 0);
--	signal dsmz2_reg		: std_logic_vector(DACBITWIDTH-3 downto 0);
--	signal dsmz3_reg		: std_logic_vector(2 downto 0);
--	signal dsme_reg			: std_logic_vector(2 downto 0);
--	signal dsmadd1_sig		: std_logic_vector(DACBITWIDTH downto 0);
--	signal dsmadd2_sig		: std_logic_vector(3 downto 0);
--	signal dsmout_reg		: std_logic;


	component sound_fifo
	PORT
	(
		aclr		: IN STD_LOGIC ;
		clock		: IN STD_LOGIC ;
		data		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		rdreq		: IN STD_LOGIC ;
		wrreq		: IN STD_LOGIC ;
		empty		: OUT STD_LOGIC ;
		full		: OUT STD_LOGIC ;
		q		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
		usedw		: OUT STD_LOGIC_VECTOR (8 DOWNTO 0)
	);
	end component;
	signal fifowrdata_sig	: std_logic_vector(7 downto 0);
	signal fifowrreq_sig	: std_logic;
	signal fifoempty_sig	: std_logic;
	signal fifofull_sig		: std_logic;
	signal fifousedw_sig	: std_logic_vector(8 downto 0);
	signal fifoq_sig		: std_logic_vector(7 downto 0);
	signal wavedata_sig		: std_logic_vector(fifoq_sig'length downto 0);
	signal fifoirq_sig		: std_logic;


begin

--==== タイミング信号生成 ===========================================

	-- fsタイミング信号を生成 

	process (csi_global_clk, csi_global_reset) begin
		if (csi_global_reset = '1') then
			divcount    <= (others=>'0');
			fsdiv_count <= (others=>'0');

		elsif(csi_global_clk'event and csi_global_clk='1') then
			if (divcount = countdiv_sig) then
				divcount    <= (others=>'0');
				fsdiv_count <= fsdiv_count + 1;
			else
				divcount <= divcount + 1;
			end if;

		end if;
	end process;

	fs8_timing_sig <= '1' when(divcount = 0) else '0';
	fs_timing_sig  <= '1' when(divcount = 0 and fsdiv_count = 0) else '0';



--==== レジスタおよびバスインターフェース ===========================


	with avs_s1_address select avs_s1_readdata <=
		irqena_reg & fifoirq_sig & "00000" & fiforeset_reg when "00",
		mute_reg & vol_reg	when "01",
		fs8div_reg			when "10",
		(others=>'X')		when others;

	avs_s1_irq <= '1' when(irqena_reg = '1' and fifoirq_sig='1') else '0';

	countdiv_sig <= ('0' & fs8div_reg) + 128;
--	countdiv_sig <= ('0' & fs8div_reg) + 1;			-- test

	process (csi_global_clk, csi_global_reset) begin
		if (csi_global_reset = '1') then
			irqena_reg   <= '0';
			fiforeset_reg<= '1';
			mute_reg     <= '1';
			vol_reg      <= (others=>'0');
			fs8div_reg   <= (others=>'0');

		elsif(csi_global_clk'event and csi_global_clk='1') then
			if (avs_s1_write = '1') then
				case avs_s1_address is
				when "00" =>
					irqena_reg   <= avs_s1_writedata(7);
					fiforeset_reg<= avs_s1_writedata(0);
				when "01" =>
					mute_reg <= avs_s1_writedata(7);
					vol_reg  <= avs_s1_writedata(6 downto 0);
				when "10" =>
					fs8div_reg <= avs_s1_writedata;
				when others=>
				end case;
			end if;

		end if;
	end process;

	fifowrdata_sig <= avs_s1_writedata;
	fifowrreq_sig  <= '1' when(avs_s1_write = '1' and avs_s1_address="11") else '0';



--==== 波形読み出しと音量変更 =======================================

	-- 波形FIFO 

	U_FIFO : sound_fifo PORT MAP (
		aclr	=> fiforeset_reg,
		clock	=> csi_global_clk,
		data	=> fifowrdata_sig,
		rdreq	=> fs_timing_sig,
		wrreq	=> fifowrreq_sig,
		empty	=> fifoempty_sig,
		full	=> fifofull_sig,
		q	 	=> fifoq_sig,
		usedw	=> fifousedw_sig
	);

	fifoirq_sig <= '1' when(fifofull_sig = '0' and fifousedw_sig(fifousedw_sig'left) = '0') else '0';


	-- 音量変更(符号なし7bit×符号付き8bit→符号付き15bit)

	wavedata_sig(wavedata_sig'left downto wavedata_sig'left-1) <= (others=>(not fifoq_sig(fifoq_sig'left)));
	wavedata_sig(wavedata_sig'left-2 downto 0) <= fifoq_sig(fifoq_sig'left-1 downto 0);

	muladd_a_sig <= muladd_reg(muladd_reg'left) & muladd_reg(muladd_reg'left downto muladd_reg'left-(muladd_a_sig'length-2));
	muladd_b_sig <= wavedata_sig when(mul_a_reg(0) = '1' and fifoempty_sig = '0') else (others=>'0');
	muladd_q_sig <= muladd_a_sig + muladd_b_sig;

	process (csi_global_clk, csi_global_reset) begin
		if (csi_global_reset = '1') then
			mul_a_reg  <= (others=>'0');
			muladd_reg <= (others=>'0');

		elsif(csi_global_clk'event and csi_global_clk = '1') then
			if (fs8_timing_sig = '1') then
				if (fs_timing_sig = '1') then
					mul_a_reg  <= vol_reg;
					muladd_reg <= (others=>'0');
				else
					mul_a_reg  <= '0' & mul_a_reg(mul_a_reg'left downto 1);
					muladd_reg(muladd_reg'left downto muladd_reg'left-(muladd_q_sig'length-1)) <= muladd_q_sig;
					muladd_reg(muladd_reg'left-muladd_q_sig'length downto 0) <= muladd_reg(muladd_reg'left-(muladd_q_sig'length-1) downto 1);
				end if;
			end if;

		end if;
	end process;

	pcmout_sig <= muladd_reg(muladd_reg'left downto muladd_reg'left-(pcmout_sig'length-1));


	-- 線形８倍オーバーサンプリング 

	process (csi_global_clk, csi_global_reset) begin
		if (csi_global_reset = '1') then
			delta_reg  <= (others=>'0');
			ovspcm_reg <= (others=>'0');
			ovsin_reg  <= (others=>'0');

		elsif (csi_global_clk'event and csi_global_clk = '1') then
			if (fs8_timing_sig = '1') then
				if (fs_timing_sig = '1') then
					delta_reg  <= (pcmout_sig(pcmout_sig'left) & pcmout_sig) - (ovsin_reg(ovsin_reg'left) & ovsin_reg);
					ovspcm_reg <= ovsin_reg & "000";
					ovsin_reg  <= pcmout_sig;
				else
					ovspcm_reg <= ovspcm_reg + (delta_reg(delta_reg'left) & delta_reg(delta_reg'left) & delta_reg);
				end if;
			end if;

		end if;
	end process;

	pcmin_sig <= (others=>'0') when(mute_reg = '1') else ovspcm_reg(ovspcm_reg'left downto 3);


	-- １次⊿∑変調 (入力bit→1bit) 

	din_sig(din_sig'left) <= not pcmin_sig(pcmin_sig'left);
	din_sig(din_sig'left-1 downto 0) <= pcmin_sig(pcmin_sig'left-1 downto 0);

	add_sig <= ('0' & din_sig) + ('0' & z_reg);

	process(csi_global_clk, csi_global_reset)begin
		if (csi_global_reset = '1') then
			dacdivcount <= (others=>'0');
			z_reg       <= (others=>'0');
			dout_reg    <= '0';

		elsif (csi_global_clk'event and csi_global_clk = '1') then
			dacdivcount <= dacdivcount + 1;

			if (dacdivcount = 0) then
				z_reg    <= add_sig(add_sig'left-1 downto 0);
				dout_reg <= add_sig(add_sig'left);
			end if;

		end if;
	end process;

	dac_out <= dout_reg;


	-- ２次⊿∑変調 (入力bit→3bit,3bit→1bit) 

--	dsmin_sig(dsmin_sig'left) <= not pcmin_sig(pcmin_sig'left);
--	dsmin_sig(dsmin_sig'left-1 downto 0) <= pcmin_sig;
--
--	dsmadd1_sig <= dsmin_sig + ("00" & dsmz1_reg & '0') - ("000" & dsmz2_reg);
--	dsmadd2_sig <= ('0' & dsmz3_reg) + ('0' & dsme_reg);
--
--	process (csi_global_clk, csi_global_reset) begin
--		if (csi_global_reset = '1') then
--			dacdivcount<= (others=>'0');
--			dsmz1_reg  <= (others=>'0');
--			dsmz2_reg  <= (others=>'0');
--			dsmz3_reg  <= (dsmz3_reg'left=>'1',others=>'0');
--
--			dsme_reg   <= (others=>'0');
--			dsmout_reg <= '0';
--
--		elsif (csi_global_clk'event and csi_global_clk = '1') then
--			dacdivcount <= dacdivcount + 1;
--
--			if (dacdivcount = 0) then
--				dsmz1_reg  <= dsmadd1_sig(dsmadd1_sig'left-3 downto 0);
--				dsmz2_reg  <= dsmz1_reg;
--				dsmz3_reg  <= dsmadd1_sig(dsmadd1_sig'left downto(dsmadd1_sig'left-3 + 1));
--
--				dsme_reg   <= dsmadd2_sig(2 downto 0);
--				dsmout_reg <= dsmadd2_sig(3);
--			end if;
--
--		end if;
--	end process;
--
--	dac_out <= dsmout_reg;


end RTL;



----------------------------------------------------------------------
--     (C)2009 Copyright J-7SYSTEM Works.  All rights Reserved.     --
----------------------------------------------------------------------
