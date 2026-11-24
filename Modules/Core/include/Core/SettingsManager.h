#pragma once
#include <UI/UI_Comp_Ptr.h>
#include <UI/SDF_Text.h>
#include <UI/ScrollBar.h>
#include <Audio/AudioGroupMixer.h>
#include <nlohmann/json.hpp>
#include <string>

class SettingsManager {
	struct Config {
		struct Screen {
			uint32_t width, height;
			uint8_t present_mode_code;
			bool fullscreen;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(Screen, width, height, present_mode_code, fullscreen)
		};
		struct Volume {
			uint32_t master, music, sfx;

			NLOHMANN_DEFINE_TYPE_INTRUSIVE(Volume, master, music, sfx)
		};
		Screen screen;
		Volume volume;
		std::string renderer;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, screen, volume, renderer)
	};
	friend class MainMenuManager;
public:
	static SettingsManager& Instance() {
		static SettingsManager menu;
		return menu;
	}
	bool load_config();
	void load_settings();
	void change_resolution();
	void toggle_fullscreen();
	void change_present_mode();
	void set_master_volume(float volume);
	void set_music_volume(float volume);
	void set_sfx_volume(float volume);
	float get_master_volume();
	float get_music_volume();
	float get_sfx_volume();
	bool is_fullscreen() const { return config.screen.fullscreen; }
	uint8_t get_present_mode_code() const { return config.screen.present_mode_code; }
	uint32_t get_res_index() const { return current_resolution_index; }
private:
	Engine::Audio::AudioGroupMixer mixer_ui;
	Engine::Audio::AudioGroupMixer mixer_music;
	Engine::Audio::AudioGroupMixer mixer_sfx;

	Config config;
	uint32_t current_resolution_index = 0;
};