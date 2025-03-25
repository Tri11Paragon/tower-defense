#include <imgui.h>
#include <blt/gfx/window.h>
#include "blt/gfx/renderer/batch_2d_renderer.h"
#include "blt/gfx/renderer/camera.h"
#include "blt/gfx/renderer/resource_manager.h"
#include <map.h>

#include <blt/math/aabb.h>

blt::gfx::matrix_state_manager global_matrices;
blt::gfx::resource_manager resources;
blt::gfx::batch_renderer_2d renderer_2d(resources, global_matrices);
blt::gfx::first_person_camera camera;

float t = 0;
float dir = 1;

blt::gfx::curve2d_t curve{blt::vec2{250, 250}, blt::vec2{350, 500}, blt::vec2{650, 500}, blt::vec2{750, 250}};
blt::gfx::curve2d_t curve2{blt::vec2{250, 250}, blt::vec2{500, 500}, blt::vec2{750, 250}};
blt::gfx::curve2d_mesh_data_t mesh;
blt::gfx::curve2d_mesh_data_t mesh2;

using curve_t = blt::gfx::curve2d_t;
using vec2 = blt::vec2f;
curve_t c1{{0, 100}, {200, 100}};
curve_t c2{{200, 100}, {300, 100}, {300, 300}};
curve_t c3{{300, 300}, {300, 400}, {400, 400}};
curve_t c4{{400, 400}, {500, 400}, {500, 500}};

td::enemy_database_t database;
td::map_t map{std::vector{td::path_segment_t{c1}, td::path_segment_t{c2}, td::path_segment_t{c3}, td::path_segment_t{c4}}, database};

void init(const blt::gfx::window_data&)
{
	blt::gfx::setWindowSize(1440, 720);
	using namespace blt::gfx;

	resources.setPrefixDirectory("../");
	BLT_INFO("Loading Resources");
	resources.enqueue("res/enemy.png", "no_enemy_texture");
	resources.enqueue("res/particle.png", "particle");
	resources.enqueue("res/tower.png", "tower");

	global_matrices.create_internals();
	resources.load_resources();
	renderer_2d.create();
	mesh = curve.to_mesh(32);
	mesh2 = curve2.to_mesh(32);
}

void update(const blt::gfx::window_data& data)
{
	global_matrices.update_perspectives(data.width, data.height, 90, 0.1, 2000);

	camera.update();
	camera.update_view(global_matrices);
	global_matrices.update();

	map.update();
	map.draw(renderer_2d);

	t += 0.01f * dir;
	if (t >= 1)
	{
		t = 1;
		dir = -1;
		map.spawn(td::enemy_id_t::TEST);
	} else if (t <= 0)
	{
		t = 0;
		dir = 1;
	}

	auto pos = curve.get_point(t);
	renderer_2d.drawRectangle(blt::gfx::rectangle2d_t{pos, blt::vec2{25, 25}}, blt::make_color(1, 0, 0));
	renderer_2d.drawCurve(mesh, blt::make_color(0, 1, 0));
	renderer_2d.drawCurve(mesh2, blt::make_color(0, 0, 1));
	// auto lines = curve.to_lines(32);
	// for (const auto& line : lines)
	// renderer_2d.drawLineInternal(blt::make_color(0, 1,0), line);

	renderer_2d.render(data.width, data.height);
}

void destroy(const blt::gfx::window_data&)
{
	global_matrices.cleanup();
	resources.cleanup();
	renderer_2d.cleanup();
	blt::gfx::cleanup();
}

int main()
{
	blt::gfx::init(blt::gfx::window_data{"My Sexy Window", init, update, destroy}.setSyncInterval(1));
}
