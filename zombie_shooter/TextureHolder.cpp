#include "TextureHolder.h"

// Include the "assert feature"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	// Get a reference to m_Textures using m_s_Instance
	// auto is equivalent of map<string, Texture>
	auto& m = m_s_Instance->m_Textures;

	// Create an iterator to hold a key-value pair (kvp) and search for the required kvp using the passed in file name
	// auto is equivalent of map<stgring, Texture>::iterator
	auto keyValuePair = m.find(filename);

	// Did we find a match?
	if(keyValuePair != m.end())
		// Return the texture, the second part of the kvp
		return keyValuePair->second;
	else
	{
		// File name not found
		// Create a new key value pair using the filename
		auto& texture = m[filename];

		// Load the texture from file in the usual way
		texture.loadFromFile(filename);

		// Return the texture to the calling code
		return texture;
	}
}
