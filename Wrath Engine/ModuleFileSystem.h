#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include <vector>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops* rw);

typedef unsigned int uint;
struct aiFileIO;

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(Application* app, bool start_enabled = true);

	~ModuleFileSystem();

	static bool AddPath(const char* path_or_zip);
	static bool Exists(const char* file);
	static bool IsDirectory(const char* file);
	static void CreateDirectory(const char* directory);
	static void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list);
	static bool CopyFromOutsideFS(const char* full_path, const char* destination);
	static bool Copy(const char* source, const char* destination);
	static void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr);
	static void NormalizePath(char* full_path);
	static void NormalizePath(std::string& full_path);

	uint Load(const char* path, const char* file, char** buffer) const;
	uint Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;
	void* BassLoad(const char* file) const;

	aiFileIO* GetAssimpIO();

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	bool Remove(const char* file);

	const char* GetBasePath() const;
	const char* GetWritePath() const;
	const char* GetReadPaths() const;

private:
	void CreateAssimpIO();

private:
	aiFileIO* AssimpIO = nullptr;
};

#endif 
