#include "Application.h"
#include "GUI_Properties.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_opengl2.h"
#include "imgui\imgui_impl_sdl.h"

#include "C_Transform.h"
#include "C_Mesh.h"
#include "C_Material.h"

#include "mmgr/mmgr.h"

GUI_Properties::GUI_Properties(SDL_Scancode shortcut) : GUI_Panel(shortcut, GUI_WINDOWS::GUI_PROPERTIES)
{}

void GUI_Properties::Draw()
{
	ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoFocusOnAppearing);

	if (App->scene->goSelected != nullptr)
	{
		for (int i = 0; i < App->scene->goSelected->components.size(); i++)
		{
			switch (App->scene->goSelected->components[i]->type)
			{
			case COMPONENT_TYPE::COMPONENT_TRANSFORM:
			{
				if (ImGui::CollapsingHeader("Transform"))
				{
					math::float3 tmpPos = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position;
					float auxPos[3] = { tmpPos.x, tmpPos.y, tmpPos.z };
					if (ImGui::InputFloat3("Position", auxPos))
					{
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->position = math::float3(auxPos[0], auxPos[1], auxPos[2]);
					}
					ImGui::Text("Rotation:\t - , - , -");

					math::float3 tmpScale = App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->scale;
					static float auxScale[3] = { tmpScale.x, tmpScale.y, tmpScale.z };
					if (ImGui::InputFloat3("Scale", auxScale))
					{
						App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_TRANSFORM)->GetComponentAsTransform()->scale = math::float3(auxScale[0], auxScale[1], auxScale[2]);
					}
				}
			}
			break;
			case COMPONENT_TYPE::COMPONENT_MESH:
			{
				if (ImGui::CollapsingHeader("Geometry"))
				{
					if (App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMesh() != nullptr)
					{
						ImGui::Text("Triangles:\t %i", App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MESH)->GetComponentAsMesh()->mesh->GetIndicesSize() / 3);
						ImGui::Text("Vertex:\t\t%i ", App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MESH)->GetComponentAsMesh()->mesh->GetVerticesSize() / 3);
					}
				}
			}
			break;
			case COMPONENT_TYPE::COMPONENT_MATERIAL:
			{
				if (ImGui::CollapsingHeader("Texture"))
				{
					if (App->scene->goSelected != nullptr)
					{
						if (App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial() != nullptr)
						{
							ImGui::Text("Image (witdth/ heigh): %i , %i", App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->texture->GetWidth(), App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->texture->GetHeigh());
							ImGui::Image((void*)(intptr_t)App->scene->goSelected->GetComponent(COMPONENT_TYPE::COMPONENT_MATERIAL)->GetComponentAsMaterial()->texture->GetBufferPos(), ImVec2(512, 512));
						}
					}
					else
					{
						ImGui::Text("Image (witdth/ heigh): - , -");
					}
				}
			}
			break;
			}
		}
	}
	ImGui::End();
}