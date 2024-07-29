#include "game/state.h"

State::State() {
	// should be overwritten when the player is created
	this->player = nullptr;
}

void State::SetPlayer(Player* player) {
	this->player = player;
}

Player* State::GetPlayer() {
	return this->player;
}

void State::AddMesh(Mesh* mesh) {
	objects.push_back(mesh);
}

std::vector<Mesh*> State::GetMeshes() {
	return objects;
}