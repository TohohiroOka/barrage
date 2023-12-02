#include "BaseOnStageDirection.h"
#include <imgui.h>

void BaseOnStageDirection::ImguiDraw()
{
	if (!camera) { return; }

	ImGui::Begin("debug imgui");

	ImGui::Text("Camera Pos    [ %f : %f : %f ]", camera->GetEyePos().x, camera->GetEyePos().y, camera->GetEyePos().z);
	ImGui::Text("Camera Target [ %f : %f : %f ]", camera->GetTargetPos().x, camera->GetTargetPos().y, camera->GetTargetPos().z);

	ImGui::End();
}
