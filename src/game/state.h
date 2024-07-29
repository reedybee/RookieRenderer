#pragma once
#include <vector>
#include "gfx/mesh.h"
#include "player/player.h"

struct State {
public:
	State();

	Player* GetPlayer();
	void SetPlayer(Player*);

	void AddMesh(Mesh* mesh);
	std::vector<Mesh*> GetMeshes();
private:
	std::vector<Mesh*> objects;
	Player* player;
};

extern State state;