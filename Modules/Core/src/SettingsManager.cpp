#include "Core/SettingsManager.h"
#include <DeviceSystem/DeviceContext.h>
#include <Audio/AudioSystem.h>
#include <Utility/Logger.h>
#include <fstream>

bool SettingsManager::load_config() {
	std::filesystem::path settings_file_path = "Config/game_config.json";
	if (!std::filesystem::exists(settings_file_path)) {
		Engine::Util::Logger::Log("Couldn't find config file!");
		return false;
	}

	std::ifstream config_read(settings_file_path);
	if (!config_read.is_open()) {
		Engine::Util::Logger::Log("Couldn't load config file!");
		return false;
	}
	nlohmann::json j;
	config_read >> j;

	config = j.get<Config>();

	if (config.renderer == "Vulkan") {
		DeviceContext::graphics_API = DeviceContext::GraphicsAPI::VULKAN;
	}
	else if (config.renderer == "DirectX12") {
		DeviceContext::graphics_API = DeviceContext::GraphicsAPI::DIRECTX;
	}
	else {
		DeviceContext::graphics_API = DeviceContext::GraphicsAPI::OPEN_GL;
	}

	return true;
}

void SettingsManager::load_settings() {
	mixer_ui.setup_mixer("Core:UI");
	mixer_music.setup_mixer("Core:Music");
	mixer_sfx.setup_mixer("Core:SFX");

	DeviceContext::display.toggle_fullscreen(config.screen.fullscreen);
	DeviceContext::display.present_mode = static_cast<DisplayController::DisplayPresentMode>(config.screen.present_mode_code);
	DeviceContext::display.change_resolution(config.screen.width, config.screen.height);

	set_master_volume((float)config.volume.master / 100.0f);
	set_music_volume((float)config.volume.music / 100.0f);
	set_sfx_volume((float)config.volume.sfx / 100.0f);
}

void SettingsManager::change_resolution() {
	DeviceContext::display.change_resolution(true);
}

void SettingsManager::toggle_fullscreen() {
	config.screen.fullscreen = !config.screen.fullscreen;
	DeviceContext::display.toggle_fullscreen(config.screen.fullscreen);
}

void SettingsManager::change_present_mode() {
	DeviceContext::display.change_present_mode();
	config.screen.present_mode_code = DeviceContext::display.get_present_mode_code();
}

void SettingsManager::set_master_volume(float volume) {
	Engine::Audio::AudioSystem::Instance().set_master_volume(volume);
}

void SettingsManager::set_music_volume(float volume) {
	mixer_music.set_volume(volume);
}

void SettingsManager::set_sfx_volume(float volume) {
	mixer_sfx.set_volume(volume);
}

float SettingsManager::get_master_volume() {
	return Engine::Audio::AudioSystem::Instance().get_master_volume();
}

float SettingsManager::get_music_volume() {
	return mixer_music.get_volume();
}

float SettingsManager::get_sfx_volume() {
	return mixer_sfx.get_volume();
}