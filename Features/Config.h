#pragma once

class CConfig
{
public:
	const bool		LoadPreset();
	const bool		SavePreset();

	const bool		Reset();

private:
	std::string		config_name = "config.json";
};

extern CConfig* Config;