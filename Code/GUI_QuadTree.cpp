#include "Application.h"
#include "GUI_QuadTree.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

GUI_QuadTree::GUI_QuadTree(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_QUADTREE)
{}

void GUI_QuadTree::Draw()
{
	ImGui::Begin("QuadTree", &active, ImGuiWindowFlags_NoFocusOnAppearing);
	if (ImGui::Button("Generate QuadTree"))
	{
		App->scene->quadTree->GenerateQuadTree();
	}
	
	ImGui::InputInt("Max. Division", &App->scene->quadTree->maxDivisions);
	ImGui::InputInt("Bucket Size", &App->scene->quadTree->bucketSize);
	if (App->scene->quadTree->maxDivisions <= 0)
		App->scene->quadTree->maxDivisions = 0;
	if (App->scene->quadTree->bucketSize <= 0)
		App->scene->quadTree->bucketSize = 0;

	ImGui::Text("Static Game Objects: %i", App->scene->quadTree->static_go_list.size());

	ImGui::End();
}