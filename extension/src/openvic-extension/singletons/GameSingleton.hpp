#pragma once

#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/texture2d_array.hpp>

#include <openvic-simulation/GameManager.hpp>
#include <openvic-simulation/dataloader/Dataloader.hpp>

namespace OpenVic {

	class GameSingleton : public godot::Object {
		GDCLASS(GameSingleton, godot::Object)

		static inline GameSingleton* singleton = nullptr;

		GameManager PROPERTY_REF(game_manager);
		Dataloader PROPERTY(dataloader);

		godot::Vector2i image_subdivisions;
		godot::Ref<godot::Texture2DArray> province_shape_texture;
		godot::Ref<godot::Image> province_colour_image;
		godot::Ref<godot::ImageTexture> province_colour_texture;
		Mapmode::index_t mapmode_index = 0;
		godot::Ref<godot::Texture2DArray> terrain_texture;
		ordered_map<Country const*, ordered_map<godot::StringName, godot::Ref<godot::Image>>> flag_image_map;

		static godot::StringName const& _signal_gamestate_updated();
		static godot::StringName const& _signal_province_selected();
		static godot::StringName const& _signal_clock_state_changed();

		godot::Error _load_map_images();
		godot::Error _load_terrain_variants();
		godot::Error _load_flag_images();

		/* Generate the province_colour_texture from the current mapmode. */
		godot::Error _update_colour_image();
		void _on_gamestate_updated();
		void _on_clock_state_changed();

	protected:
		static void _bind_methods();

	public:
		static GameSingleton* get_singleton();

		GameSingleton();
		~GameSingleton();

		static void setup_logger();

		/* Load the game's defines in compatiblity mode from the filepath
		 * pointing to the defines folder. */
		godot::Error load_defines_compatibility_mode(godot::PackedStringArray const& file_paths);

		static godot::String search_for_game_path(godot::String const& hint_path = {});

		/* Post-load/restart game setup - reset the game to post-load state and load the specified bookmark. */
		godot::Error setup_game(int32_t bookmark_index);

		int32_t get_province_index_from_uv_coords(godot::Vector2 const& coords) const;

		int32_t get_map_width() const;
		int32_t get_map_height() const;
		godot::Vector2i get_map_dims() const;
		float get_map_aspect_ratio() const;

		/* The cosmetic terrain textures stored in a Texture2DArray. */
		godot::Ref<godot::Texture2DArray> get_terrain_texture() const;

		/* The flag image corresponding to the requested country / flag_type
		 * combination, or nullptr if no such flag can be found. */
		godot::Ref<godot::Image> get_flag_image(Country const* country, godot::StringName const& flag_type) const;

		/* Number of (vertical, horizontal) subdivisions the province shape image
		 * was split into when making the province_shape_texture to ensure no
		 * piece had a dimension greater than 16383. */
		godot::Vector2i get_province_shape_image_subdivisions() const;

		/* The map, encoded in RGB8 with RG representing province index and B representing terrain texture.
		 * To support a wider range of GPUs, the image is divided so that no piece has a dimension
		 * greater than 16383 and the pieces are stored in a Texture2DArray. */
		godot::Ref<godot::Texture2DArray> get_province_shape_texture() const;

		/* The base and stripe colours for each province. */
		godot::Ref<godot::ImageTexture> get_province_colour_texture() const;

		int32_t get_mapmode_count() const;
		godot::String get_mapmode_identifier(int32_t index) const;
		godot::Error set_mapmode(godot::String const& identifier);
		bool is_parchment_mapmode_allowed() const;
		int32_t get_selected_province_index() const;
		void set_selected_province(int32_t index);

		void try_tick();
	};
}
