// license:BSD-3-Clause
// copyright-holders:Tomasz Slanina,Pierpaolo Prazzoli

#include "cpu/mcs51/mcs51.h"
#include "machine/timer.h"
#include "sound/qs1000.h"
#include "screen.h"

class eolith_state : public driver_device
{
public:
	eolith_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_eepromoutport(*this, "EEPROMOUT")
		, m_maincpu(*this, "maincpu")
		, m_soundcpu(*this, "soundcpu")
		, m_screen(*this, "screen")
		, m_palette(*this, "palette")
		, m_qs1000(*this, "qs1000")
		, m_in0(*this, "IN0")
		, m_penxport(*this, "PEN_X_P%u", 1)
		, m_penyport(*this, "PEN_Y_P%u", 1)
		, m_sndbank(*this, "sound_bank")
	{
	}

	DECLARE_CUSTOM_INPUT_MEMBER(eolith_speedup_getvblank);
	DECLARE_CUSTOM_INPUT_MEMBER(stealsee_speedup_getvblank);

	DECLARE_READ32_MEMBER(eolith_custom_r);
	DECLARE_WRITE32_MEMBER(systemcontrol_w);
	template<int Player> DECLARE_READ32_MEMBER(hidctch3_pen_r);
	DECLARE_WRITE16_MEMBER(eolith_vram_w);
	DECLARE_READ16_MEMBER(eolith_vram_r);
	DECLARE_WRITE8_MEMBER(sound_p1_w);
	DECLARE_READ8_MEMBER(qs1000_p1_r);
	DECLARE_WRITE8_MEMBER(qs1000_p1_w);
	DECLARE_WRITE8_MEMBER(soundcpu_to_qs1000);

	DECLARE_DRIVER_INIT(eolith);
	DECLARE_DRIVER_INIT(landbrk);
	DECLARE_DRIVER_INIT(hidctch2);
	DECLARE_DRIVER_INIT(hidnc2k);
	DECLARE_DRIVER_INIT(landbrka);
	DECLARE_DRIVER_INIT(landbrkb);

	DECLARE_MACHINE_RESET(eolith);
	DECLARE_VIDEO_START(eolith);

	TIMER_DEVICE_CALLBACK_MEMBER(eolith_speedup);

	uint32_t screen_update_eolith(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	void ironfort(machine_config &config);
	void eolith50(machine_config &config);
	void eolith45(machine_config &config);
	void hidctch3(machine_config &config);
	void eolith_map(address_map &map);
	void hidctch3_map(address_map &map);
	void sound_io_map(address_map &map);
	void sound_prg_map(address_map &map);
protected:
	// shared with eolith16.cpp, vegaeo.cpp
	optional_ioport m_eepromoutport;

	void speedup_read();
	void init_speedup();
	void patch_mcu_protection(uint32_t address);

private:
	required_device<cpu_device> m_maincpu;
	optional_device<i8032_device> m_soundcpu;
	required_device<screen_device> m_screen;
	required_device<palette_device> m_palette;
	optional_device<qs1000_device> m_qs1000;

	optional_ioport m_in0; // klondkp doesn't have it
	optional_ioport_array<2> m_penxport;
	optional_ioport_array<2> m_penyport;

	optional_memory_bank m_sndbank;

	int m_coin_counter_bit;
	std::unique_ptr<uint16_t[]> m_vram;
	int m_buffer;

	// speedups - see machine/eolithsp.c
	int m_speedup_address;
	int m_speedup_address2;
	int m_speedup_resume_scanline;
	int m_speedup_vblank;
	int m_speedup_scanline;
};
